/*
 * Copyright (c) 2016-2018 Qualcomm Technologies, Inc.
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
 * @file qapi_dnss.h
 *
 * @addtogroup qapi_networking_dnss
 * @{
 *
 * @brief DNS Server Service
 *
 * @details The DNS Server service provides a name resolution service to resolve a host name to an IPv4/IPv6 address.
 *
 * @}
 */

#ifndef _QAPI_DNSS_H_
#define _QAPI_DNSS_H_

#include "stdint.h"
#include "qapi_addr.h"  /* ip6_addr */

/** @addtogroup qapi_networking_dnss
@{ */

/**
 * Commands to start, stop, and disable the DNS server.
 */
typedef enum
{
    QAPI_NET_DNS_SERVER_DISABLE_E,
   /**< STOP plus free the space for internal data structures. */

    QAPI_NET_DNS_SERVER_START_E,
   /**< Allocate space for internal data structures. A DNS query is allowed after the START command. */

    QAPI_NET_DNS_SERVER_STOP_E
   /**< Stop sending DNS responses to the client. Keep internal data structures. */
} qapi_Net_DNS_Server_Command_t;

/** Maximum length of the host name, including domain (e.g., www.qualcomm.com). */
#define  QAPI_NET_DNS_MAX_HOSTNAME_LEN      128

/**
 * @brief For use with qapi_Net_DNSs_Get_Host_List() to get the host name and its IPv4/IPv6 address.
 */
typedef struct
{
    char host_Name[QAPI_NET_DNS_MAX_HOSTNAME_LEN];
   /**< Host name. */

    uint32_t addr4;
   /**< IPv4 address in network order. */

    ip6_addr addr6;
   /**< IPv6 address. */

    uint32_t ttl;
   /**< Time to live in seconds. */
} qapi_Net_DNS_Host_t;

/**
 * Gets the list of host names and their IP addresses.
 *
 * @code {.c}
 *      int32_t num_of_entries, i;
 *      qapi_Net_DNS_Host_t *list;
 *
 *      if (qapi_Net_DNSs_Get_Host_List(&num_of_entries, NULL) == 0 &&
 *          num_of_entries > 0 &&
 *          (list = (qapi_Net_DNS_Host_t *)malloc(num_of_entries * sizeof(qapi_Net_DNS_Host_t))) != NULL)
 *      {
 *          qapi_Net_DNSs_Get_Host_List(&num_of_entries, list);
 *          for (i = 0; i < num_of_entries; ++i)
 *          {
 *              // Display the list
 *          }
 *      }
 * @endcode
 *
 * @param[in,out] n     Pointer to an integer to contain the total number of entries in the list.
 * @param[in] hostlist  Pointer to a buffer to contain the list. Specify NULL if no buffer is allocated.
 *
 * @return
 * On success, 0 is returned; on error, -1 is returned.
 */
int32_t qapi_Net_DNSs_Get_Host_List(int32_t *n, qapi_Net_DNS_Host_t *hostlist);

/**
 * Checks whether the DNS server is started.
 *
 * @return
 * 0 if not started, 1 if started.
 */
int32_t qapi_Net_DNSs_Is_Started(void);

/**
 * Starts, stops, or disable the DNS server.
 *
 * @param[in] cmd   Command to start, stop, or disable the DNS server. \n
 *                  The supported commands are QAPI_NET_DNS_SERVER_DISABLE_E,
 *                  QAPI_NET_DNS_SERVER_START_E, and QAPI_NET_DNS_SERVER_STOP_E.
 *
 * @return
 * On success, 0 is returned; on error, -1 is returned.
 */
int32_t qapi_Net_DNSs_Command(qapi_Net_DNS_Server_Command_t cmd);

/**
 * Adds a host name and its IP address to the system.
 *
 * @param[in] host_Name  Host's name string.
 * @param[in] host_Addr  IPv4/IPv6 address.
 * @param[in] ttl       Time to live in seconds. \n
 *                      ttl = 0 indicates 0x7FFFFFFF seconds.
 *
 * @return
 * On success, 0 is returned; on error, -1 is returned.
 */
int32_t qapi_Net_DNSs_Add_Host(const char *host_Name, struct ip46addr *host_Addr, uint32_t ttl);

/**
 * Remove a host/IP address entry from the system.
 *
 * @param[in] hostname   Host's name string.
 *
 * @return
 * On success, 0 is returned; on error, -1 is returned.
 */
int32_t qapi_Net_DNSs_Del_Host(const char *hostname);


/** @} */

#endif /* _QAPI_DNSS_H_ */
