#ifndef _BK3431_GPIO_H_
#define _BK3431_GPIO_H_

#define GPIOA_0     0x00
#define GPIOA_1     0x01
#define GPIOA_2     0x02
#define GPIOA_3     0x03
#define GPIOA_4     0x04
#define GPIOA_5     0x05
#define GPIOA_6     0x06
#define GPIOA_7     0x07

#define GPIOB_0     0x10
#define GPIOB_1     0x11
#define GPIOB_2     0x12
#define GPIOB_3     0x13
#define GPIOB_4     0x14
#define GPIOB_5     0x15
#define GPIOB_6     0x16
#define GPIOB_7     0x17

#define GPIOC_0     0x20
#define GPIOC_1     0x21
#define GPIOC_2     0x22
#define GPIOC_3     0x23
#define GPIOC_4     0x24
#define GPIOC_5     0x25
#define GPIOC_6     0x26
#define GPIOC_7     0x27

#define GPIOD_0     0x30
#define GPIOD_1     0x31
#define GPIOD_2     0x32
#define GPIOD_3     0x33
#define GPIOD_4     0x34
#define GPIOD_5     0x35
#define GPIOD_6     0x36
#define GPIOD_7     0x37

#include "sys_types.h"

typedef    struct
{
    u_int8 port:4;
    u_int8 pin:4;
}Gpio_Type;

typedef enum
{
    INPUT,
    OUTPUT,
    FLOAT
}Dir_Type;

typedef enum
{
    PULL_HIGH,
    PULL_LOW,
    PULL_NONE
}Pull_Type;

void Beken_Gpio_Cfg(u_int8 gpio, Dir_Type dir, Pull_Type pull);
void Beken_Gpio_Func(u_int8 gpio);
void Beken_Gpio_Set(u_int8 gpio, u_int8 val);
u_int8 Beken_Gpio_Get(u_int8 gpio);


#endif
