/*************************************************************************
 * MODULE NAME:    tra_queue.c
 * PROJECT CODE:   Bluestream
 * DESCRIPTION:    Queue functions for all Queues in the Bluetooth 
 *                 host controller stack
 * AUTHOR:         HC Team
 * DATE:           28/03/2000
 *
 * SOURCE CONTROL: $Id: tra_queue.c,v 1.1 2012/05/09 00:10:22 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2004 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *      November 2000 - Added fixed size chunk allocation scheme for L2CAP data
 *
 * ISSUES:
 * NOTES TO USERS: 
 *      March 2002      P7C major restructure of queues
 *
 *      Not             Segmentation of variable length queues removed
 *************************************************************************/
#include "sys_config.h"

#include <assert.h>
#include "tra_queue.h"

#include "hw_macro_defs.h"
#include "hw_memcpy.h"
#include "sys_irq.h"
#include "sys_atomic.h"
#include "le_connection.h"//merge CEVA 0517
#include "le_security.h"//merge CEVA 0517


#if defined(SYS_HAL_OS_EVENT_DRIVEN_HOST_CONTROLLER)
#include "bt_mini_sched.h"
#endif

#include "hc_event_gen.h"
#include "hc_const.h"
#include "debug.h"

/*
 * Static space for queues (word aligned heap and an array of headers)
 */
typedef union
{
    u_int32 _dummy;
    u_int8 hci_event_heap_array_[PRH_BS_CFG_SYS_SIZEOF_HCI_EVENT_HEAP];
} t_event_heap;
t_event_heap event_heap_;

u_int8* hci_event_heap = event_heap_.hci_event_heap_array_;
t_q_descr hci_event_headers[PRH_BS_CFG_SYS_MAX_HCI_EVENTS];

typedef union
{
    u_int32 _dummy;
    u_int8 hci_command_heap_array_[PRH_BS_CFG_SYS_SIZEOF_HCI_COMMAND_HEAP];
} t_command_heap;
t_command_heap command_heap_;

u_int8* hci_command_heap = command_heap_.hci_command_heap_array_;
t_q_descr hci_command_headers[PRH_BS_CFG_SYS_MAX_HCI_COMMANDS];


typedef union
{
    u_int32 _dummy;
    u_int8 lmp_in_heap_array_[PRH_BS_CFG_SYS_SIZEOF_LMP_IN_HEAP];
} t_lmp_in_heap;
t_lmp_in_heap lmp_in_heap_;

u_int8* lmp_in_heap = lmp_in_heap_.lmp_in_heap_array_;
t_q_descr lmp_in_headers[PRH_BS_CFG_SYS_MAX_LMP_IN_MESSAGES];

u_int8 lmp_out_heaps[MAX_ACTIVE_DEVICE_LINKS][PRH_BS_CFG_SYS_SIZEOF_LMP_OUT_HEAP];
t_q_descr lmp_out_headers[MAX_ACTIVE_DEVICE_LINKS][PRH_BS_CFG_SYS_MAX_LMP_OUT_MESSAGES];    



#if (PRH_BS_CFG_SYS_TRACE_VIA_HCI_SUPPORTED==1)
/*
 * Setup a distinct queue for logging
 */
u_int8 hci_debug_heap[PRH_BS_CFG_SYS_SIZEOF_HCI_DEBUG_HEAP];
t_q_descr hci_debug_headers[PRH_BS_CFG_SYS_MAX_HCI_DEBUG_EVENTS];
#endif

/* 
 *  Declare a container for all the queues 
 */
t_queue BTQueues[TRA_QUEUE_TOTAL_NUM_QUEUES];



/*****************************************************************************
 * mBTq_Get_Q_Ref
 *
 * Return a reference to the Q for the given
 *
 * q_type
 * device_index
 * 
 * q_type               device_index        q_index offset

 * HCI_COMMAND_Q        0                   q_type
 * HCI_EVENT_Q          0                   q_type
 * LMP_IN_Q             ANY                 q_type
 * LMP_OUT_Q            ANY                 q_type + device_index
 * L2CAP_IN_Q           ANY                 q_type + device_index
 * L2CAP_OUT_Q          ANY                 q_type + device_index
 ****************************************************************************/
#define mBTq_Get_Q_Ref(q_type, device_index) \
    (BTQueues + (q_type) + (((q_type)==LMP_IN_Q) ? 0 : (device_index)))

/*****************************************************************************
 * _BTq_Initialise_Variable_Chunk_Queue
 *
 * Initialises the specified variable chunk sized queue 
 * i.e. the header array and data heap pointers
 ****************************************************************************/
static void _BTq_Initialise_Variable_Chunk_Queue(
    t_queue *q, t_q_descr *headers, 
    u_int8 num_headers, u_int8 *data_ptr,  u_int16 data_length)
{
    u_int16 i;

    /* 
     * Zero the header array equiv memset(&headers[i], 0, sizeof(t_q_descr) );
     */
    for(i=0;i<num_headers;i++)
    {
        u_int16 j = sizeof(t_q_descr);
        u_int8 *ptr=(u_int8*) &headers[i];

        while (j-- != 0)
        {
            *ptr++ = 0;
        }
    }

    q->headers = headers;

    q->num_headers = num_headers;
    q->desc_head = q->desc_tail = 0;

    q->head_offset = q->tail_offset = 0;
    q->end_offset = q->data_size = data_length;
    q->q_data = data_ptr;

    q->num_processed_chunks = 0;

    q->num_entries = 0;
    q->pending_commits = 0;

}


/*****************************************************************************
 * _BTq_Initialise_Data_Queue
 *
 * Initialises the specified queue i.e. the header array and data heap pointers
 ****************************************************************************/
static void _BTq_Initialise_Data_Queue(t_queue *q)
{
    q->headers = 0x0;
    q->headers_tail = 0x0;

    q->num_processed_chunks = 0;

    q->num_entries = 0;
    q->pending_commits = 0;

}

/*****************************************************************************
 * BTq_Initialise_Data_Queue
 *
 * Version of BTq_Initialise_Data_Queue for external use. Specically
 * used to clean up SCO Qs when a SCO/eSCO connection is torn down.
 *
 ****************************************************************************/
void BTq_Initialise_Data_Queue(unsigned int q_index)
{
    _BTq_Initialise_Data_Queue(BTQueues+q_index);
}

/*
 * The data queues now adopt a single generic data queue strategy
 * to support both ACL and SCO.
 * The queue definitions are held in RAM allowing them to
 * to be completely reconfigured after initialisation.
 * Each data queue group is completely defined by t_data_queue.
 *
 * Notes:
 * 1.  L2CAP (ACL buffers) and SCO buffers are aligned to a multiple of 
 *     PRH_BS_CFG_SYS_ACL_BUFFER_ALIGNMENT.
 *     For SCO buffers its not required.
 * 2.  IN:  Radio Baseband to Host       OUT: Host to Radio Baseband
 * 3.  ACL/SCO data is handled differently because of flow control
 * 4.  All data queues are now stored in a single RAM area which can
 *     later be completely restructured to any required size.
 * 5.  Each queue has
 *     a/  A stack of free header addresses
 *     b/  An array of headers
 *     c/  A data heap of fixed buffer sizes
 */

/*
 * Queue descriptions for L2CAP queues,
 * Deliberately placed before BTq_data_memory for test validation
 */
t_data_queue BTq_l2cap_in;
t_data_queue BTq_l2cap_out;

struct s_all_queues
{
    /*
     * ACL Queues
     */
#if (PRH_BS_CFG_SYS_FLEXIBLE_DATA_BUFFER_SIZES_SUPPORTED==1)
    t_q_descr **l2cap_in_free_header_list;
    t_q_descr *l2cap_in_headers;
    u_int8 *l2cap_in_heaps;

    t_q_descr **l2cap_out_free_header_list;
    t_q_descr *l2cap_out_headers;
    u_int8 *l2cap_out_heaps;
#else
    t_q_descr *l2cap_in_free_header_list[PRH_BS_CFG_SYS_NUM_IN_ACL_PACKETS];
    t_q_descr l2cap_in_headers[PRH_BS_CFG_SYS_NUM_IN_ACL_PACKETS];
    u_int8 l2cap_in_heaps[PRH_BS_CFG_SYS_NUM_IN_ACL_PACKETS*
        mBTq_Get_Aligned_Data_Packet_Length(PRH_BS_CFG_SYS_ACL_IN_PACKET_LENGTH)];

    t_q_descr *l2cap_out_free_header_list[PRH_BS_CFG_SYS_NUM_OUT_ACL_PACKETS];
    t_q_descr l2cap_out_headers[PRH_BS_CFG_SYS_NUM_OUT_ACL_PACKETS];
    u_int8 l2cap_out_heaps[PRH_BS_CFG_SYS_NUM_OUT_ACL_PACKETS*
        mBTq_Get_Aligned_Data_Packet_Length(PRH_BS_CFG_SYS_ACL_OUT_PACKET_LENGTH)];/* total size 20 * 46 */
#endif

} BTq_data_memory;



/*****************************************************************************
 * _BTq_Get_Data_Queue_Ref
 *
 * This function returns the data queue descriptor for the identified queue
 * If not a data queue it will return 0
 ****************************************************************************/
t_data_queue *_BTq_Get_Data_Queue_Ref(u_int8 q_type)
{
    t_data_queue *p_data_queue;

    if (q_type == L2CAP_IN_Q)
    {
        p_data_queue = &BTq_l2cap_in;
    }
    else if (q_type == L2CAP_OUT_Q)
    {
        p_data_queue = &BTq_l2cap_out;
    }
    else
    {
        p_data_queue = (t_data_queue *)0; 
    }
    return p_data_queue;    
}

/*****************************************************************************
 * BTq_Initialise_Data_Queue_Type
 *
 * This function initialises the shared queue variables used to manage 
 * a single type of queue 
 *
 * q_type                   L2CAP_IN_Q L2CAP_OUT_Q | SCO_IN_Q | SCO_OUT_Q | 
 * num_of_headers           Number of queue entries supported
 * queue_headers            The queue headers
 * free_header_list         A set of pointers to maintain the free headers
 * max_data_packet_length   Size reported to Host if applicable
 * queue_heap               Queue data area must be large enough to
 *                          support aligned version of queue
 ****************************************************************************/
void BTq_Initialise_Data_Queue_Type(u_int8 q_type,
    u_int16   num_of_headers,
    t_q_descr *queue_headers,
    t_q_descr **free_header_list,
    u_int16   max_data_packet_length,
    u_int8    *queue_heap
)
{
    u_int16 aligned_data_packet_length;
    u_int16 i;

    t_data_queue *p_data_queue = _BTq_Get_Data_Queue_Ref(q_type);

    p_data_queue->next_free_pointer = 0;
    p_data_queue->num_of_consumed_chunks = 0;
    p_data_queue->num_of_headers = num_of_headers;
    p_data_queue->max_data_packet_length = max_data_packet_length;
    p_data_queue->free_header_list = free_header_list;
    p_data_queue->heap = queue_heap;

    /*
     * Set up the global free lists and allocate data buffers to headers
     */
    aligned_data_packet_length = 
        mBTq_Get_Aligned_Data_Packet_Length(max_data_packet_length);

    for(i=0; i<num_of_headers; i++)
    {
        p_data_queue->free_header_list[i]
            = &queue_headers[i];
        p_data_queue->free_header_list[i]->data 
            = p_data_queue->heap + (i*aligned_data_packet_length);
    }
}

/*****************************************************************************
 * _BTq_Initialise_All_Data_Queues
 *
 * This function initialises the shared queue variables used to manage 
 * the global heap of header descriptors and data chunks
 ****************************************************************************/
static void _BTq_Initialise_All_Data_Queues(void)
{
    u_int16 i;

    BTq_Initialise_Data_Queue_Type(L2CAP_IN_Q,      /*q_type*/
#if (PRH_BS_CFG_SYS_FLEXIBLE_DATA_BUFFER_SIZES_SUPPORTED==1)
        g_sys_config.hc_buffer_size_in.numAclDataPackets,
#else
        PRH_BS_CFG_SYS_NUM_IN_ACL_PACKETS,          /*num_of_headers*/
#endif
        BTq_data_memory.l2cap_in_headers,           /*queue_headers*/
        BTq_data_memory.l2cap_in_free_header_list,  /*free_header_list*/
        PRH_BS_CFG_SYS_ACL_IN_PACKET_LENGTH,        /*max_data_packet_length*/
        BTq_data_memory.l2cap_in_heaps              /*queue_heap*/ );

    BTq_Initialise_Data_Queue_Type(L2CAP_OUT_Q,     /*q_type*/
#if (PRH_BS_CFG_SYS_FLEXIBLE_DATA_BUFFER_SIZES_SUPPORTED==1)
        g_sys_config.hc_buffer_size.numAclDataPackets,
#else
        PRH_BS_CFG_SYS_NUM_OUT_ACL_PACKETS,         /*num_of_headers*/
#endif
        BTq_data_memory.l2cap_out_headers,          /*queue_headers*/
        BTq_data_memory.l2cap_out_free_header_list, /*free_header_list*/
        PRH_BS_CFG_SYS_ACL_OUT_PACKET_LENGTH,       /*max_data_packet_length*/
        BTq_data_memory.l2cap_out_heaps             /*queue_heap*/ );

    /* 
     * Initialise the input and output L2CAP queues 
     *
     * In the case of ACL queues, which now use a global chunk allocation scheme,
     * most of the function arguments are not used inside the BTq_Init function 
     */

    /* 
     * L2CAP input queues
     */
    for(i=0; i<MAX_ACTIVE_DEVICE_LINKS; i++)
    {
        _BTq_Initialise_Data_Queue(BTQueues+L2CAP_IN_Q+i);
    }

    /*
     * L2CAP output queues, the extra is used for the Piconet broadcast queues.
     */
    for(i=0; i<MAX_ACTIVE_DEVICE_LINKS+1; i++)
    {
        _BTq_Initialise_Data_Queue(BTQueues+L2CAP_OUT_Q+i);
    }
}

/*****************************************************************************
 * BTq_Get_L2CAP_Out_Heap_Ref
 *
 * Return location of the L2CAP Out Data Heap Area
 ****************************************************************************/
u_int8* BTq_Get_L2CAP_Out_Heap_Ref(void)
{
    return (u_int8*) BTq_l2cap_out.heap;
}


/*****************************************************************************
 * BTq_Is_Queue_Not_Full
 *
 * This function returns one if there is any space at all on the queue 
 * (i.e. non full). Combined with the above, all four cases can be 
 * derived: empty, full, not empty, and not full.
 ****************************************************************************/
boolean BTq_Is_Queue_Not_Full(u_int8 q_type, t_deviceIndex device_index)
{
    t_data_queue *p_data_queue = _BTq_Get_Data_Queue_Ref(q_type);

    /*
     * If a data queue Then 
     *     compare next_free against number of headers for queue group
     * Else
     *     compare directly number of entries against headers for queue
     * Endif
     */
    if (p_data_queue)
    {
        return (p_data_queue->next_free_pointer < p_data_queue->num_of_headers);
    }
    else
    {
        t_queue *q = &BTQueues[q_type+device_index];
        return (boolean)(q->num_entries < q->num_headers);
    }
}

/*****************************************************************************
 * BTq_Is_Queue_Empty
 *
 * This function returns 1 if the queue specified is empty
 ****************************************************************************/
boolean BTq_Is_Queue_Empty(u_int8 q_type, t_deviceIndex device_index)
{
    return !(BTQueues[q_type+device_index].num_entries);
}

/*****************************************************************************
 * _BTq_Allocate_Header
 *
 * Allocate a queue descriptor for the required queue
 *
 ****************************************************************************/
t_q_descr * _BTq_Allocate_Header(
    t_data_queue *p_data_queue, t_length length)
{
    t_q_descr *q_descr;

    if( p_data_queue->next_free_pointer < p_data_queue->num_of_headers &&
        length <= p_data_queue->max_data_packet_length)
    {
        q_descr = p_data_queue->free_header_list[p_data_queue->next_free_pointer++];
    }
    else
    {
        q_descr = (t_q_descr *) NULL;
    }
    return q_descr;
}

/*****************************************************************************
 * _BTq_Deallocate_Header
 *
 * Deallocate a queue descriptor for the required queue
 ****************************************************************************/
void _BTq_Deallocate_Header(t_data_queue *p_data_queue, t_q_descr *q_descr)
{
    p_data_queue->free_header_list[--p_data_queue->next_free_pointer] = q_descr;
}

/*****************************************************************************
 * BTq_Get_Queue_Number_Of_Processed_Chunks
 *
 * Get the number of processed chunks on queue.
 ****************************************************************************/
u_int16 BTq_Get_Queue_Number_Of_Processed_Chunks(
        u_int8 q_type, t_deviceIndex device_index)
{
    return BTQueues[q_type+device_index].num_processed_chunks;
}

/*****************************************************************************
 * BTq_Get_and_Reset_Queue_Number_Of_Processed_Chunks
 *
 * Get the number of processed chunks on queue.
 * Reset the number of processed chunks on queue to 0
 * Decrement the associated total of processed chunks.
 *
 * q_type           L2CAP_IN_Q L2CAP_OUT_Q SCO_IN_Q SCO_OUT_Q
 * device_index     Device or Sco index for queue
 *
 ****************************************************************************/
u_int16 BTq_Get_and_Reset_Queue_Number_Of_Processed_Chunks(
        u_int8 q_type, t_deviceIndex device_index)
{
     u_int16 number_of_chunks;
     number_of_chunks = BTQueues[q_type+device_index].num_processed_chunks;
    
     /*
      * Reset Queue Number of Processed Chunks to 0
      */
     BTq_Reduce_Queue_Number_Of_Processed_Chunks(
        q_type, device_index, number_of_chunks);

    return number_of_chunks;
}

/*****************************************************************************
 * BTq_Get_Total_Number_Of_Processed_Chunks
 *
 * Get the number of processed chunks
 * For outgoing BB L2CAP queues, processed means transmitted/flushed.
 * For incoming BB L2CAP queues, processed means received into queues
 *
 * q_type           L2CAP_IN_Q L2CAP_OUT_Q SCO_IN_Q SCO_OUT_Q
 *
 ****************************************************************************/
u_int16 BTq_Get_Total_Number_Of_Processed_Chunks(u_int8 q_type)
{
    t_data_queue *p_data_queue = _BTq_Get_Data_Queue_Ref(q_type);

    return p_data_queue->num_of_consumed_chunks;
}

/*****************************************************************************
 * BTq_Reduce_Queue_Number_Of_Processed_Chunks
 *
 * Reduce the number of queue processed chunks and the associated total
 * of processed chunks for the given queue type by reduction
 *
 * q_type           L2CAP_IN_Q L2CAP_OUT_Q SCO_IN_Q SCO_OUT_Q
 * device_index     Device or Sco index for queue
 * chunk_reductions The number of processed chunks to reduce
 *
 ****************************************************************************/
void BTq_Reduce_Queue_Number_Of_Processed_Chunks(
        u_int8 q_type, t_deviceIndex device_index, u_int16 chunk_reduction)
{
    t_data_queue *p_data_queue = _BTq_Get_Data_Queue_Ref(q_type);

    u_int16 *p_total_number_of_chunks;
    u_int16 *p_q_num_processed_chunks;
    /* 
     * CPU flags - stores current interrupt settings to enforce exclusivity 
     */
    u_int32 cpu_flags;  

    p_q_num_processed_chunks = &BTQueues[q_type+device_index].num_processed_chunks;
    p_total_number_of_chunks = &p_data_queue->num_of_consumed_chunks;

    SYSirq_Disable_Interrupts_Save_Flags(&cpu_flags); 

	/*******************************************************************************************
	 * It is important to prevent the number of "processed_chunks" and "number_of_chunks" from
	 * wrapping. Currently this occurs in ARQ_BV32 with eSCO and Erroneous Data turned on.
	 * 
	 * However, we have to preclude wrapping AT all cost.
	 *******************************************************************************************/

	if (*p_q_num_processed_chunks)
		*p_q_num_processed_chunks -= chunk_reduction;

	if (*p_total_number_of_chunks)
	    *p_total_number_of_chunks -= chunk_reduction;

    SYSirq_Interrupts_Restore_Flags(cpu_flags);  
}


/*****************************************************************************
 * BTq_Initialise
 *
 * Initialise all the queues in the system.
 ****************************************************************************/
void BTq_Initialise(void)
{
    int q_offset;

    /* 
     * Initialise the input and output L2CAP queues and SCO queues if supported
     */
    _BTq_Initialise_All_Data_Queues();
    
    /* 
     * Initialise HCI Event Queue 
     */
    _BTq_Initialise_Variable_Chunk_Queue(BTQueues + HCI_EVENT_Q,
        hci_event_headers, PRH_BS_CFG_SYS_MAX_HCI_EVENTS,
        hci_event_heap, PRH_BS_CFG_SYS_SIZEOF_HCI_EVENT_HEAP);        
    /* 
     * Initialise HCI Command Queue 
     */
    _BTq_Initialise_Variable_Chunk_Queue(BTQueues + HCI_COMMAND_Q,
        hci_command_headers, PRH_BS_CFG_SYS_MAX_HCI_COMMANDS,
        hci_command_heap, PRH_BS_CFG_SYS_SIZEOF_HCI_COMMAND_HEAP);   
    
    /* 
     * Initialise the shared LMP incoming Queue 
     */
    _BTq_Initialise_Variable_Chunk_Queue(BTQueues + LMP_IN_Q, 
        lmp_in_headers, PRH_BS_CFG_SYS_MAX_LMP_IN_MESSAGES,
        lmp_in_heap, PRH_BS_CFG_SYS_SIZEOF_LMP_IN_HEAP);

#if 0 // LE Included
    /* Initialise the Shared LE_LLC incoming Queue
     *
     */
   _BTq_Initialise_Variable_Chunk_Queue(BTQueues + LE_LLC_IN_Q,
       le_llc_in_headers, PRH_BS_CFG_SYS_MAX_LE_LLC_IN_MESSAGES,
       le_llc_in_heap, PRH_BS_CFG_SYS_SIZEOF_LE_LLC_IN_HEAP);

   /*
    * Initialise the output LE_LLC queues
    */
   for(q_offset=0; q_offset < MAX_ACTIVE_DEVICE_LINKS; q_offset++)
   {
       _BTq_Initialise_Variable_Chunk_Queue(BTQueues + LE_LLC_OUT_Q + q_offset,
           le_llc_out_headers[q_offset], PRH_BS_CFG_SYS_MAX_LE_LLC_OUT_MESSAGES,
           le_llc_out_heaps[q_offset], PRH_BS_CFG_SYS_SIZEOF_LE_LLC_OUT_HEAP);
   }
#endif
    /* 
     * Initialise the output LMP queues 
     */
    for(q_offset=0; q_offset < MAX_ACTIVE_DEVICE_LINKS; q_offset++)
    {
        _BTq_Initialise_Variable_Chunk_Queue(BTQueues + LMP_OUT_Q + q_offset, 
            lmp_out_headers[q_offset], PRH_BS_CFG_SYS_MAX_LMP_OUT_MESSAGES, 
            lmp_out_heaps[q_offset], PRH_BS_CFG_SYS_SIZEOF_LMP_OUT_HEAP);        
    }
    

#if (PRH_BS_CFG_SYS_TRACE_VIA_HCI_SUPPORTED==1)
    /* 
     * Initialise TCI Debug Queue 
     */
    _BTq_Initialise_Variable_Chunk_Queue(BTQueues + TCI_DEBUG_Q,
        hci_debug_headers, PRH_BS_CFG_SYS_MAX_HCI_DEBUG_EVENTS,
        hci_debug_heap, PRH_BS_CFG_SYS_SIZEOF_HCI_DEBUG_HEAP);        
#endif
}

/*****************************************************************************
 * BTq_Reset
 *
 * Reset the queues associated with device index
 * Warning: This function should be executed atomically with 
 * respect to any other contexts that might try to enqueue or dequeue 
 * to/from this particular queue.
 *
 * Reset the LMP output Queue
 * Reset the L2CAP input and output queues after returning any entries
 *       and resetting the number of processed chunks
 * Note that LMP input queue is not reset, since shared queue.
 ****************************************************************************/
void BTq_Reset(t_deviceIndex device_index)
{   
    t_queue *q;
    t_q_descr *cursor;
 
    _BTq_Initialise_Variable_Chunk_Queue(BTQueues + LMP_OUT_Q + device_index, 
        lmp_out_headers[device_index], PRH_BS_CFG_SYS_MAX_LMP_OUT_MESSAGES, 
        lmp_out_heaps[device_index], PRH_BS_CFG_SYS_SIZEOF_LMP_OUT_HEAP);

    /*
     * Reset the Baseband incoming L2CAP Queue
     * In the case of L2cap, its allocations being dynamic in nature, all of 
     * the allocated chunks and headers must be returned to the respective  
     * global list before the initialisation function can be called again
     */
    BTq_Get_and_Reset_Queue_Number_Of_Processed_Chunks(L2CAP_IN_Q, device_index);

    /* 
     * Need to deallocate anything allocated - order is not important 
     */
    q = mBTq_Get_Q_Ref(L2CAP_IN_Q, device_index); 
    cursor = q->headers;
    while(cursor)
    {
        t_q_descr *cursor_next;
        cursor_next = cursor->next;
        _BTq_Deallocate_Header(&BTq_l2cap_in, cursor);
        cursor = cursor_next;
    }
    _BTq_Initialise_Data_Queue(q);

    /* 
     * Reset the Baseband outgoing L2CAP Queue
     */
    BTq_Get_and_Reset_Queue_Number_Of_Processed_Chunks(L2CAP_OUT_Q, device_index);

    /* 
     * Need to deallocate anything allocated - order is not important 
     */
    q = mBTq_Get_Q_Ref(L2CAP_OUT_Q, device_index); 
    cursor = q->headers;
    while(cursor)
    {
        t_q_descr *cursor_next;
        cursor_next = cursor->next;
        _BTq_Deallocate_Header(&BTq_l2cap_out, cursor);
        cursor = cursor_next;
    }
    _BTq_Initialise_Data_Queue(q);
}
/*************************************************************************** 
 * BTq_Enqueue
 *
 * Returns an empty queue descriptor if one is available.
 * NOTE: some of the fields may have residual garbage.
 * It is guaranteed that the length field will be correct
 * and the callback field set to 0x0
 ****************************************************************************/
t_q_descr *BTq_Enqueue(u_int8 q_type, t_deviceIndex device_index, t_length length)
{
    t_data_queue  *p_data_queue = _BTq_Get_Data_Queue_Ref(q_type);
    t_queue       *q;
    t_q_descr     *qd = 0x0;
    u_int32       cpu_flags;

    /* 
     * ACL/L2cap or SCO queuing is a fixed size size chunk system 
     */
    if(p_data_queue)
    {
        q = BTQueues + q_type + device_index;

        /*
         * This block must be atomic to prevent the ACK function corrupting
         * the linked list pointers through pre-emption
         */
        SYSirq_Disable_Interrupts_Save_Flags(&cpu_flags);    
            
 		/*
		 *	Bug2896 - If there is already a pending commit on this queue, return zero
		 *  and send a buffer over flow event to the host.
		 */
		if (q->pending_commits != 0)
		{
			t_lm_event_info event_info;
            SYSirq_Interrupts_Restore_Flags(cpu_flags);    

			event_info.link_type = ACL_LINK;
#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==0)//merge CEVA 0517
			HCeg_Generate_Event(HCI_DATA_BUFFER_OVERFLOW_EVENT, &event_info);
#endif//merge CEVA 0517
			return (t_q_descr *) NULL;
		}
       
        /* 
         * Firstly, get a free header - this is the most efficient way to do 
         * this without sparse lookup tables 
         */
        qd = _BTq_Allocate_Header(p_data_queue, length);

        if(qd)
        {
            /*
             * Insert the descriptor into the single linked list of descriptors.
             */
            if(!q->headers)                        /* Then it's the first entry */
            {    
                q->headers = qd;
            }
            else
            {
                q->headers_tail->next = qd;
            }    

            q->headers_tail = qd;
            q->pending_commits++;    
            SYSirq_Interrupts_Restore_Flags(cpu_flags);    
        
            qd->next = 0x0;    
            
            /* 
             * Normal setting (exception and overriden if rx broadcast packet)
             */
            qd->device_index = device_index;    
            qd->length = (u_int8)length;
			qd->message_type = 0x00;
			qd->encrypt_status = LE_DATA_NOT_ENCRYPTED;//merge CEVA 0517

        }
        else
        {
            SYSirq_Interrupts_Restore_Flags(cpu_flags);    
        }
        return qd;
    }


    /* 
     * Other queues use a variable length chunk system 
     */
    else    
    {
        /*
         * To enqueue, check if what the current tail points to is free
         * if so, try to get some memory from the chunk
         */

        q = mBTq_Get_Q_Ref(q_type, device_index);		
		/*
		 *	Bug2896 - If there is already a pending commit on this queue, return zero
		 *  and send a buffer over flow event to the host.
		 */

		if (q->pending_commits != 0)
		{
			t_lm_event_info event_info;

			event_info.link_type = ACL_LINK;
#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==0)//merge CEVA 0517
			if (q_type != HCI_EVENT_Q)
				HCeg_Generate_Event(HCI_DATA_BUFFER_OVERFLOW_EVENT, &event_info);
#endif//merge CEVA 0517
			return (t_q_descr *) NULL;
		}

        qd = &q->headers[q->desc_tail];

        if(!qd->data)                      
        {
            /* 
             * If qd->data pointer is NULL then it's Free
             * For efficiency q_data member of t_q_descr should come first.
             * If the _pointer_ is 0x0 then it's free (NOT what pointer points to!)
             *
             * The callback is set to 0x0 by default so a user function can check 
             * the value of the function ptr to determine if associated callback
             */ 
            qd->length = (u_int8)length;
            qd->device_index = device_index;
            length = (length+3)&~0x3;        /* Round up to multiple of 4 */

            if(!q->num_entries)
            {
              //  if(q->data_size >= (s_int16)length)
            	if(q->data_size >= (s_int8)length)
                {
                    SYSatomic_Increment_u_int8(&q->pending_commits);    
                    qd->data = q->q_data;
                    q->tail_offset += length;
                    goto out;    
                }
            }
        
            if((q->head_offset - q->tail_offset) >= (s_int16)length)
            {
                SYSatomic_Increment_u_int8(&q->pending_commits);    
                qd->data = q->q_data + q->tail_offset;    
                q->tail_offset += (u_int8)length;
                goto out;
            }
            if(q->tail_offset > q->head_offset)
            {
                if((q->data_size - q->tail_offset) >= (s_int16)length)
                {
                    SYSatomic_Increment_u_int8(&q->pending_commits);    
                    qd->data = q->q_data + q->tail_offset;    
                    q->tail_offset += (u_int8)length;
                    goto out;    
                }
                if(q->head_offset >= (s_int16)length)
                {
                    SYSatomic_Increment_u_int8(&q->pending_commits);    
                    qd->data = q->q_data;
                    q->end_offset = q->tail_offset;
                    q->tail_offset = (u_int8)length;        /* Wrap the tail */
                    goto out;    
                }
            }
        }
    }
    
    return (t_q_descr*)0x0;    

out:

    if (++q->desc_tail == q->num_headers)
    {
        q->desc_tail = 0;
    }
    return qd;                          /* Return the descriptor I just filled */
}    

/*****************************************************************************
 * BTq_Commit
 *
 * This function commits an entry onto a queue by atomically increasing its
 * entry count.
 * Only after this function is executed will the entry become visible on the 
 * queue. This is to prevent another source attempting to remove the entry 
 * before its contents have been fully written.
 ****************************************************************************/
void BTq_Commit(u_int8 q_type, t_deviceIndex device_index)
{
    t_queue *q;
    u_int32 cpu_flags;

    q = mBTq_Get_Q_Ref(q_type, device_index);

    SYSirq_Disable_Interrupts_Save_Flags(&cpu_flags);
    q->pending_commits--;
    q->num_entries++;

    SYSirq_Interrupts_Restore_Flags(cpu_flags);

}

/*****************************************************************************
 * BTq_Dequeue_Next
 *
 * Dequeue an entry from the selected queue
 *
 * This function is used to dequeue a full entry from the queue without
 * any segmentation.
 *
 * Note:  
 *    B1893: No longer used for remote loopback, hence remove check for 
 *    BTq_Change_Endianness_If_Required() 
 ****************************************************************************/
t_q_descr *BTq_Dequeue_Next(u_int8 q_type, t_deviceIndex device_index)
{
    t_data_queue *p_data_queue = _BTq_Get_Data_Queue_Ref(q_type);
    t_queue *q;

    q = mBTq_Get_Q_Ref(q_type, device_index);

    /*
     * If entries on queue Then
     *    Return pointer to first entry
     * Endif
     */
    if(q->num_entries)
    {
#if 0 // Not Needed for LE
        q->original_length = 0;
#endif
        if(p_data_queue)
        {
            return q->headers;    
        }
        else 
        {
            u_int32 cpu_flags;
            /* 
             * Do the get read ptr here - we need to protect this section as BTq_Enqueue 
             * could pre-empt it for this index, and fail, due to the wrap not occurring, 
             * or pre-empting between altering head_offset and end_offset
             */
            SYSirq_Disable_Interrupts_Save_Flags(&cpu_flags);
            if(q->head_offset == q->end_offset)
            {
                q->head_offset = 0;
                q->end_offset = q->data_size;
            }
            SYSirq_Interrupts_Restore_Flags(cpu_flags);

            return &q->headers[q->desc_head];    
        }
    }
    return (t_q_descr *)0x0;
}

/*****************************************************************************
 * BTq_LE_Dequeue_Next
 *
 * Special function for baseband dequeue: Additional argument is a bitmap of
 * packet types.
 * This function will select the most appropriate depending on the types
 * specified and the state of the queue in question.
 *
 * q_type           LMP_OUT_Q   L2CAP_OUT_Q    SCO_OUT_Q
 * device_index     All baseband queues have an associated device_index
 * in_packet_types  Packet types to be used for tx
 ****************************************************************************/
t_q_descr *BTq_LE_Dequeue_Next(u_int8 q_type, t_deviceIndex device_index,
                                   u_int16 length)
{
    t_queue *q;
    t_q_descr *qd;

   // u_int16 packet_type_length;
    u_int32 cpu_flags;

    q = BTQueues + q_type + device_index;

    if(q->num_entries)            /* First check what should be done.. */
    {
        if(q_type == LMP_OUT_Q)
        {
            qd = &q->headers[q->desc_head];

            /*
             * If the head has reached the end, it is not read from it now,
             * Must first wrap and then read from it
             */
            SYSirq_Disable_Interrupts_Save_Flags(&cpu_flags);
            if(q->head_offset == q->end_offset)
            {
                q->head_offset = 0;
                q->end_offset = q->data_size;
            }
            SYSirq_Interrupts_Restore_Flags(cpu_flags);

            return qd;
        }

        /*
         * Every thing here relates to L2CAP/ACL or SCO data only
         */

        /*
         * Take the descriptor from head of linked list
         */
        qd = q->headers;

        return qd;
    }

    return (t_q_descr *)0x0;
}

/*****************************************************************************
 * BTq_Dequeue_Next_Data_By_Length
 *
 * Special function for HC to HOST Data dequeuing. 
 * Additional argument a maximum length to segment to, or if supported,
 * to reassemble to.
 *
 * q_type               Normally L2CAP_IN_Q
 * device_index         Associated device index for the q_type
 * max_packet_length    If not 0 Then Segment/[Reassemble] to presented length
 *
 * This function will never roll-back to start of packet
 ****************************************************************************/
t_q_descr *BTq_Dequeue_Next_Data_By_Length(
    u_int8 q_type, t_deviceIndex device_index, u_int16 max_packet_length)
{
    t_queue   *q;
    t_q_descr *qd;

    q = BTQueues + q_type + device_index;

    /*
     * If there are entries on the queue Then
     *    Extract an appropriate queue descriptor
     * Else
     *    Return an empty descriptor
     * Endif
     */
    if(q->num_entries)
    {    
        qd = q->headers;        

#if (PRH_BS_DEV_BIG_ENDIAN_AND_DIRECT_BASEBAND_MOVE==1)
        /*
         * If Direct Baseband Copy (Big Endian) and Not Remote Loopback Then 
         *    If First Segment of ACL packet Then
         *         Change Data to Little Endian 
         *
         * Note must be done before qd->length is adjusted during segmentation
         * SCO endianess is resolved directly on baseband copies
         */
        if (q_type == L2CAP_IN_Q && qd->data_start_offset == 0 &&
                BTtst_Read_Loopback_Mode() != REMOTE_LOOPBACK)
        {
            BTq_Change_Endianness_If_Required(qd->data, qd->length);
        }
#endif

    }
    else
    {
        /*
         * Queue is empty, nothing to transmit.
         */
        qd = (t_q_descr *)0;
    }

    return qd;
}

/*****************************************************************************
 * BTq_Ack_Last_Dequeued
 *
 * Acknowledge dequeue entry, removing entry from queue.
 *
 * q_type               All queue types
 * device_index         Associated device index for the q_type
 * length               Length of entry being acknowledged (historical)
 *
 ****************************************************************************/
void BTq_Ack_Last_Dequeued(u_int8 q_type, t_deviceIndex device_index, t_length length)
{
    t_data_queue  *p_data_queue = _BTq_Get_Data_Queue_Ref(q_type);
    t_queue   *q;
    t_q_descr *qd=0;
    u_int32 cpu_flags;

    q = mBTq_Get_Q_Ref(q_type, device_index);

    if(q->num_entries == 0)
    {

        return;
    }

    {
        /*
         * Hasn't been segmented, has it been aggregated ?
         */
        if (p_data_queue)
        {
            SYSirq_Disable_Interrupts_Save_Flags(&cpu_flags);
           
            qd = q->headers;
            q->headers = qd->next;

            if (q->headers == 0)
            {
                q->headers_tail = 0x0;
            }

            _BTq_Deallocate_Header(p_data_queue, qd);
            /*
             * The following code block tracks how many "chunks" have been
             * consumed (i.e. delivered successfully by the link controller.
             * Zero length packets must also be tracked.
             */
            p_data_queue->num_of_consumed_chunks++;                  
            q->num_processed_chunks++; 

            q->num_entries--;
            SYSirq_Interrupts_Restore_Flags(cpu_flags);
        }    
        else /* Variable queue */
        {
           SYSirq_Disable_Interrupts_Save_Flags(&cpu_flags);    
         
            /*
             * Lengths always aligned to 4 byte chunks
             */
            length = (length+3)&~0x3;        
            qd = &q->headers[q->desc_head]; 
            q->head_offset += (u_int8)length;
            if (++q->desc_head == q->num_headers)
            {
                q->desc_head = 0;
            }

            /* 
             * Zero the data pointer to indicate free block 
             */
            qd->data = 0x0;

            /* 
             * Decrement number of entries and reset head/tail to zero if no entries 
             */
            q->num_entries--;
            if(!q->num_entries && !q->pending_commits)
            {
                q->head_offset = q->tail_offset = 0;
                q->end_offset = q->data_size;
            }
            SYSirq_Interrupts_Restore_Flags(cpu_flags);
        }

    }
}



/*****************************************************************************
 * BTq_Get_Queue_Number_Of_Entries
 *
 * Get the number of entries on queue.
 ****************************************************************************/
u_int16 BTq_Get_Queue_Number_Of_Entries(
        u_int8 q_type, t_deviceIndex device_index)
{
    return BTQueues[q_type+device_index].num_entries;
}

