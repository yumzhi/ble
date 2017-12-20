/*
 * MODULE NAME: tra_hcit_combined_transport
 * PROJECT CODE: BlueStream
 * DESCRIPTION: combined stack transport modules
 * AUTHOR:        Dave Airlie
 * DATE:        22/11/2000
 * 
 * SOURCE CONTROL: $Id: tra_combined.c,v 1.1 2012/05/09 00:10:22 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2004 Ceva Inc
 *     All rights reserved.
 *
 */

#include "sys_config.h"

#if PRH_BT_COMBINED == 1

#include "tra_queue.h"    
#include "tra_hcit.h"
#include "tra_combined.h"

#include "host_config.h"
#include "host_buf.h"
#include "papi.h"

struct tra_hcit_combined_info sys_hcit_combined_info;

/*
 * Transmit Variables
 */
void *TRAhcit_Register_COMBINED(void)
{
    sys_hcit_combined_info.common.type = TRA_HCIT_TYPE_COMBINED;
    return (void *)&sys_hcit_combined_info;
}



/*
 * Function: TRAhcit_COMBINED_Initialise
 * Paramters: 
 *
 * Returns: 
 * 
 * 
 * This function initialises the UART sublayer of the HCI Transport subsystem.
 * Pointers are assigned etc..
 */

void TRAhcit_COMBINED_Initialise(void)
{
    /*
     * Clear the busy flag
     */
    sys_hcit_combined_info.common.tx_busy = 0;
}

/*
 * Function: TRAhcit_COMBINED_Shutdown
 *
 */

void TRAhcit_COMBINED_Shutdown(void)
{

}



/*
 * Function: TRAhcit_COMBINED_Tx_Block
 * Paramters: 
 * data  : pointer to the data 
 * length: length of data
 * queue : queue ID to commit to (e.g. HCI_COMMAND_QUEUE )
 * device index : device index to commit to.
 *
 */
extern t_pEvent hci_transport_combined_event;
struct host_buf *hci_transport_combined_pdu;
int hci_transport_combined_type;

void TRAhcit_COMBINED_Transmit(u_int8 *data, u_int32 pdu_len, u_int8 *header, u_int8 head_len, u_int8 tra_source, t_deviceIndex device_index)
{
  /* Got to send the pdu to the host thread */
  struct host_buf *pdu;
  int total_len = pdu_len+head_len-1;
  int h_len = head_len-1;
  
  /* these are correct and must be set, even if it looks like 
      we don't use them later */
  sys_hcit_combined_info.common.tx_tra_source=tra_source;
  sys_hcit_combined_info.common.tx_device_index=device_index;

  sys_hcit_combined_info.common.tx_busy = 1;
  hci_transport_combined_type=header[0];
  pdu=host_buf_alloc(total_len);

  pMemcpy(pdu->data, header+1, h_len);
  pMemcpy(&pdu->data[h_len], data, pdu_len);
  pdu->len = total_len;

  hci_transport_combined_pdu = pdu;
  pDebugPrintf((pLOGDEBUG, "TRAC: to host %d\n", pdu_len));
  TRAhcit_Generic_Acknowledge_Complete_Tx(tra_source, pdu_len);  
  pEventSet(hci_transport_combined_event);



}

#else
void __dummy(void);

#endif /* PRH_BT_COMBINED */
