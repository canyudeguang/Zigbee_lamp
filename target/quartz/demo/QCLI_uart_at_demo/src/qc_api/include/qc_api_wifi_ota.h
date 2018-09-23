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

                                                                                
     
#ifndef __QC_API_WIFI_OTA_H                                                         
#define __QC_API_WIFI_OTA_H 

/*-------------------------------------------------------------------------     
 *  Include Files                                                               
 *-----------------------------------------------------------------------*/
#include "qcli_api.h"
#include "qcli_api.h"
#include "qapi_status.h"
#include "qc_api_main.h"
#include "qc_at_ota_ftp.h"
#include "qc_at_ota_http.h"
#include "qapi_firmware_upgrade.h"
#include "qosa_util.h"

/*-------------------------------------------------------------------------
 * Function prototypes
 *-----------------------------------------------------------------------*/
int32_t qc_api_ota_Fwd(void);
int32_t qc_api_ota_DeleteFwd(int32_t fwd_num);
int32_t qc_api_ota_Trial(QCLI_Parameter_t *Parameter_List);
int32_t qc_api_ota_ActiveImage(int32_t img_id);
int32_t qc_api_ota_FtpUpgrade(QCLI_Parameter_t *Parameter_List);
int32_t qc_api_ota_HttpUpgrade(QCLI_Parameter_t *Parameter_List);

#endif  //QC_API_WIFI_OTA_H 
