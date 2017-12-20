#ifndef _BK_REG_H_
#define _BK_REG_H_
#include "BK3431_reg.h"


#define MFC_BASE                        AHB3_MFC_BASE
#define AHB2APB_BASE                    AHB6_AHB2APB_BASE
#define ADC_BASE                        APB7_ADC_BASE
#define ICU_BASE                        AHB0_ICU_BASE
#define WDT_BASE                        APB0_WDT_BASE
#define GPIO_BASE                       APB5_GPIO_BASE
#define UART1_BASE                      APB3_UART_BASE
#define SPI1_BASE                       APB2_SPI_BASE
#define I2C1_BASE                       APB4_I2C_BASE
#define PWM_BASE                        APB1_PWM_BASE
#define RTC_BASE                        APB6_RTC_BASE

#define p_MFC_BASE                      p_AHB3_MFC_BASE
#define p_AHB2APB_BASE                  p_AHB6_AHB2APB_BASE
#define p_ADC_BASE                      p_APB7_ADC_BASE
#define p_ICU_BASE                      p_AHB0_ICU_BASE
#define P_WDT_BASE                      p_APB0_WDT_BASE
#define P_GPIO_BASE                     p_APB5_GPIO_BASE
#define P_UART1_BASE                    p_APB3_UART_BASE
#define P_SPI1_BASE                     p_APB2_SPI_BASE
#define P_I2C1_BASE                     p_APB4_I2C_BASE
#define p_PWM_BASE                      p_APB1_PWM_BASE
#define p_RTC_BASE                      p_APB6_RTC_BASE
//////////////////////////////////
//register definition
//////////////////////////////////

//MFC
#define REG_MFC_KEYWORD                 REG_AHB3_MFC_KEYWORD
#define REG_MFC_CONTROL                 REG_AHB3_MFC_CONTROL
#define REG_MFC_ADDR                    REG_AHB3_MFC_ADDR
#define REG_MFC_DATA                    REG_AHB3_MFC_DATA
#define REG_MFC_WP0                     REG_AHB3_MFC_WP0
#define REG_MFC_WP1                     REG_AHB3_MFC_WP1
#define REG_MFC_WP2                     REG_AHB3_MFC_WP2

//ADC
#define ADC_ENABLE                             0x00000004
#define ADC_DISABLE                           ~0x00000004
#define ADC_SLEEP_MODE                                  0
#define ADC_STEP_MODE                                   1
#define ADC_SOFTCTRL_MODE                               2
#define ADC_SEQUENT_MODE                                3

#define ADC_CHANNEL_SEL_BIT                             3
#define ADC_FIFO_EMPTY                                  6
#define ADC_SAMPLE_RATE_BIT                             8
#define ADC_SETTLING_BIT                                10
#define ADC_VALID_COUNT_BIT                             11
#define ADC_CLK_RATE_BIT                                16
#define ADC_INT_CLR_BIT                                 15

#define ADC_CLK_DIV_4                                   0
#define ADC_CLK_DIV_8                                   1
#define ADC_CLK_DIV_16                                  2
#define ADC_CLK_DIV_32                                  3

#define REG_ADC_CFG                     REG_APB7_ADC_CFG
#define REG_ADC_DAT                     REG_APB7_ADC_DAT


#define FIFO_DACR_INT_bit              (0x01<<16)
#define FIFO_DACL_INT_bit              (0x01<<17)
#define FIFO_ADCR_INT_bit              (0x01<<18)
#define FIFO_ADCL_INT_bit              (0x01<<19)
#define FIFO_DACR_FULL_bit                 (0x01<<8)
#define FIFO_DACL_FULL_bit             (0x01<<9)
#define FIFO_DACR_EMPTY_bit            (0x01<<12)
#define FIFO_DACL_EMPTY_bit            (0x01<<13)

//ICU register definitions
#define REG_ICU_CLOCKSOURCE             REG_AHB0_ICU_CLKSRCSEL
#define REG_ICU_CPUCLKCON               REG_AHB0_ICU_CORECLKCON
#define REG_ICU_ADCCLKCON               REG_AHB0_ICU_ADCCLKCON
#define REG_ICU_WDTCLKCON               REG_AHB0_ICU_WDTCLKCON
#define REG_ICU_UART1CLKCON             REG_AHB0_ICU_UARTCLKCON
#define REG_ICU_SPI1CLKCON              REG_AHB0_ICU_SPICLKCON
#define REG_ICU_BLECLK_PWD              REG_AHB0_ICU_BLECLKCON
#define REG_ICU_I2C1CLKCON              REG_AHB0_ICU_I2CCLKCON
#define REG_ICU_PWMCLKCON               REG_AHB0_ICU_PWMCLKCON
#define REG_ICU_INT_ENABLE              REG_AHB0_ICU_INT_ENABLE
#define REG_ICU_IRQTYPE_EN              REG_AHB0_ICU_IRQ_ENABLE
#define REG_ICU_INT_FLAG                REG_AHB0_ICU_INT_FLAG

#define SYSCLK_HFSRC_32K                                0
#define SYSCLK_HFSRC_16M                                1
#define SYSCLK_HFSRC_48M                            3
                                                                      
//FIQ
#define INT_ENABLE_BLE_bit             INT_STATUS_BLE_bit

//IRQ
#define INT_STATUS_I2C1_bit            INT_STATUS_I2C_bit
#define INT_STATUS_SPI1_bit            INT_STATUS_SPI_bit
#define INT_STATUS_UART1_bit           INT_STATUS_UART_bit
#define INT_ENABLE_UART1_bit           INT_STATUS_UART_bit

//TDES
#define DES_MODULE_RST_BIT                              0
#define DES_OUT_RST_BIT                                 1
#define DES_ADDR_RST_BIT                                2
#define DES_MAC_BIT                                     4
#define DES_ENC_BIT                                     5
#define DES_TDES_BIT                                    6
#define DES_EN_BIT                                      7

//WDT
#define REG_WDT_CFG                     REG_APB0_WDT_CFG

//GPIO                                                                                   
#define REG_GPIOA_CFG                   REG_APB5_GPIOA_CFG
#define REG_GPIOA_DATA                  REG_APB5_GPIOA_DATA
#define REG_GPIOB_CFG                   REG_APB5_GPIOB_CFG
#define REG_GPIOB_DATA                  REG_APB5_GPIOB_DATA
#define REG_GPIOC_CFG                   REG_APB5_GPIOC_CFG
#define REG_GPIOC_DATA                  REG_APB5_GPIOC_DATA
#define REG_GPIOD_CFG                   REG_APB5_GPIOD_CFG
#define REG_GPIOD_DATA                  REG_APB5_GPIOD_DATA
#define REG_GPIOE_CFG                   REG_APB5_GPIOE_CFG
#define REG_GPIOE_DATA                  REG_APB5_GPIOE_DATA
#define REG_GPIO_INT_LEVEL              REG_APB5_GPIO_WUATOD_TYPE
#define REG_GPIO_INT_ENABLE             REG_APB5_GPIO_WUATOD_ENABLE
#define REG_GPIO_INT_STATUS             REG_APB5_GPIO_WUATOD_STAT
#define REG_GPIO_INT2_LEVEL             REG_APB5_GPIO_WUE_TYPE
#define REG_GPIO_INT2_ENABLE            REG_APB5_GPIO_WUE_ENABLE
#define REG_GPIO_INT2_STATUS            REG_APB5_GPIO_WUE_STAT

//UART1

//charles merge from BK3515 charles

#define UART_INTR_TX_FIFO_NEED_WRITE_bit  0x01
#define UART_INTR_RX_FIFO_NEED_READ_bit   0x02
#define UART_INTR_RX_OV_bit               0x04
#define UART_INTR_RX_PAR_ERR_bit          0x08
#define UART_INTR_RX_STOP_ERR_bit         0x10
#define UART_INTR_TX_FIFO_NEED_WRITE_wakeup_bit  0x20
#define UART_INTR_RX_FIFO_NEED_READ_wakeup_bit   0x40     
#define UART_RXD_WAKEUP_PULSE          0x80  
 
#define UART_BIT_FIFO_WR_READY               20
#define UART_BIT_FIFO_RD_READY               21

#define UART_TX_EN_BIT                                  0
#define UART_RX_EN_BIT                                  1
#define UART_IRDA_MODE_EN_BIT                           2
#define UART_DATA_LEN_BIT                               3
#define UART_PAR_EN_BIT                                 5
#define UART_PAR_MODE_BIT                               6
#define UART_STOP_LEN_BIT                               7
#define UART_CLK_DIV_BIT                                8
#define UART_TX_FIFO_RESET_BIT                          21
#define UART_RX_FIFO_RESET_BIT                          22

#define UART_FRAME_LEN_5_BIT                            0
#define UART_FRAME_LEN_6_BIT                            1
#define UART_FRAME_LEN_7_BIT                            2
#define UART_FRAME_LEN_8_BIT                            3

#define UART_TX_INT_EN                                  REG_UART1_INT_ENABLE |= 0x01
#define UART_RX_INT_EN                                  REG_UART1_INT_ENABLE |= 0x02
#define UART_TX_INT_DISENABLE                   REG_UART1_INT_ENABLE &= ~0x01
#define UART_RX_INT_DISENABLE                   REG_UART1_INT_ENABLE &= ~0x02

#define UART_TX_INT_BIT                                         0
#define UART_RX_INT_BIT                                         1

#define REG_UART1_CFG                   REG_APB3_UART_CFG
#define REG_UART1_FIFO_THRESHOLD        REG_APB3_UART_FIFO_THRESHOLD
#define REG_UART1_FIFO_COUNT            REG_APB3_UART_FIFO_COUNT
#define REG_UART1_DATA                  REG_APB3_UART_DATA
#define REG_UART1_INT_ENABLE            REG_APB3_UART_INT_ENABLE
#define REG_UART1_INT_FLAG              REG_APB3_UART_INT_FLAG
#define REG_UART1_FLOW_CTRL             REG_APB3_UART_FLOW_CTRL
#define REG_UART1_WAKE_CTRL             REG_APB3_UART_WAKE_CTRL


//SPI1
#define REG_SPI1_CTRL                   REG_APB2_SPI_CTRL
#define REG_SPI1_STAT                   REG_APB2_SPI_STAT
#define REG_SPI1_DAT                    REG_APB2_SPI_DAT

#define SPI_EN_BIT                     (1 << 23)
#define SPI_MAST_BIT                   (1 << 22)
#define SPI_NSSMD_BIT                  (3 << 16)
#define SPI_TXINT_BIT                  (1 << 8)
#define SPI_RXINT_BIT                  (1 << 9)
#define SPI_RXEMPTY_BIT                (1 << 2)


//I2C1
#define REG_I2C1_CN                     REG_APB4_I2C_CN
#define REG_I2C1_STAT                   REG_APB4_I2C_STAT
#define REG_I2C1_DAT                    REG_APB4_I2C_DAT

#define I2C_EN_BIT                     ((unsigned int)1 << 31)
#define I2C_MAST_BIT                   (1 << 14)
#define I2C_TXMODE_BIT                 (1 << 13)
#define I2C_ACKRQ_BIT                  (1 << 12)
#define I2C_ADDRMATCH_BIT              (1 << 11)
#define I2C_STA_BIT                    (1 << 10)
#define I2C_STO_BIT                    (1 << 9)
#define I2C_ACK_BIT                    (1 << 8)
#define I2C_INTMODE_BIT                (3 << 6)
#define I2C_RXFIFO_EMPTY_BIT           (1 << 4)
#define I2C_INT_BIT                    (1 << 0)

//PWM
#define REG_PWM_0                       REG_APB1_PWM_0
#define REG_PWM_1                       REG_APB1_PWM_1
#define REG_PWM_2                       REG_APB1_PWM_2
#define REG_PWM_3                       REG_APB1_PWM_3
#define REG_PWM_4                       REG_APB1_PWM_4
#define REG_PWM_CFG                     REG_APB1_PWM_CFG

#define PT0_MODE_BIT                            0
#define PT1_MODE_BIT                            1
#define PT2_MODE_BIT                            2
#define PT3_MODE_BIT                            3
#define PT4_MODE_BIT                            4
#define PT_ENABLE_BIT                           7
#define PT_CLK_DIV_BIT                          8

#define PT0_INT_FLAG                            (1<<16)
#define PT1_INT_FLAG                            (1<<17)
#define PT2_INT_FLAG                            (1<<18)
#define PT3_INT_FLAG                            (1<<19)
#define PT4_INT_FLAG                            (1<<20)
#define SET_PT_CLK_DIV                  (0xF << PT_CLK_DIV_BIT)

#define BK_WRITE_REG(reg_addr,reg_v)            (*((volatile unsigned long *)(reg_addr)))=reg_v
#define BK_READ_REG(reg_addr)                           (*((volatile unsigned long *)   (reg_addr)))
#define UART_WRITE_BYTE(v) (REG_APB3_UART_DATA=v)
#define UART_READ_BYTE()  ((REG_APB3_UART_DATA>>8)&0xff)

#define UART_TX_FIFO_COUNT (REG_APB3_UART_FIFO_COUNT&0xff)
#define UART_RX_FIFO_COUNT ( (REG_APB3_UART_FIFO_COUNT>>8)&0xff)

#endif

