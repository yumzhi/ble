#ifndef _PARTHUS_SYS_CONFIG_IMPL_
#define _PARTHUS_SYS_CONFIG_IMPL_

/***********************************************************************
 *
 * MODULE NAME:    sys_config_impl.h
 * PROJECT CODE:    BlueStream
 * DESCRIPTION:    t_SYS_Config Wrapper Implementation
 * MAINTAINER:     Ivan Griffin
 * DATE:           25 March 2000
 *
 * SOURCE CONTROL: $Id: sys_config_impl.h,v 1.1 2012/05/09 00:10:15 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2004 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    07 January 2000 -   IG       - first version
 *
 *
 ***********************************************************************/
#include "sys_config.h"
#include "hci_params.h"



__INLINE__ const t_bd_addr *SYSconfig_Get_Local_BD_Addr_Ref(void)
{
    extern t_SYS_Config g_sys_config;
    return &(g_sys_config.local_device_address);
}


__INLINE__ void SYSconfig_Set_Local_BD_Addr(t_bd_addr bd_addr)
{
    extern t_SYS_Config g_sys_config;
    int i;

    for(i = 0; i < 6; i++)
    {
        g_sys_config.local_device_address.bytes[i] = bd_addr.bytes[i];
    }
    SYShalconfig_BD_ADDR_Was_Set();
}

#if 1//(PRH_BS_CFG_SYS_LE_GAP_INCLUDED!=1)//merge CEVA 0517
/*****************************************************************************
 * SYSconfig_Get_HC_Commands_Ref
 *
 * Returns a reference to the hci command set 
 * (always the first 64 bytes in command array) 
 *****************************************************************************/
__INLINE__ u_int8* SYSconfig_Get_HC_Commands_Ref(void)//merge CEVA 0517
{
    extern t_SYS_Config g_sys_config;//merge CEVA 0517
    return g_sys_config.hci_command_set;//merge CEVA 0517
}

#endif//merge CEVA 0517
/*****************************************************************************
 * SYSconfig_Get_Version_Info_Ref
 *
 * Returns a reference to the version info
 *****************************************************************************/
__INLINE__ const t_versionInfo* SYSconfig_Get_Version_Info_Ref(void)
{
    extern t_SYS_Config g_sys_config;
    return &g_sys_config.version_info;
}

/*****************************************************************************
 * SYSconfig_Get_HC_Buffer_Size_Ref
 *
 * Returns a reference to the Host Controller ACL/SCO Buffer Sizes/Number of
 *****************************************************************************/
__INLINE__ const t_bufferSize* SYSconfig_Get_HC_Buffer_Size_Ref(void)
{
    extern t_SYS_Config g_sys_config;
    return &g_sys_config.hc_buffer_size;
}
#if 0
__INLINE__ u_int32 SYSconfig_Get_Data_Whitening_Enable(void)
{
    extern t_SYS_Config g_sys_config;
    return g_sys_config.data_whitening_enable;
}

__INLINE__ void SYSconfig_Set_Data_Whitening_Enable(boolean dwh_enable)
{
    extern t_SYS_Config g_sys_config;
    g_sys_config.data_whitening_enable = dwh_enable;
}
#endif
__INLINE__ u_int32 SYSconfig_Get_Win_Ext(void)
{
    extern t_SYS_Config g_sys_config;
    return g_sys_config.win_ext;
}

/*****************************************************************************
 * SYSconfig_Get_Local_Device_Index
 *
 * Gets the local device index. 
 *****************************************************************************/
__INLINE__ u_int16 SYSconfig_Get_Local_Device_Index(void)
{
    extern t_SYS_Config g_sys_config;
    return g_sys_config.local_device_index;
}

/*****************************************************************************
 * SYSconfig_Set_Local_Device_Index
 *
 * Sets the local device index. 
 *****************************************************************************/
__INLINE__ void SYSconfig_Set_Local_Device_Index(u_int16 local_device_index)
{
    extern t_SYS_Config g_sys_config;
    g_sys_config.local_device_index = local_device_index;
}


/*****************************************************************************
 * SYSconfig_Get_Max_Active_Devices
 *
 * Gets the maximum number of active devices supported. 
 *****************************************************************************/
__INLINE__ u_int16 SYSconfig_Get_Max_Active_Devices(void)
{
    extern t_SYS_Config g_sys_config;
    return g_sys_config.max_active_devices;
}

/*****************************************************************************
 * SYSconfig_Set_Max_Active_Devices
 *
 * Sets the maximum number of active devices supported. 
 *****************************************************************************/
__INLINE__ void SYSconfig_Set_Max_Active_Devices(u_int16 max_active_devices)
{
    extern t_SYS_Config g_sys_config;
    g_sys_config.max_active_devices = max_active_devices;
}

/*****************************************************************************
 * SYSconfig_Get_SYS_Rand_Seed
 *
 * Gets the random number seed
 *****************************************************************************/
__INLINE__ u_int32 SYSconfig_Get_SYS_Rand_Seed(void)
{
    extern t_SYS_Config g_sys_config;
    return g_sys_config.rand_seed;
}

#endif
