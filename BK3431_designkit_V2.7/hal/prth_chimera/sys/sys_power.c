/*****************************************************************************
 *
 * MODULE NAME:    sys_power.c
 * PROJECT CODE:   BlueStream
 * DESCRIPTION:    Low Power Clock Activation
 * MAINTAINER:     Tom Kerwick
 * DATE:           10 December 2001
 *
 * SOURCE CONTROL: $Id: sys_power.c,v 1.12 2011/04/01 16:03:27 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2008 Ceva Inc.
 *     All rights reserved.
 *
 *****************************************************************************/

#include "sys_config.h"
#include "sys_types.h"
#include "sys_const.h"

#include "hw_delay.h"
#include "hw_lc.h"
#include "lslc_irq.h"
#include "sys_hal_features.h"
#include "debug.h"
#include "BKAPI.h"

#if (TRA_HCIT_GENERIC_SUPPORTED==1)
extern void (*hci_generic_exit_hc_callback)(void);
#endif

static boolean syspwr_low_power_mode_active_;
static boolean sys_sleep_now = 0;
static u_int8 LowFrequencyOscillatorAvailable = 0;

/* can be increased if CPU is slown down and execution takes time */
#define SYS_PWR_MIN_FRAMES_TO_SLEEP      1
#define SYS_PWR_AUX_TIMER_MIN_RESOLUTION 4
void go_to_Idle(void);//turn off  ARM core clock source


/*****************************************************************************
 * FUNCTION:   SYSpwr_Halt_System
 * PURPOSE:    Turn off entire chip / Deep Sleep
 ****************************************************************************/
void SYSpwr_Halt_System(void)
{
#if (TRA_HCIT_UART_ONLY_SUPPORTED==1)
    // may add cpu halt/sleep functionality here
#elif (TRA_HCIT_GENERIC_SUPPORTED==1)
    if(hci_generic_exit_hc_callback)
        hci_generic_exit_hc_callback();
#endif
    syspwr_low_power_mode_active_ = TRUE;
   // Turnoff_DEBUG_PIN(); // for debug temporay mark
   //DEBUG_MSG(0xf0);
    while(syspwr_low_power_mode_active_)
    {
        go_to_Idle();
	 //DEBUG_MSG(0xf1);
        if(sys_sleep_now)
        {
            SEL_32K_Clock_SOURCE();
	      //DEBUG_MSG(0xf2);
        }
    }
}

void SYSpowerDown_now(void)
{
    sys_sleep_now = TRUE;
}
/*****************************************************************************
 * FUNTION:    SYSpwr_Sleep_System()
 * PURPOSE:    Turn off CPU clock / Light Sleep
 ****************************************************************************/
void SYSpwr_Sleep_System(void)
{
#if (TRA_HCIT_UART_ONLY_SUPPORTED==1)
    // may add cpu halt/sleep functionality here
#elif (TRA_HCIT_GENERIC_SUPPORTED==1)
    if(hci_generic_exit_hc_callback)
        hci_generic_exit_hc_callback();
#endif
    syspwr_low_power_mode_active_ = TRUE;
}

/*****************************************************************************
 * FUNCTION:   SYSpwr_Exit_Halt_System()
 * PURPOSE:    Wake entire chip back up (re-enable any gated clocks etc.)
 ****************************************************************************/
void SYSpwr_Exit_Halt_System(void)
{
    syspwr_low_power_mode_active_ = FALSE;
    sys_sleep_now = FALSE;
}

/*****************************************************************************
 * FUNCTION:   SYSpwr_Is_Available_Halt_System()
 * PURPOSE:    Returns TRUE if system is available to halt.
 ****************************************************************************/
int SYSpwr_Is_Available_Halt_System(void)
{
    return (syspwr_low_power_mode_active_);
}


/*****************************************************************************
 * FUNCTION:   SYSpwr_LowFrequencyOscillatorAvailable()
 * PURPOSE:    Returns TRUE if low freq oscillator is available.
 ****************************************************************************/
boolean SYSpwr_LowFrequencyOscillatorAvailable(void)
{
    return LowFrequencyOscillatorAvailable;
}

/*****************************************************************************
 * FUNCTION:   SYSpwr_Initialise
 * PURPOSE:
 ****************************************************************************/
void SYSpwr_Initialise(void)
{
    syspwr_low_power_mode_active_ = FALSE;

    /*
     * Leave low power oscillator disabled until enabled by TCI.
     */
    LowFrequencyOscillatorAvailable = SYS_LF_OSCILLATOR_PRESENT;
}

/*****************************************************************************
 * FUNCTION:   SYSpwr_Handle_Early_Wakeup
 * PURPOSE:
 ****************************************************************************/
void SYSpwr_Handle_Early_Wakeup(void)
{
    /* wake chip back up to check on UART status */
    SYSpwr_Exit_Halt_System();

    /*
     * DEPLOYMENT TEAM need to put code into this if() to check for HCI transport
     * activity. If TRUE, then subsequent 'naps' for this sleep period will be aborted
     */
    // if ( (!syspwr_num_frames_to_sleep_)
    /* || ((UART Transport Activity) || (USB Transport Activity)) */
    //    )
    {
        /*
         * disable the sleep timer
         * as Setup_Sleep_Timer above uses AUX_TIMER, must disable it here
         */
        Enable_16M_Clock_When_Boot();
        //HW_set_aux_tim_intr_mask(1);
        //LSLCirq_Disable_Aux_Tim_Intr();

        //  USLCchac_Wakeup(); /* move onto next state, wake system up... */
    }
    //     else
    //     {

    //         SYSpwr_Setup_Sleep_Timer(syspwr_num_frames_to_sleep_);

    //         SYSpwr_Halt_System();
    //     }
}

/*****************************************************************************
 * FUNCTION:   SYSpwr_Is_Low_Power_Mode_Active
 * PURPOSE:
 ****************************************************************************/
boolean SYSpwr_Is_Low_Power_Mode_Active(void)
{
    return (syspwr_low_power_mode_active_);
}

/*****************************************************************************
 * FUNCTION:   SYSpwr_Set_LowFrequencyOscillatorAvailable_Value()
 * PURPOSE:    set if oscillator is available.
 *             Returns TRUE if request was processed.
 ****************************************************************************/
boolean SYSpwr_Set_LowFrequencyOscillatorAvailable_Value(u_int8 osc_available)
{
    if((SYS_LF_OSCILLATOR_PRESENT==0) && (osc_available==1))
        return FALSE;
    LowFrequencyOscillatorAvailable = osc_available;
    return TRUE;
}

/*****************************************************************************
 * SYSpwr_Get_Min_Frames_To_Sleep
 * minimum amount of frames that device can be put in deep sleep
 ****************************************************************************/
u_int32 SYSpwr_Get_Min_Frames_To_Sleep(void)
{
    return (SYS_PWR_MIN_FRAMES_TO_SLEEP + SYS_PWR_AUX_TIMER_MIN_RESOLUTION);

}

void entry_deep_sleep(void)
{
    // The low power mode: 32KHz clock and MCULDO is enable. CPU core-clock is closed.
    // All GPIO are not Inputs nor Outputs. All are pull-down. 
    REG_APB5_GPIOA_CFG  = 0xFF00FFFF ; 
    //REG_APB5_GPIOB_CFG  = 0xFF00FFFF ; 
   // REG_APB5_GPIOC_CFG  = 0xFF00FFFF ; 
    REG_APB5_GPIOD_CFG  = 0xFF00FFFF ; 
    REG_APB5_GPIOE_CFG  = 0xFF00FFFF ; 
    REG_APB5_GPIOA_DATA = 0x00000000 ; 
    //REG_APB5_GPIOB_DATA = 0x00000000 ; 
    //REG_APB5_GPIOC_DATA = 0x00000000 ; 
    REG_APB5_GPIOD_DATA = 0x00000000 ; 
    REG_APB5_GPIOE_DATA = 0x00000000 ; 

    // Setup Input GPIOs
	  REG_APB5_GPIOD_CFG  = 0xF708FFFF;
    //REG_APB5_GPIOD_CFG   = 0x0008FFFF;  //Pull-up GPIO-D[3] for DeepSleep status
    REG_APB5_GPIOD_DATA |= (0x0C << BIT_GPIO_INPUT_EN);  // GPIO-E[2],[3] are input

    // GPIO3.3-Input is necessary for wake-up
    REG_APB5_GPIOD_DATA         |= (0x08 << BIT_GPIO_INPUT_EN);
    REG_APB5_GPIO_WUATOD_TYPE   &= (~(0x00 << BIT_GPIOD_WU_TYPE));
    REG_APB5_GPIO_WUATOD_ENABLE |= (0x08 << BIT_GPIOD_WU_TYPE);
    REG_AHB0_ICU_DEEP_SLEEP0    |= (0x08 << BIT_GPIOD_WAKE_EN);

    // Write 0x3431 to Deep Sleep Word Register to enable deep-sleep
    REG_AHB0_ICU_DEEP_SLEEP1 &= 0x0000FFFF  ;
    REG_AHB0_ICU_DEEP_SLEEP1 |= (0x3431 << BIT_DEEP_SLEEP_WORD) ;
}
