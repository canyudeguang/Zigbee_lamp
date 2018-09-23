##  @file       elfUtils.py
#   @author     Christopher Ahn (cahn@qualcomm.com)
#   @brief      This file contains common utilities for the generic manipulator
#               for ELF files.

#===============================================================================
# Copyright (c) 2013-2018 Qualcomm Technologies, Inc.
# All Rights Reserved.
# Copyright (c) 2018 Qualcomm Technologies, Inc.
# All rights reserved.
# Redistribution and use in source and binary forms, with or without modification, are permitted (subject to the limitations in the disclaimer below)
# provided that the following conditions are met:
# Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
# Neither the name of Qualcomm Technologies, Inc. nor the names of its contributors may be used to endorse or promote products derived
# from this software without specific prior written permission.
# NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
# BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
# OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
# EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#===============================================================================

#  $Header: //components/rel/core.ioe/1.0/bsp/build/scripts/elfManipulator/include/elfUtils.py#2 $
#  $DateTime: 2018/05/30 03:44:40 $
#  $Author: pwbldsvc $
#  $Change: 16281478 $
#                      EDIT HISTORY FOR FILE
#
#  This section contains comments describing changes made to the module.
#  Notice that changes are listed in reverse chronological order.
#
# when      who     what, where, why
# --------  ------  ------------------------------------------------------------
# 03/29/13  cahn    Updated convertToELF and convertFromELF to take optional
#                   encoding option. Apparently can vary within ELF.
# 03/29/13  cahn    Various new APIs added. Changed the name of functions.
# 03/26/13  cahn    Minor bug fix. Was calling utils.<func> in error case.
# 03/10/13  cahn    Added convertToELF function.
# 03/10/13  cahn    Added writeDataToELF function.
# 03/07/13  cahn    Specification review, cleanup and added Doxygen comments.
# 03/05/13  cahn    Added getDataFromELF function.
# 03/05/13  cahn    Added isPowerOfTwo function.
# 03/04/13  cahn    Creation.
#===============================================================================

# Imports
import sys
import elfConstants as const
import string
import struct

## Global variable storing the endianess of the ELF
EI_DATA = const.elfData.ELFDATA2LSB

##
# @brief    This class implements a customized exception for ELF manipulator
class elfManipulatorError(Exception):
    def __init__ (self, expr):
        self.expr = expr

##
# @brief    Function used to determine if a number is a power of 2.
# @param    number  The number in question
# @returns  TRUE if the number is a power of 2, FALSE otherwise.
def isPowerOfTwo(number):
    return (number != 0 and (number & (number - 1)) == 0);

##
# @brief    Function used to raise an elfManipulatorWarning
# @param    warningMessage  Warning message to print
def raiseElfManipulatorWarning(warningMessage):
    try:
        sys.stderr.write("%s\n" % warningMessage)
    except elfManipulatorError as e:
        print "Warning raised during ELF manipulation: " + e.expr
        raise

##
# @brief    Function used to raise an elfManipulatorError
# @param    errorMessage  Error message to print
def raiseElfManipulatorError(errorMessage):
    try:
        raise elfManipulatorError(errorMessage)
    except elfManipulatorError as e:
        print "Exception raised during ELF manipulation: " + e.expr
        raise

##
# @brief    Function used to open and read out data from the ELF
# @param    elf     ELF file to read from
# @param    offset  Offset in the ELF at which to start reading
# @param    size    Amount to read
# @returns  The data read out from the ELF
def getDataFromELF(elf, offset, size):
    try :
        elfFileHandle = open(elf, 'rb')
    except IOError:
        raiseElfManipulatorError("Failed to open " + elf)
    else:
        elfFileHandle.seek(offset)
        data = elfFileHandle.read(size)
        elfFileHandle.close()
    return data

##
# @brief    Function to get null terminated string from contents
# @param    contents    Contents to search
# @param    offset      Offset into contents to start at (default '0')
def getStringFromContents(contents, offset):
    end = string.find(contents, '\0', offset)
    if end == -1:
        raiseElfManipulatorError("Failed to getStringFromContents")
    return contents[offset:end]

##
# @brief    Function used to write out data to an elfFileHandle
# @param    elfFileHandle   ELF file handle to write to
# @param    offset          Offset in the ELF at which to start writing
# @param    data            Data to write
# @pre      File handle has already been acquired.
def writeDataToELF(elfFileHandle, offset, data):
    elfFileHandle.seek(offset)
    elfFileHandle.write(data)

##
# @brief    Function to set the ELF data encoding
# @param    encoding    Data encoding as determined by ELF header
def setElfEncoding(encoding):
    global EI_DATA
    if encoding in [const.elfData.ELFDATA2LSB, const.elfData.ELFDATA2MSB]:
        EI_DATA = encoding
    else:
        raiseElfManipulatorError("Invalid data encoding specified")


##
# @brief    Function used to read a packed value from a buffer
# @param    buffer      Buffer to read from
# @param    amount      Amount to read
# @param    offset      Buffer offset to start reading from (default '0')
# @param    encoding    If specified, override EI_DATA (default '0')
# @returns  Unpacked data from the buffer
def convertFromELF(buffer, amount, offset=0, encoding=0):
    if encoding == 0:
        # Set the endianness according to global EI_DATA variable
        if EI_DATA == const.elfData.ELFDATA2LSB:
            endianness = "<"
        elif EI_DATA == const.elfData.ELFDATA2MSB:
            endianness = ">"
        else:
            raiseElfManipulatorError("Invalid data encoding specified")
    else:
        endianness = encoding
    # Set the format according to the amount
    if (amount == const.elfDataTypes.Elf32_Addr): #Elf32_Off, Elf32_Sword, Elf32_Word
        format = "L"
    elif (amount == const.elfDataTypes.Elf32_Half):
        format = "H"
    elif (amount == const.elfDataTypes.unsigned_char):
        format = "B"
    else:
        raiseElfManipulatorError("Unexpected amount specified for convertFromELF")
    # Unpack buffer with specified endianness and format at given offset
    return struct.unpack_from(endianness + format, buffer, offset)[0]

##
# @brief    Function used to write a unpacked value to a buffer
# @param    size        Size of string to pack into
# @param    number      Value to pack
# @param    encoding    If specified, override EI_DATA (default '0')
# @returns  Packed string containing the specified number
def convertToELF(size, number, encoding=0):
    if encoding == 0:
        # Set the endianess according to global EI_DATA variable
        if EI_DATA == const.elfData.ELFDATA2LSB:
            endianness = "<"
        elif EI_DATA == const.elfData.ELFDATA2MSB:
            endianness = ">"
        else:
            raiseElfManipulatorError("Invalid data encoding specified")
    else:
        endianness = encoding
    # Set the format according to the size
    if (size == const.elfDataTypes.Elf32_Addr): #Elf32_Off, Elf32_Sword, Elf32_Word
        format = "L"
    elif (size == const.elfDataTypes.Elf32_Half):
        format = "H"
    elif (size == const.elfDataTypes.unsigned_char):
        format = "B"
    else:
        raiseElfManipulatorError("Unexpected size specified for convertToELF")
    # Pack number with specified endianess and format
    return struct.pack(endianness + format, number)
