/**************************************************************************
 * MODULE NAME:    le_RTC_patch.c       
 * DESCRIPTION:    For external 32K patch
 * AUTHOR:         Charles
 * DATE:           
 *
 *
 * LICENSE:
 *     This source code is copyright (c) 2011-2012 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *
 * NOTES:
 *
 **************************************************************************/
#include "sys_features.h"
#include "sys_mmi_impl.h"
#include "le_advertise.h"
#include "hw_radio_defs.h"
#include "le_gap_const.h"
#include "BKAPI.h"
#include "lslc_irq.h"
#include "hw_jal_defs.h"
#include "le_config.h"
#include "le_const.h"
#include "sys_power.h"
#include "hw_lc_impl.h"
#include "debug.h"

void LE_conn_update_set_win_ext(void)
{
	//HW_set_win_ext(SYSconfig_Get_Win_Ext()+300);
}
