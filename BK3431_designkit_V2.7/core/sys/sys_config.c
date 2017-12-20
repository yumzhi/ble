/***********************************************************************
 *
 * MODULE NAME:    sys_config.c
 * PROJECT CODE:    BlueStream
 * DESCRIPTION:    t_SYS_Config Wrapper Implementation
 * MAINTAINER:     Ivan Griffin
 * DATE:           25 March 2000
 *
 * SOURCE CONTROL: $Id: sys_config.c,v 1.1 2012/05/09 00:10:13 garyf Exp $
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

/*
 * System configuration structure definition
 */
t_SYS_Config g_sys_config /*= { 0 }*/;

extern void SYShal_config_Initialise(void);

/*****************************************************************************
 * SYSconfig_Initialise
 *
 * Set up the major system configuration structure g_sys_config
 ****************************************************************************/
void SYSconfig_Initialise(void)
{

#if 1//(PRH_BS_CFG_SYS_LE_GAP_INCLUDED!=1)//merge CEVA 0517
    u_int i;//merge CEVA 0517

    u_int8* hci_command_bytes = g_sys_config.hci_command_set;//merge CEVA 0517

    for(i=0;i<64;i++)//merge CEVA 0517
        hci_command_bytes[i]=0x00;//merge CEVA 0517

    /*
     * Record the compile time features LMP in Bytes[0:7], PRH in Bytes [8:15]
     * Features then can be selectively be Run-Time enabled/disabled.
     */
    /*
     * Filling hci_command_bytes[]//merge CEVA 0517
     */
    hci_command_bytes[0] = 0x20; //merge CEVA 0517// HCI_Disconect

    hci_command_bytes[2] = 0xA0; //merge CEVA 0517// HCI_Read_Remote_Suported_Features & HCI_Read_Remote_Version_Info

    hci_command_bytes[5] = 0xC0; //merge CEVA 0517// HCI_Set_Event_Mask & HCI_Reset

    //hci_command_bytes[10] = 0x00; //For SM -
    /* For Dual mode (PRH_BS_CFG_SYS_HC_TO_HOST_FLOW_CONTROL_SUPPORTED << 5) |
       (PRH_BS_CFG_SYS_HC_TO_HOST_FLOW_CONTROL_SUPPORTED << 6) |
       (PRH_BS_CFG_SYS_HC_TO_HOST_FLOW_CONTROL_SUPPORTED << 7); */

    hci_command_bytes[14] = 0xA8;//merge CEVA 0517
    hci_command_bytes[15] = 0x22;//merge CEVA 0517

    hci_command_bytes[25] = 0xF7;//merge CEVA 0517
    hci_command_bytes[26] = 0xFF;//merge CEVA 0517
    hci_command_bytes[27] = 0xFF;//merge CEVA 0517
    hci_command_bytes[28] = 0x7F;//merge CEVA 0517

#endif
    /*
     * LM & HC Version Information
     * HCI_revision is 1, for spec compliant flow control on comp_id 14
     */
    g_sys_config.version_info.lmp_version = PRH_BS_CFG_LMP_VERSION;
    g_sys_config.version_info.lmp_subversion = PRH_BS_CFG_LMP_SUBVERSION;
    g_sys_config.version_info.comp_id = PRH_BS_CFG_MANUFACTURER_NAME;
    g_sys_config.version_info.HCI_revision = PRH_BS_CFG_HCI_REVISION;
    g_sys_config.version_info.HCI_version = PRH_BS_CFG_HCI_VERSION;

    /*
     * HC ACL and SCO Buffer Information
     */
    g_sys_config.hc_buffer_size.aclDataPacketLength =
        (PRH_BS_CFG_SYS_ACL_OUT_PACKET_LENGTH-4); // -4 To accmodate MIC

#if (PRH_BS_CFG_SYS_FLEXIBLE_DATA_BUFFER_SIZES_SUPPORTED==0)
    /*
     * Setup fixed buffer sizes for outgoing SCO/ACL packets.
     */
    g_sys_config.hc_buffer_size.numAclDataPackets =
        PRH_BS_CFG_SYS_NUM_OUT_ACL_PACKETS;
#endif

    /*
     * Default Whitening is On except for SINGLE frequency
     */
#if (PRH_BS_DEV_WIRED_LINK==0)
    g_sys_config.data_whitening_enable = 1 ;//(g_sys_config.hopping_mode != SINGLE_FREQ);
#else
    g_sys_config.data_whitening_enable = 0;
#endif

    /*
     * Tabasco IP (Jal-G3, Hab-F26) has its correlation window
     * centered 10us early when acting as a slave - compensating
     * for this by making win_ext 10
     */
#if (PRH_BS_CFG_SYS_LOW_POWER_MODE_SUPPORTED==1)
    /* g_sys_config.win_ext = 300; */
    g_sys_config.win_ext = 60;
#else
    g_sys_config.win_ext = 50;
#endif
    //g_sys_config.win_ext = 500;
    //g_sys_config.win_ext = 40;//org
    //g_sys_config.win_ext = 50;//this parameter can connection with Iphone is pass but this value too big ,maybe smaller this
    //g_sys_config.win_ext = 60;

    /*
     * Set the default SYSrand seed value
     */
    g_sys_config.rand_seed = 0;


    /*
     * Now do customer specific initialisation
     */
    SYShal_config_Initialise();
    g_sys_config.max_active_devices = MAX_ACTIVE_DEVICE_LINKS;

}

