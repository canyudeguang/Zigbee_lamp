/*
 * Copyright (c) 2017-2018 Qualcomm Technologies, Inc.
 * All Rights Reserved.
*/
// Copyright (c) 2018 Qualcomm Technologies, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to the limitations in the disclaimer below) 
// provided that the following conditions are met:
// Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
// Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
// Neither the name of Qualcomm Technologies, Inc. nor the names of its contributors may be used to endorse or promote products derived 
// from this software without specific prior written permission.
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE. 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, 
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, 
// OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/**
 * @file qapi_csr_generator.h
 *
 * @details QAPI to generate a Certificate Signing Request (CSR).
 *
 */

#ifndef __QAPI_CSR_GENERATOR_H__
#define __QAPI_CSR_GENERATOR_H__

#include <stdint.h>


/** @addtogroup qapi_networking_ssl_cert
@{ */


/** ECC type key-pair to use for the CSR. */
#define QAPI_KEYPAIR_PARAM_KEY_TYPE_ECC_E 1
/** RSA type key-pair to use for the CSR.  The RSA key-pair generation is not yet supported. */
#define QAPI_KEYPAIR_PARAM_KEY_TYPE_RSA_E 2

/** SECP 192R1 curve type to use for the key-pair generation of the CSR. */
#define QAPI_EC_CURVE_ID_SECP192R1  19
/** SECP 224R1 curve type to use for the key-pair generation of the CSR. */
#define QAPI_EC_CURVE_ID_SECP224R1  21
/** SECP 256R1 curve type to use for the key-pair generation of the CSR. */
#define QAPI_EC_CURVE_ID_SECP256R1  23
/** SECP 384R1 curve type to use for the key-pair generation of the CSR. */
#define QAPI_EC_CURVE_ID_SECP384R1  24


/** Size of the nonce to be used for the encryption of the CSR. */
#define QAPI_PROTECTED_CSR_NONCE_SIZE 8
/** Size of the tag to be used for authentication of the encrypted CSR. */
#define QAPI_PROTECTED_CSR_TAG_SIZE 16

/**
 * @brief Parameters for the ECC key-pair.
 */
typedef struct qapi_Crypto_Keypair_ECC_Params_s {
    uint16_t curve_Type;
    /**< Curve type to use for the key-pair generation of the Certificate Signing Request (CSR).
         Currently, the only supported curve type is QAPI_EC_CURVE_ID_SECP224R1. */
} qapi_Crypto_Keypair_ECC_Params_t;

/**
 * @brief Parameters for the RSA key-pair.
 */
typedef struct qapi_Crypto_Keypair_RSA_Params_s {
    uint32_t key_Length_In_Bits;
    /**< Length of the key to use for the key-pair generation of the CSR.
         The RSA key-pair generation is not yet supported. */
} qapi_Crypto_Keypair_RSA_Params_t;

/**
 * @brief Crypto key-pair parameters.
 */
typedef struct qapi_Crypto_Keypair_Params_s {
    uint32_t key_Type;
    /**< Key type to use. Currently, only QAPI_KEYPAIR_PARAM_KEY_TYPE_ECC_E is supported. */
    union {
        qapi_Crypto_Keypair_ECC_Params_t ecc;
        /**< ECC parameters specification if an ECC key type is to be used for key-pair generation. */
        qapi_Crypto_Keypair_RSA_Params_t rsa;
        /**< RSA parameters specification if an RSA key type is to be used for key-pair generation. */
    } params;
    /**< Parameters for the key-pair generation. */
} qapi_Crypto_Keypair_Params_t;

/** @} */

/** @addtogroup qapi_networking_ssl
@{ */

/**
 * @brief Generates a CSR and encrypts it using a device-specific key.
 *
 * @param[in]       keypair_Name                Name to use for key-pair associated with the generated CSR.
 * @param[in]       keypair_Params              Key-pair parameters.
 * @param[in]       service_Provider_ID         Service provider ID buffer to use in CSR.  This ID will be converted to hex string and be put in common name field.
 * @param[in]       service_Provider_ID_Size    Size of the service provider ID buffer.
 * @param[out]    CSR                         Pointer to the CSR buffer in binary format.
 * @param[out]    CSR_Size                    Set to the CSR buffer size.
 * @param[out]    CSR_Protected               Pointer to the encrypted CSR buffer.
 * @param[out]    CSR_Protected_Size          Set to the encrypted CSR buffer size.
 * @param[in,out] nonce                       Set to the randomly generated nonce that is used for encrypting the CSR.
 * @param[in,out] tag                         Set to authentication tag associated with the encrypted CSR.
 *
 * @return QAPI_OK on success, or an error code on failure.
 */
int
qapi_Net_SSL_CSR_Generate(
    const char *keypair_Name,
    const qapi_Crypto_Keypair_Params_t * keypair_Params,
    const uint8_t *service_Provider_ID,
    size_t service_Provider_ID_Size,
    uint8_t **CSR,
    size_t *CSR_Size,
    uint8_t **CSR_Protected,
    size_t *CSR_Protected_Size,
    uint8_t nonce[QAPI_PROTECTED_CSR_NONCE_SIZE],
    uint8_t tag[QAPI_PROTECTED_CSR_TAG_SIZE]
    );

/** @} */

#endif /* __QAPI_CSR_GENERATOR_H__ */
