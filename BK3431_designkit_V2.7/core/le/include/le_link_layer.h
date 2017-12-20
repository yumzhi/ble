
#ifndef _CEVA_LE_LINK_LAYER__
#define _CEVA_LE_LINK_LAYER__

/*********************************************************************
 * MODULE NAME:     le_link_layer.h
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

#include "le_connection.h"

typedef enum
{
	LL_CONNECTION_UPDATE_REQ = 0,
	LL_CHANNEL_MAP_REQ       = 1,
	LL_TERMINATE_IND         = 2,
	LL_ENC_REQ               = 3,
	LL_ENC_RSP               = 4,
	LL_START_ENC_REQ         = 5,
	LL_START_ENC_RSP         = 6,
	LL_UNKNOWN_RSP           = 7,
	LL_FEATURE_REQ           = 8,
	LL_FEATURE_RSP           = 9,
	LL_PAUSE_ENC_REQ         = 10,
	LL_PAUSE_ENC_RSP         = 11,
	LL_VERSION_IND           = 12,
	LL_REJECT_IND            = 13,
	LL_NO_OPERATION			 = 14  // For the case when no operation pending
} t_LE_LL_Control_PDU_Opcodes;

#define LL_CONNECTION_UPDATE_PENDING 0x01
#define LL_CHANNEL_MAP_REQ_PENDING   0x02
#define LL_TERMINATE_PENDING         0x04
#define LL_ENC_REQ_PENDING           0x08
#define LL_ENCRYPTION_TRANSITIONING  0x10
#define LL_FEATURE_REQ_PENDING       0x20
#define LL_PAUSE_ENC_REQ_PENDING     0x40
#define LL_VERSION_IND_PENDING       0x80

#define SEL_CANDIDATE_COUNT_LIMIT    10
typedef enum
{
	ADV_IND                 = 0,
	ADV_DIRECT_IND          = 1,
	ADV_NONCONN_IND         = 2,
	SCAN_REQ                = 3,
	SCAN_RSP                = 4,
	CONNECT_REQ             = 5,
	ADV_SCAN_IND       		= 6,   // Was ADV_DISCOVER_IND
    DUMMY_TYPE              = 7    //reserve type

} t_LE_Advertising_Channel_PDU_Types;

typedef enum
{
	SUBEVENT_VOID                 = 0,
	SUBEVENT_CONNECTION_COMPLETE  = 1,
	SUBEVENT_ADVERTISING_REPORT   = 2,
	SUBEVENT_CONNECTION_UPDATE_COMPLETE = 3,
	SUBEVENT_READ_REMOTE_USED_FEATURES_COMPLETE = 4,
	SUBEVENT_LONG_TERM_KEY_REQUEST = 5
} t_LE_subevents;

typedef struct
{
	u_int8 event_type;
	u_int8 addressType;
	u_int8 address[6];
	u_int8 lenData;
	u_int8 data[31];
	s_int8 rssi;
	u_int8 write_complete;

} t_advert_event;

typedef struct
{
	u_int8 add_diff : 1;
	u_int8 display_Candidate_array : 1;
	u_int8 dummy : 6;
}t_need_todo;
#ifdef AUX_CALIBRATE
extern volatile u_int8  connect_event_count_Candidate;
#endif
#define LE_LL_RESPONSE_TIMEOUT  0x1F400   /* 40.00 Seconds (BT_CLOCK_TICKS_PER_SECOND * 40) */////merge CEVA 0517
extern u_int32 conn_interval_clk_base;
extern u_int32 AUX_setting_value;
extern volatile u_int8 no_packet_recv_flag;
u_int8 Vaidate_Access_Address(u_int32 proposed_address);
t_error LE_LL_Ctrl_Handle_LE_IRQ(u_int8 IRQ_Type);
void LE_LL_Sleep(void);

void LE_LL_Init_Duplicates_List(void);
u_int8 LE_LL_Get_CRC_Err(void);
t_error LE_Decode_link_layer_PDU(t_LE_Connection* p_connection, t_p_pdu p_pdu);
void LE_LL_Encode_Own_Address_In_Payload(u_int8* p_payload,u_int8 address_type);
void LE_LL_Handle_LLC_Ack(t_LE_Connection* p_connection, u_int8 opcode);
t_error LE_LL_Check_For_Pending_Advertising_Reports(void);
void LE_LL_Check_For_Pending_Connection_Completes(void);
void LE_LL_Set_Adv_Header(u_int8 type, u_int8 tx_add, u_int8 rx_add, u_int8 length);
t_error LEll_Encode_Advertising_ChannelPDU(u_int8 pdu_type, const u_int8* pAddress,
										 u_int8 RxAdd, u_int8* pLLdata);
u_int8 LEll_Decode_Advertising_ChannelPDU(t_p_pdu p_pdu,u_int8 rssi,u_int8 length);

void LE_LL_Check_Timers(void);//merge CEVA 0517
void LE_LL_Response_Timeout(t_LE_Connection* p_connection);//merge CEVA 0517
u_int8 Get_need_to_Idle(void);
void Set_need_to_Idle(void);
void Reset_transceiver(void);
#define LE_LL_Get_CRC_Err(void)  HWle_get_crc_err(void)//merge CEVA 0517

#endif

