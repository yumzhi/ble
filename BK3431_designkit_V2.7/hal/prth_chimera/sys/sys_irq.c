/******************************************************************************
 *
 * MODULE NAME:    sys_irq.c
 * PROJECT CODE:   BlueStream
 * DESCRIPTION:    Hardware Interrupt Interface - Chimera
 * MAINTAINER:     Ivan Griffin/Cyril Jean
 * DATE:           09 August 1999
 *
 * SOURCE CONTROL: $Id: sys_irq.c,v 1.102 2009/04/28 16:08:51 morrisc Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2004 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    09 Aug 1999 -   IG       -  Initial Version
 *    Oct 2000    -   CJ       -  Port to Chimera
 *
 ******************************************************************************/
#include "sys_config.h"
#include "sys_types.h"
#include "sys_features.h"
#include "sys_irq.h"
#include "lc_types.h"
#include "lslc_irq.h"
#include "hw_jalapeno.h"
#include "hw_register.h"
#include "hw_macro_defs.h"
#include "sys_mmi.h"
#include "tra_hcit.h"
#include "BKAPI.h"// merge from target.c
#include "debug.h"
#ifndef __USE_INLINES__
#include "sys_irq_chimera_impl.h"
#endif
#include "hw_lc_impl.h"
#include "hw_radio_defs.h"
#include "le_connection.h"
#include "le_config.h"
#include "le_advertise.h"
#include "sys_power.h"
#include "le_gap_const.h"
#include "BKUart.h"

/******************************************************************************
 *
 * Module local prototypes
 *
 ******************************************************************************/
//static unsigned _SYSirq_Install_Branch_Handler(u_int32 routine,
//    u_int32 *vector);//charles merge from BK3515

/******************************************************************************
 *
 * Constants, #defines, etc.
 *
 ******************************************************************************/
#define IRQ_VECTOR (unsigned *)   0x18
#define FIQ_VECTOR (unsigned *)   0x1c
#define MASK_24_BIT         0xff000000
#define BRANCH_OP_CODE      0xea000000

#define PIPE_OFFSET               0x08
#define WORD_OFFSET               0x02

/******************************************************************************
 *
 * Module variables
 *
 ******************************************************************************/
#if (PRH_BS_DEV_USE_DELAYED_SERVICE_ROUTINES == 1)
#define DSRS_TABLE_SIZE        10
static volatile u_int32 pending_dsr_counter;
static volatile u_int32 dsr_lock;
static volatile u_int32 dsr_table_head;
static volatile u_int32 dsr_table_tail;

static void (*pending_dsr_table[DSRS_TABLE_SIZE])(void);
#endif
void SYSpwr_Exit_Halt_System(void);
//u_int32 Get_sleep_start_current_clk(void);
u_int16 Get_LE_adv_interval(void);
void Clr_need_to_sleep(void);
extern  t_LE_Config LE_config;
extern u_int16 current_connectInterval;
extern u_int8 current_connect_latency;
extern volatile u_int32 flag_sleep_with_32k;
extern volatile u_int32 flag_aux_intr_done;
extern u_int32 bak_ceva_intr;
/******************************************************************************
 *
 * FUNCTION:  SYSirq_IRQ_Handler
 * PURPOSE:   Services slow interrupt requests to micro-controller
 *
 ******************************************************************************/
__IRQ__ void SYSirq_IRQ_Handler(void)//charles merge from BK3515
{
    unsigned int IntStat,Status;
    u_int32  gpio_int_status ; 

    Status  = 0;
    IntStat = REG_ICU_INT_FLAG;
    if(IntStat & INT_STATUS_UART1_bit)          //Uart1 Int Assert
    {
        Status |= INT_STATUS_UART1_bit;

        //REG_UART1_DATA = 0x34;
        //TRAhcit_Call_Interrupt_Handler();  //merge from BK3515
        Beken_UART_Interrupt_Handler();
    }
    //
    if(IntStat & INT_STATUS_GPIO_bit)           //GPIO Int Assert
    {
        Status |= INT_STATUS_GPIO_bit;
        gpio_int_status = REG_APB5_GPIO_WUATOD_STAT ; 
        if(gpio_int_status!=0)
        {
            REG_APB5_GPIO_WUATOD_STAT = gpio_int_status ; 
        }
        else
        {
            gpio_int_status = REG_APB5_GPIO_WUE_STAT ; 
            REG_APB5_GPIO_WUE_STAT = gpio_int_status ; 
        }
        //REG_ICU_INT_ENABLE &= ~INT_STATUS_GPIO_bit;
        //REG_AHB0_ICU_DEEP_SLEEP0    &= ~(0x08 << BIT_GPIOD_WAKE_EN);
        //REG_APB5_GPIO_WUATOD_ENABLE &= ~(0x08 << BIT_GPIOD_WU_TYPE);
        //Status = IntStat;//DUMMY code
        
        Beken_GPIO_IntService();
    }

    if(IntStat & INT_STATUS_ADC_bit)            //ADC Int Assert
    {
#if (ADC_ON == 1)
        BKADCIntService();
#endif
        Status |= INT_STATUS_ADC_bit;//DUMMY code
    }

    if(IntStat & INT_STATUS_I2C1_bit)           //I2c1 Int Assert
    {
        //         BKI2cIntService(0);
        Status |= INT_STATUS_I2C1_bit;
        Status = IntStat;//DUMMY code
    }

    if(IntStat & INT_STATUS_SPI1_bit)           //Spi1 Int Assert
    {
        Status |= INT_STATUS_SPI1_bit;
        //BKSpiIntService(0);
        Status = IntStat;//DUMMY code
    }
    //
    if(IntStat & INT_STATUS_PWM1_bit)           //PWM1 Int Assert
    {
        //         BKPWMIntService(1);
        Status |= INT_STATUS_PWM1_bit;
        Status = IntStat;//DUMMY code
    }

    if(IntStat & INT_STATUS_PWM2_bit)           //PWM2 Int Assert
    {
        Status |= INT_STATUS_PWM2_bit;
        //         BKPWMIntService(2);
        Status = IntStat;//DUMMY code
    }

    if(IntStat & INT_STATUS_PWM3_bit)           //PWM3 Int Assert
    {
        Status |= INT_STATUS_PWM3_bit;
        //         BKPWMIntService(3);
        Status = IntStat;//DUMMY code
    }

    if(IntStat & INT_STATUS_PWM4_bit)           //PWM4 Int Assert
    {
        Status |= INT_STATUS_PWM4_bit;
        //         BKPWMIntService(4);
        Status = IntStat;//DUMMY code
    }

    REG_ICU_INT_FLAG = Status;
}

/*static */ t_SYSirq_Counters _SYSirq_Counters;

#if (PRH_BS_DEV_USE_DELAYED_SERVICE_ROUTINES == 1)
/******************************************************************************
 *
 * FUNCTION:  SYSirq_Enable_Tabasco_interrupt
 * PURPOSE:   Enable the Tabasco interrupt. IRQ in this case
 *
 ******************************************************************************/
static void SYSirq_Enable_Tabasco_interrupt(void)
{
#ifndef __GNUC__
    u_int32 current_flags;
    __asm
    {
        MRS current_flags, CPSR                 /* Get current CPSR */
        AND current_flags, current_flags, #0xffffff7f /* Enable IRQ */
        MSR CPSR_c, current_flags                 /* Write back */
    }
#else
    /* GNU C inline asm? */
#endif
}

/******************************************************************************
 *
 * FUNCTION:  SYSirq_Disable_Tabasco_Interrupt
 * PURPOSE:    Disable the Tabasco interrupt. IRQ in this case
 *
 ******************************************************************************/
static void SYSirq_Disable_Tabasco_Interrupt(void)
{
#ifndef __GNUC__
    u_int32 current_flags;
    __asm
    {
        MRS current_flags, CPSR                 /* Get current CPSR */
        ORR current_flags, current_flags, #0x80 /* Disable IRQ */
        MSR CPSR_c, current_flags                 /* Write back */
    }
#else
    /* GNU C inline asm? */
#endif
}

/******************************************************************************
 *
 * FUNCTION:  SYSirq_IRQ_Tabasco_ISR
 * PURPOSE:   Tabasco interrupt service routine
 *
 ******************************************************************************/
void SYSirq_IRQ_Tabasco_ISR(void)
{
    t_dsr_pointer delayed_service_routine;

    delayed_service_routine = LSLCirq_Tabasco_ISR();

    if (0 != delayed_service_routine)
    {
        if (pending_dsr_counter == 0)
        {
            dsr_lock = 0;
        }

        /* add the dsr associated with this interrupt to the dsr queue. */
        pending_dsr_table[dsr_table_tail++] = delayed_service_routine;
        if( dsr_table_tail >= DSRS_TABLE_SIZE )
            dsr_table_tail = 0;

        pending_dsr_counter++;

        if (dsr_lock == 0)
        {
            dsr_lock++;

            while(pending_dsr_counter != 0)
            {
                SYSirq_Enable_Tabasco_interrupt();
                /* Execute the dsr with interrupts enabled. */
                pending_dsr_table[dsr_table_head]();

                SYSirq_Disable_Tabasco_Interrupt();

                dsr_table_head++;
                if( dsr_table_head >= DSRS_TABLE_SIZE )
                    dsr_table_head = 0;

                pending_dsr_counter--;
            }
        }
    }
}

#else   /* (PRH_BS_DEV_USE_DELAYED_SERVICE_ROUTINES == 1) */
/* stub for proper compilation */
void SYSirq_IRQ_Tabasco_ISR(void)
{
}

/******************************************************************************
 *
 * FUNCTION:  SYSirq_FIQ_Handler
 * PURPOSE:   Services fast Interrupt Requests to micro-controller
 *
 ******************************************************************************/
__IRQ__ void SYSirq_FIQ_Handler(void)
{
    unsigned int IntStat,Status1;
    unsigned int  intr_status;
#if (PRH_BS_CFG_SYS_LOW_POWER_MODE_SUPPORTED==1)
    intr_status = REG_AHB1_LC_INTR_STATUS ;  // Decide which INTR source

    // ZGF@20140428 added as the Auxiliary Timer to wake-up 16MHz clock
    if(intr_status & 0x00000020) // LC Reg 0x14[5] - AUX_INTR
    {
        if(flag_sleep_with_32k==1)
        {
					  
            
            REG_AHB0_ICU_DIGITAL_PWD  = SET_CLK48M_PWD ;
            REG_AHB0_ICU_CLKSRCSEL    = 0x1;
					  HW_set_use_lf(0);
            REG_AHB0_ICU_CORECLKCON   = 0x0 ; // CPU is power-up and cpu-clock is not divided.
            REG_AHB0_ICU_ANALOG0_PWD = 0xFFFFFFE0; //HP-LDO and LP-LDO switch power-down for 2013Nov tape-out version
            REG_AHB0_ICU_ANALOG_MODE = 0x0;
            //HW_set_use_lf(0);
            flag_sleep_with_32k=0;
        } //else
        
        HW_set_aux_timer(0);  // AUX Counter RGF must be clear befor AUX_INTR clear.
        HW_set_aux_tim_intr_clr(1);
        flag_aux_intr_done=0x1;
//warning : do not remove this code start,find me create connection fail 
#if (BB_TEST_PIN_EN == 1)
        REG_APB5_GPIOC_CFG        = 0x0000FF00;           //GPIO-C as Test Pin, Not Output
        REG_APB5_GPIOC_DATA       = 0x00000000;           //GPIO-C Not Input
        #ifdef ANA_TEST_ON
        REG_AHB0_ICU_DIGITAL_PWD |= SET_ANA_TEST_PIN;
        REG_APB5_GPIOB_CFG        = 0x0000FF00;           //GPIO-C as Test Pin, Not Output
        REG_APB5_GPIOB_DATA       = 0x00000000;           //GPIO-C Not Input
        #else
        REG_AHB0_ICU_DIGITAL_PWD |= SET_BB_TEST_PIN;
        #endif
#else
        //REG_APB5_GPIOC_CFG        = 0x0000FFFF;           //FOR lost connection issue do not remove
        //REG_APB5_GPIOC_DATA       = 0x00000000;           //GPIO-C Not Input
        //REG_AHB0_ICU_DIGITAL_PWD &= ~SET_BB_TEST_PIN;
#endif
///warning : do not remove this code end
        SYSpwr_Exit_Halt_System();
        LE_config.device_sleep = 0;
        if (LEadv_Get_Advertising_Enable()
		   | (current_connectInterval == 24) /**< 24 * 1.25ms = 30ms */
           | (current_connectInterval == 240)) /**< 240 * 1.25ms = 300ms */
        {
            Clr_need_to_sleep();
            LE_config.state = STANDBY_STATE;
        }
        else if(LE_config.state == SLEEP_STATE)
		{
            LE_config.state = WAKEUP_STATE;
        }
        REG_CEVA_REG_4 = bak_ceva_intr; /**< Restore CEVA interrupt mask config */
    }
#endif

    Status1 = 0 ;
    IntStat = REG_ICU_INT_FLAG;

    if(IntStat & INT_STATUS_BLE_bit)          //BLE XVR Int Assert
    {
        Status1 |= INT_STATUS_BLE_bit;
        LSLCirq_IRQ_Handler(); //Replace TestCEVA_IntService
    }

    if(IntStat & INT_STATUS_LBD_bit)          //LBD Int Assert
    {
        Status1 |=  INT_STATUS_LBD_bit;
        //ADD ...
    }

    REG_ICU_INT_FLAG = Status1;

    return;
}
#endif  /* (PRH_BS_DEV_USE_DELAYED_SERVICE_ROUTINES == 1) */

/******************************************************************************
 *
 * FUNCTION:  SYSirq_Disable_Interrupts_Save_Flags
 * PURPOSE:   Disables ARM IRQ and FIQ Interrupts, saves previous
 *            PSR
 *
 ******************************************************************************/
void SYSirq_Disable_Interrupts_Save_Flags(u_int32 *flags)
{
#ifndef __GNUC__
    u_int32 current_flags,my_tmp_flags;
    __asm
    {
        MRS current_flags, CPSR                 /* Get current CPSR */
        AND my_tmp_flags, current_flags, #0xc0  /* Clear all ex. IRQ and FIQ */
        STR my_tmp_flags, [flags]               /* "save" flags */
#if (PRH_BS_DEV_UART_FIQ_ALWAYS_ENABLED==1)
        ORR current_flags, current_flags, #0x80 /* Disable IRQ Only */
#else
        ORR current_flags, current_flags, #0xc0 /* Disable IRQ and FIQ */
#endif
        MSR CPSR_c, current_flags                 /* Write back */
    }
#else
    /* GNU C inline asm? */
#endif
}

/******************************************************************************
 *
 * FUNCTION:  SYSirq_Enable_Interrupts_Save_Flags
 * PURPOSE:   Enables ARM IRQ and FIQ Interrupts, saves previous
 *            PSR
 *
 ******************************************************************************/
void SYSirq_Enable_Interrupts_Save_Flags(u_int32 *flags)
{
#ifndef __GNUC__
    u_int32 current_flags,my_tmp_flags;
    __asm
    {
        MRS current_flags, CPSR                 /* Get current CPSR */
        AND my_tmp_flags, current_flags, #0xc0  /* Clear all ex. IRQ and FIQ */
        STR my_tmp_flags, [flags]               /* "save" flags */
        AND current_flags, current_flags, #0x3f /* Enable IRQ and FIQ */
        MSR CPSR_c, current_flags                 /* Write back */
    }
#else
    /* GNU C inline asm? */
#endif
}

/******************************************************************************
 *
 * FUNCTION:  SYSirq_Interrupts_Restore_Flags
 * PURPOSE:   Restores previously saved previous PSR
 *
 ******************************************************************************/
void SYSirq_Interrupts_Restore_Flags(u_int32 flags)
{
#ifndef __GNUC__
    u_int32 current_flags;
    __asm
    {
        MRS current_flags, CPSR                 /* Get current CPSR */
        BIC current_flags, current_flags, #0xc0 /* Clear IRQ and FIQ bits */
        ORR current_flags, current_flags, flags /* OR in saved copy */
        MSR CPSR_c, current_flags                 /* Write back */
    }
#else
    /* GNU C inline asm? */
#endif
}


/******************************************************************************
 *
 * FUNCTION:  _SYSirq_Install_Branch_Handler
 * PURPOSE:   Installs an exception handler using the ARM Branch and Link
 *            instruction
 *
 ******************************************************************************/
//charles merge from BK3515
//static unsigned _SYSirq_Install_Branch_Handler(u_int32 routine,
//    u_int32 *vector)
//{
//    u_int32 new_vector_value = 0,/* initialise locals */
//        old_vector_value = 0;

//    new_vector_value = ((routine - (u_int32)vector - PIPE_OFFSET)
//                         >> WORD_OFFSET);

//    /* Ensure that result is 24bit only */
//    new_vector_value = BRANCH_OP_CODE | new_vector_value;
//    old_vector_value = *vector;

//    *vector =new_vector_value;
//    return (old_vector_value);
//}

/******************************************************************************
 *
 * FUNCTION:  SYSirq_Initialise
 * PURPOSE:   Initialise Interrupt Requests
 *
 ******************************************************************************/
void SYSirq_Initialise(void)
{
    /*
     * Intialise interrupts
     */
#if (PRH_BS_DEV_USE_DELAYED_SERVICE_ROUTINES == 1)
    dsr_table_head = 0;
    dsr_table_tail = 0;

    pending_dsr_counter = 0;
    dsr_lock = 0;
#endif
    /* SYSirq_IRQ_Disable_All(); - now done in BT_Init */

#ifndef BUILD_FOR_ROM
    // SYSirq_IRQ_Stack_Setup();
    // SYSirq_FIQ_Stack_Setup();
    // SYSirq_SYS_Stack_Setup();
#endif

    /*
     * Install Service Routines
     */
    _SYSirq_Setup_IRQ_FIQ_Enable_Masks();

#if (PRH_BS_DEV_USE_DELAYED_SERVICE_ROUTINES == 1)//charles merge from BK3515
    //    _SYSirq_Install_Branch_Handler((u_int32)SYSirq_IRQ_wrapper,
    //        IRQ_VECTOR);
#else
    //    _SYSirq_Install_Branch_Handler((u_int32)SYSirq_IRQ_Handler,
    //        IRQ_VECTOR);
#endif

    //    _SYSirq_Install_Branch_Handler((u_int32)SYSirq_FIQ_Handler, FIQ_VECTOR);

    /* DO NOT RE-ENABLE INTERRUPTS HERE!!! */
}


/******************************************************************************
 *
 * FUNCTION:  SYSirq_Disable_Baseband_ISR_Save_Flags
 * PURPOSE:   Disables ARM IRQ Baseband_ISR, saves previous
 *            PSR
 *
 ******************************************************************************/
void SYSirq_Disable_Baseband_ISR_Save_Flags(u_int32 *flags)
{
#ifndef __GNUC__
    u_int32 current_flags,my_tmp_flags;
    __asm
    {
        MRS current_flags, CPSR                 /* Get current CPSR */
        AND my_tmp_flags, current_flags, #0xC0  /* Clear all ex. IRQ and FIQ */
        STR my_tmp_flags, [flags]               /* "save" flags */
        ORR current_flags, current_flags, #0x80 /* Disable IRQ and FIQ */
        MSR CPSR_c, current_flags                 /* Write back */
    }
#else
    /* GNU C inline asm? */
#endif
}

/******************************************************************************
 *
 * FUNCTION:  SYSirq_Enable_Baseband_ISR_Save_Flags
 * PURPOSE:   Enables ARM IRQ Baseband_ISR, saves previous
 *            PSR
 *
 ******************************************************************************/
void SYSirq_Enable_Baseband_ISR_Save_Flags(u_int32 *flags)
{
#ifndef __GNUC__
    u_int32 current_flags,my_tmp_flags;
    __asm
    {
        MRS current_flags, CPSR                 /* Get current CPSR */
        AND my_tmp_flags, current_flags, #0xC0  /* Clear all ex. IRQ and FIQ */
        STR my_tmp_flags, [flags]               /* "save" flags */
        AND current_flags, current_flags, #0x7f /* Enable IRQ and FIQ */
        MSR CPSR_c, current_flags                 /* Write back */
    }
#else
    /* GNU C inline asm? */
#endif
}

/******************************************************************************
 *
 * FUNCTION:  SYSirq_Baseband_ISR_Restore_Flags
 * PURPOSE:   Restores previously saved previous PSR
 *
 ******************************************************************************/
void SYSirq_Baseband_ISR_Restore_Flags(u_int32 flags)
{
#ifndef __GNUC__
    u_int32 current_flags;
    __asm
    {
        MRS current_flags, CPSR                 /* Get current CPSR */
        BIC current_flags, current_flags, #0x80 /* Clear IRQ and FIQ bits */
        ORR current_flags, current_flags, flags /* OR in saved copy */
        MSR CPSR_c, current_flags                 /* Write back */
    }
#else
    /* GNU C inline asm? */
#endif
}
