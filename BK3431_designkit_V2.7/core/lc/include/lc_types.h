#ifndef _PARTHUS_LC_TYPES_
#define _PARTHUS_LC_TYPES_

/*************************************************************************
 * MODULE NAME:    lc_types.h
 * PROJECT CODE:   BlueStream
 * DESCRIPTION:    Baseband Link Controller Interface Module
 * MAINTAINER:     Conor Morris
 *
 * SOURCE CONTROL: $Id: lc_types.h,v 1.1 2012/05/09 00:09:58 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 1999-2004 Ceva Inc.
 *     All rights reserved.
 *
 *************************************************************************/

#include "tra_queue.h"

/*
 * Used in the Chan_ctrl procedures
 */
#define NO_LOOK_AHEAD        0
#define LOOK_AHEAD           1
#define NFA_CHAN_MASTER      2

/*
 * t_USLC_Frame_Activity                      Frame activity types.
 *
 * Each type a separate bit for rapid set comparison.
 * (Note an enumerated type due to compiler bug on early target!)
 */




typedef enum
{
    RXm_RECEIVER_DISABLED=0,
    RXm_SINGLE_WIN=1,
    RXm_DOUBLE_WIN=2,
    RXm_FULL_WIN=3
} t_Receiver_Mode;

typedef enum
{
    TXm_TRANSMITTER_DISABLED = 0,
    TXm_NORMAL=1,
    TXm_SLAVE_1ST_HALF__MASTER_BOTH=2,
    TXm_2ND_HALF_SLOT=3
} t_Transmitter_Mode;


typedef struct devlink
{
    t_deviceIndex device_index;         /* Fast access to index via ptr */

    /*
     * ACL related transmit descriptor
     */
    t_q_descr *tx_descriptor;
    u_int8 packet_type; 

    u_int8  power_level;                /* Current power level        */
    
    u_int8  clock_jitter_us;            /* Clock jitter in us         */
    u_int8  clock_drift_ppm;            /* Clock drift in ppm         */

} t_devicelink;

#endif
