#ifndef _CEVA_LE_CONST_
#define _CEVA_LE_CONST_

/**********************************************************************
 * MODULE NAME:    le_const.h
 * PROJECT CODE:    BlueStream Low Energy
 * DESCRIPTION:    LE HC Consts
 * MAINTAINER:     Gary Fleming
 * DATE:           23 JUN 99
 *
 *
 * LICENSE:
 *     This source code is copyright (c) 1999-2004 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *
 * SOURCE CONTROL: $Id: le_const.h,v 1.1 2012/05/09 00:10:06 garyf Exp $
 *    
 **********************************************************************/



#define PUBLIC_ADDRESS 0x00
#define RANDOM_ADDRESS 0x01

#define ADVERTISING_ACCESS_ADDRESS  0x8E89BED6

#define LE_DEFAULT_MIN_ADVERTISING_INTERVAL 0x0800
#define LE_DEFAULT_MAX_ADVERTISING_INTERVAL 0x0800
#define LE_MAX_ADVERTISING_EVENT_WIDTH_IN_CLK_TICKS 20
#define PUBLIC_DEVICE_ADDRESS_TYPE 0x00
#define RANDOM_DEVICE_ADDRESS_TYPE 0x01

#define LE_PASSIVE_SCAN 0x00
#define LE_ACTIVE_SCAN  0x01


#define LE_MAX_ADV_CHANNEL_PDU_LEN  0x25  // 37
#define LE_MIN_ADV_CHANNEL_PDU_LEN  0x06  //  6


// Interrupts
#define LE_TIM_0             0x00
#define LE_TIM_2             0x01
#define LE_PKA				 0x02
#define LE_PKD               0x03
#define LE_NO_PKD            0x04
#define LE_SYNC_DET          0x05
#define LE_AES_COMPLETE      0x06
#define LE_RX_HDR            0x07
#define LE_TIM_1             0x08
#define LE_TIM_3             0x09


#define LE_META_EVENT 0x3E

#define _LE_LL_CHECK_ADV_PKT_LENGTH(length)  ((length<=LE_MAX_ADV_CHANNEL_PDU_LEN) && (length >= LE_MIN_ADV_CHANNEL_PDU_LEN))

#define DIRECT_ADVERTISING_TIME_IN_TICKS  (0x10 * 0x0800)//merge CEVA 0517

#define STATIC_RANDOM_ADDRESS          0x01//merge CEVA 0517
#define NON_RESOLVABLE_RANDOM_ADDRESS  0x02//merge CEVA 0517
#define RESOLVABLE_RANDOM_ADDRESS      0x03//merge CEVA 0517

#endif
