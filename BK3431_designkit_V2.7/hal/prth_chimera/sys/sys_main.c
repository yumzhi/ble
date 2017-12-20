/*****************************************************************************
 *
 * MODULE NAME:    sys_main.c
 * PROJECT CODE:   BlueStream
 * DESCRIPTION:    Entry point to the Bluetooth stack
 * MAINTAINER:     Tom Kerwick
 * DATE:           21 January 2000
 *
 * SOURCE CONTROL: $Id: sys_main.c,v 1.56 2008/07/03 12:26:45 tomk Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2008 Ceva Inc.
 *     All rights reserved.
 *
 *****************************************************************************/

#include "sys_config.h"
#include "sys_types.h"
#include "sys_const.h"
#include "sys_post.h"

#include "bt_init.h"
#include "bt_mini_sched.h"

#include "sys_irq.h"

#include "hw_lc.h"
#include "hw_register.h"
#include "hw_hab_defs.h"

#include "tra_hcit.h"
#include "BKAPI.h"
#include "debug.h"
#include "stdio.h"
#include "string.h"
#include "le_cts_app.h"
#include "le_config.h"
#include "le_advertise.h"
#include "lslc_irq.h"
#include "sys_power.h"
#include "sys_mmi.h"
#include "hw_radio.h"
#include "hw_radio_defs.h"
#include "sys_mmi_impl.h"
#include "le_gap.h"
#include "BKUart.h"
#include "BK3431_gpio.h"

static void _SYS_Main_Loop(void);
void SYS_Main_Initialise(void);
volatile u_int8  RF_test_mode_enable = 0;
static void _SYS_Main_Perform_Stack_Integrity_Check(void);

void SYSpwr_Sleep_System(void);
void BKUartInit(unsigned int index, unsigned int speed, unsigned char* pRx, unsigned int rxLen);

#if (BUILD_TYPE==GENERIC_BUILD)
void *HCI_Generic_register_hcit(void);
int prh_generic_hci_ready = 0;
#endif

extern u_int32 AUX_setting_value;
extern volatile u_int32 flag_aux_intr_done;

/*****************************************************************************
 *
 * FUNCTION:    Main
 * PURPOSE:     Instantiation
 *
 *****************************************************************************/
#if ((BUILD_TYPE!=FLASH_BUILD) || (defined(ADS_TOOLCHAIN_SUPPORTED)))

#if (BUILD_TYPE==FLASH_BUILD)
void SYS_Vector_Init_Block(void);
#endif

int main (int argc, char *argv[])
{
    SYS_Main(); /* never returns */

#if (BUILD_TYPE==FLASH_BUILD)
    SYS_Vector_Init_Block();
#endif
    return 0;
}
#endif

/*****************************************************************************
 *
 * FUNCTION:    SYS_Main
 * PURPOSE:     Instantiation
 *
 *****************************************************************************/
void SYS_Main(void)
{
    extern  t_LE_Config LE_config;
    
#if (BUILD_TYPE!=GENERIC_BUILD)
    SYS_Main_Initialise();
#else
    while(prh_generic_hci_ready==0);
#endif
    Set_MMI_State(MMI_POWER_ON_STATE);
#if (MMI_ON == 1)
    MMI_main();
#endif
#if (ADC_ON == 1)
    Power_ON_Detect_Battery();
#endif
    bprintf("Power on!\r\n");
   //Beken_UART_Close();
    kmod_calibration();
    _SYS_Main_Loop();
}

/******************************************************************************
 *
 * FUNCTION:  SYS_Main_Initialise
 * PURPOSE:   Controller Initialisations
 *
 ******************************************************************************/
void SYS_Main_Initialise(void)
{
    u_int32 flags;

    SYSirq_Disable_Interrupts_Save_Flags(&flags);

    //---------------------------------------
    //BK3431 Initialization
    GPIO_initial();
    LED1_ON();//Blue LED On

    //initial_xver_BK3431V2_closeLoop(); 
    initial_xver_BK3431V2_openLoop();

    REG_AHB0_ICU_ANALOG_MODE = 0x0;
    REG_AHB0_ICU_ANALOG1_PWD = 0x0;

    REG_AHB0_ICU_RC32K_CONFG   = SET_CALI_MODE;  // Keep Auto-Calibration 32KHz in reset
    REG_AHB0_ICU_SLEEP_TIME    = 0;  // Disable Auto-Calibration
#ifdef LPO_CLK_SOURCE_32000HZ
    REG_AHB0_ICU_LPOCLKCON  = 0x0 ; /* 32KHz From ROSC or 16MHz division */
#endif
#ifdef LPO_CLK_SOURCE_32768HZ
    REG_AHB0_ICU_LPOCLKCON  = 0x2 ; /* 32KHz Generate by Xtal(32.768KHz) */
#endif
#ifdef SEL_LPO_TYPE_XTAL
    REG_AHB0_ICU_CLK32K_SEL = 0x0 ;/* 32KHz source from ROSC or 32.768KHz Xtal */
#endif
#ifdef SEL_LPO_TYPE_16M_DIV
    REG_AHB0_ICU_CLK32K_SEL = 0x1 ;/* 32KHz source from 16MHz frequency division */
#endif

    HWradio_Init_Tx_Power_Level(0x09);//0dbm real value : +3.68 dbm

    /// 20140421 move code from SYSirq_Initialise();
    REG_ICU_INT_ENABLE    |= INT_ENABLE_BLE_bit;   //BLE FIQ enable,UART INT enable
    REG_AHB0_ICU_FLASH    = 1;//wakeup flash wait time ,1*1/32K = 0.03125 ms = 31.25 us

   #ifdef GPIO_DEBUG //For DEBUG  from E t0 B

    //REG_APB5_GPIOB_CFG        = 0x000000FF;       // gpioe[0] pull up
    //REG_APB5_GPIOB_DATA       = 0x00000000;       // GPIO-E Not Input 
  //  while(1)
//		{
//			REG_APB5_GPIOB_DATA       = 0x00000000;       // GPIO-E Not Input
//			REG_APB5_GPIOB_DATA       = 0x000000ff;       // GPIO-E Not Input
//		}

   #endif
	 
    DEBUG_MSG(0XAA);
    DEBUG_MSG(0X55);
    DEBUG_MSG(0XCC);
    DEBUG_MSG(0X33);

  
 
    SYSconfig_Initialise();

    /*
     * Set up the HCI transport registration function (this function
     * is called on intialisation or re-initialisation of the HCI
     * transport sub-system
     */
#if (BUILD_TYPE==GENERIC_BUILD)
    TRAhcit_Set_Register_Function(HCI_Generic_register_hcit);
#else
    TRAhcit_Set_Register_Function(TRAhcit_Register_UART_Driver);
#endif

    BT_Initialise();
    Peripheral_initial();
    Beken_UART_Initial();
    MMI_init();

#if (BUILD_TYPE==GENERIC_BUILD)
    prh_generic_hci_ready=1;
#endif
    SYSirq_Enable_Interrupts_Save_Flags(&flags);
    REG_ICU_IRQTYPE_EN     = 0x03;            //irq enable,FIQ enable
#if(MMI_IO_ENABLE == 1)
     if(Read_GPIOD(2) == 1)
     {
      
         GAP_Discoverable_Mode(GAP_NON_DISCOVERABLE, ADV_IND);
         RF_test_mode_enable = 1;
         REG_APB5_GPIOA_CFG  = 0xFF00FFFC;
         REG_APB5_GPIOA_DATA = 0X00FFFFFF;
	      
         Peripheral_initial();
			
		
         return ;
		
     }
#endif
#if(MMI_IO_ENABLE == 1)
#if (AUTO_ADVERTISING == 0)
     if(Read_GPIOD(3) == 1)
     {
        
         Set_current_button_state(1);
         Set_last_button_state(1);
         entry_deep_sleep();
     }
     else
     {
         Set_current_button_state(0);
     }
#endif
#endif
}

/******************************************************************************
 *
 * FUNCTION:  _SYS_Main_Loop
 * PURPOSE:   Controller Main Loop
 *
 ******************************************************************************/
static void _SYS_Main_Loop(void)
{
    while(1)
    {
#if defined(SYS_HAL_OS_EVENT_DRIVEN_HOST_CONTROLLER)//FLASH_BULID : currently not define
        BTms_Callback();
#else
        BTms_Sched(0x1);
#endif
        Setting_WDT_TIMER(0x4C4B);//watchdog timeout is 10s
        _SYS_Main_Perform_Stack_Integrity_Check();
      // DEBUG_MSG(0X66);
      //DEBUG_MSG(0X55);
  	bprintf("debug ok!\r\n");
    }
}

/******************************************************************************
 *
 * FUNCTION:  _SYS_Main_Perform_Stack_Integrity_Check
 * PURPOSE:   Controller Stack Integrity Monitoring
 *
 ******************************************************************************/
#include "sys_irq.h"
#include "hc_event_gen.h"
//u_int8  =0x00403FF8
u_int32 IRQ_STACK[2] __attribute__((at(0x00403FF8)));
u_int32 FIQ_STACK[128] __attribute__((at(0x00403dF8)));
u_int32 SVC_STACK[96] __attribute__((at(0x00403C78)));
u_int32 USR_STACK[160] __attribute__((at(0x004039F8)));
u_int32 SYS_STACK[64] __attribute__((at(0x004038F8)));
static void _SYS_Main_Perform_Stack_Integrity_Check(void)
{

#if (BUILD_TYPE==FLASH_BUILD)
    //extern u_int32 FIQ_STACK;
    //extern u_int32 SVC_STACK;
    //extern u_int32 USR_STACK;
	 // extern u_int32 IRQ_STACK;
   // extern u_int32 SYS_STACK;

    if ((FIQ_STACK[0] != (u_int32)0xDEADBEEF) ||
        (SVC_STACK[0] != (u_int32)0xDEADBEEF) ||
        (USR_STACK[0] != (u_int32)0xDEADBEEF) ||
		    (IRQ_STACK[0] != (u_int32)0xDEADBEEF) ||
		    (SYS_STACK[0] != (u_int32)0xDEADBEEF)
        )
    {
        DEBUG_MSG(0xF8);
			if((FIQ_STACK[0] != (u_int32)0xDEADBEEF))
            DEBUG_MSG(0x01);
			if((SVC_STACK[0] != (u_int32)0xDEADBEEF))
            DEBUG_MSG(0x02);
			if((USR_STACK[0] != (u_int32)0xDEADBEEF))
            DEBUG_MSG(0x03);
			if((IRQ_STACK[0] != (u_int32)0xDEADBEEF))
            DEBUG_MSG(0x04);
		  if((SYS_STACK[0] != (u_int32)0xDEADBEEF))
            DEBUG_MSG(0x05);
// 			  
//         //HCeg_Hardware_Error_Event(PRH_BS_HW_ERROR_STACK_CORRUPTION);
// 	/* TRAhcit_Dispatch_Pending_Event(void); */
//         /* SYSirq_Disable_Interrupts_Save_Flags(&flags); */
//         /* while(1); */
    }
#endif
}
