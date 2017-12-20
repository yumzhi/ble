#ifndef _PARTHUS_HW_RADIO_
#define _PARTHUS_HW_RADIO_

/******************************************************************************
 * MODULE NAME:    hw_radio.h
 * PROJECT CODE:   BlueStream
 * DESCRIPTION:    Generic RF/DSP hardware abstraction layer
 * MAINTAINER:     Ivan Griffin
 * DATE:           20 March 2000
 *
 * SOURCE CONTROL: $Id: hw_radio.h,v 1.2 2012/05/15 16:06:34 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2004 Ceva Inc.
 *     All rights reserved.
 *
 ******************************************************************************/

#include "sys_config.h"
#include "lc_types.h"
#include "hw_macro_defs.h"

/***************************************************************************
 *
 * HWradio Typedefs
 *
 ***************************************************************************/
typedef enum
{
    HWradio_RXm_NORMAL = 0,
    HWradio_RXm_FULL_WIN = 1
} t_HWradio_Rx_Mode;

typedef enum
{
    Power_Down_Mode_Active,
    Power_Down_Mode_Not_Active
} t_HWradio_Driver_Status;

typedef u_int8 t_radio_freq_mask;

#define RADIO_FREQ_MASK(X)    ((t_radio_freq_mask)(1 << (X)))

#define TX_START_FREQ    RADIO_FREQ_MASK(TX_START)
#define TX_MID_FREQ        RADIO_FREQ_MASK(TX_MID)
#define RX_START_FREQ    RADIO_FREQ_MASK(RX_START)
#define RX_MID_FREQ        RADIO_FREQ_MASK(RX_MID)

typedef enum {
    RADIO_MODE_STANDBY,
    RADIO_MODE_TX_RX,
    RADIO_MODE_LE_TIFS_TX_RX,
    RADIO_MODE_LE_INITIAL_TX,
    RADIO_MODE_LE_FULL_RX,
    RADIO_MODE_LE_INITIAL_RX
}t_RadioMode;

typedef enum
{
    HWradio_SERV_RX = 0,
    HWradio_SERV_DISABLE_RX ,
    HWradio_SERV_TX,
    HWradio_SERV_DISABLE_TX ,
    HWradio_SERV_RX_TX
}t_RadioServiceType;

        
#define HW_RADIO_MAX_CHANNEL 80
#define HW_RADIO_SETTING_INVALID (HW_RADIO_MAX_CHANNEL+1)

/***************************************************************************
 *
 * HWradio Interface Functions
 *
 ***************************************************************************/
void HWradio_Initialise(void);
void HWradio_Reset(void);

void HWradio_TxComplete(void);
void HWradio_LE_RxComplete(void);

void HWradio_Set_Tx_Power_Level(u_int8 power_level);
u_int8 HWradio_Get_Tx_Power_Level(void);


s_int8 HWradio_Read_RSSI(void); /* N == RSSI Return, -128 <= N <= 127 */

t_HWradio_Driver_Status HWradio_Get_Radio_Driver_Status(void);
void HWradio_Enter_Low_Power_Mode_Request(void);
void HWradio_Exit_Low_Power_Mode_Request(void);

u_int8 HWradio_Convert_Tx_Power_to_Tx_Power_Level_Units(s_int8 power_level);
s_int8 HWradio_Convert_Tx_Power_Level_Units_to_Tx_Power(u_int8 power_level);

void HWradio_Program_Dummy_Rx(u_int8 io_Channel);
void HWradio_LE_Setup_Radio_For_Next_TXRX(t_frame_pos const next_slot_posn);
s_int8 HWradio_LE_Read_RSSI(void);
//boolean HWradio_LE_Service(t_RadioMode in_RadioMode, u_int8 io_Channel, t_radio_freq_mask freq_mask);
void HWradio_Set_RF_channel(u_int8 io_Channel);

t_error HWradio_Inc_Tx_Power_Level(void);//merge CEVA 0517
t_error HWradio_Dec_Tx_Power_Level(void);//merge CEVA 0517

void HWradio_DisableAllSpiWrites(void);//merge CEVA 0517
void HWradio_Setup_For_TIFS_Event(u_int8 io_Channel);//merge CEVA 0517
void HWradio_Setup_cur_cfg_win(u_int8 cur_cfg_win);//bit 11~15 charles modify

#endif

