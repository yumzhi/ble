#ifndef _BK_API_H_
#define _BK_API_H_


#if 1
#include "stdio.h"
#include "string.h"
#include "BKReg.h"
//#include "BKTestApp.h"
#include "sys_types.h"
#include "sys_hal_config.h"

#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define MAX(a,b) ((a > b) ? a : b)
#define MIN(a,b) ((a < b) ? a : b)

typedef struct DataBuffer
{
    unsigned char *pBuf;
    unsigned int  WrPtr, RdPtr;
    unsigned int  VldDataNum;
    unsigned char LastPkt;
} DataBuff_t;

//BKSystemAPI
void initial_xver_BK3431V2_openLoop(void);
void initial_xver_BK3431V2_closeLoop(void);
void initial_xver_BK3431V2(void);
void kmod_calibration(void);
void xver_soft_reset(void);


void GPIO_initial(void);
void Beken_GPIO_IntService(void);
u_int8 Read_GPIOD(u_int8 pin_no);
void Peripheral_initial(void);
void Enable_16M_Clock_When_Boot(void);

void Beken_PWM_CLK_Division(unsigned char pwm_clk_div);
void Beken_PWM_CLK_PowerUp(void);
void Beken_PWM_CLK_PowerDown(void);
void Beken_PWM_ENABLE(void);
void Beken_PWM_DISABLE(void);
void Setting_WDT_TIMER(u_int16 time);
//#define BK_WAKEUP_COND_1        0x00200000	//BK5823 wakeup enable
#define BK_WAKEUP_COND_1        0x00000000	//BK5823 wakeup disable
#define BK_WAKEUP_COND_2        0x00002000	//gpiof[5] wakeup enable for test

#define MFC_READ                    0x00        
#define MFC_WRITE                   0x01
#define MFC_SECTOR_EARSE            0x02
#define MFC_BLOCK_EARSE             0x03
#define MFC_CHIP_EARSE              0x07
#define MAIN_SPACE                  0x00
#define NVR_SPACE                   0x01

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



#define BIT_RTC_ENABLE                  0
#define BIT_RTC_CLEAR                   1
#define BIT_RTC_ALARM_EN                2
#define BIT_RTC_ALARM_MODE              3
#define SET_RTC_ENABLE                  (0x1 << BIT_RTC_ENABLE     )
#define SET_RTC_CLEAR                   (0x1 << BIT_RTC_CLEAR      )
#define SET_RTC_ALARM_EN                (0x1 << BIT_RTC_ALARM_EN   )
#define SET_RTC_ALARM_MODE              (0x1 << BIT_RTC_ALARM_MODE )

#define BIT_RTC_DIV                     0
#define BIT_RTC_MSUNIT                  2
#define BIT_RTC_SUNIT                   8
#define SET_RTC_DIV                     (0x3    << BIT_RTC_DIV   )
#define SET_RTC_MSUNIT                  (0x3F   << BIT_RTC_MSUNIT)
#define SET_RTC_SUNIT                   (0xFFFF << BIT_RTC_SUNIT )


#define BIT_RTC_SECOND                  0
#define BIT_RTC_MINUTE                  6
#define BIT_RTC_HOUR                    12
#define BIT_RTC_WEEK                    17
#define SET_RTC_SECOND                  (0x3F << BIT_RTC_SECOND )
#define SET_RTC_MINUTE                  (0x3F << BIT_RTC_MINUTE )
#define SET_RTC_HOUR                    (0x1F << BIT_RTC_HOUR   )
#define SET_RTC_WEEK                    (0x7  << BIT_RTC_WEEK   )

#define BIT_RTC_ALARM_SECOND            0
#define BIT_RTC_ALARM_MINUTE            6
#define BIT_RTC_ALARM_HOUR              12
#define BIT_RTC_ALARM_MILLISEC          17
#define SET_RTC_ALARM_SECOND            (0x3F  << BIT_RTC_ALARM_SECOND   )
#define SET_RTC_ALARM_MINUTE            (0x3F  << BIT_RTC_ALARM_MINUTE   )
#define SET_RTC_ALARM_HOUR              (0x1F  << BIT_RTC_ALARM_HOUR     )
#define SET_RTC_ALARM_MILLISEC          (0x3FF << BIT_RTC_ALARM_MILLISEC )

//======================================================
// RTC Test Firmware Definition
#define BK_RTC_CLOCK_MODE         0
#define BK_RTC_MILLISEC_MODE      1

void Delay(int num);
void SEL_32K_Clock_SOURCE(void);
void Enable_16M_Clock_When_Boot(void);
void set_sleep_start_curr_clk(u_int32 clk,u_int32 syssleep_interval);


//BKADCAPI
typedef enum {
	BK_ADC_MODE = 0x01,
	BK_ADC_CHNL = 0x02,
	BK_ADC_SAMPLE_RATE = 0x04,
	BK_ADC_SETTLING = 0x08,
	BK_ADC_CLK_RATE = 0x10,
	BK_ADC_VALID_COUNT = 0x20,
}BKADC_CONFIG_ENUM;

typedef struct BKADCCONFIG_STRU_s
{
	unsigned char	mode;			//0x00:pwrdown, 0x01:step, 0x02:software, 0x03:continue
	unsigned char	channel;		//channel selection
	unsigned char	sample_rate;	//0x00:adc_clk/36, 0x01:adc_clk/18
	unsigned char	settling;		//0x00:4 ADC_CLK,0x01:8 ADC_CLK
	unsigned char   validcount;  //0~3
	unsigned char	clk_rate;		//0x00: 4div, 0x01:8div, 0x02:16div, 0x03:32div
}BKADCCONFIG_STRU;

void BKADCIntService(void);
void Power_ON_Detect_Battery(void);
void read_adc_start(void);

//ADC
//__INLINE__ void BKADC_CLK_PowerUp(void);
void BKADC_CLK_PowerUp(void);
void BKADC_CLK_PowerDown(void);
void BKADC_INT_ENABLE(void);
void BKADC_INT_DISABLE(void);
void BKADC_ENABLE(void);
void BKADC_DISABLE(void);

void Beken_ADC_Config(BKADCCONFIG_STRU *p_ADC_Config);
void Beken_ADC_Initial(void);
void Beken_ADC_Mode_Config(unsigned char adc_mode);
void Beken_ADC_PowerUP(void);
void Beken_ADC_Disable(void);

//BKPWM
void BKPWMInit(unsigned int index, unsigned char type, unsigned int counter);
void BKPWMIntService(unsigned char PwmIdx);



//BKUART
#define UART_CLK 				16000000
#define UART_FIFO_DEPTH			16
#define BK_UART1	        0
#define BK_UART2	        1

typedef struct BKUART_CONTROL_STRU_internal
{
	unsigned char	*pTx;
	unsigned int 	txBufLen;
	unsigned int 	txIndex;	
	unsigned char	*pRx;
	unsigned int 	rxBufLen;
	unsigned int 	rxWrIndex;
	unsigned int	rxRdIndex;	
	unsigned int	active;							//active flag
	unsigned int 	busy;
}BKUART_CONTROL_STRU;

void BKUartInit(unsigned int index, unsigned int speed, unsigned char* pRx, unsigned int rxLen);
void BKUart1IntService(void);
void BKUartPrint(unsigned int index, unsigned char* pTx, unsigned int txLen);
int fputc(int ch, FILE *f);


#endif
#endif

