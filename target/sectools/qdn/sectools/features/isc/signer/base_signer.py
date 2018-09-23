# ===============================================================================
#
#  Copyright (c) 2013-2017 Qualcomm Technologies, Inc.
#  All Rights Reserved.
#  Confidential and Proprietary - Qualcomm Technologies, Inc.
#
# ===============================================================================

import re
from abc import ABCMeta

from sectools.common import crypto
from sectools.common.utils import c_path
from sectools.common.utils.c_logging import logger
from sectools.common.utils.c_misc import create_mismatch_table
from sectools.common.core.plugin import SecPluginIntf_Abs
from sectools.features.isc import data_provision_enquirer as openssl_fetch_module
from sectools.features.isc.defines import AUTHORITY_OEM
from sectools.features.isc.hasher import Hasher
from sectools.features.isc.hasher import HmacParams
from sectools.features.isc.signer import signer_mgr
from sectools.features.isc.signer.utils.hmac import HMAC
from sectools.features.isc.signer.utils.certkey import CertKeyPair
from sectools.features.isc.signer.signerutils import get_sw_id
from sectools.features.isc.signer.signerutils import get_msm_id
from sectools.features.isc.signer.signerutils import ExtractedAttributes
from sectools.features.isc.signer.signerutils.attributes_base import SigningAttributesBase
from sectools.features.isc.signer.signerutils.certificate_base import CertificateBase
from sectools.features.isc.signer.signerutils.certconfigparser import get_cert_data
from sectools.features.isc.signer.signerutils.attribute_extractor_base import\
    AttributeExtractorBase

from .signer_output import SignerOutput
from .signer_errors import ConfigError


def ishmac(imageinfo):
    return not imageinfo \
           or not imageinfo.general_properties \
           or imageinfo.general_properties.hmac is None \
           or imageinfo.general_properties.hmac


class BaseSigner(SecPluginIntf_Abs):
    __metaclass__ = ABCMeta

    DEBUG_DISABLED = 2

    # Error message strings
    MSG_INVALID_32_INTEGER = '{0} {1}:{2} is not a valid 32 bit integer'

    # Certificate properties
    CERT_DAYS = 7300
    CERT_SERIAL = 1

    # Padding types
    PAD_PSS = crypto.cert.SIGN_ALGO_RSA_PSS
    PAD_PKCS = crypto.cert.SIGN_ALGO_RSA_PKCS

    # PSS Fixes properties
    PAD_PSS_SALT_1 = crypto.cert.PAD_PSS_SALT_1

    # OU hash field strings
    SHA1_OU_STRING = '07 0000 SHA1'
    SHA256_OU_STRING = '07 0001 SHA256'
    SHA384_OU_STRING = '07 0002 SHA384'

    OU_SHA_MAP = {
        SHA1_OU_STRING: "sha1",
        SHA256_OU_STRING: "sha256",
        SHA384_OU_STRING: "sha384",
    }

    SHA_OU_MAP = dict([(algo, ou) for ou, algo in OU_SHA_MAP.items()])
    SHA_OU_MAP[None] = SHA256_OU_STRING

    # Certificate types
    ROOT = 'Root'
    CA = 'CA'
    ATTEST = 'Attest'
    CERT_LIST = [ROOT, CA, ATTEST]

    signing_attributes_class = SigningAttributesBase

    def __init__(self, config=None):
        # Config info
        self.config = config
        self.openssl_path = crypto.cert.openssl

        # Fetch the openssl info
        self.openssl_info = None
        try:
            self.openssl_info = openssl_fetch_module.OpenSSLPathsObject(config)
        except Exception as e:
            logger.debug('Error on fetching openssl info: ' + str(e))

        # Certificates properties for data prov
        self.certs_info = None
        self.padding = self.PAD_PKCS

        # Public variables
        self.debug_dir = None
        self.signing_attributes = None

        # Input data
        self.hash_to_sign = None
        self.data_to_sign = None
        self.data_to_sign_len = None

        # Certificates & signature
        self.certs = {}
        self.signature = None

        # Configure dsa mode
        self.using_ecdsa = self.config.general_properties.dsa_type == "ecdsa"

    @classmethod
    def get_plugin_id(cls):
        return cls.signer_id()

    @classmethod
    def get_plugin_manager(cls):
        return signer_mgr

    @classmethod
    def is_prod_signer(cls):
        return False

    @classmethod
    def is_plugin(cls):
        return True

    @classmethod
    def signer_id(cls):
        return 'base'

    def set_input(self, data_to_sign, imageinfo, debug_dir=None, is_hash=False):
        sa = self.set_input_impl(data_to_sign, imageinfo, debug_dir=debug_dir, is_hash=is_hash)
        if self.using_ecdsa:
            logger.info("Using ECDSA with {0} curve".format(sa.ecdsa_curve))
        else:
            logger.info('Using ' + self.padding.upper() + ' RSA padding')

    def set_input_impl(self, data_to_sign, imageinfo, debug_dir=None, is_hash=False):
        """Set the input.
        :type data_to_sign: str
        :type imageinfo: ImageInfo
        """

        # Validate the image info
        self.validate_config(imageinfo)

        # Set public variables for this session
        self.debug_dir = debug_dir
        sa = self.signing_attributes = imageinfo.general_properties

        # Set the certificates dictionary
        self.certs = dict()
        self.certs[self.ROOT] = CertKeyPair()
        self.certs[self.ATTEST] = CertKeyPair()
        if sa.num_certs_in_certchain == 3:
            self.certs[self.CA] = CertKeyPair()

        # Set the data to sign and the hash to sign
        if is_hash:
            self.hash_to_sign = data_to_sign
            self.data_to_sign = None
        else:
            hasher = HMAC()
            hasher.init_from_config(sa)
            self.hash_to_sign = hasher.hmac(data_to_sign)
            self.data_to_sign = data_to_sign
            self.data_to_sign_len = len(data_to_sign)

        self.padding = (self.PAD_PSS if (sa.rsa_padding and sa.rsa_padding.lower() == 'pss') else self.PAD_PKCS)
        self.hash_algo = sa.hash_algorithm if sa.hash_algorithm is not None else 'sha256'

        # Initialize the secure assets
        self.initialize(imageinfo)

        return sa

    def initialize(self, imageinfo):
        """Initialize the secure assets.

        Uses data prov to set secure assets.

        :type imageinfo: ImageInfo
        """
        # Generate cert info object
        cert_data = get_cert_data(imageinfo, self.config.data_provisioning.base_path)
        self.certs_info = cert_data.get_certs_info(self.signing_attributes.num_root_certs)

        # Certificate info object contains these parameters:
        #   1. root certificate path in DEM format
        #   2. private key path
        # For IoT, there should be only one (1) root certificate.
        crypto_params = imageinfo.crypto_params if hasattr(imageinfo, "crypto_params") else {}
        self.certs_info = cert_data.get_certs_info(
            self.signing_attributes.num_root_certs, crypto_params=crypto_params)

        # Get the root cert from data prov
        root_obj = self.certs[self.ROOT]
        root_obj.params = self.certs_info.root  # an instance of CertProperty
        root_obj.cert, root_obj.priv_key, root_obj.pub_key = \
            self._get_cert_key_from_property(root_obj.params, self.ROOT)

        # Get the CA cert from data prov
        if self.CA in self.certs:
            ca_obj = self.certs[self.CA]
            ca_obj.params = self.certs_info.ca
            ca_obj.cert, ca_obj.priv_key, ca_obj.pub_key = \
                self._get_cert_key_from_property(ca_obj.params, self.CA)

        # Get the attest cert from data prov
        attest_obj = self.certs[self.ATTEST]
        attest_obj.params = self.certs_info.attest
        attest_obj.cert, attest_obj.priv_key, attest_obj.pub_key = \
            self._get_cert_key_from_property(attest_obj.params, self.ATTEST)

    def _attribute_extractor(self, cert_data=None, attributes=None, **kwargs):
        return AttributeExtractorBase(cert_data=cert_data, attributes=attributes)

    def _print_attestation_cert_props(self, attestation_cert, parsegen=None):
        logger.info('\nAttestation Certificate Properties:\n' +
                    str(CertificateBase(attestation_cert)))

    def sign(self, data_to_sign, imageinfo, debug_dir=None, is_hash=False, **kwargs):
        """
        This function returns a SignerOutput object which has all the security assets generated
        by the signer.
        """
        # Set the input information
        self.set_input(data_to_sign, imageinfo, debug_dir=debug_dir, is_hash=is_hash)

        # Set the certificates and keys for output
        signer_output = SignerOutput()
        signer_output.root_cert, signer_output.root_key = self.get_root_cert_key()
        if self.CA in self.certs:
            signer_output.attestation_ca_cert, signer_output.attestation_ca_key = self.get_ca_cert_key()
        signer_output.attestation_cert, signer_output.attestation_key = self.get_attest_cert_key()

        # Set the root certs for MRC
        signer_output.root_cert_list = self.get_root_cert_list()

        # Get the hmac params from attestation cert or hash segment
        extracted_image_attributes = self._attribute_extractor(
            cert_data=signer_output.attestation_cert,
            attributes=self.signing_attributes_class(), **kwargs).attributes
        hmac_from_image = HMAC()
        hmac_from_image.init_from_image_attributes(extracted_image_attributes, self.signing_attributes)

        # Get the hmac params from config
        hmac_from_config = HMAC()
        hmac_from_config.init_from_config(self.signing_attributes)

        # Recreate the hash to sign if necessary
        if hmac_from_config.hmac_type == hmac_from_config.HMAC_TYPE_QTI and not hmac_from_image.is_equal(hmac_from_config):
            if self.data_to_sign is not None:
                self.hash_to_sign = hmac_from_image.hmac(self.data_to_sign)
            else:
                raise RuntimeError('HMAC params from image cannot be used with pre-generated hash.')

        # Set the signature
        signer_output.signature = self.get_signature()
        signer_output.unsigned_hash = self.hash_to_sign

        # Update the certs
        signer_output.update_certs_format()

        # Set the cert chain
        signer_output.generate_cert_chain()

        self._print_attestation_cert_props(signer_output.attestation_cert, **kwargs)

        return signer_output

    @classmethod
    def get_hmac_params_from_cert(cls, certificate=None, extracted_attributes=None):
        """ Return a dictionary of the HMAC params from the certificate subject dictionary
        input:
            certificate_subject_dictionary = dictionary of subject params from certificate

        output:
            hmac_params = Dictionary of HMAC parameters from certificate subject
        """

        if certificate:
            certificate_subject_dictionary = crypto.cert.get_subject_params(crypto.cert.get_subject(certificate))
            sw_id_re = re.compile(r'01 ([0-9A-F]{16}) SW_ID')
            hw_id_re = re.compile(r'02 ([0-9A-F]{16}) HW_ID')
            if 'OU' in certificate_subject_dictionary.keys() and type(certificate_subject_dictionary['OU']) == list:
                certificate_subject_dictionary['OU'].sort()
            sw_id_element = sw_id_re.match(certificate_subject_dictionary['OU'][0])
            hw_id_element = hw_id_re.match(certificate_subject_dictionary['OU'][1])
            logger.debug("Valid certificate: Found SW_ID and HW_ID")
            sw_id = sw_id_element.group(1) if sw_id_element is not None else None
            hw_id = hw_id_element.group(1) if hw_id_element is not None else None
        elif extracted_attributes:
            if extracted_attributes.from_hash_segment:
                hmac_params = cls.get_hmac_params_from_extracted_attributes(extracted_attributes)
                sw_id = hmac_params.sw_id_str
                hw_id = hmac_params.msm_id_str
            else:
                sw_id = extracted_attributes.sw_id
                hw_id = extracted_attributes.hw_id
        else:
            logger.critical("certificate and extracted_attributes cannot both be none")
            raise RuntimeError("certificate and attribute_extractor cannot both be none")

        if sw_id is None:
            logger.critical("Error in image. SW_ID field not found. Exiting")
            raise RuntimeError("Error in image. SW_ID field not found. Exiting")
        elif hw_id is None:
            logger.critical("Error in image. HW_ID field not found. Exiting")
            raise RuntimeError("Error in image. HW_ID field not found. Exiting")

        logger.debug('SW_ID = ' + sw_id)
        logger.debug('HW_ID = ' + hw_id)
        hmac_params = HmacParams(int(hw_id, 16), int(sw_id, 16))
        return hmac_params

    def get_root_cert_list(self):
        if self.certs_info is not None:
            root_cert_list = self.certs_info.root_certs
            if len(root_cert_list) > 0:
                return root_cert_list
        return [self.get_root_cert_key()[0]]

    def get_signature(self):
        # Return existing signature
        if self.signature is not None:
            return self.signature

        # Create the signature
        logger.info('Creating signature')
        signature = self.sign_hash(self.hash_to_sign, self.certs[self.ATTEST].priv_key, self.padding, self.hash_algo)
        # Return the generated signature
        return signature

    def get_root_cert_key(self):
        return self._get_cert_key(self.ROOT)

    def get_ca_cert_key(self):
        return self._get_cert_key(self.CA)

    def get_attest_cert_key(self):
        return self._get_cert_key(self.ATTEST)

    def _get_cert_key(self, cert_type):
        cert_type_obj = self.certs[cert_type]

        # Return existing cert
        if cert_type_obj.cert is not None:
            return cert_type_obj.cert, cert_type_obj.priv_key

        # Get the previous cert
        prev_type_obj = None
        for k in self.CERT_LIST:
            if k in self.certs:
                v = self.certs[k]
                if prev_type_obj is None:
                    prev_type_obj = v
                if k == cert_type:
                    break
                prev_type_obj = v
        else:
            raise RuntimeError('Cert type is invalid: ' + cert_type)

        # Generate new cert
        logger.info('Generating new certificate ' + cert_type)
        cert, cert_type_obj.priv_key, cert_type_obj.pub_key = self.generate_new_cert(cert_type,
                                                                                     cert_type_obj,
                                                                                     prev_type_obj,
                                                                                     extfile=cert_type_obj.extfile,
                                                                                     self_sign=(cert_type == self.ROOT),
                                                                                     padding=self.padding)
        # Ensure cert is in pem format
        cert = crypto.cert.get_cert_in_format(cert, crypto.utils.FORMAT_PEM)
        cert_type_obj.cert = cert

        return cert_type_obj.cert, cert_type_obj.priv_key

    def generate_new_cert(self, cert_type, cert_info, prev_cert_info,
                          extfile=None, self_sign=False, padding=PAD_PKCS):

        # Update the subject parameters
        subject_params = self.create_subject_params(cert_type, cert_info.params.params)

        # Generate the certificate request
        cert_req, priv_key, pub_key = self.generate_cert_request(cert_type,
                                                                 cert_info,
                                                                 subject_params,
                                                                 self_sign=self_sign,
                                                                 key_exp=self.signing_attributes.exponent,
                                                                 key_size=self.signing_attributes.key_size,
                                                                 padding=padding)

        # Settle on the extfile
        if extfile is None:
            extfile = cert_info.extfile

        # Sign the certificate request
        if not self_sign:
            logger.info('Signing certificate request for ' + cert_type)
            cert = self.sign_cert_request(cert_req, prev_cert_info, extfile, padding=padding)
        else:
            cert = cert_req

        return cert, priv_key, pub_key

    def sign_cert_request(self, cert_req, prev_cert_info, extfile=None, padding=PAD_PKCS):
        return crypto.cert.sign_cert(cert_req,
                                        prev_cert_info.cert,
                                        prev_cert_info.priv_key,
                                        days=self.CERT_DAYS,
                                        serial=self.CERT_SERIAL,
                                        hash_algo=self.hash_algo,
                                        extfile=extfile,
                                        sign_algo=padding,
                                        pad_salt_len=self.PAD_PSS_SALT_1,
                                        pad_hash_algo=self.hash_algo)

    def create_subject_params(self, cert_type, params):
        subject_param_updater = {
            self.ROOT: lambda x: dict(x),
            self.CA: self.create_subject_params_ca,
            self.ATTEST: self.create_subject_params_attest
        }
        return subject_param_updater[cert_type](params)

    def create_subject_params_ca(self, in_params):
        self.certs[self.CA].extfile = self.openssl_info.ca_cert_xts
        return dict(in_params)

    def validate_root_cert_hash(self, cert_chain_der, expected_root_cert_hash, extracted_image_attributes=None):
        if len(cert_chain_der) < 2:
            raise RuntimeError('Cert chain must contain at least 2 certificates.')
        if len(cert_chain_der) == 2:  # 2-level cert chain
            root_cert = cert_chain_der[1].rstrip(chr(0xff))
        elif len(cert_chain_der) == 3:  # 3-level cert chain
            root_cert = cert_chain_der[2].rstrip(chr(0xff))
        else:  # MRC use case
            root_cert_concatenation = ''.join(cert_chain_der[2:])
            root_cert = root_cert_concatenation.rstrip(chr(0xff))

        # determine hash algo used from attest cert
        if extracted_image_attributes is None:
            extracted_image_attributes = AttributeExtractorBase(
                cert_data=cert_chain_der[0],
                attributes=self.signing_attributes_class()).attributes
        root_cert_hash = HMAC.HASH_ALGO_MAP[extracted_image_attributes.hash_algorithm](root_cert).hexdigest()

        return root_cert_hash == expected_root_cert_hash

    def validate_signing_attributes(self, cert_chain_der, imageinfo, extracted_image_attributes):
        if not cert_chain_der:
            raise RuntimeError('Cert chain must contain at least 1 certificate.')
        attr_config = self.signing_attributes_class()
        attr_config.update_from_image_info_attrs(imageinfo.signing_attributes)
        return extracted_image_attributes.compare(attr_config, extracted_image_attributes.EXCLUDE_NON_ATTEST)

    def _get_cert_key_from_property(self, cert_property, cert_type):
        """Gets the cert & key from dataprov info

        :type cert_property: CertProperty
        :type cert_type: str
        """
        cert, priv_key, pub_key = None, None, None

        # Validate the cert property
        if not cert_property.validate():
            raise RuntimeError(cert_type.title() + " certificate params are invalid! Please check config file.")
        logger.info('Initialization %r with dataprov. These fields might not be used in final output if overridden' %
                    cert_type)

        # Extract the private and public key
        if cert_property.priv_path and c_path.validate_file(cert_property.priv_path):
            logger.info('Using a predefined ' + cert_type + ' private key from: ' + cert_property.priv_path)
            with open(cert_property.priv_path, 'rb') as fp:
                priv_key = fp.read()
                if self.using_ecdsa:
                    priv_key = crypto.ecdsa.get_key_in_format(priv_key, crypto.utils.FORMAT_PEM)
                    pub_key = crypto.ecdsa.get_public_key_from_private(priv_key)
                else:
                    priv_key = crypto.rsa.get_key_in_format(priv_key, crypto.utils.FORMAT_PEM)
                    pub_key = crypto.rsa.get_public_key_from_private(priv_key)

        # Extract the certificate
        if cert_property.cert_path and c_path.validate_file(cert_property.cert_path):
            logger.info('Using a predefined ' + cert_type + ' certificate from: ' + cert_property.cert_path)
            with open(cert_property.cert_path, 'rb') as fp:
                cert = fp.read()
                cert = crypto.cert.get_cert_in_format(cert, crypto.utils.FORMAT_PEM)
        return cert, priv_key, pub_key

    def sign_hash(self, hashbin, priv_key, padding, hash_algo):
        if self.using_ecdsa:
            return crypto.ecdsa.sign(hashbin, priv_key)
        else:
            return crypto.rsa.sign(hashbin, priv_key, padding=padding,
                                   hash_algo=hash_algo, salt_len=self.PAD_PSS_SALT_1)

    def generate_cert_request(self,
                              cert_type,
                              cert_info,
                              subject_params,
                              self_sign=False,
                              key_exp=None,
                              key_size=None,
                              padding=PAD_PKCS):

        # Ensure that the number of SHA bits is equal to or exceeds the number of ECDSA bits
        if self.using_ecdsa:
            num_sha_bits = int(filter(str.isdigit, self.hash_algo))
            num_ecdsa_bits = crypto.ecdsa.curve_size_map[self.signing_attributes.ecdsa_curve]
            if num_sha_bits < num_ecdsa_bits:
                logger.warning("The number of SHA bits is less than the number of ECDSA bits.\n"
                               "SHA is set to {0} bits and ECDSA is set to {1} bits."
                               .format(num_sha_bits, num_ecdsa_bits))

        # Create a new key if there isn't one
        if cert_info.priv_key is None:
            logger.info('Generating new private/public key pair for ' + cert_type)
            cert_info.priv_key, cert_info.pub_key = self.generate_key_pair(key_exp, key_size)

        logger.info('Creating certificate request for ' + cert_type)

        cert = crypto.cert.create_cert(cert_info.priv_key,
                                          subject_params=subject_params,
                                          config=self.openssl_info.openssl_config,
                                          hash_algo=self.hash_algo,
                                          serial=self.CERT_SERIAL,
                                          days=self.CERT_DAYS,
                                          self_sign=self_sign,
                                          sign_algo=padding,
                                          pad_salt_len=self.PAD_PSS_SALT_1,
                                          pad_hash_algo=self.hash_algo)
        return cert, cert_info.priv_key, cert_info.pub_key

    def generate_key_pair(self, exponent=None, keysize=None):
        if self.using_ecdsa:
            return crypto.ecdsa.gen_keys(self.signing_attributes.ecdsa_curve)
        else:
            return crypto.rsa.gen_keys(exponent, keysize)

    def create_subject_params_attest(self, in_params):
        # Set exfile
        self.certs[self.ATTEST].extfile = self.openssl_info.attest_ca_xts

        # GET SIGNING ATTRIBUTE DATA
        num_root_certs = (int(self.signing_attributes.num_root_certs) if
                          self.signing_attributes.num_root_certs is not None else None)
        debug_val = (int(self.signing_attributes.debug, 16) if
                     self.signing_attributes.debug is not None else None)
        oem_id = int(self.signing_attributes.oem_id, 16) & 0xFFFF
        model_id = int(self.signing_attributes.model_id, 16) & 0xFFFF
        in_use_soc_hw_version = (self.signing_attributes.in_use_soc_hw_version if
                                 self.signing_attributes.in_use_soc_hw_version is not None else None)
        use_serial_number_in_signing = (self.signing_attributes.use_serial_number_in_signing if
                                        self.signing_attributes.use_serial_number_in_signing is not None else None)

        # Get the binary to sign length
        if self.data_to_sign_len is None:
            if self.hash_to_sign is not None:
                self.data_to_sign_len = len(self.hash_to_sign)
            else:
                raise RuntimeError('Length of binary could not be computed')

        logger.info('Generating new Attestation certificate and a random key')
        hmac_params = HMAC()
        hmac_params.init_from_config(self.signing_attributes)
        certificate_ou_sw_id = '01 ' + '%.16X' % hmac_params.sw_id + ' SW_ID'
        certificate_ou_hw_id = '02 ' + '%.16X' % hmac_params.msm_id + ' HW_ID'
        certificate_ou_oem_id = '04 ' + '%0.4X' % oem_id + ' OEM_ID'
        certificate_ou_sw_size = '05 ' + '%0.8X' % self.data_to_sign_len + ' SW_SIZE'
        certificate_ou_model_id = '06 ' + '%0.4X' % model_id + ' MODEL_ID'
        certificate_hash_alg = self.SHA_OU_MAP[self.signing_attributes.hash_algorithm]

        certificate_ou = [
            certificate_ou_sw_id,
            certificate_ou_hw_id,
            certificate_ou_oem_id,
            certificate_ou_sw_size,
            certificate_ou_model_id,
            certificate_hash_alg
        ]

        # Optional attributes
        if debug_val is not None:
            certificate_ou_debug_id = '03 ' + '%0.16X' % debug_val + ' DEBUG'
            certificate_ou.append(certificate_ou_debug_id)
        if in_use_soc_hw_version == 1:
            certificate_in_use_soc_hw_version = '13 ' + '%0.4X' % in_use_soc_hw_version + ' IN_USE_SOC_HW_VERSION'
            certificate_ou.append(certificate_in_use_soc_hw_version)
        if use_serial_number_in_signing == 1:
            certificate_use_serial_number_in_signing = '14 ' + '%0.4X' % use_serial_number_in_signing +\
                                                       ' USE_SERIAL_NUMBER_IN_SIGNING'
            certificate_ou.append(certificate_use_serial_number_in_signing)

        # Handle OU property binding
        params = dict(in_params)
        if 'OU' in params.keys():
            if type(params['OU']) == list:
                for item in params['OU']:
                    certificate_ou.append(item)
            else:
                certificate_ou.append(params['OU'])

        # Add OU fields
        params['OU'] = certificate_ou
        logger.info("Adding OU fields to attest certificate.")

        return params

    def validate_sig_using_hash(self, image_hash, signature, cert_chain_der,
                                signed_authority=None, extracted_image_attributes=None,
                                message_function=None):
        # Check that openssl is available
        try:
            crypto.are_available([crypto.modules.MOD_OPENSSL])
        except Exception as e:
            raise RuntimeError('Cannot validate signing: ' + str(e))

        if extracted_image_attributes is None:
            extracted_image_attributes = self._attribute_extractor(
                cert_data=cert_chain_der[0],
                attributes=self.signing_attributes_class()).attributes
        cert_sign_algo = crypto.cert.get_sign_algo(extracted_image_attributes.cert_text)
        use_pss = cert_sign_algo == crypto.cert.SIGN_ALGO_RSA_PSS
        use_dsa = cert_sign_algo == crypto.cert.SIGN_ALGO_ECDSA

        if message_function is None:
            logger.info("Image is signed")
        else:
            logger.info(message_function(signed_authority, use_pss, use_dsa))

        cert_chain_pem = []
        for cert in cert_chain_der:
            cert_chain_pem.append(crypto.cert.get_cert_in_format(cert, crypto.utils.FORMAT_PEM))

        attest_cert, ca_cert_chain = cert_chain_pem[0], '\n'.join(cert_chain_pem[-(len(cert_chain_pem) - 1):])
        crypto.cert.validate_cert_chain(attest_cert, ca_cert_chain)
        public_key = crypto.cert.get_pubkey(attest_cert)

        if use_dsa:
            matches = crypto.ecdsa.verify(image_hash, public_key, signature)
        else:
            matches = crypto.rsa.verify(image_hash, public_key, signature,
                                        padding=crypto.rsa.RSA_PAD_PSS if use_pss else crypto.rsa.RSA_PAD_PKCS,
                                        hash_algo=extracted_image_attributes.hash_algorithm)
        return matches

    def validate_sig(self, to_sign, signature, cert_chain_der, signed_authority=None,
                     extracted_image_attributes=None, imageinfo=None):
        # Check that openssl is available
        try:
            crypto.are_available([crypto.modules.MOD_OPENSSL])
        except Exception as e:
            raise RuntimeError('Cannot validate signing: ' + str(e))

        # Get the hash
        if extracted_image_attributes is None:
            extracted_image_attributes = self._attribute_extractor(
                cert_data=cert_chain_der[0],
                attributes=self.signing_attributes_class()).attributes
        cert_text = extracted_image_attributes.cert_text
        cert_sign_algo = crypto.cert.get_sign_algo(cert_text)
        use_pss = cert_sign_algo == crypto.cert.SIGN_ALGO_RSA_PSS
        use_dsa = cert_sign_algo == crypto.cert.SIGN_ALGO_ECDSA
        # Get the hmac params from attestation cert or hash segment
        hmac_params = self.get_hmac_params_from_cert(
            extracted_attributes=extracted_image_attributes)
        if use_dsa:
            # RSA padding mapping cannot be used to determine
            # whether HMAC was used or not, so config hmac value must be used
            logger.debug("Signer is relying on config's hmac value to determine "
                         "whether signature was generated using HMAC or SHA.")
            if imageinfo.signing_attributes.hmac:
                logger.debug("Signer is assuming that signature was generated using HMAC.")
            else:
                logger.debug("Signer is assuming that signature was generated using SHA.")
                hmac_params = None
        elif use_pss:
            hmac_params = None

        image_hash = Hasher().get_hash(to_sign, hmac_params=hmac_params,
                                       sha_algo=extracted_image_attributes.hash_algorithm)

        # Validate the hash
        return self.validate_sig_using_hash(
            image_hash, signature, cert_chain_der, signed_authority=signed_authority,
            extracted_image_attributes=extracted_image_attributes)

    def validate_more(self, parsegen, imageinfo, attribute_extractor, root_cert_hash, cert_chain_der, errstr):
        # Root cert hash validation
        if root_cert_hash:
            if not cert_chain_der:
                errstr.append('Root certificate for ' + parsegen.authority +
                              ' is not present in image for root cert hash verification')
            elif not self.validate_root_cert_hash(cert_chain_der, root_cert_hash,
                                                  attribute_extractor.attributes):
                errstr.append('Root certificate from image does not match the given root cert hash value')

        # Signing attributes validation
        if imageinfo is not None:
            if not cert_chain_der:
                errstr.append('Certificate chain for ' + parsegen.authority +
                              ' is not present in image signing attributes verification')
            else:
                mismatches = self.validate_signing_attributes(
                    cert_chain_der, imageinfo, attribute_extractor.attributes)
                create_mismatch_table(mismatches, errstr, operation="signing",
                                      data_type_to_compare="Attribute",
                                      image_region=attribute_extractor.image_region)
        if errstr:
            raise RuntimeError('Following validations failed for the image:\n       ' +
                               '\n       '.join(['%d. %s' % (signed_authority, e) for
                                                 signed_authority, e in enumerate(errstr, start=1)]))

    def validate(self, parsegen, root_cert_hash=None, imageinfo=None):
        if parsegen.is_signed():
            # Create error string
            errstr = []

            (signed_authority, data_to_sign, data_signature, cert_chain, __unused) =\
                    parsegen.get_signing_assets()[0]

            # Extract the cert chain list
            cert_chain_der = crypto.cert.split_cert_chain_bin(cert_chain)

            # Extract signing attributes from image
            extracted_image_attributes = AttributeExtractorBase(
                cert_data=cert_chain_der[0],
                attributes=self.signing_attributes_class()).attributes

            # Signature verification
            if not self.validate_sig(data_to_sign, data_signature, cert_chain_der,
                                     signed_authority=signed_authority,
                                     extracted_image_attributes=extracted_image_attributes,
                                     imageinfo=imageinfo):
                errstr.append('Signature is invalid')

            # Extract the cert chain list
            cert_chain_blob = parsegen.cert_chain
            cert_chain_der = crypto.cert.split_cert_chain_bin(cert_chain_blob)

            # Extract signing attributes from image
            attribute_extractor = AttributeExtractorBase(
                cert_data=cert_chain_der[0],
                attributes=self.signing_attributes_class())

            self.validate_more(parsegen, imageinfo, attribute_extractor, root_cert_hash, cert_chain_der, errstr)

            return True
        else:
            raise RuntimeError("Image supplied is not signed.")

    def validate_config(self, imageinfo):
        sa = imageinfo.signing_attributes
        if sa.num_root_certs == 0:
            raise ConfigError('Number of root certificates cannot be set to zero')
        elif sa.num_root_certs > 16:
            raise ConfigError('Number of root certificates cannot be more than 16')
        return sa

    @staticmethod
    def get_hmac_params_from_extracted_attributes(extracted_attributes):
        """
        :returns: A HMAC object with the HMAC parameters from the MBN header values.
        :rtype: obj
        """
        ea = ExtractedAttributes()
        ea.in_use_soc_hw_version = 0
        ea.msm_part = extracted_attributes.hw_id
        ea.mask_soc_hw_version = None
        ea.use_serial_number_in_signing = extracted_attributes.use_serial_number_in_signing
        ea.oem_id = extracted_attributes.oem_id
        ea.model_id = extracted_attributes.model_id
        ea.sw_id = extracted_attributes.sw_id
        msm_id = get_msm_id(ea)
        sw_id = get_sw_id(ea)
        return HmacParams(msm_id, sw_id)
