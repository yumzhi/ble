#ifndef _SYS_UART_H_
#define _SYS_UART_H_

#include "sys_config.h"
#include "sys_types.h"

void Beken_UART_Initial(void);
void Beken_UART_Close(void);
void Beken_UART_TxChar(u_int8 tx_char);
void Beken_UART_TxStr(const u_int8 * str);
void Beken_UART_Interrupt_Handler(void);

/*read_buf长度必须大于64*/
u_int8  Beken_UART_Rx(u_int8 * read_buf);

#endif
