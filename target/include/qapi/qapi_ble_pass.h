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
 * @file qapi_ble_pass.h
 *
 * @brief
 * QCA QAPI for Bluetopia Bluetooth Phone Alert State Service (PASS)
 * (GATT based) API Type Definitions, Constants, and Prototypes.
 *
 * @details
 * The Phone Alert State Service programming interface defines the protocols and
 * procedures to be used to implement the Phone Alert State Service
 * capabilities.
 */

#ifndef __QAPI_BLE_PASS_H__
#define __QAPI_BLE_PASS_H__

#include "./qapi_ble_btapityp.h"  /* Bluetooth API Type Definitions.          */
#include "./qapi_ble_bttypes.h"   /* Bluetooth Type Definitions/Constants.    */
#include "./qapi_ble_gatt.h"      /* QAPI GATT prototypes.                    */
#include "./qapi_ble_passtypes.h" /* QAPI PASS prototypes.                    */

/**
 * @addtogroup qapi_ble_services
 * @{
 */

   /* Error Return Codes.                                               */

   /* Error Codes that are smaller than these (less than -1000) are     */
   /* related to the Bluetooth Protocol Stack itself (see               */
   /* qapi_ble_errors.h).                                               */
#define QAPI_BLE_PASS_ERROR_INVALID_PARAMETER            (-1000)
/**< Invalid parameter. */
#define QAPI_BLE_PASS_ERROR_INVALID_BLUETOOTH_STACK_ID   (-1001)
/**< Invalid Bluetooth Stack ID. */
#define QAPI_BLE_PASS_ERROR_INSUFFICIENT_RESOURCES       (-1002)
/**< Insufficient resources. */
#define QAPI_BLE_PASS_ERROR_SERVICE_ALREADY_REGISTERED   (-1003)
/**< Service is already registered. */
#define QAPI_BLE_PASS_ERROR_INVALID_INSTANCE_ID          (-1004)
/**< Invalid service instance ID. */
#define QAPI_BLE_PASS_ERROR_MALFORMATTED_DATA            (-1005)
/**< Malformatted data. */
#define QAPI_BLE_PASS_ERROR_UNKNOWN_ERROR                (-1006)
/**< Unknown error. */

/**
 * Structure that represents the format of the PASS Alert Status
 * Characteristic value.
 */
typedef struct qapi_BLE_PASS_Alert_Status_s
{
   /**
    * Flags if the ringer state is active.
    */
   boolean_t RingerStateActive;

   /**
    * Flags if the vibrate state is active.
    */
   boolean_t VibrateStateActive;

   /**
    * Flags if the display state is active.
    */
   boolean_t DisplayStateActive;
} qapi_BLE_PASS_Alert_Status_t;

#define QAPI_BLE_PASS_ALERT_STATUS_DATA_SIZE             (sizeof(qapi_BLE_PASS_Alert_Status_t))
/**<
 * Size of the #qapi_BLE_PASS_Alert_Status_t structure.
 */

/**
 * Enumeration that represents the valid Ringer Setting characteristic
 * values.
 */
typedef enum
{
   QAPI_BLE_RS_SILENT_E = QAPI_BLE_PASS_RINGER_SETTING_RINGER_SILENT,
   /**< Silent ringer setting. */
   QAPI_BLE_RS_NORMAL_E = QAPI_BLE_PASS_RINGER_SETTING_RINGER_NORMAL
   /**< Normal ringer setting. */
} qapi_BLE_PASS_Ringer_Setting_t;

/**
 * Enumeration that represents the commands that may be received in the
 * QAPI_BLE_ET_PASS_SERVER_RINGER_CONTROL_COMMAND_E event and that may be
 * written using the qapi_BLE_PASS_Write_Ringer_Control_Command()
 * function.
 */
typedef enum
{
   QAPI_BLE_RC_SILENT_E        = QAPI_BLE_PASS_RINGER_CONTROL_COMMAND_SILENT_MODE,
   /**< Silent mode control command. */
   QAPI_BLE_RC_MUTE_ONCE_E     = QAPI_BLE_PASS_RINGER_CONTROL_COMMAND_MUTE_ONCE,
   /**< Mute once control command. */
   QAPI_BLE_RC_CANCEL_SILENT_E = QAPI_BLE_PASS_RINGER_CONTROL_COMMAND_CANCEL_SILENT_MODE
   /**< Cancel silent mode control command. */
} qapi_BLE_PASS_Ringer_Control_Command_t;

/**
 * Enumeration that represents the valid Read Request types that a server
 * may receive in a
 * QAPI_BLE_ET_PASS_SERVER_READ_CLIENT_CONFIGURATION_REQUEST_E or
 * QAPI_BLE_ET_PASS_SERVER_CLIENT_CONFIGURATION_UPDATE_E event. This is
 * also used by the qapi_BLE_PASS_Send_Notification() to denote the
 * characteristic value to notify.
 *
 * For each event, it is up to the application to return (or write) the
 * correct Client Configuration descriptor based on this value.
 */
typedef enum
{
   QAPI_BLE_RR_ALERT_STATUS_E,  /**< Alert Status */
   QAPI_BLE_RR_RINGER_SETTING_E /**< Ringer Setting. */
} qapi_BLE_PASS_Characteristic_Type_t;

/**
 * Enumeration that represents all the events generated by the PASS
 * service. These are used to determine the type of each event
 * generated, and to ensure the proper union element is accessed for the
 * #qapi_BLE_PASS_Event_Data_t structure.
 */
typedef enum
{
   QAPI_BLE_ET_PASS_SERVER_READ_CLIENT_CONFIGURATION_REQUEST_E,
   /**< Read CCCD request event. */
   QAPI_BLE_ET_PASS_SERVER_CLIENT_CONFIGURATION_UPDATE_E,
   /**< Write CCCD request event. */
   QAPI_BLE_ET_PASS_SERVER_RINGER_CONTROL_COMMAND_INDICATION_E
   /**< Ringer Control Command indication event. */
} qapi_BLE_PASS_Event_Type_t;

/**
 * Structure that contains the attribute handles that will need to be
 * cached by a PASS client in order to only do service discovery once.
 */
typedef struct qapi_BLE_PASS_Client_Information_s
{
   /**
    * PASS Alert Status attribute handle.
    */
   uint16_t Alert_Status;

   /**
    * PASS Alert Status Client Characteristic Configuration
    * Descriptor (CCCD) attribute handle.
    */
   uint16_t Alert_Status_Client_Configuration;

   /**
    * PASS Ringer Setting attribute handle.
    */
   uint16_t Ringer_Setting;

   /**
    * PASS Ringer Setting CCCD attribute handle.
    */
   uint16_t Ringer_Setting_Client_Configuration;

   /**
    * PASS Ringer Control Point attribute handle.
    */
   uint16_t Ringer_Control_Point;
} qapi_BLE_PASS_Client_Information_t;

#define QAPI_BLE_PASS_CLIENT_INFORMATION_DATA_SIZE       (sizeof(qapi_BLE_PASS_Client_Information_t))
/**<
 * Size of the #qapi_BLE_PASS_Client_Information_t structure.
 */

/**
 * Structure that contains all of the per client data that will need to
 * be stored by a PASS server.
 */
typedef struct qapi_BLE_PASS_Server_Information_s
{
   /**
    * Flags if notifications have been configured for the PASS Alert
    * Status by the PASS client.
    */
   boolean_t Alert_Status_Client_Configuration;

   /**
    * Flags if notifications have been configured for the PASS Ringer
    * Setting by the PASS client.
    */
   boolean_t Ringer_Setting_Client_Configuration;
} qapi_BLE_PASS_Server_Information_t;

#define QAPI_BLE_PASS_SERVER_INFORMATION_DATA_SIZE        (sizeof(qapi_BLE_PASS_Server_Information_t))
/**<
 * Size of the #qapi_BLE_PASS_Server_Information_t structure.
 */

/**
 * Structure that represents the format for the data that is dispatched
 * to a PASS server when a PASS client has sent a request to read a PASS
 * characteristic's CCCD.
 *
 * Some of the structure fields will be required when responding to a
 * request using the
 * qapi_BLE_PASS_Read_Client_Configuration_Response() function.
 */
typedef struct qapi_BLE_PASS_Read_Client_Configuration_Data_s
{
   /**
    * PASS instance that dispatched the event.
    */
   uint32_t                            InstanceID;

   /**
    * GATT connection ID for the connection with the PASS client
    * that made the request.
    */
   uint32_t                            ConnectionID;

   /**
    * GATT transaction ID for the request.
    */
   uint32_t                            TransactionID;

   /**
    * GATT connection type, which identifies the transport used for
    * the connection with the PASS client.
    */
   qapi_BLE_GATT_Connection_Type_t     ConnectionType;

   /**
    * Bluetooth address of the PASS client that made the request.
    */
   qapi_BLE_BD_ADDR_t                  RemoteDevice;

   /**
    * Identifies the requested CCCD based on the PASS characteristic
    * type.
    */
   qapi_BLE_PASS_Characteristic_Type_t ClientConfigurationType;
} qapi_BLE_PASS_Read_Client_Configuration_Data_t;

#define QAPI_BLE_PASS_READ_CLIENT_CONFIGURATION_DATA_SIZE  (sizeof(qapi_BLE_PASS_Read_Client_Configuration_Data_t))
/**<
 * Size of the #qapi_BLE_PASS_Read_Client_Configuration_Data_t structure.
 */

/**
 * Structure that represents the format for the data that is dispatched
 * to a PASS server when a PASS client has sent a request to write a PASS
 * characteristic's CCCD.
 */
typedef struct qapi_BLE_PASS_Client_Configuration_Update_Data_s
{
   /**
    * PASS instance that dispatched the event.
    */
   uint32_t                            InstanceID;

   /**
    * GATT connection ID for the connection with the PASS client
    * that made the request.
    */
   uint32_t                            ConnectionID;

   /**
    * GATT connection type, which identifies the transport used for
    * the connection with the PASS client.
    */
   qapi_BLE_GATT_Connection_Type_t     ConnectionType;

   /**
    * Bluetooth address of the PASS client that made the request.
    */
   qapi_BLE_BD_ADDR_t                  RemoteDevice;

   /**
    * Identifies the requested CCCD based on the PASS Characteristic
    * type.
    */
   qapi_BLE_PASS_Characteristic_Type_t ClientConfigurationType;

   /**
    * Flags if the CCCD
    * has been configured for notifications.
    */
   boolean_t                           NotificationsEnabled;
} qapi_BLE_PASS_Client_Configuration_Update_Data_t;

#define QAPI_BLE_PASS_CLIENT_CONFIGURATION_UPDATE_DATA_SIZE  (sizeof(qapi_BLE_PASS_Client_Configuration_Update_Data_t))
/**<
 * Size of the #qapi_BLE_PASS_Client_Configuration_Update_Data_t
 * structure.
 */

/**
 * Structure that represents the format for the data that is dispatched
 * to a PASS server when a PASS client has sent a request to write the
 * PASS Ringer Control Point.
 */
typedef struct qapi_BLE_PASS_Ringer_Control_Command_Data_s
{
   /**
    * PASS instance that dispatched the event.
    */
   uint32_t                               InstanceID;

   /**
    * GATT connection ID for the connection with the PASS client
    * that made the request.
    */
   uint32_t                               ConnectionID;

   /**
    * GATT connection type, which identifies the transport used for
    * the connection with the PASS client.
    */
   qapi_BLE_GATT_Connection_Type_t        ConnectionType;

   /**
    * Bluetooth address of the PASS client that made the request.
    */
   qapi_BLE_BD_ADDR_t                     RemoteDevice;

   /**
    * PASS Control Point command that has been requested to be
    * written.
    */
   qapi_BLE_PASS_Ringer_Control_Command_t Command;
} qapi_BLE_PASS_Ringer_Control_Command_Data_t;

#define QAPI_BLE_PASS_RINGER_CONTROL_COMMAND_DATA_SIZE   (sizeof(qapi_BLE_PASS_Ringer_Control_Command_Data_t))
/**<
 * Size of the #qapi_BLE_PASS_Ringer_Control_Command_Data_t structure.
 */

/**
 * Structure that represents the container structure for holding all the
 * event data for a PASS instance.
 */
typedef struct qapi_BLE_PASS_Event_Data_s
{
   /**
    * Event type used to determine the appropriate union member of
    * the Event_Data field to access.
    */
   qapi_BLE_PASS_Event_Type_t Event_Data_Type;

   /**
    * Total size of the data contained in the event.
    */
   uint16_t                   Event_Data_Size;
   union
   {
      /**
       * PASS Read CCCD event data.
       */
      qapi_BLE_PASS_Read_Client_Configuration_Data_t   *PASS_Read_Client_Configuration_Data;

      /**
       * PASS Write CCCD event data.
       */
      qapi_BLE_PASS_Client_Configuration_Update_Data_t *PASS_Client_Configuration_Update_Data;

      /**
       * PASS Ringer Control Point event data.
       */
      qapi_BLE_PASS_Ringer_Control_Command_Data_t      *PASS_Ringer_Control_Command_Data;
   }
   /**
    * Event data.
    */
   Event_Data;
} qapi_BLE_PASS_Event_Data_t;

#define QAPI_BLE_PASS_EVENT_DATA_SIZE                    (sizeof(qapi_BLE_PASS_Event_Data_t))
/**<
 * Size of the #qapi_BLE_PASS_Event_Data_t structure.
 */

/**
 * @brief
 * This declared type represents the Prototype Function for a
 * PASS Event Callback. This function will be called whenever a define
 * PASS Event occurs within the Bluetooth Protocol Stack that is
 * specified with the specified Bluetooth Stack ID.
 *
 * @details
 * The event information is passed to the user in an qapi_BLE_PASS_Event_Data_t
 * structure. This structure contains all the information about the
 * event that occurred.
 *
 * The caller should use the contents of the PASS Event Data
 * only in the context of this callback. If the caller requires
 * the data for a longer period of time, the callback function
 * must copy the data into another data buffer.
 *
 * This function is guaranteed not to be invoked more than once
 * simultaneously for the specified installed callback (i.e,. this
 * function does not have be reentrant). It should be noted, however,
 * that if the same callback is installed more than once, the
 * callbacks will be called serially. Because of this, the processing
 * in this function should be as efficient as possible.
 *
 * It should also be noted that this function is called in the Thread
 * Context of a Thread that the user does not own. Therefore, processing
 * in this function should be as efficient as possible (this argument holds
 * anyway because another PASS Event will not be processed while this
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
 * @param[in]  PASS_Event_Data       Pointer to a structure that contains
 *                                   information about the event that has
 *                                   occurred.
 *
 * @param[in]  CallbackParameter     User-defined value that will be
 *                                   received with the PASS Event data.
 *
 * @return None.
 */
typedef void (QAPI_BLE_BTPSAPI *qapi_BLE_PASS_Event_Callback_t)(uint32_t BluetoothStackID, qapi_BLE_PASS_Event_Data_t *PASS_Event_Data, uint32_t CallbackParameter);

   /* PASS server API.                                                  */

/**
 * @brief
 * Opens a PASS server on a specified Bluetooth Stack.
 *
 * @details
 * Only one PASS server may be open at a time, per the Bluetooth Stack ID.
 *
 * All Client Requests will be dispatch to the EventCallback function
 * that is specified by the second parameter to this function.
 *
 * @param[in]  BluetoothStackID     Unique identifier assigned to this
 *                                  Bluetooth Protocol Stack via a
 *                                  call to qapi_BLE_BSC_Initialize().
 *
 * @param[in]  EventCallback        Callback function that is registered
 *                                  to receive events that are associated
 *                                  with the specified service.
 *
 * @param[in]  CallbackParameter    User-defined parameter that will be
 *                                  passed back to the user in the callback
 *                                  function.
 *
 * @param[out]  ServiceID           Unique GATT service ID of the
 *                                  registered PASS service returned from the
 *                                  qapi_BLE_GATT_Register_Service() API.
 *
 * @return      Positive, nonzero if successful. The return value will
 *              be the Service Instance ID of the PASS server that was successfully
 *              opened on the specified Bluetooth Stack ID. This is the value
 *              that should be used in all subsequent function calls that
 *              require an Instance ID.
 *
 * @return      An error code if negative; one of the following values:
 *              @par
 *                 QAPI_BLE_PASS_ERROR_INSUFFICIENT_RESOURCES \n
 *                 QAPI_BLE_PASS_ERROR_INVALID_PARAMETER \n
 *                 QAPI_BLE_BT_GATT_ERROR_INVALID_SERVICE_TABLE_FORMAT \n
 *                 QAPI_BLE_BT_GATT_ERROR_INSUFFICIENT_RESOURCES \n
 *                 QAPI_BLE_BT_GATT_ERROR_INVALID_PARAMETER \n
 *                 QAPI_BLE_BT_GATT_ERROR_INVALID_BLUETOOTH_STACK_ID \n
 *                 QAPI_BLE_BT_GATT_ERROR_NOT_INITIALIZED
 */
QAPI_BLE_DECLARATION int QAPI_BLE_BTPSAPI qapi_BLE_PASS_Initialize_Service(uint32_t BluetoothStackID, qapi_BLE_PASS_Event_Callback_t EventCallback, uint32_t CallbackParameter, uint32_t *ServiceID);

/**
 * @brief
 * Opens a PASS server on a specified Bluetooth Stack with
 * the ability to control the location of the service in the GATT datapasse.
 *
 * @details
 * Only one PASS server may be open at a time, per the Bluetooth Stack ID.
 *
 * All Client Requests will be dispatch to the EventCallback function
 * that is specified by the second parameter to this function.
 *
 * @param[in]      BluetoothStackID      Unique identifier assigned to
 *                                       this Bluetooth Protocol Stack
 *                                       via a call to
 *                                       qapi_BLE_BSC_Initialize().
 *
 * @param[in]      EventCallback         Callback function that is
 *                                       registered to receive events
 *                                       that are associated with the
 *                                       specified service.
 *
 * @param[in]      CallbackParameter     User-defined parameter that
 *                                       will be passed back to the user
 *                                       in the callback function.
 *
 * @param[out]     ServiceID             Unique GATT Service ID of the
 *                                       registered service returned
 *                                       from
 *                                       qapi_BLE_GATT_Register_Service()
 *                                       API.
 *
 * @param[in,out]  ServiceHandleRange    Pointer to a Service Handle
 *                                       Range structure that, on input,
 *                                       can be used to control the
 *                                       location of the service in the
 *                                       GATT database, and on output,
 *                                       returns the handle range that
 *                                       the service is using in the GATT
 *                                       database.
 *
 * @return      Positive, nonzero if successful. The return value will
 *              be the Service Instance ID of the PASS server that was successfully
 *              opened on the specified Bluetooth Stack ID. This is the value
 *              that should be used in all subsequent function calls that
 *              require Instance ID.
 *
 * @return      An error code if negative; one of the following values:
 *              @par
 *                 QAPI_BLE_PASS_ERROR_INSUFFICIENT_RESOURCES \n
 *                 QAPI_BLE_PASS_ERROR_INVALID_PARAMETER \n
 *                 QAPI_BLE_BT_GATT_ERROR_INVALID_SERVICE_TABLE_FORMAT \n
 *                 QAPI_BLE_BT_GATT_ERROR_INSUFFICIENT_RESOURCES \n
 *                 QAPI_BLE_BT_GATT_ERROR_INVALID_PARAMETER \n
 *                 QAPI_BLE_BT_GATT_ERROR_INVALID_BLUETOOTH_STACK_ID \n
 *                 QAPI_BLE_BT_GATT_ERROR_NOT_INITIALIZED
 */
QAPI_BLE_DECLARATION int QAPI_BLE_BTPSAPI qapi_BLE_PASS_Initialize_Service_Handle_Range(uint32_t BluetoothStackID, qapi_BLE_PASS_Event_Callback_t EventCallback, uint32_t CallbackParameter, uint32_t *ServiceID, qapi_BLE_GATT_Attribute_Handle_Group_t *ServiceHandleRange);

/**
 * @brief
 * Cleans up and frees all resources
 * associated with a PASS Service Instance.
 *
 * @details
 * After this function is called, no other PASS service function can be
 * called until after a successful call to the qapi_BLE_PASS_Initialize_Service()
 * function is performed.
 *
 * @param[in]  BluetoothStackID    Unique identifier assigned to this
 *                                 Bluetooth Protocol Stack via a
 *                                 call to qapi_BLE_BSC_Initialize().
 *
 * @param[in]  InstanceID          Service instance ID to close.
 *                                 This is the value that was returned
 *                                 from the qapi_BLE_PASS_Initialize_Service()
 *                                 function.
 *
 * @return      Zero if successful.
 *
 * @return      An error code if negative; one of the following values:
 *              @par
 *                 QAPI_BLE_PASS_ERROR_INVALID_PARAMETER \n
 *                 QAPI_BLE_PASS_ERROR_INVALID_INSTANCE_ID
 */
QAPI_BLE_DECLARATION int QAPI_BLE_BTPSAPI qapi_BLE_PASS_Cleanup_Service(uint32_t BluetoothStackID, uint32_t InstanceID);

/**
 * @brief
 * Queries the number of attributes
 * that are contained in the PASS service that is registered with a
 * call to qapi_BLE_PASS_Initialize_Service() or
 * qapi_BLE_PASS_Initialize_Service_Handle_Range().
 *
 * @return   Positive, nonzero, number of attributes that will be
 *           registered by a PASS service instance.
 *
 * @return   Zero on failure.
 */
QAPI_BLE_DECLARATION unsigned int QAPI_BLE_BTPSAPI qapi_BLE_PASS_Query_Number_Attributes(void);

/**
 * @brief
 * Sets the Alert Status on the
 * specified PASS instance.
 *
 * @param[in]  BluetoothStackID    Unique identifier assigned to this
 *                                 Bluetooth Protocol Stack via a
 *                                 call to qapi_BLE_BSC_Initialize().
 *
 * @param[in]  InstanceID          Service instance ID to close.
 *                                 This is the value that was returned
 *                                 from either of
 *                                 the qapi_BLE_PASS_Initialize_XXX()
 *                                 functions.
 *
 * @param[in]  AlertStatus         Enumeration for the Alert Status
 *                                 that will be set if this function is
 *                                 successful.
 *
 * @return      Zero if successful.
 *
 * @return      An error code if negative; one of the following values:
 *              @par
 *                 QAPI_BLE_PASS_ERROR_INVALID_PARAMETER \n
 *                 QAPI_BLE_PASS_ERROR_INVALID_INSTANCE_ID
 */
QAPI_BLE_DECLARATION int QAPI_BLE_BTPSAPI qapi_BLE_PASS_Set_Alert_Status(uint32_t BluetoothStackID, uint32_t InstanceID, qapi_BLE_PASS_Alert_Status_t AlertStatus);

/**
 * @brief
 * Queries the Alert Status on the
 * specified PASS Instance.
 *
 * @param[in]  BluetoothStackID    Unique identifier assigned to this
 *                                 Bluetooth Protocol Stack via a
 *                                 call to qapi_BLE_BSC_Initialize().
 *
 * @param[in]  InstanceID          Service instance ID to close.
 *                                 This is the value that was returned
 *                                 from either of
 *                                 the qapi_BLE_PASS_Initialize_XXX()
 *                                 functions.
 *
 * @param[out]  AlertStatus        Pointer that will hold the Alert
 *                                 Status currently set for the PASS
 *                                 server if this function is successful.
 *
 * @return      Zero if successful.
 *
 * @return      An error code if negative; one of the following values:
 *              @par
 *                 QAPI_BLE_PASS_ERROR_INVALID_PARAMETER \n
 *                 QAPI_BLE_PASS_ERROR_INVALID_INSTANCE_ID
 */
QAPI_BLE_DECLARATION int QAPI_BLE_BTPSAPI qapi_BLE_PASS_Query_Alert_Status(uint32_t BluetoothStackID, uint32_t InstanceID, qapi_BLE_PASS_Alert_Status_t *AlertStatus);

/**
 * @brief
 * Sets the Ringer Setting on the
 * specified PASS Instance.
 *
 * @param[in]  BluetoothStackID    Unique identifier assigned to this
 *                                 Bluetooth Protocol Stack via a
 *                                 call to qapi_BLE_BSC_Initialize().
 *
 * @param[in]  InstanceID          Service instance ID to close.
 *                                 This is the value that was returned
 *                                 from either of
 *                                 the qapi_BLE_PASS_Initialize_XXX()
 *                                 functions.
 *
 * @param[in]  RingerSetting       Enumeration for the Ringer
 *                                 Setting that will be set if this
 *                                 function is successful.
 *
 * @return      Zero if successful.
 *
 * @return      An error code if negative; one of the following values:
 *              @par
 *                 QAPI_BLE_PASS_ERROR_INVALID_PARAMETER \n
 *                 QAPI_BLE_PASS_ERROR_INVALID_INSTANCE_ID
 */
QAPI_BLE_DECLARATION int QAPI_BLE_BTPSAPI qapi_BLE_PASS_Set_Ringer_Setting(uint32_t BluetoothStackID, uint32_t InstanceID, qapi_BLE_PASS_Ringer_Setting_t RingerSetting);

/**
 * @brief
 * Queryies the Ringer Setting on the
 * specified PASS Instance.
 *
 * @param[in]  BluetoothStackID    Unique identifier assigned to this
 *                                 Bluetooth Protocol Stack via a
 *                                 call to qapi_BLE_BSC_Initialize().
 *
 * @param[in]  InstanceID          Service instance ID to close.
 *                                 This is the value that was returned
 *                                 from either of
 *                                 the qapi_BLE_PASS_Initialize_XXX()
 *                                 functions.
 *
 * @param[out]  RingerSetting      A pointer that will hold the Ringer
 *                                 Setting currently set for the PASS server
 *                                 if this function is successful.
 *
 * @return      Zero if successful.
 *
 * @return      An error code if negative; one of the following values:
 *              @par
 *                 QAPI_BLE_PASS_ERROR_INVALID_PARAMETER \n
 *                 QAPI_BLE_PASS_ERROR_INVALID_INSTANCE_ID
 */
QAPI_BLE_DECLARATION int QAPI_BLE_BTPSAPI qapi_BLE_PASS_Query_Ringer_Setting(uint32_t BluetoothStackID, uint32_t InstanceID, qapi_BLE_PASS_Ringer_Setting_t *RingerSetting);

/**
 * @brief
 * Provides a mechanism for a PASS server to
 * successfully respond to a received read client configuration request.
 *
 * @details
 * Possible Events:
 *
 *    QAPI_BLE_ET_PASS_SERVER_READ_CLIENT_CONFIGURATION_REQUEST_E
 *
 * @param[in]  BluetoothStackID        Unique identifier assigned to this
 *                                     Bluetooth Protocol Stack via a
 *                                     call to qapi_BLE_BSC_Initialize().
 *
 * @param[in]  InstanceID              Service instance ID to close.
 *                                     This is the value that was returned
 *                                     from the qapi_BLE_PASS_Initialize_Service()
 *                                     function.
 *
 * @param[in]  TransactionID           Transaction ID of the original
 *                                     read request. This value was
 *                                     received in the
 *                                     QAPI_BLE_ET_PASS_SERVER_READ_CLIENT_CONFIGURATION_REQUEST_E
 *                                     event.
 *
 * @param[in]  NotificationsEnabled    Specifies whether the PASS client
 *                                     has successfully enabled
 *                                     notifications.
 *
 * @return      Zero if successful.
 *
 * @return      An error code if negative; one of the following values:
 *              @par
 *                 QAPI_BLE_PASS_ERROR_INVALID_PARAMETER \n
 *                 QAPI_BLE_PASS_ERROR_INVALID_INSTANCE_ID \n
 *                 QAPI_BLE_BT_GATT_ERROR_NOT_INITIALIZED \n
 *                 QAPI_BLE_BT_GATT_ERROR_INVALID_TRANSACTION_ID
 */
QAPI_BLE_DECLARATION int QAPI_BLE_BTPSAPI qapi_BLE_PASS_Read_Client_Configuration_Response(uint32_t BluetoothStackID, uint32_t InstanceID, uint32_t TransactionID, boolean_t NotificationsEnabled);

/**
 * @brief
 * Sends a
 * notification for an LNS characteristic to a remote LNS client.
 *
 * @param[in]  BluetoothStackID    Unique identifier assigned to this
 *                                 Bluetooth Protocol Stack via a
 *                                 call to qapi_BLE_BSC_Initialize().
 *
 * @param[in]  InstanceID          Service instance ID to close.
 *                                 This is the value that was returned
 *                                 from either of
 *                                 the qapi_BLE_LNS_Initialize_XXX()
 *                                 functions.
 *
 * @param[in]  ConnectionID        GATT connection ID of the LNS
 *                                 client that will receive the
 *                                 notification.
 *
 * @param[in]  CharacteristicType    Enumeration that identifies the
 *                                   PASS characteristic that will be
 *                                   notified to the PASS client.
 *
 * @return      Positive nonzero if successful (represents the
 *              length of the notification).
 *
 * @return      An error code if negative; one of the following values:
 *              @par
 *                 QAPI_BLE_LNS_ERROR_INVALID_PARAMETER \n
 *                 QAPI_BLE_LNS_ERROR_INVALID_INSTANCE_ID \n
 *                 QAPI_BLE_GATT_ERROR_INVALID_CONNECTION_ID
 */
QAPI_BLE_DECLARATION int QAPI_BLE_BTPSAPI qapi_BLE_PASS_Send_Notification(uint32_t BluetoothStackID, uint32_t InstanceID, uint32_t ConnectionID, qapi_BLE_PASS_Characteristic_Type_t CharacteristicType);

   /* PASS client API.                                                  */

/**
 * @brief
 * Parses a value received in an
 * indication from a remote PASS server, interpreting it as a PASS Alert
 * Status.
 *
 * @param[in]  ValueLength    Length of the value received from the
 *                            PASS server.
 *
 * @param[in]  Value          Value received from the
 *                            PASS server.
 *
 * @param[out]  AlertStatusResult    Pointer that will hold the
 *                                   decoded PASS Alert Status if this
 *                                   function is successful.
 *
 * @return      Zero if successful.
 *
 * @return      An error code if negative; one of the following values:
 *              @par
 *                 QAPI_BLE_PASS_ERROR_INVALID_PARAMETER \n
 *                 QAPI_BLE_PASS_ERROR_MALFORMATTED_DATA
 */
QAPI_BLE_DECLARATION int QAPI_BLE_BTPSAPI qapi_BLE_PASS_Decode_Alert_Status(uint32_t ValueLength, uint8_t *Value, qapi_BLE_PASS_Alert_Status_t *AlertStatusResult);

/**
 * @brief
 * Parses a value received in an
 * indication from a remote PASS server, interpreting it as a PASS Ringer
 * Setting.
 *
 * @param[in]  ValueLength    Length of the value received from the
 *                            PASS server.
 *
 * @param[in]  Value          Value received from the
 *                            PASS server.
 *
 * @param[out]  RingerSetting    Pointer that will hold the  decoded
 *                               PASS Ringer Setting if this function is
 *                               successful.
 *
 * @return      Zero if successful.
 *
 * @return      An error code if negative; one of the following values:
 *              @par
 *                 QAPI_BLE_PASS_ERROR_INVALID_PARAMETER \n
 *                 QAPI_BLE_PASS_ERROR_MALFORMATTED_DATA
 */
QAPI_BLE_DECLARATION int QAPI_BLE_BTPSAPI qapi_BLE_PASS_Decode_Ringer_Setting(uint32_t ValueLength, uint8_t *Value, qapi_BLE_PASS_Ringer_Setting_t *RingerSetting);

/**
 * @brief
 * Formats the PASS Ringer
 * Control Point command into a user-specified buffer for a GATT write
 * request that will be sent to the PASS server.
 *
 * @details
 * The BufferLength parameter must be large enough to hold the formatted
 * command.
 *
 * @param[in]  RingerControlCommand    Enumeration for the command
 *                                     that will be formatted into the
 *                                     user-specified buffer.
 *
 * @param[in]  BufferLength            Length of the user-specified
 *                                     buffer.
 *
 * @param[out] Buffer                  Uuser-specified buffer that
 *                                     will hold the formatted data if
 *                                     this function is successful.
 *
 * @return      The number of bytes formatted into the buffer if successful.
 *
 * @return      An error code if negative; one of the following values:
 *              @par
 *                 QAPI_BLE_PASS_ERROR_INVALID_PARAMETER
 */
QAPI_BLE_DECLARATION int QAPI_BLE_BTPSAPI qapi_BLE_PASS_Format_Ringer_Control_Command(qapi_BLE_PASS_Ringer_Control_Command_t RingerControlCommand, uint32_t BufferLength, uint8_t *Buffer);

/**
 * @}
 */

#endif

