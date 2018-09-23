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
 * @file qapi_ble_ipsp.h
 *
 * @brief
 * QCA QAPI for Bluetopia Bluetooth Internet Protocol Support Profile
 * (IPSP) (GATT based) API Type Definitions, Constants, and Prototypes.
 *
 * @details
 * The Internet Protocol Support Profile programming interface  defines the
 * protocols and procedures to be used to implement the Automation IO
 * Service capabilities.
 */

#ifndef __QAPI_BLE_IPSP_H__
#define __QAPI_BLE_IPSP_H__

#include "./qapi_ble_btapityp.h"  /* Bluetooth API Type Definitions.          */
#include "./qapi_ble_bttypes.h"   /* Bluetooth Type Definitions/Constants.    */
#include "./qapi_ble_l2cap.h"     /* QAPI L2CAP prototypes.                   */
#include "./qapi_ble_gatt.h"      /* QAPI GATT prototypes.                    */
#include "./qapi_ble_ipsptypes.h" /* QAPI IPSP prototypes.                    */

/**
 * @addtogroup qapi_ble_services
 * @{
 */

   /* Error Codes that are smaller than these (less than -1000) are     */
   /* related to the Bluetooth Protocol Stack itself (see               */
   /* qapi_ble_errors.h).                                               */
#define QAPI_BLE_IPSP_ERROR_INVALID_PARAMETER            (-1000)
/**< Invalid parameter. */
#define QAPI_BLE_IPSP_ERROR_INVALID_BLUETOOTH_STACK_ID   (-1001)
/**< Invalid Bluetooth Stack ID. */
#define QAPI_BLE_IPSP_ERROR_INSUFFICIENT_RESOURCES       (-1002)
/**< Insufficient resources. */
#define QAPI_BLE_IPSP_ERROR_NODE_ALREADY_INITIALIZED     (-1003)
/**< Node is already initialized. */
#define QAPI_BLE_IPSP_ERROR_NODE_NOT_INITIALIZED         (-1004)
/**< Node has not been initialized. */
#define QAPI_BLE_IPSP_ERROR_NODE_IPSS_NOT_REGISTERED     (-1005)
/**< IPSS is not registered. */
#define QAPI_BLE_IPSP_ERROR_MALFORMATTED_DATA            (-1006)
/**< Malformatted data. */
#define QAPI_BLE_IPSP_ERROR_UNKNOWN_ERROR                (-1007)
/**< Unknown error. */
#define QAPI_BLE_IPSP_ERROR_NO_CONNECTION_INFORMATION    (-1008)
/**< No connection information. */
#define QAPI_BLE_IPSP_ERROR_CONNECTION_NOT_READY         (-1009)
/**< Connection is not ready. */
#define QAPI_BLE_IPSP_ERROR_CONNECTION_NOT_PENDING       (-1010)
/**< Connection is not pending. */

   /* The following constants represent the Open Status Values that are */
   /* possible in the IPSP Open Confirmation Event Data Information.    */
#define QAPI_BLE_IPSP_OPEN_STATUS_SUCCESS                0x00
/**< IPSP connection was successfully opened. */
#define QAPI_BLE_IPSP_OPEN_STATUS_CONNECTION_TIMEOUT     0x01
/**< IPSP connection timed out. */
#define QAPI_BLE_IPSP_OPEN_STATUS_CONNECTION_REFUSED     0x02
/**< IPSP connection was refused. */
#define QAPI_BLE_IPSP_OPEN_STATUS_UNKNOWN_ERROR          0x03
/**< IPSP connection failed for an unknown reason. */

/**
 * Enumeration that represents the default configuration, which is
 * QAPI_BLE_CM_AUTOMATIC_ACCEPT_E and is included here in QAPI for
 * reference. This is also subject to change.
 *
 * The default configuration is QAPI_BLE_CM_AUTOMATIC_ACCEPT_E and is
 * included here in QAPI for reference. This is also subject to change.
 */
typedef enum
{
   QAPI_BLE_CM_AUTOMATIC_ACCEPT_E, /**< Auto accept. */
   QAPI_BLE_CM_AUTOMATIC_REJECT_E, /**< Auto reject. */
   QAPI_BLE_CM_MANUAL_ACCEPT_E     /**< Manual accept. */
} qapi_BLE_IPSP_Node_Connection_Mode_t;

/**
 * Enumeration that represents all the events generated by the IPSP
 * profile. These are used to determine the type of each event generated,
 * and to ensure the proper union element is accessed for the
 * #qapi_BLE_IPSP_Event_Data_t structure.
 */
typedef enum
{
   QAPI_BLE_ET_IPSP_OPEN_INDICATION_E,
   /**< Connection request indication event. */
   QAPI_BLE_ET_IPSP_OPEN_REQUEST_INDICATION_E,
   /**< Open connection request indication event. */
   QAPI_BLE_ET_IPSP_OPEN_CONFIRMATION_E,
   /**< Connection confirmation event. */
   QAPI_BLE_ET_IPSP_CLOSE_INDICATION_E,
   /**< Connection close indication event. */
   QAPI_BLE_ET_IPSP_CLOSE_CONFIRMATION_E,
   /**< Connection close confirmation event. */
   QAPI_BLE_ET_IPSP_DATA_INDICATION_E,
   /**< Data indication event. */
   QAPI_BLE_ET_IPSP_BUFFER_EMPTY_INDICATION_E
   /**< Buffer empty indication event. */
} qapi_BLE_IPSP_Event_Type_t;

/**
 * Structure that represents the data for the
 * QAPI_BLE_ET_IPSP_OPEN_INDICATION_E event.
 */
typedef struct qapi_BLE_IPSP_Open_Indication_Data_s
{
   /**
    * Remote device address.
    */
   qapi_BLE_BD_ADDR_t              RemoteDevice;

   /**
    * Maximum Service Data Unit (SDU).
    */
   uint16_t                        MaxSDUSize;

   /**
    * Maximum Protocol Data Unit (PDU).
    */
   uint16_t                        MaxPDUSize;

   /**
    * Initialize LE credits for the channel.
    */
   uint16_t                        InitialCredits;
} qapi_BLE_IPSP_Open_Indication_Data_t;

#define QAPI_BLE_IPSP_OPEN_INDICATION_DATA_SIZE          (sizeof(qapi_BLE_IPSP_Open_Indication_Data_t))
/**<
 * Size of the #qapi_BLE_IPSP_Open_Indication_Data_t structure.
 */

/**
 * Structure that represents the data for the
 * QAPI_BLE_ET_IPSP_OPEN_REQUEST_INDICATION_E event.
 */
typedef struct qapi_BLE_IPSP_Open_Request_Indication_Data_s
{
   /**
    * Remote device address.
    */
   qapi_BLE_BD_ADDR_t              RemoteDevice;

   /**
    * Maximum SDU.
    */
   uint16_t                        MaxSDUSize;

   /**
    * Maximum PDU.
    */
   uint16_t                        MaxPDUSize;

   /**
    * Initialize LE credits for the channel.
    */
   uint16_t                        InitialCredits;
} qapi_BLE_IPSP_Open_Request_Indication_Data_t;

#define QAPI_BLE_IPSP_OPEN_REQUEST_INDICATION_DATA_SIZE  (sizeof(qapi_BLE_IPSP_Open_Request_Indication_Data_t))
/**<
 * Size of the #qapi_BLE_IPSP_Open_Request_Indication_Data_t structure.
 */

/**
 * Structure that represents the data for the
 * QAPI_BLE_ET_IPSP_OPEN_CONFIRMATION_E event.
 */
typedef struct qapi_BLE_IPSP_Open_Confirmation_Data_s
{
   /**
    * Remote device address.
    */
   qapi_BLE_BD_ADDR_t              RemoteDevice;

   /**
    * Connection status.
    */
   uint32_t                        ConnectionStatus;

   /**
    * Maximum SDU.
    */
   uint16_t                        MaxSDUSize;

   /**
    * Maximum PDU.
    */
   uint16_t                        MaxPDUSize;

   /**
    * Initialize LE credits for the channel.
    */
   uint16_t                        InitialCredits;
} qapi_BLE_IPSP_Open_Confirmation_Data_t;

#define QAPI_BLE_IPSP_OPEN_CONFIRMATION_DATA_SIZE        (sizeof(qapi_BLE_IPSP_Open_Confirmation_Data_t))
/**<
 * Size of the #qapi_BLE_IPSP_Open_Confirmation_Data_t structure.
 */

/**
 * Structure that represents the data for the
 * QAPI_BLE_ET_IPSP_CLOSE_INDICATION_E event.
 */
typedef struct qapi_BLE_IPSP_Close_Indication_Data_s
{
   /**
    * Remote device address.
    */
   qapi_BLE_BD_ADDR_t   RemoteDevice;

   /**
    * Reason for the disconnection.
    */
   uint32_t             Reason;
} qapi_BLE_IPSP_Close_Indication_Data_t;

#define QAPI_BLE_IPSP_CLOSE_INDICATION_DATA_SIZE         (sizeof(qapi_BLE_IPSP_Close_Indication_Data_t))
/**<
 * Size of the #qapi_BLE_IPSP_Close_Indication_Data_t structure.
 */

/**
 * Structure that represents the data for the
 * QAPI_BLE_ET_IPSP_CLOSE_CONFIRMATION_E event.
 */
typedef struct qapi_BLE_IPSP_Close_Confirmation_Data_s
{
   /**
    * Remote device address.
    */
   qapi_BLE_BD_ADDR_t   RemoteDevice;

   /**
    * Result of the disconnection.
    */
   uint32_t             Result;
} qapi_BLE_IPSP_Close_Confirmation_Data_t;

#define QAPI_BLE_IPSP_CLOSE_CONFIRMATION_DATA_SIZE       (sizeof(qapi_BLE_IPSP_Close_Confirmation_Data_t))
/**<
 * Size of the #qapi_BLE_IPSP_Close_Confirmation_Data_t structure.
 */

/**
 * Structure that represents the data for the
 * QAPI_BLE_ET_IPSP_DATA_INDICATION_E event.
 */
typedef struct qapi_BLE_IPSP_Data_Indication_Data_s
{
   /**
    * Remote device address.
    */
   qapi_BLE_BD_ADDR_t              RemoteDevice;

   /**
    * Number of consumed credits.
    */
   uint16_t                        CreditsConsumed;

   /**
    * Length of the data received in the indication.
    */
   uint16_t                        DataLength;

   /**
    * Pointer to the data received in the indication.
    */
   uint8_t                        *Data;
} qapi_BLE_IPSP_Data_Indication_Data_t;

#define QAPI_BLE_IPSP_DATA_INDICATION_DATA_SIZE          (sizeof(qapi_BLE_IPSP_Data_Indication_Data_t))
/**<
 * Size of the #qapi_BLE_IPSP_Data_Indication_Data_t structure.
 */

/**
 * Structure that represents the data for the
 * QAPI_BLE_ET_IPSP_BUFFER_EMPTY_INDICATION_E event.
 */
typedef struct qapi_BLE_IPSP_Buffer_Empty_Indication_Data_s
{
   /**
    * Remote device address.
    */
   qapi_BLE_BD_ADDR_t              RemoteDevice;
} qapi_BLE_IPSP_Buffer_Empty_Indication_Data_t;

#define QAPI_BLE_IPSP_BUFFER_EMPTY_INDICATION_DATA_SIZE  (sizeof(qapi_BLE_IPSP_Buffer_Empty_Indication_Data_t))
/**<
 * Size of the #qapi_BLE_IPSP_Buffer_Empty_Indication_Data_t structure.
 */

/**
 * Structure that represents the container structure for holding all the
 * event data for IPSP.
 */
typedef struct qapi_BLE_IPSP_Event_Data_s
{
   /**
    * Event type used to determine the appropriate union member of
    * the Event_Data field to access.
    */
   qapi_BLE_IPSP_Event_Type_t Event_Data_Type;

   /**
    * Total size of the data contained in the event.
    */
   uint8_t                    Event_Data_Size;
   union
   {
      /**
       * IPSP Open Indication event data.
       */
      qapi_BLE_IPSP_Open_Indication_Data_t         *IPSP_Open_Indication_Data;

      /**
       * IPSP Open Response Indication event data.
       */
      qapi_BLE_IPSP_Open_Request_Indication_Data_t *IPSP_Open_Request_Indication_Data;

      /**
       * IPSP Open Confirmation event data.
       */
      qapi_BLE_IPSP_Open_Confirmation_Data_t       *IPSP_Open_Confirmation_Data;

      /**
       * IPSP Close Indication event data.
       */
      qapi_BLE_IPSP_Close_Indication_Data_t        *IPSP_Close_Indication_Data;

      /**
       * IPSP Close Confirmation event data.
       */
      qapi_BLE_IPSP_Close_Confirmation_Data_t      *IPSP_Close_Confirmation_Data;

      /**
       * IPSP Data Indication event data.
       */
      qapi_BLE_IPSP_Data_Indication_Data_t         *IPSP_Data_Indication_Data;

      /**
       * IPSP Buffer Empty Indication event data.
       */
      qapi_BLE_IPSP_Buffer_Empty_Indication_Data_t *IPSP_Buffer_Empty_Indication_Data;
   }
   /**
    * Event data.
    */
   Event_Data;
} qapi_BLE_IPSP_Event_Data_t;

#define QAPI_BLE_IPSP_EVENT_DATA_SIZE                    (sizeof(qapi_BLE_IPSP_Event_Data_t))
/**<
 * Size of the #qapi_BLE_IPSP_Event_Data_t structure.
 */

/**
 * @brief
 * This declared type represents the Prototype Function for an
 * IPSP Event Callback. This function will be called whenever a define
 * IPSP Event occurs within the Bluetooth Protocol Stack that is
 * specified with the specified Bluetooth Stack ID.
 *
 * @details
 * The event information is passed to the user in the #qapi_BLE_IPSP_Event_Data_t
 * structure. This structure contains all the information about the
 * event that occurred.
 *
 * The caller should use the contents of the L2CAP Event Data
 * only in the context of this callback. If the caller requires
 * the data for a longer period of time, the callback function
 * must copy the data into another data buffer.
 *
 * This function is guaranteed not to be invoked more than once
 * simultaneously for the specified installed callback (i.e., this
 * function does not have be reentrant). It should be noted, however,
 * that if the same callback is installed more than once, the
 * callbacks will be called serially. Because of this, the processing
 * in this function should be as efficient as possible.
 *
 * It should also be noted that this function is called in the Thread
 * Context of a Thread that the user does not own. Therefore, processing
 * in this function should be as efficient as possible (this argument holds
 * anyway because another IPSP Event will not be processed while this
 * function call is outstanding).
 *
 * @note1hang
 * This function must not block and wait for events that can only be
 * satisfied by receiving other Bluetooth Stack Events. A Deadlock
 * will occur because other callbacks might not be issued while
 * this function is currently outstanding.
 *
 * @param[in]  BluetoothStackID      Unique identifier assigned to this
 *                                   Bluetooth Protocol Stack on which the
 *                                   event occurred.
 *
 * @param[in]  IPSP_Event_Data       Pointer to a structure that contains
 *                                   information about the event that has
 *                                   occurred.
 *
 * @param[in]  CallbackParameter     User-defined value that will be
 *                                   received with the IPSP Event data.
 *
 * @return None.
 */
typedef void (QAPI_BLE_BTPSAPI *qapi_BLE_IPSP_Event_Callback_t)(uint32_t BluetoothStackID, qapi_BLE_IPSP_Event_Data_t *IPSP_Event_Data, uint32_t CallbackParameter);

   /* IPSP Node API.                                                    */

/**
 * @brief
 * Intitializesg an IPSP Node (will wait
 * for an IPSP connection to be established by a remote IPSP router).
 * This function will also register the Internet Protocol Support Service
 * (IPSS) that is required for an IPSP Node.
 *
 * @details
 * This function allows the application to select a handle range in GATT
 * to store the service.
 *
 * Only one IPSS server may be open at a time, per the Bluetooth Stack ID.
 *
 * Since IPSS only has one attribute (the Primary Service Declaration),
 * we will not include an API to query the attributes like other services
 * since there is only one.
 *
 * @param[in]  BluetoothStackID     Unique identifier assigned to this
 *                                  Bluetooth Protocol Stack via a
 *                                  call to qapi_BLE_BSC_Initialize().
 *
 * @param[in]  ChannelParameters    IPSP Node's LE Channel Parameters
 *                                  that will be used to configure the
 *                                  IPSP connection when an IPSP
 *                                  connection request is received from
 *                                  an IPSP Router.
 *
 * @param[in]  EventCallback        IPSP Event Callback that will
 *                                  receive IPSP connection events.
 *
 * @param[in]  CallbackParameter    User-defined value that will be
 *                                  received with the specified
 *                                  EventCallback parameter.
 *
 * @param[in,out]  ServiceHandleRange    Pointer that, on input, holds
 *                                       the handle range to store the
 *                                       IPSS service in GATT, and on
 *                                       output, contains the handle
 *                                       range for where the service is
 *                                       stored in GATT.
 *
 * @return      Zero if successful.
 *
 * @return      An error code if negative; one of the following values:
 *              @par
 *                 QAPI_BLE_IPSP_ERROR_INSUFFICIENT_RESOURCES \n
 *                 QAPI_BLE_IPSP_ERROR_INVALID_PARAMETER \n
 *                 QAPI_BLE_IPSP_ERROR_NODE_ALREADY_INITIALIZED \n
 *                 QAPI_BLE_BTGATT_ERROR_INVALID_SERVICE_TABLE_FORMAT \n
 *                 QAPI_BLE_BTGATT_ERROR_INSUFFICIENT_RESOURCES \n
 *                 QAPI_BLE_BTGATT_ERROR_INVALID_PARAMETER \n
 *                 QAPI_BLE_BTGATT_ERROR_INVALID_BLUETOOTH_STACK_ID \n
 *                 QAPI_BLE_BTGATT_ERROR_NOT_INITIALIZED
 */
QAPI_BLE_DECLARATION int QAPI_BLE_BTPSAPI qapi_BLE_IPSP_Initialize_Node_Role(uint32_t BluetoothStackID, qapi_BLE_L2CA_LE_Channel_Parameters_t *ChannelParameters, qapi_BLE_IPSP_Event_Callback_t EventCallback, uint32_t CallbackParameter, qapi_BLE_GATT_Attribute_Handle_Group_t *ServiceHandleRange);

/**
 * @brief
 * Cleans up and frees all resources
 * associated with an IPSP Node that was initialized via the
 * qapi_BLE_IPSP_Initialize_Node_Role() function.
 *
 * @param[in]  BluetoothStackID    Unique identifier assigned to this
 *                                 Bluetooth Protocol Stack via a
 *                                 call to qapi_BLE_BSC_Initialize().
 *
 * @return      Zero if successful.
 *
 * @return      An error code if negative; one of the following values:
 *              @par
 *                 QAPI_BLE_IPSP_ERROR_INVALID_PARAMETER
 */
QAPI_BLE_DECLARATION int QAPI_BLE_BTPSAPI qapi_BLE_IPSP_Cleanup_Node_Role(uint32_t BluetoothStackID);

/**
 * @brief
 * Responds to connection requests
 * received from an IPSP router if the IPSP Node's connection mode
 * is set to QAPI_BLE_CM_MANUAL_ACCEPT_E. Otherwise, the connection
 * request will automatically be accepted or rejected.
 *
 * @param[in]  BluetoothStackID    Unique identifier assigned to this
 *                                 Bluetooth Protocol Stack via a
 *                                 call to qapi_BLE_BSC_Initialize().
 *
 * @param[in]  RemoteDevice        Bluetooth address of
 *                                 the remote device.
 *
 * @param[in]  AcceptConnection    Boolean value that is used to
 *                                 accept or reject the connection.
 *
 * @return      Zero if successful.
 *
 * @return      An error code if negative; one of the following values:
 *              @par
 *                 QAPI_BLE_IPSP_ERROR_INVALID_PARAMETER \n
 *                 QAPI_BLE_IPSP_ERROR_CONNECTION_NOT_PENDING
 */
QAPI_BLE_DECLARATION int QAPI_BLE_BTPSAPI qapi_BLE_IPSP_Open_Connection_Request_Response(uint32_t BluetoothStackID, qapi_BLE_BD_ADDR_t RemoteDevice, boolean_t AcceptConnection);

/**
 * @brief
 * Getts the connection mode of
 * the IPSP Node.
 *
 * @param[in]  BluetoothStackID    Unique identifier assigned to this
 *                                 Bluetooth Protocol Stack via a
 *                                 call to qapi_BLE_BSC_Initialize().
 *
 * @param[out]  ConnectionMode     Pointer that, on ouput, will hold
 *                                 the connection mode for the IPSP
 *                                 Node if this function is
 *                                 successful.
 *
 * @return      Zero if successful.
 *
 * @return      An error code if negative; one of the following values:
 *              @par
 *                 QAPI_BLE_IPSP_ERROR_INVALID_PARAMETER
 */
QAPI_BLE_DECLARATION int QAPI_BLE_BTPSAPI qapi_BLE_IPSP_Get_Node_Connection_Mode(uint32_t BluetoothStackID, qapi_BLE_IPSP_Node_Connection_Mode_t *ConnectionMode);

/**
 * @brief
 * Sets the connection mode of
 * the IPSP Node.
 *
 * @param[in]  BluetoothStackID    Unique identifier assigned to this
 *                                 Bluetooth Protocol Stack via a
 *                                 call to qapi_BLE_BSC_Initialize().
 *
 * @param[in]  ConnectionMode      Enumeration for the connection
 *                                 mode that will be set for the IPSP
 *                                 Node if this function is
 *                                 successful.
 *
 * @return      Zero if successful.
 *
 * @return      An error code if negative; one of the following values:
 *              @par
 *                 QAPI_BLE_IPSP_ERROR_INVALID_PARAMETER
 */
QAPI_BLE_DECLARATION int QAPI_BLE_BTPSAPI qapi_BLE_IPSP_Set_Node_Connection_Mode(uint32_t BluetoothStackID, qapi_BLE_IPSP_Node_Connection_Mode_t ConnectionMode);

   /* IPSP Router API.                                                  */

/**
 * @brief
 * Sends a connection request from
 * an IPSP Router to an IPSP Node.
 *
 * @details
 * Once a connection is opened to an IPSP Node, it can only be closed
 * via a call to the qapi_BLE_IPSP_Close_Connection() function.
 *
 * A positive return value does not mean that a IPSP connection
 * already exists, only that the IPSP connection request has been
 * successfully submitted.
 *
 * The ACL connection to the remote device must already exist before
 * calling this function
 *
 * @param[in]  BluetoothStackID    Unique identifier assigned to this
 *                                 Bluetooth Protocol Stack via a
 *                                 call to qapi_BLE_BSC_Initialize().
 *
 * @param[in]  BD_ADDR             Bluetooth Address of the IPSP
 *                                 Node.
 *
 * @param[in]  ChannelParameters    IPSP Router's LE Channel
 *                                  Parameters that will be used to
 *                                  configure the IPSP connection
 *                                  when an IPSP connection request
 *                                  is sent to the IPSP Node.
 *
 * @param[in]  EventCallback        IPSP Event Callback that will
 *                                  receive IPSP Connection events.
 *
 * @param[in]  CallbackParameter    User-defined value that will be
 *                                  received with the specified
 *                                  EventCallback parameter.
 *
 * @return      Zero if successful.
 *
 * @return      An error code if negative; one of the following values:
 *              @par
 *                 QAPI_BLE_IPSP_ERROR_INVALID_PARAMETER \n
 *                 QAPI_BLE_IPSP_ERROR_INSUFFICIENT_RESOURCES
 */
QAPI_BLE_DECLARATION int QAPI_BLE_BTPSAPI qapi_BLE_IPSP_Connect_Remote_Node(uint32_t BluetoothStackID, qapi_BLE_BD_ADDR_t BD_ADDR, qapi_BLE_L2CA_LE_Channel_Parameters_t *ChannelParameters, qapi_BLE_IPSP_Event_Callback_t EventCallback, uint32_t CallbackParameter);

   /* IPSP Common Role API.                                             */

/**
 * @brief
 * Closes an IPSP connection to a
 * remote device. This function may be called by an IPSP Node or IPSP
 * Router.
 *
 * @details
 * If this function completes successfully, an IPSP Close
 * Indication (QAPI_BLE_ET_IPSP_CLOSE_INDICATION_E) event will be
 * sent to the IPSP Callback Function that was handling the IPSP
 * Events for this connection.
 *
 * @param[in]  BluetoothStackID    Unique identifier assigned to this
 *                                 Bluetooth Protocol Stack via a
 *                                 call to qapi_BLE_BSC_Initialize().
 *
 * @param[in]  RemoteDevice        Bluetooth address of
 *                                 the remote device for which the IPSP
 *                                 connection is to be closed.
 *
 * @return      Zero if successful.
 *
 * @return      An error code if negative; one of the following values:
 *              @par
 *                 QAPI_BLE_IPSP_ERROR_INVALID_PARAMETER \n
 *                 QAPI_BLE_IPSP_ERROR_CONNECTION_NOT_READY \n
 *                 QAPI_BLE_IPSP_ERROR_NO_CONNECTION_INFORMATION
 */
QAPI_BLE_DECLARATION int QAPI_BLE_BTPSAPI qapi_BLE_IPSP_Close_Connection(uint32_t BluetoothStackID, qapi_BLE_BD_ADDR_t RemoteDevice);

/**
 * @brief
 * Sends a packet over an IPSP
 * connection to the specified IPSP remote device.
 *
 * @details
 * This function provides additional functionality to control the
 * ammount of buffer usage for the local IPSP device.
 *
 * If this function returns the Error Code
 * QAPI_BLE_BTPS_ERROR_INSUFFICIENT_BUFFER_SPACE, this is a signal to
 * the caller that the requested data could not be sent because the requested data
 * could not be queued for the IPSP connection. The caller must then wait for
 * the QAPI_BLE_ET_IPSP_BUFFER_EMPTY_INDICATION_E Event before trying to send any
 * more data.
 *
 * If this function is called with a non-NULL QueueingParameters, the data
 * is queued conditionally. If successful, the return value will indicate the
 * number of packets/bytes that are currently queued for the IPSP connection at
 * the time the function returns.
 *
 * @note1hang
 * The application is responsible for managing the data that is sent to the
 * remote device. This means that if all the data cannot fit in one or more SDU
 * packets (depending on if the QueueingParameters are sepcified), the
 * application must maintain how much data was sent. The local device can
 * determine the SDU size for a packet based on the MaxSDUSize field that is
 * received with an IPSP connection event. This way, the application can send
 * more data when the QAPI_BLE_ET_IPSP_BUFFER_EMPTY_INDICATION_E is received.
 *
 * @param[in]  BluetoothStackID    Unique identifier assigned to this
 *                                 Bluetooth Protocol Stack via a
 *                                 call to qapi_BLE_BSC_Initialize().
 *
 * @param[in]  RemoteDevice        Bluetooth address of
 *                                 the remote device.
 *
 * @param[in]  QueueingParameters   Pointer to the optional queueing
 *                                  parameters that control how many SDU
 *                                  packets/bytes may be queued and when
 *                                  we will be able to queue more SDU
 *                                  packets/bytes. If NULL, one SDU
 *                                  will be queued at a time.
 *
 * @param[in]  Data_Length          Length of the data that will be sent
 *                                  to the remote device.
 *
 * @param[in]  Data                 Data that will be sent to the
 *                                  remote device.
 *
 * @return      If QueueingParameters is NULL, then zero for success
 *
 * @return      If QueueingParameters is specified, then Positive nonzero, this
 *              represents the number of packets/bytes that are currently queued
 *              to send to the remote device.
 *
 * @return      An error code if negative; one of the following values:
 *              @par
 *                 QAPI_BLE_IPSP_ERROR_INVALID_PARAMETER \n
 *                 QAPI_BLE_IPSP_ERROR_CONNECTION_NOT_READY \n
 *                 QAPI_BLE_IPSP_ERROR_NO_CONNECTION_INFORMATION
 */
QAPI_BLE_DECLARATION int QAPI_BLE_BTPSAPI qapi_BLE_IPSP_Send_Packet(uint32_t BluetoothStackID, qapi_BLE_BD_ADDR_t RemoteDevice, qapi_BLE_L2CA_Queueing_Parameters_t *QueueingParameters, uint16_t Data_Length, uint8_t *Data);

/**
 * @brief
 * Provides a machanism for granting
 * the specified amount of credits for the IPSP Connection.
 *
 * @details
 * This function must be used for an IPSP Connection if the Manual
 * Credit Mode is specified by the
 * #qapi_BLE_L2CA_LE_Channel_Parameters_t structure. Otherwise, the
 * connection will be in Automatic Credit Mode (the default) and this
 * function is not needed. Credits should be granted back when the
 * remote device receives and QAPI_BLE_ET_IPSP_DATA_INDICATION_E
 * event.
 *
 * @param[in]  BluetoothStackID    Unique identifier assigned to this
 *                                 Bluetooth Protocol Stack via a
 *                                 call to qapi_BLE_BSC_Initialize().
 *
 * @param[in]  RemoteDevice        Bluetooth address of
 *                                 the remote device.
 *
 * @param[in]  Credits             Credits granted, if any.
 *
 * @return      Zero if successful.
 *
 * @return      An error code if negative; one of the following values:
 *              @par
 *                 QAPI_BLE_IPSP_ERROR_INVALID_PARAMETER \n
 *                 QAPI_BLE_IPSP_ERROR_CONNECTION_NOT_READY \n
 *                 QAPI_BLE_IPSP_ERROR_NO_CONNECTION_INFORMATION
 */
QAPI_BLE_DECLARATION int QAPI_BLE_BTPSAPI qapi_BLE_IPSP_Grant_Credits(uint32_t BluetoothStackID, qapi_BLE_BD_ADDR_t RemoteDevice, uint16_t Credits);

/**
 * @}
 */

#endif

