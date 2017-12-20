/*************************************************************************
 *
 * MODULE NAME:    hw_radio.c
 * PROJECT CODE:   XpertBlue
 * DESCRIPTION:    Rohm Bluetooth Ver.2.0+EDR radio driver for Tabasco.
 * MAINTAINER:     Tom Kerwick
 * CREATION DATE:  07.09.07
 *
 * SOURCE CONTROL: $Id: hw_radio.c,v 1.32 2011/05/17 16:19:41 tomk Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2007 Ceva Inc.
 *     All rights reserved.
 *
 * RECENT REVISION HISTORY:
 *
 *  07.09.07 TK    initial reference version, untested.
 *  28.09.07 TK    modified for rohm bu9468muv radio, still untested.
 *  01.02.08 TK    modified for independent tx/rx setup times.
 *
 ********************************************************************************/

/********************************************************************************
 *      Included File
 ********************************************************************************/

#include "sys_config.h"
#include "sys_types.h"
#include "hw_radio.h"
#include "hw_hab_defs.h"
#include "hw_habanero.h"
#include "hw_habanero_impl.h"
#include "hw_lc.h"
#include "hw_radio_defs.h"
#include "sys_mmi.h"
#include "hw_delay.h"
#include "le_config.h"
#include "BKAPI.h"
#include "debug.h"
#define valid_frequency(FRAME_POS,MASK)    ((MASK & RADIO_FREQ_MASK(FRAME_POS)) != 0)
t_RadioMode CurrentRadioMode =RADIO_MODE_STANDBY;
boolean HWradio_LE_Service(t_RadioMode in_RadioMode, u_int8 io_Channel, t_radio_freq_mask freq_mask);
void HWradio_Init_Tx_Power_Level(u_int8 power_level);
void HWradio_Initialise(void);
void HWradio_Reset(void);
void HWradio_Set_Syncword(t_syncword const syncword);
/*************************************************************************************
 *
 * FUNCTION NAME: HWradio_TxComplete
 * DESCRIPTION:    This function should be called from the PKA interrupt handler.
 *                Its purpose is to cancel the GIO lines extension after transmitting
 *                a multi-slot packet. It is also used to force the BPKTCTL line low
 *                in order to disable the radio's power amplifier so as to save power.
 *
 * PARAMETER:      None.
 *
 *
 * RETURNS :       None.
 *
 *
 * SPECIAL CONSIDERATIONS:  None.
 *
 *
 *************************************************************************************/
void HWradio_TxComplete(void)
{
}


s_int8 HWradio_LE_Read_RSSI(void)
{
    return 0;
    //return (s_int8)(-95 + ((s_int8)((_HWradio_ReadNow(READ_REG(3))>>8)&0xff)));
}

u_int8 HWradio_Convert_Tx_Power_to_Tx_Power_Level_Units(s_int8 power_level)
{
    if (power_level >= -2)
        return MAX_POWER_LEVEL_UNITS;
    else if ((power_level > -7) && (power_level < -2))
        return 1;
    else
        return 0;
}

s_int8 HWradio_Convert_Tx_Power_Level_Units_to_Tx_Power(u_int8 power_level)
{
    return 0;
}
/***************************************************************************
 *
 * FUNCTION:   HWradio_Reset
 * PURPOSE:    Resets the Habanero core AND register map by writing special
 *             sequence to reset register
 *               Also resets radio registers.
 *
 *    NOTE:      Reset for Habanero implemented in core (hw_lc.c)
 ***************************************************************************/
void HWradio_Reset(void)
{
}
/***************************************************************************
 *
 * FUNCTION:   HWradio_Initialise
 * PURPOSE:    Initialises Habanero
 *
 ***************************************************************************/
void HWradio_Initialise(void)
{
    CurrentRadioMode =RADIO_MODE_STANDBY;
}
void HWradio_Setup_cur_cfg_win(u_int8 cur_cfg_win)//bit 11~15 charles modify auto sync win 0 low thodhlod
{
    u_int32 temp_REG24;

    temp_REG24 = XVR_REG24;
    temp_REG24 &= ~0x0000F800;
    temp_REG24 |= (((u_int32)cur_cfg_win)<<11);
    XVR_REG24 = temp_REG24;
}

/*************************************************************************************
 *
 * FUNCTION NAME: HWradio_Init_Tx_Power_Level
 *
 * Initialises the radio power level - uses abstract representation of
 * transmitter power (0 to MAX_POWER_LEVEL_UNITS)
 *
 *************************************************************************************/
void HWradio_Init_Tx_Power_Level(u_int8 power_level)
{
    /* ProgNow initial radio power level, explicitly update power_level_now */
    u_int32 temp_REG24;

	
    temp_REG24 = XVR_REG24;
    temp_REG24 &= ~0x00000780;
    temp_REG24 |= (((u_int32)power_level)<<7);
    XVR_REG24 = temp_REG24;

    //Pwl<3:0>

    // Pout(dBm)

    // 0  : -38.02

    // 1  : -30.67

    // 2  : -23.06

    // 3  : -17.2

    // 4  : -12.39

    // 5  : -7.92

    // 6  : -3.73

    // 7  : -1.67

    // 8  : -0.32

    // 9  : 0.51

    // A  : 1.28

    // B  : 1.98

    // C  : 2.53

    // D  : 3.09

    // E  : 3.61

    // F  : 3.68
    //
    //
    //
}

//#if (PRH_BS_CFG_SYS_LMP_RSSI_SUPPORTED==1)
/*************************************************************************************
 *
 * FUNCTION NAME: HWradio_Set_Tx_Power_Level
 *
 * Set the radio power level - uses abstract representation of
 * transmitter power (0 to MAX_POWER_LEVEL_UNITS)
 *
 *************************************************************************************/
void HWradio_Set_Tx_Power_Level(u_int8 power_level)
{
#if 0 /* temp remove */
    u_int32 temp_REG24;
#endif
    return;//charles test
	
#if 0 /* temp remove */
    temp_REG24 = XVR_REG24;
    temp_REG24 &= ~0x00000780;
    temp_REG24 |= (((u_int32)power_level)<<7);
    XVR_REG24 = temp_REG24;
	
    // Pout(dBm)
#endif
    // 0  : -38.02

    // 1  : -30.67

    // 2  : -23.06

    // 3  : -17.2

    // 4  : -12.39

    // 5  : -7.92

    // 6  : -3.73

    // 7  : -1.67

    // 8  : -0.32

    // 9  : 0.51

    // A  : 1.28

    // B  : 1.98

    // C  : 2.53

    // D  : 3.09

    // E  : 3.61

    // F  : 3.68
}
void HWradio_Set_RF_channel(u_int8 io_Channel)
{
	  volatile u_int32 temp_REG24;

            temp_REG24 = XVR_REG24;
            temp_REG24 &= ~0x0000007F;
            temp_REG24 |= (io_Channel+2);
            XVR_REG24 = temp_REG24;
                temp_REG24 = XVR_REG24;
	
}

