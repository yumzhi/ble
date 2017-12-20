/**************************************************************************
 *
 * MODULE NAME:    tc_cmd_disp.c
 * PROJECT CODE:    BlueStream
 * DESCRIPTION:    Test Control command dispatcher
 * MAINTAINER:     Daire McNamara <Daire.McNamara@sslinc.com>
 * CREATION DATE:  13 April 2000
 *
 * SOURCE CONTROL: $Id: tc_cmd_disp.c,v 1.2 2012/05/14 23:14:18 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2004 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *
 * SOURCE: Based on V2.0 HC Design
 * ISSUES:
 *
 *************************************************************************/

#include "sys_config.h"
#include "hc_const.h"
#include "hc_event_gen.h"
#include "hci_params.h"
#include "hw_radio.h"
#include "tc_const.h"
#include "tc_interface.h"
#include "tc_event_gen.h"

#include "lslc_stat.h"
#include "le_config.h"
#include "le_advertise.h"
#include "le_scan.h"
#include "le_connection.h"


#include "hw_lc.h"
#include "hw_le_lc.h"
extern t_LE_Config LE_config;


#if (PRH_BS_CFG_SYS_EXTENDED_HCI_COMMANDS_SUPPORTED==1)
/*****************************************************************
 *  FUNCTION :- TCI_Dispatch_Prop_Command
 *
 *  DESCRIPTION 
 *  Handles any Parthus TCI proprietary commands
 *****************************************************************/ 
t_error TCI_Dispatch_Prop_Command(t_p_pdu p_payload, u_int16 opcode)
{
    u_int16 ocf;
    ocf = ( opcode & 0x03FF);
    
    switch(opcode) 
    {
    case TCI_BEKEN_HARDWARE_TEST:
        TC_Beken_Hardware_Test(p_payload);
        break;
    case TCI_READ_LOCAL_BASEBAND_MONITORS:
        TC_Read_Local_Baseband_Monitors();
        break;
    case TCI_RESET_LOCAL_BASEBAND_MONITORS:
        LSLCstat_Reset_Monitors();
        TCeg_Command_Complete_Event(TCI_RESET_LOCAL_BASEBAND_MONITORS, NO_ERROR);
        break;

	case TCI_LE_SET_TRANSMIT_WINDOW_PARAMS :
		LEconnection_TCI_Set_Transmit_Window_Params(HCIparam_Get_Uint16(p_payload+3), *(p_payload+5));
		break;

	case TCI_LE_SET_DIRECT_ADVERTISING_TIMEOUT ://merge CEVA 0517
		LEadv_TCI_Set_Direct_Adv_Timeout(HCIparam_Get_Uint16(p_payload+3));//merge CEVA 0517
		break;//merge CEVA 0517

	case TCI_LE_SET_TIFS_TX_ADJUSTMENT :
		LEconfig_TCI_Set_TIFS_Tx_Adjustment(*(p_payload+3));
		break;

	case TCI_LE_SET_SEARCH_WINDOW_DELAY :
		LEconfig_TCI_Set_Search_Window_Delay(*(p_payload+3));
		break;

	case TCI_LE_AUTO_ADVERTISE_AFTER_SLAVE_DISCONNECT :
		LE_config.auto_advertise = *(p_payload+3);
		TCeg_Command_Complete_Event(TCI_LE_AUTO_ADVERTISE_AFTER_SLAVE_DISCONNECT, NO_ERROR);
		break;

	case TCI_LE_AUTO_INITIATE_AFTER_MASTER_DISCONNECT :
		LE_config.auto_initiate = *(p_payload+3);
		TCeg_Command_Complete_Event(TCI_LE_AUTO_INITIATE_AFTER_MASTER_DISCONNECT, NO_ERROR);
		break;

#if (PRH_BS_CFG_SYS_LE_OBSERVER_DEVICE==1) || (PRH_BS_CFG_SYS_LE_CENTRAL_DEVICE==1)//merge CEVA 0517
	case TCI_LE_WRITE_SCAN_FREQUENCIES :
		LEscan_TCI_Write_Scan_Freqs(*(p_payload+3));
		break;

	case TCI_LE_READ_SCAN_FREQUENCIES :
		LEscan_TCI_Read_Scan_Freqs();
		break;

	case TCI_LE_WRITE_INITIATING_FREQUENCIES :
		LEscan_TCI_Write_Initiating_Freqs(*(p_payload+3));
		break;

	case TCI_LE_READ_INITIATING_FREQUENCIES :
		LEscan_TCI_Read_Initiating_Freqs();
		break;
#endif//merge CEVA 0517

	case TCI_LE_SLAVE_LISTEN_OUTSIDE_LATENCY :
		LE_config.slave_listen_outside_latency = (*(p_payload+3));
		TCeg_Command_Complete_Event(TCI_LE_SLAVE_LISTEN_OUTSIDE_LATENCY, NO_ERROR);
		break;

#if ((PRH_BS_CFG_SYS_LE_BROADCAST_DEVICE==1) || (PRH_BS_CFG_SYS_LE_PERIPHERAL_DEVICE==1))//merge CEVA 0517
	case TCI_LE_DIRECT_ADV_TIMEOUT :
		LEadv_TCI_Set_Direct_Adv_Timeout(HCIparam_Get_Uint16(p_payload+3));
		break;

	case TCI_LE_WRITE_ADVERTISING_DELTA :
		LEadv_TCI_Write_Advertising_Delta(*(p_payload+3));
		break;
#endif//merge CEVA 0517
	case TCI_LE_PRECONFIGURE_HOP_INC:
		LE_config.preConfigured_hop_inc_available = 1;
		LE_config.preConfigured_hop_inc = (*(p_payload+3));
		TCeg_Command_Complete_Event(TCI_LE_PRECONFIGURE_HOP_INC, NO_ERROR);
		break;

	case TCI_LE_PRECONFIGURE_ACCESS_CODE :
		LE_config.preConfigured_access_address_available = 1;
		LE_config.preConfigured_access_address = HCIparam_Get_Uint32(p_payload+3);
		TCeg_Command_Complete_Event(TCI_LE_PRECONFIGURE_ACCESS_CODE, NO_ERROR);
		break;

#if (PRH_BS_CFG_SYS_LE_OBSERVER_DEVICE==1) || (PRH_BS_CFG_SYS_LE_CENTRAL_DEVICE==1)//merge CEVA 0517
	case TCI_LE_ENABLE_SCAN_BACKOFF :
		LEscan_TCI_Enable_Scan_Backoff(*(p_payload+3));
		TCeg_Command_Complete_Event(TCI_LE_ENABLE_SCAN_BACKOFF, NO_ERROR);
		break;


	case TCI_LE_READ_SCAN_BACKOFF_INFO :
		LEscan_TCI_Read_Scan_Backoff_Info();
		break;


	case TCI_LE_READ_SCAN_PARAMS :
		LEscan_TCI_Read_Scan_Params();
		break;

	case TCI_LE_READ_LOCAL_POWER_LEVEL ://merge CEVA 0517

		break;//merge CEVA 0517


	case TCI_LE_INC_LOCAL_POWER_LEVEL ://merge CEVA 0517
		{
			t_q_descr *qd;//merge CEVA 0517

			qd = BTq_Enqueue(HCI_EVENT_Q, 0, 6);//merge CEVA 0517
			if(qd)//merge CEVA 0517
			{
				qd->data[0] = HCI_COMMAND_COMPLETE_EVENT;//merge CEVA 0517
				qd->data[1] = 4;//merge CEVA 0517
				qd->data[2] = 1;//merge CEVA 0517
				_Insert_Uint16(&(qd->data[3]), TCI_LE_INC_LOCAL_POWER_LEVEL);//merge CEVA 0517
				qd->data[5] = HWradio_Inc_Tx_Power_Level();//merge CEVA 0517 

				BTq_Commit(HCI_EVENT_Q, 0);//merge CEVA 0517
			}
		}
		break;//merge CEVA 0517

	case TCI_LE_DEC_LOCAL_POWER_LEVEL ://merge CEVA 0517
		{
			t_q_descr *qd;//merge CEVA 0517

			qd = BTq_Enqueue(HCI_EVENT_Q, 0, 6);//merge CEVA 0517
			if(qd)//merge CEVA 0517
			{
				qd->data[0] = HCI_COMMAND_COMPLETE_EVENT;//merge CEVA 0517
				qd->data[1] = 4;//merge CEVA 0517
				qd->data[2] = 1;//merge CEVA 0517
				_Insert_Uint16(&(qd->data[3]), TCI_LE_DEC_LOCAL_POWER_LEVEL);//merge CEVA 0517
				qd->data[5] = HWradio_Dec_Tx_Power_Level();//merge CEVA 0517 //charles remove

				BTq_Commit(HCI_EVENT_Q, 0);//merge CEVA 0517
			}
		}
		break;//merge CEVA 0517
#endif//merge CEVA 0517

#if ((PRH_BS_CFG_SYS_LE_BROADCAST_DEVICE==1) || (PRH_BS_CFG_SYS_LE_PERIPHERAL_DEVICE==1))
	case TCI_LE_READ_ADV_PARAMS :
		LEadv_TCI_Read_Advertising_Params();
		break;
#endif//merge CEVA 0517

	case TCI_LE_READ_ACCESS_CODE :
		LEconnetion_TCI_Read_Access_Code(HCIparam_Get_Uint16(p_payload+3));
		break;

	case TCI_LE_READ_HOP_INCREMENT :
		LEconnetion_TCI_Read_Hop_Increment(HCIparam_Get_Uint16(p_payload+3));
		break;

	case TCI_LE_READ_SESSION_KEY:
		LEconnection_TCI_Read_Session_Key(HCIparam_Get_Uint16(p_payload+3));
		break;

	case TCI_LE_READ_PEER_SCA :
		LEconnection_TCI_Read_Peer_SCA(HCIparam_Get_Uint16(p_payload+3));
		break;

	case TCI_WRITE_LOCAL_WHITENING_ENABLE :
		if (*(p_payload+3))//merge CEVA 0517
			HWle_set_whitening_enable();//merge CEVA 0517
		else//merge CEVA 0517
			HWle_clear_whitening_enable();//merge CEVA 0517

		TCeg_Command_Complete_Event(TCI_WRITE_LOCAL_WHITENING_ENABLE, NO_ERROR);
		break;

	case TCI_LE_SET_TRACE_LEVEL :
		LE_config.trace_level = *(p_payload+3);
		TCeg_Command_Complete_Event(TCI_LE_SET_TRACE_LEVEL, NO_ERROR);
		break;

	case TCI_LE_ECHO_TRANSMIT_WINDOW_SIZE_AND_OFFSET ://merge CEVA 0517
		LEconnection_TCI_Read_Window_Size_and_Offset(HCIparam_Get_Uint16(p_payload+3));//merge CEVA 0517
		break;//merge CEVA 0517

	/* Below commands have yet to be completed - to be added prior to UPF 42 - June 2012 */
	case TCI_LE_WRITE_NUM_PACKETS_PER_CE :
	case TCI_LE_GET_DEVICE_STATES :
	case TCI_LE_NUM_TIMES_MASTER_DOESNT_TX_FIRST_WIN :

     default :
         return ILLEGAL_COMMAND;
     }
    (void)ocf;
    return NO_ERROR;
}

#endif
