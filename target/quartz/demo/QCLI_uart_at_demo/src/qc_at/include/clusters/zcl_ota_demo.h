/*
 * Copyright (c) 2018 Qualcomm Technologies, Inc.
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

#ifndef __ZCL_OTA_DEMO_H__
#define __ZCL_OTA_DEMO_H__

#include "qapi_zb_cl_ota.h"

#ifdef __ICCARM__ /** for IAR */
#pragma pack(1)
typedef struct Zigbee_OTA_Demo_Prepend_Header_s
{
   uint32_t FileIdentifier;
   uint16_t HeaderVersion;
   uint16_t HeaderLength;
   uint16_t HeaderFieldControl;
   uint16_t ManufacturerCode;
   uint16_t ImageType;
   uint32_t FileVersion;
   uint16_t StackVersion;
   uint8_t  HeaderString[QAPI_ZB_CL_OTA_HEADER_STRING_LENGTH];
   uint32_t TotalImageSize;
   uint16_t TagID;
   uint32_t TagLength;
} Zigbee_OTA_Demo_Prepend_Header_t;
#pragma pack()
#else
typedef struct __attribute__((__packed__)) Zigbee_OTA_Demo_Prepend_Header_s
{
   uint32_t FileIdentifier;
   uint16_t HeaderVersion;
   uint16_t HeaderLength;
   uint16_t HeaderFieldControl;
   uint16_t ManufacturerCode;
   uint16_t ImageType;
   uint32_t FileVersion;
   uint16_t StackVersion;
   uint8_t  HeaderString[QAPI_ZB_CL_OTA_HEADER_STRING_LENGTH];
   uint32_t TotalImageSize;
   uint16_t TagID;
   uint32_t TagLength;
} Zigbee_OTA_Demo_Prepend_Header_t;
#endif

#define ZIGBEE_OTA_DEMO_PREPEND_SIZE         sizeof(Zigbee_OTA_Demo_Prepend_Header_t)
#define ZIGBEE_OTA_DEMO_HEADER_SIZE          (sizeof(Zigbee_OTA_Demo_Prepend_Header_t) - sizeof(uint16_t) - sizeof(uint32_t))

/**
   @brief Initializes the ZCL OTA demo.

   @param ZigBee_QCLI_Handle is the parent QCLI handle for the OTA demo.

   @return true if the ZigBee OTA demo initialized successfully, false
           otherwise.
*/
qbool_t Initialize_ZCL_OTA_Demo(QCLI_Group_Handle_t ZigBee_QCLI_Handle);

/**
   @brief Registers a client callback from the ZCL OTA demo.

   @param Event_CB is the callback.

   @param CB_Param is the callback parameter.

   @return true if the callback was registered successfully, false
           otherwise.
*/
void ZCL_OTA_Demo_Register_Client_Callback(qapi_ZB_CL_OTA_Client_CB_t Event_CB, uint32_t CB_Param);

/**
   @brief Unregisters a client callback from the ZCL OTA demo.
*/
void ZCL_OTA_Demo_Unregister_Client_Callback(void);

/**
   @brief Queries for an OTA image on a client endpoint.

   @param Endpoint  is the endpoint on which to query the image.

   @param ImageName is the name of the image to query.

   @return true if the query was initiated successfully, false
           otherwise.
*/
qbool_t ZCL_OTA_Demo_Query_Image(uint8_t Endpoint, const char *ImageName);

/**
   @brief Starts an OTA transfer on a client endpoint.

   @param Endpoint is the endpoint on which to start the transfer.

   @return true if the transfer was started successfully, false
           otherwise.
*/
qbool_t ZCL_OTA_Demo_Start_Transfer(uint8_t Endpoint);

/**
   @brief Creates an OTA server cluster.

   @param Endpoint is the endpoint the cluster will be part of.
   @param PrivData is a pointer to the private data for the cluster demo.  This
                   will be initaialized to NULL before the create function is
                   called so can be ignored if the demo has no private data.

   @return The handle for the newly created function or NULL if there was an
           error.
*/
qapi_ZB_Cluster_t ZCL_OTA_Demo_Create_Server(uint8_t Endpoint, void **PrivData);

/**
   @brief Creates an OTA client cluster.

   @param Endpoint is the endpoint the cluster will be part of.
   @param PrivData is a pointer to the private data for the cluster demo.  This
                   will be initaialized to NULL before the create function is
                   called so can be ignored if the demo has no private data.

   @return The handle for the newly created function or NULL if there was an
           error.
*/
qapi_ZB_Cluster_t ZCL_OTA_Demo_Create_Client(uint8_t Endpoint, void **PrivData);

#endif

