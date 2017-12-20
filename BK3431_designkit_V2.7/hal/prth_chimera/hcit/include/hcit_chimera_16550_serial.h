#ifndef _HCIT_PARTHUS_CHIMERA_16550_SERIAL_H
#define _HCIT_PARTHUS_CHIMERA_16550_SERIAL_H

/******************************************************************************
 * MODULE NAME:    hcit_chimera_16550_serial.h
 * PROJECT CODE:   Bluestream
 * DESCRIPTION:    HCI RS232 Transport header for 16550 compatible UART on Chimera
 * AUTHOR:         John Sheehy
 * DATE:           09 March 2001
 *
 * SOURCE CONTROL: $Id: hcit_chimera_16550_serial.h,v 1.7 2009/04/28 16:08:48 morrisc Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2001-2004 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    09 March 2001 - Rewritten for new framework
 *
 ******************************************************************************/

#include "sys_config.h"    
#include "sys_types.h"
#include "sys_features.h"

/*
 * Base addresses for the two UARTs
 * It is important to note here that only UART1 has a 
 * 16 byte FIFO. The other two have only 4 bytes. Another problem is that
 * only UART2 has the modem control features.
 */

#define XR7_UART1_BASE    0x10400
#define XR7_UART2_BASE    0x10500


#if XR7_HCIT_UART == 1
    #define XR7_HCIT_UART_BASE XR7_UART1_BASE
#elif XR7_HCIT_UART == 2
    #define XR7_HCIT_UART_BASE XR7_UART2_BASE
#endif
 
/*
 * Offsets of registers 
 */
 
#define XR7_HCIT_RBRTHR 0x00
#define XR7_HCIT_IER    0x04
#define XR7_HCIT_IIR    0x08
#define XR7_HCIT_FCR    0x08
#define XR7_HCIT_LCR    0x0C
#define XR7_HCIT_MCR    0x10
#define XR7_HCIT_LSR    0x14
#define XR7_HCIT_MSR    0x18

#define XR7_HCIT_DLL    0x00

/*
 * NCO is common to all three UARTs MSB of divisor latch is common, LSB is individual to each UART
 */

#define XR7_COMMON_DLM  0x04
#define XR7_COMMON_LCR  0x0C

/* 
 * Interrupt Enable Bits 
 */

#define XR7_IER_ERBFI 0x01
#define XR7_IER_ETBEI 0x02
#define XR7_IER_ELSI  0x04
#define XR7_IER_EDSSI 0x08

/* 
 * Interrupt ID bits 
 */

#define XR7_IIR_INT_NOT_PENDING 0x01
#define XR7_IIR_MODEM           0x00
#define XR7_IIR_THRE            0x02
#define XR7_IIR_RECVBUF         0x04
#define XR7_IIR_RLS             0x06
#define XR7_IIR_ID_MASK         0x06

/* 
 * Line control LCR 
 */

#define XR7_LCR_DLAB          0x80
#define XR7_LCR_SB            0x40
#define XR7_LCR_PAR_MASK      0x30
#define XR7_LCR_PEN           0x08
#define XR7_LCR_STB           0x04
#define XR7_LCR_CHAR_LEN_MASK 0x03

#define XR7_LCR_5BITS         0x0
#define XR7_LCR_6BITS         0x2
#define XR7_LCR_7BITS         0x1
#define XR7_LCR_8BITS         0x3

#define XR7_LCR_1STOP         0x0
#define XR7_LCR_2STOP         0x4

#define XR7_LCR_PODD          0x0
#define XR7_LCR_PEVEN         0x10
#define XR7_LCR_PFORCE1       0x20
#define XR7_LCR_PFORCE0       0x30

/* 
 * Modem Control MCR 
 */

#define XR7_MCR_LOOP 0x10
#define XR7_MCR_OUT2 0x08
#define XR7_MCR_OUT1 0x04
#define XR7_MCR_RTS  0x02
#define XR7_MCR_DTR  0x01

/* 
 * Line status LSR 
 */

#define XR7_LSR_TEMT 0x40
#define XR7_LSR_THRE 0x20
#define XR7_LSR_BI   0x10
#define XR7_LSR_FE   0x08
#define XR7_LSR_PE   0x04
#define XR7_LSR_OE   0x02
#define XR7_LSR_DR   0x01

/* 
 * Modem status MSR 
 */

#define XR7_MSR_DCD  0x80
#define XR7_MSR_RI   0x40
#define XR7_MSR_DSR  0x20
#define XR7_MSR_CTS  0x10
#define XR7_MSR_DDCD 0x08
#define XR7_MSR_TERI 0x04
#define XR7_MSR_DDSR 0x02
#define XR7_MSR_DCTS 0x01


/*
 * FIFO Control Register FCR
 * If UART2 or UART3 are used, then
 * the FIFO levels are 1,2,3 and 4
 * as there is only a 4 byte FIFO on
 * these UARTs
 */

#define XR7_FCR_ENABLE         0x1
#define XR7_FCR_RX_FIFO_RESET  0x2
#define XR7_FCR_TX_FIFO_RESET  0x4

#define XR7_FCR_RECV_TRIGL_14  0x40
#define XR7_FCR_RECV_TRIGM_14  0x80

#define XR7_FCR_RECV_TRIGL_8   0x40
#define XR7_FCR_RECV_TRIGM_8   0x00

#define XR7_FCR_RECV_TRIGL_4   0x00
#define XR7_FCR_RECV_TRIGM_4   0x80

#define XR7_FCR_RECV_TRIGL_1   0x00
#define XR7_FCR_RECV_TRIGM_1   0x00


/*
 * UART register access macro
 * The first pair get/set registers from the chosen UART, the latter
 * access the "common UART" registers (which are located at UART1 )
 * 
 * NOTE: The u_int16* is required as only 16 and 32 bit register accesses
 * will activate the correct signals needed to access the UART on the XR7. The unused
 * 8 bits are thrown away.
 */

#define mHCIT_CHIMERA_GET_REG(x)      (*(volatile u_int16*)(XR7_HCIT_UART_BASE + (x)))
#define mHCIT_CHIMERA_SET_REG(r,x)    (*(volatile u_int16*)(XR7_HCIT_UART_BASE + (r))) = (u_int8)(x) 

#define mHCIT_CHIMERA_GET_REG_COMMON(x)    (*(volatile u_int16*)(XR7_UART1_BASE + (x)))
#define mHCIT_CHIMERA_SET_REG_COMMON(r,x)  (*(volatile u_int16*)(XR7_UART1_BASE + (r))) = (u_int8)(x) 

/*
 * Function Interfaces
 */

void HCIT_Chimera_16550_Initialise(u_int32 baud_rate);
void HCIT_Chimera_16550_Shutdown(void);

void HCIT_Chimera_16550_Tx_Block(volatile u_int8 **bufp, volatile u_int32 *length, void (*txcb)(void));
void HCIT_Chimera_16550_Tx_Char_Polled(volatile u_int8 **bufp, volatile u_int32 *length);

u_int16 HCIT_Chimera_16550_Get_Char(void);

void HCIT_Chimera_16550_Interrupt_Handler(void);
void HCIT_Chimera_16550_Empty_FIFO(void);

#if (PRH_BS_DEV_PROCESS_UART_RX_DATA_IN_IRQ_CONTEXT == 1)
void HCIT_Chimera_16550_Service_Rx_Fifo_Cache(void);
#endif

#endif /* _PARTHUS_HCIT_CHIMERA_SERIAL_H */
