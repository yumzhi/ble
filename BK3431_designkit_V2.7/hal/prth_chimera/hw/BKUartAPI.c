#include "BKUart.h"
#include "BKAPI.h"
#include "le_config.h"
#include "debug.h"
#include "sys_power.h"
#include "sys_types.h"
#include "hw_lc_impl.h"
#include "hcit_chimera_16550_serial.h"
#include "Tra_hcit.h"
#include "sys_mmi_impl.h"
#include "BKAPI.h"
#include "hw_radio_defs.h"
#include "lslc_irq.h"

#define RX_BUF_LEN  64

static u_int8  rx_buf[RX_BUF_LEN];
static u_int8  rx_index = 0;


void Beken_UART_Initial(void)
{
    u_int16 uart_clk_div;

    //uart_clk_div = 0x683;     //9600bps          
    uart_clk_div = 0x8A;
	// 16M/115200=138.89=0x8A
      //		uart_clk_div = 0x200;
    REG_APB5_GPIOA_CFG  &= 0xFCFCFFFC;
    REG_APB5_GPIOA_CFG |= (0x3<<8);
    REG_APB5_GPIOA_DATA &= 0x00FCFFFF;
    
    REG_UART1_CFG = (1<<UART_TX_EN_BIT)|(1<<UART_RX_EN_BIT)|(UART_FRAME_LEN_8_BIT<<UART_DATA_LEN_BIT)|(uart_clk_div<<UART_CLK_DIV_BIT);
    REG_UART1_FIFO_THRESHOLD = (1<<BIT_TX_FIFO_THRESHOLD)|(32<<BIT_RX_FIFO_THRESHOLD);
    REG_ICU_UART1CLKCON   = 0x0 ;                   // UART1 clock enable
    REG_ICU_INT_ENABLE |= INT_STATUS_UART1_bit;    // IRQ UART interrupt enable
    //REG_UART1_WAKE_CTRL = 0x0fffff;                // Disable wakeup
    REG_UART1_INT_ENABLE |= (SET_RX_FIFO_NEED_READ_EN | SET_UART_RX_STOP_END_EN) ; //UART Data RX interrupt enable
    //   REG_UART1_INT_ENABLE |= SET_UART_RX_STOP_END_EN; // Enable Rx Stop for 4-Byte-time of no-data */
}

void Beken_UART_Close(void)
{
    REG_ICU_UART1CLKCON = 0x01;
    REG_ICU_INT_ENABLE &= ~INT_STATUS_UART1_bit;
}

void Beken_UART_Interrupt_Handler(void)
{
    u_int8  uart_int_status ; 
    u_int8  uart_fifo_rdata ;
    uart_int_status = REG_APB3_UART_INT_FLAG  ;

    //DEBUG_MSG(0x5A);
    //DEBUG_MSG(uart_int_status);

    if(uart_int_status & SET_TX_FIFO_NEED_WRITE_EN)
    {
        UART_TX_INT_DISENABLE;
    }

    if(uart_int_status & (SET_RX_FIFO_NEED_READ_EN | SET_UART_RX_STOP_END_EN ))
    {
        while(REG_APB3_UART_FIFO_COUNT & (0x1 << BIT_FIFO_RD_READY))
	  {
            uart_fifo_rdata= (REG_APB3_UART_DATA>>BIT_UART_RX_FIFO_DOUT) ;
            rx_buf[rx_index] = uart_fifo_rdata;
            rx_index++;
            if(rx_index>=RX_BUF_LEN)
            {
                rx_index = 0;
            }
	  }
        REG_APB3_UART_INT_FLAG = 0x42 ;
    }

    if(uart_int_status & SET_RX_FIFO_OVER_FLOW_EN)
    {
        REG_APB3_UART_INT_FLAG = uart_int_status  ; 
    }

    if(uart_int_status & SET_UART_RX_PARITY_ERR_EN)
    {
        REG_APB3_UART_INT_FLAG = uart_int_status  ;     
    }

    if(uart_int_status & SET_UART_RX_STOP_ERR_EN)
    {
        REG_APB3_UART_INT_FLAG = uart_int_status  ;     
    }

    if(uart_int_status & SET_UART_TX_STOP_END_EN)
    {
        REG_APB3_UART_INT_FLAG = uart_int_status  ;     
    }

    if(uart_int_status & SET_UART_RX_STOP_END_EN)
    {
        REG_APB3_UART_INT_FLAG = uart_int_status  ;     
    }
}

void Beken_UART_TxChar(u_int8 tx_char)
{
    u_int32 fifo_length;

    do
    {
        fifo_length=UART_TX_FIFO_COUNT&0xff;
    }while(fifo_length!=0);
        
    REG_APB3_UART_DATA = tx_char;
}

/*
void Beken_UART_TxStr(const u_int8 * str)
{
    while(*str != 0)
    {
        Beken_UART_TxChar(*str++);
    }
}
*/


void Beken_UART_TxStr(const u_int8 * str)
{
    //while(*str != '\n')
    while(*str != 0)
    {
        Beken_UART_TxChar(*str++);
    }
}


u_int8  Beken_UART_Rx(u_int8 * read_buf)
{
    u_int8  ret;
    if(rx_index == 0)
        return 0;

    memcpy(read_buf, rx_buf, rx_index);
    ret = rx_index;
    rx_index = 0;
    return ret;
}






