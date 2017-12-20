/*****************************************************************************
 * MODULE NAME:    bt_init.c
 * PROJECT CODE:   BlueStream
 * DESCRIPTION:    Routines to initialise Bluetooth stack higher layers.
 * MAINTAINER:     Ivan Griffin
 * CREATION DATE:  06 August 1999
 *
 * SOURCE CONTROL: $Id: bt_init.c,v 1.3 2012/05/15 16:06:19 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2004 Ceva Inc.
 *     All rights reserved.
 *
 *****************************************************************************/

#include "sys_config.h"
#include "sys_irq.h"
#include "sys_rand_num_gen.h"
#include "sys_mmap.h"
#include "sys_mmi.h"
#include "sys_power.h"

#include "tra_hcit.h"
#include "tra_queue.h"

#include "bt_init.h"

#include "lc_interface.h"

#include "hw_lc.h"
#include "hw_radio.h"
#include "hw_delay.h"


#include "hc_flow_control.h"
#include "hc_event_gen.h"

#include "lslc_irq.h"

#include "sys_init.h"


/*****************************************************************************
 * BT_Initialise
 *
 * Initialise all layers and major functions in the device.
 *
 ****************************************************************************/
void BT_Initialise(void)
{
    /* 
     * Initialise the BT Modules: Transport Queues, Tester, and Pump (Optional)
     */
    BTq_Initialise();
 
    /*
     * Map Hardware into Memory
     */
    //SYSmmap_Map_BlueStream();//dummy function
    
    /*
     * Initialise the Random Number Generator with the default seed
     * and some relatively nondeterminitic hardware readings, using
     * pre-reset values so re-seed on HCI reset isn't determinisitc.
     */
    SYSrand_Seed_Rand(SYSconfig_Get_SYS_Rand_Seed()
                      ^ HW_get_bt_clk()
                      ^ HW_get_native_bit_count()
                      ^ HW_get_uap_lap());

    /* 
     * Initialise the LC Hardware 
     */
    HWlc_Initialise();
 
    /*
     * Reset the radio.
     */
    HWradio_Reset();
    

    /*
     * For some systems there is additional hardware setup required
     * between radios, therefore sequence here is critical
     * E.g. Clock divider init etc required for radio.
     *      must be after HWradioReset and before HWradio_Initialise.
     */
    SYSinit_Initialise();

    /* 
     * Initialise the RF/DSP Hardware 
     */

    HWradio_Initialise();
    /*
     * Initialise the self-calibrating HW Delay function
     */
    HWdelay_Initialise();//calibration HWdelay parameter 
    
    /* 
     * Initialise the HCI transport subsystem
     */
    TRAhcit_Initialise();

    /*
     * Initialise the IRQ Handlers
     */
    SYSirq_Initialise();
#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==0)//merge CEVA 0517
    HCeg_Initialise();
#endif
#if (PRH_BS_CFG_SYS_LE_CENTRAL_DEVICE==1) || (PRH_BS_CFG_SYS_LE_PERIPHERAL_DEVICE==1)
    HCfc_Initialise();
#endif

    /* 
     * Initialise the low power mode sub-system if available
     */
#if (BUILD_TYPE!=UNIT_TEST_BUILD)
    SYSpwr_Initialise();
#endif

  //  LEconfig_Init();
    /*
     * Initialise the Link Control Modules (must be last)
     * - Last task in LC_initialise is interrupt initialisation
     * - sys_main will have enabled the IRQs
     */
    LC_Initialise();
}

/*****************************************************************************
 * BT_Initialise_Ex
 *
 * Initialise all layers and major functions in the device with an 
 * invoked callback function on completion of initialisation
 *
 * Useful for combined stacks
 *
 * callback         function of type t_SYS_Fn_Ptr to be called back
 ****************************************************************************/
void BT_Initialise_Ex(t_SYS_Fn_Ptr callback)
{
    BT_Initialise();

    if (0 != callback)
    {
        (*callback)();
    }
}


