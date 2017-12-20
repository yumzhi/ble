#ifndef _PARTHUS_LMP_TYPES_
#define _PARTHUS_LMP_TYPES_

/******************************************************************************
 * MODULE NAME:    lmp_types.h
 * PROJECT CODE:    BlueStream
 * DESCRIPTION:   
 * MAINTAINER:     Gary Fleming
 * CREATION DATE:        
 *
 * SOURCE CONTROL: $Id: lmp_types.h,v 1.1 2012/05/09 00:10:10 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2004 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *
 ******************************************************************************/

#include "sys_config.h"
#include "sys_types.h"


typedef enum { OFF = 0x00, ON = 0x01 } t_flag;

typedef struct t_lm_event_info {
    t_error status;
    t_linkType link_type;
    u_int8 number;
    u_int8  mode;
    u_int8  role;
    t_connectionHandle handle;
    t_bd_addr* p_bd_addr;
    t_error reason;
    u_int8* p_u_int8;
    u_int16 value16bit;
    u_int8 lmp_version;
    u_int16 lmp_subversion;
    u_int16 comp_id;
    u_int8 ack_required;
} t_lm_event_info;

typedef t_error (*t_lm_callback)(u_int8, t_lm_event_info*);


/*
 * t_lmp_pdu_info
 *      Holds all the parameters for LM_Encode_LMP_PDU() to encode the LMP pdu
 */

typedef enum {
    MAX_POWER,
    MIN_POWER,
    INTERMEDIATE_POWER
 } t_power_level;

typedef enum {
    MIN=0, /* used to signify DM1-only in EDR mode */
    MEDIUM=1, /* with FEC for BR or 2MBPS in EDR mode */
    HIGH=2, /* without FEC for BR or 3MBPS in EDR mode */
    AUTO=3 /* auto rate on all packets in active ptt */
 } t_rate;


typedef enum {
     REJECT = 0,
     DONT_AUTO_ACCEPT = 1,
     AUTO_ACCEPT = 2,
     AUTO_ACCEPT_WITH_MSS = 3
     
} t_filter_settings;


typedef enum
{
    LMP_No_Features_Complete = 0,
    LMP_Remote_Features_Complete = 1,
    LMP_Local_Features_Complete = 2,
    LMP_Both_Features_Complete = 3
} t_LMP_Features_Bitmask;


#endif

