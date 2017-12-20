/*************************************************************************
 *
 * MODULE NAME:     tc_interface.c   
 * PROJECT CODE:    BlueStream
 * DESCRIPTION:     test control interface implementations
 * MAINTAINER:      Daire McNamara 
 * CREATION DATE:   27th January 2000     
 *
 * SOURCE CONTROL: $Id: tc_interface.c,v 1.2 2012/05/09 01:17:27 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2004 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *     27.Jan.2000     DMN    Initial Draft - support TCI_Read_Packet_Log
 *     01.Feb.2000     IG     Build Params, Active IRQ
 *     ??.???.2000     DMN    Added more functionality and methods
 *     22.Jun.2000     IG     compressed for loops to save code size
 *
 ************************************************************************/
#include "sys_config.h"

#include "hc_const.h"

//#include "dl_dev.h"

#include "lc_interface.h"
#include "lslc_stat.h"

#include "hci_params.h"

#include "hw_radio.h"
#include "tc_const.h"
#include "tc_event_gen.h"
#include "tc_interface.h"
#include "tc_types.h"

#include "tra_uart.h"

#include "sys_power.h"
#include "sys_irq.h"

#include "hw_lc.h"
#include "hc_event_gen.h"
#include "BK3431_reg.h"
#include "hw_radio_defs.h"

#if (PRH_BS_CFG_SYS_LOW_POWER_MODE_SUPPORTED == 1)
boolean SYSpwr_Set_LowFrequencyOscillatorAvailable_Value(u_int8 osc_available);
#endif
/*
 * Useful constants
 */
#define TC_SIZE_OF_EVENT_HEADER  6


#if defined(SHOGA) || defined(TERASIC)
u_int32 SYS_Main_Check_Unused_Stack_Space();
#endif

/***************************************************************************
 * TC_Write_Local_Hardware_Register
 *
 * Takes a relative address, a mask, and a value, and sets the
 * requested register to the requested value.
 *
 * Issues:
 * If this function is called to write a cached register, the register
 * value will be overwritten by the cached value. You should ensure
 * that the register being written to by this function is not
 * a cached register.
 *
 *
 ***************************************************************************/
void TC_Write_Local_Hardware_Register(u_int8 jal_or_hab, u_int32 address, u_int32 value, u_int32 mask)
{
    t_error status = COMMAND_DISALLOWED;
    if(!((jal_or_hab & ~1) || (address & 3)))
    {
        u_int32* reg_addr = (u_int32*)((u_int32)(jal_or_hab ? JAL_BASE_ADDR:HAB_BASE_ADDR) + address);
        u_int32 reg = *reg_addr;
        reg &= ~mask; /* invert mask and AND with current contents -- zero required part of reg */
        value &= mask; /* just to be sure that value is not outside mask */
        reg |= value;
        *reg_addr = reg; /* write back reg */ 
        status = NO_ERROR;
    }
    TCeg_Command_Complete_Event(TCI_WRITE_LOCAL_HARDWARE_REGISTER, status);
}

/***************************************************************************
 * TC_Read_Local_Hardware_Version
 *
 * This function reads back the current hardware version.
 *
 ***************************************************************************/
void TC_Read_Local_Hardware_Version(void)
{
    t_p_pdu p_buffer;
    t_q_descr *qd;
 
    qd = BTq_Enqueue(HCI_EVENT_Q, 0, 2 + TC_SIZE_OF_EVENT_HEADER);
    if(qd)
    {
        p_buffer = qd->data;
        _Insert_Uint8(p_buffer, HCI_COMMAND_COMPLETE_EVENT);
        _Insert_Uint8(p_buffer+1, 6);
        _Insert_Uint8(p_buffer+2, 1);
        _Insert_Uint16(p_buffer+3, TCI_READ_LOCAL_HARDWARE_VERSION); 
        _Insert_Uint8(p_buffer+5, 0); /* always good status */
        _Insert_Uint8(p_buffer+6, (u_int8)HW_get_major_revision());
        _Insert_Uint8(p_buffer+7, (u_int8)HW_get_minor_revision());
        BTq_Commit(HCI_EVENT_Q, 0);
    }
}

/***************************************************************************
 * TC_Read_Local_Baseband_Monitors
 *
 * This function retrieves the baseband monitors
 *
 ***************************************************************************/
void TC_Read_Local_Baseband_Monitors(void)
{
    u_int16 para_length;
    t_p_pdu p_buffer;
    t_q_descr *qd;
#if(PRH_BS_DBG_LC_LSLC_STAT==1)
    int i = 0;
#endif

#if(PRH_BS_DBG_LC_LSLC_STAT==1)
    u_int8 num_packet_types = MAX_PACKET_TYPES; /* either report EDR or BR only */    
#endif

    t_LLCmonitors* p_monitors = LSLCstat_Get_Monitors();
    para_length = sizeof(*p_monitors);
    qd = BTq_Enqueue(HCI_EVENT_Q, 0, (t_length)(para_length + TC_SIZE_OF_EVENT_HEADER));
    if(qd)
    {
        p_buffer = qd->data;
        _Insert_Uint8(p_buffer, HCI_COMMAND_COMPLETE_EVENT);
        _Insert_Uint8(p_buffer+1, (u_int8)(para_length+4));
        _Insert_Uint8(p_buffer+2, 1);
        _Insert_Uint16(p_buffer+3, TCI_READ_LOCAL_BASEBAND_MONITORS);
        _Insert_Uint8(p_buffer+5, 0); /* Always good status */
        _Insert_Uint32(p_buffer+6, p_monitors->RxValidPackets);
        _Insert_Uint32(p_buffer+10, p_monitors->RxInvalidPackets);
        _Insert_Uint32(p_buffer+14, p_monitors->RxNoPacketCntr);
        _Insert_Uint32(p_buffer+18, p_monitors->RxHECErrorCntr);
        _Insert_Uint32(p_buffer+22, p_monitors->RxCRCErrorCntr);
        _Insert_Uint32(p_buffer+26, p_monitors->RxDuplicateCntr);
#if(PRH_BS_DBG_LC_LSLC_STAT==1)
        _Insert_Uint32(p_buffer+30, p_monitors->RxRestrictedCntr);
        _Insert_Uint32(p_buffer+34, p_monitors->RxAMADDRErrorCntr);
#if (PRH_BS_CFG_SYS_LMP_EDR_SUPPORTED==1)
        for(i = 0; i < num_packet_types; i++)
            _Insert_Uint16(p_buffer+38+(i*2), p_monitors->TxPacketCntr[i]);
        for(i = 0; i < num_packet_types; i++)
            _Insert_Uint16(p_buffer+38+(num_packet_types*2)+(i*2), p_monitors->RxPacketCntr[i]);
        _Insert_Uint32(p_buffer+38+(num_packet_types*2*2), p_monitors->TxBroadcastCntr);
        _Insert_Uint32(p_buffer+38+(num_packet_types*2*2)+4, p_monitors->RxBroadcastCntr);
        _Insert_Uint32(p_buffer+38+(num_packet_types*2*2)+8, p_monitors->RxBroadcastDuplicateCntr);
        _Insert_Uint32(p_buffer+38+(num_packet_types*2*2)+12, p_monitors->TxAckSentCntr);
        _Insert_Uint32(p_buffer+38+(num_packet_types*2*2)+16, p_monitors->RxAckProcessedCntr);
#else /* use old DataTester, represent PacketCntrs as full 32 bits */
        for(i = 0; i < num_packet_types; i++)
            _Insert_Uint32(p_buffer+38+(i*4), p_monitors->TxPacketCntr[i]);
        for(i = 0; i < num_packet_types; i++)
            _Insert_Uint32(p_buffer+38+(num_packet_types*4)+(i*4), p_monitors->RxPacketCntr[i]);
        _Insert_Uint32(p_buffer+38+(num_packet_types*2*4), p_monitors->TxBroadcastCntr);
        _Insert_Uint32(p_buffer+38+(num_packet_types*2*4)+4, p_monitors->RxBroadcastCntr);
        _Insert_Uint32(p_buffer+38+(num_packet_types*2*4)+8, p_monitors->RxBroadcastDuplicateCntr);
        _Insert_Uint32(p_buffer+38+(num_packet_types*2*4)+12, p_monitors->TxAckSentCntr);
        _Insert_Uint32(p_buffer+38+(num_packet_types*2*4)+16, p_monitors->RxAckProcessedCntr);
#endif
#endif
        BTq_Commit(HCI_EVENT_Q, 0);
    }
}


/***************************************************************************
 * TC_Update_Manufacturing_Information
 *
 * This command allows the update of manufacturing information
 * to non-volatile storage such as FLASH.
 *
 ***************************************************************************/

void TC_Update_Manufacturing_Information(u_int8* pdu)
{
    t_q_descr *qd;
    u_int8 status;

#if defined(SHOGA)
    status = SYShalconfig_Store_Manufacturing_Information();
#else
    status = COMMAND_DISALLOWED;
#endif
            
    qd = BTq_Enqueue(HCI_EVENT_Q, 0, 6);
    if(qd)
    {
        qd->data[0] = HCI_COMMAND_COMPLETE_EVENT;
        qd->data[1] = 4;
        qd->data[2] = 1;
        _Insert_Uint16(&(qd->data[3]), TCI_UPDATE_MANUFACTURING_INFORMATION);
        qd->data[5] = status;
        BTq_Commit(HCI_EVENT_Q, 0);
    }
}

#if 0 // Not Required in LE SM
/***************************************************************************
 * TC_Set_HCIT_UART_Baud_Rate
 *
 * This command allows the setting of the local HCIT UART baud rate.
 * IMMEDIATE Option:
 * Immediate effect & command complete event will be sent at new baud rate.
 * Otherwise:
 * Flash update & cold reset are required for new baud rate to take effect.
 *
 ***************************************************************************/ 
void TC_Set_HCIT_UART_Baud_Rate(u_int32 baud_rate)
{
    t_q_descr *qd;
    t_error status;
    
#if (TRA_HCIT_UART_SUPPORTED==1)
    extern struct tra_hcit_info volatile *sys_hcit_info;
    if(sys_hcit_info->type==TRA_HCIT_TYPE_UART)
    {
#if defined(TRA_HCIT_UART_SET_BAUD_RATE_IMMEDIATE)
        TRAhcit_Shutdown();
#endif
#if 0 // Not Required in LE SM
        TRAhcit_UART_Set_Default_Baud_Rate(baud_rate);
#endif
#if defined(TRA_HCIT_UART_SET_BAUD_RATE_IMMEDIATE)
        TRAhcit_Initialise();
#endif
        status = NO_ERROR;
    }
    else
#endif
    {
#if (BUILD_TYPE==GENERIC_BUILD)
        TRAhcit_UART_Set_Default_Baud_Rate(baud_rate);
        status = NO_ERROR;
#else
        status = COMMAND_DISALLOWED;
#endif
    }
            
    qd = BTq_Enqueue(HCI_EVENT_Q, 0, 6);
    if(qd)
    {
        qd->data[0] = HCI_COMMAND_COMPLETE_EVENT;
        qd->data[1] = 4;
        qd->data[2] = 1;
        _Insert_Uint16(&(qd->data[3]), TCI_SET_HCIT_UART_BAUD_RATE);
        qd->data[5] = status;
        BTq_Commit(HCI_EVENT_Q, 0);
    }
}
#endif

/***************************************************************************
 * TC_Read_Unused_Stack_Space
 *
 ***************************************************************************/

void TC_Read_Unused_Stack_Space(void)
{
    u_int8 status = NO_ERROR;    
    u_int32 unused_stack = 0;
    t_q_descr *qd;
    
#if defined(SHOGA) || defined(TERASIC)
    unused_stack = SYS_Main_Check_Unused_Stack_Space();
#else
    status = COMMAND_DISALLOWED;
#endif
    
    qd = BTq_Enqueue(HCI_EVENT_Q, 0, 10);
    if(qd)
    {
        qd->data[0] = HCI_COMMAND_COMPLETE_EVENT;
        qd->data[1] = 8;
        qd->data[2] = 1;
        _Insert_Uint16(&(qd->data[3]), TCI_READ_UNUSED_STACK_SPACE);
        qd->data[5] = status;
        _Insert_Uint32(&(qd->data[6]), unused_stack);
        BTq_Commit(HCI_EVENT_Q, 0);
    }
}

/****ADD BEKEN Test Control */

void Beken_Write_Register(u_int32 addr,u_int32 dat)
{
    *(u_int32 *)addr = ( u_int32 )dat;
}

u_int32 Beken_Read_Register(u_int32 addr)
{
  return *(u_int32 *)addr;  
}


void Tc_Beken_Debug_UART_Access_Register(u_int8* pdu,u_int32 b_continuous)
{
//    t_p_pdu p_buffer;
    t_q_descr *qd;

    u_int32 addr;
    u_int32 v,len,index;
    //volatile unsigned int *p_APB_XVER_ADDR = (volatile unsigned int *) APB_XVER_BASE;

    u_int8 sub_cmd=pdu[3];//begin from byte third

    addr=HCIparam_Get_Uint32(pdu+4);

    if(sub_cmd==BEKEN_UART_REGISTER_READ_CMD)
    {
        if((addr>=0x00801400)&&(addr<=0x0080143C))
        {
            /* v=p_APB_XVER_ADDR[(addr-0x00801400)/4]; */
            v = backup_xvr_analog_reg[(addr-APB_XVER_BASE)/4];
        }
        else
        {
            v=Beken_Read_Register(addr);
        }
        
    }
    else
    {
        if(b_continuous)
        {
            index=1;
            len=pdu[8];
        }
        else
        {
            index=0;
            len=1;
        }

        while(len>0)
        {

            v=HCIparam_Get_Uint32(pdu+8+index);
            Beken_Write_Register(addr,v);

            if((addr>=0x00801400)&&(addr<=0x0080143c))
            {
                //p_APB_XVER_ADDR[(addr-APB_XVER_BASE)/4]=v;
                backup_xvr_analog_reg[(addr-APB_XVER_BASE)/4] = v;
            }

            addr+=4;
            index+=4;
            len-=1;
        }
        return;
    }

    qd = BTq_Enqueue(HCI_EVENT_Q, 0, 14);

    if(qd)
    {

        qd->data[0] = HCI_COMMAND_COMPLETE_EVENT;
        qd->data[1] = 12;
        qd->data[2] = 1;
        _Insert_Uint16(&(qd->data[3]), TCI_BEKEN_HARDWARE_TEST);
        qd->data[5] = sub_cmd;

        qd->data[6] = (addr>>0)&0xff;
        qd->data[7] = (addr>>8)&0xff;
        qd->data[8] = (addr>>16)&0xff;
        qd->data[9] = (addr>>24)&0xff;

        qd->data[10] = (v>>0)&0xff;
        qd->data[11] = (v>>8)&0xff;
        qd->data[12] = (v>>16)&0xff;
        qd->data[13] = (v>>24)&0xff;

        BTq_Commit(HCI_EVENT_Q, 0);
    }

}
void Tc_Beken_Debug_UART_Send_Link_Ok()
{    
    //t_p_pdu p_buffer;
    t_q_descr *qd;
 
    qd = BTq_Enqueue(HCI_EVENT_Q, 0, 8);
    if(qd)
    {
        qd->data[0] = HCI_COMMAND_COMPLETE_EVENT;
        qd->data[1] = 6;
        qd->data[2] = 1;
        _Insert_Uint16(&(qd->data[3]), TCI_BEKEN_HARDWARE_TEST);
        qd->data[5] = BEKEN_UART_LINK_CHECK;
        qd->data[6] = 0;
        qd->data[7] = 0;
        BTq_Commit(HCI_EVENT_Q, 0);
    }
}
void TC_Beken_Hardware_Test(u_int8* pdu)
{

    //u_int8 p_buffer[21];
    //t_q_descr *qd;
    
    u_int8 sub_cmd=pdu[3];//begin from byte third
    switch(sub_cmd)
    {
        case BEKEN_UART_LINK_CHECK:
           Tc_Beken_Debug_UART_Send_Link_Ok();                    
           break;                    
            
        case BEKEN_UART_REGISTER_WRITE_CMD:
        case BEKEN_UART_REGISTER_READ_CMD:
           Tc_Beken_Debug_UART_Access_Register(pdu,0);                    
           break;

        case BEKEN_UART_REGISTER_CONINUOUSE_WRITE_CMD:
            Tc_Beken_Debug_UART_Access_Register(pdu,1);                    
            break;

        //                case BEKEN_UART_BT_START_CMD:
        //                    Beken_Start_BlueTooth();
        //                    break;
        //                case BEKEN_UART_BT_STOP_CMD:
        //                    Beken_Stop_BlueTooth();
        //                    break;
//        case BEKEN_UART_PATCH_CMD:
//            patch_func(&pdu[4]);
//            break;

//            case BEKEN_UART_SET_PLATFORM_TYPE:
//             current_mobile_platform=  pdu[4];
//             SetPlatform();
//             break;

//             case BEKEN_UART_SET_UART_BAUDRATE:
//             g_uart_config.uart_clock_freq=HCIparam_Get_Uint32(pdu+4);

//             g_uart_config.baud_rate=HCIparam_Get_Uint32(pdu+8);

//             g_uart_config.par_en=pdu[12];
//             g_uart_config.par_mode=pdu[13];

//             Set_UART_Baudrate();
//             break;

//         case BEKEN_DISABLE_AFC:
//             g_disable_afc=pdu[4];
//             break;

//         case BEKEN_CONFIG_PCM:
//             g_beken_initial_config.VCI_Clk_Sel=pdu[4];  
//             g_beken_initial_config.VCI_Clk_Sel_Map=pdu[5]; 
//             g_beken_initial_config.g_sync_dir=pdu[6];   
//             break;

//         case BEKEN_SET_UART_LOW_LEVEL_WAKEUP_VALUE: 
//             g_beken_initial_config.uart_low_level_setting=HCIparam_Get_Uint32(pdu+4);
//             break;

//         case BEKEN_ENABLE_32K_SLEEP:
//             g_beken_initial_config.g_enable_32k_sleep=pdu[4];
//             break;

//         case BEKEN_ENABLE_HOST_WAKEUP:
//             g_beken_initial_config.g_enable_host_wakeup=pdu[4];
//             break;

//         case BEKEN_ENABLE_BT_WAKEUP:
//             g_beken_initial_config.g_enable_bt_wakeup=pdu[4];
//             break;

//         case BEKEN_ENABLE_UART_LOW_LEVEL_WAKEUP:
//             g_beken_initial_config.g_enable_uart_low_level_wakeup=pdu[4];
//             break;

//         case BEKEN_DISABLE_CPU_HALT_SLEEP:
//             g_beken_initial_config.g_disable_CPU_Halt_sleep=pdu[4];
//             break;

//         case BEKEN_DISABLE_ANALOG_POWERDOWN:
//             g_beken_initial_config.g_disable_Analog_Powerdown=pdu[4];
//             break;

//         case BEKEN_ENABLE_UART_TX_DELAY:
//             g_beken_initial_config.g_Enable_UART_TX_Delay=HCIparam_Get_Uint32(pdu+4);
//             break;

//         case BEKEN_MAX_ACL_BUFF_SIZE:
//             g_beken_initial_config.g_max_acl_buff_size=HCIparam_Get_Uint16(pdu+4);
//             break;

//         case BEKEN_ACL_ACTIVE_CHECK_WHEN_SLEEP:
//             g_beken_initial_config.g_Disable_ACL_active_check_when_Sleep=pdu[4];
//             break;

//         case BEKEN_DISABLE_SNIFFER_WHEN_OTHER_LINK_ACTIVE:
//             g_beken_initial_config.g_Disable_Sniffer_when_other_link_active=pdu[4];
//             break;

//         case BEKEN_LM_CONFIG_SLEEP_IN_STANDBY_MONITOR_PERIOD:
//             g_beken_initial_config.g_lm_config_sleep_in_standby_monitor_period=HCIparam_Get_Uint32(pdu+4);
//             break;

//         case BEKEN_LM_CONFIG_AWAKE_IN_STANDBY_MONITOR_PERIOD:
//             g_beken_initial_config.g_lm_config_awake_in_standby_monitor_period=HCIparam_Get_Uint32(pdu+4);
//             break;

//         case BEKEN_DISABLE_ACL_ACCEPT_WHEN_ACL_EXIST:
//             g_beken_initial_config.g_Disable_ACL_Accept_When_ACL_exist=pdu[4];
//             break;

//         case BEKEN_ENABLE_TX_POWER_CONTROL:
//             g_beken_initial_config.g_Enable_TX_Power_Control=pdu[4];
//             break;

//         case BEKEN_LM_CONFIG_AWAKE_KEEP_WHEN_UART_WAKEUP:
//             g_beken_initial_config.g_lm_config_awake_keep_when_UART_Wakeup_period=HCIparam_Get_Uint32(pdu+4);
//             break;

//         case BEKEN_DISABLE_INQUIRY_WHEN_ACL_EXIST:
//             g_beken_initial_config.g_Disable_Inquiry_When_ACL_exist=pdu[4];
//             break;

//         case BEKEN_DELAY_BETWEEN_EVERY_PACKET_UART_TX:
//             g_beken_initial_config.g_delay_Between_every_packet_uart_tx=HCIparam_Get_Uint16(pdu+4);
//             break;

//         case BEKEN_SCATTER_LENGTH_FOR_PACKET_UART_TX:
//             g_beken_initial_config.g_delay_scatter_length_for_packet_uart_tx=HCIparam_Get_Uint16(pdu+4);
//             break;


//         case BEKEN_ENABLE_ACK_SEQ_CHECK:
//             g_beken_initial_config.g_enable_ack_seq_check=pdu[4];
//             break;

//         case BEKEN_RESET_ACK_SEQ_AFTER_TX:
//             g_beken_initial_config.g_reset_ack_seq_after_tx=pdu[4];
//             break;

//         case BEKEN_GET_FW_VERSION:

//             p_buffer[0]=1;
//             p_buffer[1]=(TCI_BEKEN_HARDWARE_TEST)&0xff;
//             p_buffer[2]=(TCI_BEKEN_HARDWARE_TEST>>8)&0xff;

//             p_buffer[3]=BEKEN_GET_FW_VERSION;

//             memcpy(&p_buffer[4], BK3000_VERSION, 17);

//             TC_Beken_Send_General_To_Host(p_buffer,21);/*Robin modified Aug.13 2012*/

//             break;


//         case BEKEN_UART_CLOCK_CONFIG_BEFORE_TX_LOW_LEVEL:
//             g_beken_initial_config.g_uart_clock_config_before_tx_low_level=HCIparam_Get_Uint16(pdu+4);
//             break;

//         case BEKEN_UART_BAUD_RATE_FOR_TX_LOW_LEVEL:
//             g_beken_initial_config.g_uart_baud_rate_for_tx_low_level=HCIparam_Get_Uint16(pdu+4);
//             break;

//         case BEKEN_DELAY1_AFTER_TX_LOW_LEVEL:
//             g_beken_initial_config.g_uart_delay1_after_tx_low_level=HCIparam_Get_Uint16(pdu+4);
//             break;

//         case BEKEN_DELAY2_AFTER_TX_LOW_LEVEL:
//             g_beken_initial_config.g_uart_delay2_after_tx_low_level=HCIparam_Get_Uint16(pdu+4);
//             break;

//         case BEKEN_DELAY_FOR_OBEX_PACKET_FINAL:
//             g_beken_initial_config.g_delay_for_obex_packet_final=HCIparam_Get_Uint16(pdu+4);
//             break;

//         case BEKEN_DISABLE_SPREADTRUM_HCI:
//             g_beken_initial_config.disable_spreadtrum_hci=pdu[4];
//             break;

//         case BEKEN_MAX_ACL_BUFF_NUMBER:
//             g_beken_initial_config.g_max_acl_buff_number=pdu[4];
//             break;


//         case BEKEN_WRITE_ADDR_FOR_RDA_ENABLE:
//             g_beken_initial_config.g_write_addr_for_rda_enable=pdu[4];
//             break;
//         case BEKEN_PATCH_102_BYTE_NUM:
//             g_beken_initial_config.g_TCI_PATCH_102_BYTE_Number=pdu[4];
//             break;
//         case BEKEN_PATCH_102_BYTE_VALUE:
//            g_beken_initial_config.g_TCI_PATCH_102_BYTE_Value=pdu[4];
//             break;

//         case BEKEN_UART_BAUDRATE_FOR_SREADTRUM:
//             g_beken_initial_config.g_uart_baudrate_for_spreadtrum=HCIparam_Get_Uint32(pdu+4);
//             break;
//                     
//         case BEKEN_ENABLE_MASTER_AFC:
//             g_enable_master_AFC=pdu[4];
//             break;
//             
//         case BEKEN_ENABLE_VIMicro_ENCRYPTION_ISSUE:
//             g_beken_initial_config.g_enable_ViMicro_encryption_issue=pdu[4];
//             break;
//         case BEKEN_ENABLE_CPU_SPEED_FOR_ECC:
//             g_beken_initial_config.g_enable_CPU_speed_for_ecc=pdu[4];
//             break;

//         case BEKEN_CHANGE_CPU_CLK:
//             switch (pdu[4])
//             {
//                 case CPU_26M_CLK:
//                     Set_CPU_Speed_26M();
//                     break;
//                 case CPU_52M_CLK:
//                     Set_CPU_Speed_52M();
//                     break;
//                 case CPU_104M_CLK:
//                     Set_CPU_Speed_104M();
//                     break;
//             }
//             g_beken_initial_config.cur_cpu_clk = pdu[4];
//             break;

//         case BEKEN_ENABLE_CSR_TX_CRC:
//             g_beken_initial_config.enable_csr_tx_crc=pdu[4];
//             break;

//                 case SLEEP_FOR_ATE_POWER_TEST:
//                     if(pdu[4]==0x01)
//                         Force_Sleep_For_ATE_Power_Test();
//                     break;
//                     
//         case BEKEN_DISALBE_HAREWARE_ERROR_LOG:
//             g_beken_initial_config.Disable_Hardware_Error_Log=pdu[4];
//             break;


//         case BEKEN_SET_32K_WAKUP_TIME:
//             g_beken_initial_config.delay_when_wakeup_from_32K=HCIparam_Get_Uint16(pdu+4);
//             break;


//                 case BEKEN_SET_LMP_FEATURES:
//                     SYSconfig_Set_LMP_Features(&(pdu[4]));
//                     break;


//                 case BEKEN_SET_LMP_EXT_FEATURES:
//                     SYSconfig_Set_Ext_Features(&(pdu[4]));
//                     break;


//                 case BEKEN_DISALBE_2DH1_WHEN_AUTORATE:
//                     g_beken_initial_config.disable_2DH1_when_Autorate=pdu[4];
//                     break;

//                 case BEKEN_FORCE_DM1_WHEN_LITTLE_PACKET:
//                     g_beken_initial_config.force_DM1_when_little_packet=pdu[4];
//                     break;



//                 case BEKEN_DISALBE_EDR3:
// //                    b_disable_edr3=pdu[4];
//                     break;
//                 
//                 case BEKEN_CFG_MIN_SLOTS_FOR_SLEEP_PROCEDURE:
//                     g_beken_initial_config.sys_cfg_min_slots_for_sleep_procedure=HCIparam_Get_Uint16(pdu+4);
//                     break;
//                 case BEKEN_DISABLE_ESCO:
//                     g_beken_initial_config.disable_esco=pdu[4];
//                     break;

//                 case BEKEN_DELAY_PTT_SET:
//                     g_beken_initial_config.Delay_Ptt_Set=pdu[4];
//                     break;

//                 case BEKEN_SET_HOST_WAKEUP_TIME:
//                     g_beken_initial_config.host_wakeup_time=HCIparam_Get_Uint32(pdu+4);
//                     break;

//         case BEKEN_BT_ACTIVE_PIN_SEL:
//             g_beken_initial_config.BT_Active_Pin_Sel=pdu[4];
//             break;

//         case BEKEN_BT_PRIORITY_PIN_SEL:
//             g_beken_initial_config.BT_Priority_Pin_Sel=pdu[4];
//             break;

//         case BEKEN_WLAN_ACTIVE_PIN_SEL:
//             g_beken_initial_config.BT_Wlan_Active_Pin_Sel=pdu[4];
//             break;

//         case BEKEN_WLAN_ACTIVE_PIN_POL:
//             g_beken_initial_config.BT_Wlan_Active_Pin_Pol=pdu[4];
//             break;
// 	
//         case BEKEN_ENABLE_SOFTIRQ_FOR_UART:
// //            g_beken_initial_config.Enable_SoftIRQ_For_UART=pdu[4];
//             break;


//         case BEKEN_SET_T_SNIFF_MIN:
//             g_beken_initial_config.default_T_sniff_min=HCIparam_Get_Uint16(pdu+4);
//             break;

//         case BEKEN_SET_T_SNIFF_MAX:
//             g_beken_initial_config.default_T_sniff_max=HCIparam_Get_Uint16(pdu+4);
//             break;

//         case BEKEN_CHANGE_SNIFF_ATTEMP_WHEN_SNIFF_MIN_IS_SMALL:
//             g_beken_initial_config.change_sniff_attemp_when_sniff_min_is_small=pdu[4];
//             break;

//         case BEKEN_CHANGE_SNIFF_TIMEOUT_WHEN_SNIFF_MIN_IS_SMALL:
//             g_beken_initial_config.change_sniff_timeout_when_sniff_min_is_small=pdu[4];
//             break;

//         case BEKEN_PTA_TX_DELAY_AFTER_PRIORITY:
//             g_beken_initial_config.PTA_TX_Delay_After_Priority=HCIparam_Get_Uint16(pdu+4);
//             break;

//         case BEKEN_PTA_TX_DELAY_AFTER_FREQ_OVERLAP:
//             g_beken_initial_config.PTA_TX_Delay_After_Freq_Overlap=HCIparam_Get_Uint16(pdu+4);
//             break;

//         case BEKEN_PTA_RX_DELAY_AFTER_PRIORITY:
//             g_beken_initial_config.PTA_RX_Delay_After_Priority=HCIparam_Get_Uint16(pdu+4);
//             break;


//         case BEKEN_ENABLE_PTA:
//             g_beken_initial_config.Enable_PTA=pdu[4];

//             if(g_beken_initial_config.Enable_PTA)
//     	        HWpta_Initialise();

//             BK3000_PTA_GPIO_Initial();

//             break;

//         case BEKEN_ENABLE_CPU_SPEED_FOR_ECC2:
//             g_beken_initial_config.g_enable_CPU_speed_for_ecc2=pdu[4];
//             break;

//         case BEKEN_SPEED_UART_CRC:
//             g_beken_initial_config.speed_uart_crc=pdu[4];
//             break;






    }

    
}







