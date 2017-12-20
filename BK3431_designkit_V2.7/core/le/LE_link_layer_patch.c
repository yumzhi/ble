/**************************************************************************
 * MODULE NAME:    le_link_layer_patch.c
 * DESCRIPTION:    LE Link Layer Engine
 * AUTHOR:         Charles
 * DATE:
 *
 *
 * LICENSE:
 *     This source code is copyright (c) 2011-2012 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *
 * NOTES:
 *
 **************************************************************************/
#include "sys_features.h"
#include "sys_mmi_impl.h"
#include "le_advertise.h"
#include "hw_radio_defs.h"
#include "le_gap_const.h"
#include "BKAPI.h"
#include "lslc_irq.h"
#include "hw_jal_defs.h"
#include "le_config.h"
#include "le_const.h"
#include "sys_power.h"
#include "hw_lc_impl.h"
#include "le_link_layer.h"
#include "debug.h"
#include "le_gap.h"


//extern u_int8  low_power_adv_event_count;
extern  t_LE_Config LE_config;
extern u_int8  low_power_connect_event_count;
//extern u_int8  low_power_adv_event_count;
extern u_int8  Enter_low_power_mode_count;
extern u_int16 current_connectInterval;
//extern u_int32 current_clk_patch;
extern u_int8  early_wakeup;
extern u_int8  current_connect_latency;
extern volatile u_int8  button_detect_ctrl;
extern volatile u_int8 no_packet_recv_flag;
extern volatile u_int8  CRC_error_count;
extern volatile u_int16 current_window_size;
static u_int8  need_to_sleep;
static u_int8  need_to_Idle;
static u_int8  need_to_write_data;
//static volatile u_int32 sleep_start_current_clk;
extern volatile u_int16 current_lost_window_size;
//extern volatile u_int32 current_clk_TIM1;
//volatile u_int32 anchor_point_delta_current_clk;
#define NO_DETECT          0
#define LONG_PRESS_DETECT  1
#define SHORT_PRESS_DETECT 2
#define BUTTON_RELEASE    3
u_int32 AUX_setting_value=0;
u_int32 conn_interval_clk_base;
u_int16 Get_LE_adv_interval(void);

//u_int32 Get_sleep_start_current_clk(void)
//{
//    return sleep_start_current_clk;
//}

void Set_need_to_sleep(void)
{
    need_to_sleep = 1;
}
void Clr_need_to_sleep(void)
{
    need_to_sleep = 0;
}

void Set_need_to_write(void)
{
    need_to_write_data = 1;
}

void Clr_need_to_write(void)
{
    need_to_write_data = 0;
}

u_int8 Get_need_to_write(void)
{
    return need_to_write_data;
}

void Clr_need_to_Idle(void)
{
    need_to_Idle = 0;
}
u_int8 Get_need_to_Idle(void)
{
    return need_to_Idle;
}
void Set_need_to_Idle(void)
{
    need_to_Idle = 1;
}
void Cal_Enter_low_power_mode_count(void)
{
    //Enter_low_power_mode_count = (current_connectInterval/300)+3;
    Enter_low_power_mode_count = 10;
    //Enter_low_power_mode_count = 100;
}
void Set_earlywakeup(void)
{
	  /* u_int8 active_link = LE_config.active_link; */
	  /* t_LE_Connection* p_connection  = LEconnection_Find_Link_Entry(active_link); */
// 	#ifndef DISABLE_LOW_POWER_CONNECT
// 	  if( (CRC_error_count < no_packet_recv_flag) && (no_packet_recv_flag == 1))
// 	{
// 		   early_wakeup = 4; /* cover AUX timer switch to native clock fail issue */

// 	}
// 	else
// 	#endif
//   {
//   
//        early_wakeup = 0;
// 	}
	//if (LEconnection_Get_Initial_Anchor_Point_Obtained(p_connection)==1)
	{
//		if(no_packet_recv_flag >= 7 /* 0 */)
//		{
//	       early_wakeup = 4;//4; /* cover AUX timer switch to native clock fail issue */
//			   
//		}
//		else
		{
					early_wakeup = 0;
		}
	}
}
void Pre_Sleep_State_patch(u_int8 IRQ_Type)
{
    if ((IRQ_Type == LE_TIM_0) || (IRQ_Type >= LE_TIM_1))
    {
#if (IDLE_MODE == 1)
        need_to_Idle = 1;
#endif
        LE_config.device_sleep = 1;
        if(SYSpwr_Is_Low_Power_Mode_Active())
        {
            LSLCirq_Disable_All_Tim_Intr_Except(JAL_TIM0_INTR_MSK_MASK |JAL_TIM2_INTR_MSK_MASK );
            //REG_ICU_INT_ENABLE &= (~INT_ENABLE_BLE_bit);   //BLE FIQ disable
            LE_config.state = SLEEP_STATE;
//            sleep_start_current_clk = current_clk_patch;
            SYSpowerDown_now();
        }
    }
    else
    {
#if (IDLE_MODE == 1)
        need_to_Idle = 1;
#endif
    }
}
void Wakeup_State_patch(void)
{
    if(LEadv_Get_Advertising_Enable())
    {
        Clr_need_to_sleep();
        LE_config.state = STANDBY_STATE;
    }
    else
    {
        need_to_sleep = 0;
        LE_config.state = STANDBY_STATE;
    }
}

void Link_Layer_AUX_setting_patch(u_int32 current_clk)
{
	  #ifdef DISABLE_LOW_POWER_CONNECT
    //if(button_detect_ctrl != BUTTON_RELEASE)
    {
        need_to_sleep = 0;/* charles test */
    }
		#endif
		
		#if (MMI_ON == 1)/*  revision 335 change */
		
		if((button_detect_ctrl != BUTTON_RELEASE) || (Get_need_to_write()))
		{
		    need_to_sleep = 0;
		}
		#endif
// 		if( (CRC_error_count < no_packet_recv_flag) && (no_packet_recv_flag == 3))
// 		{
// 			need_to_sleep = 0;
// 		}
    if(no_packet_recv_flag >= 7 /* 0 */)/* option 1 because aux timer transfer to native clock not precise, */
                                  /* so if no packets receive must close aux timer just use native clock*/
		{
			//need_to_sleep = 0;
		}
// 		  if(current_clk > current_clk_TIM1)
// 		    anchor_point_delta_current_clk = current_clk-current_clk_TIM1;
// 		  else
// 	        anchor_point_delta_current_clk = current_clk_TIM1-current_clk;

    if(LEadv_Get_Advertising_Enable())
    {
			 
//        if((Get_MMI_State()== MMI_DISCONNECTED_STATE) && ((REG_AHB0_ICU_PWMCLKCON&0x01)==0))
//        {
//            need_to_sleep = 0;
//        }
 //       if(low_power_adv_event_count == 1) {
            if(need_to_sleep == 1) {
               
							  //AUX_setting_value = ((gCurrent_ADV_interval*625/1000)-5)*10/25;
							  AUX_setting_value = ((gCurrent_ADV_interval*625/1000)-10)*10/25;
                

							  
                LSLCirq_Disable_All_Tim_Intr_Except(JAL_TIM0_INTR_MSK_MASK |JAL_TIM2_INTR_MSK_MASK | JAL_TIM1_INTR_MSK_MASK |JAL_TIM3_INTR_MSK_MASK);
                LE_config.state = PRE_SLEEP_STATE;
                LE_config.device_sleep = 1;
#if (IDLE_MODE == 1)
                need_to_Idle = 1;
#endif
            }
 //       }
    } else if ((LE_config.num_le_links) && (LE_config.slave_link_active) && (low_power_connect_event_count >= Enter_low_power_mode_count)) {
//		if((Get_MMI_State()== ALERT_LEVEL_SETTING_STATE) && ((REG_AHB0_ICU_PWMCLKCON&0x01)==0))
//			need_to_sleep = 0;

        if(need_to_sleep == 1) {
            //current_connectInterval : 240 * 1.25 == 300 ms
            //current_connectInterval : 24 * 1.25 == 30 ms
					  //2.5ms=80*31.25us /* 32K XTAL */
					  //(1/32.768) * 128 = 3.90625ms
            //(1/32) * 125 = 3.90625ms
					  //2000 is 80 and 125 LCM value
					  //2000*31.25us = 2048*(1/32.768);
					  //2000/80 = 25---> min AUX value
            u_int32 value = (u_int32)current_connectInterval*125/100;
			conn_interval_clk_base = (u_int32)current_connectInterval*4;//unit = 1.25ms = 4 clks
			

			if(value <= 12)		/* connection_interval < 12 ms */	
						{
							need_to_sleep = 0;
							goto no_need_sleep;
						}
					  #ifdef LPO_CLK_SOURCE_32000HZ
		//	AUX_setting_value = 1;			////(value-8)*10/25;			////AUX_setting_value = (value-10)*10/25;
//		if(value < 30)/* 30 ms */
//		{
//		  AUX_setting_value = (value-9)*10/25;
//			AUX_setting_value += 1;
//			
//    }
//    else
    {
			  //aux_timer unit = 2.5 ms = 8 clks
			  AUX_setting_value = (conn_interval_clk_base-20)/8;
//			AUX_setting_value = (value-10)*10/25;
//			mod_value = ((value-10)*10)%25;
//		  AUX_setting_value += 1;
//			if(mod_value >= 5)
//			{
//				AUX_setting_value += 1;
//			}
    }  	
              //DEBUG_MSG(0x99);
		//DEBUG_MSG(conn_interval_clk_base);
		//DEBUG_MSG(0x99);
		//DEBUG_MSG(AUX_setting_value);		
					  #endif
					  #ifdef LPO_CLK_SOURCE_32768HZ
           
			if( (AUX_setting_value = (value * 10/25))>= MIN_AUX_VALULE)			////if(( (AUX_setting_value =(value-10)* 10/25))>= MIN_AUX_VALULE)
						{
							 AUX_setting_value = AUX_setting_value / MIN_AUX_VALULE;
							 AUX_setting_value = AUX_setting_value * MIN_AUX_VALULE;
						}
						else
						{
							need_to_sleep = 0;
							goto no_need_sleep;
						}
					  
					  #endif
					   //if( (CRC_error_count < no_packet_recv_flag) && (no_packet_recv_flag == 1))
						 if(early_wakeup == 4)
						 {
							 AUX_setting_value -= 1;
						 }
// 					  if(no_packet_recv_flag == 3)/* option 2 to cover AUX timer switch to native clock fail issue */
// 						{
// 							AUX_setting_value -= 1;
// 						}
					    //AUX_setting_value -= (anchor_point_delta_current_clk/8);
					 
            LSLCirq_Disable_All_Tim_Intr_Except(JAL_TIM0_INTR_MSK_MASK |JAL_TIM2_INTR_MSK_MASK | JAL_TIM1_INTR_MSK_MASK |JAL_TIM3_INTR_MSK_MASK);
            LE_config.state = PRE_SLEEP_STATE;
			
#if (IDLE_MODE == 1)
            need_to_Idle = 1;
#endif
		} else {
no_need_sleep :
#if (IDLE_MODE == 1)
            need_to_Idle = 1;
#endif
			  return;
        }
    } else {
#if (IDLE_MODE == 1)
        need_to_Idle = 1;
#endif
    }
}
