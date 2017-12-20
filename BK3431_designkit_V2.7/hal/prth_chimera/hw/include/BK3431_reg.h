#ifndef _BK3431_CHIP_H_
#define _BK3431_CHIP_H_


/////////////////////////////////////////////////////////////////////////////
//memory layout
/////////////////////////////////////////////////////////////////////////////
#define ROM_START_ADDR           0x00000000
#define RAM_START_ADDR           0x00400000

#define AHB0_ICU_BASE            0x00800000
#define AHB1_BLE_BASE            0x00801000
#define AHB3_MFC_BASE            0x00803000
#define AHB6_AHB2APB_BASE        0x00806000

#define APB0_WDT_BASE            0x00806000
#define APB1_PWM_BASE            0x00806100
#define APB2_SPI_BASE            0x00806200
#define APB3_UART_BASE           0x00806300
#define APB4_I2C_BASE            0x00806400
#define APB5_GPIO_BASE           0x00806500
#define APB6_RTC_BASE            0x00806600
#define APB7_ADC_BASE            0x00806700

#define APB_BLE_BASE             0x00802000

#define p_AHB0_ICU_BASE          (*((volatile unsigned long *)  0x00800000))
#define p_AHB1_BLE_BASE          (*((volatile unsigned long *)  0x00801000))
#define p_AHB3_MFC_BASE          (*((volatile unsigned long *)  0x00803000))
#define p_AHB6_AHB2APB_BASE      (*((volatile unsigned long *)  0x00806000))

#define p_APB0_WDT_BASE          (*((volatile unsigned long *)  0x00806000))
#define p_APB1_PWM_BASE          (*((volatile unsigned long *)  0x00806100))
#define p_APB2_SPI_BASE          (*((volatile unsigned long *)  0x00806200))
#define p_APB3_UART_BASE         (*((volatile unsigned long *)  0x00806300))
#define p_APB4_I2C_BASE          (*((volatile unsigned long *)  0x00806400))
#define p_APB5_GPIO_BASE         (*((volatile unsigned long *)  0x00806500))
#define p_APB6_RTC_BASE          (*((volatile unsigned long *)  0x00806600))
#define p_APB7_ADC_BASE          (*((volatile unsigned long *)  0x00806700))

/////////////////////////////////////////////////////////////////////////////
//register definition
/////////////////////////////////////////////////////////////////////////////
//ICU register definitions
#define REG_AHB0_ICU_CLKSRCSEL             (*((volatile unsigned long *)   0x00800000))
#define SET_CLK_SEL_32K                    0x0
#define SET_CLK_SEL_16M                    0x1
#define SET_CLK_SEL_48M                    0x3

#define REG_AHB0_ICU_CORECLKCON            (*((volatile unsigned long *)   0x00800004))
#define BIT_CORE_CLK_DIV                   1

#define REG_AHB0_ICU_ADCCLKCON             (*((volatile unsigned long *)   0x00800008))
#define REG_AHB0_ICU_UARTCLKCON            (*((volatile unsigned long *)   0x0080000c))
#define REG_AHB0_ICU_I2CCLKCON             (*((volatile unsigned long *)   0x00800010))
#define REG_AHB0_ICU_SPICLKCON             (*((volatile unsigned long *)   0x00800014))
#define REG_AHB0_ICU_BLECLKCON             (*((volatile unsigned long *)   0x00800018))
#define REG_AHB0_ICU_WDTCLKCON             (*((volatile unsigned long *)   0x0080001c))
#define REG_AHB0_ICU_RNGCLKCON             (*((volatile unsigned long *)   0x00800020))
#define REG_AHB0_ICU_PWMCLKCON             (*((volatile unsigned long *)   0x00800024))
#define BIT_PWM_CLK_SEL                    1
#define SET_PWM_CLK_SEL                    (0x1 << BIT_PWM_CLK_SEL)

#define REG_AHB0_ICU_RTCCLKCON             (*((volatile unsigned long *)   0x00800028))

#define REG_AHB0_ICU_LPOCLKCON            (*((volatile unsigned long *)   0x0080002C))

#define REG_AHB0_ICU_CLK32K_SEL            (*((volatile unsigned long *)   0x00800030))

#define REG_AHB0_ICU_INT_ENABLE            (*((volatile unsigned long *)   0x00800040))
#define REG_AHB0_ICU_IRQ_ENABLE            (*((volatile unsigned long *)   0x00800044))
#define REG_AHB0_ICU_INT_FLAG              (*((volatile unsigned long *)   0x00800048))
#define REG_AHB0_ICU_FLASH                 (*((volatile unsigned long *)   0x0080004c))
#define REG_AHB0_ICU_ANALOG_MODE           (*((volatile unsigned long *)   0x00800050))
#define REG_AHB0_ICU_ANALOG0_PWD           (*((volatile unsigned long *)   0x00800054))
#define REG_AHB0_ICU_ANALOG1_PWD           (*((volatile unsigned long *)   0x00800058))
#define REG_AHB0_ICU_DIGITAL_PWD           (*((volatile unsigned long *)   0x0080005c))



#define REG_CEVA_REG_4                     (*((volatile unsigned long *)   0x00801610))

//Digital PWD Bit
#define BIT_ANA_TEST_PIN                   5
#define BIT_BB_TEST_PIN                    4
#define BIT_CLK16M_PWD                     3
#define BIT_HP_LDO_PWD                     2
#define BIT_CLK48M_PWD                     1
#define BIT_CB_BAIS_PWD                    0
#define SET_ANA_TEST_PIN                   (0x01 << BIT_ANA_TEST_PIN)
#define SET_BB_TEST_PIN                    (0x01 << BIT_BB_TEST_PIN)
#define SET_CLK16M_PWD                     (0x01 << BIT_CLK16M_PWD )
#define SET_HP_LDO_PWD                     (0x01 << BIT_HP_LDO_PWD )
#define SET_CLK48M_PWD                     (0x01 << BIT_CLK48M_PWD )
#define SET_CB_BAIS_PWD                    (0x01 << BIT_CB_BAIS_PWD)


// DEEP_SLEEP0 is GPIO-A to GPIO-D deep sleep wake-en
// DEEP_SLEEP1 is GPIO-E deep sleep wake-en and deep sleep word
#define REG_AHB0_ICU_DEEP_SLEEP0           (*((volatile unsigned long *)   0x00800060))
#define REG_AHB0_ICU_DEEP_SLEEP1           (*((volatile unsigned long *)   0x00800064))
#define BIT_GPIOA_WAKE_EN                  0
#define BIT_GPIOB_WAKE_EN                  8
#define BIT_GPIOC_WAKE_EN                  16
#define BIT_GPIOD_WAKE_EN                  24
#define BIT_GPIOE_WAKE_EN                  0
#define BIT_DEEP_SLEEP_WORD                16
#define SET_DEEP_SLEEP_WORD                (0xFFFF << BIT_DEEP_SLEEP_WORD)

#define REG_AHB0_ICU_RC32K_CONFG           (*((volatile unsigned long *)   0x00800068))
#define REG_AHB0_ICU_SLEEP_TIME            (*((volatile unsigned long *)   0x0080006C))
#define BIT_MANU_FIN                       8
#define BIT_MANU_CIN                       3
#define BIT_MANU_EN                        2
#define BIT_CALI_MODE                      1
#define BIT_SPI_TRIG                       0
#define SET_MANU_FIN                       (0x1FF<< BIT_MANU_FIN )
#define SET_MANU_CIN                       (0x1F << BIT_MANU_CIN )
#define SET_MANU_EN                        (0x1  << BIT_MANU_EN  )
#define SET_CALI_MODE                      (0x1  << BIT_CALI_MODE)
#define SET_SPI_TRIG                       (0x1  << BIT_SPI_TRIG )



//FIQ
#define INT_STATUS_PWM0_bit     (0x01<<0)
#define INT_STATUS_LBD_bit      (0x01<<10)
#define INT_STATUS_BLE_bit      (0x01<<15)
//IRQ
#define INT_STATUS_RTC_bit      (0x01<<12)
#define INT_STATUS_RNG_bit      (0x01<<11)
#define INT_STATUS_GPIO_bit     (0x01<< 9)
#define INT_STATUS_ADC_bit      (0x01<< 8)
#define INT_STATUS_I2C_bit      (0x01<< 7)
#define INT_STATUS_SPI_bit      (0x01<< 6)
#define INT_STATUS_UART_bit     (0x01<< 5)
#define INT_STATUS_PWM4_bit     (0x01<< 4)
#define INT_STATUS_PWM3_bit     (0x01<< 3)
#define INT_STATUS_PWM2_bit     (0x01<< 2)
#define INT_STATUS_PWM1_bit     (0x01<< 1)

#define INT_IRQ_BIT             (0x01<<0)
#define FIQ_IRQ_BIT             (0x01<<1)

//RF register definitions
//=====================================================================================



//=====================================================================================
//MFC
//=====================================================================================
#define REG_AHB3_MFC_KEYWORD             (*((volatile unsigned long *)   0x00803000))
#define REG_AHB3_MFC_CONTROL             (*((volatile unsigned long *)   0x00803004))
#define REG_AHB3_MFC_ADDR                (*((volatile unsigned long *)   0x00803008))
#define REG_AHB3_MFC_DATA                (*((volatile unsigned long *)   0x0080300C))
#define REG_AHB3_MFC_WP0                 (*((volatile unsigned long *)   0x00803010))
#define REG_AHB3_MFC_WP1                 (*((volatile unsigned long *)   0x00803014))
#define REG_AHB3_MFC_WP2                 (*((volatile unsigned long *)   0x00803018))
#define REG_AHB3_MFC_WP3                 (*((volatile unsigned long *)   0x0080301c))
//#define REG_AHB3_MFC_TEST_REG               (*((volatile unsigned long *)   0x00803034))

//// MFC INTRN ////
#define MFC_CONTROL_READ                (0x00)
#define MFC_CONTROL_WRITE               (0x01<<2)
#define MFC_CONTROL_SECT_ERASE          (0x02<<2)
#define MFC_CONTROL_BLOCK_ERASE         (0x03<<2)
#define MFC_CONTROL_CHIP_ERASE          (0x07<<2)
#define MFC_CONTROL_MAIN_SPACE          (0x00<<5)
#define MFC_CONTROL_NVR_SPACE           (0x01<<5)
#define MFC_CONTROL_RDN_SPACE           (0x02<<5)

//WDT
#define REG_APB0_WDT_CFG                (*((volatile unsigned long *)   0x00806000))


//=====================================================================================
//PWM
//=====================================================================================
#define REG_APB1_PWM_0                  (*((volatile unsigned long *)   0x00806100))
#define REG_APB1_PWM_1                  (*((volatile unsigned long *)   0x00806104))
#define REG_APB1_PWM_2                  (*((volatile unsigned long *)   0x00806108))
#define REG_APB1_PWM_3                  (*((volatile unsigned long *)   0x0080610c))
#define REG_APB1_PWM_4                  (*((volatile unsigned long *)   0x00806110))
#define BIT_PWM_MODE_MIDDLE             0
#define BIT_TIMER_MODE_START            16
#define SET_TIMER_MODE_START            (0xFFFF << BIT_TIMER_MODE_START)

#define REG_APB1_PWM_CFG                (*((volatile unsigned long *)   0x00806114))
#define PT0_MODE_BIT                    0
#define PT1_MODE_BIT                    1
#define PT2_MODE_BIT                    2
#define PT3_MODE_BIT                    3
#define PT4_MODE_BIT                    4
#define PT_ENABLE_BIT                   7
#define PT_CLK_DIV_BIT                  8
#define PT0_INT_FLAG                    (1<<16)
#define PT1_INT_FLAG                    (1<<17)
#define PT2_INT_FLAG                    (1<<18)
#define PT3_INT_FLAG                    (1<<19)
#define PT4_INT_FLAG                    (1<<20)
#define SET_PT_CLK_DIV                  (0xF << PT_CLK_DIV_BIT)


// PWM Test Firmware Definition
#define BK_TIMER_MODE       0
#define BK_PWM_MODE         1

#define PWM_CLK_SEL32K      0
#define PWM_CLK_SEL16M      1

//=====================================================================================


//SPI
#define REG_APB2_SPI_CTRL               (*((volatile unsigned long *)   0x00806200))
#define REG_APB2_SPI_STAT               (*((volatile unsigned long *)   0x00806204))
#define REG_APB2_SPI_DAT                (*((volatile unsigned long *)   0x00806208))

//=====================================================================================
// UART
//=====================================================================================
#define REG_APB3_UART_CFG               (*((volatile unsigned long *)   0x00806300))
#define REG_APB3_UART_FIFO_THRESHOLD    (*((volatile unsigned long *)   0x00806304))
#define REG_APB3_UART_FIFO_COUNT        (*((volatile unsigned long *)   0x00806308))
#define REG_APB3_UART_DATA              (*((volatile unsigned long *)   0x0080630C))
#define REG_APB3_UART_INT_ENABLE        (*((volatile unsigned long *)   0x00806310))
#define REG_APB3_UART_INT_FLAG          (*((volatile unsigned long *)   0x00806314))
#define REG_APB3_UART_FLOW_CTRL         (*((volatile unsigned long *)   0x00806318))
#define REG_APB3_UART_WAKE_CTRL         (*((volatile unsigned long *)   0x0080631c))

#define BIT_UART_TX_ENABLE              0
#define BIT_UART_RX_ENABLE              1
#define BIT_UART_IRDA                   2
#define BIT_UART_LEN                    3
#define BIT_UART_PAR_EN                 5
#define BIT_UART_PAR_MODE               6
#define BIT_UART_STOP_LEN               7
#define BIT_UART_CLK_DIVID              8
#define SET_UART_TX_ENABLE              (0x1 << BIT_UART_TX_ENABLE)
#define SET_UART_RX_ENABLE              (0x1 << BIT_UART_RX_ENABLE)
#define SET_UART_IRDA                   (0x1 << BIT_UART_IRDA     )
#define SET_UART_LEN                    (0x3 << BIT_UART_LEN      )
#define SET_UART_PAR_EN                 (0x1 << BIT_UART_PAR_EN   )
#define SET_UART_PAR_MODE               (0x1 << BIT_UART_PAR_MODE )
#define SET_UART_STOP_LEN               (0x1 << BIT_UART_STOP_LEN )
#define SET_UART_CLK_DIVID              (0x1FFF<<BIT_UART_CLK_DIVID)

#define BIT_TX_FIFO_THRESHOLD           0
#define BIT_RX_FIFO_THRESHOLD           8
#define BIT_STOP_DETECT_TIME            16
#define SET_TX_FIFO_THRESHOLD           (0xFF<< BIT_TX_FIFO_THRESHOLD)
#define SET_RX_FIFO_THRESHOLD           (0xFF<< BIT_RX_FIFO_THRESHOLD)
#define SET_STOP_DETECT_TIME            (0x3 << BIT_STOP_DETECT_TIME)

// FIFO Read-Only Status
#define BIT_TX_FIFO_COUNT               0
#define BIT_RX_FIFO_COUNT               8
#define BIT_TX_FIFO_FULL                16
#define BIT_TX_FIFO_EMPTY               17
#define BIT_RX_FIFO_FULL                18
#define BIT_RX_FIFO_EMPTY               19
#define BIT_FIFO_WR_READY               20
#define BIT_FIFO_RD_READY               21

#define BIT_UART_TX_FIFO_DIN            0
#define BIT_UART_RX_FIFO_DOUT           8
#define SET_UART_TX_FIFO_DIN            (0xFF<< BIT_UART_TX_FIFO_DIN )
#define SET_UART_RX_FIFO_DOUT           (0xFF<< BIT_UART_RX_FIFO_DOUT)

#define BIT_TX_FIFO_NEED_WRITE_EN       0
#define BIT_RX_FIFO_NEED_READ_EN        1
#define BIT_RX_FIFO_OVER_FLOW_EN        2
#define BIT_UART_RX_PARITY_ERR_EN       3
#define BIT_UART_RX_STOP_ERR_EN         4
#define BIT_UART_TX_STOP_END_EN         5
#define BIT_UART_RX_STOP_END_EN         6
#define BIT_UART_RXD_WAKEUP_EN          7
#define SET_TX_FIFO_NEED_WRITE_EN       (0x1 << BIT_TX_FIFO_NEED_WRITE_EN)
#define SET_RX_FIFO_NEED_READ_EN        (0x1 << BIT_RX_FIFO_NEED_READ_EN )
#define SET_RX_FIFO_OVER_FLOW_EN        (0x1 << BIT_RX_FIFO_OVER_FLOW_EN )
#define SET_UART_RX_PARITY_ERR_EN       (0x1 << BIT_UART_RX_PARITY_ERR_EN)
#define SET_UART_RX_STOP_ERR_EN         (0x1 << BIT_UART_RX_STOP_ERR_EN  )
#define SET_UART_TX_STOP_END_EN         (0x1 << BIT_UART_TX_STOP_END_EN  )
#define SET_UART_RX_STOP_END_EN         (0x1 << BIT_UART_RX_STOP_END_EN  )
#define SET_UART_RXD_WAKEUP_EN          (0x1 << BIT_UART_RXD_WAKEUP_EN   )

#define BIT_TX_FIFO_NEED_WRITE_FLAG     0
#define BIT_RX_FIFO_NEED_READ_FLAG      1
#define BIT_RX_FIFO_OVER_FLOW_FLAG      2
#define BIT_UART_RX_PARITY_ERR_FLAG     3
#define BIT_UART_RX_STOP_ERR_FLAG       4
#define BIT_UART_TX_STOP_END_FLAG       5
#define BIT_UART_RX_STOP_END_FLAG       6
#define BIT_UART_RXD_WAKEUP_FLAG        7
//=====================================================================================



//IIC
#define REG_APB4_I2C_CN                 (*((volatile unsigned long *)   0x00806400))
#define REG_APB4_I2C_STAT               (*((volatile unsigned long *)   0x00806404))
#define REG_APB4_I2C_DAT                (*((volatile unsigned long *)   0x00806408))

#define I2C_INTR_BIT                    (0x01<<9)
#define I2C_INTR_TX                     (0x01<<1)


//=====================================================================================
// GPIO
//=====================================================================================
#define REG_APB5_GPIOA_CFG              (*((volatile unsigned long *)   0x00806500))
#define REG_APB5_GPIOA_DATA             (*((volatile unsigned long *)   0x00806504))
#define REG_APB5_GPIOB_CFG              (*((volatile unsigned long *)   0x00806508))
#define REG_APB5_GPIOB_DATA             (*((volatile unsigned long *)   0x0080650C))
#define REG_APB5_GPIOC_CFG              (*((volatile unsigned long *)   0x00806510))
#define REG_APB5_GPIOC_DATA             (*((volatile unsigned long *)   0x00806514))
#define REG_APB5_GPIOD_CFG              (*((volatile unsigned long *)   0x00806518))
#define REG_APB5_GPIOD_DATA             (*((volatile unsigned long *)   0x0080651C))
#define REG_APB5_GPIOE_CFG              (*((volatile unsigned long *)   0x00806520))
#define REG_APB5_GPIOE_DATA             (*((volatile unsigned long *)   0x00806524))

#define REG_APB5_GPIO_WUATOD_TYPE       (*((volatile unsigned long *)   0x00806540))
#define REG_APB5_GPIO_WUE_TYPE          (*((volatile unsigned long *)   0x00806544))
#define REG_APB5_GPIO_WUATOD_ENABLE     (*((volatile unsigned long *)   0x00806548))
#define REG_APB5_GPIO_WUE_ENABLE        (*((volatile unsigned long *)   0x0080654C))
#define REG_APB5_GPIO_WUATOD_STAT       (*((volatile unsigned long *)   0x00806550))
#define REG_APB5_GPIO_WUE_STAT          (*((volatile unsigned long *)   0x00806554))

#define BIT_GPIO_PERI_EN                0
#define BIT_GPIO_OUT_EN_N               8
#define BIT_GPIO_PULL_UP                16
#define BIT_GPIO_PULL_DOWN              24
#define BIT_GPIO_IN_BUFFER              0
#define BIT_GPIO_OUT_BUFFER             8
#define BIT_GPIO_INPUT_EN               16

#define BIT_GPIOA_WU_TYPE               0
#define BIT_GPIOB_WU_TYPE               8
#define BIT_GPIOC_WU_TYPE               16
#define BIT_GPIOD_WU_TYPE               24
#define BIT_GPIOE_WU_TYPE               0


//=====================================================================================
//RTC
//=====================================================================================
#define REG_APB6_RTC_CFG                (*((volatile unsigned long *)   0x00806600))
#define BIT_RTC_ENABLE                  0
#define BIT_RTC_CLEAR                   1
#define BIT_RTC_ALARM_EN                2
#define BIT_RTC_ALARM_MODE              3
#define SET_RTC_ENABLE                  (0x1 << BIT_RTC_ENABLE     )
#define SET_RTC_CLEAR                   (0x1 << BIT_RTC_CLEAR      )
#define SET_RTC_ALARM_EN                (0x1 << BIT_RTC_ALARM_EN   )
#define SET_RTC_ALARM_MODE              (0x1 << BIT_RTC_ALARM_MODE )

#define REG_APB6_RTC_UNIT               (*((volatile unsigned long *)   0x00806604))
#define BIT_RTC_DIV                     0
#define BIT_RTC_MSUNIT                  2
#define BIT_RTC_SUNIT                   8
#define SET_RTC_DIV                     (0x3    << BIT_RTC_DIV   )
#define SET_RTC_MSUNIT                  (0x3F   << BIT_RTC_MSUNIT)
#define SET_RTC_SUNIT                   (0xFFFF << BIT_RTC_SUNIT )

#define REG_APB6_RTC_SET                (*((volatile unsigned long *)   0x00806608))
#define BIT_RTC_SECOND                  0
#define BIT_RTC_MINUTE                  6
#define BIT_RTC_HOUR                    12
#define BIT_RTC_WEEK                    17
#define SET_RTC_SECOND                  (0x3F << BIT_RTC_SECOND )
#define SET_RTC_MINUTE                  (0x3F << BIT_RTC_MINUTE )
#define SET_RTC_HOUR                    (0x1F << BIT_RTC_HOUR   )
#define SET_RTC_WEEK                    (0x7  << BIT_RTC_WEEK   )

#define REG_APB6_RTC_ALM_TIME           (*((volatile unsigned long *)   0x0080660C))
#define BIT_RTC_ALARM_SECOND            0
#define BIT_RTC_ALARM_MINUTE            6
#define BIT_RTC_ALARM_HOUR              12
#define BIT_RTC_ALARM_MILLISEC          17
#define SET_RTC_ALARM_SECOND            (0x3F  << BIT_RTC_ALARM_SECOND   )
#define SET_RTC_ALARM_MINUTE            (0x3F  << BIT_RTC_ALARM_MINUTE   )
#define SET_RTC_ALARM_HOUR              (0x1F  << BIT_RTC_ALARM_HOUR     )
#define SET_RTC_ALARM_MILLISEC          (0x3FF << BIT_RTC_ALARM_MILLISEC )

#define REG_APB6_RTC_ALM_FLAG           (*((volatile unsigned long *)   0x00806610))

// RTC Test Firmware Definition
#define BK_RTC_CLOCK_MODE         0
#define BK_RTC_MILLISEC_MODE      1

//=====================================================================================
// AUX
//=====================================================================================
#define REG_AHB1_LC_INTR_STATUS         (*((volatile unsigned long *)   0x00801614))


//=====================================================================================
// ADC
//=====================================================================================
#define REG_APB7_ADC_CFG                (*((volatile unsigned long *)   0x00806700))
#define REG_APB7_ADC_DAT                (*((volatile unsigned long *)   0x00806704))

#define BIT_ADC_MODE                    0
#define BIT_ADC_EN                      2
#define BIT_ADC_CHNL                    3
#define BIT_ADC_FIFO_EMPTY              6
#define BIT_ADC_FIFO_FULL               7
#define BIT_ADC_SAMPLE_RATE             8
#define BIT_ADC_WAIT_CLK_SETTING        10
#define BIT_ADC_VALID_MODE              11
#define BIT_ADC_INT_CLEAR               15
#define BIT_ADC_CLK_RATE                16
#define SET_ADC_MODE                    (0x3 << BIT_ADC_MODE            )
#define SET_ADC_EN                      (0x1 << BIT_ADC_EN              )
#define SET_ADC_CHNL                    (0x7 << BIT_ADC_CHNL            )
#define SET_ADC_FIFO_EMPTY              (0x1 << BIT_ADC_FIFO_EMPTY      )
#define SET_ADC_FIFO_FULL               (0x1 << BIT_ADC_FIFO_FULL       )
#define SET_ADC_SAMPLE_RATE             (0x3 << BIT_ADC_SAMPLE_RATE     )
#define SET_ADC_WAIT_CLK_SETTING        (0x1 << BIT_ADC_WAIT_CLK_SETTING)
#define SET_ADC_VALID_MODE              (0x3 << BIT_ADC_VALID_MODE      )
#define SET_ADC_INT_CLEAR               (0x1 << BIT_ADC_INT_CLEAR       )
#define SET_ADC_CLK_RATE                (0x3 << BIT_ADC_CLK_RATE        )
//=====================================================================================


//FLASH/RAM space (program)
#define FLASH_ADDR0                     (*((volatile unsigned long *)   0x0000F000))
#define FLASH_ADDR1                     (*((volatile unsigned long *)   0x0000F100))
#define FLASH_ADDR2                     (*((volatile unsigned long *)   0x0000F200))
#define FLASH_ADDR3                     (*((volatile unsigned long *)   0x0000F300))


#endif
