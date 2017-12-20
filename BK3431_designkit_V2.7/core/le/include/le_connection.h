#ifndef _CEVA_LE_CONNECTION__
#define _CEVA_LE_CONNECTION__
#include "sys_config.h"
/*********************************************************************
 * MODULE NAME:     le_connection.h
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
typedef struct t_LEpairingParams {
    u_int8 io_cap;
    u_int8 oob_data_delivery;
    u_int8 auth_requirements;
    u_int8 max_encry_key_size;
    //u_int8 InitiatorKeyDistribution;
    //u_int8 ResponderKeyDistribution;
} t_LEpairingParams;


typedef enum le_security_state {
    LE_ENCRYPTION_NOT_ACTIVE            = 0x00,
    LE_ENCRYPTION_ACTIVE             = 0x01,
    LE_ENCRYPTION_PAUSED                = 0x02,
    LE_W4_ENC_REQ                    = 0x04,
    LE_W4_ENC_RSP                    = 0x08,
    LE_W4_START_ENC_REQ                = 0x10,
    LE_W4_START_ENC_RSP                = 0x20,
    LE_W4_PAUSE_ENC_RSP                = 0x40,
    LE_W4_LTK_REPLY                    = 0x80
}t_le_security_state;


typedef struct t_connect_params {//merge CEVA 0517
    u_int8 filter_policy;//merge CEVA 0517
    u_int16 conn_interval_min;//merge CEVA 0517
    u_int16 conn_interval_max;//merge CEVA 0517
    u_int16 latency;//merge CEVA 0517
    u_int16 scan_interval;//merge CEVA 0517
    u_int16 scan_window;//merge CEVA 0517
    u_int8 connection_own_address_type;//merge CEVA 0517
    u_int8 peer_address_type;//merge CEVA 0517
    u_int8* p_peer_address;//merge CEVA 0517
    u_int16 supervision_timeout;//merge CEVA 0517
} t_connect_params;//merge CEVA 0517

//#define mLEconnection_Get_Role(p_connection)  (p_connection->status & 0x01)

typedef struct
{
    //*********************************************************************************
    // Link Status
    // Bit 0   :- active             - indicates if a link is active
    // Bit 1   :- own_address_type   - devices own address type Public/Random
    // Bit 2   :- peer_address_type  - peer devices address type Public/Random
    // Bit 3   :- role               - Slave or Master on the link.
    // Bit 4   :- terminate_pending  - Indicates if a terminate is pending on the link
    // Bit 5-7 :- sub_state          - Indicates the device connection sub-state
    //*********************************************************************************

#if 0 // Optimisation not fully tested yet - (LINK_STATUS_BITFIELD==1)//merge CEVA 0517
    u_int8  link_status;//merge CEVA 0517
#else//merge CEVA 0517
    u_int8 active;//merge CEVA 0517
    u_int8 own_address_type;//merge CEVA 0517
    u_int8 peer_address_type;//merge CEVA 0517
    u_int8 role;//merge CEVA 0517
    u_int8 sub_state;//merge CEVA 0517
#endif//merge CEVA 0517
    //*********************************************************************************//merge CEVA 0517
    // CE Tx_Rx_Status
    // Bit 0     :- transmitSeqNum
    // Bit 1     :- nextExpectedSeqNum
    // Bit 2     :- rx_md
    // Bit 3     :- ack_pending
    // Bit 4     :- intitial_anchor_point_obtained
    // Bit 5     :- first_sync_in_connection_event
    //*********************************************************************************///merge CEVA 0517

#if 0 // Optimisation not fully tested yet - (TX_RX_STATUS_BITFIELD==1)//merge CEVA 0517
    u_int8 Tx_Rx_status;//merge CEVA 0517
#else//merge CEVA 0517
    u_int8 transmitSeqNum;//merge CEVA 0517
    u_int8 nextExpectedSeqNum;//merge CEVA 0517
    u_int8 rx_md;//merge CEVA 0517
    u_int8 ack_pending;//merge CEVA 0517
    u_int8 intitial_anchor_point_obtained;//merge CEVA 0517
    u_int8 first_sync_in_connection_event;//merge CEVA 0517
#endif//merge CEVA 0517
    u_int8 prev_crc;
    t_bd_addr peer_address;
    u_int8 device_index;
    u_int8 terminate_reason;
    // Not used//merge CEVA 0517
    //u_int16 minimum_CE_length; // ?//merge CEVA 0517
    u_int16 maximum_CE_length; // ?
    u_int8 ll_action_pending;
    u_int32 access_address;
    u_int32 crc_init;
    u_int16 connInterval;
    u_int16 connInterval_new;

    u_int8 transmit_window_size;
    u_int8 transmit_window_size_new;
    u_int8 transmit_window_remaining;
    u_int16 transmit_window_offset;
    u_int16 transmit_window_offset_new;
    u_int16 timeout;
    u_int16 timeout_new;

    u_int32 supervision_TO_timer;
    u_int16 latency;
    u_int16 latency_new;
    u_int16 latency_count;
    u_int8 data_channel_map[5];
    u_int8 channel_map_new[5];

    u_int8 hop_increment; // 5 Bits
    u_int8 sleep_clock_accuracy; // 3 Bits


    u_int16 connection_update_instant;
    u_int16 channel_map_update_instant;

    u_int8 lastUnmapped_Channel; // 6 Bits

    u_int8 channelRemapTable[37];
    u_int8 numUsedChannels;  // 6 Bits
    //merge CEVA 0517 remove u_int8 role;,u_int8 sub_state,u_int8 num_pkd;
    u_int8 link_id; // 4 Bits    // zzzzzzzzzzzzzzzzzzzzzzzzzzz
    u_int8 connection_event_rf_channel;
    u_int16 connection_event_counter;
    u_int16 connection_event_of_last_rx;
    u_int8 pdu_pending_length; // 4 bits

    u_int8 current_data_channel; // 5 bits
    struct q_desc* pqD;
    u_int8 peer_features; // Reduced to a single byte (instead of 8) as bytes 1-7 should be zero.
    u_int32 next_connect_event_start_time;
    u_int32 connection_event_max_length;

    u_int32 terminate_timer;
    u_int32 ll_response_timer;//merge CEVA 0517
    //*********************************************************************************
    // CE Tx_Rx_Status
    // Bit 0     :- transmitSeqNum
    // Bit 1     :- nextExpectedSeqNum
    // Bit 2     :- rx_md
    // Bit 3     :- ack_pending////merge CEVA 0517
    // Bit 4-7   :- pdu_pending_length
    //*********************************************************************************

    //*********************************************************************************
    // CE Control Status
    // Bit 0     :- first_sync_in_connection_event
    // Bit 1     :- intitial_anchor_point_obtained
    // Bit 2     :- UnUsed
    // Bit 3-7   :- current_data_channel
    //*********************************************************************************

    s_int8 rssi;

    // 129  bytes to here !!
#if (PRH_BS_CFG_SYS_ENCRYPTION_SUPPORTED==1)
    // Security values
    u_int32 on_air_tx_pkt_count;
    u_int32 enc_tx_pkt_counter;
    u_int32 on_air_rx_pkt_count;

    t_le_security_state current_security_state;

    u_int32 u32session_key[4];
    u_int8*  session_key;
    u_int32 u32IV[2];
    u_int8*  IV;   /* initialization vector */
    u_int8  IVnew[4]; /* Master component of new initialisation vector used in ReStart encryption */
#endif

#if (PRH_BS_CFG_SYS_LE_SMP_INCLUDED==1)
    //u_int8 EI_long_term_key[16];  // Check requirement for this//merge CEVA 0517
    t_LEpairingParams remote_pairing_params;
    u_int8 SMP_sec_state;

    u_int8 pres[7];
    u_int8 preq[7];
    u_int8 SMP_Enc_Active;//merge CEVA 0517
    u_int8 STK_GenerationMethod;
    u_int8 SMP_InitiatorKeyDistribution;
    u_int8 SMP_ResponderKeyDistribution;
    u_int8 SMP_local_rand[16];  // Move to SMP scratch-pad//merge CEVA 0517
    u_int8 SMP_peerConfirm[16]; // Move to SMP scratch-pad//merge CEVA 0517
    u_int8 SMP_localConfirm[16]; // Move to SMP scratch-pad//merge CEVA 0517
    u_int8 SMP_key_type;
    u_int8 SMP_STK[16];          // Mode to SMP scratch-pad//merge CEVA 0517
    u_int16 SMP_EDIV;             // Not required - store in DB//merge CEVA 0517
    //u_int8 SMP_LTK[16]; - Not Used
    u_int32 SMP_timer;
    u_int8 encry_key_size;

    u_int8 SMP_passkey_io;
    u_int8 SMP_Mconfirm_rxed;
    u_int8 SMP_Address_Type;
    t_bd_addr SMP_bd_addr;
#endif

#if (PRH_BS_CFG_SYS_LE_GATT_INCLUDED==1)//merge CEVA 0517
    u_int32 GATT_timer;//merge CEVA 0517
#endif//merge CEVA 0517
} t_LE_Connection;


typedef enum
    {
        STANDBY_STATE     = 0,
        ADVERTISING_STATE = 1,
        SCANNING_STATE    = 2,
        INITIATING_STATE  = 3,
        CONNECTION_STATE  = 4,
        TEST_STATE            = 5,
        PRE_SLEEP_STATE   = 6, //T05 add
        WAKEUP_STATE      = 7,  //T05 add
        WAIT_SLEEP_STATE  = 8,
        SLEEP_STATE       = 9
    } t_LE_link_layer_state;


typedef enum
    {
        CONNECTION_IDLE = 0,
        CONNECTION_CREATED = 1,
        CONNECTION_ESTABLISHED = 2
    } t_LE_connection_substates;


#define DEAULT_MIN_CE_LENGTH 0x04//merge CEVA 0517
#define DEAULT_MAX_CE_LENGTH 0x10//merge CEVA 0517

/********************************************************************************
 * MACROs For Handling Link Elements
 ********************************************************************************/

//*********************************************************************************
// Link Status
// Bit 0   :- active             - indicates if a link is active
// Bit 1   :- own_address_type   - devices own address type Public/Random
// Bit 2   :- peer_address_type  - peer devices address type Public/Random
// Bit 3   :- role               - Slave or Master on the link.
// Bit 4   :- terminate_pending  - Indicates if a terminate is pending on the link
// Bit 5-7 :- sub_state          - Indicates the device connection sub-state
//*********************************************************************************

#if 0 // Optimisation not fully tested yet - (LINK_STATUS_BITFIELD==1)//merge CEVA 0517
#define LEconnection_Set_Link_Active(p_connection)    (p_connection->link_status |= 0x01)
#define LEconnection_Clear_Link_Active(p_connection)  (p_connection->link_status |= ~0x01)
#define LEconnection_Get_Link_Active(p_connection)     (p_connection->link_status & 0x01)

#define LEconnection_Set_Own_Address_Type(p_connection)    (p_connection->link_status |= 0x02)
#define LEconnection_Clear_Own_Address_Type(p_connection)  (p_connection->link_status |= ~0x02)
#define LEconnection_Get_Own_Address_Type(p_connection)     (p_connection->link_status & 0x02)

#define LEconnection_Set_Peer_Address_Type(p_connection)    (p_connection->link_status |= 0x04)
#define LEconnection_Clear_Peer_Address_Type(p_connection)  (p_connection->link_status |= ~0x04)
#define LEconnection_Get_Peer_Address_Type(p_connection)     (p_connection->link_status & 0x04)

#define LEconnection_Set_Role(p_connection)    (p_connection->link_status |= 0x08)
#define LEconnection_Clear_Role(p_connection)  (p_connection->link_status |= ~0x08)
#define LEconnection_Get_Role(p_connection)     (p_connection->link_status & 0x08)

#define LEconnection_Set_Terminate_Pending(p_connection)    (p_connection->link_status |= 0x10)
#define LEconnection_Clear_Terminate_Pending(p_connection)  (p_connection->link_status |= ~0x10)
#define LEconnection_Get_Terminate_Pending(p_connection)     (p_connection->link_status & 0x10)

#define LEconnection_Set_Substate(p_connection,substate)    (p_connection->link_status |= (substate<<5)))
#define LEconnection_Get_Substate(p_connection)     ((p_connection->link_status & 0xE0) >> 5)

#endif//merge CEVA 0517

t_error LEconnection_Create_Connection(t_p_pdu p_pdu);
t_error LEconnection_Create_Connection_Cancel(t_p_pdu p_pdu);
t_error LEconnection_HCI_Connection_Update(t_p_pdu p_pdu);
t_error LEconnection_Read_Remote_Used_Features(u_int16 handle);
t_error LEconnection_Read_Remote_Version_Info(u_int16 handle);
u_int8 LEconnection_Is_Valid_Handle(u_int16 handle);
u_int8 LE_Connection_Extact_Connection_Req_Parameters(t_LE_Connection* p_connection,t_p_pdu p_payload);
t_error LEconnection_Generate_Connection_Complete(u_int8 link_id,t_error status); //  ???????
void LEconnection_Handle_Connection_Event(u_int8 link_id,u_int32 current_clk);//merge CEVA 0517

u_int8 LEconnections_Link_In_Use(u_int8 link_id);
t_LE_Connection*  LEconnection_Find_P_Connection(u_int16 handle);
#define LEconnection_Find_LE_Link(handle)  LEconnection_Find_P_Connection(handle)
t_LE_Connection* LEconnection_Find_Link_Entry(u_int8 link_id);
t_error LEconnection_Find_Link_Id(u_int16 handle,u_int8* p_link_id);
u_int16 LEconnection_Find_Handle(u_int8 device_index);
void LEconnection_Init(void);
void LEconnection_Setup_First_Connection_Event(u_int8 link_id,u_int32 current_clk);
void LEconnection_Disconnect(t_LE_Connection* p_connection,u_int8 reason);//merge CEVA 0517
void LEconnection_Terminate_Ack(t_deviceIndex device_index);
void LEconnection_Local_Disconnect(t_LE_Connection* p_connection,u_int8 reason);//merge CEVA 0517

__INLINE__ u_int8 LEconnection_Connection_Event_Still_Open(t_LE_Connection* p_connection , u_int32 current_clk);

t_error LEconnection_Connection_Update(t_LE_Connection* p_connection);//merge CEVA 0517
t_error LEllc_Encode_PDU(u_int8 opcode,t_LE_Connection* p_connection, u_int8 param1);
t_LE_Connection* LEconnection_Find_Device_Index(u_int8 device_index);
t_error LEconnection_Decrypt_Incoming_Pdu(t_LE_Connection* p_connection,u_int8* payload,u_int8 length,u_int32 rx_pkt_counter,u_int8 llid);
void LEconnection_Set_HW_Pkt_Counter(u_int32 pkt_count);
u_int8 LEconnection_Encrypt_Payload(t_LE_Connection* p_connection,u_int8* payload,u_int8 length,u_int8 llid,u_int32 tx_pkt_count);
void LEconnection_Initiator_Scan_Interval_Timeout(u_int32 current_clk);
boolean LEconnection_Does_Slave_Link_Exist(void);
u_int32  LEconnection_Find_Next_Connection_Event(u_int32 cur_clk);
void LEconnection_Handle_Data_To_Encrypt(void);
t_error LEeventgen_Generate_LE_Meta_Event(u_int8 event_id,t_LE_Connection* p_connection,t_error status);
u_int8 LEconnection_Check_Current_Address(t_LE_Connection* p_connection,u_int8 RxAdd, u_int8* p_pdu);
void LEconnection_Encode_CONNECT_REQ_pdu(void);//merge CEVA 0517

#if (PRH_BS_CFG_SYS_EXTENDED_HCI_COMMANDS_SUPPORTED==1)
t_error LEconnetion_TCI_Read_Access_Code(u_int16 handle);
t_error LEconnetion_TCI_Read_Hop_Increment(u_int16 handle);
t_error LEconnection_TCI_Read_Peer_SCA(u_int16 handle);
t_error LEconnection_TCI_Read_Session_Key(u_int16 handle);
t_error LEconnection_TCI_Set_Transmit_Window_Params(u_int16 offset,u_int8 size);
t_error LEconnection_TCI_Read_Window_Size_and_Offset(u_int16 handle);//merge CEVA 0517
#endif//merge CEVA 0517

#if (PRH_BS_CFG_SYS_LE_GAP_INCLUDED==1)//merge CEVA 0517
t_error LEconnection_GAP_Create_Connection(t_connect_params* p_conn_params);//merge CEVA 0517
u_int8 LEconnection_GAP_Create_Connection_Cancel(void);//merge CEVA 0517
#endif//merge CEVA 0517

#if (PRH_BS_CFG_SYS_LE_L2CAP_INCLUDED==1)//merge CEVA 0517
t_error LEconnection_L2CAP_Connection_Update(u_int16 handle,u_int16 interval_max,u_int16 interval_min,
                                             u_int16 latency,u_int16 timeout);//merge CEVA 0517

#endif//merge CEVA 0517


#if 0 // Optimisation not fully tested yet - (TX_RX_STATUS_BITFIELD==1)//merge CEVA 0517
//*********************************************************************************
// Link Status
// Bit 0   :- active             - indicates if a link is active
// Bit 1   :- own_address_type   - devices own address type Public/Random
// Bit 2   :- peer_address_type  - peer devices address type Public/Random
// Bit 3   :- role               - Slave or Master on the link.
// Bit 4-7 :- sub_state          - Indicates the device connection sub-state
//*********************************************************************************

#define LINK_STATUS_ACTIVE_SHFT  0x00
#define LINK_STATUS_ACTIVE_MASK  0x01

#define LINK_STATUS_OWN_ADDR_TYPE_SHFT 0x01
#define LINK_STATUS_OWN_ADDR_TYPE_MASK 0x02

#define LINK_STATUS_PEER_ADDR_TYPE_SHFT 0x02
#define LINK_STATUS_PEER_ADDR_TYPE_MASK 0x04

#define LINK_STATUS_ROLE_SHFT 0x03
#define LINK_STATUS_ROLE_MASK 0x08

#define LINK_STATUS_SUB_STATE_SHFT 0x04
#define LINK_STATUS_SUB_STATE_MASK 0xF0


#define LEconnection_Is_Active(p_connection)           (p_connection->link_status & LINK_STATUS_ACTIVE_MASK)
#define LEconnection_Set_Active(p_connection,value)    (p_connection->link_status = ((((value) << LINK_STATUS_ACTIVE_SHFT) \
                                                                                      & LINK_STATUS_ACTIVE_MASK)  | (p_connection->link_status & ~LINK_STATUS_ACTIVE_MASK)))

#define LEconnection_Get_Own_Addr_Type(p_connection)         ((p_connection->link_status & LINK_STATUS_OWN_ADDR_TYPE_MASK )>> LINK_STATUS_OWN_ADDR_TYPE_SHFT)
#define LEconnection_Set_Own_Addr_Type(p_connection,value)    (p_connection->link_status = ((((value) << LINK_STATUS_OWN_ADDR_TYPE_SHFT) \
                                                                                             & LINK_STATUS_OWN_ADDR_TYPE_MASK)  | (p_connection->link_status & ~LINK_STATUS_OWN_ADDR_TYPE_MASK)))

#define LEconnection_Get_Peer_Addr_Type(p_connection)         ((p_connection->link_status & LINK_STATUS_PEER_ADDR_TYPE_MASK )>> LINK_STATUS_PEER_ADDR_TYPE_SHFT)
#define LEconnection_Set_Peer_Addr_Type(p_connection,value)   (p_connection->link_status = ((((value) << LINK_STATUS_PEER_ADDR_TYPE_SHFT) \
                                                                                             & LINK_STATUS_PEER_ADDR_TYPE_MASK)  | (p_connection->link_status & ~LINK_STATUS_PEER_ADDR_TYPE_MASK)))


#define LEconnection_Get_Role(p_connection)    ((p_connection->link_status & LINK_STATUS_ROLE_MASK) >> LINK_STATUS_ROLE_SHFT)
#define LEconnection_Set_Role(p_connection,value) (p_connection->link_status = ((((value) << LINK_STATUS_ROLE_SHFT) \
                                                                                   & LINK_STATUS_ROLE_MASK)  | (p_connection->link_status & ~LINK_STATUS_ROLE_MASK)))


#define LEconnection_Get_Substate(p_connection)       ((p_connection->link_status & LINK_STATUS_SUB_STATE_MASK) >> LINK_STATUS_SUB_STATE_SHFT)
#define LEconnection_Set_Substate(p_connection,value) (p_connection->link_status = ((((value) << LINK_STATUS_SUB_STATE_SHFT) \
                                                                                     & LINK_STATUS_SUB_STATE_MASK)  | (p_connection->link_status & ~LINK_STATUS_SUB_STATE_MASK)))

#else
// Bit 0   :- active             - indicates if a link is active
// Bit 1   :- own_address_type   - devices own address type Public/Random
// Bit 2   :- peer_address_type  - peer devices address type Public/Random
// Bit 3   :- role               - Slave or Master on the link.
// Bit 4-7 :- sub_state

#define LEconnection_Is_Active(p_connection)                  (p_connection->active)
#define LEconnection_Set_Active(p_connection,value)           (p_connection->active = value)

#define LEconnection_Get_Own_Addr_Type(p_connection)          (p_connection->own_address_type)
#define LEconnection_Set_Own_Addr_Type(p_connection,value)    (p_connection->own_address_type = value)

#define LEconnection_Get_Peer_Addr_Type(p_connection)         (p_connection->peer_address_type)
#define LEconnection_Set_Peer_Addr_Type(p_connection,value)   (p_connection->peer_address_type = value)

#define LEconnection_Get_Role(p_connection)                   (p_connection->role)
#define LEconnection_Set_Role(p_connection,value)             (p_connection->role = value)

#define LEconnection_Get_Substate(p_connection)                  (p_connection->sub_state)
#define LEconnection_Set_Substate(p_connection,value)         (p_connection->sub_state = value)

#endif//merge CEVA 0517

//*********************************************************************************
// CE Tx_Rx_Status
// Bit 0     :- transmitSeqNum
// Bit 1     :- nextExpectedSeqNum
// Bit 2     :- rx_md
// Bit 3     :- ack_pending
// Bit 4     :- intitial_anchor_point_obtained
// Bit 5     :- first_sync_in_connection_event
//*********************************************************************************

#if 0 // Optimisation not fully tested yet - (TX_RX_STATUS_BITFIELD==1)//merge CEVA 0517
#define TX_RX_TX_SEQ_NUM_SHFT         0x00
#define TX_RX_TX_SEQ_NUM_MASK         0x01

#define TX_RX_NEXT_EXP_SEQ_NUM_SHFT   0x01
#define TX_RX_NEXT_EXP_SEQ_NUM_MASK   0x02

#define TX_RX_RX_MD_SHFT              0x02
#define TX_RX_RX_MD_MASK              0x04

#define TX_RX_ACK_PENDING_SHFT        0x03
#define TX_RX_ACK_PENDING_MASK        0x08

#define TX_RX_INIT_ANCHOR_POINT_SHFT  0x04
#define TX_RX_INIT_ANCHOR_POINT_MASK  0x10

#define TX_RX_FIRST_SYNC_IN_CE_SHFT   0x05
#define TX_RX_FIRST_SYNC_IN_CE_MASK   0x20

#define LEconnection_Get_TxSeqNum(p_connection)                      (p_connection->Tx_Rx_status & TX_RX_TX_SEQ_NUM_MASK)
#define LEconnection_Set_TxSeqNum(p_connection,value)               (p_connection->Tx_Rx_status = ((((value) << TX_RX_TX_SEQ_NUM_SHFT) \
                                                                                                    & TX_RX_TX_SEQ_NUM_MASK)  | (p_connection->Tx_Rx_status & ~TX_RX_TX_SEQ_NUM_MASK)))

#define LEconnection_Get_NextExpSeqNum(p_connection)                ((p_connection->Tx_Rx_status & TX_RX_NEXT_EXP_SEQ_NUM_MASK) >> TX_RX_NEXT_EXP_SEQ_NUM_SHFT)
#define LEconnection_Set_NextExpSeqNum(p_connection,value)           (p_connection->Tx_Rx_status = ((((value) << TX_RX_NEXT_EXP_SEQ_NUM_SHFT) \
                                                                                                     & TX_RX_NEXT_EXP_SEQ_NUM_MASK)  | (p_connection->Tx_Rx_status & ~TX_RX_NEXT_EXP_SEQ_NUM_MASK)))

#define LEconnection_Toggle_NextExpSeqNum(p_connection)              (p_connection->Tx_Rx_status =(((( ^((p_connection->Tx_Rx_status & TX_RX_NEXT_EXP_SEQ_NUM_MASK) >> TX_RX_NEXT_EXP_SEQ_NUM_SHFT)) << TX_RX_NEXT_EXP_SEQ_NUM_SHFT) \
                                                                                                    & TX_RX_NEXT_EXP_SEQ_NUM_MASK)  | (p_connection->Tx_Rx_status & ~TX_RX_NEXT_EXP_SEQ_NUM_MASK)))

#define LEconnection_Get_Rx_Md(p_connection)                        ((p_connection->Tx_Rx_status & TX_RX_RX_MD_MASK) >> TX_RX_RX_MD_SHFT)
#define LEconnection_Set_Rx_Md(p_connection,value)                   (p_connection->Tx_Rx_status = ((((value) << TX_RX_RX_MD_SHFT) \
                                                                                                     & TX_RX_RX_MD_MASK)  | (p_connection->Tx_Rx_status & ~TX_RX_RX_MD_MASK)))

#define LEconnection_Get_Ack_Pending(p_connection)                  ((p_connection->Tx_Rx_status & TX_RX_ACK_PENDING_MASK) >> TX_RX_ACK_PENDING_SHFT)
#define LEconnection_Set_Ack_Pending(p_connection,value)             (p_connection->Tx_Rx_status = ((((value) << TX_RX_ACK_PENDING_SHFT) \
                                                                                                     & TX_RX_ACK_PENDING_MASK)  | (p_connection->Tx_Rx_status & ~TX_RX_ACK_PENDING_MASK)))

#define LEconnection_Get_Initial_Anchor_Point_Obtained(p_connection)  ((p_connection->Tx_Rx_status & TX_RX_INIT_ANCHOR_POINT_MASK) >> TX_RX_INIT_ANCHOR_POINT_SHFT)
#define LEconnection_Set_Initial_Anchor_Point_Obtained(p_connection,value)  (p_connection->Tx_Rx_status = ((((value) << TX_RX_INIT_ANCHOR_POINT_SHFT) \
                                                                                                            & TX_RX_INIT_ANCHOR_POINT_MASK)  | (p_connection->Tx_Rx_status & ~TX_RX_INIT_ANCHOR_POINT_MASK)))

#define LEconnection_Get_First_Sync_In_Connection_Event(p_connection)      ((p_connection->Tx_Rx_status & TX_RX_FIRST_SYNC_IN_CE_MASK) >> TX_RX_FIRST_SYNC_IN_CE_SHFT)
#define LEconnection_Set_First_Sync_In_Connection_Event(p_connection,value)  (p_connection->Tx_Rx_status = ((((value) << TX_RX_FIRST_SYNC_IN_CE_SHFT) \
                                                                                                             & TX_RX_FIRST_SYNC_IN_CE_MASK)  | (p_connection->Tx_Rx_status & ~TX_RX_FIRST_SYNC_IN_CE_MASK)))
#else

#define LEconnection_Get_TxSeqNum(p_connection)                      (p_connection->transmitSeqNum)
#define LEconnection_Set_TxSeqNum(p_connection,value)               (p_connection->transmitSeqNum = value)

#define LEconnection_Get_NextExpSeqNum(p_connection)                (p_connection->nextExpectedSeqNum)
#define LEconnection_Set_NextExpSeqNum(p_connection,value)           (p_connection->nextExpectedSeqNum = value)

#define LEconnection_Toggle_NextExpSeqNum(p_connection)             ( p_connection->nextExpectedSeqNum ^= 0x01)

#define LEconnection_Get_Rx_Md(p_connection)                        (p_connection->rx_md)
#define LEconnection_Set_Rx_Md(p_connection,value)                  (p_connection->rx_md = value)

#define LEconnection_Get_Prev_CRC(p_connection)                        (p_connection->prev_crc)
#define LEconnection_Set_Prev_CRC(p_connection,value)                  (p_connection->prev_crc = value)


#define LEconnection_Get_Ack_Pending(p_connection)                  (p_connection->ack_pending)
#define LEconnection_Set_Ack_Pending(p_connection,value)             (p_connection->ack_pending = value)

#define LEconnection_Get_Initial_Anchor_Point_Obtained(p_connection)  (p_connection->intitial_anchor_point_obtained)
#define LEconnection_Set_Initial_Anchor_Point_Obtained(p_connection,value)  (p_connection->intitial_anchor_point_obtained = value)

#define LEconnection_Get_First_Sync_In_Connection_Event(p_connection)      (p_connection->first_sync_in_connection_event)
#define LEconnection_Set_First_Sync_In_Connection_Event(p_connection,value)  (p_connection->first_sync_in_connection_event = value)

#endif//merge CEVA 0517
#endif
