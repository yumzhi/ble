#ifndef DEBUG
#define DEBUG

#include "BKReg.h"
#include "BKUart.h"

#define bprintf   Beken_UART_TxStr

#ifdef GPIO_DEBUG
//#define DEBUG_MSG(x) {REG_APB5_GPIOC_CFG &=0xffff7fff;REG_APB5_GPIOC_DATA &=0xffffff7f;REG_GPIOB_DATA = x;REG_APB5_GPIOC_DATA |=0x00000080;}
#define DEBUG_MSG(x) {REG_GPIOE_DATA = x;}

#else
#define DEBUG_MSG(x) ;//{REG_GPIOE_DATA = 0;}
//#define DEBUG_MSG(x) {int v=x;(void)v;}
#endif

#endif
