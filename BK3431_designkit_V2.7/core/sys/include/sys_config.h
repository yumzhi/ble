#ifndef _PARTHUS_SYS_CONFIG_
#define _PARTHUS_SYS_CONFIG_

/***********************************************************************
 *
 * MODULE NAME:    sys_config.h
 * PROJECT CODE:   BlueStream
 * MAINTAINER:     Ivan Griffin
 * DATE:           Wed Jul 14 11:39:10 BST 1999
 *
 * SOURCE CONTROL: $Id: sys_config.h,v 1.2 2012/05/14 23:14:15 garyf Exp $
 *
 * LICENCE:
 *    This source code is copyright (c) 1999-2004 Ceva Inc.
 *    All rights reserved.
 *
 * REVISION HISTORY:
 *    14 Jul 1999 -         IG      - Macro defines for complete system.
 *     8 Mar 2001 -         JS      - Seperated hal and core defines (each HAL
 *                                    has its own sys_hal_config.h
 *    Jul/Aug 2001 -       JN       - Cleanup of sys_config for configuration
 *
 * Configuration is based on 5 major files 
 *
 * 1. Core configurable Features (including LMP) sys_features.h
 * 2. HAL configurable                           sys_hal_config.h
 * 3. Debugging options for components/modules   sys_debug.h
 * 4. Development options (new code selectiion)  sys_develop.h
 * 5. General configuration options              sys_config.h
 *
 * This file abstracts all others and is the only file that should be
 * included in the component modules within the system.
 *
 ***********************************************************************/

/*
 * System wide types
 */
#include "sys_types.h"

/*
 * System wide constants
 */
#include "sys_const.h"

/*
 * Generated Features file from tool prh_bs_config
 */
#if defined(SHOGA)
#include "prh_bs_features.h"
#endif
/*
 * Generic Features
 */
#include "sys_features.h"


/*
 * Platform specific features 
 */
#include "sys_hal_features.h"

#include "sys_debug.h"

/*
 * Include development header file at end so that all can be overridden
 */

/*
 * Include HAL specific config file
 */
#include "sys_hal_config.h"

/*
 * The following constants are platform specific
 * and will normally be defined in sys_hal_config.h in the 
 * individual hal subdirectories.
 */

#ifndef __INLINE__
    #define __INLINE__
    #undef __USE_INLINES__
#endif   

#ifndef __COMPILER_VERSION__
    #define __COMPILER_VERSION__ "unknown compiler"
#endif

#ifndef __IRQ__   
    #define __IRQ__  
#endif

/*
 * For some compilers, inlines are handled via pragmas, ignore if not defined
 */
#ifndef PRAGMA_INLINE
#define PRAGMA_INLINE 0
#endif


/*
 * Macro to print version & date and time of compilation
 */
//#define VERSION "(\"Ceva XpertBlue\" - Bluetooth Host Controller Stack - "__DATE__" - "__TIME__")"


/*
 * Include development overrides at end so that all can be overridden
 */
//#include "sys_develop.h"

/***********************************************************************
 * t_SYS_Config
 *
 * The system configuration data structure
 *
 * Contains the global wide system configuration variables.
 ***********************************************************************/

typedef struct
{
    t_bd_addr local_device_address;

/*
 * Basic configuration
 */
    u_int32 data_whitening_enable;
    u_int32 win_ext;
#if 1//(PRH_BS_CFG_SYS_LE_GAP_INCLUDED!=1)//merge CEVA 0517
	u_int8 hci_command_set[64];			//merge CEVA 0517/* HCI Supported Command Set */
#endif//merge CEVA 0517
    t_versionInfo version_info;         /* HC & LMP Version Information */
    t_bufferSize  hc_buffer_size;       /* HCI ( Queue ) Buffer size    */
#if (PRH_BS_CFG_SYS_FLEXIBLE_DATA_BUFFER_SIZES_SUPPORTED==1)
    t_bufferSize  hc_buffer_size_in;    /* Buffer size on incoming data */
#endif
 
    u_int16  max_active_devices;
    u_int16 local_device_index;
    u_int32 rand_seed;                  /* Seed for random number       */
} t_SYS_Config;


void SYSconfig_Initialise(void);

__INLINE__ t_syncword        SYSconfig_Get_Local_Syncword(void);
__INLINE__ const t_syncword* SYSconfig_Get_Local_Syncword_Ref(void);
__INLINE__ void SYSconfig_Get_Local_Syncword_Ex(t_syncword* p_syncword);
__INLINE__ void SYSconfig_Set_Local_Syncword(t_syncword syncword);

__INLINE__ t_bd_addr SYSconfig_Get_Local_BD_Addr(void);
__INLINE__ const t_bd_addr *SYSconfig_Get_Local_BD_Addr_Ref(void);
__INLINE__ void SYSconfig_Get_Local_BD_Addr_Ex(t_bd_addr *addr);
__INLINE__ void SYSconfig_Get_Local_BD_Addr_U32(u_int32* addr);

__INLINE__ void SYSconfig_Set_Local_BD_Addr(t_bd_addr bd_addr);

__INLINE__ u_int8* SYSconfig_Get_HC_Commands_Ref(void);

__INLINE__ boolean SYSconfig_Is_Feature_Supported(u_int32 feature);
__INLINE__ boolean SYSconfig_Is_Command_Supported(u_int32 command);
__INLINE__ void    SYSconfig_Disable_Feature(u_int32 feature);
__INLINE__ void    SYSconfig_Enable_Feature(u_int32 feature);

__INLINE__ u_int8* SYSconfig_Get_Ext_Features_Ref(void);
__INLINE__ void    SYSconfig_Set_Ext_Features(const u_int8 ext_features_array[]);

__INLINE__ const t_versionInfo* SYSconfig_Get_Version_Info_Ref(void);
__INLINE__ const t_bufferSize* SYSconfig_Get_HC_Buffer_Size_Ref(void);

__INLINE__ u_int32 SYSconfig_Get_Data_Whitening_Enable(void);
__INLINE__ void SYSconfig_Set_Data_Whitening_Enable(boolean dwh_enable);

__INLINE__ u_int32 SYSconfig_Get_Tx_Freq(void);
__INLINE__ u_int32 SYSconfig_Get_Rx_Freq(void);
__INLINE__ u_int32 SYSconfig_Get_Win_Ext(void);

__INLINE__ u_int8 SYSconfig_Get_Clock_Jitter(void);
__INLINE__ void   SYSconfig_Set_Clock_Jitter(u_int8 jitter);
__INLINE__ u_int8 SYSconfig_Get_Clock_Drift(void);
__INLINE__ void   SYSconfig_Set_Clock_Drift(u_int8 drift);

__INLINE__ u_int16 SYSconfig_Get_Local_Device_Index(void);
__INLINE__ void SYSconfig_Set_Local_Device_Index(u_int16 local_device_index);

__INLINE__ u_int16 SYSconfig_Get_Max_Active_Devices(void);
__INLINE__ void SYSconfig_Set_Max_Active_Devices(u_int16 max_active_devices);

__INLINE__ void* SYSconfig_Get_Device_Links_Location(void);
__INLINE__ void SYSconfig_Set_Device_Links_Location(void* pDL);

__INLINE__ u_int32 SYSconfig_Get_SYS_Rand_Seed(void);

void erase_nvr(u_int32 addr);
void read_nvr(u_int32 addr, u_int32 *pData, u_int32 len);
void write_nvr(u_int32 addr, u_int32 *pData, u_int32 len);
void FlashStorageInit(void);

#define MEM_BUF_LEN     (sizeof(t_sys_vars)+4*(sizeof(t_sys_vars)%4?1:0))
#define MEM_START_ADDR 0x7F00   //use the last 1K mem for save config data

typedef struct 
{
    t_bd_addr  bt_addr;
    u_int16     reserve1;
    t_bd_addr  test_addr;
    u_int16     reserve2;
    u_int8      bt_name[16];
}t_sys_vars;

extern t_sys_vars *g_sys_vars;

#ifdef __USE_INLINES__
#include "sys_config_impl.h"
#endif

#endif

