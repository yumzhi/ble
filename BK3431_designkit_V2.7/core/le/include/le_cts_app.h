#ifndef _CEVA_LE_CTS_APP_H__
#define _CEVA_LE_CTS_APP_H__
#define DEBUG_FOR_LOW_POWER 0
#define ENABLE_L2CAP_UPDATE_PARAMETER  //frank 160617
#define APP_FIND_ME    0
#define APP_HT_DEVICE  1
#define APP_HR_DEVICE  2
#define APP_DEFINE FIND_ME
/*********************************************************************
 * MODULE NAME:     le_cts_app.h
 * PROJECT CODE:    BlueStream
 * DESCRIPTION:
 * MAINTAINER:      Gary Fleming
 * DATE:            June-2012
 * SOURCE CONTROL:
 *
 * LICENSE:
 *     This source code is copyright (c) 1999-2012 Ceva Inc.
 *     All rights reserved.
 *
 *********************************************************************/

typedef struct t_App_Control
{
	
	//u_int8 Flash_Type;// no used
	#if (APP_DEFINE == APP_FIND_ME)
	u_int8 Alert_Level;
	//u_int32 TAPP_Alerting_Timer;// no used
	u_int32 TAPP_BatteryDetect_Timer;
	//u_int32 TAPP_DISCOVERABLE_Timer;//for connectable mode transfer to discoverable timer
	u_int32 TAPP_POWERON_STANDBY_Timer;
  u_int32 MMI_Timer[3];
	#endif
	#if (APP_DEFINE == APP_HT_DEVICE)
	u_int32 TAPP_HT_Update_Timer;//charles remove for FIND me only
	#endif
	#if (APP_DEFINE == APP_HR_DEVICE)
	u_int32 TAPP_HR_Update_Timer;//charles remove for FIND me only
	#endif
	u_int32 TAPP_L2CAP_PARA_UPDATE_Timer;
	#if (APP_DEFINE == APP_HT_DEVICE)
	u_int32 TAPP_HT_Measurement_Interval;//charles remove for FIND me only
	u_int8 HT_Updated;
	float New_Temp;
	float Prev_Temp;
  #endif
	#if (APP_DEFINE == APP_HR_DEVICE)
	u_int8 HR_Sample_Number;
	u_int8 HR_Updated;
	u_int32 HR_Aggretate_Time_for_10pulse;
	u_int16 HR_BPM;
	u_int32 HR_Clk_NthPulse;
	u_int32 HR_Clk_FirstPulse;
	u_int32 HR_Clk_TenthPulse;
	#endif
	
	

} t_App_Control;


t_error LE_Simple_APP_Info(u_int8 para1,u_int8 para2,u_int8 para3);
t_error LE_CTS_APP_Info(u_int8 command,u_int8 command_val,u_int8* p_command_params);
t_error LE_NCST_APP_Info(u_int8 command,u_int8 command_len,u_int8* p_command_params);
t_error LE_HID_APP_Info(u_int8 command,u_int8 command_len,u_int8* p_pdu);
t_error LE_HT_APP_Info(u_int8 command,u_int8 command_len,u_int8* p_pdu);
t_error LE_HRS_APP_Info(u_int8 command,u_int8 command_len,u_int8* p_pdu);
t_error LE_CSC_APP_Info(u_int8 command,u_int8 command_len,u_int8* p_pdu);
t_error LE_RSC_APP_Info(u_int8 command,u_int8 command_len,u_int8* p_pdu);
t_error LE_BP_APP_Info(u_int8 command,u_int8 command_len,u_int8* p_pdu);
t_error LE_BG_APP_Info(u_int8 command,u_int8 command_len,u_int8* p_pdu);

u_int16 App_Setup_GAP_GATT_DB(void);
u_int16 App_Setup_GATT_GATT_DB(void);
u_int16 App_Setup_DEV_INFO_GATT_DB(void);
u_int16 App_Setup_IAS_GATT_DB(void);
u_int16 App_Setup_TX_POWER_GATT_DB(void);
u_int16 App_Setup_LINK_LOSS_GATT_DB(void);
u_int16 App_Setup_BATTERY_SERVICE_GATT_DB(void);
u_int16 App_Setup_SCAN_PARAMETERS_GATT_DB(void);

u_int16 App_Setup_CURRENT_TIME_GATT_DB(void);
u_int16 App_Setup_REFERENCE_TIME_GATT_DB(void);
u_int16 App_Setup_NEXT_DST_GATT_DB(void);

u_int16 App_Setup_HID_GATT_DB(void);
u_int16 App_Setup_HRS_GATT_DB(void);
u_int16 App_Setup_HT_GATT_DB(void);
u_int16 App_Setup_CSC_GATT_DB(void);
u_int16 App_Setup_RSC_GATT_DB(void);
u_int16 App_Setup_BLOOD_PRESSURE_GATT_DB(void);
u_int16 App_Setup_BLOOD_GLUCOSE_GATT_DB(void);
u_int16 App_Setup_FFE0_GATT_DB(void);
u_int16 App_Setup_FFA0_GATT_DB(void);
void LE_App_Discover_Services_By_UUID_Event(u_int8 attrib_len, u_int8 value_payload_len, u_int8* p_pdu);
void LE_App_Command_Complete(u_int16 handle,u_int8 operation_id,u_int8 status);
void LE_App_Discover_Chars_Event(u_int8 attrib_len, u_int8 value_payload_len, u_int8* p_pdu);
u_int8 LE_App_Local_Event_Indication(u_int16 attrib_handle, u_int8 status);
u_int8 LE_App_Peer_Event_Indication(u_int16 attrib_handle, u_int8 status, u_int8 length, u_int8* p_data);
void LE_App_Read_Response(u_int8 length,u_int8* p_data);
u_int8 LE_App_Local_Disconnect_Indication(t_error reason);
void Trans_ADC_Battery_Cap(void);
void User_App_Check_Timers(void);
void Restart_Battery_Detect_Timer(void);
void LE_APP_Init(void);
void Save_Battery_level(void);
u_int16 Get_Battery(void);
void Save_Power_on_Battery(u_int8 value);
u_int8 Battery_level_to_Percentage(u_int16 level);

#endif
