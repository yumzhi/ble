/*****************************************************************************
 *
 * MODULE NAME:    sys_mmi.c
 * PROJECT CODE:   BlueStream
 * DESCRIPTION:    man-machine interface - visual debug feedback on
 *                 stack "events" happening.
 *
 * MAINTAINER:     Ivan Griffin
 * DATE:           20 September 2000
 *
 * SOURCE CONTROL: $Id: sys_mmi.c,v 1.7 2004/07/07 14:30:49 namarad Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2004 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    20 Sept 2000  -   IG       - Initial Version 
 *
 * SOURCE: 
 * ISSUES:
 * 
 *****************************************************************************/
#include "BKReg.h"
#include "BKAPI.h"
#include "hw_lc_impl.h"
#include "sys_mmi.h" 
#include "le_cts_app.h"
#include "bt_timer.h"
#include "le_config.h"
#include "sys_power.h"
#include "le_advertise.h"
#include "le_att.h"
#include "LE_gatt_server.h"
#include "le_gap.h"
#include "debug.h"
#include "BKUart.h"

#define DEBOUNCE  10
#define POWER_ON_STANDBY_TO     576000//576000 * 0.3125 ms = 3 min
#define LONG_PRESS      3200    //6400 * 0.3125 = 2 sec
#define SHORT_PRESS     400    //800 * 0.3125 = 250 ms
#define NO_DETECT          0
#define LONG_PRESS_DETECT  1
#define SHORT_PRESS_DETECT 2
#define BUTTON_RELEASE    3

MMI_STATE_t MMI_STATE;
MMI_SUB_STATE_t MMI_SUB_STATE;
static u_int8  proc = 0;
static u_int16 MMI_cycle_time = 0;
static u_int8  current_button_state = 1;
static u_int8  last_button_state = 1;
volatile u_int8  button_detect_ctrl = NO_DETECT;//0 : no detect, 1: long press detect , 2 : short press detect
//extern volatile u_int8 button_release;
//static u_int8  Alert_Switch = 0;
extern t_App_Control LEapp_Config;
extern  t_LE_Config LE_config;
static u_int8  uart_len;
static u_int8  uart_buf[64];

typedef void (*MMI_FUNCTION)(void);
void Clr_need_to_write(void);
void Set_need_to_write(void);
//typedef u_int16 (*GATT_DB_setup_function)(void);//merge CEVA 0517
MMI_FUNCTION MMI_functions[9] = {
    DUMMY_FUNC,
    POWER_ON_MMI,
    ADVERTISING_MMI,
    CONNECTION_SUCCESS_MMI,
    DISCONNECTED_MMI,
    LOST_CONNECTION_MMI,
    DEEPSLEEP_MMI,
    SEND_DATA_MMI,
    ALERT_LEVEL_SETTING_MMI,
};
void Set_MMI_Cycle_time(u_int16 value)
{
    MMI_cycle_time = value;
}
MMI_STATE_t Get_MMI_State(void)
{
    return MMI_STATE;
}
void Set_MMI_State(MMI_STATE_t value)
{
    MMI_STATE = value;
    MMI_SUB_STATE = MMI_START;
    if((value == MMI_IDLE ) || (value == MMI_ADVERTISING_STATE))
    {
        Clr_need_to_write();
    }
    else
    {
        Set_need_to_write();
    }
}

__INLINE__ void Set_MMI_SUB_State(MMI_SUB_STATE_t value)
{
     MMI_SUB_STATE = value;
}
// /* completely empty C files are illegal in ANSI */
// void SYSmmi_Dummy__(void) { }

//Do(523Hz), Re (587Hz), Mi(659Hz), Fa(698Hz), So(784Hz), La(880Hz), Si(988Hz) 到高音 Do (1047Hz) 這八個不同音階的聲音，每個音階響 0.5 秒鐘
//Do : PWM counter = 1912 us
//Re       1703
//Mi : PWM counter = 1517 us
//Fa : PWM counter = 1432 us
//So : 1275 us
//La : 1136 us
//Si : 1012 us
//high Do : 995 us    
//#define LED0_ON(x){REG_GPIOB_DATA  |= 0x00000008; }
void LED0_ON(void)
{
#if (MMI_IO_ENABLE == 1)
    REG_GPIOB_DATA  |= 0x00000008;
#endif   //P1.3//Red LED
}
//#define LED1_ON(){REG_GPIOB_DATA  |= 0x00000010; }
void LED1_ON(void)//P1.4 //Blue LED
{
#if (MMI_IO_ENABLE == 1)
    //REG_GPIOB_DATA  |= 0x00000010;
#endif
}
//#define LED_OFF(){REG_GPIOB_DATA  &= ~0x00000018;}
void LED_OFF(void)//P1.3,P1.4
{
#if (MMI_IO_ENABLE == 1)
    //REG_GPIOB_DATA  &= ~0x00000018;
#endif
}
void BUZZER_HIGH_RATE(void)
{
#if (MMI_IO_ENABLE == 1)
    BKPWMInit(PT0_MODE_BIT, BK_PWM_MODE, 10);
#endif
}
void BUZZER_LOW_RATE(void)
{
#if (MMI_IO_ENABLE == 1)
    BKPWMInit(PT0_MODE_BIT, BK_PWM_MODE, 50);
#endif
}
void MMI_FINISH(void)
{
    if(MMI_cycle_time == 0xFFFF)
    {
        MMI_SUB_STATE = MMI_START;
        return;
    }
    
    if(MMI_cycle_time)
    {
        MMI_cycle_time--;
        MMI_SUB_STATE = MMI_START;
    }
    else
    {
        MMI_SUB_STATE = MMI_END;
    }
}
void MMI_init(void)
{
    u_int8 i;
    for(i=0;i<3;i++)
    {
        LEapp_Config.MMI_Timer[i] = BT_CLOCK_MAX_TIMEOUT;
    }        
}
void MMI_DUMMY(void)
{
}
void POWER_ON_MMI(void)
{ 
    u_int32 current_clk;

    Set_MMI_State(MMI_ADVERTISING_STATE);//MMI_ADVERTISING_STATE,customer require
    current_clk = HW_Get_Native_Clk_Avoid_Race();
    LEapp_Config.TAPP_POWERON_STANDBY_Timer = current_clk+POWER_ON_STANDBY_TO;
}
void ADVERTISING_MMI(void)
{
    const t_MMI_SUB_FUNC  LED_PROC[5] = {{LED1_ON,160},{LED_OFF,3200},{LED1_ON,160},{LED_OFF,3200},{MMI_FINISH,0}};
    u_int32 current_clk;

    current_clk = HW_Get_Native_Clk_Avoid_Race();

    switch(MMI_SUB_STATE)
    {
    case MMI_START :
        if(LEadv_Get_Advertising_Enable())
        {
            Set_MMI_Cycle_time(1);
        }
        else
        {
            Set_MMI_Cycle_time(0);
        }
        LED_OFF();
        proc = 0;
        Set_MMI_SUB_State(MMI_NEXT_PROC);
        //break;
    case MMI_NEXT_PROC :
        LEapp_Config.MMI_Timer[0] = current_clk+LED_PROC[proc].time;
        Set_MMI_SUB_State(MMI_ACTIVE);//must place before run MMI_sub_func
        LED_PROC[proc].MMI_sub_func(); 
        proc++;//next procedure
        break;
    case MMI_ACTIVE :
        if ((LEapp_Config.MMI_Timer[0]  !=  BT_CLOCK_MAX_TIMEOUT) &&
            (((LEapp_Config.MMI_Timer[0]  - current_clk - 1) & BT_CLOCK_MAX_TICKS) >= BT_CLOCK_MAX_TIMEOUT))
        {                                    
            Set_MMI_SUB_State(MMI_NEXT_PROC);
        }
        break;
    case MMI_END :
        Set_MMI_State(MMI_IDLE);
        break;
    }
}
void CONNECTION_SUCCESS_MMI(void)
{
    LED_OFF();
    Beken_PWM_CLK_PowerDown();
    Set_MMI_State(MMI_IDLE);             
    return;
#if 0
    const t_MMI_SUB_FUNC  LED_PROC[5] = {{LED1_ON,320},{LED_OFF,320},{LED1_ON,320},{LED_OFF,16},{MMI_FINISH,2}};
    const t_MMI_SUB_FUNC  BUZZER_PROC[5] = {{BUZZER_HIGH_RATE,320},{Beken_PWM_DISABLE,320},{BUZZER_HIGH_RATE,320},{Beken_PWM_DISABLE,16},{MMI_FINISH,2}};//time unit 312.5 us
    u_int32 current_clk;
         
    //while(1)
    {
        current_clk = HW_Get_Native_Clk_Avoid_Race();
            
        switch(MMI_SUB_STATE)
        {
        case MMI_START :
            Set_MMI_Cycle_time(0);
            LED_OFF();
            proc = 0;
#if (BUZZER_ENABLE == 1)
            Beken_PWM_CLK_PowerUp();
#endif
            Set_MMI_SUB_State(MMI_NEXT_PROC);
            //break;
        case MMI_NEXT_PROC :
                           
            LEapp_Config.MMI_Timer[0] = current_clk+LED_PROC[proc].time;
            LEapp_Config.MMI_Timer[1] = current_clk+BUZZER_PROC[proc].time;
            Set_MMI_SUB_State(MMI_ACTIVE);//must place before run MMI_sub_func
            LED_PROC[proc].MMI_sub_func();
                       
            BUZZER_PROC[proc].MMI_sub_func();
                       
                             
            proc++;//next procedure
            break;
        case MMI_ACTIVE :
            if ((LEapp_Config.MMI_Timer[0]  !=  BT_CLOCK_MAX_TIMEOUT) &&
                (((LEapp_Config.MMI_Timer[0]  - current_clk - 1) & BT_CLOCK_MAX_TICKS) >= BT_CLOCK_MAX_TIMEOUT))
            {
                                    
                //if(proc < 4)
                //{
                Set_MMI_SUB_State(MMI_NEXT_PROC);
                                    
                //}
                //else
                //{
                                    
                //  Set_MMI_SUB_State(MMI_END);
                //}
            }
            break;
        case MMI_END :
            Beken_PWM_CLK_PowerDown();
            //#if (MMI_IO_ENABLE == 1)
            //REG_APB5_GPIOB_CFG  = 0x0000F0FC; ;           //GPIO-B as Output. LED0(P1.2),LED1(P1.3)
            //REG_APB5_GPIOB_DATA = 0x00000000 ; 
            //#endif
            Set_MMI_State(MMI_IDLE);
                         
            return;
            break;
        }
         
    } 
#endif
}

void ALERT_LEVEL_SETTING_MMI(void)
{
    if(LEapp_Config.Alert_Level > 0)
    {    
        DISCONNECTED_MMI();
    }
    else if(LEapp_Config.Alert_Level == 0)
    {
        Set_MMI_Cycle_time(0);
        MMI_FINISH();
        DISCONNECTED_MMI();
    }
}

void DISCONNECTED_MMI(void)
{
#if (BUZZER_ENABLE == 1)
    const t_MMI_SUB_FUNC  LED_PROC[3] = {{LED0_ON,640},{LED_OFF,640},{MMI_DUMMY,2}};//SecuriTag TI sample
    const t_MMI_SUB_FUNC  BUZZER_PROC[3] = {{BUZZER_HIGH_RATE,640},{Beken_PWM_DISABLE,640},{MMI_FINISH,2}};//SecuriTag TI sample//time unit 312.5 us
#endif
#if (BUZZER_ENABLE == 0)
    const t_MMI_SUB_FUNC  LED_PROC[3] = {{LED0_ON,640},{LED_OFF,1920},{MMI_FINISH,2}};//SecuriTag TI sample
#endif
		
    //const t_MMI_SUB_FUNC  LED_PROC[3] = {{LED0_ON,320},{LED_OFF,320},{MMI_DUMMY,2}};
    //const t_MMI_SUB_FUNC  BUZZER_PROC[3] = {{BUZZER_HIGH_RATE,320},{Beken_PWM_DISABLE,320},{MMI_FINISH,2}};//time unit 312.5 us
    u_int32 current_clk;
    //cycle time = 25 , 25*(640+640)*312.5 = 10 sec
    
    current_clk = HW_Get_Native_Clk_Avoid_Race();
            
    switch(MMI_SUB_STATE)
    {
    case MMI_START :
        LED_OFF();
        proc = 0;
#if (BUZZER_ENABLE == 1)
        if(LEapp_Config.Alert_Level > 0)
        {
            Beken_PWM_CLK_PowerUp();
        }
#endif
        Set_MMI_SUB_State(MMI_NEXT_PROC);
        //break;
    case MMI_NEXT_PROC :
                           
        LEapp_Config.MMI_Timer[0] = current_clk+LED_PROC[proc].time;
        //LEapp_Config.MMI_Timer[1] = current_clk+BUZZER_PROC[proc].time;//the same as MMI_Timer[0]
        Set_MMI_SUB_State(MMI_ACTIVE);//must place before run MMI_sub_func
                         
        LED_PROC[proc].MMI_sub_func();
#if (BUZZER_ENABLE == 1)
        if(LEapp_Config.Alert_Level > 0)
        {
            BUZZER_PROC[proc].MMI_sub_func();
        }
#endif
                             
        proc++;//next procedure
        break;
    case MMI_ACTIVE :
        if ((LEapp_Config.MMI_Timer[0]  !=  BT_CLOCK_MAX_TIMEOUT) &&
            (((LEapp_Config.MMI_Timer[0]  - current_clk - 1) & BT_CLOCK_MAX_TICKS) >= BT_CLOCK_MAX_TIMEOUT))
        {
            Set_MMI_SUB_State(MMI_NEXT_PROC);
        }
        break;
    case MMI_END :
        LED_OFF();
        //if(LEapp_Config.Alert_Level > 0)
        {
            Beken_PWM_CLK_PowerDown();
        }
#if (MMI_IO_ENABLE == 1)
        REG_APB5_GPIOB_CFG  = 0x0000e7fc; ;           //GPIO-B as Output. LED0(P1.2),LED1(P1.3)
#endif
        Set_MMI_State(MMI_IDLE);
        break;
    }
}

void LOST_CONNECTION_MMI(void)
{
    
}
void SEND_DATA_MMI(void)
{
    static u_int8 temp;
    u_int32 current_clk;
    current_clk = HW_Get_Native_Clk_Avoid_Race();
    
    switch(MMI_SUB_STATE)
    {
    case MMI_START:
        LEapp_Config.MMI_Timer[0] = current_clk+5;
        Set_MMI_SUB_State(MMI_ACTIVE);
        break;
    case MMI_ACTIVE:
        if ((LEapp_Config.MMI_Timer[0]  !=  BT_CLOCK_MAX_TIMEOUT) &&
        (((LEapp_Config.MMI_Timer[0]  - current_clk - 1) & BT_CLOCK_MAX_TICKS) >= BT_CLOCK_MAX_TIMEOUT))
        {
            LEapp_Config.MMI_Timer[0] = current_clk+100;
            if(uart_len>temp)
            {
                if((uart_len-temp) > 20)
                {
                    User_Notify(&uart_buf[temp], 20);
                    temp+=20;
                }
                else
                {
                    User_Notify(&uart_buf[temp], uart_len-temp);
                    temp = uart_len;
                }
            }
            else
            {
                temp = 0;
                uart_len = 0;
                Set_MMI_SUB_State(MMI_END);
            }
        }
        break;
    case MMI_END:
    default:
        Set_MMI_State(MMI_IDLE);
        break;
    }
}
void DEEPSLEEP_MMI(void)
{
    u_int8 active_link = LE_config.active_link;
    t_LE_Connection* p_connection  = LEconnection_Find_Link_Entry(active_link);
    LED_OFF();
    if(LEconnection_Is_Active(p_connection) == 1)
    {
        if(MMI_SUB_STATE == MMI_START)
        {
            MMI_SUB_STATE = MMI_ACTIVE;
            GAP_Disconnect(0xFFFF);
        }
        return;
    }
         
    Beken_PWM_CLK_PowerDown();
    Set_MMI_State(MMI_IDLE);
    entry_deep_sleep();
}
void DUMMY_FUNC(void)
{
    ;
}

void Set_current_button_state(u_int8 value)
{
    current_button_state = value;
}

void Set_last_button_state(u_int8 value)
{
    last_button_state = value ;
}
     
void MMI_main(void)
{
    MMI_functions[MMI_STATE]();
    if(uart_len == 0)
    {
        uart_len = Beken_UART_Rx(uart_buf);
    }
    else
    {
        if(Get_MMI_State() != MMI_SEND_REPORT_STATE)
            Set_MMI_State(MMI_SEND_REPORT_STATE);
    }
}
