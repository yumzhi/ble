#ifndef _PARTHUS_HC_FLOW_CONTROL
#define _PARTHUS_HC_FLOW_CONTROL

/**************************************************************************
 * MODULE NAME:    hc_flow_control.h
 * PROJECT CODE:   BlueStream
 * DESCRIPTION:    Host Controller to Host Flow Control
 * AUTHOR:         John Nelson
 * DATE:           03-01-2002
 *
 * LICENSE:
 *     This source code is copyright (c) 2002-2004 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    02-02-2002 -   JN Initial Version
 *
 * SOURCE CONTROL: $Id $:
 * 
 *************************************************************************/

#include "tra_queue.h"

/*
 * Constants
 */
#define HCfc_COMPLETED_DATA_PACKETS_TIMEOUT_DEFAULT_SLOTS 400 /*0.25 seconds*/
#define HCfc_COMPLETED_ACL_DATA_PACKETS_REPORT_THRESHOLD 1
#define HCfc_COMPLETED_SYN_DATA_PACKETS_REPORT_THRESHOLD 1

/*
 * Flow control enable values for HC to Host Flow
 */
#define HC_TO_HOST_FLOW_CONTROL_OFF     0x00
#define HC_TO_HOST_ACL_FLOW_CONTROL     0x01
#define HC_TO_HOST_SYN_FLOW_CONTROL     0x02
#define HC_TO_HOST_BOTH_FLOW_CONTROL    0x03

/*
 * Flow control persistent variables structure
 */
typedef struct s_hc_flow_crtl
{
    /*
     * Host to Host Controller flow control parameters
     */
    t_timer  hc_completed_data_packets_report_timer;

}  t_hc_flow_ctrl;

/*
 * General operations
 */
void    HCfc_Initialise(void);
void    HCfc_Handle_Host_To_Host_Controller_Flow_Control(void);

t_q_descr *HCfc_Get_Next_HC_to_Host_Data_Descriptor(t_deviceIndex device_index);
void    HCfc_Release_All_Data_Queue_Packets(t_deviceIndex device_index);

#endif /*_PARTHUS_HC_FLOW_CONTROL*/

