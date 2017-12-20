
#ifndef _CEVA_LE_GAP_H__
#define _CEVA_LE_GAP_H__
#include "le_link_layer.h"
/*********************************************************************
 * MODULE NAME:     le_gap.h
 * PROJECT CODE:    BlueStream
 * DESCRIPTION:
 * MAINTAINER:      Gary Fleming
 * DATE:            Jan-2012
 * SOURCE CONTROL:
 *
 * LICENSE:
 *     This source code is copyright (c) 1999-2012 Ceva Inc.
 *     All rights reserved.
 *
 *********************************************************************/

#define GAP_NON_DISCOVERABLE     0x00
#define GAP_LIMITED_DISCOVERABLE 0x01
#define GAP_GENERAL_DISCOVERABLE 0x02

#define GAP_NON_DISCOVERY        0x00
#define GAP_LIMITED_DISCOVERY    0x01
#define GAP_GENERAL_DISCOVERY    0x02

/** @defgroup GAP_ADTYPE_DEFINES GAP Advertisment Data Types
 * These are the data type identifiers for the data tokens in the advertisement data field.
 * @{
 */
#define GAP_ADTYPE_FLAGS                        0x01 //!< Discovery Mode: @ref GAP_ADTYPE_FLAGS_MODES
#define GAP_ADTYPE_16BIT_MORE                   0x02 //!< Service: More 16-bit UUIDs available
#define GAP_ADTYPE_16BIT_COMPLETE               0x03 //!< Service: Complete list of 16-bit UUIDs
#define GAP_ADTYPE_32BIT_MORE                   0x04 //!< Service: More 32-bit UUIDs available
#define GAP_ADTYPE_32BIT_COMPLETE               0x05 //!< Service: Complete list of 32-bit UUIDs
#define GAP_ADTYPE_128BIT_MORE                  0x06 //!< Service: More 128-bit UUIDs available
#define GAP_ADTYPE_128BIT_COMPLETE              0x07 //!< Service: Complete list of 128-bit UUIDs
#define GAP_ADTYPE_LOCAL_NAME_SHORT             0x08 //!< Shortened local name
#define GAP_ADTYPE_LOCAL_NAME_COMPLETE          0x09 //!< Complete local name
#define GAP_ADTYPE_POWER_LEVEL                  0x0A //!< TX Power Level: 0xXX: -127 to +127 dBm
#define GAP_ADTYPE_OOB_CLASS_OF_DEVICE          0x0D //!< Simple Pairing OOB Tag: Class of device (3 octets)
#define GAP_ADTYPE_OOB_SIMPLE_PAIRING_HASHC     0x0E //!< Simple Pairing OOB Tag: Simple Pairing Hash C (16 octets)
#define GAP_ADTYPE_OOB_SIMPLE_PAIRING_RANDR     0x0F //!< Simple Pairing OOB Tag: Simple Pairing Randomizer R (16 octets)
#define GAP_ADTYPE_SM_TK                        0x10 //!< Security Manager TK Value
#define GAP_ADTYPE_SM_OOB_FLAG                  0x11 //!< Secutiry Manager OOB Flags
#define GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE    0x12 //!< Min and Max values of the connection interval (2 octets Min, 2 octets Max) (0xFFFF indicates no conn interval min or max)
#define GAP_ADTYPE_SIGNED_DATA                  0x13 //!< Signed Data field
#define GAP_ADTYPE_SERVICES_LIST_16BIT          0x14 //!< Service Solicitation: list of 16-bit Service UUIDs
#define GAP_ADTYPE_SERVICES_LIST_128BIT         0x15 //!< Service Solicitation: list of 128-bit Service UUIDs
#define GAP_ADTYPE_SERVICE_DATA                 0x16 //!< Service Data
#define GAP_ADTYPE_APPEARANCE                   0x19 //!< Appearance
#define GAP_ADTYPE_MANUFACTURER_SPECIFIC        0xFF //!< Manufacturer Specific Data: first 2 octets contain the Company Identifier Code followed by the additional manufacturer specific data
/** @} End GAP_ADTYPE_DEFINES */

/** @defgroup GAP_ADTYPE_FLAGS_MODES GAP ADTYPE Flags Discovery Modes
 * @{
 */
#define GAP_ADTYPE_FLAGS_LIMITED                0x01 //!< Discovery Mode: LE Limited Discoverable Mode
#define GAP_ADTYPE_FLAGS_GENERAL                0x02 //!< Discovery Mode: LE General Discoverable Mode
#define GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED    0x04 //!< Discovery Mode: BR/EDR Not Supported
/** @} End GAP_ADTYPE_FLAGS_MODES */

/** @defgroup GAP_APPEARANCE_VALUES GAP Appearance Values
 * @{
 */
#define GAP_APPEARE_UNKNOWN                     0x0000 //!< Unknown
#define GAP_APPEARE_GENERIC_PHONE               0x0040 //!< Generic Phone
#define GAP_APPEARE_GENERIC_COMPUTER            0x0080 //!< Generic Computer
#define GAP_APPEARE_GENERIC_WATCH               0x00C0 //!< Generic Watch
#define GAP_APPEARE_WATCH_SPORTS                0x00C1 //!< Watch: Sports Watch
#define GAP_APPEARE_GENERIC_CLOCK               0x0100 //!< Generic Clock
#define GAP_APPEARE_GENERIC_DISPLAY             0x0140 //!< Generic Display
#define GAP_APPEARE_GENERIC_RC                  0x0180 //!< Generic Remote Control
#define GAP_APPEARE_GENERIC_EYE_GALSSES         0x01C0 //!< Generic Eye-glasses
#define GAP_APPEARE_GENERIC_TAG                 0x0200 //!< Generic Tag
#define GAP_APPEARE_GENERIC_KEYRING             0x0240 //!< Generic Keyring
#define GAP_APPEARE_GENERIC_MEDIA_PLAYER        0x0280 //!< Generic Media Player
#define GAP_APPEARE_GENERIC_BARCODE_SCANNER     0x02C0 //!< Generic Barcode Scanner
#define GAP_APPEARE_GENERIC_THERMOMETER         0x0300 //!< Generic Thermometer
#define GAP_APPEARE_GENERIC_THERMO_EAR          0x0301 //!< Thermometer: Ear
#define GAP_APPEARE_GENERIC_HR_SENSOR           0x0340 //!< Generic Heart rate Sensor
#define GAP_APPEARE_GENERIC_HRS_BELT            0x0341 //!< Heart Rate Sensor: Heart Rate Belt
#define GAP_APPEARE_GENERIC_BLOOD_PRESSURE      0x0380 //!< Generic Blood Pressure
#define GAP_APPEARE_GENERIC_BP_ARM              0x0381 //!< Blood Pressure: Arm
#define GAP_APPEARE_GENERIC_BP_WRIST            0x0382 //!< Blood Pressure: Wrist
#define GAP_APPEARE_GENERIC_HID                 0x03C0 //!< Generic Human Interface Device (HID)
#define GAP_APPEARE_HID_KEYBOARD                0x03C1 //!< HID Keyboard
#define GAP_APPEARE_HID_MOUSE                   0x03C2 //!< HID Mouse
#define GAP_APPEARE_HID_JOYSTIC                 0x03C3 //!< HID Joystick
#define GAP_APPEARE_HID_GAMEPAD                 0x03C4 //!< HID Gamepad
#define GAP_APPEARE_HID_DIGITIZER_TYABLET       0x03C5 //!< HID Digitizer Tablet
#define GAP_APPEARE_HID_DIGITAL_CARDREADER      0x03C6 //!< HID Card Reader
#define GAP_APPEARE_HID_DIGITAL_PEN             0x03C7 //!< HID Digital Pen
#define GAP_APPEARE_HID_BARCODE_SCANNER         0x03C8 //!< HID Barcode Scanner
/** @} End GAP_APPEARANCE_VALUES */

typedef struct t_gap_event_params {
	u_int8 event;
	u_int16 handle;
	u_int8 status;
	u_int8* p_address;
	u_int8 valid_reconn_addr;
	u_int8 encrypt;
	u_int8 role;

} t_gap_event_params;

extern u_int32 gCurrent_ADV_interval;

void GAP_Set_AD_Data(u_int8 length, u_int8* p_data);
void GAP_Set_ScanResp_Data(u_int8 length, u_int8* p_data);

u_int8 GAP_Discoverable_Mode(u_int8 mode,u_int8 adv_type);
u_int8 GAP_Discovery_Mode(u_int8 mode);

u_int8 GAP_Set_Broadcast_Data(u_int8 length,u_int8* p_data);
u_int8 GAP_Set_Broadcast_Mode(u_int8 mode, u_int8 type);

u_int8 GAP_Connectable_Mode(u_int8 mode, u_int8 peer_addr_type,u_int8 own_addr_type, u_int8* p_bdaddr);

#if (PRH_BS_CFG_SYS_LE_CENTRAL_DEVICE == 1)
u_int8 GAP_Connection_Establishment(u_int8 mode, u_int8* peer_address,u_int8 peer_address_type,
		                            u_int16 max_conn_int, u_int16 min_conn_int, u_int16 latency,u_int16 super_to);
#endif
u_int8  GAP_Find_Connectable_Devices(void);
u_int8  GAP_Connection_Complete(t_LE_Connection* p_connection, u_int8 status);
t_error LEgap_Handle_Encryption_Change_Event(t_LE_Connection* p_connection,u_int8 status, u_int8 mode );

u_int8 GAP_Init(void);
void   GAP_Check_GAP_Timers(void);
void   GAP_Register_Callbacks(void* event_funct_ptr, void* command_complete_funct_ptr);
void   GAP_DeRegister_Callbacks(void);
u_int8 GAP_Handle_Advertising_Events(t_advert_event* p_Advert_Event);
void   GAP_Clear_Connect_Params_Update_Pending(void);
u_int8 GAP_Set_Connect_Params_Update_Pending(u_int16 handle);
void GAP_Disconnect(u_int16 handle);
t_error GAP_Set_Privacy(u_int8 privacy);
u_int8 GAP_Set_Advertising_Interval(u_int32 min, u_int32 max);
#endif
