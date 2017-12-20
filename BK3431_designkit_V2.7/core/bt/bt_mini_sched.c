/***********************************************************************
 *
 * MODULE NAME:    bt_mini_sched.c
 * PROJECT CODE:   BlueStream
 * DESCRIPTION:    HC scheduling methods for non-interrupt activities.
 * MAINTAINER:     Tom Kerwick
 * CREATION DATE:  November 1999
 *
 * SOURCE CONTROL: $Id: bt_mini_sched.c,v 1.1 2012/05/09 00:09:28 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 1999-2004 Ceva Inc.
 *     All rights reserved.
 *
 ***********************************************************************/

#include "sys_config.h"
#include "sys_rand_num_gen.h"

#include "lc_interface.h"

#include "bt_mini_sched.h"
#include "bt_init.h"
#include "bt_timer.h"

#include "hc_const.h"
#include "hc_event_gen.h"
#include "hc_cmd_disp.h"
#include "hc_flow_control.h"

#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==0)
#include "tra_hcit.h"
#endif
#include "tra_uart.h"

#include "lslc_irq.h"

#include "sys_power.h"

#include "le_scan.h"
#include "le_config.h"
#include "le_link_layer.h"
#include "le_connection.h"
#include "le_security.h"
#include "le_cts_app.h"//merge CEVA 0517
#include "lmp_cmd_disp.h"
#include "debug.h"
#include "sys_irq.h"

#if (PRH_BS_CFG_SYS_LE_SMP_INCLUDED==1)//merge CEVA 0517
#include "le_smp.h"
#endif
#if (PRH_BS_CFG_SYS_LE_GATT_INCLUDED==1)//merge CEVA 0517
#include "le_att.h"
#endif
#if(PRH_BS_CFG_SYS_LE_GAP_INCLUDED==1)//merge CEVA 0517
#include "le_gap.h"
#endif
#include "sys_mmi.h"
extern t_LE_Config LE_config;
extern  u_int8 RF_test_mode_enable;
static t_error _BTms_Handle_HC_Reset_If_Reqd(void);
static t_error _BTms_Handle_HC_Sleep_If_Reqd(void);
extern void LETestPattern_Encode_PDU(u_int8 data_length);
extern  u_int8 Get_Current_LE_config_state(void);
void go_to_Idle(void);//turn off  ARM core clock source
int SYSpwr_Is_Available_Halt_System(void);

extern t_queue BTQueues[];

u_int8 _BTms_pending_hc_reset = 0;

#ifdef AUX_CALIBRATE
Aux_calibrate_t Candidate_array[4];
Aux_cali_t Calibrate_drift;
/**************************************************************
  Clean all Candidate_array
***************************************************************/
void Choice_Calibrate_value(void)
{
	 u_int8 i;
	
	 if(Candidate_array[0].used)
		{
		  Calibrate_drift.value = Candidate_array[0].Candidate;
		}
	for(i=1;i<(sizeof(Candidate_array)/sizeof(Aux_calibrate_t));i++)
	{
			if((Candidate_array[i].used == 1) && (Calibrate_drift.value  > Candidate_array[i].Candidate))
			{
				 Calibrate_drift.value = Candidate_array[i].Candidate;
			}
	}
	Calibrate_drift.Start_calibrate = 1;
}
/**************************************************************
  Clean all Candidate_array
***************************************************************/
void Clean_Candidate(void)
{
	 u_int8 i;
	 extern Aux_calibrate_t Candidate_array[4];
	 Calibrate_drift.Start_calibrate = 0;
	 Calibrate_drift.value = 0;
	 for(i = 0;i< (sizeof(Candidate_array)/sizeof(Aux_calibrate_t));i++)
   {
		 Candidate_array[i].Candidate = 0;
     Candidate_array[i].used = 0;
     Candidate_array[i].voting_count = 0;	 
	 }
}
/**************************************************************
  Add Candidate to array
return 0 :success
return 1 : fail
***************************************************************/
u_int8 Add_Candidate(u_int8 value)
{
	 u_int8 i,result;
	 result = 1;
	 for(i = 0;i< (sizeof(Candidate_array)/sizeof(Aux_calibrate_t));i++)
   {
		 if((Candidate_array[i].Candidate == value) && (Candidate_array[i].used == 0) )
		 {
			 Candidate_array[i].used = 1;
			 result = 0 ;
			 return result ;
		 }
		 if((Candidate_array[i].Candidate != value) && (Candidate_array[i].used == 0) )
		 {
			  Candidate_array[i].Candidate = value;
			  Candidate_array[i].used = 1;
			   result = 0 ;
			 return result ;
		 }
		 if((Candidate_array[i].Candidate == value) && (Candidate_array[i].used == 1) )
		 {
			  result = 0 ;
			 return result ;
		 }
	 }
	 return result; 
}
#endif
void go_to_Idle(void)//turn off ARM core clock source
{
#if(IDLE_MODE == 1)
    if(RF_test_mode_enable == 0)
    {
        if(Get_need_to_Idle())
        {
            //DEBUG_MSG(0xf6);
            REG_AHB0_ICU_CORECLKCON  = 0x1;         // ARM9 Core clock close
        }
    }
#endif
}
//merge CEVA 0517 remove "BTms_OS_Event_Dispatcher_Initialise","BTms_OS_Event_Dispatcher","BTms_Sched"
t_error BTms_Sched(u_int32 num_interations)
{
    t_error status = NO_ERROR;
    u_int32 inter;
    extern t_need_todo need_to_do;

    for(inter = 0; inter < num_interations; num_interations--)
    {
        /*
         * Dispatch any LMP commands
         */
        //                 if((LE_config.state == ADVERTISING_STATE) ||
        //                     (LE_config.state == INITIATING_STATE) ||
        //                   (LE_config.state == CONNECTION_STATE))
        //                 {
        //                      break;
        //                 }
        status = LMdisp_LMP_Command_Dispatcher();

#if (PRH_BS_CFG_SYS_ENCRYPTION_SUPPORTED==1) && ((PRH_BS_CFG_SYS_LE_CENTRAL_DEVICE==1) || (PRH_BS_CFG_SYS_LE_PERIPHERAL_DEVICE))//merge CEVA 0517
#if ((PRH_BS_CFG_SYS_LE_GATT_INCLUDED==1) || (PRH_BS_CFG_SYS_LE_SMP_INCLUDED==1) || (PRH_BS_CFG_SYS_LE_L2CAP_INCLUDED==1))//merge CEVA 0517
        // Encryption is handled in the host layers//merge CEVA 0517
#else //merge CEVA 0517
        LEconnection_Handle_Data_To_Encrypt();//merge CEVA 0517//no used
#endif
#endif//merge CEVA 0517
        /*
         * Report Number of Completed Packets Event to Host, if necessary
         */
#if (PRH_BS_CFG_SYS_LE_CENTRAL_DEVICE==1) || (PRH_BS_CFG_SYS_LE_PERIPHERAL_DEVICE==1)
#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==0)//merge CEVA 0517
        HCfc_Handle_Host_To_Host_Controller_Flow_Control();
#endif
#endif//merge CEVA 0517
#ifdef _VIRTUAL_HOST_
        LETestPattern_Encode_PDU(23);//charles add for transfer data test
#endif
        //#if (PRH_BS_CFG_REMOVE_UART_HCI==0)//merge CEVA 0517

        /*
         * Dispatch any pending HCI events (e.g. there could be a num_completed_packets
         * event, recently created, due to go to the host.
         */
        status = TRAhcit_Dispatch_Pending_Event();

        //#endif//merge CEVA 0517
        {
#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==0)////merge CEVA 0517
            //remove if(Get_Current_LE_config_state() == STANDBY_STATE)//merge CEVA 0517
            {
                /*
                 * Dispatch pending SCO/L2CAP pending data subsequent to LMP & HCI events to avoid
                 * race condition between HCI events and subsequent data.
                 */
                status = TRAhcit_Dispatch_Pending_Data();
            }
#endif//merge CEVA 0517
#if (PRH_BS_CFG_SYS_LE_OBSERVER_DEVICE==1) || (PRH_BS_CFG_SYS_LE_CENTRAL_DEVICE==1)//merge CEVA 0517
            LEscan_Check_For_Pending_Advertising_Reports();//no used
#endif

#if (PRH_BS_CFG_SYS_LE_CENTRAL_DEVICE==1) || (PRH_BS_CFG_SYS_LE_PERIPHERAL_DEVICE==1)//merge CEVA 0517
            LE_LL_Check_For_Pending_Connection_Completes();
#endif
        }

#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==0)//merge CEVA 0517
        /*
         * Finally, handle any incoming commands that may have arrived. This is placed here
         * to give priority to upward (in the host direction) data, and events (which could
         * possibly be num_completed_packets) to ensure high data transfer efficiency.
         */

        status = HC_Command_Dispatcher();

#endif////merge CEVA 0517
#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==0)////merge CEVA 0517
#if TRA_HCIT_UART_POLLED_TX_SUPPORTED == 1
        /*
         * If polled Tx UART then process if in the middle of a transmit
         */
        if (TRAhcit_Is_Tx_Busy())
        {
            TRAhcit_UART_Tx_PDU();
        }

#endif /* TRA_HCIT_UART_POLLED_TX_SUPPORTED */
#endif////merge CEVA 0517

        //merge CEVA 0517 remove #if (PRH_BS_CFG_SYS_ENCRYPTION_SUPPORTED==1)
        LE_LL_Check_Timers();

#if (PRH_BS_CFG_SYS_LE_SMP_INCLUDED==1)//merge CEVA 0517
        LEsmp_Check_Pairing_Timers();//merge CEVA 0517

#endif//merge CEVA 0517
#if (PRH_BS_CFG_SYS_LE_GATT_INCLUDED==1)//merge CEVA 0517
        GATT_Check_GATT_Timers();//merge CEVA 0517

#endif//merge CEVA 0517
#if (PRH_BS_CFG_SYS_LE_GAP_INCLUDED==1)//merge CEVA 0517
        GAP_Check_GAP_Timers();//merge CEVA 0517

#endif//merge CEVA 0517

#if (PRH_BS_CFG_SYS_EMBEDDED_PROFILES==1)//merge CEVA 0517
        User_App_Check_Timers();//merge CEVA 0517

#endif//merge CEVA 0517
    } /* end for */
    if(RF_test_mode_enable == 0)
    {
#if (MMI_ON == 1)
        MMI_main();//MMI entry point
#endif
    }
    else
    {
        Set_MMI_State(MMI_IDLE);
    }
#if (MMI_ON == 0)
    Set_MMI_State(MMI_IDLE);
#endif
#if (ADC_ON == 1)//for battery detect function
    Trans_ADC_Battery_Cap();
#endif
    if(RF_test_mode_enable == 1)
    {
        goto Reset_check;
    }
//#if (IDLE_MODE == 1)    //frank 1606017
    //while((LE_config.state == PRE_SLEEP_STATE) &&(Get_MMI_State()== MMI_IDLE))
   // while(LE_config.state == PRE_SLEEP_STATE)  //this sentence enable
//#endif
    {
        /*
         * Handle sleep opportunities or HCI reset if one pending
         */
    Reset_check :
        if(_BTms_pending_hc_reset == 0)
        {
            _BTms_Handle_HC_Sleep_If_Reqd();
        }
        else
        {
            _BTms_Handle_HC_Reset_If_Reqd();
        }
	 //DEBUG_MSG(0xf5);
        go_to_Idle();
    }
    #ifdef AUX_CALIBRATE
				if(connect_event_count_Candidate == 0)
				{
					Clean_Candidate();
				}
				if(need_to_do.add_diff )
				{ 
					 need_to_do.add_diff = 0;
					 if(Add_Candidate(diff_candidate) == 1)
					 {
						 //DEBUG_MSG(0x96);
					 }
				}
				if(need_to_do.display_Candidate_array)
				{ 
					u_int8 i;
					//DEBUG_MSG(0xA7);
					
					need_to_do.display_Candidate_array = 0;
					  for(i = 0;i< (sizeof(Candidate_array)/sizeof(Aux_calibrate_t));i++)
            {
							if(Candidate_array[i].used)
							{
						    //DEBUG_MSG(Candidate_array[i].Candidate);
							}
						}
						
					
					//DEBUG_MSG(0xA8);
					Choice_Calibrate_value();	
					//DEBUG_MSG(Calibrate_drift.value);	
				}
#endif
    /*
     * Dump one random number
     */
    (void)SYSrand_Get_Rand();

#if (IDLE_MODE == 1)
    //while((LE_config.state== STANDBY_STATE)&&(Get_MMI_State()== MMI_IDLE))
    if(LE_config.state== STANDBY_STATE)
#endif
    {
        //DEBUG_MSG(0xf4);
        go_to_Idle();
    }

    if((LE_config.state == CONNECTION_STATE) || (LE_config.state == ADVERTISING_STATE) )
    {
        //DEBUG_MSG(0xf3);
        go_to_Idle();
    }

    return status;
}

/*****************************************************************************
 * BTms_Request_HC_Reset
 *
 * Request that scheduler completes a full reset e.g. HCI_Reset command
 ****************************************************************************/
void BTms_Request_HC_Reset(void)
{
    _BTms_pending_hc_reset = 1;
#if defined(SYS_HAL_OS_EVENT_DRIVEN_HOST_CONTROLLER)
    BTms_OS_Post(BTMS_OS_EVENT_RESET_PENDING);
#endif
}

/*****************************************************************************
 * _BTms_Handle_HC_Reset_If_Reqd
 *
 * Perform a full reset e.g. HCI_Reset command
 *
 * Reset will be completed if not sending data/event to Host
 * If busy in HCI transmit then defer the reset.
 ****************************************************************************/
static t_error _BTms_Handle_HC_Reset_If_Reqd(void)
{
#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==0)////merge CEVA 0517
    t_cmd_complete_event cmd_complete_info;
#endif//merge CEVA 0517
    //u_int32 current_flags;
    /*
     * Reset if requested  and not currently sending Data/Event to Host
     * If busy sending data then will check on next pass of the mini-scheduler
     */
#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==0)//merge CEVA 0517
    if(!TRAhcit_Is_Tx_Busy())
#endif//merge CEVA 0517
    {
        LSLCirq_Disable_And_Clear_Tabasco_Intr();

        /*
         * Full initialisation of device
         */
        BT_Initialise();

#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==0)//merge CEVA 0517
        /*
         * Now that HCI_Reset has been performed
         * Instruct the Host that the Host Controller is now ready
         */
        cmd_complete_info.opcode = HCI_RESET;
        cmd_complete_info.status = NO_ERROR;
        HCeg_Command_Complete_Event(&cmd_complete_info);
#endif////merge CEVA 0517
        /*
         * Cancel request
         */
        _BTms_pending_hc_reset = 0;

        return NO_ERROR;
    }
#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==0)////merge CEVA 0517
    else
    {
        return UNSPECIFIED_ERROR;
    }
#endif////merge CEVA 0517
}

/*****************************************************************************
 * _BTms_Handle_HC_Sleep_If_Reqd
 *
 * Handles opportunities to sleep processor to various low power modes.
 ****************************************************************************/
static t_error _BTms_Handle_HC_Sleep_If_Reqd(void)
{
#if (PRH_BS_CFG_SYS_LOW_POWER_MODE_SUPPORTED==1)

    //T03 no use this function
    if (LE_config.device_sleep == 0x01)
    {
        /* if baseband is asleep, stop system
         * otherwise do nothing, to prevent race between
         * CPU powerdown and baseband powerdown
         *
         * need to ensure no events pending transmission to the host
         *
         * when exiting power down mode the USE_LF is set to 0
         * however, due to the latency in Tabasco, the clearing of the SLEEP
         * bit is delayed. This is now handled transparently in
         * LC_Is_Tabasco_Asleep()
         */
#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==0)////merge CEVA 0517
        //            if ((!TRAhcit_Is_Tx_Busy()) && (LE_config.tabasco_asleep == 0x01))
#endif////merge CEVA 0517
        if(BTms_Is_Available_Sleep_System())
        {
            SYSpwr_Halt_System();//deepsleep wakeup use I/O
        }
    }
 else
 {
     //         if(BTms_Is_Available_Sleep_System())
     //         {
     //             /*
     //              * We can sleep the CPU at this point so that with Interrupt from baseband
     //              * it is back to servicing interrupts and continue in this scheduler.
     //              */

     //             SYSpwr_Sleep_System();
     //         }
 }
#endif
 return NO_ERROR;
}

/*
 * FUNCTION:   BTms_Is_Available_Sleep_System()
 * PURPOSE:    Returns TRUE if system is available to sleep.
 */
int BTms_Is_Available_Sleep_System(void)
{
    /* No need to monitor LMP_OUT, L2CAP_OUT and SCO_OUT queue
     * because once placed data there, dequeued in LC ISR
     * so that halt scheduler at this point is no problem
     * even if containing any data
     */

    t_queue *q = BTQueues;

    /*
     * Check for HCI_COMMAND_Q, HCI_EVENT_Q and LMP_IN_Q
     */
    do
    {
        if(q->num_entries)
        {
            return(0/*Not Empty*/);
        }
        q++;
    }
    while(q != &BTQueues[LMP_OUT_Q]);

    /*
     * Check for L2CAP_IN_Q
     */
    q = BTQueues + L2CAP_IN_Q;
    do
    {
        if(q->num_entries)
        {
            return(0/*Not Empty*/);
        }
        q++;
    }
    while(q != &BTQueues[L2CAP_OUT_Q]);

    /*
     * Allow sleep if timers not expired
     */
    //return(!1);
    return(1);
}

/*
 * FUNCTION:   BTms_Is_Available_Halt_System()
 * PURPOSE:    Returns TRUE if system is available to sleep.
 */
int BTms_Is_Available_Halt_System(void)
{
#if (BUILD_TYPE!=UNIT_TEST_BUILD)
    return SYSpwr_Is_Available_Halt_System();
#else
    return 0;
#endif
}
