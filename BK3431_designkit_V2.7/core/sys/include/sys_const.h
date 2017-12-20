#ifndef _PARTHUS_SYS_CONST_
#define _PARTHUS_SYS_CONST_

/******************************************************************************
 * MODULE NAME:    sys_const.h
 * PROJECT CODE:   BlueStream
 * DESCRIPTION:    All System Wide Constants
 * MAINTAINER:     John Nelson
 * DATE:           15 May 1999
 *
 * SOURCE CONTROL: $Id: sys_const.h,v 1.2 2012/05/09 01:17:25 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 1999-2004 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    15 May 1999 -   jn       - Initial Version V0.9
 *    21 July 1999 -  cm       - Update
 *    25 July 1999 -  gf       - Update
 *
 * NOTES:
 *    Little endian format used, i.e. least significant byte in lowest address    
 ******************************************************************************/

#define MIN_L2CAP_MTU            (27+4)
#define DEFAULT_L2CAP_MTU        (672+4)

/* 
 *V1.1 P21 Transmitter Characteristics - Power Control
 *
 * Power control is required for power class 1 equipment. The power control is
 * used for limiting the transmitted power over 0 dBm. Power control capability
 * under 0 dBm is optional and could be used for optimizing the power consump-tion
 * and overall interference level. The power steps shall form a monotonic
 * sequence, with a maximum step size of 8 dB and a minimum step size of 2 dB.
 * A class 1 equipment with a maximum transmit power of +20 dBm must be able
 * to control its transmit power down to 4 dBm or less.
 * 
 *                                             with min step size 2dB 
 * Class 1 Pmax 20dBm   Pmin  0dBm..~-30dBm    ==> 25 levels
 * Class 2 Pmax  4dBm   Pmin -6dBm..~-30dBm    ==> 17 levels
 * Class 3 Pmax  0dBm   Pmin        ~-30dBm    ==> 15 levels
 */


#if !defined(TERASIC) && !defined(SHOGA) && !defined(CHIMERA) && !defined (LINUX) && !defined (WIN32)
#define MAX_POWER_LEVEL_UNITS 15
#define TX_POWER_LEVEL_Pmax 0
#define TX_POWER_LEVEL_Pmin -30

#define GOLDEN_RECEIVER_RSSI_MIN -56
#define GOLDEN_RECEIVER_RSSI_MAX -30
#endif

#define MAX_PEER_POWER_LEVEL_UNITS 15

/*
 * Adding HW Error Events
 */
#define PRH_BS_HW_ERROR_NONE                        0x00
#define PRH_BS_HW_ERROR_HCIT_UART_OVERRUN           0x01
#define PRH_BS_HW_ERROR_HCIT_RX_PDU_MALFORMED       0x02
#define PRH_BS_HW_ERROR_INVALID_DEVICE_LINK         0x03
#define PRH_BS_HW_ERROR_MSS_FAILURE                 0x04
#define PRH_BS_HW_ERROR_DEBUG_CLKN_READ             0x05
#define PRH_BS_HW_ERROR_STACK_CORRUPTION            0x06
#define PRH_BS_HW_ERROR_HCIT_UART_FRAMING           0x07
#define PRH_BS_HW_ERROR_INVALID_LMP_LINK            0x08
#define PRH_BS_HW_ERROR_INVALID_PICONET_INDEX       0x09
#define PRH_BS_HW_ERROR_INCORRECT_RADIO_VERSION     0x0A
#define PRH_BS_HW_ERROR_SCHEDULER_CONFIGURATION     0x10
#define PRH_BS_HW_ERROR_INCORRECT_TABASCO_VERSION   0x11

#define PRH_BS_HW_ERROR_INCORRECT_CTRLSTATE         0x20
#define PRH_BS_HW_ERROR_OUT_OF_LMP_TIMERS           0x21
#define PRH_BS_HW_ERROR_INCORRECT_SUPER_STATE       0x22
#define PRH_BS_HW_ERROR_CORRUPTION_OF_LMP_TIMERS    0x23
#define PRH_BS_HW_ERROR_CORRUPTION_OF_QUEUES        0x24
#define PRH_BS_HW_ERROR_OUT_OF_LMP_QUEUE_BUFFERS    0x25
#define PRH_BS_HW_ERROR_LMP_QUEUE_CORRUPTED         0x26
#define PRH_BS_HW_ERROR_OUT_OF_LE_LLC_QUEUE_BUFFERS 0x27
#define PRH_BS_HW_ERROR_LE_LLC_QUEUE_CORRUPTED      0x28
/*
 *  System hardware radio type definitions
 */
#define HWradio_NSC_LMX3162            1
#define HWradio_ALPS_UGSA4_REV_C       2
#define HWradio_ALPS_UGSA4_REV_D       3
#define HWradio_SIWAVE_SIW1502         4
#define HWradio_CONEXANT_CX72303       5
#define HWradio_HD157100               6
#define HWradio_HD157101               7
#define HWradio_HD157102               8
#define HWradio_BU9469EKN              9
#define HWradio_BU9468MUV              10
#define HWradio_BK3431                 11
 
/*
 *  Tabasco hardware revision numbers
 */

#define PRH_BS_HW_TABASCO_VERSION_T1_0_1        5
#define PRH_BS_HW_TABASCO_VERSION_T2_0_0        6
 

#define GAP_PROTOCOL  0x10//merge CEVA 0517
#define SMP_PROTOCOL  0x11//merge CEVA 0517
#define HCI_PROTOCOL  0x00//merge CEVA 0517

#endif   

