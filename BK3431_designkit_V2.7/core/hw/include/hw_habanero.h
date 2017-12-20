#ifndef _PARTHUS_HW_HABANERO_
#define _PARTHUS_HW_HABANERO_

/******************************************************************************
 * MODULE NAME:    hw_habanero.h
 * PROJECT CODE:    BlueStream
 * DESCRIPTION:    Habanero API
 * MAINTAINER:     Ivan Griffin
 * DATE:           20 March 2000
 *
 * SOURCE CONTROL: $Id: hw_habanero.h,v 1.1 2012/05/09 00:09:52 garyf Exp $
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
#include "sys_config.h"
#include "sys_types.h"
#include "hw_hab_defs.h"                         /* All Habanero definitions */

/***************************************************************************
 *
 * HWradio Interface Functions - these are declared in hw_radio.h
 * but defined in hw_habanero.c
 *
 *
 ***************************************************************************/

/***************************************************************************
 *
 * HABANERO Specific Functions
 *
 ***************************************************************************/

/*
 * Receive Control Registers
 */
__INLINE__ void HWhab_Set_Sync_Error(const u_int8 sync_error);
__INLINE__ void HWhab_Set_Rx_Mode(const t_HWradio_Rx_Mode rx_mode);
__INLINE__ void HWhab_Set_FDB_DIS(const boolean state);
__INLINE__ void HWhab_Set_Sym_Gn(const u_int8 gain);
__INLINE__ void HWhab_Set_Sym_Enb(const boolean state);
__INLINE__ void HWhab_Set_Sym_Mask(const boolean state);

__INLINE__ u_int8 HWhab_Get_Sync_Error(void);
__INLINE__ t_HWradio_Rx_Mode HWhab_Get_Rx_Mode(void);
__INLINE__ boolean HWhab_Get_FDB_DIS(void);
__INLINE__ u_int8 HWhab_Get_Sym_Gn(void);
__INLINE__ boolean HWhab_Get_Sym_Enb(void);
__INLINE__ boolean HWhab_Get_Sym_Mask(void);

/*
 * Interrupt Control Registers
 */
__INLINE__ void HWhab_Set_RIF_Intr_Mask0(const boolean state);
__INLINE__ void HWhab_Set_RIF_Intr_Mask1(const boolean state);
__INLINE__ void HWhab_Set_RIF_Intr_Mask2(const boolean state);
__INLINE__ void HWhab_Set_RIF_Intr_Mask3(const boolean state);
__INLINE__ void HWhab_Set_LTR_Intr_Mask(const boolean state);

__INLINE__ void HWhab_Set_RIF_Intr_Clr(const u_int8 clear_value);
__INLINE__ void HWhab_Set_LTR_Intr_Clr(const u_int8 value);

__INLINE__ boolean HWhab_Get_RIF_Intr_Mask0(void);
__INLINE__ boolean HWhab_Get_RIF_Intr_Mask1(void);
__INLINE__ boolean HWhab_Get_RIF_Intr_Mask2(void);
__INLINE__ boolean HWhab_Get_RIF_Intr_Mask3(void);
__INLINE__ boolean HWhab_Get_LTR_Intr_Mask(void);

__INLINE__ u_int8 HWhab_Get_RIF_Intr_Clr(void);
__INLINE__ u_int8 HWhab_Get_LTR_Intr_Clr(void);

/*
 * Interface Configuration Registers
 */
__INLINE__ void HWhab_Set_PHY_Cfg(const u_int32 magic_phy_value);

__INLINE__ u_int32 HWhab_Get_PHY_Cfg(void);

/*
 * Status Registers
 */

__INLINE__ u_int8 HWhab_Get_LTR_Long_Period(void); /* Trimming Comparison Indicator */
__INLINE__ u_int8 HWhab_Get_LTR_Intr(void);        /* LTR generated LPO Period Interrupt */
__INLINE__ u_int8 HWhab_Get_RIF_Intr(void);        /* RF Status Line Interrupt */
__INLINE__ u_int8 HWhab_Get_GIO_Status(void);

/*
 * GIO Control Registers
 */
__INLINE__ u_int8 HWhab_Set_GIO_Ctrl(const u_int8 index, const u_int8 value);


#ifdef __USE_INLINES__
#include "hw_habanero_impl.h"
#endif

#endif
