/*
 * MODULE NAME:  hcit_chimera_16550_serial.c
 * PROJECT CODE: BlueStream
 * DESCRIPTION:  HCI RS232 Transport UART driver for 16550 compatible UART on Chimera
 * AUTHOR:       John Sheehy
 * DATE:         09 March 2001
 *
 * SOURCE CONTROL: $Id: hcit_chimera_16550_serial.c,v 1.21 2009/04/28 16:08:46 morrisc Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001-2004 Ceva Inc.
 *     All rights reserved.
 *
 * NOTE TO USERS: In this driver, the constants XR7_HCIT are used. These do not identify
 *                any particular UART on the XR7 (there are two). To use the other UART,
 *                the XR7_HCIT_UART should be changed appropriately in the sys_hal_features.h header file.
 *                Also, the baud rate and receive FIFO trigger level are found here.
 */

#include "sys_config.h"

#if (BUILD_TYPE!=GENERIC_BUILD)

#include "sys_types.h"
#include "sys_features.h"
#include "sys_irq.h"

#include "tra_hcit.h"
#include "tra_uart.h"

#include "hcit_chimera_16550_serial.h"
#include "BKreg.h" //charles add
#include "BKAPI.h" //charles add
#include "debug.h"
/*
 * End of transmit callback function pointer.
 */

void (*hcit_chimera_16550_tx_fptr)(void);

/*
 * State of CTS (are we allowed to send ?)
 */

// #if HCIT_CHIMERA_USE_CTSRTS == 1

// static volatile u_int8 hcit_chimera_16550_can_send;

// #endif

/*
 * Implements function pointer from tra_hcit.c, - when called, it registers
 * this HCI Transport system (UART)
 * This function returns a void pointer to the UART structure
 * In tra_hcit.c this is cast to the generic tra_hcit_info structure
 * (which must be the first member or sys_hcit_uart_info)
 */

/*
 * Reference the uart_info structure (comes from tra_uart.c)
 * It is referenced rather than defined here as its space reservation
 * is in the domain of its parent module (tra_uart.c)
 */

extern struct tra_hcit_uart_info sys_hcit_uart_info;


#if (PRH_BS_DEV_PROCESS_UART_RX_DATA_IN_IRQ_CONTEXT == 1)
u_int8 rx_fifo_cache[40]; /* 460Kbits/sec = approx 37 bytes/slot.  Between 2 & 5 IRQ/slot.
                             Therfore, 40 byte buffer should be more than sufficient given
                             that we'll service the buffer at least twice a slot*/
u_int8 *FIQ_insert, *IRQ_process, *rx_fifo_cache_end, *rx_fifo_cache_start;

void _Insert_Rx_Char(u_int8 rx_char);
#endif

void* TRAhcit_Register_UART_Driver(void)
{
    /*
     * Set up common members first, then UART specific ones.
     */

    sys_hcit_uart_info.common.type = TRA_HCIT_TYPE_UART;
    sys_hcit_uart_info.common.access_type = TRA_HCIT_FAST_IRQ;
    sys_hcit_uart_info.common.interrupt_handler = HCIT_Chimera_16550_Interrupt_Handler;

    sys_hcit_uart_info.initialise = HCIT_Chimera_16550_Initialise;
    sys_hcit_uart_info.shutdown = HCIT_Chimera_16550_Shutdown;
#if TRA_HCIT_UART_POLLED_TX_SUPPORTED == 1//flash bulid : TRA_HCIT_UART_POLLED_TX_SUPPORTED = 1
    sys_hcit_uart_info.poll_tx = HCIT_Chimera_16550_Tx_Char_Polled;
#else
    sys_hcit_uart_info.transmit = HCIT_Chimera_16550_Tx_Block;
#endif

#if (PRH_BS_DEV_SERVICE_TRANSPORT_DURING_SPINS_SUPPORTED==1)
    sys_hcit_uart_info.common.poll_rx = HCIT_Chimera_16550_Empty_FIFO;
    sys_hcit_uart_info.common.suspend = 0; /* no callback registered */
    sys_hcit_uart_info.common.resume = 0; /* no callback registered */
#endif

    return (void*)&sys_hcit_uart_info;
}

void Beken_UART1_Initial(void)
{
    u_int16 uart_clk_div;

    uart_clk_div = 0x8A;                            // 16M/115200=138.89=0x8A
    REG_APB5_GPIOA_CFG  &= 0xFFFFFFFC;
    REG_APB5_GPIOA_DATA = 0x00FFFFFF;
    REG_UART1_CFG = (1<<UART_TX_EN_BIT)|(1<<UART_RX_EN_BIT)|(UART_FRAME_LEN_8_BIT<<UART_DATA_LEN_BIT)|(uart_clk_div<<UART_CLK_DIV_BIT);
    REG_UART1_FIFO_THRESHOLD = (1<<BIT_TX_FIFO_THRESHOLD)|(32<<BIT_RX_FIFO_THRESHOLD);
    REG_ICU_UART1CLKCON   = 0x0 ;                   // UART1 clock enable
    REG_ICU_INT_ENABLE |= INT_STATUS_UART1_bit;    // IRQ UART interrupt enable
    //REG_UART1_WAKE_CTRL = 0x0fffff;                // Disable wakeup
    REG_UART1_INT_ENABLE |= (SET_RX_FIFO_NEED_READ_EN | SET_UART_RX_STOP_END_EN) ; //UART Data RX interrupt enable
    //   REG_UART1_INT_ENABLE |= SET_UART_RX_STOP_END_EN; // Enable Rx Stop for 4-Byte-time of no-data */


#if 0
    // GPIO-A: [0]-UART Output(Tx). [1]-UART Input(Rx)
    REG_APB5_GPIOA_CFG  &= 0xFFFFFFFC;
    REG_APB5_GPIOA_DATA = 0x00FFFFFF;
    uart_clk_div    = 0x8A;  // 16M/115200=138.89=0x8A
    uart_conf_value = 0x0;
    uart_conf_value  = REG_APB3_UART_CFG;
    uart_conf_value |= SET_UART_TX_ENABLE;
    uart_conf_value |= SET_UART_RX_ENABLE;
    uart_conf_value &= (~SET_UART_IRDA);
    uart_conf_value |= SET_UART_LEN;
    //uart_conf_value |= SET_UART_PAR_EN;
    //uart_conf_value |= SET_UART_PAR_MODE;
    uart_conf_value &= (~SET_UART_STOP_LEN);
    uart_conf_value  = (uart_conf_value & (~SET_UART_CLK_DIVID)) | (uart_clk_div << BIT_UART_CLK_DIVID);
    REG_APB3_UART_CFG = uart_conf_value;

    REG_APB3_UART_FIFO_THRESHOLD = (1<<BIT_TX_FIFO_THRESHOLD)|(32<<BIT_RX_FIFO_THRESHOLD);

    REG_AHB0_ICU_UARTCLKCON   = 0x0;                      // UART1 clock enable
    REG_AHB0_ICU_INT_ENABLE  |= INT_STATUS_UART_bit;      // IRQ UART interrupt enable

    REG_APB3_UART_INT_ENABLE |= SET_RX_FIFO_NEED_READ_EN; // UART Data RX interrupt enable
    REG_APB3_UART_INT_ENABLE |= SET_UART_RX_STOP_END_EN;  // Enable Rx Stop for 4-Byte-time of no-data
#endif
}

/*
 * HCIT_Chimera_16550_Initialise
 *
 * paramters: baud_rate = baud rate
 * returns: void
 *
 * Initialises the 16550 to the specified baud rate, sets up interrupt driven FIFO mode
 */

void HCIT_Chimera_16550_Initialise(u_int32 baud_rate)
{
    /*
     * Clear the fptr in case an MSR interrupt happens before the upper layer
     * has initialised the pointer via a call to Tx_Block. We can validly
     * check against NULL.
     */
    hcit_chimera_16550_tx_fptr = 0x0;
#if (PRH_BS_DEV_PROCESS_UART_RX_DATA_IN_IRQ_CONTEXT == 1)
    rx_fifo_cache_start = &rx_fifo_cache[0];
    rx_fifo_cache_end = &rx_fifo_cache[(sizeof(rx_fifo_cache)/sizeof(rx_fifo_cache[0]))-1];
    FIQ_insert = rx_fifo_cache_start;
    IRQ_process = rx_fifo_cache_start;
#endif

    // #if HCIT_CHIMERA_USE_CTSRTS == 1
    //     /*
    //      * Enable CTS/RTS pins (these registers are set directly, see header file)
    //      */
    //     /*
    //      * SREGB holds the configuration for IOBUSOUT[5]. Using option
    //      * 2 (non PORT_A signals), this brings out RTS for UART2.
    //      * changed by D.A.. need to use IOBUSOUT[11] - option 2 on SREGC
    //      */
    //     XR7_SREGC |= 0x040;
    //     /*
    //      * IOBUSIN[3] is wired to CTS of UART2 when set to option 3
    //      * SREGD sets option3 for IOBUSIN
    //      * changed by D.A. need to use IOBUSOUT[10]
    //      */
    //     XR7_SREGD |= 0x400;
    // #endif
/*#ifdef UART_ENABLE
    Beken_UART1_Initial();
#endif*/
}

/*
 * HCIT_Chimera_16550_Shutdown
 *
 * paramters: void
 * returns: void
 *
 * This function shuts down UART operation by disabling Interrupts.
 */

void HCIT_Chimera_16550_Shutdown(void)
{
    //mHCIT_CHIMERA_SET_REG(XR7_HCIT_IER,0);             /* Disable 16550 Interrupts */
    REG_APB3_UART_INT_ENABLE=0;     //charles merge from BK3515        /* Disable 16550 Interrupts */
}

#if TRA_HCIT_UART_POLLED_TX_SUPPORTED != 1 //charles merge from BK3515

/*
 * HCIT_Chimera_16550_Tx_Block
 *
 * parameters: buf (pointer to pointer to buffer - double pointer needed to alter the original pointer)
 *             length = pointer to length field (updated by amount transmitted)
 *             txcb = function to call on end of transmission interrupt
 * Returns: void
 *
 * Block transmission function. Takes pointer and changes it (adds) by amount transmitted.
 * Decrements length accordingly
 */

void HCIT_Chimera_16550_Tx_Block(volatile u_int8 **buf, volatile u_int32 *length, void (*txcb)(void))
{

    // #if HCIT_CHIMERA_USE_CTSRTS == 1
    //     if(hcit_chimera_16550_can_send)
    // #endif
    {
        u_int32 count = *length;

        hcit_chimera_16550_tx_fptr = txcb;

        //if(count >= HCIT_CHIMERA_16550_TX_FIFO_LENGTH)
        //   count = HCIT_CHIMERA_16550_TX_FIFO_LENGTH;
        if(count >= (HCIT_CHIMERA_16550_TX_FIFO_LENGTH- (REG_APB3_UART_FIFO_COUNT&0xff) ) ) //charles merge from BK3515
            count = HCIT_CHIMERA_16550_TX_FIFO_LENGTH- (REG_APB3_UART_FIFO_COUNT&0xff);

        (*length)-= count;
        //clear TX interrupt
        REG_APB3_UART_INT_FLAG=REG_APB3_UART_INT_FLAG|UART_INTR_RX_FIFO_NEED_READ_wakeup_bit; //charles merge from BK3515

        do
        {
            //mHCIT_CHIMERA_SET_REG(XR7_HCIT_RBRTHR,*((*buf)++));
            UART_WRITE_BYTE(*((*buf)++)); //charles merge from BK3515
            count--;
        }
        while(count);

        /*
         * Turn on transmitter interrupts
         */

        //        mHCIT_CHIMERA_SET_REG(XR7_HCIT_IER, mHCIT_CHIMERA_GET_REG(XR7_HCIT_IER) | XR7_IER_ETBEI);
        REG_APB3_UART_INT_ENABLE=REG_APB3_UART_INT_ENABLE|UART_INTR_TX_FIFO_NEED_WRITE_bit|UART_INTR_TX_FIFO_NEED_WRITE_wakeup_bit;//charles merge from BK3515 //enable tx interrupt
    }
}
#endif //charles merge from BK3515


#if TRA_HCIT_UART_POLLED_TX_SUPPORTED == 1


/*
 * HCIT_Chimera_16550_Tx_Char_Polled
 *
 * paramters: buf = double pointer to buffer (See block transmit function)
 *            length = length pointer
 * returns: void
 *
 * This function waits until the UART is ready to receive another character and
 * then writes this character to hte register
 */

void HCIT_Chimera_16550_Tx_Char_Polled(volatile u_int8 **buf, volatile u_int32 *length)
{
    u_int8 ch;//charles merge from BK3515
    u_int32 count = *length;
    u_int32 fifo_length;//charles merge from BK3515

    // #if HCIT_CHIMERA_USE_CTSRTS == 1
    //     if(hcit_chimera_16550_can_send)
    // #endif
    {
        //while(!(mHCIT_CHIMERA_GET_REG(XR7_HCIT_LSR) & XR7_LSR_THRE));    /* Wait until the UART transmitter is empty */
        // do //charles merge from BK3515
        // {
        //     fifo_length=UART_TX_FIFO_COUNT&0xff;
        // }while(fifo_length>(HCIT_CHIMERA_16550_TX_FIFO_LENGTH/2)); //currently XR7_HCIT_UART == 1,HCIT_CHIMERA_16550_TX_FIFO_LENGTH = 16    /* Wait until the UART transmitter is empty */

        do //charles merge from BK3515
        {
            fifo_length=UART_TX_FIFO_COUNT&0xff;

        }while(fifo_length!=0); //currently XR7_HCIT_UART == 1,HCIT_CHIMERA_16550_TX_FIFO_LENGTH = 16    /* Wait until the UART transmitter is empty */
        /*
         * Once the transmitter is empty, fill the FIFO with characters
         */

        if(count >= HCIT_CHIMERA_16550_TX_FIFO_LENGTH)
            count = HCIT_CHIMERA_16550_TX_FIFO_LENGTH;

        (*length)-= count;

        do
        {

            //mHCIT_CHIMERA_SET_REG(XR7_HCIT_RBRTHR,*((*buf)++));
            ch= *((*buf)++);//charles merge from BK3515
            UART_WRITE_BYTE(ch);//charles merge from BK3515
            count--;

            do //charles merge from BK3515
            {
                fifo_length=UART_TX_FIFO_COUNT&0xff;

            }while(fifo_length!=0); //currently XR7_HCIT_UART == 1,HCIT_CHIMERA_16550_TX_FIFO_LENGTH = 16    /* Wait until the UART transmitter is
        }while(count);
    }
}

#endif


/*
 * HCIT_Chimera_16550_Interrupt_Handler
 *
 * paramters: void
 * returns: void
 *
 * Handle interrupts from the 16550 UART
 */
void HCIT_Chimera_16550_Interrupt_Handler(void)
{
    u_int8 status;
    //u_int8 ch;
    u_int8 uart_value;//merge from BK3515
    u_int32 process_counter=0; //merge from BK3515

    /*
     * Level triggered interrupt, no need to clear any ICR condition
     */

    //status = mHCIT_CHIMERA_GET_REG(XR7_HCIT_IIR) & 0x7;    /* Not interested in upper 5 bits */
    status=REG_APB3_UART_INT_FLAG;//charles merge from BK3515
    //charles merge from BK3515 start
    if(status&UART_INTR_RX_PAR_ERR_bit)
    {
        uart_value=UART_READ_BYTE();//read out one bytes
    }
    else
    {
        if(status&(UART_INTR_RX_FIFO_NEED_READ_bit| UART_INTR_RX_FIFO_NEED_READ_wakeup_bit))
        {
            while(REG_APB3_UART_FIFO_COUNT & (1<<UART_BIT_FIFO_RD_READY))
            {
                uart_value=UART_READ_BYTE();
                TRAhcit_UART_Rx_Char(uart_value);
                if(process_counter++>50)
                    break;
            }
        }
    }
    REG_APB3_UART_INT_FLAG =status;
    //charles merge from BK3515 end

    //charles remove  lookup BK3515   start
    //while(!((status) & (XR7_IIR_INT_NOT_PENDING)))//charles remove  lookup BK3515                    /* While there are interrupts pending */
    //    {
    //switch(status)
    //        {
#if HCIT_CHIMERA_USE_CTSRTS == 1
    //    case XR7_IIR_MODEM:

    //        ch = mHCIT_CHIMERA_GET_REG(XR7_HCIT_MSR);

    /* Got a modem status interrupt, check CTS */

    //        if(ch & XR7_MSR_DCTS)
    //        {
    //            if(ch & XR7_MSR_CTS)                             /* If CTS goes high */
    //            {
    //                hcit_chimera_16550_can_send = 1;
    //                HWled6_On();
    /*
     * If polled mode is in use, then we do not need to call the transmit
     * callback, as polled transmit spins while it cannot send (based on the
     * can send variable above). All that is necessary is that the can_send
     * variable is set to one, and polled transmission should automatically
     * continue after this interrupt.
     */

#if TRA_HCIT_UART_POLLED_TX_SUPPORTED == 0

    //               if(hcit_chimera_16550_tx_fptr)
    //                  (*hcit_chimera_16550_tx_fptr)();
#endif
    //          }
    //          else
    //      {
    //          HWled6_Off();
    //          hcit_chimera_16550_can_send = 0;
    //      }
    //  }
    //  break;
#endif
    //case XR7_IIR_RLS:                                          /* This only fires with break or an error */
    //    ch = mHCIT_CHIMERA_GET_REG(XR7_HCIT_LSR);              /* Clear interrupt by reading LSR */
    //    if(ch & XR7_LSR_FE)
    //        TRAhcit_UART_Indicate_Error(TRA_HCIT_UART_FRAMING_ERROR);
    //    if(ch & XR7_LSR_OE)
    //        TRAhcit_UART_Indicate_Error(TRA_HCIT_UART_OVERRUN_ERROR);

    //    break;
    //case XR7_IIR_RECVBUF:
    //    while((mHCIT_CHIMERA_GET_REG(XR7_HCIT_LSR)) & XR7_LSR_DR) /* Read all characters out of the fifo */
    //#if (PRH_BS_DEV_PROCESS_UART_RX_DATA_IN_IRQ_CONTEXT ==0)
    //                TRAhcit_UART_Rx_Char(mHCIT_CHIMERA_GET_REG(XR7_HCIT_RBRTHR));
    //#else
    //                _Insert_Rx_Char(mHCIT_CHIMERA_GET_REG(XR7_HCIT_RBRTHR));
    //#endif
    //            break;

    //            case XR7_IIR_THRE:                                         /* Turn off tx interrupts again */
    //                /* Why do this ? maybe avoids one lockup but replaces it with another */
    //                (*hcit_chimera_16550_tx_fptr)();
    //                break;
    //        }
    //        status = mHCIT_CHIMERA_GET_REG(XR7_HCIT_IIR) & 0x7;            /* Re-read status */
    //    }
    //charles remove  lookup BK3515  end

}

void HCIT_Chimera_16550_Empty_FIFO(void)
{
    //while((mHCIT_CHIMERA_GET_REG(XR7_HCIT_LSR)) & XR7_LSR_DR) /* Read all characters out of the fifo */
    while(UART_RX_FIFO_COUNT!=0) /* Read all characters out of the fifo */
        TRAhcit_UART_Rx_Char(UART_READ_BYTE() );  //charles merge from BK3515
}


#if (PRH_BS_DEV_PROCESS_UART_RX_DATA_IN_IRQ_CONTEXT == 1)//currently define 0 in Sys_feature.h
void _Insert_Rx_Char(u_int8 rx_char)
{
    *FIQ_insert = rx_char;
    if (FIQ_insert == rx_fifo_cache_end)
        FIQ_insert = rx_fifo_cache_start;
    else
        FIQ_insert++;
}

void HCIT_Chimera_16550_Service_Rx_Fifo_Cache(void)
{
    while(IRQ_process != FIQ_insert)
    {
        TRAhcit_UART_Rx_Char(*IRQ_process);
        if (IRQ_process == rx_fifo_cache_end)
            IRQ_process = rx_fifo_cache_start;
        else
            IRQ_process++;
    }
}
#endif
#endif
