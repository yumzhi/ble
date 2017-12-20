
#include "BK3431_reg.h"
#include "BK3431_gpio.h"
#include "debug.h"

volatile unsigned long * GPIO_CFG[] = 
{
    &REG_APB5_GPIOA_CFG,
    &REG_APB5_GPIOB_CFG,
    &REG_APB5_GPIOC_CFG,
    &REG_APB5_GPIOD_CFG,
    &REG_APB5_GPIOE_CFG
};

volatile unsigned long * GPIO_DATA[] = 
{
    &REG_APB5_GPIOA_DATA,
    &REG_APB5_GPIOB_DATA,
    &REG_APB5_GPIOC_DATA,
    &REG_APB5_GPIOD_DATA,
    &REG_APB5_GPIOE_DATA
};

void Beken_Gpio_Cfg(u_int8 gpio, Dir_Type dir, Pull_Type pull)
{
    u_int32  temp_cfg, temp_data;
    u_int8 port = (gpio&0xf0)>>4;
    u_int8  pin = gpio&0xf;
    temp_cfg = *(GPIO_CFG[port]);
    temp_data = *(GPIO_DATA[port]);
 
    if(dir == OUTPUT)
    {
        temp_cfg |= (1<<pin);
        temp_cfg &= ~(1<<pin<<8);
        temp_data &= ~(1<<pin<<16);
    }
    else if(dir== INPUT)
    {
        temp_cfg |= 1<<pin;
        temp_cfg |= 1<<pin<<8;
        temp_data |= (1<<pin<<16);
    }
    else
    {
        temp_cfg |= 1<<pin;
        temp_cfg |= 1<<pin<<8;
        temp_data &= ~(1<<pin<<16);
    }

    switch(pull)
    {
    case PULL_HIGH:
        temp_cfg |= (1<<pin<<16);
        temp_cfg &= ~(1<<pin<<24);
        break;
    case PULL_LOW:
        temp_cfg &= ~(1<<pin<<16);
        temp_cfg |= (1<<pin<<24);
        break;
    case PULL_NONE:
        temp_cfg &= ~(1<<pin<<16);
        temp_cfg &= ~(1<<pin<<24);
        break;
    }
    *(GPIO_CFG[port]) = temp_cfg;
    *(GPIO_DATA[port]) = temp_data;
}

u_int8 Beken_Gpio_Get(u_int8 gpio)
{
    u_int32 temp;
    u_int8 port = (gpio&0xf0)>>4;
    u_int8  pin = gpio&0xf;

    temp = *(GPIO_DATA[port]);

    temp = temp>>8>>pin;
    return temp&0x01;
}

void Beken_Gpio_Set(u_int8 gpio, u_int8 val)
{
    u_int32 temp;
    u_int8 port = (gpio&0xf0)>>4;
    u_int8  pin = gpio&0xf;
    
    temp = *(GPIO_DATA[port]);
    if(val)
    {
        temp |= 1<<pin;
    }
    else
    {
        temp &= ~(1<<pin);
    }

    *(GPIO_DATA[port]) = temp;
}

