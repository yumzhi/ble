#ifndef __PARTHUS_LC_INTERFACE
#define __PARTHUS_LC_INTERFACE

/*************************************************************************
 * MODULE NAME:    lc_interface.h
 * PROJECT CODE:    BlueStream
 * DESCRIPTION:    Baseband Link Controller Interface Module
 * MAINTAINER:     Gary Fleming, John Nelson
 * CREATION DATE:  2 February 2000
 *
 * SOURCE CONTROL: $Id: lc_interface.h,v 1.2 2012/05/09 01:17:22 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 1999-2004 Ceva Inc.
 *     All rights reserved.
 *
 *************************************************************************/

#include "sys_config.h"
#include "sys_types.h"
#include "lc_types.h"
//#include "dl_dev.h"
#include "hc_event_gen.h"

/*
 * For LC_Get_Piconet_Clock(), LC_Get_Native_Clock()
 */
#include "hw_lc.h"
#include "hw_le_lc.h"

//typedef t_devicelink* (*t_callback)(void);

/*
 * Initialisation of LC and of a previously allocated device link
 */
void LC_Initialise(void);


/************************************************************************
 * LC_Get_Radio_Setup_Time
 *
 * Return setup time for radio, currently defaults to maximum
 ************************************************************************/
#define LC_Get_Radio_Setup_Time_Us()                220


/************************************************************************
 * LC_Get_Max_Sleep_Time_Slots
 *
 * Return maximum sleep time for presented device_index in Slots
 ************************************************************************/
u_int16 LC_Get_Max_Sleep_Time_Slots(
    t_deviceIndex device_index    /* i - index of device link    */);

/*************************************************************************
 * LC_Sleep
 *
 * Put LC to sleep. 
 *************************************************************************/
t_error LC_Sleep(t_slots slots);

/*************************************************************************
 * LC_Wakeup
 *
 * Wakeup the LC.
 * timeout - specifies a timeout value - should be zero for Return from Hold,
 * and a number of slots for Sniff etc.
 * instance - specifices the precice clock instance the wakup should occur.
 *************************************************************************/
t_error LC_Wakeup(t_deviceIndex device_index, t_slots timeout, t_clock instance);

/*
 * Provide Clock Access to Higher levels
 */

#define LC_Get_Native_Clock()    ( (t_clock) HW_Get_Native_Clk_Avoid_Race() )
#define LC_Get_Piconet_Clock(device_index) ( (t_clock) LC_Get_Piconet_Clock_Ex((device_index),FALSE) )

#if (PRH_BS_CFG_SYS_LMP_RSSI_SUPPORTED==1)
/************************************************************************
 * LC_Get_RSSI
 *
 * Get the most recent RSSI average on a link
 * maps directly to s_int8 DL_Get_RSSI(t_devicelink* p_device_link);
 ************************************************************************/
#define LC_Get_RSSI(device_index) \
    DL_Get_RSSI(DL_Get_Device_Ref(device_index))
#endif  /*(PRH_BS_CFG_SYS_LMP_RSSI_SUPPORTED==1)*/

#if (PRH_BS_CFG_SYS_LOW_POWER_MODE_SUPPORTED==1)
#define LC_Is_Sleep_Pending() (USLCchac_Get_Active_Procedure() = SLEEP_PROCEDURE)

/*
 * when exiting power down the USE_LF is set to 0. However, due to the latency
 * involved in Tabasco, the clearing of the SLEEP beet is delayed.
 * Need to thus check both SLEEP and USE_LF to see if asleep
 */
#define LC_Is_Tabasco_Asleep USLCsleep_Is_Tabasco_Asleep
#endif


boolean LC_Is_Sleep_Possible(t_clock *next_instant);


#endif
