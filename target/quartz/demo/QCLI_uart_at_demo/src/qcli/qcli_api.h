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

#ifndef __QCLI_API_H__  // [
#define __QCLI_API_H__

/*-------------------------------------------------------------------------
 * Include Files
 *-----------------------------------------------------------------------*/

#include "qcli.h"
#include "qurt_error.h"
#include "qapi_types.h"
#include "qurt_mutex.h"
#include "qurt_signal.h"

/*-------------------------------------------------------------------------
 * Preprocessor Definitions and Constants
 *-----------------------------------------------------------------------*/

/**
   This definition determines the size of the input buffer for CLI
   commansd. It effectively controls the maximum length of a command and
   its parameters.
*/
#define MAXIMUM_QCLI_COMMAND_STRING_LENGTH                              (256)

/**
   This definition determines the maximum number of parameters that can be
   provided. Note that this may also include paramters used to navigete
   into groups.
*/
#define MAXIMUM_NUMBER_OF_PARAMETERS                                    (17)

/**
   This definition determines the size of the buffer used for formatted
   messages to the console when using QCLI_Printf.
*/
#define MAXIMUM_PRINTF_LENGTH                                           (1024)

/**
   This definition determines the maximum number of command threads that
   the QCLI allows to be running at a time.
*/
#define MAXIMUM_THREAD_COUNT                                            (5)

/**
   This definition determines the size of the stack (in bytes) that is used
   for command threads.
*/
#define THREAD_STACK_SIZE                                               (3072)

/**
   This definition indicates if received characters should be echoed to
   the console.
*/
#define ECHO_CHARACTERS                                                 (true)

/**
   This defintion determines the index that is used for the first command
   in a command list.  Typically this will be eiher 0 or 1.
*/
#define COMMAND_START_INDEX                                             0


#define TAKE_LOCK(__lock__)                                             ((qurt_mutex_lock_timed(&(__lock__), QURT_TIME_WAIT_FOREVER)) == QURT_EOK)
#define RELEASE_LOCK(__lock__)                                          do { qurt_mutex_unlock(&(__lock__)); } while(0)

/**
   This enumeration represents the valid error codes that can be returned
   by the command functions.  They represent a success, a general error or
   a usage error.
*/
typedef enum
{
   QCLI_STATUS_SUCCESS_E,
   QCLI_STATUS_ERROR_E,
   QCLI_STATUS_USAGE_E
} QCLI_Command_Status_t;

/*-------------------------------------------------------------------------
 * Type Declarations
 *-----------------------------------------------------------------------*/
/**
   This type represents a group handle.
*/
typedef void *QCLI_Group_Handle_t;

/**
   This structure contains the information for a single parameter entered
   into the command line.  It contains the string value (as entered), the
   integer value (if the string could be successfully converted) and a
   boolean flag which indicates if the integer value is valid.
*/
typedef struct QCLI_Parameter_s
{
   char    *String_Value;
   int32_t  Integer_Value;
   qbool_t  Integer_Is_Valid;
} QCLI_Parameter_t;

/**
   @brief Type which represents the format of a function which processes
          commands from the CLI.

   @param Parameter_Count indicates the number of parameters that were
          specified to the CLI for the function.
   @param Parameter_List is the list of parameters specified to the CLI
          for the function.

   @return
    - QCLI_STATUS_SUCCESS_E if the command executed successfully.
    - QCLI_STATUS_ERROR_E if the command encounted a general error. Note
      that the CLI currently doesn't take any action for this error.
    - QCLI_STATUS_USAGE_E indicates that the parameters passed to the CLI
      were not correct for the command.  When this error code is returned,
      the CLI will display the usage message for the command.
*/
typedef QCLI_Command_Status_t (*QCLI_Command_Function_t)(uint32_t Parameter_Count, QCLI_Parameter_t *Parameter_List);

/**
   This structure represents the information for a single command in a
   command list.
*/
typedef struct QCLI_Command_s
{
   QCLI_Command_Function_t  Command_Function; /** The function that will be called when the command is executed from the CLI. */
   qbool_t                  Start_Thread;     /** A flag which indicates if the command should start on its own thread.       */
   const char              *Command_String;   /** The string representation of the function.                                  */
   const char              *Usage_String;     /** The usage string for the command.                                           */
   const char              *Description;      /** The description string for the commmand.                                    */
} QCLI_Command_t;

/**
   The following structure represents a command group that can be
   registered with the CLI.
*/
typedef struct QCLI_Command_Group_s
{
   const char           *Group_String;   /** The string representation of the group. */
   uint32_t              Command_Count; /** The number of commands in the group.    */
   const QCLI_Command_t *Command_List;   /** The list of commands for the group.     */
} QCLI_Command_Group_t;

/**
   This structure represents a command group list entry.
*/
typedef struct Group_List_Entry_s
{
   const QCLI_Command_Group_t *Command_Group;         /**< The command group information. */
   struct Group_List_Entry_s  *Parent_Group;          /**< the parent group for this subgroup. */
   struct Group_List_Entry_s  *Subgroup_List;         /**< The list of subgroups registerd for this group. */
   struct Group_List_Entry_s  *Next_Group_List_Entry; /**< The next entry in the list. */
} Group_List_Entry_t;

/**
   This structure contains the information needed for starting a command
   thread.
*/
typedef struct Thread_Info_s
{
   qurt_signal_t         Thread_Ready_Event; /**< Event which indicates the thread no longer needs this information structure. */
   uint32_t              Command_Index;      /**< The index of the command that will be executed. */
   const QCLI_Command_t *Command;            /**< The command that will be executed. */
   uint32_t              Parameter_Count;    /**< The number of parameters specified for the command. */
   QCLI_Parameter_t     *Parameter_List;     /**< The list of paramters for the command. */
} Thread_Info_t;

/**
   This structure contains the context information for the QCLI module.
*/
typedef struct QCLI_Context_s
{
   Group_List_Entry_t  Root_Group;                                           /**< The root of the group menu structure. */
   Group_List_Entry_t *Current_Group;                                        /**< The current group. */
   Group_List_Entry_t *Executing_Group;                                      /**< Group of currently executing command. */

   uint32_t            Input_Length;                                         /**< The length of the current console input string. */
   char                Input_String[MAXIMUM_QCLI_COMMAND_STRING_LENGTH + 1]; /**< Buffer containing the current console input string. */
   QCLI_Parameter_t    Parameter_List[MAXIMUM_NUMBER_OF_PARAMETERS + 1];     /**< List of parameters for input command. */

   uint32_t            Thread_Count;                                         /**< THe number of command threads that are currently running. */
   Thread_Info_t       Thread_Info;                                          /**< Information structure for passing information to command threads. */
   qurt_mutex_t        CLI_Mutex;                                            /**< The Mutex used to protect shared resources of the module. */

   char                Printf_Buffer[MAXIMUM_PRINTF_LENGTH];                 /**< The buffer used for formatted output strings. */
   QCLI_Group_Handle_t Current_Printf_Group;                                 /**< The group handle that was last passed to QCLI_Printf(). */
   qbool_t             Printf_New_Line;                                      /**< Indicates that a newline should be displayed if a printf changes groups. */
} QCLI_Context_t;

/*-------------------------------------------------------------------------
 * Function Declarations and Documentation
 *-----------------------------------------------------------------------*/

/**
   @brief This function is used to register a command group with the CLI.

   @param Parent_Group is the group which this group should be registerd
          under as a subgroup.  If this parameter is NULL, then the group
          will be registered at the top level.
   @param Command_Group is the command group to be registered.  Note that
          this function assumes the command group information will be
          constant and simply stores a pointer to the data.  If the command
          group and its associated information is not constant, its memory
          MUST be retained until the command is unregistered.

   @return
    - THe handle for the group that was added.
    - NULL if there was an error registering the group.
*/
QCLI_Group_Handle_t QCLI_Register_Command_Group(QCLI_Group_Handle_t Parent_Group, const QCLI_Command_Group_t *Command_Group);

/**
   @brief This function is used to usregister a command group from the CLI.

   @param Group_Handle is the handle for the group to be unregistered.
          This will be the value returned form
          QCLI_Register_Command_Group() when the function was registered.
          Note that if the specified group has subgroups, they will be
          unregistred as well.
*/
void QCLI_Unregister_Command_Group(QCLI_Group_Handle_t Group_Handle);

/**
   @brief This function prints the prompt to the console.

   This provides a means to re-display the prompt after printing data to
   the console from an asynchronous function such as a callback or seperate
   command thread.
*/
void QCLI_Display_Prompt(void);

/**
   @brief This function prints a formated string to the CLI.

   Note that this function will also replace newline uint8_tacters ('\n') with
   the string specified by PAL_OUTPUT_END_OF_LINE_STRING.

   @param Format is the formatted string to be printed.
   @param ... is the variatic parameter for the format string.
*/
void QCLI_Printf(const char *format, ...);

#endif   // ] #ifndef __QCLI_API_H__

