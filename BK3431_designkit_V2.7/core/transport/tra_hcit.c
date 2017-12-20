/*****************************************************************************
 * MODULE NAME:    tra_hcit.c
 * PROJECT CODE:   Bluetooth
 * DESCRIPTION:    HCI Generic Transport Interface
 * MAINTAINER:     Tom Kerwick
 * CREATION DATE:  19 April 2000
 *
 * SOURCE CONTROL: $Id: tra_hcit.c,v 1.2 2012/05/14 23:14:23 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2004 Ceva Inc
 *     All rights reserved.
 *
 * NOTES TO USERS: <none>
 * 
 *
 *****************************************************************************/
#include "sys_config.h"

#include "tra_hcit.h"
#include "tra_uart.h"
#include "tra_usb.h"
#include "tra_queue.h"

#include "lc_interface.h"

#include "hc_flow_control.h"
#include "hc_event_gen.h"
#include "hc_const.h"

#include "sys_mmi.h"
#if 1// (LE_INCLUDED == 1)
#include "le_config.h"
#include "le_security.h"
#include "le_connection.h"
#include "LE_link_layer.h"//merge CEVA 0517
#if (PRH_BS_CFG_SYS_LE_SMP_INCLUDED==1)
#include "le_smp.h"
#endif
#if (PRH_BS_CFG_SYS_LE_GATT_INCLUDED==1)//merge CEVA 0517
#include "le_att.h"//merge CEVA 0517
#endif//merge CEVA 0517
#endif
#ifndef __USE_INLINES__
#include "tra_hcit_impl.h"
#endif
#if (PRH_BS_CFG_SYS_LE_L2CAP_INCLUDED==1)//merge CEVA 0517
#include "le_l2cap.h"//merge CEVA 0517
#endif//merge CEVA 0517
#include "debug.h"

extern t_LE_Config LE_config;
#ifdef ATT_CNT_TEST
volatile u_int8 ATT_cmd_count = 0;
volatile u_int8 total_l2cap_data_count = 0;
#endif
//extern u_int8 packet_buffer_index;
#define MAX_TRANSFER_PACKET 10000
/*
 * Structure to store necessary information for HCI transport
 * subsystem.
 */ 

struct tra_hcit_info volatile *sys_hcit_info;

/*
 * Function pointer, pointing to the HCI transport registration function
 * This function is implemented in sys_main.c (or equivalent) in the appropriate
 * hal subdirectory. The assignment is also performed in sys_main.c
 */

void* (*TRAhcit_Register)(void);

void (*hci_generic_event_callback)(u_int8 *data, u_int32 pdu_len, u_int8 *header, 
                            u_int8 head_len);

void TRAhcit_Generic_Dispatcher(u_int8 *data, u_int32 pdu_len, u_int8 *header, 
                            u_int8 head_len,u_int8 q_type,t_deviceIndex device_index);

#define TRAhcit_Inc_Cntr(cntr) 


/*
 * This is a table of function pointers corresponding to initialisation functions
 * of layers which use the physical devices.
 * At present, only a UART decoding layer is supported. TRA_HCIT_TYPE_ENDS
 * is the last enumerator in the HCI physical transport device list. It is
 * used to terminate the list.
 */

void (*tra_hcit_initialise_table[TRA_HCIT_TYPE_ENDS])(void) = 
{ 
#if (TRA_HCIT_UART_SUPPORTED==1)
    TRAhcit_UART_Initialise,
#endif
#if (TRA_HCIT_USB_SUPPORTED==1)
    TRAhcit_USB_Initialise,
#endif
#if (TRA_HCIT_PCMCIA_SUPPORTED==1)
    NULL,
#endif
#if (TRA_HCIT_COMBINED_SUPPORTED==1)
    TRAhcit_COMBINED_Initialise,
#endif
#if (TRA_HCIT_GENERIC_SUPPORTED==1)
    NULL,
#endif
};


/*
 * Corresponding table for shutdown routines.
 */

void (*tra_hcit_shutdown_table[TRA_HCIT_TYPE_ENDS])(void) = 
{
#if (TRA_HCIT_UART_SUPPORTED==1)
    TRAhcit_UART_Shutdown,
#endif
#if (TRA_HCIT_USB_SUPPORTED==1)
    TRAhcit_USB_Shutdown,
#endif
#if (TRA_HCIT_PCMCIA_SUPPORTED==1)
    NULL,
#endif
#if (TRA_HCIT_COMBINED_SUPPORTED==1)
    TRAhcit_COMBINED_Shutdown,
#endif
#if (TRA_HCIT_GENERIC_SUPPORTED==1)
    NULL,
#endif
};


/*
 * And a table for the transmit routines - NOTE: See the optimisation below
 * for the case of UART support only being included.
 */

void (*tra_hcit_transmit_table[TRA_HCIT_TYPE_ENDS])
    (u_int8 *, u_int32, u_int8 *, u_int8, u_int8, t_deviceIndex) = 
{
#if (TRA_HCIT_UART_SUPPORTED==1)
    TRAhcit_UART_Transmit,
#endif
#if (TRA_HCIT_USB_SUPPORTED==1)
    TRAhcit_USB_Transmit,
#endif
#if (TRA_HCIT_PCMCIA_SUPPORTED==1)
    NULL,
#endif
#if (TRA_HCIT_COMBINED_SUPPORTED ==1)
    TRAhcit_COMBINED_Transmit,
#endif
#if (TRA_HCIT_GENERIC_SUPPORTED==1)
    TRAhcit_Generic_Dispatcher,
#endif
};

/*
 * Variable determining if the HCI transport is running or not
 */

static volatile u_int8 _tra_hcit_active;

/*****************************************************************************
 * TRAhcit_Initialise
 *
 * This function is not re-entrant
 *
 * Returns:     0   if initialisation completed successfully
 *              1   if transport system already active
 ****************************************************************************/
int TRAhcit_Initialise(void)
{
    /*
     * If it is already initialised, do nothing, return 1
     */
    if(_tra_hcit_active)
    {
        return 1;
    }    

    /*
     * Call the register function pointer - this is implemented 
     * in the appropriate HAL. This will return a pointer to the 
     * appropriate structure, casted to the general structure.
     * see tra_hcit.h for details.
     */

    sys_hcit_info = (struct tra_hcit_info volatile*)TRAhcit_Register();

    /*
     * Initialise the handling layer (which in turn will initialise the physical device)
     */
    if (tra_hcit_initialise_table[sys_hcit_info->type])
        tra_hcit_initialise_table[sys_hcit_info->type]();
    _tra_hcit_active = 1;

    return 0;
}

/*****************************************************************************
 * TRAhcit_Shutdown
 *
 * This function is not re-entrant 
 *
 * Returns:     0   if shutdown completed successfully
 *              1   if transport system not active
 ****************************************************************************/
int TRAhcit_Shutdown(void)
{
    /*
     * Shutdown the physical layer.
     * If not already initialised, return 1
     */
    if(!_tra_hcit_active)
        return 1;

    /*
     * Shut down the decoding layer (which in turn will shutdown the physical device)
     */
    if(tra_hcit_shutdown_table[sys_hcit_info->type])
       tra_hcit_shutdown_table[sys_hcit_info->type]();
    _tra_hcit_active = 0;
    return 0;
}


/*****************************************************************************
 * TRAhcit_Generic_Get_Rx_Buf
 *
 * Gets a buffer from the queuing system
 *
 * ptype            TRA_HCIT_COMMAND | TRA_HCIT_ACLDATA | TRA_HCIT_SCODATA
 * size             Length of command, of acl data, of sco data
 * hci_header       Header of HCI packet
 * 
 ****************************************************************************/
u_int8 *TRAhcit_Generic_Get_Rx_Buf(u_int8 ptype, int size, u_int8 *hci_header)
{
    t_q_descr *qd;
 
  //merge CEVA 0517 remove   SYSmmi_Display_Event(eSYSmmi_HCIt_Activity_Event);

    /*
     * At this point I have the length (in size) and the 
     * handle/pb/bc flags in rx_scratch [0] and [1]
     */

    if (ptype==TRA_HCIT_COMMAND)
    {
        qd = BTq_Enqueue(HCI_COMMAND_Q, 0, (t_length) size);
        if(qd)
        {    
            /* Explicit copies for unrolling */
            qd->data[0] = hci_header[0];
            qd->data[1] = hci_header[1];
            qd->data[2] = hci_header[2];
            return &qd->data[3];    /* Return the next location to write into (but don't modify qd->data!) */
        }
        return 0x0;
    }
#if (PRH_BS_CFG_SYS_LE_CENTRAL_DEVICE==1) || (PRH_BS_CFG_SYS_LE_PERIPHERAL_DEVICE==1)
    else if (ptype==TRA_HCIT_ACLDATA)
    {
        /* Need to decode the handle and the pb/bc flag  */
       
        /*
         * In this case, if at any stage during the decoding, something
         * is found to be out of range or otherwise malformed, a goto
         * to the "received_pdu_is_malformed" is executed. The other paths
         * of the function will return before this code is reached, if no
         * errors have occurred. This is done to minimise both code duplication
         * and the amount of time spent in the transport driver interrupt
         * e.g. UARTs may use FIQ (highest priority) on an ARM based platform
         * and as such need to be as fast as possible.
         */

        t_deviceIndex device_index=0;
        u_int16 handle = hci_header[0] | ((hci_header[1] & 0xf) << 8);
        u_int8 pb_flag = (hci_header[1] >> 4) & 0x3;
    	t_LE_Connection* p_le_connection;

        /*
         * Is this a broadcast packet ?
         */


#if 1// (LE_INCLUDED == 1)

        {
            {
            	p_le_connection = LEconnection_Find_LE_Link((t_connectionHandle)handle);

            	if (p_le_connection)
            	{
            		device_index = p_le_connection->device_index;
            	}
            	else
            	{
#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==0)//merge CEVA 0517 
            		HCeg_Hardware_Error_Event(PRH_BS_HW_ERROR_INVALID_LMP_LINK);
#endif//merge CEVA 0517 
                    return 0x0;
            	}

            }
        }

#endif
        if (pb_flag == 3) /* Exception, invalid flags  pb = 0 is now valid*/
        {
#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==0)//merge CEVA 0517
            HCeg_Hardware_Error_Event(PRH_BS_HW_ERROR_HCIT_RX_PDU_MALFORMED);
#endif////merge CEVA 0517
            return 0x0;
        }
      
        /*
         * If ACL Data Queue Space is Available Then
         *    Enqueue packet (assigns qd->device_index, qd->length)
         * Else
         *    Report HCI_DATA_BUFFER_OVERFLOW_EVENT
         * Endif
         */
        /*
         * If encrypted link add 4 bytes for the MIC which will be added by the
         * MiniSched when encrypting the payload.
         */
#if (PRH_BS_CFG_SYS_ENCRYPTION_SUPPORTED==1)
        if (p_le_connection->current_security_state & LE_ENCRYPTION_ACTIVE)
        	size+=4;
#endif
        qd = BTq_Enqueue(L2CAP_OUT_Q, device_index, (t_length) size);                    
        if(qd)
        {
             TRAhcit_Inc_Cntr(tra_num_acl_pkts_enqueued);
			 if ((pb_flag == 00) ||(pb_flag == 0x02))
			 {
                 qd->message_type = LCH_start;
			 }
			 else
				 qd->message_type = LCH_continue;
#if (PRH_BS_CFG_SYS_ENCRYPTION_SUPPORTED==1)
			 if (p_le_connection->current_security_state & LE_ENCRYPTION_ACTIVE)
			 {
				 qd->encrypt_status = LE_DATA_ENCRYPT_PENDING;
			 }
			 else
#endif
				 qd->encrypt_status = LE_DATA_NOT_ENCRYPTED;//merge CEVA 0517

             sys_hcit_info->rx_device_index = device_index;
             return qd->data; /* Early return for speed (this call is in the context of UART interrupt) */
        }
        else 
        {
#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==0)//merge CEVA 0517
             t_lm_event_info event_info;

             TRAhcit_Inc_Cntr(tra_num_acl_pkts_missed);
             event_info.link_type = ACL_LINK;
             HCeg_Generate_Event(HCI_DATA_BUFFER_OVERFLOW_EVENT, &event_info);
#endif//merge CEVA 0517
             return 0x0;
        }
    }
#endif


   return 0x0;
}

/*****************************************************************************
 * TRAhcit_Generic_Commit_Rx_Buf
 *
 * Commits a buffer allocated by HCit_generic_get_buf to the queueing system
 ****************************************************************************/
void TRAhcit_Generic_Commit_Rx_Buf(u_int8 buffer_type)
{
 //merge CEVA 0517 remove SYSmmi_Display_Event(eSYSmmi_HCIt_Activity_Event);

    if (buffer_type == TRA_HCIT_COMMAND)
    {
        BTq_Commit(HCI_COMMAND_Q,0);

    }
#if (PRH_BS_CFG_SYS_LE_CENTRAL_DEVICE==1) || (PRH_BS_CFG_SYS_LE_PERIPHERAL_DEVICE==1)
    else if (buffer_type == TRA_HCIT_ACLDATA)
    {
        TRAhcit_Inc_Cntr(tra_num_acl_ser_commit);
        BTq_Commit(L2CAP_OUT_Q, sys_hcit_info->rx_device_index);

        /*
         * Check if the link is encrypted and raise a Flag for the MiniShed to indicate
         * data should be Enctypted.
         */
        {
#if (PRH_BS_CFG_SYS_ENCRYPTION_SUPPORTED==1)
        	t_LE_Connection* p_connection = LEconnection_Find_Device_Index(sys_hcit_info->rx_device_index);

        	if ((p_connection) && (LEconnection_Is_Active(p_connection)) && (p_connection->current_security_state & LE_ENCRYPTION_ACTIVE))//merge CEVA 0517
        	{
        		LE_config.encrypt_pending_flag =0x01;
        		LE_config.device_index_pending_data_encrypt = sys_hcit_info->rx_device_index;// p_connection->link_id;
        	}
#endif
        }

    }
#endif
 
}

/*****************************************************************************
 * TRAhcit_Generic_Acknowledge_Complete_Tx
 *
 * Acknowledge the completion of tranmission - this allows the next item 
 * on the queue to be transmitted.
 ****************************************************************************/
void TRAhcit_Generic_Acknowledge_Complete_Tx(u_int8 queue, u_int32 buf_len)
{
    t_deviceIndex i;
    
    if (queue == HCI_EVENT_Q)
        i=0;
    else
        i=sys_hcit_info->tx_device_index;

    if(!BTq_Is_Queue_Empty(queue, i)) /* PROTECT QUEUES FROM APP ISSUES */
    {
#if (PRH_BS_CFG_SYS_LE_CENTRAL_DEVICE==1) || (PRH_BS_CFG_SYS_LE_PERIPHERAL_DEVICE==1)
#if (PRH_BS_CFG_SYS_ENCRYPTION_SUPPORTED==1)
    	t_LE_Connection* p_connection = LEconnection_Find_Device_Index(i);

    	if (p_connection->current_security_state & LE_ENCRYPTION_ACTIVE)
    		buf_len+=4;
#endif
#endif

    	BTq_Ack_Last_Dequeued(queue, i, (t_length)buf_len);

    }

}

/*****************************************************************************
 * TRAhcit_Dispatch_Pending_Data
 *
 * Dispatch any data to be transmitted to the host
 *
 * Check the L2CAP incoming queues for entries.
 * Service each device in a round robin fashion - priorities can 
 * be easily retrofitted if required for added value
 *
 * HC to Host  flow control needs to be added if necessary
 ****************************************************************************/


t_error TRAhcit_Dispatch_Pending_Data(void)
{
#if (PRH_BS_CFG_SYS_LE_CENTRAL_DEVICE==1) || (PRH_BS_CFG_SYS_LE_PERIPHERAL_DEVICE==1)
    u_int8 link_id;
    t_q_descr *qd;

	u_int8 i;

     u_int8 test_materdata[9] = { 0xFA,0xFB,0xFC,0xFD,
                                             0x04,0x00,0xA2,0xA6,0xBB};

     u_int8 test_slavedata[9] ={ 0xFA,0xFB,0xFC,0xFD,
                                           0x04,0x00,0x00,0x04,0xBB};
    /*
     * This array stores the header of the PDU to be transmitted to the host.
     * The maximum header is 5, 1 for the type byte and 4 for an ACL header.
     */
    #if ((PRH_BS_CFG_SYS_LE_L2CAP_INCLUDED==1) || (PRH_BS_CFG_SYS_LE_GATT_INCLUDED==1) || (PRH_BS_CFG_SYS_LE_SMP_INCLUDED==1))//merge CEVA 0517

    #else//merge CEVA 0517
   
    static u_int8 hcit_tx_to_host_acl_header[5];
   
    #endif//merge CEVA 0517

#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY == 0)//merge CEVA 0517
#if (BUILD_TYPE==GENERIC_BUILD)
    if(! sys_hcit_info->tx_acl_busy)
#else
    if(! sys_hcit_info->tx_busy)
#endif
#endif//merge CEVA 0517
    {

    	u_int16 length;//merge CEVA 0517


    	for (link_id = 0; link_id < DEFAULT_MAX_NUM_LE_LINKS;link_id++)
        {
        	t_LE_Connection* p_connection = LEconnection_Find_Link_Entry(link_id);
        	if(LEconnection_Is_Active(p_connection))//merge CEVA 0517 if(p_connection->active)
        	{
        		qd = HCfc_Get_Next_HC_to_Host_Data_Descriptor(p_connection->device_index);

        		if(qd)
        		{
        			/*
        			 * As this is run from MiniSched we can Decrypt any L2CAP data at this point.
        			 * As the original Queue Entry had a MIC field included we have to adjust the length
        			 * field by 4.
        			 */
#if 1 // GF 16 Oct//merge CEVA 0517
    				//handle = LEconnection_Determine_Connection_Handle(p_connection);
#if (PRH_BS_CFG_SYS_ENCRYPTION_SUPPORTED==1)//merge CEVA 0517
    				if ((qd->encrypt_status & LE_DATA_DECRYPT_PENDING) && (p_connection->current_security_state & LE_ENCRYPTION_ACTIVE))//merge CEVA 0517
    				{
    					t_error status;//merge CEVA 0517

						status = LEconnection_Decrypt_Incoming_Pdu(p_connection,qd->data,(u_int8)qd->length,qd->enc_pkt_count,qd->message_type);//merge CEVA 0517
						if (status == NO_ERROR)//merge CEVA 0517
						{
							length = qd->length - 4;//merge CEVA 0517
							qd->encrypt_status = LE_DATA_NOT_ENCRYPTED;//merge CEVA 0517
						}
						else
						{
							BTq_Ack_Last_Dequeued(L2CAP_IN_Q,qd->device_index,qd->length);//merge CEVA 0517
							LEconnection_Local_Disconnect(p_connection, status /* MIC Mismatch */);//merge CEVA 0517
							return NO_ERROR;//merge CEVA 0517
						}

    				}
    				else//merge CEVA 0517
#endif//merge CEVA 0517
    				{
    					length = qd->length;//merge CEVA 0517
							#ifdef ATT_CNT_TEST
							total_l2cap_data_count++;
							#endif
    				}
#endif//merge CEVA 0517

#if (PRH_BS_CFG_SYS_LE_SMP_INCLUDED==1)//merge CEVA 0517
        			// Check of the CID == 0x0006 the CID for SMP
        			if(0x6 ==  ((u_int16)qd->data[2] + (u_int16)(qd->data[3]<< 8)))//merge CEVA 0517
        			{
            			// This is an SMP Signalling PDU//merge CEVA 0517
            			LEsmp_Decode_Incoming_PDU(p_connection,(qd->data+4),(length-4));//merge CEVA 0517
            			BTq_Ack_Last_Dequeued(L2CAP_IN_Q,qd->device_index,qd->length);//merge CEVA 0517
						return NO_ERROR;//merge CEVA 0517
        			}
        			else//merge CEVA 0517
#endif

#if (PRH_BS_CFG_SYS_LE_GATT_INCLUDED==1)//merge CEVA 0517
            		// Check of the CID == 0x0004 the CID for ATT
        			if(0x4 ==  ((u_int16)qd->data[2] + (u_int16)(qd->data[3]<< 8)))//merge CEVA 0517
            		{
									#ifdef ATT_CNT_TEST
									ATT_cmd_count++;
									//DEBUG_MSG(0xC6);
									//DEBUG_MSG(ATT_cmd_count);
									#endif
				//comprare att data,and then send data if euqal
				if( ( qd->data[7+0] ==test_materdata[0])  && ( qd->data[7+1] ==test_materdata[1]) && ( qd->data[7+2] ==test_materdata[2]) &&
				     ( qd->data[7+3] ==test_materdata[3])  && ( qd->data[7+4] ==test_materdata[4]) &&  ( qd->data[7+5] ==test_materdata[5]) &&
				     ( qd->data[7+6] ==test_materdata[6])  && ( qd->data[7+7] ==test_materdata[7]) && ( qd->data[7+8] ==test_materdata[8])  )
					{
						                   //DEBUG_MSG(0xbe);
					                    // User_Notify(test_slavedata,9);//send data for test box
					                   // bprintf("Check Ok!\r\n");
					                    Beken_UART_Initial();
					                     for(i =0; i < 9; i++)
					                    {
					                      	Beken_UART_TxChar(test_materdata[i]);   //add frank 160701
					                      	//Delay(1);
					                     }
							      Delay(2);
							      Beken_UART_Close();
					}				
				
				// DEBUG_MSG(0xC6);
            			LEatt_Decode_PDU(p_connection,(u_int8)(length),(qd->data+4));//merge CEVA 0517
            			BTq_Ack_Last_Dequeued(L2CAP_IN_Q,qd->device_index,qd->length);//merge CEVA 0517
						return NO_ERROR;//merge CEVA 0517
            		}
        			else //merge CEVA 0517
#endif

#if (PRH_BS_CFG_SYS_LE_L2CAP_INCLUDED==1)//merge CEVA 0517	
					if (0x5 == ((u_int16)qd->data[2] + (u_int16)(qd->data[3]<< 8)))//merge CEVA 0517
        			{
        				LE_L2CAP_Decode_Incoming_PDU(p_connection,(qd->data+4),(length-4));//merge CEVA 0517
            			       BTq_Ack_Last_Dequeued(L2CAP_IN_Q,qd->device_index,qd->length);//merge CEVA 0517
						return NO_ERROR;//merge CEVA 0517
        			}
					else//merge CEVA 0517
#endif
#if ((PRH_BS_CFG_SYS_LE_L2CAP_INCLUDED==1) || (PRH_BS_CFG_SYS_LE_GATT_INCLUDED==1) || (PRH_BS_CFG_SYS_LE_SMP_INCLUDED==1))//merge CEVA 0517
					{
						// Ack the Data as it has invalid CID.//merge CEVA 0517
						BTq_Ack_Last_Dequeued(L2CAP_IN_Q,qd->device_index,qd->length);//merge CEVA 0517
						return NO_ERROR;//merge CEVA 0517
					}
#else//merge CEVA 0517
        			{
        				/*
        				 * At this point, we have to construct the HCI payload header
        				 * The event queue entries will already contain this.
        				 * Format = (lsb l to r) handle : pb : bc : length
        				 */
        				u_int16 handle;

#if 1 //(LE_INCLUDED == 1)


        				handle = LEconnection_Determine_Connection_Handle(p_connection);//merge CEVA 0517
#if (PRH_BS_CFG_SYS_ENCRYPTION_SUPPORTED==1)
        				if ((qd->encrypt_status & LE_DATA_DECRYPT_PENDING) && (p_connection->current_security_state & LE_ENCRYPTION_ACTIVE))
        				{
        					t_error status;//merge CEVA 0517
							status = LEconnection_Decrypt_Incoming_Pdu(p_connection,qd->data,(u_int8)qd->length,qd->enc_pkt_count,qd->message_type);//merge CEVA 0517
							if (status == NO_ERROR)//merge CEVA 0517
							{
								length = qd->length - 4;//merge CEVA 0517
								qd->encrypt_status = LE_DATA_NOT_ENCRYPTED;//merge CEVA 0517
							}
							else
							{
								BTq_Ack_Last_Dequeued(L2CAP_IN_Q,qd->device_index,qd->length);//merge CEVA 0517
								LEconnection_Local_Disconnect(p_connection, status /* MIC Mismatch */);//merge CEVA 0517
								return 0;//merge CEVA 0517
							}

        				}
        				else
#endif
        				{
        					length = qd->length;
        				}

#else
        				p_link = LMaclctr_Find_Device_Index(qd->device_index);

        				handle = p_link->handle;
#endif

#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==0)//merge CEVA 0517
								

        				hcit_tx_to_host_acl_header[0] = TRA_HCIT_ACLDATA;
        				hcit_tx_to_host_acl_header[1] = handle & 0xff;
        				hcit_tx_to_host_acl_header[2] = (u_int8)(handle >> 8)
                                                | (qd->message_type<<4);
        				hcit_tx_to_host_acl_header[3] = length & 0xff;
        				hcit_tx_to_host_acl_header[4] = (u_int8)((length >> 8) & 0xff);

        				/*
        				 * The PDU is now ready for transmission to the host. At this
        				 * point, the decoding sub-layer transmit function is called.
        				 * This will in turn call the appropriate physical layer
        				 * transmit function.
        				 */

        				TRAhcit_UART_Transmit(
        						qd->data, length, hcit_tx_to_host_acl_header, 5,
        						L2CAP_IN_Q, qd->device_index);
#endif
        				return (t_error)0;//merge CEVA 0517

        				/*
        				 * This means we only send one PDU per iteration, so as not to
        				 * overload the physical transport driver.
        				 */
        			}
#endif
        		}

            }
        }

    }
#endif
    return (t_error)1;
}

/*****************************************************************************
 * TRAhcit_Dispatch_Pending_Event
 *
 * Take pending events off the HCI Event queue and transmit them
 * to the host.
 ****************************************************************************/
t_error TRAhcit_Dispatch_Pending_Event(void)
{
    t_q_descr *qd;
    u_int8 _header_byte;

#if (BUILD_TYPE==GENERIC_BUILD)
    if(!sys_hcit_info->tx_evt_busy)
#else
    if(!sys_hcit_info->tx_busy)
#endif
    {
        qd = BTq_Dequeue_Next(HCI_EVENT_Q,0);
        if(qd)
        {        
            _header_byte = TRA_HCIT_EVENT;

#if TRA_HCIT_UART_ONLY_SUPPORTED == 1

            /*
             * This is an optimisation for the most common case 
             * - only a UART supported for hci transport.
             */
            
               TRAhcit_UART_Transmit(qd->data, qd->length, 
                &_header_byte, 1, HCI_EVENT_Q, qd->device_index);

#else        
            /* 
             * Otherwise, the slightly more expensive array lookup is used 
             */
#if (BUILD_TYPE==GENERIC_BUILD)
            sys_hcit_info->tx_evt_busy = 1;
#endif
            (*tra_hcit_transmit_table[sys_hcit_info->type])(qd->data, qd->length,
                &_header_byte, 1, HCI_EVENT_Q, qd->device_index);                    
#endif
            return (t_error)0;
        }
    }
    return (t_error)1;
}


/*****************************************************************************
 * TRAhcit_Register_Generic_Dispatcher
 *
 * Register the generic data dispatcher for HCI Generic
 ****************************************************************************/
t_error TRAhcit_Register_Generic_Dispatcher(
    void (*callback)(u_int8 *data, u_int32 pdu_len, u_int8 *header, u_int8 head_len))
{
    hci_generic_event_callback = callback;
    return NO_ERROR;
}

/*****************************************************************************
 * TRAhcit_Generic_Dispatcher
 *
 * The generic data dispatcher for HCI Generic
 ****************************************************************************/
void TRAhcit_Generic_Dispatcher(u_int8 *data, u_int32 pdu_len, u_int8 *header, 
                            u_int8 head_len, u_int8 q_type, t_deviceIndex device_index)
{
    hci_generic_event_callback(data, pdu_len, header, head_len);
}

