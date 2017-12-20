/***********************************************************************
 *
 * MODULE NAME:    le_config.c
 * PROJECT CODE:   CEVA Low Energy Single Mode
 * DESCRIPTION:    Low Energy Configuration file.
 * MAINTAINER:     Gary Fleming
 * CREATION DATE:  September 2011
 *
 *
 * LICENSE:
 *     This source code is copyright (c) 2011-2012 Ceva Inc.
 *     All rights reserved.
 *
 ***********************************************************************/

#include "sys_config.h"
#include "hc_const.h"
#include "hci_params.h"

#include "le_advertise.h"
#include "le_scan.h"
#include "le_const.h"
#include "le_config.h"
#include "le_connection.h"
#include "LE_link_layer.h"
#include "le_white_list.h"
#include "le_frequency.h"
#include "le_scan.h"//merge CEVA 0517
#include "sys_rand_num_gen.h"
#include "hw_radio.h"

#include "bt_timer.h"
#include "lslc_irq.h"
#include "le_white_list.h"

#include "hw_lc.h"
#include "hw_le_lc.h"

#include "tc_event_gen.h"
#include "tc_const.h"
#if (PRH_BS_CFG_SYS_LE_GATT_INCLUDED==1)//merge CEVA 0517
#include "le_att.h"//merge CEVA 0517
#endif//merge CEVA 0517
#if (PRH_BS_CFG_SYS_LE_SMP_INCLUDED==1)//merge CEVA 0517
#include "le_smp.h"//merge CEVA 0517
#include "le_security_db.h"//merge CEVA 0517
#endif//merge CEVA 0517
#if (PRH_BS_CFG_SYS_LE_GAP_INCLUDED==1)//merge CEVA 0517
#include "le_gap.h"//merge CEVA 0517
#endif//merge CEVA 0517
#if (PRH_BS_CFG_SYS_LE_GATT_INCLUDED==1)//merge CEVA 0517
#include "le_gatt_server.h"//merge CEVA 0517
#endif//merge CEVA 0517
#if (PRH_BS_CFG_SYS_LE_L2CAP_INCLUDED==1)//merge CEVA 0517
#include "le_l2cap.h"//merge CEVA 0517
#endif//merge CEVA 0517

#if 0 // Only for Terasic Demo - with UART Sensor//merge CEVA 0517
#include "system.h"//merge CEVA 0517
#endif//merge CEVA 0517
#include "le_cts_app.h"//merge CEVA 0517
t_LE_Config LE_config;

#define LE_MAX_NUMBER_OF_EVENTS 20//merge CEVA 0517

/*
 * Initial release is a fully functional system with all states and state combinations supported.
 * Later to be combined with Build options which will define the supported states in any given
 * configuration.
 */
const u_int8 LEconfig_Classic_BT_Features[8] = {0x00,0x00,0x00,0x00,0x60,0x00,0x00,0x00}; /* LE supported, BR/EDR not supported */
const u_int8 _LEconfig_supported_states[8] = {0xFF,0xFF,0xFF,0x1F,0x00,0x00,0x00,0x00};
const u_int8 _LEconfig_local_features[8]   = {PRH_BS_CFG_SYS_ENCRYPTION_SUPPORTED,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//remove Get_Current_LE_config_state merge CEVA 0517
u_int8 Get_Current_LE_config_state(void)
{
    return LE_config.state;
}
/**************************************************************************
 * FUNCTION :- LEconfig_Init
 *
 * DESCRIPTION
 * Initialisation of the LE_config container and all its elements.
 *
 **************************************************************************/

t_error LEconfig_Init(void)
{
#if (PRH_BS_CFG_SYS_LE_CENTRAL_DEVICE==1) || (PRH_BS_CFG_SYS_LE_PERIPHERAL_DEVICE==1)
    u_int8 i;
#endif

    BDADDR_Set_to_Zero(&LE_config.random_address);

    LEadv_Init();
    LEscan_Init();

    LE_config.next_initiator_scan_timer = BT_CLOCK_MAX_TIMEOUT;
    LE_config.initiating_active = 0;//merge CEVA 0517
    LE_config.state = STANDBY_STATE;
    LE_config.sub_state = SUBSTATE_IDLE;
    LE_config.event_mask = 0x1F;
    LE_config.sleep_clock_accuracy = PRH_BS_CFG_MASTER_SLEEP_CLOCK_ACCURACY;

#if (PRH_BS_CFG_SYS_LE_CENTRAL_DEVICE==1) || (PRH_BS_CFG_SYS_LE_PERIPHERAL_DEVICE==1)
    LE_config.link_id_pool = 0xFFFFFFFF;

    LE_config.next_connection_event_time = BT_CLOCK_MAX_TIMEOUT;

    for (i=0;i<4;i++)
        LE_config.data_channel_map[i]=0xFF;

    LE_config.data_channel_map[4] = 0x1F;// data channel not include  channel 37,38,39 */

    LE_config.data_channel_map[0] = 0x0;//BK3431 low channel not good,2404~2418
    LE_config.data_channel_map[1] = 0x0;//BK3431 low channel not good,2420~2436 (not include 2426)
    LE_config.data_channel_map[2] = 0x0;//BK3431 low channel not good,2438~2452

    LE_config.num_le_links=0;
    // no need to init AES until used
    //LE_config.AES_complete = 0;

    LE_config.connection_complete_pending = 0;
    LE_config.p_connection_complete_pending = 0;
#if (PRH_BS_CFG_SYS_ENCRYPTION_SUPPORTED==1)
    LE_config.link_key_request_timer = BT_CLOCK_MAX_TIMEOUT;
#endif
    LEconnection_Init();
#endif
    LE_config.device_sleep=0;
    LE_config.tabasco_asleep = 0;
    LE_config.test_mode = 0;

    LEwl_Clear_White_List();
#if (PRH_BS_CFG_SYS_LE_SMP_INCLUDED==1)
    LEsmp_Init_SMP();
#if ((PRH_BS_CFG_SYS_LE_CENTRAL_DEVICE==1) || (PRH_BS_CFG_SYS_LE_SMP_PERIPHERAL_SUPPORTS_KEY_HIERARCHY!=1))//merge CEVA 0517
    LEsecdb_Clear_Security_DB();//merge CEVA 0517
#endif//merge CEVA 0517

#endif
    HWle_set_whitening_enable();//merge CEVA 0517 HWle_set_whitening_enable(0x01);-->HWle_set_whitening_enable();
    // Full investigation of below options required for Each Specific Radio//merge CEVA 0517
    //!!!!!! !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//merge CEVA 0517
    // This one of the most sensitive parameter settings in HW for LE//merge CEVA 0517
    ////merge CEVA 0517
    /* HWle_set_delay_search_win(31-TIFS_TX_ADJUSTMENT); */
    //HWle_set_delay_search_win(10); /**< ZGF@2014.0521 */
	HWle_set_delay_search_win(0);//charles modify 2017/11/27
    //HWle_set_tifs_delay(TIFS_TX_ADJUSTMENT);//org
    //HWle_set_tifs_delay(TIFS_TX_ADJUSTMENT-10);//charles modify for BK3431 (scanning side) and CSR (Scanning side)
    //that can success receive scan request and send scan response to scanning side
    //orignal value just only match peer device CSR but BK3431 can not receive scan request from BK3431
    // can used Flash write to memory tifs delay value
    // maybe this value is dynamic value for each scanning side


#ifdef VERSION_D_CHIP
	HWle_set_tifs_delay(TIFS_TX_ADJUSTMENT-10);
#else
    /* Ensure that no TIFS count is running
     */
    //HWle_set_tifs_delay(TIFS_TX_ADJUSTMENT-9);//sniffer real case 150 us 
		//HWle_set_tifs_delay(TIFS_TX_ADJUSTMENT-10);//sniffer real case 151 us
		//HWle_set_tifs_delay(TIFS_TX_ADJUSTMENT-11);//sniffer real case 152 us
		//HWle_set_tifs_delay(TIFS_TX_ADJUSTMENT-12);//sniffer real case 153 us revision 335 change
		//HWle_set_tifs_delay(TIFS_TX_ADJUSTMENT-14);//sniffer real case 155 us revision 335 change
		                                           //master no pack recv rate 0.1 %
        HWle_set_tifs_delay(TIFS_TX_ADJUSTMENT-10);//sniffer real case 151 us //charles modify 2017/11/27
#endif   

    HWle_abort_tifs_count();
    HWle_clear_tifs_default();//merge CEVA 0517 HWle_set_tifs_default(0);-->HWle_clear_tifs_default()

#if (PRH_BS_CFG_SYS_EXTENDED_HCI_COMMANDS_SUPPORTED==1)
    LE_config.auto_advertise = 0x00;
    LE_config.auto_initiate = 0x00;
    LE_config.slave_listen_outside_latency = 0;
    LE_config.TCI_transmit_win_config = 0x00;
#endif
    LE_config.trace_level = 0x31;//merge CEVA 0517
#if (PRH_BS_CFG_SYS_LE_GATT_INCLUDED==1)//merge CEVA 0517
    GATT_init();//merge CEVA 0517
    GATTserver_Init();//merge CEVA 0517
#endif//merge CEVA 0517

#if (PRH_BS_CFG_SYS_LE_GAP_INCLUDED==1)//merge CEVA 0517
    GAP_Init();//merge CEVA 0517
#endif//merge CEVA 0517

#if (PRH_BS_CFG_SYS_LE_L2CAP_INCLUDED==1)//merge CEVA 0517
    LE_L2CAP_Init();//merge CEVA 0517
#endif//merge CEVA 0517

#if ((PRH_BS_CFG_SYS_LE_PERIPHERAL_DEVICE == 1) && (PRH_BS_CFG_SYS_EMBEDDED_PROFILES==1))//merge CEVA 0517
    LE_APP_Init();//merge CEVA 0517
#endif//merge CEVA 0517
    return NO_ERROR;
}

/*************************************************************************
 * LEconfig_Register_Event_Handler
 *
 * Register the presented event handler for immediate call back on event
 *************************************************************************/
void LEconfig_Register_Event_Handler(u_int8 event_id,
                                     void *func_ptr)//merge CEVA 0517
{
    // Registration for HCI not complete yet - optional extra //(PRH_BS_CFG_SYS_LE_HCI_INCLUDED)
    //  le_call_back_list[event_id] = func_ptr;
}//merge CEVA 0517

/**************************************************************************
 * FUNCTION :- LEconfig_Set_Event_Mask
 *
 * DESCRIPTION
 * Sets the HCI event mask for LE events.
 *
 **************************************************************************/

void LEconfig_Set_Event_Mask(u_int8* p_pdu)
{
    /*
     * As only one of the eight bytes in the event mask is actually used.
     * We restrict our event mask to a single byte.
     */

    LE_config.event_mask = p_pdu[0];
}

/**************************************************************************
 * FUNCTION :- LEconfig_Get_LE_Features_Ref
 *
 * DESCRIPTION
 * Returns a pointer to the set of LE supported features.
 *
 **************************************************************************/


const u_int8* LEconfig_Get_LE_Features_Ref(void)
{
    return _LEconfig_local_features;
}

/**************************************************************************
 * FUNCTION :- LEconfig_Is_Features_Encryption_Supported
 *
 * DESCRIPTION
 * Used to determine if encryption is supported in the current device.
 * Should be replaced by a build parameter as this is a waste of code.
 *
 **************************************************************************/


u_int8 LEconfig_Is_Features_Encryption_Supported(void)
{
    return (_LEconfig_local_features[0] & PRH_BS_CFG_SYS_ENCRYPTION_SUPPORTED);
}

/**************************************************************************
 * FUNCTION :- LEconfig_Set_Random_Address
 *
 * DESCRIPTION
 * Sets the RANDOM Address to be used by a device.
 *
 **************************************************************************/

void LEconfig_Set_Random_Address(u_int8* p_random_address)
{
    BDADDR_Assign_from_Byte_Array(&LE_config.random_address, p_random_address);
}


/***************************************************************************
 * Function :- LEconfig_Read_Supported_States
 *
 * Description :-
 * Simple function to return the current combination of states supported in
 * an LE device.
 *
 * Context :- Schedular
 ***************************************************************************/

const u_int8* LEconfig_Read_Supported_States(void)
{
    return _LEconfig_supported_states;
}

/***************************************************************************
 * Function :- LEconfig_Check_Current_Address
 *
 * Description :-
 * This fuction checks if a given address and address type match the local
 * address type and address.
 *
 * Context :- IRQ
 ***************************************************************************/

u_int8 LEconfig_Check_Current_Address(u_int8 RxAdd,t_p_pdu p_pdu)
{
    //
    //  Checks if there is a match to the current device address..
    //
    u_int8 i;
#if 1 // GF 26 June 2012 -- fix for Direct Advertising On Random Address
    if (((LE_config.state == SCANNING_STATE) && ( RxAdd == LEscan_Get_Scanning_Own_Address_Type())) ||
        ((LE_config.state == ADVERTISING_STATE) && (RxAdd == LEadv_Get_Advertising_Own_Address_Type())))
#else
        if (RxAdd == LEadv_Get_Advertising_Own_Address_Type())
#endif
        {
            const t_bd_addr* p_bd_addr;

            if (RxAdd == 0x01 /* RANDOM_ADDRESS */)
            {
                p_bd_addr = &LE_config.random_address;
            }
            else
            {
                p_bd_addr = SYSconfig_Get_Local_BD_Addr_Ref();
            }

            for (i=0;i<6;i++)
            {
                if(p_pdu[i] != p_bd_addr->bytes[i])
                    return 0;
            }

            return 1;
        }
        else
        {
            return 0;
        }
}

#if (PRH_BS_CFG_SYS_LE_CENTRAL_DEVICE==1) || (PRH_BS_CFG_SYS_LE_PERIPHERAL_DEVICE==1)

/***************************************************************************
 * Function :- LEconfig_Allocate_Link_Id
 *
 * Description :-
 * This function allocates an Identifier for an LE link.
 *
 ***************************************************************************/

u_int8 LEconfig_Allocate_Link_Id(u_int8* p_link_id)
{
    u_int8 i=0;
    u_int32 pool_entry;
    // This allocates a free link id.
    // use a u_int32 to store 32 link id as a bitfield.

    while( i < DEFAULT_MAX_NUM_LE_LINKS)
    {
        pool_entry = (1 << i);
        if ( LE_config.link_id_pool & pool_entry)
        {
            LE_config.link_id_pool &= ~pool_entry;
            *p_link_id = i;
            return NO_ERROR;
        }
        else
        {
            i++;
        }
    }
    return UNSPECIFIED_ERROR;
}

/***************************************************************************
 * Function :- LEconfig_Free_Link_Id
 *
 * Description :-
 * This function allocates an Identifier for an LE link.
 *
 ***************************************************************************/

void LEconfig_Free_Link_Id(t_LE_Connection* p_connection)
{
#if (PRH_BS_CFG_SYS_LE_DUAL_MODE==1)
    DL_Free_Link(p_connection->device_index);
#endif
    LE_config.link_id_pool |=  (1 << p_connection->link_id);
}


/***************************************************************************
 * Function :- LEconnections_Link_In_Use
 *
 * Description :-
 * This funtion checks if a link id is in use.
 *
 ***************************************************************************/

u_int8 LEconnections_Link_In_Use(u_int8 link_id)
{
    return !(LE_config.link_id_pool & (1 << link_id));
}

#if (PRH_BS_CFG_SYS_EXTENDED_HCI_COMMANDS_SUPPORTED==1)

/***************************************************************************
 * Function :- LEconfig_TCI_Set_TIFS_Tx_Adjustment
 *
 * Description :-
 * This functions is used to support the equivalent TCI command. This writes
 * the TIFS adjustment directly to the HW.
 *
 ***************************************************************************/

t_error LEconfig_TCI_Set_TIFS_Tx_Adjustment(u_int8 tifs_adjustment)
{
    HWle_set_tifs_delay(tifs_adjustment);

    TCeg_Command_Complete_Event(TCI_LE_SET_TIFS_TX_ADJUSTMENT, NO_ERROR);
    return NO_ERROR;
}

/***************************************************************************
 * Function :- LEconfig_TCI_Set_Search_Window_Delay
 *
 * Description :-
 * This functions is used to support the equivalent TCI command. This writes
 * the Search Delay directly to the HW.
 *
 ***************************************************************************/

t_error LEconfig_TCI_Set_Search_Window_Delay(u_int8 search_win_delay)
{
    HWle_set_delay_search_win(31-search_win_delay);

    TCeg_Command_Complete_Event(TCI_LE_SET_SEARCH_WINDOW_DELAY, NO_ERROR);
    return NO_ERROR;
}

#endif


#endif
