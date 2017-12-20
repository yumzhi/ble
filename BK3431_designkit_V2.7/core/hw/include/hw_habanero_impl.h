#ifndef _PARTHUS_HW_HABANERO_IMPL_
#define _PARTHUS_HW_HABANERO_IMPL_

/******************************************************************************
 * MODULE NAME:    hw_habanero_impl.h
 * PROJECT CODE:    BlueStream
 * DESCRIPTION:    Inline implementation of register accesses of Habanero API
 * MAINTAINER:     Ivan Griffin
 * DATE:           20 March 2000
 *
 * SOURCE CONTROL: $Id: hw_habanero_impl.h,v 1.1 2012/05/09 00:09:52 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2004 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    20 March 2000          IG           Initial Version
 *
 * NOTES TO USERS:
 ******************************************************************************/
#include "hw_macro_defs.h"

#if (PRAGMA_INLINE==1)
#pragma inline(HWhab_Set_Syncword_Ref,\
HWhab_Set_Sync_Error,\
HWhab_Set_Rx_Mode,\
HWhab_Set_FDB_DIS,\
HWhab_Set_Sym_Gn,\
HWhab_Set_Sym_Enb,\
HWhab_Set_Sym_Mask,\
HWhab_Get_Sync_Error,\
HWhab_Get_Rx_Mode,\
HWhab_Get_FDB_DIS,\
HWhab_Get_Sym_Gn,\
HWhab_Get_Sym_Enb,\
HWhab_Get_Sym_Mask,\
HWhab_Set_RIF_Intr_Mask0,\
HWhab_Set_RIF_Intr_Mask1,\
HWhab_Set_RIF_Intr_Mask2,\
HWhab_Set_RIF_Intr_Mask3,\
HWhab_Set_LTR_Intr_Mask,\
HWhab_Set_RIF_Intr_Clr,\
HWhab_Set_LTR_Intr_Clr,\
HWhab_Get_RIF_Intr_Mask0,\
HWhab_Get_RIF_Intr_Mask1,\
HWhab_Get_RIF_Intr_Mask2,\
HWhab_Get_RIF_Intr_Mask3,\
HWhab_Get_LTR_Intr_Mask,\
HWhab_Get_RIF_Intr_Clr,\
HWhab_Get_LTR_Intr_Clr,\
HWhab_Set_PHY_Cfg,\
HWhab_Get_PHY_Cfg,\
HWhab_Get_LTR_Long_Period,\
HWhab_Get_LTR_Intr,\
HWhab_Get_RIF_Intr,\
HWhab_Get_GIO_Status,\
HWhab_Set_GIO_Ctrl)
#endif



/***************************************************************************
 * Receive Control Registers
 ***************************************************************************/

__INLINE__ void HWhab_Set_Sync_Error(const u_int8 sync_error)
{
    mSetHWEntry(HAB_SYNC_ERR, sync_error);
}

__INLINE__ u_int8 HWhab_Get_Sync_Error(void)
{
    return (u_int8)mGetHWEntry(HAB_SYNC_ERR);
}

#define HWradio_Set_Rx_Mode HWhab_Set_Rx_Mode

__INLINE__ void HWhab_Set_Rx_Mode(const t_HWradio_Rx_Mode rx_mode)
{
    mSetHWEntry(HAB_RX_MODE, rx_mode);
}

__INLINE__ t_HWradio_Rx_Mode HWhab_Get_Rx_Mode(void)
{
    return (t_HWradio_Rx_Mode)mGetHWEntry(HAB_RX_MODE);
}
__INLINE__ void HWhab_Set_FDB_DIS(const boolean state)
{
    mSetHWEntry(HAB_FDB_DIS, state);
}
__INLINE__ boolean HWhab_Get_FDB_DIS(void)
{
    return (boolean)mGetHWEntry(HAB_FDB_DIS);
}

__INLINE__ void HWhab_Set_Sym_Gn(const u_int8 gain)
{
    mSetHWEntry(HAB_SYM_GN, gain);
}

__INLINE__ u_int8 HWhab_Get_Sym_Gn(void)
{
    return (u_int8)mGetHWEntry(HAB_SYM_GN);
}

__INLINE__ void HWhab_Set_Sym_Enb(const boolean state)
{
    mSetHWEntry(HAB_SYM_ENB, state);
}

__INLINE__ boolean HWhab_Get_Sym_Enb(void)
{
    return (boolean)mGetHWEntry(HAB_SYM_ENB);
}

__INLINE__ void HWhab_Set_Sym_Mask(const boolean state)
{
    mSetHWEntry(HAB_SYM_MSK, state);
}

__INLINE__ boolean HWhab_Get_Sym_Mask(void)
{
    return (boolean)mGetHWEntry(HAB_SYM_MSK);
}


/***************************************************************************
 * Interrupt Control Registers
 ***************************************************************************/
__INLINE__ void HWhab_Set_RIF_Intr_Mask0(const boolean state) 
{
    mSetHWEntry(HAB_RIF_INTR_MSK0, state);
}

__INLINE__ boolean HWhab_Get_RIF_Intr_Mask0(void)
{
    return (boolean)mGetHWEntry(HAB_RIF_INTR_MSK0);
}

__INLINE__ void HWhab_Set_RIF_Intr_Mask1(const boolean state) 
{
    mSetHWEntry(HAB_RIF_INTR_MSK1, state);
}

__INLINE__ boolean HWhab_Get_RIF_Intr_Mask1(void)
{
    return (boolean)mGetHWEntry(HAB_RIF_INTR_MSK1);
}

__INLINE__ void HWhab_Set_RIF_Intr_Mask2(const boolean state) 
{
    mSetHWEntry(HAB_RIF_INTR_MSK2, state);
}

__INLINE__ boolean HWhab_Get_RIF_Intr_Mask2(void)
{
    return (boolean)mGetHWEntry(HAB_RIF_INTR_MSK2);
}

__INLINE__ void HWhab_Set_RIF_Intr_Mask3(const boolean state) 
{
    mSetHWEntry(HAB_RIF_INTR_MSK3, state);
}

__INLINE__ boolean HWhab_Get_RIF_Intr_Mask3(void)
{
    return (boolean)mGetHWEntry(HAB_RIF_INTR_MSK3);
}

__INLINE__ void HWhab_Set_LTR_Intr_Mask(const boolean state) 
{
    mSetHWEntry(HAB_LTR_INTR_MSK, state);
}

__INLINE__ boolean HWhab_Get_LTR_Intr_Mask(void)
{
    return (boolean)mGetHWEntry(HAB_LTR_INTR_MSK);
}


__INLINE__ u_int8 HWhab_Get_RIF_Intr_Clr(void)
{
    return (u_int8)mGetHWEntry(HAB_RIF_INTR_CLR);
}

__INLINE__ void HWhab_Set_RIF_Intr_Clr(const u_int8 clear_value) 
{
    mSetHWEntry(HAB_RIF_INTR_CLR, clear_value);
}

__INLINE__ void HWhab_Set_LTR_Intr_Clr(const u_int8 value)
{
    mSetHWEntry(HAB_LTR_INTR_CLR, value);
}

__INLINE__ u_int8 HWhab_Get_LTR_Intr_Clr(void)
{
    return (u_int8)mGetHWEntry(HAB_LTR_INTR_CLR);
}


/*
 * Interface Configuration Registers
 */
__INLINE__ void HWhab_Set_PHY_Cfg(const u_int32 magic_phy_value)
{
    mSetHWEntry32(HAB_PHY_CFG, magic_phy_value);
}

__INLINE__ u_int32 HWhab_Get_PHY_Cfg(void)
{
    return (u_int32)(mGetHWEntry32(HAB_PHY_CFG));
}



/***************************************************************************
 * Status Registers
 ***************************************************************************/

__INLINE__ u_int8 HWhab_Get_LTR_Long_Period(void) 
{
    return (u_int8)mGetHWEntry(HAB_LTR_LONG_PERIOD);
}

__INLINE__ u_int8 HWhab_Get_LTR_Intr(void) 
{
    return (u_int8)mGetHWEntry(HAB_LTR_INTR);
}

__INLINE__ u_int8 HWhab_Get_RIF_Intr(void) 
{
    return (u_int8)mGetHWEntry(HAB_RIF_INTR);
}

__INLINE__ u_int8 HWhab_Get_GIO_Status(void)
{
    return (u_int8)mGetHWEntry(HAB_GIO_STATUS);
}

__INLINE__ void HWhab_Set_High_Early(const u_int value)
{
    mSetHWEntry(HAB_HIGH_EARLY, value);
}

__INLINE__ void HWhab_Set_High_Late(const u_int value)
{
    mSetHWEntry(HAB_HIGH_LATE, value);
}

__INLINE__ void HWhab_Set_Low_Early(const u_int value)
{
    mSetHWEntry(HAB_LOW_EARLY, value);
}

__INLINE__ void HWhab_Set_Low_Late(const u_int value)
{
    mSetHWEntry(HAB_LOW_LATE, value);
}

#endif
