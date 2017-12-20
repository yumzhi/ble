#ifndef _PARTHUS_SYS_POWER_
#define _PARTHUS_SYS_POWER_

/******************************************************************************
 * MODULE NAME:    sys_power.h
 * PROJECT CODE:   BlueStream
 * DESCRIPTION:    Low Power Mode Clock Control
 * MAINTAINER:     Ivan Griffin
 * DATE:           10 December 2001
 *
 * SOURCE CONTROL: $Id: sys_power.h,v 1.1 2012/05/09 00:10:15 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001-2004 Ceva Inc.
 *     All rights reserved.
 *
 ******************************************************************************/

#if (PRH_BS_CFG_SYS_LOW_POWER_MODE_SUPPORTED==1)

typedef enum {
    SYS_LF_OSC_NONE   = 0,
    SYS_LF_OSC_32K000 = 1,
    SYS_LF_OSC_32K768 = 2
} t_sys_lf_osc_type;

/*
 * Function prototypes
 */
void SYSpwr_Halt_System(void);
void SYSpwr_Sleep_System(void);
void SYSpwr_Exit_Halt_System(void);
void SYSpwr_Force_System_Halt(void);
void SYSpowerDown_now(void);

void SYSpwr_Initialise(void);
void SYSpwr_Handle_Early_Wakeup(void);
boolean SYSpwr_Is_Low_Power_Mode_Active(void);

boolean SYSpwr_Set_LowFrequencyOscillatorAvailable_Value(u_int8 osc_available);
t_sys_lf_osc_type SYSpwr_LowFrequencyOscillatorAvailable(void);
u_int32 SYSpwr_Get_Min_Frames_To_Sleep(void);
void entry_deep_sleep(void);

#define RTC_MODE_32K_16M  1
#define RTC_MODE_EXT_32K  2
#else
/*
 * No sys_power, hence empty functions
 */
#define SYSpwr_Initialise()
#define SYSpwr_Is_Low_Power_Mode_Active()  0
#endif

#endif

