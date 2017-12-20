/***********************************************************************
 *
 * MODULE NAME:    le_cts_app.c
 * PROJECT CODE:   CEVA Low Energy Single Mode
 * DESCRIPTION:    Implementation of the Services required for Simple Server App and HID App.
 * MAINTAINER:     Gary Fleming
 * CREATION DATE:  June 2012
 *
 *
 * LICENSE:
 *     This source code is copyright (c) 2011-2012 Ceva Inc.
 *     All rights reserved.
 *
 ***********************************************************************/

// Only required for Terasic Demo of SM Peripheral
//#include "system.h"
#include "sys_types.h"
#include "sys_features.h"
#include "hc_const.h"

#include "hw_lc.h"
#include "hw_le_lc.h"
#include <stdio.h>

#if (PRH_BS_CFG_SYS_EMBEDDED_PROFILES==1)
#if ((PRH_BS_CFG_SYS_LE_SMP_INCLUDED==1) && (PRH_BS_CFG_SYS_LE_GATT_INCLUDED==1) && (PRH_BS_CFG_SYS_LE_GAP_INCLUDED==1))

#include "le_connection.h"
#include "le_att.h"
#include "le_gatt_server.h"
#include "le_gap.h"


#include "hc_const.h"
#include "hc_event_gen.h"
#include "tra_queue.h"
#include "bt_timer.h"
#include "sys_mmi.h"
#include "debug.h"
#include "le_gap_const.h"

#include "LE_link_layer.h"
#if (PRH_BS_CFG_SYS_LE_SMP_INCLUDED==1)
#include "le_smp.h"
#endif
#if (PRH_BS_CFG_SYS_LE_GAP_INCLUDED==1)
#include "le_gap.h"
#endif
#if (PRH_BS_CFG_SYS_LE_L2CAP_INCLUDED==1)
#include "le_l2cap.h"
#endif

#include "le_cts_app.h"
#include "BKAPI.h"


/********************************************************************************
 * APPLICATION EVENT IDS
 * These are used for encoding Events which will be encoded for transmission over
 * the Serial to the Applications
 ********************************************************************************/
/* GAP Event Id */
#define APP_ALERTING_EVENT                               0x01
#define APP_CONNECT_EVENT                                0x02
#define APP_DISCONNECT_EVENT                             0x03
#define APP_CONNECTABLE_EVENT                            0x04
/* Scan Service Event Id */
#define APP_SCAN_INTERVAL_INDICATION                     0x05

/* Time Services Event Ids */
#define APP_CURRENT_TIME_UPDATE_INDICATION               0x06
#define APP_LOCAL_TIME_INFORMATION_UPDATE_INDICATION     0x07
#define APP_REFERENCE_TIME_INFORMATION_UPDATE_INDICATION 0x08
#define APP_DATE_TIME_WITH_DST_UPDATE_INDICATION         0x09

/* HID Service Event Ids */
#define APP_HID_PROTOCOL_MODE_INDICATION                 0x0A
#define APP_HID_CONTROL_POINT_INDICATION                 0x0B
#define APP_HID_BOOT_KEYBOARD_OUTPUT_INDICATION          0x0C
#define APP_HID_REPORT_OUTPUT_INDICATION                 0x0D

#define APP_HID_BOOT_KEYBOARD_INPUT_INDICATION           0x0E
#define APP_HID_BOOT_MOUSE_INPUT_INDICATION              0x0F
#define APP_HID_REPORT_FEATURE_INDICATION                0x10
#define APP_HID_REPORT_INPUT_INDICATION                  0x11

/* HRS */
#define APP_HRS_CONTROL_POINT_INDICATION                 0x12

/* HT  */
#define APP_HT_MEASUREMENT_INTERVAL_INDICATION           0x13

/* CSC  */
#define APP_CSC_REPORT_CONTROL_POINT_INDICATION          0x14

/* RSC */
#define APP_RSC_REPORT_CONTROL_POINT_INDICATION          0x15

/* BG */
#define APP_BG_REPORT_CONTROL_POINT_INDICATION           0x16

/*************************************************************
 * APPLICATION COMMAND IDs
 ************************************************************/
/* HID Commands */
#define HID_APP_UPDATE_BOOT_KEYBOARD_INPUT  0x01
#define HID_APP_UPDATE_BOOT_MOUSE_INPUT     0x02
#define HID_APP_UPDATE_BOOT_KEYBOARD_OUTPUT 0x03
#define HID_APP_UPDATE_HID_INFO             0x04
#define HID_APP_UPDATE_REPORT_MAP           0x05
#define HID_APP_UPDATE_REPORT_INPUT         0x06
#define HID_APP_UPDATE_REPORT_OUTPUT        0x07
#define HID_APP_UPDATE_REPORT_FEATURE       0x08


/* CTS Commands */
#define CTS_APP_UPDATE_CURRENT_TIME                0x01
#define CTS_APP_UPDATE_LOCAL_TIME                  0x02
#define CTS_APP_UPDATE_REFERENCE_TIME_INFORMATION  0x03
#define CTS_APP_WRITE_CURRENT_TIME_NOTIFICATIONS   0x04

/* NDST Command */
#define NDCS_APP_UPDATE_CURRENT_TIME 0x01

/* Misc Service Commands */

#define APP_INIT                      0x00  // Init the Application
#define TPS_APP_UPDATE_POWER_LEVEL    0x01  // Update Tx Power
#define BAS_APP_UPDATE_BATTERY        0x02  // Update Battery Level
#define IAS_APP_CANCEL_ALERTING       0x03  // Cancle Alerting
#define GAP_APP_MAKE_CONNECTABLE      0x04  // Make Device Connectable
#define SCPS_APP_REQUEST_SCAN_REFRESH 0x05  // Request refresh of Scan Parameters from Peer
#define APP_DISCONNECT                   0x06  // Disconnect the Application
#define APP_LINK_DISCONNECT           0x07  // Disconnect Current Link

/* HRS Commands */

#define HRS_APP_UPDATE_HEART_RATE_MEASUREMENT 0x01
#define HRS_APP_UPDATE_BODY_SENSOR_LOCATION   0x02

/* Health Thermometer */
#define HT_APP_UPDATE_TEMP_MEASUREMENT         0x01
#define HT_APP_UPDATE_TEMP_TYPE                0x02
#define HT_APP_UPDATE_INT_TEMP_MEASUREMENT     0x03
#define HT_APP_UPDATE_MEASUREMENT_INTERVAL     0x04

/* CSC Commands */
#define CSC_APP_UPDATE_CSC_MEASUREMENT              0x01
#define CSC_APP_UPDATE_CSC_FEATURE_SUPPORTED        0x02
#define CSC_APP_UPDATE_CSC_SENSOR_LOCATION          0x03

/* RSC Commands */
#define RSC_APP_UPDATE_RSC_MEASUREMENT              0x01
#define RSC_APP_UPDATE_RSC_FEATURE_SUPPORTED        0x02
#define RSC_APP_UPDATE_RSC_SENSOR_LOCATION          0x03


#define BP_APP_UPDATE_BP_MEASUREMENT                0x01
#define BP_APP_UPDATE_INT_CUFF_PRESSURE_MEASUREMENT 0x02
#define BP_APP_UPDATE_BP_FEATURE                    0x03

#define BG_APP_UPDATE_GLUCOSE_MEASUREMENT            0x01
#define BG_APP_UPDATE_MEASUREMENT_CONTEXT            0x02
#define BG_APP_UPDATE_GLUCOSE_FEATURE                0x03
#define BG_APP_UPDATE_RECORD_ACCESS_CONTROL_POINT    0x04 // Not Sure if Needed

/************************************************************************
 * Handles for Attributes and Characteristic in the Local GATT Server Database
 * This are hardcoded as the layout of our local GATT Database is know.
 *
 * NOTE !! If the handles in the local GATT Database are changed then
 * this values may also need to change.
 *************************************************************************/
/* HID */
#define HID_PROTOCOL_MODE_HANDLE            0x00D2
#define HID_BOOT_KEYBOARD_INPUT_HANDLE      0x00E1
#define HID_BOOT_MOUSE_INPUT_HANDLE         0x00E6
#define HID_BOOT_KEYBOARD_OUTPUT_HANDLE     0x00E4
#define HID_HID_INFO_HANDLE                 0x00E9
#define HID_REPORT_MAP_HANDLE               0x00DE
#define HID_REPORT_INPUT_HANDLE             0x00D4
#define HID_REPORT_OUTPUT_HANDLE            0x00D8
#define HID_REPORT_FEATURE_HANDLE           0x00DB
#define HID_CONTROL_POINT_HANDLE            0x00EB


/* Heart Rate Service */
#define HRS_MEASUREMENT_HANDLE               0x00F2
#define HRS_SENSOR_LOCATION_HANDLE           0x00F5
#define HRS_CONTROL_POINT_HANDLE             0x00F7

/* Health Thermometer */

#define HT_TEMP_MEASUREMENT_HANDLE           0x0102
#define HT_TEMP_TYPE_HANDLE                  0x0105
#define HT_INT_TEMP_MEASUREMENT_HANDLE       0x0107
#define HT_MEASUREMENT_INTERVAL_HANDLE       0x010A

/* Current Time and NDST */
#define CTS_SERVER_CURRENT_TIME_HANDLE                0x00A2
#define CTS_SERVER_LOCAL_TIME_HANDLE                  0x00A5
#define CTS_SERVER_REFERENCE_TIME_INFORMATION_HANDLE  0x00A7
#define NDCS_SERVER_TIME_HANDLE                       0x00C2

/* Battery and Tx Power */
#define BAS_SERVER_BATTERY_LEVEL_HANDLE               0x00A2
#define TPS_SERVER_TX_POWER_HANDLE                    0x0082
#define NOTIFY_PHONE_HANDLE                           0x00B2


/* Alerting */
#define LINK_LOSS_SERVER_IMMEDIATE_ALERT_HANDLE       0x0072
#define IAS_SERVER_IMMEDIATE_ALERT_HANDLE             0x0052
/* Scan Service */
#define SCPS_SERVER_SCAN_REFRESH_HANDLE               0x0094
#define SCPS_SERVER_SCAN_INTERVAL_HANDLE              0x0092

/* Cycling Speed and Cadence Handles */
#define CSC_MEASUREMENT_HANDLE                        0x0112
#define CSC_SUPPORTED_FEATURES_HANDLE                 0x0115
#define CSC_SENSOR_LOCATION_HANDLE                    0x0117
#define CSC_SC_CONTROL_POINT_HANDLE                   0x0119
#define CSC_SC_CONTROL_POINT_CCC_HANDLE               0x011A

/* Running Speed and Cadence Handles */
#define RSC_MEASUREMENT_HANDLE                        0x0122
#define RSC_SUPPORTED_FEATURES_HANDLE                 0x0125
#define RSC_SENSOR_LOCATION_HANDLE                    0x0127
#define RSC_SC_CONTROL_POINT_HANDLE                   0x0129
#define RSC_SC_CONTROL_POINT_CCC_HANDLE               0x012A

/* Blood Pressure */
#define BP_MEASUREMENT_HANDLE                         0x0132
#define BP_INT_CUFF_PRESSURE_MEASUREMENT_HANDLE       0x0135
#define BP_FEATURE_HANDLE                             0x0138

/* Blood Glucose */
#define BG_MEASUREMENT_HANDLE                         0x0142
#define BG_MEASUREMENT_CONTEXT_HANDLE                 0x0145
#define BG_FEATURE_HANDLE                             0x0148

/************************************************************
 * Simple States to track device discovery.
 * This is only relevant for the CTS and NDST services, as the peer
 * device is GATT Server for these services.
 ************************************************************/
#define W4_CTS_DISCOVERY                  0x01
#define W4_CTS_CHAR_DISCOVERY            0x02
#define W4_NDST_DISCOVERY                0x03



#define  NO_APPLICATION                             0x00
#define  PROXIMITY_FIND_ME_APP                        0x01
#define  HID_APPLICATION                            0x04

#define NUM_SENSOR_LOCATIONS 15

// Maps the HID Characteristic to Handles
const u_int16 hid_handles[HID_APP_UPDATE_REPORT_FEATURE+1]
= { 0x00, // Invalid
    HID_BOOT_KEYBOARD_INPUT_HANDLE,
    HID_BOOT_MOUSE_INPUT_HANDLE,
    HID_BOOT_KEYBOARD_OUTPUT_HANDLE,
    HID_HID_INFO_HANDLE,
    HID_REPORT_MAP_HANDLE,
    HID_REPORT_INPUT_HANDLE,
    HID_REPORT_OUTPUT_HANDLE,
    HID_REPORT_FEATURE_HANDLE
};


u_int8 SENSOR_LOCATIONS[NUM_SENSOR_LOCATIONS] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14};
const u_int16 ADC_table[11] = {0x3EC,0x380,0x31C,0x2B8,0x254,0x1F1,0x18c,0x129,0x0C8,0x062,0x001};//for battery detect function
/* 1.5v,1.35v,1.2v,1.05v,0.9v,0.75v,0.6v,0.45v,0.3v,0.15v,0v */
static u_int16 Battery_level_adc;//for battery detect function

#if 0
typedef struct t_App_Control
{
    u_int8 Flash_Type;
    u_int8 Alert_Level;
    u_int32 TAPP_Alerting_Timer;
    u_int32 TAPP_HT_Update_Timer;
    u_int8 HT_Updated;
    float New_Temp;
    float Prev_Temp;
} t_App_Control;
#endif

#define HEART_RATE_SAMPLES 4

#define APP_SIMPLE 0x02
#define APP_HEALTH_SERVICES 0x03
#define APP_HID_SERVICE 0x04
#define APP_FITNESS_SERVICES 0x05
#define APP_FIND_ME_TI_LIKE 0x80 //charles

//t_App_Control LEapp_Config = {0x01,0x02,BT_CLOCK_MAX_TIMEOUT,BT_CLOCK_MAX_TIMEOUT,0,98.4,0.0};//org
#if (APP_DEFINE == APP_FIND_ME)
t_App_Control LEapp_Config = {0x01,0x02,BT_CLOCK_MAX_TIMEOUT,BT_CLOCK_MAX_TIMEOUT,BT_CLOCK_MAX_TIMEOUT};//charles modify
//u_int8 Flash_Type,u_int8 Alert_Level,TAPP_Alerting_Timer,u_int32 TAPP_BatteryDetect_Timer,TAPP_L2CAP_PARA_UPDATE_Timer
#endif
#if 0 //do not used
void User_App_Alerting(u_int8 alerting);
void User_App_Flash_7Segment(u_int8 level);
void User_App_Flash_Leds(u_int8 ON_OFF);
void User_App_Flash_Leds_OFF(void);
#endif

void LE_App_Generate_Event(u_int8 event,u_int8 para1,u_int8* p_data);

u_int8 App_State = 0;
u_int16 LE_App_ACL_Handle = 0;
u_int16 LE_CTS_Peer_Server_Current_Time_Handle = 0;
u_int16 LE_CTS_Peer_Server_Current_Time_End_Handle = 0;
u_int16 LE_CTS_Peer_Server_Local_Time_Info_Handle = 0;
u_int16 LE_CTS_Peer_Server_Reference_Time_Handle = 0;
u_int16 LE_NDST_Peer_Server_Time_Handle = 0;
u_int16 LE_CTS_Peer_Server_Current_Time_CC_Descr_Handle = 0;
u_int16 CTS_start_handle = 0;
u_int16 CTS_end_handle = 0;
u_int16 NDST_start_handle = 0;
u_int16 NDST_end_handle = 0;
u_int16 LE_Pending_Read_Handle=0;
u_int16 LE_Pending_Write_Handle=0;

u_int8 LE_CTS_Peer_Server_Current_Time_Valid = 0;
u_int8 LE_CTS_Peer_Server_Local_Time_Info_Valid = 0;
u_int8 LE_CTS_Peer_Server_Reference_Time_Valid = 0;
u_int8 LE_CTS_Peer_Server_Current_Time_CCC_Handle = 0;
u_int8 LE_NDST_Peer_Server_Time_Valid=0;

void User_App_Update_LCD(void);
u_int8 App_Config_App_Present = 0;
u_int8 App_Config_Type = NO_APPLICATION;

u_int16 g_pulse_count=0;

static u_int16 handle = 1;
static u_int16  notify_handle;

/*
 * Callback functions which are registered with the lower layers.
 */
void LE_APP_GATT_Event_Handler(u_int8 eventCode, t_gatt_event_params* p_gatt_event_params);
void LE_APP_GATT_Command_Complete(u_int8 operation_id,t_gatt_command_complete_params* p_params);
void LE_APP_GAP_Connection_Complete(u_int8 eventCode, t_gap_event_params* p_gap_event_params);

#if (PRH_BS_CFG_SYS_LE_GAP_RECONNECTION_ADDRESS_SUPPORTED==1)
static u_int8 reconnection_addr[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
#endif

u_int8 Service_Change_Range[4] = {0x01,0x00,0x4F,0x01};

static u_int8 gap_name_array[] = "BleSeriaPort";//"SecuriTag";//"A-CEVA";
//static u_int8 gap_name_array[] = "零速争霸";//"SecuriTag";//"A-CEVA";
/*
static u_int8 gap_name_array[] = {0xe9,0x9B,0xB6,  //零
                                                       0xe9,0x80,0x9f, //速
                                                       0xe4,0xBA,0x89, //争
                                                       0xe9,0x9C,0xB8,  //霸
                                                       ' '
                                                     };
                                                     */

//static u_int8 gap_appearance_array[2] = {0x03,0xC1} ; // Hid Keyboardstatic 
static u_int8 gap_appearance_array[2] = {0x00,0x02} ; // Generic Tag
static u_int8 gap_connection_params[8] = {24,0,240,0,0,0,250,0};

u_int8 manufacturer_name_string[] = "BEKEN";
u_int8 model_number_string[7] = "BT 4.0";
u_int8 serial_number_string[11] = "12x07x2012";
u_int8 fw_revision_string[5] = "01.1";
u_int8 sw_revision_string[5] = "01.1";
u_int8 hw_revision_string[5] = "SM-1";
u_int8 system_id[8] = {0xDE,0xBC,0x9A,0xFE,0xFF,0x56,0x34,0x12};
u_int8 pnp_id[7] = {0x01,0x0E,0x00,0x12,0x34,0x01,0x67};
u_int8 IEEE_Reg_Cert[4] = {0x89,0xAB,0xCD,0xEF};

static u_int8 alert_level_array[1] = {0x00};
static u_int8 tx_power_array[1] = {0};
static u_int8 tx_power_format[7];
static u_int8 link_loss_alert_level_array[1] = {0x00};
static u_int8 battery_level[1] = {100};

#if 1
static u_int8 Notify_Phone_Alert[20];//02 : turn on phone alert , 00 : turn off phone alert
static u_int8 Range_Alarm_Enable[1] = {0};
static u_int8 Beep_range[1] = {0};
static u_int8 Reserve0[1]={0};
static u_int8 Reserve1[1]={0};
static u_int8 Reserve2[1]={0};
static u_int8 Range_Alarm_Enable_str[18] = {"Range Alarm Enable"};
static u_int8 Notify_Phone_description_CCC[2] = {0x01,0x00};
static u_int8 Notify_Phone_Alert_str[] = {"BleSeriaPort"};
static u_int8 Beep_range_str[10] = {"Beep Range" };
static u_int8 Reserve0_str[9] = {"Reserve 0"};
static u_int8 Reserve1_str[9] = {"Reserve 1"};
static u_int8 Reserve2_str[9] = {"Reserve 2"};
#endif

static u_int8 battery_service_format[7];

/* Client configuration characteristics - CCC */
u_int8 GATT_service_change_CCC[2] = {0x00,0x00};
u_int8 TX_Power_Level_CCC[2] = {0x00,0x00};
u_int8 Battery_Service_CCC[2] = {0x00,0x00};
u_int8 Scan_Parameters_CCC[2] = {0x00,0x00};
u_int8 Current_Time_CCC[2] = {0x00,0x00};
u_int8 NDST_Current_Time_CCC[2] = {0x00,0x00};
u_int8 hid_input_report_CCC[2] = {0x00,0x00};
u_int8 hid_boot_keyboard_input_report_CCC[2] = {0x00,0x00};
u_int8 hid_boot_mouse_input_report_CCC[2] = {0x00,0x00};
u_int8 heart_rate_measurement_CCC[2] = {0x00,0x00};
u_int8 ht_temperature_measurement_CCC[2] = {0x00,0x00};
u_int8 ht_int_temperature_measurement_CCC[2] = {0x00,0x00};
u_int8 ht_measurement_interval_CCC[2] = {0x00,0x00};
u_int8 csc_measurement_CCC[2] = {0x00,0x00};
u_int8 csc_control_point_CCC[2] = {0x00,0x00};
u_int8 rsc_measurement_CCC[2] = {0x00,0x00};
u_int8 rsc_control_point_CCC[2] = {0x00,0x00};
u_int8 bp_measurement_CCC[2] = {0x00,0x00};
u_int8 bp_int_cuff_measurement_CCC[2] = {0x00,0x00};
u_int8 bg_measurement_CCC[2] = {0x00,0x00};
u_int8 bg_measurement_context_CCC[2] = {0x00,0x00};
u_int8 bg_control_point_CCC[2] = {0x00,0x00};

const u_int8 Simple_App_Adv_Ind[26] = {
    0x02,0x01,0x06 /*GAP_LE_BR_EDR_NOT_SUPPORTED*/, /* Flags Field */
    0x02,0x0A,0x00,                        /* Tx Power    */
    0x05,0x08,'C','E','V','A',             /* Local name  */
    0x0D,0x02,0x00,0x18,0x01,0x18,0x02,0x18,0x03,0x18,0x04,0x18,0x05,0x18};
const u_int8  Find_ME_adv_data[14]= {
    /* Flags Field */ 0x02,0x01,0x06 /*GAP_LE_BR_EDR_NOT_SUPPORTED*/,
    /* Tx Power    */ 0x02,0x0A,0x00,
    /* Services //0x07 data length,0x02 16-bits UUIDS incomplete */ 0x07,0x02,0x03,0x18,0x02,0x18,0x04,0x18
    /* 0x1803 LINK LOSS */
    /* 0x1802 Immediate Alert */
    /* 0x1804 Tx Power */
};

const u_int8 Simple_App_Scan_Resp[21] = {
    0x02,0x11,0x02,      /*Le Host+No OOB+ Public */
    0x03,0x16,0x00,0x18, /*UUID for GAP */
    0x03,0x16,0x01,0x18, /*UUID for GATT*/
    0x09,0x02,0x06,0x18,0x07,0x18,0x0a,0x18,0x0f,0x18 };
// static u_int8  Find_ME_scan_resp_data[12] =
//                        {
//                                                   0x0B,0x09,'A','N','T','I','L','O','S','S','B','L'   //C_Chip sample
//                        };
const u_int8  Find_ME_scan_resp_data[12] = //Find_ME_scan_resp_data[0] : Length of data
    //Find_ME_scan_resp_data[1]= 9 : Complete Name see GAP profile assign number
    {
        0x0B,0x09,'S','e','c','u','r','i','T','a','g','D',
    };
#if 0
const u_int8 Health_App_Adv_Ind[26] = {
    0x02,0x01,0x06 /*GAP_LE_BR_EDR_NOT_SUPPORTED*/, /* Flags Field */
    0x02,0x0A,0x00,                        /* Tx Power    */
    0x05,0x08,'C','E','V','A',             /* Local name  */
    //  0x0D,0x02,0x00,0x18,0x01,0x18,0x02,0x18,0x03,0x18,0x04,0x18,0x05,0x18};
    0x0D,0x02,0x00,0x18,0x01,0x18,0x08,0x18,0x10,0x18,0x09,0x18,0x0D,0x18};
const u_int8 Health_App_Scan_Resp[21] = {
    0x02,0x11,0x02,      /*Le Host+No OOB+ Public */
    0x03,0x16,0x00,0x18, /*UUID for GAP */
    0x03,0x16,0x01,0x18, /*UUID for GATT*/
    0x09,0x02,0x00,0x18,0x09,0x18,0x0d,0x18,0x10,0x18 };


const u_int8 Fitness_App_Adv_Ind[26] = {
    0x02,0x01,0x06 /*GAP_LE_BR_EDR_NOT_SUPPORTED*/, /* Flags Field */
    0x02,0x0A,0x00,                        /* Tx Power    */
    0x05,0x08,'C','E','V','A',             /* Local name  */
    0x0D,0x02,0x00,0x18,0x01,0x18,0x02,0x18,0x03,0x18,0x04,0x18,0x05,0x18};

const u_int8 Fitness_App_Scan_Resp[21] = {
    0x02,0x11,0x02,      /*Le Host+No OOB+ Public */
    0x03,0x16,0x00,0x18, /*UUID for GAP */
    0x03,0x16,0x01,0x18, /*UUID for GATT*/
    0x05,0x02,0x08,0x18,0x09,0x18 };

const u_int8 HID_App_Adv_Ind[26] = {
    0x02,0x01,0x06 /*GAP_LE_BR_EDR_NOT_SUPPORTED*/, /* Flags Field */
    0x02,0x0A,0x00,                        /* Tx Power    */
    0x05,0x08,'C','E','V','A',             /* Local name  */
    0x0D,0x02,0x00,0x18,0x01,0x18,0x02,0x18,0x03,0x18,0x04,0x18,0x12,0x18};

const u_int8 HID_App_Scan_Resp[21] = {
				   0x02,0x11,0x02,      /*Le Host+No OOB+ Public */
				   0x03,0x16,0x00,0x18, /*UUID for GAP */
				   0x03,0x16,0x01,0x18, /*UUID for GATT*/
		           0x03,0x02,0x12,0x18 };
#endif

const u_int8 SerialPort_App_Adv_Ind[] = {
    // flags
    0x02,   // length of this data
    GAP_ADTYPE_FLAGS,
    GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
    0x03, 0x02, 0xE0, 0xFF};

//const u_int8 SerialPort_App_Scan_Resp[] = {
u_int8 SerialPort_App_Scan_Resp[] = {
    0x0E,
    GAP_ADTYPE_LOCAL_NAME_COMPLETE,
    'B','l','e','S','e','r','i','a','l','P','o','r','t'};
		 /*
    0xe9,0x9B,0xB6,  //零
    0xe9,0x80,0x9f, //速
    0xe4,0xBA,0x89, //争
    0xe9,0x9C,0xB8,  //霸
    ' '
    };
 */
    

//static u_int8 gap_name_array[] = "零速争霸";//"SecuriTag";//"A-CEVA";
   
extern u_int16 LMutils_Get_Uint16(t_p_pdu p_pdu);
extern u_int32 LMutils_Get_Uint32(t_p_pdu p_u_int8);
extern void LMutils_Set_Uint16(t_p_pdu p_buffer, u_int16 value_16_bit);
extern volatile u_int8 ADC_finish;//for battery detect function
extern volatile u_int8 ADC_captrue;
extern  volatile u_int8 RF_test_mode_enable;

//extern  t_LE_Config LE_config;

u_int16 Get_Battery(void)//for battery detect function
{
    return Battery_level_adc;
}
void Save_Power_on_Battery(u_int8 value)//for battery detect function
{
    battery_level[0] = value;
}
void Save_Battery_level(void)//for battery detect function
{
    Battery_level_adc = REG_ADC_DAT&0x03FF;
}
void Restart_Battery_Detect_Timer(void)//for battery detect function
{
    u_int32 current_clk = HW_Get_Native_Clk_Avoid_Race();
    LEapp_Config.TAPP_BatteryDetect_Timer = current_clk+32000;
}
void LE_APP_Init(void)
{
	u_int8 i; //Kevin Add @ 20171220
    // Initially for fully embedded Proximity, IAS and Find Me
    App_Config_App_Present = 1;
    LEapp_Config.TAPP_BatteryDetect_Timer = BT_CLOCK_MAX_TIMEOUT;//for find me
    //    App_Config_Type = PROXIMITY_FIND_ME_APP;//charles remove
    
#if 0 //do not used
    User_App_Flash_Leds_OFF();
    LEapp_Config.Flash_Type = 0x00;
#endif

    LEapp_Config.Alert_Level = 0x00;
#if 0 //do not used
    LEapp_Config.TAPP_Alerting_Timer = BT_CLOCK_MAX_TIMEOUT;
#endif

#if (APP_DEFINE == APP_HT_DEVICE)
    LEapp_Config.HT_Updated = 0;
    LEapp_Config.TAPP_HT_Update_Timer = BT_CLOCK_MAX_TIMEOUT;
    LEapp_Config.TAPP_HT_Measurement_Interval  = BT_CLOCK_MAX_TIMEOUT;
#endif

#if (APP_DEFINE == APP_HR_DEVICE)
    LEapp_Config.TAPP_HR_Update_Timer = BT_CLOCK_MAX_TIMEOUT;
#endif

    LEapp_Config.TAPP_L2CAP_PARA_UPDATE_Timer = BT_CLOCK_MAX_TIMEOUT;
    //.TAPP_DISCOVERABLE_Timer = BT_CLOCK_MAX_TIMEOUT;
    LEapp_Config.TAPP_POWERON_STANDBY_Timer = BT_CLOCK_MAX_TIMEOUT;
    {
#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==1)
        volatile u_int32 *pulseOut = (u_int32*) PULSE_TIMER_OUT8BIT_BASE;
        // Init the Pulse Rate/ Heart Rate Time.
        LEapp_Config.HR_Sample_Number = 0;
        LEapp_Config.HR_Updated = 0;
        LEapp_Config.HR_Aggretate_Time_for_10pulse = 0;
        LEapp_Config.HR_BPM = 0x0;

        *pulseOut = 1;
        *pulseOut = 0;
#endif

#if (APP_DEFINE == APP_HR_DEVICE)
        LEapp_Config.TAPP_HR_Update_Timer = HW_Get_Native_Clk_Avoid_Race() + 32 /* 100ms interval */;
#endif
    }
    // Register Callbacks for GATT Event and GATT Command Complete handling
    GATT_Register_Callbacks((void*)LE_APP_GATT_Event_Handler,(void*)0x00);///* do not used /*  revision 335 change */ */
    GAP_Register_Callbacks((void*)LE_APP_GAP_Connection_Complete, (void*)0x00);

/*
    LEsmp_Configure_Security(SMP_DisplayOnly, 0x00, SMP_AUTHREQ_BONDING_MITM, 
                                            0x10, 0x07, 0x03, 
                                            0x03, 201609); //123456
 */
                                            
    LEsmp_Configure_Security(0x03 /* io_capability - No Input/No Output */, 
		                                   0x00 /* oob_flag - Data Not Present */,
		                                   0x00 /* auth_req - No Bonding / No MITM */,
                                                 0x10 /* max_encry_key_size*/,
                                                 0x07 /* min_encry_key_size */,
                                                 0x01 /* initiator_key_dist - Enc Key */,
                                                 0x01 /* responder_key_dist - Enc Key */,
											00
					);      
	
#ifdef ADD_CHANGE_BTNAME
    //driver_mfc_read_buf(NVR_SPACE, 0, g_sys_vars->bt_name, 16); //Kevin Remove @ 20171220

    for(i = 0; i < 16; i++)
    {
        if(g_sys_vars->bt_name[i] != 0xFF)
        {

            SerialPort_App_Scan_Resp[i + 2] = g_sys_vars->bt_name[i];

            gap_name_array[i] = g_sys_vars->bt_name[i];
        }
        else
        {
            break;
        }

    }

#endif

    GAP_Set_AD_Data(sizeof(SerialPort_App_Adv_Ind), (u_int8*)SerialPort_App_Adv_Ind);
    GAP_Set_ScanResp_Data(sizeof(SerialPort_App_Scan_Resp), (u_int8*)SerialPort_App_Scan_Resp);
    //LE_Simple_APP_Info(GAP_APP_MAKE_CONNECTABLE,0,0);
    if(RF_test_mode_enable == 0)
    {
		GAP_Set_Advertising_Interval(160, 160);
            GAP_Discoverable_Mode(GAP_GENERAL_DISCOVERABLE,ADV_IND);
    }
    else
    {
        GAP_Discoverable_Mode(GAP_NON_DISCOVERABLE,ADV_IND);
    }
}

/**************************************************************************
 * FUNCTION :- LE_CTS_APP_Info
 *
 * DESCRIPTION
 * This is the handler for Commands from the Simple Server Application, which
 * are specific to this allocation (GATT interaction for CTS characteristics).
 * It it triggered by the hc_cmd_disp.c on receipt of an Application command over
 * the Serial/UART interface.
 *
 * It supports the initiating of a read of the following characteristic from the peer :
 *   CURRENT_TIME
 *   LOCAL_TIME
 *   REFERENCE TIME INFORMATION
 *
 * The response to each read will result in an update of the peers characteric value
 * displayed on the application.
 *
 * In addition this also supports the Enabling/Disabling of automatic update from the peer
 * for its CURRENT TIME characteristic on the peer by writeing to the relevant
 * Client Characteristic Configuration (CCC) Attribute.
 *
 * INPUTS
 * u_int8 command :-  Instruction on what has to be performed.
 * u_int8 command_val :- Only used in the Characteristic Write - to carry Enable/Disable Flag
 * u_int8* p_command_params :- Current UNUSED.
 *************************************************************************/


t_error LE_CTS_APP_Info(u_int8 command,u_int8 command_val,u_int8* p_command_params)
{
    u_int8 dummy_uuid[2] = {0x00,0x00};
    switch(command)
    {

    case CTS_APP_UPDATE_CURRENT_TIME :
        LE_Pending_Read_Handle = LE_CTS_Peer_Server_Current_Time_Handle; //CTS_PEER_SERVER_CURRENT_TIME_HANDLE;
        GATT_Characteristic_Value_Read(LE_App_ACL_Handle,0,0,dummy_uuid,LE_CTS_Peer_Server_Current_Time_Handle, 0x00, 0x02 );
        break;

    case CTS_APP_UPDATE_LOCAL_TIME :
        LE_Pending_Read_Handle = LE_CTS_Peer_Server_Local_Time_Info_Handle; //CTS_PEER_SERVER_LOCAL_TIME_INFO_HANDLE;
        GATT_Characteristic_Value_Read(LE_App_ACL_Handle,0,0,dummy_uuid,LE_CTS_Peer_Server_Local_Time_Info_Handle, 0x00, 0x02 );
        break;

    case CTS_APP_UPDATE_REFERENCE_TIME_INFORMATION :
        LE_Pending_Read_Handle = LE_CTS_Peer_Server_Reference_Time_Handle; //CTS_PEER_SERVER_REFERENCE_TIME_HANDLE;
        GATT_Characteristic_Value_Read(LE_App_ACL_Handle,0,0,dummy_uuid,LE_CTS_Peer_Server_Reference_Time_Handle, 0x00, 0x02 );
        break;

    case CTS_APP_WRITE_CURRENT_TIME_NOTIFICATIONS :
        {
            u_int8 current_time_cc[2] = {0x00,0x00};

            LE_Pending_Write_Handle = LE_CTS_Peer_Server_Current_Time_CC_Descr_Handle;

            if (command_val == 0x01)
            {
                current_time_cc[0] = 0x01;
            }
            else
            {
                current_time_cc[0] = 0x00;
            }
            GATT_Client_Charactaristic_Descriptor_Write(LE_App_ACL_Handle,LE_CTS_Peer_Server_Current_Time_CC_Descr_Handle, 2, current_time_cc);
        }
        break;
    }

    return NO_ERROR;
}

/**************************************************************************
 * FUNCTION :- LE_NCST_APP_Info
 *
 * DESCRIPTION
 * This is the handler for Commands from the he Simple Server Application, which
 * are specific to this allocation (GATT interaction for the Next DST service change
 * characteristic.
 *
 * It it triggered by the hc_cmd_disp.c on receipt of an Application command over
 * the Serial/UART interface.
 *
 * It supports the initiating of a read of the following characteristic from the peer :
 *   TIME WITH DST
 *
 * The response to  read will result in an update of the peers characteric value
 * displayed on the application.
 *
 * INPUTS
 * u_int8 command :-  Instruction on what has to be performed.
 * u_int8 command_val :- Currently UNUSED
 * u_int8* p_command_params :- Current UNUSED.
 *************************************************************************/


t_error LE_NCST_APP_Info(u_int8 command,u_int8 command_len,u_int8* p_command_params)
{
    u_int8 dummy_uuid[2] = {0x00,0x00};

    if (command == NDCS_APP_UPDATE_CURRENT_TIME)
    {
        LE_Pending_Read_Handle = LE_NDST_Peer_Server_Time_Handle;
        GATT_Characteristic_Value_Read(LE_App_ACL_Handle,0,0,dummy_uuid,LE_NDST_Peer_Server_Time_Handle, 0x00, 0x02 );
    }
    return NO_ERROR;
}

/**************************************************************************
 * FUNCTION :- LE_Simple_APP_Info
 *
 * DESCRIPTION
 * This is the handler for Commands which are common to many applications. It is
 * triggered by the hc_cmd_disp.c on receipt of an Application command over
 * the Serial/UART interface.
 *
 *
 * It supports the following :
 *     APP_INIT - Initialise the application.
 *  APP_DISCONNECT             - Disconnect the App from the LE stack - deregisters any callbacks.
 *  GAP_APP_MAKE_CONNECTABLE   - Calls GAP procedures to make device connectable.
 *  TPS_APP_UPDATE_POWER_LEVEL - Updates the Tx Power characteristic in the local DB.
 *  BAS_APP_UPDATE_BATTERY     - Updates the Battery Level characteristic in the local DB.
 *  IAS_APP_CANCEL_ALERTING    - Provides a hook from which platform specific procedures
 *                               can be called
 *  SCPS_APP_REQUEST_SCAN_REFRESH - Updates the Scan Refresh Characteristic in the local DB.
 *
 *
 * INPUTS
 * u_int8 command :-  Instruction on what has to be performed.
 * u_int8 command_len :- Length of data to be written to local DB
 * u_int8 param1 :-  A one byte parameters - to be used in DB writes.
 *************************************************************************/

t_error LE_Simple_APP_Info(u_int8 command,u_int8 command_len,u_int8 param1)
{
    return NO_ERROR;
}

/**************************************************************************
 * FUNCTION :- LE_HID_APP_Info
 *
 * DESCRIPTION
 * This is the handler for Commands which are specific to the HID Application.
 * All these commands perform a local write to a specific characteristic in the
 * local GATT DB Server. It is triggered by the hc_cmd_disp.c on receipt of an
 * Application command over the Serial/UART interface.
 *
 *
 * INPUTS
 * u_int8 command :-  Instruction on what has to be performed.
 * u_int8 command_len :- Length of data to be written to local DB
 * u_int8* p_pdu :-  The Data to be written.
 *************************************************************************/

t_error LE_HID_APP_Info(u_int8 command,u_int8 command_len,u_int8* p_pdu)
{

    return NO_ERROR;
}

/**************************************************************************
 * FUNCTION :- LE_HRS_APP_Info
 * ( Heart Rate Service )
 *
 * DESCRIPTION
 * This is the handler for Commands which are specific to the HRS Application.
 * All these commands perform a local write to a specific characteristic in the
 * local GATT DB Server. It is triggered by the hc_cmd_disp.c on receipt of an
 * Application command over the Serial/UART interface.
 *
 * INPUTS
 * u_int8 command :-  Instruction on what has to be performed.
 * u_int8 command_len :- Length of data to be written to local DB
 * u_int8* p_pdu :-  The Data to be written.
 **************************************************************************/

t_error LE_HRS_APP_Info(u_int8 command,u_int8 command_len,u_int8* p_pdu)
{

    return NO_ERROR;
}

t_error LE_HT_APP_Info(u_int8 command,u_int8 command_len,u_int8* p_pdu)
{

    return NO_ERROR;
}

/********************************************************
 * Arrays mapping CSC and RSC Characteristics to handles
 *
 */

const u_int16 CSC_Command_To_Handle_Map[CSC_APP_UPDATE_CSC_SENSOR_LOCATION+1] = {
    0x00,// INVALID
    CSC_MEASUREMENT_HANDLE,// CSC_APP_UPDATE_CSC_MEASUREMENT
    CSC_SUPPORTED_FEATURES_HANDLE, // CSC_APP_UPDATE_CSC_FEATURE_SUPPORTED
    CSC_SENSOR_LOCATION_HANDLE// CSC_APP_UPDATE_CSC_SENSOR_LOCATION
};

const u_int16 RSC_Command_To_Handle_Map[RSC_APP_UPDATE_RSC_SENSOR_LOCATION+1] = {
    0x00,// INVALID
    RSC_MEASUREMENT_HANDLE,// RSC_APP_UPDATE_RSC_MEASUREMENT
    RSC_SUPPORTED_FEATURES_HANDLE, // RSC_APP_UPDATE_RSC_FEATURE_SUPPORTED
    RSC_SENSOR_LOCATION_HANDLE// RSC_APP_UPDATE_RSC_SENSOR_LOCATION
};


t_error LE_CSC_APP_Info(u_int8 command,u_int8 command_len,u_int8* p_pdu)
{

    return NO_ERROR;
}

t_error LE_RSC_APP_Info(u_int8 command,u_int8 command_len,u_int8* p_pdu)
{

    return NO_ERROR;
}

t_error LE_BP_APP_Info(u_int8 command,u_int8 command_len,u_int8* p_pdu)
{

    return NO_ERROR;
}

t_error LE_BG_APP_Info(u_int8 command,u_int8 command_len,u_int8* p_pdu)
{

    return NO_ERROR;
}

/**************************************************************************
 * FUNCTION :- LE_App_Local_Event_Indication
 *
 * DESCRIPTION
 * This function is triggered on receipt of a Write_Indication Event/Callback from
 * the GATT layer.
 *
 * For each handle the local GATT DB is read to determine the new value which has
 * been written into the DB by the Peer. An Application Event is then generated to
 * inform the App/User of the new value.
 *
 * INPUTS
 * u_int16 attrib_handle - Handle which has been written to.
 * u_int8  status  :- Currently UNUSED
 *************************************************************************/

u_int8 LE_App_Local_Event_Indication(u_int16 attrib_handle, u_int8 status)
{
    u_int8* p_value_array;
    //u_int8 alert_level;
    u_int8 length;

    switch (attrib_handle)
    {
    case IAS_SERVER_IMMEDIATE_ALERT_HANDLE :
        GATTserver_Characteristic_Read_Local_Value(attrib_handle,&length,&p_value_array);
#if 0 // no used
        alert_level = p_value_array[0];
        User_App_Alerting(alert_level);
#endif
        break;


    case SCPS_SERVER_SCAN_INTERVAL_HANDLE :

        break;

    case HID_PROTOCOL_MODE_HANDLE :

        break;

    case HID_CONTROL_POINT_HANDLE :

        break;

    case HID_REPORT_OUTPUT_HANDLE :

        break;


    case HID_REPORT_FEATURE_HANDLE :

        break;

    case HID_REPORT_INPUT_HANDLE :

        break;

    case HID_BOOT_KEYBOARD_OUTPUT_HANDLE :

        break;

    case HID_BOOT_KEYBOARD_INPUT_HANDLE :

        break;

    case HID_BOOT_MOUSE_INPUT_HANDLE :

        break;

    case HRS_CONTROL_POINT_HANDLE :

        break;

    case HT_MEASUREMENT_INTERVAL_HANDLE :

        break;

    case CSC_SC_CONTROL_POINT_HANDLE :

        break;

    case RSC_SC_CONTROL_POINT_HANDLE :

        break;


    default :
        break;
    }
    return NO_ERROR;
}


/**************************************************************************
 * FUNCTION :- LE_APP_GATT_Event_Handler
 *
 * DESCRIPTION
 * This key function is used to handle all events raised by the GATT layer.
 *
 *
 *
 * GATT_VALUE_NOTIFICATION_EVENT :
 * This occurs when one of the Current Time Characteristics has been modified by
 * the peer and we have previously enable notifications by writing to it's CCC.
 * In this cases we construct an APP_INDICATION event using the information in the
 * notification and sent it to the application.
 *
 * GATT_READ_RESPONSE_EVENT
 * If a READ_RESPONSE_EVENT is recieved for a handle which is valid (i.e not zero).
 * The the payload in the READ_RESPONSE will be sent to the application in an
 * APP_INDICATION event.
 *
 * The READ_RESPONSE_EVENT is also used to trigger discovery of the peers CTS and NDST
 * handles.
 *
 * GATT_DISCOVER_SERVICES_BY_UUID_EVENT
 * This the response to the Discover Services by UUID request which was issued to determine the Handles
 * ranges in use of the CTS and NDST profiles.
 *
 * GATT_DISCOVER_CHARACTERISTICS_EVENT
 * This the response to the Discover Characteristics request which was issue to determine the values of
 * the handles for the Characterisitics of the CTS and NDST profiles.
 *
 * INPUTS
 * u_int8 eventCode              - Identifier for the Event
 * u_int8* p_gatt_event_params   - pointer to a structure of GATT event Parameters
 *
 *************************************************************************/


void LE_APP_GATT_Event_Handler(u_int8 eventCode, t_gatt_event_params* p_gatt_event_params)
{
    u_int16 attrib_handle=0;
    u_int8 length=0;
    u_int8* p_data = p_gatt_event_params->payload;
    u_int8 event;

    switch(eventCode)
    {
    case GATT_READ_RESPONSE_EVENT :
    case GATT_VALUE_NOTIFICATION_EVENT :
        // Need to Add Error Handling here !!!
        if (p_gatt_event_params->status==0x00)
        {
            if (eventCode == GATT_READ_RESPONSE_EVENT)
            {
                attrib_handle = LE_Pending_Read_Handle;
                p_data = p_gatt_event_params->payload;
                length = p_gatt_event_params->payload_length;
            }
            else if (p_gatt_event_params->att_handle == 0x00)
            {
                attrib_handle = HCIparam_Get_Uint16(p_gatt_event_params->payload);
                p_data = p_gatt_event_params->payload+2;
                length = p_gatt_event_params->payload_length-2;
            }
            else
            {
                attrib_handle = p_gatt_event_params->att_handle;
                p_data = p_gatt_event_params->payload;
                length = p_gatt_event_params->payload_length;
            }
        }
        else
        {
            // Need to determine what to do here
        }

        if (attrib_handle == LE_CTS_Peer_Server_Current_Time_Handle) //CTS_PEER_SERVER_CURRENT_TIME_HANDLE :
        {
            event = APP_CURRENT_TIME_UPDATE_INDICATION;
            LE_App_Generate_Event(event,length,p_data);

        }
        else if (attrib_handle == LE_CTS_Peer_Server_Local_Time_Info_Handle) // //CTS_PEER_SERVER_LOCAL_TIME_INFO_HANDLE :
        {
            event = APP_LOCAL_TIME_INFORMATION_UPDATE_INDICATION;
            LE_App_Generate_Event(event,length,p_data);

        }
        else if (attrib_handle == LE_CTS_Peer_Server_Local_Time_Info_Handle)//CTS_PEER_SERVER_LOCAL_TIME_INFO_HANDLE :
        {
            event = APP_LOCAL_TIME_INFORMATION_UPDATE_INDICATION;
            LE_App_Generate_Event(event,length,p_data);

        }
        else if (attrib_handle == LE_CTS_Peer_Server_Reference_Time_Handle) //CTS_PEER_SERVER_REFERENCE_TIME_HANDLE :
        {
            event = APP_REFERENCE_TIME_INFORMATION_UPDATE_INDICATION;
            LE_App_Generate_Event(event,length,p_data);

        }
        else if (attrib_handle == LE_NDST_Peer_Server_Time_Handle) //NDST_PEER_SERVER_TIME_HANDLE :
        {
            event = APP_DATE_TIME_WITH_DST_UPDATE_INDICATION;
            LE_App_Generate_Event(event,length,p_data);
        }

        if (( eventCode == GATT_READ_RESPONSE_EVENT ) && (p_gatt_event_params->status==0x00))
        {
            u_int8 uuid[2] = {0x00,0x00};

            if ((LE_Pending_Read_Handle == LE_CTS_Peer_Server_Current_Time_Handle) &&
                (LE_CTS_Peer_Server_Local_Time_Info_Valid == 0))
            {
                //
                LE_CTS_Peer_Server_Current_Time_Valid = 1;
                GATT_Characteristic_Value_Read(LE_App_ACL_Handle,0x00,0x00,uuid,LE_CTS_Peer_Server_Local_Time_Info_Handle,0x00,0x02);

                LE_Pending_Read_Handle = LE_CTS_Peer_Server_Local_Time_Info_Handle;
            }
            else if ((LE_Pending_Read_Handle == LE_CTS_Peer_Server_Local_Time_Info_Handle) &&
                     (LE_CTS_Peer_Server_Reference_Time_Valid == 0))
            {
                LE_CTS_Peer_Server_Local_Time_Info_Valid = 1;
                GATT_Characteristic_Value_Read(LE_App_ACL_Handle,0x00,0x00,uuid,LE_CTS_Peer_Server_Reference_Time_Handle,0x00,0x02);

                LE_Pending_Read_Handle = LE_CTS_Peer_Server_Reference_Time_Handle;
            }
            else if ((LE_Pending_Read_Handle == LE_CTS_Peer_Server_Reference_Time_Handle) &&
                     (LE_CTS_Peer_Server_Reference_Time_Valid == 0))
            {
                LE_CTS_Peer_Server_Reference_Time_Valid = 1;

                // Now all 3 chars have been read.
                // Should move on to discover the NDST.
                //    if (App_Config_App_Present)
                {
                    u_int8 UUID[2] = { 0x07, 0x18 };

                    GATT_Discover_Primary_Service(LE_App_ACL_Handle,0x00,UUID,0x02);

                    App_State = W4_NDST_DISCOVERY;
                }
            }
            else if ((LE_Pending_Read_Handle == LE_NDST_Peer_Server_Time_Handle) &&
                     ( LE_NDST_Peer_Server_Time_Valid == 0))
            {
                LE_NDST_Peer_Server_Time_Valid = 1;
            }

        }
        break;

    case GATT_DISCOVER_SERVICES_BY_UUID_EVENT :
        {
            u_int8* p_pdu = p_gatt_event_params->payload;
            if (App_State == W4_CTS_DISCOVERY)
            {
                // Num entries should only be 1 as there should be only one CTS service in a device.
                CTS_start_handle = (p_pdu[0] + (p_pdu[1]<<8));
                CTS_end_handle =  (p_pdu[2] + (p_pdu[3]<<8));

                //
                // Now Find the CTS Current Time Char Handle.

            }
            else if (App_State == W4_NDST_DISCOVERY)
            {
                NDST_start_handle = (p_pdu[0] + (p_pdu[1]<<8));
                NDST_end_handle =  (p_pdu[2] + (p_pdu[3]<<8));
            }
        }
        break;

    case GATT_DISCOVER_CHARACTERISTICS_EVENT :
        {
            u_int16 current_time_UUID = 0x2A2B;
            u_int16 local_time_UUID = 0x2A0F;
            u_int16 reference_time_UUID = 0x2A14;
            u_int16 NDST_time_UUID = 0x2A11;
            u_int8 i;
            u_int8 num_entries = p_gatt_event_params->payload_length/p_gatt_event_params->att_len;
            u_int8* p_pdu = p_gatt_event_params->payload;

            for(i=0;i<num_entries;i++)
            {
                u_int16 attrib_uuid = p_pdu[5] + (p_pdu[6]<<8);
                u_int16 attrib_value_handle = p_pdu[3] + (p_pdu[4]<<8);

                if (attrib_uuid == current_time_UUID)
                {
                    LE_CTS_Peer_Server_Current_Time_Handle = attrib_value_handle;
                    // NOTE !!!! TO DO
                    // TEMP !! we assume - that the CCC handle is directly after the Value handle
                    // This will not always be valid and in final version. We have to obtain
                    // the CCC handle by doing a characteristic Descriptor discovery for the
                    // CCC Attrib in the characteristic value range.

                    LE_CTS_Peer_Server_Current_Time_CC_Descr_Handle = attrib_value_handle + 1;
                }
                else if (attrib_uuid == local_time_UUID)
                {
                    LE_CTS_Peer_Server_Local_Time_Info_Handle = attrib_value_handle;
                }
                else if (attrib_uuid == reference_time_UUID)
                {
                    LE_CTS_Peer_Server_Reference_Time_Handle = attrib_value_handle;
                }
                else if (attrib_uuid == NDST_time_UUID)
                {
                    LE_NDST_Peer_Server_Time_Handle = attrib_value_handle;
                }
                p_pdu+=7;
            }

        }
        break;

    case GATT_WRITE_INDICATION_EVENT :
        LE_App_Local_Event_Indication(p_gatt_event_params->att_handle,p_gatt_event_params->status);
        break;

    }
}


/**************************************************************************
 * FUNCTION :- LE_App_Local_Disconnect_Indication
 *
 * DESCRIPTION
 * This the Callback function invoked by the Lower layers layer when a connection is disconnected
 * either by the peer or due to link loss.
 *
 * Since the 'Simple Server Application' encompasses the link loss profile, the
 * Link Loss IAS alert characteristic will be read from the local GATT DB and an alerting
 * event will be send to the application.
 *
 * For all applications a Disconnect Event will be sent to the application and the GAP
 * will be instructed to place the device in CONNECTABLE mode. A 'Connectable' event will
 * also be sent to the application.
 *************************************************************************/

u_int8 LE_App_Local_Disconnect_Indication(t_error reason)
{
#if 0
    u_int8* p_value_array;
    u_int8 alert_level;
    u_int8 length;
#endif
    /* u_int8 dummy_addr[6] = { 0x00,0x00,0x00,0x00,0x00,0x00 }; */
    u_int32 current_clk = HW_Get_Native_Clk_Avoid_Race();
#if (APP_DEFINE == APP_HT_DEVICE)
    LEapp_Config.Prev_Temp = 0xFFFF;
#endif

    if (!App_Config_App_Present)
        return 0;

#if (PRH_BS_CFG_SYS_LE_PERIPHERAL_DEVICE==1)
#if 0
    if (App_Config_App_Present && (App_Config_Type != HID_APPLICATION))
    {
        // Need to check the Alert Level in the Link Loss Profile on the Server
        // To determine if we should raise and Alerting Event
        GATTserver_Characteristic_Read_Local_Value(LINK_LOSS_SERVER_IMMEDIATE_ALERT_HANDLE,&length,&p_value_array);
        alert_level = p_value_array[0];

        if (alert_level != 0)
        {
            LE_App_Generate_Event(APP_ALERTING_EVENT,alert_level,0x00);
#if 0 // no used
            User_App_Alerting(alert_level);
#endif
        }
    }
#endif
    //Set_MMI_Cycle_time(25);
    //Set_MMI_Cycle_time(0xFFFF);
    //Set_MMI_State(MMI_DISCONNECTED_STATE);

    if(Get_MMI_State() != MMI_DEEPSLEEP_STATE)
    {
				Notify_Phone_description_CCC[0] = 0x01;
        Set_MMI_State(MMI_POWER_ON_STATE);//for C_Chip
        LE_App_Generate_Event(APP_DISCONNECT_EVENT,0x00,0x00);
        LEapp_Config.TAPP_BatteryDetect_Timer = BT_CLOCK_MAX_TIMEOUT;
        //LEapp_Config.TAPP_DISCOVERABLE_Timer = current_clk+16;//wait 5 msec
        //GAP_Discoverable_Mode(GAP_NON_DISCOVERABLE,ADV_IND);
        GAP_Discoverable_Mode(GAP_GENERAL_DISCOVERABLE,ADV_IND);
        //GAP_Connectable_Mode(0x02 /*GAP_UNDIRECTED_CONNECTABLE*/, 0x00 /*PUBLIC_ADDRESS*/, 0x00/*PUBLIC_ADDRESS*/,dummy_addr);
        //LE_App_Generate_Event(APP_CONNECTABLE_EVENT,0x00,0x00);
    }
#endif
    return NO_ERROR;
}

/**************************************************************************
 * FUNCTION :- LE_APP_GAP_Connection_Complete
 *
 * DESCRIPTION
 * This the Callback function invoked by the GAP layer when a connection complete has been
 * recieved from the lower layers and any necessary processing (i.e setup of an encrypted link)
 * is complete.
 *
 * This is mainly used by the CTS based app (Server Simple Service) to cancle alerting
 * (if the Proximity/Link Loss was in use). Also used to trigger Discovery for the handles
 * of the Timer Characteristics on the Peer device.
 *************************************************************************/
void LE_APP_GAP_Connection_Complete(u_int8 event,t_gap_event_params* p_event_params )
{
    /* u_int8 dummy_addr[6] = { 0x00,0x00,0x00,0x00,0x00,0x00 }; */
    u_int32 current_clk = HW_Get_Native_Clk_Avoid_Race();
    if (!App_Config_App_Present)
        return;

    if (p_event_params->status == NO_ERROR)
    {
#if (APP_DEFINE == APP_HT_DEVICE)
        if  (App_Config_Type == APP_HEALTH_SERVICES)
        {
            LEapp_Config.TAPP_HT_Update_Timer = HW_Get_Native_Clk_Avoid_Race() + 400;
            LEapp_Config.HT_Updated = 1;
        }
#endif
        LE_App_ACL_Handle = p_event_params->handle;
#if 0 // no used
        User_App_Alerting(0);
#endif
        //DEBUG_MSG(0x56);
        //DEBUG_MSG(0xfd);
        //DEBUG_MSG(App_Config_App_Present);
        //DEBUG_MSG(App_Config_Type);
        //DEBUG_MSG(p_event_params->status);

        

		
        if (App_Config_App_Present && (App_Config_Type != HID_APPLICATION) && (p_event_params->status == NO_ERROR))  //frank 160617
        {
            // On Connect Stop and Alerting
            //bprintf("9\r\n");
            //DEBUG_MSG(0xfc);
            LE_App_Generate_Event(APP_ALERTING_EVENT,0x00,0x00);
	     //DEBUG_MSG(current_clk);
            LEapp_Config.TAPP_L2CAP_PARA_UPDATE_Timer = current_clk+25600;//charles modify after connection complete wait 8 s to start l2cap parameter change
            //DEBUG_MSG(0xff);
	     //DEBUG_MSG((LEapp_Config.TAPP_L2CAP_PARA_UPDATE_Timer  & 0xff000000)>>24);
            //DEBUG_MSG((LEapp_Config.TAPP_L2CAP_PARA_UPDATE_Timer  & 0xff0000)>>16);
            //DEBUG_MSG((LEapp_Config.TAPP_L2CAP_PARA_UPDATE_Timer  & 0xff00)>>8);
	     //DEBUG_MSG(LEapp_Config.TAPP_L2CAP_PARA_UPDATE_Timer  & 0x00ff);
            
            LEapp_Config.TAPP_POWERON_STANDBY_Timer = BT_CLOCK_MAX_TIMEOUT;
            Restart_Battery_Detect_Timer();
            // Indicate to UI (on PC that connection has been setup
            LE_App_Generate_Event(APP_CONNECT_EVENT,0x00,0x00);

	     //LEsmp_Initiate_Security(0xFFFF, SMP_AUTHREQ_BONDING_MITM);//SMP_AUTHREQ_BONDING_NO_MITM  //frank 160719
	    //frank 170506
#if 0 // GF Temp 14 May 2013
            if (0) // Must Repair later
#else
                if (1)
#endif
                {
                    //    u_int8 UUID[2] = { 0x05, 0x18 };
                    //    GATT_Discover_Primary_Service(LE_App_ACL_Handle,0x00,UUID,0x02);

                    App_State = W4_CTS_DISCOVERY;
                }

        }
        else if (App_Config_App_Present && (App_Config_Type == HID_APPLICATION))
        {
            // Indicate to UI (on PC that connection has been setup
            LE_App_Generate_Event(APP_CONNECT_EVENT,0x00,0x00);
        }
    }
    else
    {
        // Connection Failure
        // Go Back Connectable..!!
        LEapp_Config.TAPP_BatteryDetect_Timer = BT_CLOCK_MAX_TIMEOUT;
        //LEapp_Config.TAPP_DISCOVERABLE_Timer = current_clk+TGAP_adv_fast_period*2;
        GAP_Discoverable_Mode(GAP_GENERAL_DISCOVERABLE,ADV_IND);
        //GAP_Connectable_Mode(0x02 /*GAP_UNDIRECTED_CONNECTABLE*/, 0x00 /*PUBLIC_ADDRESS*/, 0x00/*PUBLIC_ADDRESS*/,dummy_addr);
        LE_App_Generate_Event(APP_CONNECTABLE_EVENT,0x00,0x00);

    }
    return;
}
/**************************************************************************
 * FUNCTION :- LE_APP_GATT_Command_Complete
 *
 * DESCRIPTION
 * This the Callback function invoked by the GATT layer when a procedure is complete
 * It is used by the Current Time Application to allow the Discovery of the Handles for
 * Time Characteristics on the Peer Devices GATT DB.
 *
 * It is used in combination with 'LE_APP_GATT_Event_Handler' to progress through the
 * discovery state machine. Once discovery is complete it also writes to the Client
 * Charactaristic Configuration for the Current Time Characteristic
 *************************************************************************/


void LE_APP_GATT_Command_Complete(u_int8 operation_id,t_gatt_command_complete_params* p_params)
{
    u_int8 dummy_UUID[2] = { 0x00, 0x00 };

    switch(operation_id)
    {
    case 0x01 :// Primary Service Discovery :
        if (App_State == W4_CTS_DISCOVERY)
        {
            //
            // Now Find the CTS Current Time Char Handle.
            GATT_Characteristic_Discovery(LE_App_ACL_Handle, 0, dummy_UUID, CTS_start_handle, CTS_end_handle, 0x02);
        }
        else if (App_State == W4_NDST_DISCOVERY)
        {
            GATT_Characteristic_Discovery(LE_App_ACL_Handle, 0, dummy_UUID, NDST_start_handle, NDST_end_handle, 0x02);
        }
        break;


    case 0x03 : // GATT_CHARACTERISTIC_DISCOVERY :

        if (App_State == W4_CTS_DISCOVERY)
        {
            // Write the CC Descriptor for Current Time
            if (LE_CTS_Peer_Server_Current_Time_CC_Descr_Handle != 0x00)
            {
                u_int8 current_time_cc[2] = {0x01,0x00};
                GATT_Client_Charactaristic_Descriptor_Write(LE_App_ACL_Handle,LE_CTS_Peer_Server_Current_Time_CC_Descr_Handle, 1, current_time_cc);
            }

            LE_Pending_Read_Handle = LE_CTS_Peer_Server_Current_Time_Handle;
            // Now Read each of the CTS characteristics
            GATT_Characteristic_Value_Read(LE_App_ACL_Handle,0x00,0x00,dummy_UUID,LE_CTS_Peer_Server_Current_Time_Handle,0x00,0x02);
        }
        else if (App_State == W4_NDST_DISCOVERY)
        {
            LE_Pending_Read_Handle = LE_NDST_Peer_Server_Time_Handle;
            // Now Read each of the CTS characteristics
            GATT_Characteristic_Value_Read(LE_App_ACL_Handle,0x00,0x00,dummy_UUID,LE_NDST_Peer_Server_Time_Handle,0x00,0x02);

        }
        break;
    }
}
/**************************************************************************
 * FUNCTION :- LE_App_Generate_Event
 *
 * DESCRIPTION
 * Simple function for encoding Events which have to be sent to the Application
 * over the Serial/UART IF.
 *
 *************************************************************************/

void LE_App_Generate_Event(u_int8 event,u_int8 para1,u_int8* p_data)
{

    u_int8* p_buffer;
    u_int8  i;
    struct q_desc* qd;
    /*
     * Determine event length:
     *     2 bytes for event header
     *     1 byte for event
     *     1 byte for param
     */
    if (!App_Config_App_Present)
        return;

    if (event == APP_SCAN_INTERVAL_INDICATION)
    {
        qd=BTq_Enqueue(HCI_EVENT_Q, 0, (t_length)
                       (6+SIZE_OF_EVENT_HEADER ) );
    }
    else if (( event ==  APP_LOCAL_TIME_INFORMATION_UPDATE_INDICATION ) ||
             ( event == APP_CURRENT_TIME_UPDATE_INDICATION ) ||
             ( event == APP_REFERENCE_TIME_INFORMATION_UPDATE_INDICATION ) ||
             ( event == APP_DATE_TIME_WITH_DST_UPDATE_INDICATION) ||
             ( event == APP_HID_REPORT_OUTPUT_INDICATION) ||
             ( event == APP_HID_BOOT_KEYBOARD_OUTPUT_INDICATION)||
             ( event ==APP_HID_BOOT_KEYBOARD_INPUT_INDICATION)||
             ( event ==APP_HID_BOOT_MOUSE_INPUT_INDICATION)||
             ( event ==APP_HID_REPORT_FEATURE_INDICATION)||
             ( event ==APP_HID_REPORT_INPUT_INDICATION) ||
             ( event ==APP_HID_CONTROL_POINT_INDICATION) ||
             ( event ==APP_HRS_CONTROL_POINT_INDICATION) ||
             ( event ==APP_CSC_REPORT_CONTROL_POINT_INDICATION) ||
             ( event ==APP_RSC_REPORT_CONTROL_POINT_INDICATION) ||
             ( event ==APP_HID_PROTOCOL_MODE_INDICATION) ||
             ( event ==APP_HT_MEASUREMENT_INTERVAL_INDICATION)
             )
    {
        qd=BTq_Enqueue(HCI_EVENT_Q, 0, (t_length)
                       (para1+2+SIZE_OF_EVENT_HEADER ) );

    }
    else
    {
        qd=BTq_Enqueue(HCI_EVENT_Q, 0, (t_length)
                       (2+SIZE_OF_EVENT_HEADER ) );
    }


    if(qd)
    {
        p_buffer = qd->data;
    }
    else
    {
        return;
    }

    switch(event)
    {

    case APP_ALERTING_EVENT :
    case APP_CONNECT_EVENT :
    case APP_DISCONNECT_EVENT :
    case APP_CONNECTABLE_EVENT :
        p_buffer[2] = event;
        p_buffer[3] = para1;
        break;


    case APP_SCAN_INTERVAL_INDICATION :
        p_buffer[2] = event;
        p_buffer[3] = para1;
        for (i=0;i<para1;i++)
        {
            p_buffer[4+i] = p_data[i];
        };
        break;

    case APP_HID_CONTROL_POINT_INDICATION :
    case APP_HID_PROTOCOL_MODE_INDICATION :
    case APP_HID_REPORT_INPUT_INDICATION :
    case APP_HID_REPORT_FEATURE_INDICATION :
    case APP_HID_BOOT_MOUSE_INPUT_INDICATION :
    case APP_HID_BOOT_KEYBOARD_INPUT_INDICATION :
    case APP_HID_BOOT_KEYBOARD_OUTPUT_INDICATION :
    case APP_HID_REPORT_OUTPUT_INDICATION :

    case APP_LOCAL_TIME_INFORMATION_UPDATE_INDICATION :
    case APP_CURRENT_TIME_UPDATE_INDICATION :
    case APP_REFERENCE_TIME_INFORMATION_UPDATE_INDICATION :
    case APP_DATE_TIME_WITH_DST_UPDATE_INDICATION :
    case APP_HRS_CONTROL_POINT_INDICATION :
    case APP_CSC_REPORT_CONTROL_POINT_INDICATION :
    case APP_RSC_REPORT_CONTROL_POINT_INDICATION :
    case APP_HT_MEASUREMENT_INTERVAL_INDICATION :
        p_buffer[2] = event;
        p_buffer[3] = para1;
        for (i=0;i<para1;i++)
        {
            p_buffer[4+i] = p_data[i];
        }
        break;

    }
    p_buffer[0] = HCI_LE_APP_EVENT;

    if (event == APP_SCAN_INTERVAL_INDICATION)
        p_buffer[1] = 6;
    else if (( event ==  APP_LOCAL_TIME_INFORMATION_UPDATE_INDICATION ) ||
             ( event == APP_CURRENT_TIME_UPDATE_INDICATION ) ||
             ( event == APP_REFERENCE_TIME_INFORMATION_UPDATE_INDICATION ) ||
             ( event == APP_DATE_TIME_WITH_DST_UPDATE_INDICATION) ||
             ( event == APP_HID_BOOT_KEYBOARD_OUTPUT_INDICATION) ||
             ( event == APP_HID_REPORT_OUTPUT_INDICATION) ||
             ( event == APP_HID_REPORT_INPUT_INDICATION ) ||
             ( event == APP_HID_REPORT_FEATURE_INDICATION ) ||
             ( event == APP_HID_BOOT_MOUSE_INPUT_INDICATION ) ||
             ( event == APP_HID_BOOT_KEYBOARD_INPUT_INDICATION ) ||
             ( event == APP_HID_CONTROL_POINT_INDICATION) ||
             ( event == APP_HRS_CONTROL_POINT_INDICATION) ||
             ( event == APP_CSC_REPORT_CONTROL_POINT_INDICATION ) ||
             ( event == APP_RSC_REPORT_CONTROL_POINT_INDICATION ) ||
             ( event == APP_HID_PROTOCOL_MODE_INDICATION) ||
             ( event == APP_HT_MEASUREMENT_INTERVAL_INDICATION)

             )
    {
        p_buffer[1] = para1+2;
    }
    else
        p_buffer[1] = 2;

    BTq_Commit(HCI_EVENT_Q, 0);

}

u_int8 Battery_level_to_Percentage(u_int16 level)//for battery detect function
{
    u_int8 i;

    level &= ~0x03;
    for(i = 0;i < 11;i++)
    {
        if(level >= ADC_table[i])
            break;
    }
    return (100-(i*10));
}
/*************************************************************
 * SUPPORT FUNCTIONS FOR ALERTING
 *
 *************************************************************/
void Trans_ADC_Battery_Cap(void)
{
    u_int8 Battery_Capacity=0;
    if (ADC_captrue == TRUE)
    {
        if(ADC_finish == TRUE)
        {
            ADC_captrue = FALSE;
            ADC_finish = FALSE;

//            BKADC_DISABLE();/* /*  revision 335 change */ this line let chip can not make ADC int  */
            BKADC_CLK_PowerDown();
            BKADC_INT_DISABLE();

            Battery_Capacity = Battery_level_to_Percentage(Get_Battery());
            Save_Power_on_Battery(Battery_Capacity);
            GATTserver_Characteristic_Write_Local_Value( BAS_SERVER_BATTERY_LEVEL_HANDLE,1,&Battery_Capacity);
            
        }
    }
}

u_int8 Transuart_status = TRANSUART_OFF_STATED;
void User_App_Check_Timers(void)
{
    u_int32 current_clk = HW_Get_Native_Clk_Avoid_Race();
    /*u_int8 active_link = LE_config.active_link;
    t_LE_Connection* p_connection  = LEconnection_Find_Link_Entry(active_link);
    u_int8* p_value_array;
    u_int8 length;*/
    if(GATT_Check_Is_Link_Encrypted())  //frank 160723
    {
    	if(Transuart_status == TRANSUART_OFF_STATED)
    	{
    		Transuart_status = TRANSUART_ON_STATED;
		Beken_UART_Initial();
    	}
    }
#if (APP_DEFINE == APP_HT_DEVICE)
    if ((LEapp_Config.TAPP_HT_Update_Timer !=  BT_CLOCK_MAX_TIMEOUT) &&
        (((LEapp_Config.TAPP_HT_Update_Timer - current_clk - 1) & BT_CLOCK_MAX_TICKS) >= BT_CLOCK_MAX_TIMEOUT))
    {
#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==1)
        if (LEapp_Config.HT_Updated)
        {
            // Update the Temperature..
            LEapp_Config.HT_Updated = 0;
            {
                u_int32 mantissa = (u_int32)(LEapp_Config.New_Temp*10);
                u_int8 p_pdu[8] = {0x00 };

                p_pdu[4] = 0xFF; // Exponent = -1 = 0xFF
                p_pdu[3] = (mantissa & 0x00FF0000)>>16;
                p_pdu[2] = (mantissa & 0x0000FF00)>>8;
                p_pdu[1] = mantissa & 0xFF;

                LEapp_Config.Prev_Temp = LEapp_Config.New_Temp;
                GATTserver_Characteristic_Write_Local_Value(HT_TEMP_MEASUREMENT_HANDLE,0x05,p_pdu);
                User_App_Update_LCD();
            }
            LEapp_Config.TAPP_HT_Update_Timer = current_clk + 400;
        }
#else
        {
            if (LEapp_Config.TAPP_HT_Measurement_Interval!=0)
                LEapp_Config.TAPP_HT_Update_Timer = current_clk + (LEapp_Config.TAPP_HT_Measurement_Interval*3200);
            else
                LEapp_Config.TAPP_HT_Update_Timer = BT_CLOCK_MAX_TIMEOUT;
            // Read and Write the Measurement !!
            GATTserver_Characteristic_Write_Local_Value(HT_TEMP_MEASUREMENT_HANDLE,13,ht_temperature_measurement);
        }
#endif
    }
#endif

       //DEBUG_MSG(0xf9);
     // DEBUG_MSG((LEapp_Config.TAPP_L2CAP_PARA_UPDATE_Timer  & 0xff000000)>>24);
      //DEBUG_MSG((LEapp_Config.TAPP_L2CAP_PARA_UPDATE_Timer  & 0xff0000)>>16);
      // DEBUG_MSG((LEapp_Config.TAPP_L2CAP_PARA_UPDATE_Timer  & 0xff00)>>8);
	//DEBUG_MSG(LEapp_Config.TAPP_L2CAP_PARA_UPDATE_Timer  & 0x00ff);
      //DEBUG_MSG(0xfa);
	//DEBUG_MSG((current_clk  & 0xff000000)>>24);
       //DEBUG_MSG((current_clk  & 0xff0000)>>16);
       //DEBUG_MSG((current_clk & 0xff00)>>8);
	//DEBUG_MSG(current_clk& 0x00ff);
	//DEBUG_MSG(0xfb);
	
	
	
    if ((LEapp_Config.TAPP_L2CAP_PARA_UPDATE_Timer !=  BT_CLOCK_MAX_TIMEOUT) &&
        (((LEapp_Config.TAPP_L2CAP_PARA_UPDATE_Timer - current_clk - 1) & BT_CLOCK_MAX_TICKS) >= BT_CLOCK_MAX_TIMEOUT))
    {
        // LEsmp_Initiate_Security(0xFFFF, 0x05);  //frank 160719
        LEapp_Config.TAPP_L2CAP_PARA_UPDATE_Timer = BT_CLOCK_MAX_TIMEOUT;
#ifdef ENABLE_L2CAP_UPDATE_PARAMETER

#ifdef CONNECT_INTERVAL_50MS
			 LE_L2CAP_Connection_Update(0xFFFF/*use internal valid handle*/,0x0028,0x0024,0,0x0258);  //latency must 0
#endif

#ifdef CONNECT_INTERVAL_100MS
        //LE_L2CAP_Connection_Update(0xFFFF/*use internal valid handle*/,0x0050,0x0048,0,0x0258);
			  LE_L2CAP_Connection_Update(0xFFFF/*use internal valid handle*/,0x0050,0x0048,0,0x0258);
#endif	
#ifdef CONNECT_INTERVAL_180MS
			  
			  LE_L2CAP_Connection_Update(0xFFFF/*use internal valid handle*/,0x0090,0x0080,0,0x0258);/*  revision 335 change */
#endif			
#ifdef CONNECT_INTERVAL_200MS
        LE_L2CAP_Connection_Update(0xFFFF/*use internal valid handle*/,0x0050*2,0x0048*2,0,0x0258);
#endif			

#ifdef CONNECT_INTERVAL_290MS
			 
        LE_L2CAP_Connection_Update(0xFFFF/*use internal valid handle*/,0x00E8,0x00D0,0,0x0258);/*  revision 335 change */
#endif 

#ifdef CONNECT_INTERVAL_300MS
			 
        LE_L2CAP_Connection_Update(0xFFFF/*use internal valid handle*/,0x0050*3,0x0048*3,0,0x0258);/*  revision 335 change */
			
        //LE_L2CAP_Connection_Update(0xFFFF/*use internal valid handle*/,0x0048,0x0030,10,0x0258);//300ms
#endif

#ifdef CONNECT_INTERVAL_600MS
        //DEBUG_MSG(0xfe);
        LE_L2CAP_Connection_Update(0xFFFF/*use internal valid handle*/,0x01E0,0x0190,0,0x0258);//600ms
         //LE_L2CAP_Connection_Update(0xFFFF/*use internal valid handle*/,0x01E0,0x0190,0,0xC258);//600ms
      // LE_L2CAP_Connection_Update(0xFFFF/*use internal valid handle*/,0x01E0,0x0190,1,0x0C58);//600ms //frank 160617
     // LE_L2CAP_Connection_Update(0xFFFF/*use internal valid handle*/,0x03c0,0x0320,1,0x0C58);//600ms //frank 160617
#endif

#endif
        //LE_L2CAP_Connection_Update(0xFFFF/*use internal valid handle*/,0x00F0,0x00C8,0,0x12C);//300ms
        //LE_L2CAP_Connection_Update(0xFFFF/*use internal valid handle*/,0x00FF,0x00C8,0,0x0258);//310ms
        /* max_interval = 0x00F0 * 1.25 ms,min_interval = 0x00C8 * 1.25 ms,latency = 0,supervision timeout : 0x0258 * 10 ms = 6000 ms */
        //LE_L2CAP_Connection_Update(0xFFFF/*use internal valid handle*/,0x0010,0x0008,0,0x0064);//20ms
#endif
        //GATTserver_Characteristic_Write_Local_Value( KEY_PRESS_HANDLE,1,&Keypress_state);//charles test
    }

	
    if ((LEapp_Config.TAPP_BatteryDetect_Timer !=  BT_CLOCK_MAX_TIMEOUT) &&
        (((LEapp_Config.TAPP_BatteryDetect_Timer - current_clk - 1) & BT_CLOCK_MAX_TICKS) >= BT_CLOCK_MAX_TIMEOUT))
    {
    }
    if ((LEapp_Config.TAPP_POWERON_STANDBY_Timer !=  BT_CLOCK_MAX_TIMEOUT) &&
        (((LEapp_Config.TAPP_POWERON_STANDBY_Timer - current_clk) & BT_CLOCK_MAX_TICKS) >= BT_CLOCK_MAX_TIMEOUT))
    {
        LEapp_Config.TAPP_POWERON_STANDBY_Timer = BT_CLOCK_MAX_TIMEOUT;
#if (AUTO_ADVERTISING == 0)
        Set_MMI_State(MMI_DEEPSLEEP_STATE);
#endif
    }
}

void User_App_Flash_7Segment(u_int8 level)
{
#if 0

#endif
}

void User_App_Update_LCD(void)
{
#ifdef LCD_ENABLED
    char str[40];
    sprintf(str,"BPM :- %5d    ", LEapp_Config.HR_BPM);
    LCD_Init();
    LCD_Show_Text(str);
    LCD_Line2();
    sprintf(str,"Temp :- %4f    ", LEapp_Config.New_Temp);
    LCD_Show_Text(str);
#endif
}

#endif


#if (PRH_BS_CFG_SYS_EMBEDDED_PROFILES==1)
/********************************************************************
 * GATT Server Database setup functions.
 * Following functions are used to configure the GATT DATABASE
 */

/*******************************************************
 * GERERIC ACCESS PROFILE DEFINITION
 * Handle 0x10 to 0x1A
 *
 * Peripheral Privacy is at 0x18
 * ReConnection Addr  is at 0x1A
 ******************************************************/

u_int16 App_Setup_GAP_GATT_DB(void)
{
    //u_int16 handle = 0x10; /* GAP handles goes from 0x10- 0x1B   */
    extern u_int8 GATTprivacy_Flags[];
    extern u_int16 GATT_reconnection_address_handle;

    _GATTserver_Add_Service(PRIMARY_SERVICE_16BIT, handle,0x00,GENERIC_ACCESS_PROFILE);
    handle++;

    _GATTserver_Add_Characteristic_Declaration(handle,DEVICE_NAME,0x08);
    handle++;
    //_GATTserver_Add_Characteristic_Value(handle,DEVICE_NAME,NO_LOCAL_NOTIFY,GATT_PERMISSION_READ |GATT_PERMISSION_WRITE,13,gap_name_array);
    _GATTserver_Add_Characteristic_Value(handle,DEVICE_NAME,NO_LOCAL_NOTIFY,GATT_PERMISSION_READ |GATT_PERMISSION_WRITE,sizeof(gap_name_array)-1,gap_name_array);//charles modify
    handle++;

    _GATTserver_Add_Characteristic_Declaration(handle,APPERANCE,0x02);
    handle++;
    _GATTserver_Add_Characteristic_Value(handle,APPERANCE,NO_LOCAL_NOTIFY,GATT_PERMISSION_READ|GATT_PERMISSION_WRITE,1,gap_appearance_array);
    handle++;

    _GATTserver_Add_Characteristic_Declaration(handle,PERIPHERAL_PREFERRED_CONNECTION_PARAMETERS,0x02);
    handle++;
    _GATTserver_Add_Characteristic_Value(handle,PERIPHERAL_PREFERRED_CONNECTION_PARAMETERS,NO_LOCAL_NOTIFY,GATT_PERMISSION_READ,7,gap_connection_params);
    handle++;


#if (PRH_BS_CFG_SYS_LE_GAP_PRIVACY_SUPPORTED==1)
    _GATTserver_Add_Characteristic_Declaration(handle,PERIPHERAL_PRIVACY_FLAGS,0x02);
    // Value handle = 0x18
    handle++;
    _GATTserver_Add_Characteristic_Value(handle,PERIPHERAL_PRIVACY_FLAGS,NO_LOCAL_NOTIFY,GATT_PERMISSION_READ|GATT_PERMISSION_WRITE,1,GATTprivacy_Flags);
    handle++;
#endif

    // handle = 0x19
    _GATTserver_Add_Characteristic_Declaration(handle,RECONNECTION_ADDRESS,0x02);
    handle++;
    // Value handle = 0x1A
    GATT_reconnection_address_handle = handle;
    _GATTserver_Add_Characteristic_Value(handle,RECONNECTION_ADDRESS,NO_LOCAL_NOTIFY,GATT_PERMISSION_READ|GATT_PERMISSION_WRITE,6,reconnection_addr);
    //0x10+0x0A = 0x1A Enablehandle
    handle++;
    return handle;
}


/*******************************************************
 * GATT PROFILE DEFINITION
 * Handle 0x20 to 0x22
 *
 * Service Changed is at 0x22
 *
 ******************************************************/
u_int16 App_Setup_GATT_GATT_DB(void)
{
    //u_int16 handle = 0x20;
    _GATTserver_Add_Service(PRIMARY_SERVICE_16BIT, handle,0x00,GENERIC_ATTRIBUTE_PROFILE);
    handle++;

    _GATTserver_Add_Characteristic_Declaration(handle,SERVICE_CHANGED,0x20);
    handle++;
    _GATTserver_Add_Characteristic_Value(handle,SERVICE_CHANGED,NO_LOCAL_NOTIFY,0x00,4,Service_Change_Range);
    handle++;

    _GATTserver_Add_Char_Descriptor(handle,CLIENT_CHARACTERISTIC_CONFIGURATION,GATT_PERMISSION_READ|GATT_PERMISSION_WRITE,0x02,GATT_service_change_CCC);
    //0x20+0x03 = 0x23 Enable handle
    handle++;
    return handle;
}


/*******************************************************
 * DERVICE INFORMATION PROFILE DEFINITION
 * Handle 0x30 to 0x42
 *
 ******************************************************/
u_int16 App_Setup_DEV_INFO_GATT_DB(void)
{
    //u_int16 handle = 0x30;
    _GATTserver_Add_Service(PRIMARY_SERVICE_16BIT, handle,0x00,DEVICE_INFORMATION);
    handle++;

    _GATTserver_Add_Characteristic_Declaration(handle,GATT_CHAR_MANUFACTURER_NAME,0x02);
    handle++;
    _GATTserver_Add_Characteristic_Value(handle,GATT_CHAR_MANUFACTURER_NAME,NO_LOCAL_NOTIFY,GATT_PERMISSION_READ,sizeof(manufacturer_name_string)-1,manufacturer_name_string);
    handle++;

    _GATTserver_Add_Characteristic_Declaration(handle,GATT_CHAR_MODEL_NUMBER,0x02);
    handle++;
    _GATTserver_Add_Characteristic_Value(handle,GATT_CHAR_MODEL_NUMBER,NO_LOCAL_NOTIFY,GATT_PERMISSION_READ,6,model_number_string);
    handle++;

    _GATTserver_Add_Characteristic_Declaration(handle,GATT_CHAR_SERIAL_NUMBER,0x02);
    handle++;
    _GATTserver_Add_Characteristic_Value(handle,GATT_CHAR_SERIAL_NUMBER,NO_LOCAL_NOTIFY,GATT_PERMISSION_READ,10,serial_number_string);
    handle++;

    _GATTserver_Add_Characteristic_Declaration(handle,GATT_CHAR_FW_REVISION,0x02);
    handle++;
    _GATTserver_Add_Characteristic_Value(handle,GATT_CHAR_FW_REVISION,NO_LOCAL_NOTIFY,GATT_PERMISSION_READ,4,fw_revision_string);
    handle++;

    _GATTserver_Add_Characteristic_Declaration(handle,GATT_CHAR_HW_REVISION,0x02);
    handle++;
    _GATTserver_Add_Characteristic_Value(handle,GATT_CHAR_HW_REVISION,NO_LOCAL_NOTIFY,GATT_PERMISSION_READ,4,hw_revision_string);
    handle++;

    _GATTserver_Add_Characteristic_Declaration(handle,GATT_CHAR_SW_REVISION,0x02);
    handle++;
    _GATTserver_Add_Characteristic_Value(handle,GATT_CHAR_SW_REVISION,NO_LOCAL_NOTIFY,GATT_PERMISSION_READ,4,sw_revision_string);
    handle++;


    _GATTserver_Add_Characteristic_Declaration(handle,GATT_CHAR_SYSTEM_ID,0x02);
    handle++;
    _GATTserver_Add_Characteristic_Value(handle,GATT_CHAR_SYSTEM_ID,NO_LOCAL_NOTIFY,GATT_PERMISSION_READ,8,system_id);
    handle++;

    _GATTserver_Add_Characteristic_Declaration(handle,GATT_CHAR_REG_CERT,0x02);
    handle++;
    _GATTserver_Add_Characteristic_Value(handle,GATT_CHAR_REG_CERT,NO_LOCAL_NOTIFY,GATT_PERMISSION_READ,4,IEEE_Reg_Cert);
    handle++;

    _GATTserver_Add_Characteristic_Declaration(handle,GATT_CHAR_PnP_ID,0x02);
    handle++;
    _GATTserver_Add_Characteristic_Value(handle,GATT_CHAR_PnP_ID,NO_LOCAL_NOTIFY,GATT_PERMISSION_READ,7,pnp_id);
    //0x30+0x12 = 0x66 End handle
    handle++;
    return handle;
}

/*******************************************************
 * IMMEDIATE ALERT PROFILE DEFINITION
 * Handle 0x50 to 0x52
 *
 *
 ******************************************************/
u_int16 App_Setup_IAS_GATT_DB(void)
{
    //u_int16 handle = 0x70;
    _GATTserver_Add_Service(PRIMARY_SERVICE_16BIT, handle,0x00,IMMEDIATE_ALERT);
    handle++;

    _GATTserver_Add_Characteristic_Declaration(handle,GATT_CHAR_ALERT_LEVEL,0x04);
    handle++;
    _GATTserver_Add_Characteristic_Value(handle,GATT_CHAR_ALERT_LEVEL,USE_LOCAL_NOTIFY,GATT_PERMISSION_READ | GATT_PERMISSION_WRITE,1,alert_level_array);
    //0x70+0x02 = 0x72 Enable handles
    handle++;
    return handle;
}

/*******************************************************
 * TX POWER PROFILE DEFINITION
 * Handle 0x60 to 0x62
 *
 ******************************************************/

u_int16 App_Setup_TX_POWER_GATT_DB(void)
{
    //u_int16 handle = 0x80;
    _GATTserver_Add_Service(PRIMARY_SERVICE_16BIT, handle,0x00,TX_POWER);
    handle++;

    _GATTserver_Add_Characteristic_Declaration(handle,TX_POWER_LEVEL,0x12);//Read,Notify
    handle++;
    _GATTserver_Add_Characteristic_Value(handle,TX_POWER_LEVEL,NO_LOCAL_NOTIFY,GATT_PERMISSION_READ,1,tx_power_array);
    handle++;

    _GATTserver_Add_Char_Descriptor(handle,CLIENT_CHARACTERISTIC_CONFIGURATION,GATT_PERMISSION_READ|GATT_PERMISSION_WRITE,0x02,TX_Power_Level_CCC);

    handle++;

    tx_power_format[0] = 12; // Format
    tx_power_format[1] = 0; // Exponent
    LMutils_Set_Uint16(tx_power_format+2, 0x2726); /* UUID for Watts */
    tx_power_format[4] = 1; // Name Space - Sig Specified
    LMutils_Set_Uint16(tx_power_format+5, 0x000E);

    _GATTserver_Add_Char_Descriptor(handle,CHARACTERISTIC_PRESENTATION_FORMAT,GATT_PERMISSION_READ | GATT_PERMISSION_WRITE,0x07,tx_power_format);
    //0x70+0x04 = 0x84 Enable handles
    handle++;
    return handle;
}

/********************************************************
 * LINK LOSS PROFILE DEFINITION
 * Handle 0x70 to 0x72
 ********************************************************/
u_int16 App_Setup_LINK_LOSS_GATT_DB(void)
{
    //u_int16 handle = 0x90;
    _GATTserver_Add_Service(PRIMARY_SERVICE_16BIT, handle,0x00,LINK_LOSS);
    handle++;
    _GATTserver_Add_Characteristic_Declaration(handle,GATT_CHAR_ALERT_LEVEL,0x0A);
    handle++;
    _GATTserver_Add_Characteristic_Value(handle,GATT_CHAR_ALERT_LEVEL,USE_LOCAL_NOTIFY,GATT_PERMISSION_READ | GATT_PERMISSION_WRITE,1,link_loss_alert_level_array);
    //0x80+0x02 = 0x92 Enable handles
    handle++;
    return handle;
}

/********************************************************
 * BATTERY SERVICE PROFILE DEFINITION
 * Handle 0x80 to 0x82
 ********************************************************/
u_int16 App_Setup_BATTERY_SERVICE_GATT_DB(void)
{
    //u_int16 handle = 0xA0;
    _GATTserver_Add_Service(PRIMARY_SERVICE_16BIT, handle,0x00,BATTERY_SERVICE);
    handle++;
    //handle = 0xA1
    _GATTserver_Add_Characteristic_Declaration(handle,BATTERY_LEVEL ,0x12);//Characteristic Properties// bit 0 :Broadcast, bit 1 : Read , bit 2 : Write Without Response, bit 3 : Write
    // bit 4 : Notify, bit 5 : Indicate, bit 6 : Authenticated Signed Writes,
    //bit 7 : Extended Properties

    handle++;
    //handle = 0xA2
    _GATTserver_Add_Characteristic_Value(handle,BATTERY_LEVEL,NO_LOCAL_NOTIFY,GATT_PERMISSION_READ,1,battery_level);//battery level initial value for var declare
    handle++;
    //handle = 0xA3
    _GATTserver_Add_Char_Descriptor(handle,CLIENT_CHARACTERISTIC_CONFIGURATION,GATT_PERMISSION_READ|GATT_PERMISSION_WRITE,0x02,Battery_Service_CCC);

    handle++;
    //handle = 0xA4
    battery_service_format[0] = 4; // Format
    battery_service_format[1] = 0; // Exponent
    LMutils_Set_Uint16(battery_service_format+2, 0x27AD); /* UUID for Percentage */
    battery_service_format[4] = 1; // Name Space - Sig Specified
    LMutils_Set_Uint16(battery_service_format+5, 0x000E);

    _GATTserver_Add_Char_Descriptor(handle,CHARACTERISTIC_PRESENTATION_FORMAT,GATT_PERMISSION_READ | GATT_PERMISSION_WRITE,0x07,battery_service_format);
    //0xA0+0x04 = 0xA4 Enable handles
    handle++;
    return handle;
}

#if 1
/*******************************************************
 * KEY PRESS STATE DEFINITION (USER DEFINE)
 * Handle
 *
 * Service Changed is at
 *
 ******************************************************/
u_int16 App_Setup_FFE0_GATT_DB(void)
{
    //u_int16 handle = 0xB0;
    _GATTserver_Add_Service(PRIMARY_SERVICE_16BIT, handle,0x00,0xFFE0);
    handle++;

    _GATTserver_Add_Characteristic_Declaration(handle,0xFFE1,0x1A);//Characteristic Properties// bit 0 :Broadcast, bit 1 : Read , bit 2 : Write Without Response, bit 3 : Write
    // bit 4 : Notify, bit 5 : Indicate, bit 6 : Authenticated Signed Writes,
    //bit 7 : Extended Properties
    handle++;
    _GATTserver_Add_Characteristic_Value(handle,0xFFE1,USE_LOCAL_NOTIFY,GATT_PERMISSION_READ|GATT_PERMISSION_WRITE,20,Notify_Phone_Alert);
    notify_handle = handle;
    handle++;
    _GATTserver_Add_Char_Descriptor(handle,CLIENT_CHARACTERISTIC_CONFIGURATION,GATT_PERMISSION_READ|GATT_PERMISSION_WRITE,0x02,Notify_Phone_description_CCC);//handle == 0x00B3 == 179
    handle++;
    _GATTserver_Add_Char_Descriptor(handle,CHARACTERISTIC_USER_DESCRIPTION,GATT_PERMISSION_READ,sizeof(Notify_Phone_Alert_str)-1,Notify_Phone_Alert_str);
    //0xB0+0x04 = 0xB4 Enable handle
    handle++;
    return handle;
}
#endif
#if 1
/*******************************************************
 * MISC STATE DEFINITION (USER DEFINE)
 * Handle
 *
 * Service Changed is at
 *
 ******************************************************/
u_int16 App_Setup_FFA0_GATT_DB(void)
{
    //u_int16 handle = 0xC0;

    _GATTserver_Add_Service(PRIMARY_SERVICE_16BIT, handle,0x00,0xFFA0);
    handle++;

    _GATTserver_Add_Characteristic_Declaration(handle,0xFFA1,0x0A);//Characteristic Properties// bit 0 :Broadcast, bit 1 : Read , bit 2 : Write Without Response, bit 3 : Write
    // bit 4 : Notify, bit 5 : Indicate, bit 6 : Authenticated Signed Writes,
    //bit 7 : Extended Properties
    handle++;
    _GATTserver_Add_Characteristic_Value(handle,0xFFA1,NO_LOCAL_NOTIFY,0x00,1,Range_Alarm_Enable);
    handle++;
    _GATTserver_Add_Char_Descriptor(handle,CHARACTERISTIC_USER_DESCRIPTION,GATT_PERMISSION_READ,18,Range_Alarm_Enable_str);
    handle++;


    _GATTserver_Add_Characteristic_Declaration(handle,0xFFA2,0x0A);//Characteristic Properties// bit 0 :Broadcast, bit 1 : Read , bit 2 : Write Without Response, bit 3 : Write
                                                                   // bit 4 : Notify, bit 5 : Indicate, bit 6 : Authenticated Signed Writes,
    handle++;
    _GATTserver_Add_Characteristic_Value(handle,0xFFA2,NO_LOCAL_NOTIFY,0x00,1,Beep_range);
    handle++;
    _GATTserver_Add_Char_Descriptor(handle,CHARACTERISTIC_USER_DESCRIPTION,GATT_PERMISSION_READ,10,Beep_range_str);
    handle++;

    _GATTserver_Add_Characteristic_Declaration(handle,0xFFA3,0x02);//Characteristic Properties// bit 0 :Broadcast, bit 1 : Read , bit 2 : Write Without Response, bit 3 : Write
    // bit 4 : Notify, bit 5 : Indicate, bit 6 : Authenticated Signed Writes,
    handle++;
    _GATTserver_Add_Characteristic_Value(handle,0xFFA3,NO_LOCAL_NOTIFY,0x00,1,Reserve0);
    handle++;
    _GATTserver_Add_Char_Descriptor(handle,CHARACTERISTIC_USER_DESCRIPTION,GATT_PERMISSION_READ,9,Reserve0_str);
    handle++;

    _GATTserver_Add_Characteristic_Declaration(handle,0xFFA4,0x02);//Characteristic Properties// bit 0 :Broadcast, bit 1 : Read , bit 2 : Write Without Response, bit 3 : Write
    // bit 4 : Notify, bit 5 : Indicate, bit 6 : Authenticated Signed Writes,
    handle++;
    _GATTserver_Add_Characteristic_Value(handle,0xFFA4,NO_LOCAL_NOTIFY,0x00,1,Reserve1);
    handle++;
    _GATTserver_Add_Char_Descriptor(handle,CHARACTERISTIC_USER_DESCRIPTION,GATT_PERMISSION_READ,9,Reserve1_str);
    handle++;

    _GATTserver_Add_Characteristic_Declaration(handle,0xFFA5,0x02);//Characteristic Properties// bit 0 :Broadcast, bit 1 : Read , bit 2 : Write Without Response, bit 3 : Write
    // bit 4 : Notify, bit 5 : Indicate, bit 6 : Authenticated Signed Writes,
    handle++;
    _GATTserver_Add_Characteristic_Value(handle,0xFFA5,NO_LOCAL_NOTIFY,0x00,1,Reserve2);
    handle++;
    _GATTserver_Add_Char_Descriptor(handle,CHARACTERISTIC_USER_DESCRIPTION,GATT_PERMISSION_READ,9,Reserve2_str);

    //0xC0+0x0F = 0xCF Enable handle
    handle++;
    return handle;
}
#endif




/********************************************************
 * SCAN PRAMETERS PROFILE SERVICE DEFINITION
 * Handle 0x90 to 0x95
 ********************************************************/

u_int16 App_Setup_SCAN_PARAMETERS_GATT_DB(void)
{

    return handle;
}

/********************************************************
 * CURRENT TIME PROFILE SERVICE DEFINITION
 * Handle 0xA0 to 0xA7
 ********************************************************/

u_int16 App_Setup_CURRENT_TIME_GATT_DB(void)
{
    u_int16 handle = 0xFFFF;

    return handle;
}


/********************************************************
 * REFERENCE TIME PROFILE SERVICE DEFINITION
 *  Handle 0xB0 to 0xB4
 ********************************************************/

u_int16 App_Setup_REFERENCE_TIME_GATT_DB(void)
{
    u_int16 handle = 0xFFFF;

    return handle;
}

/********************************************************
 * NEXT DST CHANGE PROFILE SERVICE DEFINITION
 * Handle 0xC0 to 0xB4
 ********************************************************/

u_int16 App_Setup_NEXT_DST_GATT_DB(void)
{
    u_int16 handle = 0xFFFF;

    return handle;

}


/********************************************************
 * HID - PROFILE SERVICE DEFINITION
 * Handle 0xD0 to 0xD
 ********************************************************/

u_int16 App_Setup_HID_GATT_DB(void)
{
    u_int16 handle = 0xFFFF;

    return handle;
}


/********************************************************
 * HRS - Heart Rate ServicePROFILE SERVICE DEFINITION
 * Handle 0xF0 to 0xFF
 ********************************************************/
u_int16 App_Setup_HRS_GATT_DB(void)
{
    u_int16 handle = 0xFFFF;

    return handle;
}

/********************************************************
 * HT - Health Thermometer
 * Handle 0x100 to 0x110
 ********************************************************/
u_int16 App_Setup_HT_GATT_DB(void)
{
    u_int16 handle = 0xFFFF;

    return handle;
}


/********************************************************
 * CSC - Cycling Speed and Cadence - PROFILE SERVICE DEFINITION
 * Handle 0x110 to 0x120
 ********************************************************/
u_int16 App_Setup_CSC_GATT_DB(void)
{
    u_int16 handle = 0xFFFF;

    return handle;
}

/********************************************************
 * RSC - Running Speed and Cadence - PROFILE SERVICE DEFINITION
 * Handle 0x120 to 0x12F
 ********************************************************/
u_int16 App_Setup_RSC_GATT_DB(void)
{
    u_int16 handle = 0xFFFF;

    return handle;
}

/********************************************************
 * BP - Blood Pressure - PROFILE SERVICE DEFINITION
 * Handle 0x130 to 0x13F
 ********************************************************/

u_int16 App_Setup_BLOOD_PRESSURE_GATT_DB(void)
{
    u_int16 handle = 0xFFFF;

    return handle;
}

/********************************************************
 * BG - Blood GLucose - PROFILE SERVICE DEFINITION
 * Handle 0x140 to 0x14F
 ********************************************************/

u_int16 App_Setup_BLOOD_GLUCOSE_GATT_DB(void)
{
    u_int16 handle = 0xFFFF;

    return handle;
}

void User_Notify(u_int8* data, u_int8 len)
{
    GATTserver_Characteristic_Write_Local_Value( notify_handle,len,data);
}
#endif 
