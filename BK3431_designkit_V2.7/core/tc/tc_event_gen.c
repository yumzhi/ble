/*************************************************************************
 *
 * MODULE NAME:    tc_event_gen.c
 * PROJECT CODE:    BlueStream
 * DESCRIPTION:    TCI Event Generator
 * MAINTAINER:     Daire McNamara <Daire.McNamara@sslinc.com>
 * CREATION DATE:  13 April 2000
 *
 * SOURCE CONTROL: $Id: tc_event_gen.c,v 1.4 2012/05/14 23:44:14 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2004 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *
 * SOURCE: Based on V1.0 HC Design
 * ISSUES:
 * NOTES TO USERS:
 *  
 ************************************************************************/

#include "sys_config.h"

#include "hc_const.h"
#include "hc_event_gen.h"
#include "hw_radio.h"
#include "tc_const.h"
#include "tc_event_gen.h"
#include "tc_interface.h"
#include "tra_queue.h"
#include "hw_lc.h"//merge CEVA 0517
#include "hw_le_lc.h"//merge CEVA 0517
#include "hw_memcpy.h"
#include "hw_jal_defs.h"
#include "lmp_utils.h"
#if (PRH_BS_CFG_SYS_TRACE_LLC_VIA_HCI_SUPPORTED==1)
#include "hw_lc.h"
#endif
#include "le_scan.h"
#include "le_advertise.h"
#if ((PRH_BS_CFG_SYS_TRACE_LE_DEVICE_STATE_VIA_HCI_SUPPORTED == 1) ||  (PRH_BS_CFG_SYS_TRACE_ADV_VIA_HCI_SUPPORTED==1) || \
	 (PRH_BS_CFG_SYS_TRACE_LLC_VIA_HCI_SUPPORTED==1) || (PRH_BS_CFG_SYS_TRACE_LE_DEVICE_STATE_VIA_HCI_SUPPORTED == 1) || \
	 (PRH_BS_CFG_SYS_TRACE_ATT_VIA_HCI_SUPPORTED==1) || (PRH_BS_CFG_SYS_TRACE_SMP_VIA_HCI_SUPPORTED == 1) )//merge CEVA 0517
#include "le_config.h"
#include "le_link_layer.h"

extern void _Insert_Uint16(t_p_pdu p_buffer, u_int16 value_16_bit);
extern void _Insert_Uint32(t_p_pdu p_buffer, u_int32 value_32_bit);
extern void _Insert_Uint24(t_p_pdu p_buffer,u_int32 val24);
extern t_LE_Config LE_config;
#endif
/*****************************************************************************
 *  FUNCTION :- TCeg_Command_Status_Event
 *
 *  DESCRIPTION :-
 *  This function creates an TCI Command Status Event and places it on
 *  outgoing HCI Queue. 
 *
 *****************************************************************************/
void TCeg_Command_Status_Event(t_error status, u_int16 opcode)
{
    u_int8 queue_length = 6; /* Length of a Status Event */
    t_p_pdu p_buffer = 0;
    struct q_desc* qd = 0;

    qd = BTq_Enqueue(HCI_EVENT_Q,0,queue_length);
    if(qd)
    {
        p_buffer=qd->data;
        *(p_buffer+0) = 0x0F; /* event code */
        *(p_buffer+1) = 4; /* length */
        *(p_buffer+2) = (u_int8)status;
        *(p_buffer+3) = 1;
        _Insert_Uint16(p_buffer+4, opcode);
        BTq_Commit(HCI_EVENT_Q,0);
    }
}

/*****************************************************************************
 *  FUNCTION :- TCI_Command_Complete_Event
 *
 *  DESCRIPTION :-
 *  This function creates an TCI Command Complete Event and places it on
 *  outgoing HCI Queue. 
 *
 *****************************************************************************/
void TCeg_Command_Complete_Event(u_int16 opcode, t_error status)
{
    u_int8 para_length;
    t_p_pdu p_buffer = 0;
    u_int8 size_of_header = SIZE_OF_EVENT_HEADER + SIZE_OF_COMMAND_COMPLETE_HEADER;

    struct q_desc *qd = 0;

    /* the default command complete carries only a status parameter */
    para_length=1;
    qd = BTq_Enqueue(HCI_EVENT_Q, 0, (t_length)(para_length + size_of_header));
    if(qd) 
    {
        p_buffer = qd->data;
        _Insert_Uint8(p_buffer,HCI_COMMAND_COMPLETE_EVENT);
        _Insert_Uint8(p_buffer+1,(u_int8)(para_length+SIZE_OF_COMMAND_COMPLETE_HEADER));  
        _Insert_Uint8(p_buffer+2,1);
        _Insert_Uint16(p_buffer+3,opcode);
        _Insert_Uint8(p_buffer+5,(unsigned char)status);

    BTq_Commit(HCI_EVENT_Q,0);
    }
}

#if 0
/*****************************************************************************
 *  FUNCTION :- TCeg_Read_Pump_Monitors_Event
 *
 *  DESCRIPTION :-
 *  This function creates an TCI Read Pump Monitors Event and places it on
 *  outgoing HCI Queue. 
 *  The event returns all details about the data pump
 *****************************************************************************/
void TCeg_Read_Pump_Monitors_Event(t_BTpump_Log* pump)
{
    u_int8 length = 140;
    u_int8* p_buf;
    struct q_desc *qd;
    
    qd = BTq_Enqueue(HCI_EVENT_Q, 0, (t_length)(length + 2));
    if(qd)
    {
        p_buf = qd->data;
        p_buf[0] = 0xFF;          /* Reserved Event Code */
        p_buf[1] = length;        /* PDU Length          */
        p_buf[2] = 0x01;          /* pump event          */
        p_buf[3] = 0x00;          /* status              */

        LMutils_Set_Uint32(&(p_buf[4]), pump->pump_type);
        LMutils_Set_Uint32(&(p_buf[8]), pump->pump_options);
        LMutils_Set_Uint32(&(p_buf[12]), pump->packet_type);
        LMutils_Set_Uint32(&(p_buf[16]), pump->packet_length);
        LMutils_Set_Uint32(&(p_buf[20]), pump->target_device_index);
        LMutils_Set_Uint32(&(p_buf[24]), pump->handle);
        LMutils_Set_Uint32(&(p_buf[28]), pump->rx_expected_packet_num);
        LMutils_Set_Uint32(&(p_buf[32]), pump->tx_max_packets);
        LMutils_Set_Uint32(&(p_buf[36]), pump->TX_PDUS_ENQUEUED);
        LMutils_Set_Uint32(&(p_buf[40]), pump->TX_ENQUEUE_FULL_DETECTED);
        
        LMutils_Set_Uint32(&(p_buf[44]), pump->tx_start_time);
        LMutils_Set_Uint32(&(p_buf[48]), pump->tx_last_time);
        LMutils_Set_Uint32(&(p_buf[52]), pump->tx_bytes);
        LMutils_Set_Uint32(&(p_buf[56]), pump->tx_bytes_per_sec);
        LMutils_Set_Uint32(&(p_buf[60]), pump->tx_bits_per_sec);

        LMutils_Set_Uint32(&(p_buf[64]), pump->RX_PDUS_DEQUEUED);
        LMutils_Set_Uint32(&(p_buf[68]), pump->RX_CRC_ERRS_ON_DEQUEUE);
        LMutils_Set_Uint32(&(p_buf[72]), pump->RX_SEQ_ERRS_ON_DEQUEUE);
        LMutils_Set_Uint32(&(p_buf[76]), pump->RX_LAST_ERROR_PACKET_NUM_ON_DEQUEUE);

        LMutils_Set_Uint32(&(p_buf[80]), pump->rx_start_time);
        LMutils_Set_Uint32(&(p_buf[84]), pump->rx_last_time);
        LMutils_Set_Uint32(&(p_buf[88]), pump->rx_bytes);
        LMutils_Set_Uint32(&(p_buf[92]), pump->rx_bytes_per_sec);
        LMutils_Set_Uint32(&(p_buf[96]), pump->rx_bits_per_sec);
        LMutils_Set_Uint32(&(p_buf[100]), pump->rx_hw_crc_errors);
        LMutils_Set_Uint32(&(p_buf[104]), pump->rx_hw_hec_errors);
        LMutils_Set_Uint32(&(p_buf[108]), pump->rx_duplicates);
        LMutils_Set_Uint32(&(p_buf[112]), pump->rx_no_pkt_count);

        LMutils_Set_Uint32(&(p_buf[116]), pump->LSLCacc_NUM_PUMP_PDUS_TX);
        LMutils_Set_Uint32(&(p_buf[120]), pump->LSLCacc_NUM_PUMP_CRC_ERRS_BEFORE_TX);
        LMutils_Set_Uint32(&(p_buf[124]), pump->LSLCacc_NUM_PUMP_MSGTYPE_ERRS_AFTER_RX);
        LMutils_Set_Uint32(&(p_buf[128]), pump->LSLCacc_MIN_PUMP_LEN);
        LMutils_Set_Uint32(&(p_buf[132]), pump->LSLCacc_MAX_PUMP_LEN);
        LMutils_Set_Uint32(&(p_buf[136]), pump->LSLCacc_PUMP_LAST_ERROR_PACKET_NUM);
            
        BTq_Commit(HCI_EVENT_Q,0);
    }
}
#endif

#define mHWreg_Move_Cache_Register_To_Byte_Array(Register, Destination_Byte_Array) \
       {  u_int8 *dest = Destination_Byte_Array; \
          *dest++ = Cache_##Register ;     \
          *dest++ = Cache_##Register>>8 ;  \
          *dest++ = Cache_##Register>>16 ; \
          *dest = Cache_##Register>>24; }

#if (PRH_BS_CFG_SYS_EXTENDED_HCI_COMMANDS_SUPPORTED==1)
#if (PRH_BS_CFG_SYS_TRACE_LLC_VIA_HCI_SUPPORTED==1)
void TCeg_Send_LE_LLC_Event( const u_int8* le_llc_data, u_int8 le_llc_length, u_int8 tx0_rx1)
{
    int i;
    struct q_desc *qd_tci;

    /*
     * If currently active then send as special Event
     */
    if (LE_config.trace_level & 0x01) // Needs to be build based (SYSconfig_Is_Feature_Supported(PRH_BS_CFG_SYS_TRACE_LMP_VIA_HCI_FEATURE))
    {
        qd_tci = BTq_Enqueue(HCI_EVENT_Q, 0,8+le_llc_length);
        if(qd_tci)
        {
           qd_tci->data[0] = 0xFF;    /* TCI event code    */
           qd_tci->data[1] = 6+le_llc_length;      /* Max LLC msg Len   */
           qd_tci->data[2] = 0x22;    /* subcode           */
           qd_tci->data[3] = tx0_rx1; /* receive direction */
           for(i = 0; i < le_llc_length; i++)
           {
               qd_tci->data[i+4] = le_llc_data[i];
           }
       for(i = le_llc_length; i < 32; i++)
       {
           qd_tci->data[i+4] = 0;
       }
           BTq_Commit(HCI_EVENT_Q,0);
        }
    }
}
#endif

#if (PRH_BS_CFG_SYS_TRACE_SMP_VIA_HCI_SUPPORTED==1)//merge CEVA 0517
void TCeg_Send_LE_SMP_Event(u_int8* le_smp_data,u_int8 le_smp_length, u_int8 tx0_rx1)//merge CEVA 0517
{
    int i;//merge CEVA 0517
    struct q_desc *qd_tci;//merge CEVA 0517

    /*
     * If currently active then send as special Event
     */
    if (LE_config.trace_level & 0x20)//merge CEVA 0517 // Needs to be build based (SYSconfig_Is_Feature_Supported(PRH_BS_CFG_SYS_TRACE_LMP_VIA_HCI_FEATURE))
    {
        qd_tci = BTq_Enqueue(HCI_EVENT_Q, 0,8+le_smp_length);//merge CEVA 0517
        if(qd_tci)//merge CEVA 0517
        {
           qd_tci->data[0] = 0xFF;  //merge CEVA 0517  /* TCI event code    */
           qd_tci->data[1] = 6+le_smp_length; //merge CEVA 0517     /* Max SMP msg Len   */
           qd_tci->data[2] = 0x26; //merge CEVA 0517   /* subcode           */
           qd_tci->data[3] = tx0_rx1; //merge CEVA 0517/* receive direction */
           for(i = 0; i < le_smp_length; i++)//merge CEVA 0517
           {
               qd_tci->data[i+4] = le_smp_data[i];//merge CEVA 0517
           }
       for(i = le_smp_length; i < 32; i++)//merge CEVA 0517
       {
           qd_tci->data[i+4] = 0;//merge CEVA 0517
       }
           BTq_Commit(HCI_EVENT_Q,0);//merge CEVA 0517
        }
    }
}
#endif//merge CEVA 0517

#if (PRH_BS_CFG_SYS_TRACE_ATT_VIA_HCI_SUPPORTED==1)//merge CEVA 0517
void TCeg_Send_LE_GATT_Event(u_int8* le_gatt_data,u_int8 le_gatt_length, u_int8 tx0_rx1)//merge CEVA 0517
{
    int i;//merge CEVA 0517
    struct q_desc *qd_tci;//merge CEVA 0517

    /*
     * If currently active then send as special Event
     */
    if (LE_config.trace_level & 0x10) //merge CEVA 0517// Needs to be build based (SYSconfig_Is_Feature_Supported(PRH_BS_CFG_SYS_TRACE_LMP_VIA_HCI_FEATURE))
    {
        qd_tci = BTq_Enqueue(HCI_EVENT_Q, 0,8+le_gatt_length);//merge CEVA 0517
        if(qd_tci)
        {
           qd_tci->data[0] = 0xFF;//merge CEVA 0517    /* TCI event code    */
           qd_tci->data[1] = 6+le_gatt_length; //merge CEVA 0517     /* Max SMP msg Len   */
           qd_tci->data[2] = 0x27;  //merge CEVA 0517  /* subcode           */
           qd_tci->data[3] = tx0_rx1;//merge CEVA 0517 /* receive direction */
           for(i = 0; i < le_gatt_length; i++)//merge CEVA 0517
           {
               qd_tci->data[i+4] = le_gatt_data[i];//merge CEVA 0517
           }

           BTq_Commit(HCI_EVENT_Q,0);//merge CEVA 0517
        }
    }//merge CEVA 0517
}//merge CEVA 0517

#endif

#if (PRH_BS_CFG_SYS_TRACE_L2CAP_VIA_HCI_SUPPORTED==1)//merge CEVA 0517
void TCeg_Send_LE_L2CAP_Event( u_int8* le_l2cap_data,u_int8 le_l2cap_length, u_int8 tx0_rx1)//merge CEVA 0517
{
    int i;//merge CEVA 0517
    struct q_desc *qd_tci;//merge CEVA 0517

    /*
     * If currently active then send as special Event//merge CEVA 0517
     */
    if (1) //merge CEVA 0517//(LE_config.trace_level & 0x10) // Needs to be build based (SYSconfig_Is_Feature_Supported(PRH_BS_CFG_SYS_TRACE_LMP_VIA_HCI_FEATURE))
    {
        qd_tci = BTq_Enqueue(HCI_EVENT_Q, 0,8+le_l2cap_length);//merge CEVA 0517
        if(qd_tci)//merge CEVA 0517
        {
           qd_tci->data[0] = 0xFF; //merge CEVA 0517   /* TCI event code    */
           qd_tci->data[1] = 6+le_l2cap_length; //merge CEVA 0517     /* Max SMP msg Len   */
           qd_tci->data[2] = 0x28;//merge CEVA 0517    /* subcode           */
           qd_tci->data[3] = tx0_rx1;//merge CEVA 0517 /* receive direction */
           for(i = 0; i < le_l2cap_length; i++)//merge CEVA 0517
           {
               qd_tci->data[i+4] = le_l2cap_data[i];//merge CEVA 0517
           }

           BTq_Commit(HCI_EVENT_Q,0);//merge CEVA 0517
        }
    }
}
#endif//merge CEVA 0517

#if (PRH_BS_CFG_SYS_TRACE_ADV_VIA_HCI_SUPPORTED==1)
void TCeg_Send_LE_Adv_Event(u_int16 header, const u_int8* le_adv_data, u_int8 le_adv_length, u_int8 channel,u_int8 tx)
{
    int i;
    struct q_desc *qd_tci;
    u_int8 crc_error;


    if(LE_config.trace_level & 0x02)
    {
    	crc_error = LE_LL_Get_CRC_Err();

    	qd_tci = BTq_Enqueue(HCI_EVENT_Q, 0, 11+3+le_adv_length);
    	if(qd_tci)
    	{
    		qd_tci->data[0] = 0xFF;    /* TCI event code    */
    		qd_tci->data[1] = 9+le_adv_length+3;      /* Max Adv msg Len   */
    		qd_tci->data[2] = 0x23;    /* subcode           */
    		_Insert_Uint32(qd_tci->data+3,HW_get_native_clk());
    		qd_tci->data[7] = tx;
    		if ((tx==0) && (crc_error))
    			qd_tci->data[8] = 99;
    		else
    			qd_tci->data[8] = channel; /* receive direction */
    		_Insert_Uint16(qd_tci->data+9,header);
    		for(i = 0; i < le_adv_length; i++)
    		{
    			qd_tci->data[i+11] = le_adv_data[i];
    		}

    		/* Include the Backoff Count and Upper Limit */
    		if ((tx == 0x00) && (((header & 0x000F) == 0x00 /* Adv_Ind */) || ((header & 0x000F) == 0x06 /* Adv_Scan_Ind */)))
    		{
    			qd_tci->data[le_adv_length+11] = LEscan_Get_Scan_Backoff_Count();
    			_Insert_Uint16(qd_tci->data+le_adv_length+12,LEscan_Get_Scan_Upper_Limit());
    		}
    		else
    		{
    			qd_tci->data[le_adv_length+11] = 0;
    			qd_tci->data[le_adv_length+12] = 0;
    			qd_tci->data[le_adv_length+13] = 0;
    		}
    		BTq_Commit(HCI_EVENT_Q,0);
    	}
    }
}
#endif

#if (PRH_BS_CFG_SYS_TRACE_LLDATA_VIA_HCI_SUPPORTED==1)

void TCeg_Send_LE_LLData_Event(u_int16 header, const u_int8* le_ll_data, u_int8 le_lldata_length, u_int8 channel,u_int8 tx,u_int8 link_id)
{
    int i;
    struct q_desc *qd_tci;

    /*
     * If currently active then send as special Event
     */
    if(LE_config.trace_level & 0x04)
    {
    	qd_tci = BTq_Enqueue(HCI_EVENT_Q, 0, 12+le_lldata_length);
    	if(qd_tci)
    	{
    		qd_tci->data[0] = 0xFF;    /* TCI event code    */
    		qd_tci->data[1] = 10+le_lldata_length;      /* Max Adv msg Len   */
    		qd_tci->data[2] = 0x24;    /* subcode           */
    		_Insert_Uint32(qd_tci->data+3,HW_get_native_clk());
    		qd_tci->data[7] = tx;
    		qd_tci->data[8] = link_id;
    		qd_tci->data[9] = channel; /* receive direction */
    		_Insert_Uint16(qd_tci->data+10,header);
    		for(i = 0; i < le_lldata_length; i++)
    		{
    			qd_tci->data[i+12] = le_ll_data[i];
    		}

    		BTq_Commit(HCI_EVENT_Q,0);
    	}
    }
}

#endif


#if (PRH_BS_CFG_SYS_TRACE_LE_DEVICE_STATE_VIA_HCI_SUPPORTED == 1)

void TCeg_Send_LE_LEDeviceState_Event(u_int8 state, t_LE_Connection* p_connection )
{
    struct q_desc *qd_tci;

    /*
     * If currently active then send as special Event
     */

    if(LE_config.trace_level & 0x08)
    {
    	qd_tci = BTq_Enqueue(HCI_EVENT_Q, 0, 12); //+le_lldata_length);
    	if(qd_tci)
    	{
    		qd_tci->data[0] = 0xFF;    /* TCI event code    */
    		qd_tci->data[1] = 10; //+le_lldata_length;      /* Max Adv msg Len   */
    		qd_tci->data[2] = 0x25;    /* subcode           */
    		_Insert_Uint32(qd_tci->data+3,HW_get_native_clk());
    		qd_tci->data[7] = state;
    		qd_tci->data[9] = 0;
    		qd_tci->data[10] = 0;
    		qd_tci->data[11] = 0;
    		qd_tci->data[12] = 0;
    		switch(state)
    		{
#if ((PRH_BS_CFG_SYS_LE_BROADCAST_DEVICE == 1) || (PRH_BS_CFG_SYS_LE_PERIPHERAL_DEVICE == 1))//merge CEVA 0517
    		case ADVERTISING_STATE :
				{
					u_int8 channel_map = LEadv_Get_Adv_Channel_Map();

					if(channel_map & 0x01)
						qd_tci->data[8] = 37;
					if(channel_map & 0x02)
						qd_tci->data[9] = 38;
					if(channel_map & 0x04)
						qd_tci->data[10] = 39;

					qd_tci->data[11] = LEadv_Get_Adv_Type();
				}
				break;
#endif//merge CEVA 0517
#if (PRH_BS_CFG_SYS_LE_OBSERVER_DEVICE == 1) || (PRH_BS_CFG_SYS_LE_CENTRAL_DEVICE == 1)//merge CEVA 0517
    		case INITIATING_STATE :
    			qd_tci->data[8] = LEscan_Get_Initiating_Channel_Index();
    			break;
#endif//merge CEVA 0517

#if (PRH_BS_CFG_SYS_LE_OBSERVER_DEVICE == 1) || (PRH_BS_CFG_SYS_LE_CENTRAL_DEVICE == 1)//merge CEVA 0517
    		case SCANNING_STATE :
    			qd_tci->data[8] =  LEscan_Get_Scanning_Channel_Index();
    			break;
#endif//merge CEVA 0517
    		case CONNECTION_STATE :
    			if (p_connection)
    			{
    				qd_tci->data[8] = (p_connection->link_id+1);
    				qd_tci->data[9] = p_connection->current_data_channel;
    				_Insert_Uint16(&qd_tci->data[10],p_connection->connection_event_counter);
    			}
    			break;

    		case TEST_STATE :

    			break;


    		}

    		BTq_Commit(HCI_EVENT_Q,0);
    	}
    }
}

#endif
#else

void TCeg_Send_LE_LLC_Event( const u_int8* le_llc_data, u_int8 le_llc_length, u_int8 tx0_rx1)
{
	;
}
void TCeg_Send_LE_Adv_Event(u_int16 header, const u_int8* le_adv_data, u_int8 le_adv_length, u_int8 channel,u_int8 tx)
{
	;
}
void TCeg_Send_LE_LLData_Event(u_int16 header, const u_int8* le_ll_data, u_int8 le_lldata_length, u_int8 channel,u_int8 tx,u_int8 link_id)
{
	;
}

void TCeg_Send_LE_LEDeviceState_Event(u_int8 state, t_LE_Connection* p_connection )
{
	;
}

#endif

