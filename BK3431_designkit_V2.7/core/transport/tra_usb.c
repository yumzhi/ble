
/*
 * MODULE NAME: 
 * PROJECT CODE: 
 * DESCRIPTION: usb physical layer transport module.
 * AUTHOR:        
 * DATE:        
 * 
 * SOURCE CONTROL: $Id: tra_usb.c,v 1.8 2008/07/03 12:26:22 tomk Exp $
 *
 * LICENCE:    Copyright (c) 2000-2004 Ceva Inc.
 *     All rights reserved.
 *
 */
 
#include "sys_config.h"

#if (BUILD_TYPE!=GENERIC_BUILD)

#include "tra_queue.h"    
#include "tra_hcit.h"
#include "tra_usb.h"
struct tra_hcit_usb_info sys_hcit_usb_info;

static u_int32 tra_hcit_usb_tx_original_length;

void TRAhcit_USB_Initialise(void)
{
    /*
     * Initialise layer variables
     * For rev3 of wasabi board this function is called by GPIOA1 interrupt.
     */
    /*
     * Clear the busy flag
     */
    sys_hcit_usb_info.common.tx_busy = 0;

}


void TRAhcit_USB_Transmit (u_int8 *data, u_int32 pdu_len, u_int8 *header, 
                           u_int8 head_len, u_int8 tra_source, 
                           t_deviceIndex device_index)
{
	sys_hcit_usb_info.common.tx_busy = 1;

    sys_hcit_usb_info.common.tx_device_index = device_index;
    sys_hcit_usb_info.common.tx_tra_source = tra_source;

	/* 
     * Original length of PDU (minus header) for ACKing the PDU from the queue
     */
	tra_hcit_usb_tx_original_length = pdu_len;

	/*
	** the first byte of the header (HCI packet indicator byte) is removed 
	** for USB. If the parameter 'head_len' was to be used here it would also 
	** need to be decreased by one. 
	*/
	sys_hcit_usb_info.transmit(data,pdu_len,(header+1),tra_source,device_index);
}

void TRAhcit_USB_Shutdown(void)
{
    sys_hcit_usb_info.shutdown();
}

void TRAhcit_USB_Acknowledge_Complete_Tx(void)
{
	TRAhcit_Generic_Acknowledge_Complete_Tx(
		sys_hcit_usb_info.common.tx_tra_source, 
		tra_hcit_usb_tx_original_length); 
	sys_hcit_usb_info.common.tx_busy = 0;
}

#endif
