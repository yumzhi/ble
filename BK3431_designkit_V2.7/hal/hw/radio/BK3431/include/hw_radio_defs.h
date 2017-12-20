/*****************************************************************************
 *
 * MODULE NAME:    hw_radio_defs.h
 * PROJECT CODE:   XpertBlue
 * DESCRIPTION:    Rohm Bluetooth Ver.2.0+EDR radio driver for Tabasco.
 * MAINTAINER:     Tom Kerwick
 * CREATION DATE:  07.09.07
 *
 * SOURCE CONTROL: $Id: hw_radio_defs.h,v 1.22 2011/04/01 16:03:26 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2007 Ceva Inc.
 *     All rights reserved.
 *
 * RECENT REVISION HISTORY:
 *
 *  07.09.07 TK    initial reference version, untested.
 *  28.09.07 TK    modified for rohm bu9468muv radio.
 *  01.02.08 TK    modified for independent tx/rx setup times.
 *
 *****************************************************************************/
#ifndef HW_RADIO_DEFS
#define HW_RADIO_DEFS

//#include "hw_hab_defs.h"
#include "hw_register.h"
#include "BK3431_reg.h"

//===========================================================================================
// Analog RF module + Transceiver Registers
//===========================================================================================
//Analog SPI Registers
#define APB_XVER_BASE    0x00801400
#define XVR_REG00               (*((volatile unsigned int *)   (APB_XVER_BASE+0x0)))
#define XVR_REG01               (*((volatile unsigned int *)   (APB_XVER_BASE+0x4)))
#define XVR_REG02               (*((volatile unsigned int *)   (APB_XVER_BASE+0x8)))
#define XVR_REG03               (*((volatile unsigned int *)   (APB_XVER_BASE+0xC)))

#define XVR_REG04               (*((volatile unsigned int *)   (APB_XVER_BASE+0x10)))
#define XVR_REG05               (*((volatile unsigned int *)   (APB_XVER_BASE+0x14)))
#define XVR_REG06               (*((volatile unsigned int *)   (APB_XVER_BASE+0x18)))
#define XVR_REG07               (*((volatile unsigned int *)   (APB_XVER_BASE+0x1C)))

#define XVR_REG08               (*((volatile unsigned int *)   (APB_XVER_BASE+0x20)))
#define XVR_REG09               (*((volatile unsigned int *)   (APB_XVER_BASE+0x24)))
#define XVR_REG0A               (*((volatile unsigned int *)   (APB_XVER_BASE+0x28)))
#define XVR_REG0B               (*((volatile unsigned int *)   (APB_XVER_BASE+0x2C)))

#define XVR_REG0C               (*((volatile unsigned int *)   (APB_XVER_BASE+0x30)))
#define XVR_REG0D               (*((volatile unsigned int *)   (APB_XVER_BASE+0x34)))
#define XVR_REG0E               (*((volatile unsigned int *)   (APB_XVER_BASE+0x38)))
#define XVR_REG0F               (*((volatile unsigned int *)   (APB_XVER_BASE+0x3C)))

// Read-Back Registers
#define XVR_REG10                (*((volatile unsigned int *)   (APB_XVER_BASE+0x40)))
#define XVR_REG11                (*((volatile unsigned int *)   (APB_XVER_BASE+0x44)))
#define XVR_REG12                (*((volatile unsigned int *)   (APB_XVER_BASE+0x48)))
#define XVR_REG13                (*((volatile unsigned int *)   (APB_XVER_BASE+0x4C)))

#define XVR_REG14                (*((volatile unsigned int *)   (APB_XVER_BASE+0x50)))
#define XVR_REG15                (*((volatile unsigned int *)   (APB_XVER_BASE+0x54)))
#define XVR_REG16                (*((volatile unsigned int *)   (APB_XVER_BASE+0x58)))
#define XVR_REG17                (*((volatile unsigned int *)   (APB_XVER_BASE+0x5C)))

#define XVR_REG18                (*((volatile unsigned int *)   (APB_XVER_BASE+0x60)))
#define XVR_REG19                (*((volatile unsigned int *)   (APB_XVER_BASE+0x64)))
#define XVR_REG1A                (*((volatile unsigned int *)   (APB_XVER_BASE+0x68)))
#define XVR_REG1B                (*((volatile unsigned int *)   (APB_XVER_BASE+0x6C)))

#define XVR_REG1C                (*((volatile unsigned int *)   (APB_XVER_BASE+0x70)))
#define XVR_REG1D                (*((volatile unsigned int *)   (APB_XVER_BASE+0x74)))
#define XVR_REG1E                (*((volatile unsigned int *)   (APB_XVER_BASE+0x78)))
#define XVR_REG1F                (*((volatile unsigned int *)   (APB_XVER_BASE+0x7C)))

// XVR Config Registers 
#define XVR_REG20                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x80)))
#define XVR_REG21                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x84)))
#define XVR_REG22                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x88)))
#define XVR_REG23                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x8C)))

#define XVR_REG24                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x90)))
#define XVR_REG25                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x94)))
#define XVR_REG26                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x98)))
#define XVR_REG27                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x9C)))

#define XVR_REG28                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xA0)))
#define XVR_REG29                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xA4)))
#define XVR_REG2A                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xA8)))
#define XVR_REG2B                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xAC)))

#define XVR_REG2C                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xB0)))
#define XVR_REG2D                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xB4)))
#define XVR_REG2E                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xB8)))
#define XVR_REG2F                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xBC)))

#define XVR_REG30                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xC0)))
#define XVR_REG31                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xC4)))
#define XVR_REG32                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xC8)))
#define XVR_REG33                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xCC)))

#define XVR_REG34                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xD0)))
#define XVR_REG35                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xD4)))
#define XVR_REG36                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xD8)))
#define XVR_REG37                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xDC)))

#define XVR_REG38                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xE0)))
#define XVR_REG39                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xE4)))
#define XVR_REG3A                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xE8)))
#define XVR_REG3B                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xEC)))

#define XVR_REG3C                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xF0)))
#define XVR_REG3D                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xF4)))
#define XVR_REG3E                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xF8)))
#define XVR_REG3F                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xFC)))

#define XVR_REG40                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x100)))
#define XVR_REG41                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x104)))
#define XVR_REG42                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x108)))
#define XVR_REG43                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x10C)))

#define XVR_REG44                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x110)))
#define XVR_REG45                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x114)))
#define XVR_REG46                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x118)))
#define XVR_REG47                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x11C)))

#define XVR_REG48                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x120)))
#define XVR_REG49                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x124)))
#define XVR_REG4A                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x128)))
#define XVR_REG4B                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x12C)))

#define XVR_REG4C                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x130)))
#define XVR_REG4D                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x134)))
#define XVR_REG4E                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x138)))
#define XVR_REG4F                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x13C)))


/// backup analog register
extern u_int32 backup_xvr_analog_reg[];
#define mHWreg_Assign_XVR_Regsiter(No,Value) \
    { \
        XVR_REG##No = Value; \
        if(((0x##No)>=0) && ((0x##No)<=0x0f)) {\
            backup_xvr_analog_reg[(0x##No)] = Value; \
        } \
    }


#define BIT_AUTO_SYNCWORD              (0x1<<31)
#define BIT_AUTO_SYNCWIN               (0x1<<30)
#define BIT_AUTO_CHNN                  (0x1<<29)


                                   
                                      

#define BIT_CUR_CFG_AGC                (0x01<<16)
#define BIT_CUR_CFG_WIN                (0x1F<<11)
#define BIT_CUR_CFG_PWR                (0x0F<<7)
#define BIT_CUR_CFG_CHN                (0x7F)

/* #define XVR_REG25                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x94))) */
#define BIT_RSSI_CAL_EN 
#define BIT_VCO_KCAL_EN 
#define BIT_TEST_EDR3   
#define BIT_TEST_EDR2   
#define BIT_TEST_RADIO
#define BIT_TEST_TMODE
#define BIT_PN9_SEND_EN
#define BIT_PN9_RECV_EN
#define BIT_PN9_HOLD_EN
#define BIT_PAT_SEND_EN
#define BIT_TEST_PATTERN

/* #define XVR_REG26                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x98))) */
/* #define XVR_REG27                      (*((volatile unsigned int *)   (APB_XVER_BASE+0x9C))) */
/* #define XVR_REG28                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xA0))) */
/* #define XVR_REG29                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xA4))) */
/* #define XVR_REG2A                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xA8))) */
/* #define XVR_REG2B                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xAC))) */
/* #define XVR_REG2C                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xB0))) */
/* #define XVR_REG2D                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xB4))) */
/* #define XVR_REG2E                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xB8))) */
/* #define XVR_REG2F                      (*((volatile unsigned int *)   (APB_XVER_BASE+0xBC))) */

/*****************************************************************************
* Hardware Physical Config word:
******************************************************************************/

#define HWhab_PHY_CFG              0x37016a98 // 24MHz

/*****************************************************************************
* Rohm Radio Verion :
******************************************************************************/

#define ROHM_RADIO_VER  94672

//#define ROHM_RADIO_VER  9467  // Newer Radio Version - BU9467MUV
                              // Revision :- 2.14.0
                              // Date :- April 2009

//#define ROHM_RADIO_VER  9468  // Older Radio Version - BU9468MUV
                              // Revision :- 2.7.0
                              // Date :- August 2007

/*****************************************************************************
* Rohm Radio Power Level Definitions:
******************************************************************************/

#define HW_RADIO_ENHANCED_POWER_CONTROL_TEST_MODE 0 /* use with 9468 only */

#if (ROHM_RADIO_VER==9468)
#define HW_RADIO_MAX_TX_POWER_LEVEL 4 /* 5 power levels */
#define HW_RADIO_TX_POWER_STEP_SIZE 3 /* 3dBm step sizes */
#else
#define HW_RADIO_MAX_TX_POWER_LEVEL 2 /* 3 power levels */
#define HW_RADIO_TX_POWER_STEP_SIZE 5 /* 5dBm step sizes */
#endif

#if (HW_RADIO_ENHANCED_POWER_CONTROL_TEST_MODE==1)

#define HW_RADIO_MIN_1MBIT_TX_POWER 0
#define HW_RADIO_MIN_2MBIT_TX_POWER 1
#define HW_RADIO_MIN_3MBIT_TX_POWER 1

#define HW_RADIO_MAX_1MBIT_TX_POWER HW_RADIO_MAX_TX_POWER_LEVEL
#define HW_RADIO_MAX_2MBIT_TX_POWER (HW_RADIO_MAX_TX_POWER_LEVEL-1)
#define HW_RADIO_MAX_3MBIT_TX_POWER (HW_RADIO_MAX_TX_POWER_LEVEL-1)

#else /* all transmit levels available to all data rates */

#define HW_RADIO_MIN_1MBIT_TX_POWER 0
#define HW_RADIO_MIN_2MBIT_TX_POWER 0
#define HW_RADIO_MIN_3MBIT_TX_POWER 0

#define HW_RADIO_MAX_1MBIT_TX_POWER HW_RADIO_MAX_TX_POWER_LEVEL
#define HW_RADIO_MAX_2MBIT_TX_POWER HW_RADIO_MAX_TX_POWER_LEVEL
#define HW_RADIO_MAX_3MBIT_TX_POWER HW_RADIO_MAX_TX_POWER_LEVEL

#endif

/*****************************************************************************
* The definitions for Radio Setup:
******************************************************************************/

#define HW_RADIO_TIME_TX_SLOT 390 /* max radio time tx slot (acl/syn) */
#define HW_RADIO_TIME_RX_SLOT 390 /* max radio time rx slot (acl/syn) */
#define HW_RADIO_TIME_TX_ID_PACKET_SLOT 80 /* radio time tx slot (id) */

#define HW_RADIO_INDEPENDENT_TX_RX_SETUP_TIMES 1

#define HW_RADIO_TX_SETUP_TIME 192
#define HW_RADIO_RX_SETUP_TIME 192

/* recommended time for T_SYNC = T2+T3+T4 = 1us + 109us + 86us = 196us,
   however, instead calculate relative to HW_RADIO_SETUP_TIME */
#define HW_RADIO_SPI_WRITE_LENGTH 12 /* aprox 12us to perform a SPI read/write */
#define HW_RADIO_T1_PLL_START_TO_TX_SYNC_PULSE 8 /* spec: 8us to Tx Pulse */

#define HW_RADIO_PRE_TX_SYNC_TIME_T_SYNC /* BDATA1 sync pulse to TX */ \
    (HW_RADIO_TX_SETUP_TIME - HW_RADIO_SPI_WRITE_LENGTH \
    - HW_RADIO_T1_PLL_START_TO_TX_SYNC_PULSE)

#define HW_RADIO_PRE_TX_PULSE_LENGTH 1 /* min BDATA1 sync pulse width */

// Had to change to 5ms - as 6ms resulted in failure.
#define HW_RADIO_BLUE_RF_TIME_T_XTL 5000 /* BXTLEN low to BRCLK output available */
#define HW_RADIO_BLUE_RF_TIME_T_PA 86 /* BPKTCTL high to BDATA1 transmit available */
//#define HW_RADIO_BLUE_RF_TIME_T_PA 90
#define HW_RADIO_BLUE_RF_TIME_T_PD 6 /* BDATA1 transmit disable to BPKTCTL low */ 

/*****************************************************************************
*
* HD15710x PROPOGATION DELAYS BETWEEN ANTENNA AND BDATA1:
*
******************************************************************************/

#if 1 /* tab_chimera_rohm_050410 */

#define mHWradio_RX_TAB_DELAY 5
#define mHWradio_TX_TAB_DELAY 1
//#define mHWradio_RX_TAB_DELAY 3
//#define mHWradio_TX_TAB_DELAY 3

#define mHWradio_RX_DELAY (5 + mHWradio_RX_TAB_DELAY)
#define mHWradio_TX_DELAY (4 + mHWradio_TX_TAB_DELAY)

#else

#define mHWradio_RX_TAB_DELAY 0
#define mHWradio_TX_TAB_DELAY 4

#if 1 // GF BQB - Changes Suggested by Chen - ViMicro - Test Trx-03
      // Trx_03 passes with Rx_Delay set to 7
#define mHWradio_RX_DELAY (5 + mHWradio_RX_TAB_DELAY)
#else
#define mHWradio_RX_DELAY (7 + mHWradio_RX_TAB_DELAY)
#endif
#define mHWradio_TX_DELAY (4 + mHWradio_TX_TAB_DELAY)

#endif

/*****************************************************************************
* Register bit definitions
******************************************************************************/

#define mBIT(X) (((u_int32)1) << (X))

/*****************************************************************************
*
* HD15710x REGISTER READS:
* bit 31 = 0 for SPI block to read
* bits 22..24 => 101 device address.
* Read from Radio - bit 21 = 1.
* reg address N - bit 16..20
*
******************************************************************************/

#define READ_REG(n)  (0x01600000|((n)<<16))

/*****************************************************************************
*
* HD15710x REGISTER WRITES:
* bit 31 = 1 for SPI block to write
* bits 22..24 => 101 device address.
* Write to Radio - bit 21 = 0.
* reg address N - bit 16..20
*
******************************************************************************/

#define WRITE_REG(n,val)  (0x81400000|((n)<<16)|(val))

/*****************************************************************************
*
* BU9468MUV REGISTER CONFIGURATIONS:
*
******************************************************************************/
#if (ROHM_RADIO_VER==9468) // Pre 19 Oct - Old Radio
#define WRITE_REG0_RECEIVE           WRITE_REG(0x00,0x0000) // 00: TxChannel + rx freq 
#define WRITE_REG0_TRANSMIT          WRITE_REG(0x00,0x0001) // 00: RxChannel + tx freq
#define WRITE_REG1                   WRITE_REG(0x01,0x4032 /*0x9632*/) // 01: SyntRxOnCounter
#define WRITE_REG2                   WRITE_REG(0x02,0x0016 /*0x0020*/) // 02: XO Trim
#define WRITE_REG4                   WRITE_REG(0x04,0x4C00 /*0x2C00*/) // 04: Trim Data
//#define WRITE_REG5                   WRITE_REG(0x05,0xCF10 /*0x8844*/) // 05: Control [bbRdy[2]=1 BXTLEN]
// GF 7 March correct setting for bbRdy
#define WRITE_REG5                   WRITE_REG(0x05,0xCF14 /*0x8844*/) // 05: Control [bbRdy[2]=1 BXTLEN]
#define WRITE_REG6                   WRITE_REG(0x06,0x0020 /*0x0000*/) // 06: BlockOn 1
#define WRITE_REG7                   WRITE_REG(0x07,0x0000) // 07: BlockOn 2
#define WRITE_REG8                   WRITE_REG(0x08,0x0008) // 08: Enable
//#define WRITE_REG9                   WRITE_REG(0x09,0xFCFF) // 09: Power Ctrl
#define WRITE_REG9                   WRITE_REG(0x09,0xFFFF) // 09: Power Ctrl

#define WRITE_REG9_TX_POWER(x) ( WRITE_REG(0x09,((u_int32)Power_control_table[x])))

#define WRITE_REGA                   WRITE_REG(0x0A,0x0012) // 10: Frequency Reference HI
#define WRITE_REGB                   WRITE_REG(0x0B,0x4f80) // 11: Frequency Reference LO
#define WRITE_REGC                   WRITE_REG(0x0C,0x0000) // 12: DSM IN (test) HI
#define WRITE_REGD                   WRITE_REG(0x0D,0x0000) // 13: DSM IN (test) LO
#define WRITE_REGE                   WRITE_REG(0x0E,0x0000) // 14: DSM OUT (test)
#define WRITE_REGF                   WRITE_REG(0x0F,0x800C /*0x80ff*/) // 15: Test Pins (test)
#define WRITE_REG10                  WRITE_REG(0x10,0x000A /*0x010a*/) // 16: Auxilary 1
#define WRITE_REG11                  WRITE_REG(0x11,0x1C00 /*0x1900*/) // 17: Auxilary 2

#define WRITE_REG13                  WRITE_REG(0x13,0x0808) // 19: PA Counter
#define WRITE_REG14_IDLE             WRITE_REG(0x14,0x0000) // 20: wrOffMode (goto idle)
#define WRITE_REG15                  WRITE_REG(0x15,0x0570 /*0x057a*/) // 21: LNA Auto Gain 
#define WRITE_REG16                  WRITE_REG(0x16,0x0000) // 22: 
#define WRITE_REG17                  WRITE_REG(0x17,0x0003 /* 1 2 4  0x0003*/) // 23: PSKSYNCACCDET
#define WRITE_REG18                  WRITE_REG(0x18,0x0810 /*0x0810*/) // 24: ADC_DAC_ctrl
#define WRITE_REG19                  WRITE_REG(0x19,0x3ff8) // 25: Clock Division HI
#define WRITE_REG1A                  WRITE_REG(0x1A,0x0004) // 26: Clock Division LO
#define WRITE_REG1B                  WRITE_REG(0x1B,0x48AD /*0x48AD*/) // 27: Modem Debug 1
#define WRITE_REG1C                  WRITE_REG(0x1C,0x84E2 /*0x84E2*/) // 28: Modem Debug 2
#define WRITE_REG1D                  WRITE_REG(0x1D,0x0000) // 29: Modem Debug 3

#else
#if (ROHM_RADIO_VER==9467)

#define WRITE_REG0_RECEIVE           WRITE_REG(0x00,0x0000) // 00: TxChannel + rx freq 
#define WRITE_REG0_TRANSMIT          WRITE_REG(0x00,0x0001) // 00: RxChannel + tx freq
#define WRITE_REG1                   WRITE_REG(0x01,0x9632 /*0x9632*/) // 01: SyntRxOnCounter
#define WRITE_REG2                   WRITE_REG(0x02,0x0050 /*0x0020*/) // 02: XO Trim
#define WRITE_REG4                   WRITE_REG(0x04,0x3C28 /*0x2C00*/) // 04: Trim Data
//#define WRITE_REG5                   WRITE_REG(0x05,0xCF10 /*0x8844*/) // 05: Control [bbRdy[2]=1 BXTLEN]
// GF 7 March correct setting for bbRdy
#define WRITE_REG5                   WRITE_REG(0x05,0xCF14 /*0x8844*/) // 05: Control [bbRdy[2]=1 BXTLEN]
#define WRITE_REG6                   WRITE_REG(0x06,0x8020 /*0x0000*/) // 06: BlockOn 1
#define WRITE_REG7                   WRITE_REG(0x07,0x0000) // 07: BlockOn 2
#define WRITE_REG8                   WRITE_REG(0x08,0x5400) // 08: Enable
#define WRITE_REG9                   WRITE_REG(0x09,0x874F) // 09: Power Ctrl

#define WRITE_REG9_TX_POWER(x) ( WRITE_REG(0x09,((u_int32)Power_control_table[x])))
#define WRITE_REG2_XO_TRIM(x)  ( WRITE_REG(0x02,(u_int8) x))
#define WRITE_REGA                   WRITE_REG(0x0A,0x0012) // 10: Frequency Reference HI
#define WRITE_REGB                   WRITE_REG(0x0B,0x4f80) // 11: Frequency Reference LO
#define WRITE_REGC                   WRITE_REG(0x0C,0x0000) // 12: DSM IN (test) HI
#define WRITE_REGD                   WRITE_REG(0x0D,0x0000) // 13: DSM IN (test) LO
#define WRITE_REGE                   WRITE_REG(0x0E,0x0000) // 14: DSM OUT (test)
#define WRITE_REGF                   WRITE_REG(0x0F,0x800D /*0x80ff*/) // 15: Test Pins (test)
#define WRITE_REG10                  WRITE_REG(0x10,0x000A /*0x010a*/) // 16: Auxilary 1
#define WRITE_REG11                  WRITE_REG(0x11,0x9B00 /*0x1900*/) // 17: Auxilary 2
#define WRITE_REG12                  WRITE_REG(0x12,0x0000) // 18 :  IQreq
#define WRITE_REG13                  WRITE_REG(0x13,0x0008) // 19: PA Counter

#define WRITE_REG14_IDLE             WRITE_REG(0x14,0x0000) // 20: wrOffMode (goto idle)
#define WRITE_REG15                  WRITE_REG(0x15,0xA32A /*0x057a*/) // 21: LNA Auto Gain 
#define WRITE_REG21_RX_GAIN(x)      (WRITE_REG(0x15,x))

#define WRITE_REG16                  WRITE_REG(0x16,0x4000) // 22: 


#define WRITE_REG17                  WRITE_REG(0x17,0x0003 /* 1 2 4  0x0003*/) // 23: PSKSYNCACCDET
#define WRITE_REG18                  WRITE_REG(0x18,0x0210 /*0x0810*/) // 24: ADC_DAC_ctrl
#define WRITE_REG19                  WRITE_REG(0x19,0x3ff8) // 25: Clock Division HI
#define WRITE_REG1A                  WRITE_REG(0x1A,0x0004) // 26: Clock Division LO
#define WRITE_REG1B                  WRITE_REG(0x1B,0x48AD /*0x48AD*/) // 27: Modem Debug 1
#define WRITE_REG1C                  WRITE_REG(0x1C,0x84E2 /*0x84E2*/) // 28: Modem Debug 2
#define WRITE_REG1D                  WRITE_REG(0x1D,0x0000) // 29: Modem Debug 3

#else  //((ROHM_RADIO_VER==94672)

#define WRITE_REG0_RECEIVE           WRITE_REG(0x00,0x0000) // 00: TxChannel + rx freq 
#define WRITE_REG0_TRANSMIT          WRITE_REG(0x00,0x0001) // 00: RxChannel + tx freq
//#define WRITE_REG1                   WRITE_REG(0x01,0x9632 /*0x9632*/) // 01: SyntRxOnCounter
//#define WRITE_REG1_LE                WRITE_REG(0x01,0x6432 /*0x9632*/)
#if 1 // TEMP CHECK with EDD !! Anritsu.
#define WRITE_REG1                WRITE_REG(0x01,0x4032 /*0x9632*/)
#else
#define WRITE_REG1                WRITE_REG(0x01,0x4052 /*0x9632*/)
#endif
#define WRITE_REG2                   WRITE_REG(0x02,0x0064 /*0x0020*/) // 02: XO Trim
#define WRITE_REG4                   WRITE_REG(0x04,0x4C28 /*0x2C00*/) // 04: Trim Data
//#define WRITE_REG5                   WRITE_REG(0x05,0xCF10 /*0x8844*/) // 05: Control [bbRdy[2]=1 BXTLEN]
// GF 7 March - correct setting for bbRdy
#define WRITE_REG5                   WRITE_REG(0x05,0xCF14 /*0x8844*/) // 05: Control [bbRdy[2]=1 BXTLEN]
#define WRITE_REG6                   WRITE_REG(0x06,0x8020 /*0x0000*/) // 06: BlockOn 1
#define WRITE_REG7                   WRITE_REG(0x07,0x0000) // 07: BlockOn 2
#define WRITE_REG8                   WRITE_REG(0x08,0x5400) // 08: Enable
#define WRITE_REG9                   WRITE_REG(0x09,0x86AF) // 09: Power Ctrl

#define WRITE_REG9_TX_POWER(x) ( WRITE_REG(0x09,((u_int32)Power_control_table[x])))

#define WRITE_REGA                   WRITE_REG(0x0A,0x0012) // 10: Frequency Reference HI
#define WRITE_REGB                   WRITE_REG(0x0B,0x4f80) // 11: Frequency Reference LO
#define WRITE_REGC                   WRITE_REG(0x0C,0x0000) // 12: DSM IN (test) HI
#define WRITE_REGD                   WRITE_REG(0x0D,0x0000) // 13: DSM IN (test) LO
#define WRITE_REGE                   WRITE_REG(0x0E,0x0000) // 14: DSM OUT (test)
#define WRITE_REGF                   WRITE_REG(0x0F,0x80FF /*0x80ff*/) // 15: Test Pins (test)
#define WRITE_REG10                  WRITE_REG(0x10,0x000A /*0x010a*/) // 16: Auxilary 1
#define WRITE_REG11                  WRITE_REG(0x11,0x9B00 /*0x1900*/) // 17: Auxilary 2
#define WRITE_REG12                  WRITE_REG(0x12,0x0000) // 18 :  IQreq
// GF Changed 8 Dec
#if 1 // GF Changed 8 Dec - Changed Back
#define WRITE_REG13                  WRITE_REG(0x13,0x0008) // 19: PA Counter
#else
#define WRITE_REG13                  WRITE_REG(0x13,0x2608) // 19: PA Counter    PACOUNT = 40us - Depends on BPKTCTL position
#endif
#define WRITE_REG14_IDLE             WRITE_REG(0x14,0x0000) // 20: wrOffMode (goto idle)
#define WRITE_REG15                  WRITE_REG(0x15,0xA32A /*0x057a*/) // 21: LNA Auto Gain 
#define WRITE_REG21_RX_GAIN(x)      (WRITE_REG(0x15,x))

#define WRITE_REG16                  WRITE_REG(0x16,0x4000) // 22: 


#define WRITE_REG17                  WRITE_REG(0x17,0x0003 /* 1 2 4  0x0003*/) // 23: PSKSYNCACCDET
#define WRITE_REG18                  WRITE_REG(0x18,0x0210 /*0x0810*/) // 24: ADC_DAC_ctrl
#define WRITE_REG19                  WRITE_REG(0x19,0x3ff8) // 25: Clock Division HI
#define WRITE_REG1A                  WRITE_REG(0x1A,0x0004) // 26: Clock Division LO
#define WRITE_REG1B                  WRITE_REG(0x1B,0x48AD /*0x48AD*/) // 27: Modem Debug 1
#define WRITE_REG1C                  WRITE_REG(0x1C,0x84E2 /*0x84E2*/) // 28: Modem Debug 2
#define WRITE_REG1D                  WRITE_REG(0x1D,0x0000) // 29: Modem Debug 3


#endif
#endif
/*****************************************************************************
*
* Defines to support high/low override control of GIO lines (debug purposes):
*
* HWradio_SetOverrideLow(GIO_N);
* HWradio_SetOverrideHigh(GIO_N);
*
******************************************************************************/


#define HW_RADIO_GIO_LOW_GIO_0              HAB_GIO_HIGH_CTRL_1_AND_0_REG
#define HW_RADIO_GIO_HIGH_GIO_0             HAB_GIO_LOW_CTRL_1_AND_0_REG
#define HW_RADIO_GIO_OVERRIDE_MASK_GIO_0    0x00008000

#define HW_RADIO_GIO_LOW_GIO_1              HAB_GIO_HIGH_CTRL_1_AND_0_REG
#define HW_RADIO_GIO_HIGH_GIO_1             HAB_GIO_LOW_CTRL_1_AND_0_REG
#define HW_RADIO_GIO_OVERRIDE_MASK_GIO_1    0x80000000

#define HW_RADIO_GIO_LOW_GIO_2              HAB_GIO_HIGH_CTRL_3_AND_2_REG
#define HW_RADIO_GIO_HIGH_GIO_2             HAB_GIO_LOW_CTRL_3_AND_2_REG
#define HW_RADIO_GIO_OVERRIDE_MASK_GIO_2    0x00008000

#define HW_RADIO_GIO_LOW_GIO_3              HAB_GIO_HIGH_CTRL_3_AND_2_REG
#define HW_RADIO_GIO_HIGH_GIO_3             HAB_GIO_LOW_CTRL_3_AND_2_REG
#define HW_RADIO_GIO_OVERRIDE_MASK_GIO_3    0x80000000

#define HW_RADIO_GIO_LOW_GIO_4              HAB_GIO_HIGH_CTRL_5_AND_4_REG
#define HW_RADIO_GIO_HIGH_GIO_4             HAB_GIO_LOW_CTRL_5_AND_4_REG
#define HW_RADIO_GIO_OVERRIDE_MASK_GIO_4    0x00008000

#define HW_RADIO_GIO_LOW_GIO_5              HAB_GIO_HIGH_CTRL_5_AND_4_REG
#define HW_RADIO_GIO_HIGH_GIO_5             HAB_GIO_LOW_CTRL_5_AND_4_REG
#define HW_RADIO_GIO_OVERRIDE_MASK_GIO_5    0x80000000

#define HW_RADIO_GIO_LOW_GIO_6              HAB_GIO_HIGH_CTRL_7_AND_6_REG
#define HW_RADIO_GIO_HIGH_GIO_6             HAB_GIO_LOW_CTRL_7_AND_6_REG
#define HW_RADIO_GIO_OVERRIDE_MASK_GIO_6    0x00008000

#define HW_RADIO_GIO_LOW_GIO_7              HAB_GIO_HIGH_CTRL_7_AND_6_REG
#define HW_RADIO_GIO_HIGH_GIO_7             HAB_GIO_LOW_CTRL_7_AND_6_REG
#define HW_RADIO_GIO_OVERRIDE_MASK_GIO_7    0x80000000

#define HW_RADIO_GIO_LOW_GIO_8              HAB_GIO_HIGH_CTRL_9_AND_8_REG
#define HW_RADIO_GIO_HIGH_GIO_8             HAB_GIO_LOW_CTRL_9_AND_8_REG
#define HW_RADIO_GIO_OVERRIDE_MASK_GIO_8    0x00008000

#define HW_RADIO_GIO_LOW_GIO_9              HAB_GIO_HIGH_CTRL_9_AND_8_REG
#define HW_RADIO_GIO_HIGH_GIO_9             HAB_GIO_LOW_CTRL_9_AND_8_REG
#define HW_RADIO_GIO_OVERRIDE_MASK_GIO_9    0x80000000

#define HW_RADIO_GIO_LOW_GIO_A              HAB_GIO_HIGH_CTRL_B_AND_A_REG
#define HW_RADIO_GIO_HIGH_GIO_A             HAB_GIO_LOW_CTRL_B_AND_A_REG
#define HW_RADIO_GIO_OVERRIDE_MASK_GIO_A    0x00008000

#define HW_RADIO_GIO_LOW_GIO_B              HAB_GIO_HIGH_CTRL_B_AND_A_REG
#define HW_RADIO_GIO_HIGH_GIO_B             HAB_GIO_LOW_CTRL_B_AND_A_REG
#define HW_RADIO_GIO_OVERRIDE_MASK_GIO_B    0x80000000


/*****************************************************************************
* GIO override macros
******************************************************************************/

#define HWradio_SetOverrideLow(GIO_NAME)	\
				mHWreg_Logical_OR_With_Register(HW_RADIO_GIO_LOW_##GIO_NAME, \
					HW_RADIO_GIO_OVERRIDE_MASK_##GIO_NAME); \
				mHWreg_Logical_AND_With_Register(HW_RADIO_GIO_HIGH_##GIO_NAME, \
					~(HW_RADIO_GIO_OVERRIDE_MASK_##GIO_NAME))

#define HWradio_SetOverrideHigh(GIO_NAME)	\
				mHWreg_Logical_OR_With_Register(HW_RADIO_GIO_HIGH_##GIO_NAME, \
					HW_RADIO_GIO_OVERRIDE_MASK_##GIO_NAME); \
				mHWreg_Logical_AND_With_Register(HW_RADIO_GIO_LOW_##GIO_NAME, \
					~(HW_RADIO_GIO_OVERRIDE_MASK_##GIO_NAME)) 

#define HWradio_CancelOverride(GIO_NAME) \
				mHWreg_Logical_AND_With_Register(HW_RADIO_GIO_LOW_##GIO_NAME, \
					~(HW_RADIO_GIO_OVERRIDE_MASK_##GIO_NAME)); \
				mHWreg_Logical_AND_With_Register(HW_RADIO_GIO_HIGH_##GIO_NAME, \
					~(HW_RADIO_GIO_OVERRIDE_MASK_##GIO_NAME))

void HWradio_Init_Tx_Power_Level(u_int8 power_level);
void HWradio_Set_Tx_Power_Level(u_int8 power_level);

#endif // HW_RADIO_DEFS
