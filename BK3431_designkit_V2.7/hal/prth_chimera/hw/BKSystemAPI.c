//**********************************************************************//
//File          : BKSystemAPI.c                                     //
//Author        : Huaming Jiang                                         //
//Corp          : Beken Co.Ltd(Shanghai)                                //
//Date          : 2013/01/27                                            //
//Description   : Application Interface Source File For Soc System      //
//                in BK Chip.                                       //
//**********************************************************************//
//
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
#include "hw_jal_defs.h"

#include "lslc_irq.h"
#include "BKUart.h"

//frank 171123
#define JAL_BASE_ADDR1 0x0F26000 
#define BEKEN_xver_soft_reset_ADDR         (0x00000070 + JAL_BASE_ADDR1)/* 0x00F76070 */
#define BEKEN_xver_soft_reset_MASK         0x80000000
#define BEKEN_xver_soft_reset_SHFT         31


volatile u_int32 flag_sleep_with_32k=0;
volatile u_int32 flag_aux_intr_done=0;
u_int32 bak_ceva_intr=0;
extern u_int32 AUX_setting_value;
extern  t_LE_Config LE_config;
extern  volatile u_int8 RF_test_mode_enable;
BKADCCONFIG_STRU  ADCconfig={2,2,1,1,3,3};/* mode,channel_sel,sample_rate,settling,clk_rate */

u_int32 backup_xvr_analog_reg[0x60] = {0};

#ifdef AUX_CALIBRATE
u_int8 wakeup_from_32K = 0;
#endif

//********************************************************//
//Function: Enable_16M_Clock_When_Boot
//Parameter: None
//********************************************************//
void Enable_16M_Clock_When_Boot(void)
{
    REG_ICU_CLOCKSOURCE    = SYSCLK_HFSRC_16M;     //clock source select
    REG_ICU_CPUCLKCON      = 0x0 ;                 //clock division frequency factor
    REG_ICU_IRQTYPE_EN     = 0x03;                 //IRQ enable=1, FIQ enable=1
    REG_ICU_BLECLK_PWD     = 0x0;
    //REG_AHB0_ICU_ANALOG_MODE = 0;         //analog normal mode
    //REG_AHB0_ICU_ANALOG0_PWD = 0xFFFFFFEF;    //At initialization, force pwdCB-Bias=0

    // Do Not calibrate the 32K clock as the Find-Me device use external 32K XTAL
    //REG_AHB0_ICU_RC32K_CONFG   = (0x1<<1) + 0x1 ;
}

void Peripheral_initial(void)
{
    REG_AHB0_ICU_ADCCLKCON  = 0x1; //default turn off
    REG_AHB0_ICU_I2CCLKCON  = 0x1; //default turn off
    REG_AHB0_ICU_SPICLKCON  = 0x1; //default turn off
    REG_AHB0_ICU_WDTCLKCON  = 0x1; //default turn off
    #ifdef ENABLE_WDT
    REG_AHB0_ICU_WDTCLKCON  = 0x0; //default turn on
	Setting_WDT_TIMER(0xFFFF);
	#endif
    REG_AHB0_ICU_PWMCLKCON  = 0x1; //default turn off
    REG_AHB0_ICU_RTCCLKCON  = 0x1 ; //default turn off

    //#if (TRA_HCIT_DO_NOT_USED_UART == 1)
    if(RF_test_mode_enable == 0)
    {
        REG_ICU_INT_ENABLE &= ~INT_STATUS_UART1_bit;            //interrupt disable
        REG_AHB0_ICU_UARTCLKCON = 0x1; //default turn off
    }
    else
    {
        REG_ICU_INT_ENABLE |= INT_STATUS_UART1_bit;            //interrupt enable
        REG_AHB0_ICU_UARTCLKCON = 0x0;
    }
    //#endif

#if (MMI_ON == 1)
#if (BUZZER_ENABLE == 1)
#if (MMI_IO_ENABLE == 1)
    Beken_PWM_CLK_PowerUp(); //BUZZER
    Beken_PWM_CLK_Division(1);  //BUZZER PWM0
#endif
#endif
#endif
}

u_int8 Read_GPIOD(u_int8 pin_no)
{
    u_int32 GPIO_IN_DATA;

    REG_APB5_GPIOD_DATA |= (1<<pin_no<<16);          //Set pin_no as input
    GPIO_IN_DATA = REG_APB5_GPIOD_DATA;
    GPIO_IN_DATA = GPIO_IN_DATA >> 8;
    GPIO_IN_DATA = ((GPIO_IN_DATA >> pin_no)&0x01);

    return GPIO_IN_DATA;
}
void GPIO_initial(void)
{

    // Disable Input except [1]. Output Buffer default=0xFF
    REG_APB5_GPIOA_CFG  = 0xFF00FFFF;
    REG_APB5_GPIOA_DATA = 0x00000000;

    //REG_APB5_GPIOD_CFG  = 0xFF00FFFF; //default no used turn off
    //REG_APB5_GPIOD_DATA = 0x00000000; //default no used turn off
#ifndef GPIO_DEBUG  //For Normal No debug
    REG_APB5_GPIOB_CFG  = 0xFF00FFFF;
    REG_APB5_GPIOB_DATA = 0x00000000;
    REG_APB5_GPIOC_CFG  = 0xFF00FFFF;
    REG_APB5_GPIOC_DATA = 0x00000000;
    REG_APB5_GPIOD_CFG  = 0xFF00FFFF;
    REG_APB5_GPIOD_DATA = 0x00000000;
    REG_APB5_GPIOE_CFG  = 0xFF00FFFF;
    REG_APB5_GPIOE_DATA = 0x00000000;
#endif

#if (MMI_IO_ENABLE == 1)
    /* charles add for C_chip require */
    REG_APB5_GPIOB_DATA = 0x00000000;           /**< 0x01[7:0] : output data value
                                                   0x01[15:8]: input  data value
                                                   0x01[23:16] : 1 --> input / 0 --> not input ,0x00 */
    REG_APB5_GPIOB_CFG  = 0x0000e7FC;           /**< GPIO-B as Output. LED0(P1.3 no pull up/down),LED1(P1.4 no pull up/down)
                                                   0x0[7:0] : 1 --> GPIO / 0 --> second function , GPIOB_2~7 GPIO function,0xFC
                                                   0x0[15:8] : 1 --> not output / 0 --> output , GPIOB_0~2 not output,GPIOB3 output
                                                   GPIOB4 --> output,GPIOB5~7 --> not output,0xe7
                                                   0x00[23:16]: 1 --> pull up  / 0 --> not pull up,0x00
                                                   0x00[31:24]: 1 --> pull down  / 0 --> not pull down,0x00 */
    // Setup Input GPIOs
    //REG_APB5_GPIOD_CFG   = 0xF708FFFF;  //Pull-up GPIO-D[3] for DeepSleep status
    //REG_APB5_GPIOD_CFG   = 0x0008FFFF;  //Pull-up GPIO-D[3] for DeepSleep status
    REG_APB5_GPIOD_CFG  = 0xF708FFFF;
    REG_APB5_GPIOD_DATA |= (0x0C << BIT_GPIO_INPUT_EN);  // GPIO-E[2],[3] are input
#endif

#if (BB_TEST_PIN_EN == 1)
    REG_APB5_GPIOC_CFG        = 0x0000FF00;           //GPIO-C as Test Pin, Not Output
    REG_APB5_GPIOC_DATA       = 0x00000000;           //GPIO-C Not Input
#ifdef ANA_TEST_ON
    REG_APB5_GPIOB_CFG        = 0x0000FF00;           //GPIO-C as Test Pin, Not Output
    REG_APB5_GPIOB_DATA       = 0x00000000;           //GPIO-C Not Input
    REG_AHB0_ICU_DIGITAL_PWD |= SET_ANA_TEST_PIN;
#else
    REG_AHB0_ICU_DIGITAL_PWD |= SET_BB_TEST_PIN;
#endif
#endif

#ifdef GPIO_DEBUG //For DEBUG
    REG_APB5_GPIOE_CFG        = 0x000000FF;       // gpioe[0] pull up
    REG_APB5_GPIOE_DATA       = 0x00000000;       // GPIO-E Not Input
    //REG_APB5_GPIOB_CFG        = 0x000000FF;       // gpioe[0] 
    //REG_APB5_GPIOB_DATA       = 0x00000000;       // GPIO-E Not Input
#endif
    REG_ICU_BLECLK_PWD = 0; //BLE Clock power up
}

void Beken_GPIO_IntService(void)
{
    /*REG_AHB0_ICU_DEEP_SLEEP0    &= ~(0x08 << BIT_GPIOD_WAKE_EN);
      REG_ICU_INT_ENABLE &= ~INT_STATUS_GPIO_bit;
      REG_APB5_GPIO_WUATOD_ENABLE &= ~(0x08 << BIT_GPIOD_WU_TYPE);*/
}

//********************************************************//
//Function: SEL_32K_Clock_SOURCE
//Parameter: close 26M and enable 32K
//********************************************************//
void SEL_32K_Clock_SOURCE(void)
{
      #ifdef AUX_CALIBRATE
	   extern u_int8  timer_int_cnt;
        extern u_int8  delay_one_clk; //aux_error;
	   #endif

#ifdef SLEEP_WITH_32K
    u_int8 active_link = LE_config.active_link;
    t_LE_Connection* p_connection  = LEconnection_Find_Link_Entry(active_link);
#if (PRH_BS_CFG_SYS_CPU_NO_POWER_DOWN == 0)//for test remove CPU power down code
    {	 
        //REG_ICU_INT_ENABLE |= INT_ENABLE_BLE_bit;
        bak_ceva_intr = REG_CEVA_REG_4;
        REG_CEVA_REG_4 = 0xFFFF;/**< Disable CAVA interrupt */
        #ifdef AUX_CALIBRATE
        timer_int_cnt = 0;
	    delay_one_clk = 0;
	    #endif
			    #if 1 /*  revision 335 change */
        REG_APB5_GPIOD_CFG  = 0xF708FFFF;
        REG_APB5_GPIOD_DATA |= (0x08 << BIT_GPIO_INPUT_EN);
        REG_APB5_GPIO_WUATOD_TYPE   |= (0x08 << BIT_GPIOD_WU_TYPE);
        REG_APB5_GPIO_WUATOD_ENABLE |= (0x08 << BIT_GPIOD_WU_TYPE);
        REG_AHB0_ICU_DEEP_SLEEP0    |= (0x08 << BIT_GPIOD_WAKE_EN);
        REG_ICU_INT_ENABLE |= INT_STATUS_GPIO_bit;
        REG_APB5_GPIOA_CFG  = 0xFF00FFFF;
        REG_APB5_GPIOA_DATA = 0x00000000;
       // REG_APB5_GPIOC_CFG  = 0xFF00FFFF;
       // REG_APB5_GPIOC_DATA = 0x00000000;
#ifndef GPIO_DEBUG //For DEBUG
        REG_APB5_GPIOE_CFG  = 0xFF00FFFF;
        REG_APB5_GPIOE_DATA = 0x00000000;
#endif
#endif

        // Step 1. Set USE_LF and Start the 1st AUX_INTR
        HW_set_use_lf(1);
	    #if 0
        HW_set_aux_timer(1);
        HW_set_aux_tim_intr_mask(0);
        REG_AHB0_ICU_CORECLKCON   = 0x1; // CPU clock power-down to wait SLEEP_MODE=1
        if (flag_aux_intr_done == 1) flag_aux_intr_done=0; // Clear 1st SW AUX_INTR Flag
        #endif
         while(!HW_get_sleep_status());

        // Step 2. Start the 2nd AUX_INTR
        //DEBUG_MSG(0XC4);
#ifndef ANA_TEST_ON
        //REG_APB5_GPIOD_CFG  = 0xFF00FFFF; //for GPIO interrupt service do not turn off
        //REG_APB5_GPIOD_CFG  = 0xF708FFFF; //GPIO 3.3 input
        //REG_APB5_GPIOD_DATA = 0x00000000; //for GPIO interrupt service do not turn off
        //REG_APB5_GPIOD_DATA = 0x00080000; //GPIO 3.3 input
			
        //REG_APB5_GPIOB_CFG  = 0xFF00FFFF;
        //REG_APB5_GPIOB_DATA = 0x00000000;
#endif
        
        //AUX_setting_value = 1600;//for current measure
        HW_set_aux_timer(AUX_setting_value);
        HW_set_aux_tim_intr_mask(0);
       
        // Step 3. Power-Down 16MHz clock and CPU clock
        if(HW_get_sleep_status())
        {
           
            flag_sleep_with_32k=1;
            LE_config.device_sleep = 0x01;
#ifdef SEL_LPO_TYPE_XTAL
            REG_AHB0_ICU_CLKSRCSEL    = 0x0;        // Select 32KHz clock
#endif
            REG_AHB0_ICU_DIGITAL_PWD  = SET_CB_BAIS_PWD + SET_CLK48M_PWD;
#ifdef SEL_LPO_TYPE_XTAL
            REG_AHB0_ICU_DIGITAL_PWD |= SET_CLK16M_PWD ;
#endif
           	  if(XVR_REG11 == 0x13B20800)
						{
						
              REG_AHB0_ICU_ANALOG0_PWD &= (~0x7) ;
              REG_AHB0_ICU_ANALOG0_PWD |= 0x5; //HP-LDO and LP-LDO switch power-down for 2013Nov tape-out version
              REG_AHB0_ICU_ANALOG_MODE |= 0x7;
						}
						else
						{
						
							REG_AHB0_ICU_DIGITAL_PWD |= SET_HP_LDO_PWD ;  
						}
        }
        else
        {
					 #ifdef ENABLE_WDT
					 REG_ICU_INT_ENABLE &= (~INT_ENABLE_BLE_bit);
			     //DEBUG_MSG(0x8E);
		       Setting_WDT_TIMER(0x02);
					 #endif
					 //DEBUG_MSG(0XC8);
        }
#if 0
        REG_APB5_GPIOD_CFG  = 0xF708FFFF;
        REG_APB5_GPIOD_DATA |= (0x08 << BIT_GPIO_INPUT_EN);
        REG_APB5_GPIO_WUATOD_TYPE   |= (0x08 << BIT_GPIOD_WU_TYPE);
        REG_APB5_GPIO_WUATOD_ENABLE |= (0x08 << BIT_GPIOD_WU_TYPE);
        REG_AHB0_ICU_DEEP_SLEEP0    |= (0x08 << BIT_GPIOD_WAKE_EN);
        REG_ICU_INT_ENABLE |= INT_STATUS_GPIO_bit;
        REG_APB5_GPIOA_CFG  = 0xFF00FFFF;
        REG_APB5_GPIOA_DATA = 0x00000000;
        REG_APB5_GPIOC_CFG  = 0xFF00FFFF;
        REG_APB5_GPIOC_DATA = 0x00000000;
#ifndef GPIO_DEBUG //For DEBUG
        REG_APB5_GPIOE_CFG  = 0xFF00FFFF;
        REG_APB5_GPIOE_DATA = 0x00000000;
#endif
#endif
        if (flag_aux_intr_done == 1) flag_aux_intr_done=0;

        REG_AHB0_ICU_CORECLKCON   = 0x1; // ARM9 Core clock close

        while(!flag_aux_intr_done)
        {
            //DEBUG_MSG(Read_GPIOD(3));
            //MMI_button();
        }

        REG_AHB0_ICU_DEEP_SLEEP0    &= ~(0x08 << BIT_GPIOD_WAKE_EN);
        REG_ICU_INT_ENABLE &= ~INT_STATUS_GPIO_bit;
        REG_APB5_GPIO_WUATOD_ENABLE &= ~(0x08 << BIT_GPIOD_WU_TYPE);

        // Step 4. Wake-up from sleep with 32Kz by AUX_INTR Interrupt
        //DEBUG_MSG(0XC6);
        if (flag_aux_intr_done == 1) flag_aux_intr_done=0; // Clear 1st SW AUX_INTR Flag
        //GPIO_initial();
    }
#endif
#endif


    //REG_ICU_INT_ENABLE |= INT_ENABLE_BLE_bit;
    #ifdef AUX_CALIBRATE
		wakeup_from_32K = 1;
    timer_int_cnt = 0;
		LSLCirq_Disable_All_Tim_Intr_Except(JAL_TIM0_INTR_MSK_MASK |JAL_TIM2_INTR_MSK_MASK | JAL_TIM1_INTR_MSK_MASK |JAL_TIM3_INTR_MSK_MASK );
		#endif

    /* { */
    /*     u_int32 clk = HW_get_native_clk(); */
    /*     DEBUG_MSG((clk>>8) & 0xff); */
    /*     DEBUG_MSG((clk>>0) & 0xff); */
    /* } */
}

//********************************************************//
//Function: BK Delay
//Parameter: None
//********************************************************//
void Delay(int num)
{
    int x, y;
    for(y = 0; y < num; y ++ )
    {
        for(x = 0; x < 50; x++);
    }
}
void LE_delay_time(volatile int time)/* charles add */
{
    volatile u_int8 unit = 12;//loootimes:10=>78M clk,12=>90M clk
   if(time !=0)
   	{
        while(time--)
    	{
    	  while(unit--);
    	}
   	}
   /* for(y = 0; y < num; y ++ )
    {
        for(x = 0; x < 50; x++);
   }*/
}

void initial_xver_BK3431V2(void)
{
    // ZGF@20140411 Find-Me CEVA SW: Reg09=0xA4251C08, Set Reg09[26]=1 to enable External 32K
    //Analog Registers
    mHWreg_Assign_XVR_Regsiter(00, 0x0414B07B);
    mHWreg_Assign_XVR_Regsiter(01, 0x22A82C55);// 0x22A82455 modified by ZhaoYunfeng
    mHWreg_Assign_XVR_Regsiter(02, 0x34080551);
    mHWreg_Assign_XVR_Regsiter(03, 0x8D1FFC1F);
    mHWreg_Assign_XVR_Regsiter(04, 0xF093DC20);// 0x03D90AA8 modified by ZhaoYunfeng
    mHWreg_Assign_XVR_Regsiter(05, 0x04590AA8);
    mHWreg_Assign_XVR_Regsiter(06, 0x39521000);    /**< 20140423 yfchen:  disable agc in LNA */
    mHWreg_Assign_XVR_Regsiter(07, 0x18000100);
    mHWreg_Assign_XVR_Regsiter(08, 0xD7E21C28);    /**< 20140423 yfchen:  disable agc in RxIF path */
    mHWreg_Assign_XVR_Regsiter(09, 0xA4251C08);
    mHWreg_Assign_XVR_Regsiter(0A, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(0B, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(0C, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(0D, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(0E, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(0F, 0x00000000);
    //-------------------------------------------------------

    //XVR Registers initialization
    //mHWreg_Assign_XVR_Regsiter(20, 0x0295E5DC);
    mHWreg_Assign_XVR_Regsiter(21, 0x96000000);
    mHWreg_Assign_XVR_Regsiter(22, 0xAB6DB6DB);
    mHWreg_Assign_XVR_Regsiter(23, 0xA0222222);
    mHWreg_Assign_XVR_Regsiter(24, 0x00080780);
    mHWreg_Assign_XVR_Regsiter(25, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(26, 0x08100104);//0x08100100
    mHWreg_Assign_XVR_Regsiter(27, 0x0008C900);
    mHWreg_Assign_XVR_Regsiter(28, 0x01021010);
    mHWreg_Assign_XVR_Regsiter(29, 0x00104E00);
    mHWreg_Assign_XVR_Regsiter(2A, 0x0178385C);//0x01783864
    mHWreg_Assign_XVR_Regsiter(2B, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(2C, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(2D, 0x002AC441);
    mHWreg_Assign_XVR_Regsiter(2E, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(2F, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(30, 0x10010001);
    mHWreg_Assign_XVR_Regsiter(31, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(32, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(33, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(34, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(35, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(36, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(37, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(38, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(39, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(38, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(39, 0x00000000);
    //mHWreg_Assign_XVR_Regsiter(3A, 0x00148000);
    mHWreg_Assign_XVR_Regsiter(3A, 0x0014A000);
    mHWreg_Assign_XVR_Regsiter(3B, 0x00001040);//0x00003050
    mHWreg_Assign_XVR_Regsiter(3C, 0x98001CFF);
    mHWreg_Assign_XVR_Regsiter(3D, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(3E, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(3F, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(40, 0x01000000);
    mHWreg_Assign_XVR_Regsiter(41, 0x07050402);
    mHWreg_Assign_XVR_Regsiter(42, 0x120F0C0A);
    mHWreg_Assign_XVR_Regsiter(43, 0x221E1A16);
    mHWreg_Assign_XVR_Regsiter(44, 0x35302B26);
    mHWreg_Assign_XVR_Regsiter(45, 0x4B45403A);
    mHWreg_Assign_XVR_Regsiter(46, 0x635D5751);
    mHWreg_Assign_XVR_Regsiter(47, 0x7C767069);
    mHWreg_Assign_XVR_Regsiter(48, 0x968F8983);
    mHWreg_Assign_XVR_Regsiter(49, 0xAEA8A29C);
    mHWreg_Assign_XVR_Regsiter(4A, 0xC5BFBAB4);
    mHWreg_Assign_XVR_Regsiter(4B, 0xD9D4CFCA);
    mHWreg_Assign_XVR_Regsiter(4C, 0xE9E5E1DD);
    mHWreg_Assign_XVR_Regsiter(4D, 0xF5F3F0ED);
    mHWreg_Assign_XVR_Regsiter(4E, 0xFDFBFAF8);
    mHWreg_Assign_XVR_Regsiter(4F, 0xFFFFFFFE);
    //-------------------------------------------------------

    // Trigger 0x5,0x7
    mHWreg_Assign_XVR_Regsiter(05, 0x84590AA8); //Step1. Write 1 to bit[31]
    Delay(500);
    mHWreg_Assign_XVR_Regsiter(05, 0x04590AA8); //Step2. Write 0 to bit[31]
    mHWreg_Assign_XVR_Regsiter(07, 0x1C000100); //Step1. Write 1 to bit[26]
    Delay(500);
    mHWreg_Assign_XVR_Regsiter(07, 0x18000100); //Step2. Write 0 to bit[26]
    //-------------------------------------------------------
}
void initial_xver_BK3431V2_closeLoop(void)
{
    //ZGF@20140411 Find-Me CEVA SW: Reg09=0xA4251C08, Set Reg09[26]=1 to enable External 32K

    //Analog Registers
    mHWreg_Assign_XVR_Regsiter(00, 0x0414B07B);
    mHWreg_Assign_XVR_Regsiter(01, 0x22A82C55);// 0x22A82455 modified by ZhaoYunfeng
    mHWreg_Assign_XVR_Regsiter(02, 0x34090551);
    mHWreg_Assign_XVR_Regsiter(03, 0x8D1FFC1F);
    mHWreg_Assign_XVR_Regsiter(04, 0xF093DC20);
    mHWreg_Assign_XVR_Regsiter(05, 0x04590AA8);// 0x03D90AA8 modified by ZhaoYunfeng
    mHWreg_Assign_XVR_Regsiter(06, 0x39521000);
    mHWreg_Assign_XVR_Regsiter(07, 0x18000100);
    mHWreg_Assign_XVR_Regsiter(08, 0xD7E21C28);// 20140423 yfchen:  disable agc in RxIF path
    mHWreg_Assign_XVR_Regsiter(09, 0xA4251C08);
    mHWreg_Assign_XVR_Regsiter(0A, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(0B, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(0C, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(0D, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(0E, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(0F, 0x00000000);
    //-------------------------------------------------------

    //XVR initialization
    //mHWreg_Assign_XVR_Regsiter(20, 0x0295E5DC);
    mHWreg_Assign_XVR_Regsiter(21, 0x96000000);
    mHWreg_Assign_XVR_Regsiter(22, 0xAB6DB6DB);
    mHWreg_Assign_XVR_Regsiter(23, 0xA0222222);
    mHWreg_Assign_XVR_Regsiter(24, 0x00080780);
    mHWreg_Assign_XVR_Regsiter(25, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(26, 0x08100104);// 0x08100100 modified by ZhaoYunfeng. Enable Close-Loop TX
    mHWreg_Assign_XVR_Regsiter(27, 0x0008C900);
    mHWreg_Assign_XVR_Regsiter(28, 0x01021010);
    mHWreg_Assign_XVR_Regsiter(29, 0x00104E00);
    mHWreg_Assign_XVR_Regsiter(2A, 0x01783864);
    mHWreg_Assign_XVR_Regsiter(2B, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(2C, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(2D, 0x002AC441);//ZGF@20140428 for Analog Tx range Debug: wrong case: 0x002AC411
    mHWreg_Assign_XVR_Regsiter(2E, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(2F, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(30, 0x10010001);
    mHWreg_Assign_XVR_Regsiter(31, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(32, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(33, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(34, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(35, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(36, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(37, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(38, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(39, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(38, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(39, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(3A, 0x0014A000);
    mHWreg_Assign_XVR_Regsiter(3B, 0x00001040);// Sync-word is 24bit and non-reverse-order.
    mHWreg_Assign_XVR_Regsiter(3C, 0x98001CFF);
    mHWreg_Assign_XVR_Regsiter(3D, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(3E, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(3F, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(40, 0x01000000);
    mHWreg_Assign_XVR_Regsiter(41, 0x07050402);
    mHWreg_Assign_XVR_Regsiter(42, 0x120F0C0A);
    mHWreg_Assign_XVR_Regsiter(43, 0x221E1A16);
    mHWreg_Assign_XVR_Regsiter(44, 0x35302B26);
    mHWreg_Assign_XVR_Regsiter(45, 0x4B45403A);
    mHWreg_Assign_XVR_Regsiter(46, 0x635D5751);
    mHWreg_Assign_XVR_Regsiter(47, 0x7C767069);
    mHWreg_Assign_XVR_Regsiter(48, 0x968F8983);
    mHWreg_Assign_XVR_Regsiter(49, 0xAEA8A29C);
    mHWreg_Assign_XVR_Regsiter(4A, 0xC5BFBAB4);
    mHWreg_Assign_XVR_Regsiter(4B, 0xD9D4CFCA);
    mHWreg_Assign_XVR_Regsiter(4C, 0xE9E5E1DD);
    mHWreg_Assign_XVR_Regsiter(4D, 0xF5F3F0ED);
    mHWreg_Assign_XVR_Regsiter(4E, 0xFDFBFAF8);
    mHWreg_Assign_XVR_Regsiter(4F, 0xFFFFFFFE);
    //-------------------------------------------------------

    // Trigger 0x5,0x7
    mHWreg_Assign_XVR_Regsiter(05, 0x84590AA8); //Step1. Write 1 to bit[31]
    Delay(500);
    mHWreg_Assign_XVR_Regsiter(05, 0x04590AA8); //Step2. Write 0 to bit[31]
    mHWreg_Assign_XVR_Regsiter(07, 0x1C000100); //Step1. Write 1 to bit[26]
    Delay(500);
    mHWreg_Assign_XVR_Regsiter(07, 0x18000100); //Step2. Write 0 to bit[26]
}
void initial_xver_BK3431V2_openLoop(void)
{
    // ZGF@20140411 Find-Me CEVA SW: Reg09=0xA4251C08, Set Reg09[26]=1 to enable External 32K
    //Analog Registers
    mHWreg_Assign_XVR_Regsiter(00, 0x0414B07B);
    mHWreg_Assign_XVR_Regsiter(01, 0x22A82C55);// 0x22A82455 modified by ZhaoYunfeng
    mHWreg_Assign_XVR_Regsiter(02, 0x34080551);
    mHWreg_Assign_XVR_Regsiter(03, 0x8D1FFC1F);
    mHWreg_Assign_XVR_Regsiter(04, 0xF093DC20);// 0x03D90AA8 modified by ZhaoYunfeng
    mHWreg_Assign_XVR_Regsiter(05, 0x04590AA8);
    mHWreg_Assign_XVR_Regsiter(06, 0x39521000);    /**< 20140423 yfchen:  disable agc in LNA */
    mHWreg_Assign_XVR_Regsiter(07, 0x18000100);
    mHWreg_Assign_XVR_Regsiter(08, 0xD7E21C28);    /**< 20140423 yfchen:  disable agc in RxIF path */
	#ifdef SEL_LPO_TYPE_XTAL
    mHWreg_Assign_XVR_Regsiter(09, 0xA4251C08);
	#else
	mHWreg_Assign_XVR_Regsiter(09, 0xA0251C08);
	#endif
    mHWreg_Assign_XVR_Regsiter(0A, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(0B, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(0C, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(0D, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(0E, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(0F, 0x00000000);
    //-------------------------------------------------------

    //XVR Registers initialization
    //mHWreg_Assign_XVR_Regsiter(20, 0x0295E5DC);
    /* mHWreg_Assign_XVR_Regsiter(21, 0x95FF0000); */
    mHWreg_Assign_XVR_Regsiter(21, 0x96000000);//mHWreg_Assign_XVR_Regsiter(21, 0x95FD0000);
    mHWreg_Assign_XVR_Regsiter(22, 0xAB6DB6DB);
    mHWreg_Assign_XVR_Regsiter(23, 0xA0222222);
    mHWreg_Assign_XVR_Regsiter(24, 0x00080780);
    mHWreg_Assign_XVR_Regsiter(25, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(26, 0x08100904);
    mHWreg_Assign_XVR_Regsiter(27, 0x0008C900);
    mHWreg_Assign_XVR_Regsiter(28, 0x01021010);
    mHWreg_Assign_XVR_Regsiter(29, 0x001F7E00);
	#ifdef VERSION_D_CHIP
    mHWreg_Assign_XVR_Regsiter(2A, 0x01782864);
	#else
	mHWreg_Assign_XVR_Regsiter(2A, 0x01783864);
	// mHWreg_Assign_XVR_Regsiter(2A, 0x01782864); //3431C = 3431D frank 170506
	#endif
    mHWreg_Assign_XVR_Regsiter(2B, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(2C, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(2D, 0x002AC441);
    mHWreg_Assign_XVR_Regsiter(2E, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(2F, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(30, 0x10010001);
    mHWreg_Assign_XVR_Regsiter(31, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(32, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(33, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(34, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(35, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(36, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(37, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(38, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(39, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(38, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(39, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(3A, 0x00148000);
    /* mHWreg_Assign_XVR_Regsiter(3A, 0x0014A000); */
    mHWreg_Assign_XVR_Regsiter(3B, 0x00001040);//0x00003050
    mHWreg_Assign_XVR_Regsiter(3C, 0x98001CFF);
    mHWreg_Assign_XVR_Regsiter(3D, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(3E, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(3F, 0x00000000);
    mHWreg_Assign_XVR_Regsiter(40, 0x01000000);
    mHWreg_Assign_XVR_Regsiter(41, 0x07050402);
    mHWreg_Assign_XVR_Regsiter(42, 0x120F0C0A);
    mHWreg_Assign_XVR_Regsiter(43, 0x221E1A16);
    mHWreg_Assign_XVR_Regsiter(44, 0x35302B26);
    mHWreg_Assign_XVR_Regsiter(45, 0x4B45403A);
    mHWreg_Assign_XVR_Regsiter(46, 0x635D5751);
    mHWreg_Assign_XVR_Regsiter(47, 0x7C767069);
    mHWreg_Assign_XVR_Regsiter(48, 0x968F8983);
    mHWreg_Assign_XVR_Regsiter(49, 0xAEA8A29C);
    mHWreg_Assign_XVR_Regsiter(4A, 0xC5BFBAB4);
    mHWreg_Assign_XVR_Regsiter(4B, 0xD9D4CFCA);
    mHWreg_Assign_XVR_Regsiter(4C, 0xE9E5E1DD);
    mHWreg_Assign_XVR_Regsiter(4D, 0xF5F3F0ED);
    mHWreg_Assign_XVR_Regsiter(4E, 0xFDFBFAF8);
    mHWreg_Assign_XVR_Regsiter(4F, 0xFFFFFFFE);
    //-------------------------------------------------------

    // Trigger 0x5,0x7
    mHWreg_Assign_XVR_Regsiter(05, 0x84590AA8); //Step1. Write 1 to bit[31]
    Delay(500);
    mHWreg_Assign_XVR_Regsiter(05, 0x04590AA8); //Step2. Write 0 to bit[31]
    mHWreg_Assign_XVR_Regsiter(07, 0x1C000100); //Step1. Write 1 to bit[26]
    Delay(500);
    mHWreg_Assign_XVR_Regsiter(07, 0x18000100); //Step2. Write 0 to bit[26]

    mHWreg_Assign_XVR_Regsiter(05, 0x02D90AA8);
    //-------------------------------------------------------
}

void kmod_calibration(void) {
    u_int32 value;
    u_int32 value_kcal_result;

    XVR_REG25 |= (1<<16);

    HW_set_aux_timer(10); // delay 30ms
    HW_set_aux_tim_intr_mask(0);

    while(!flag_aux_intr_done);
    if (flag_aux_intr_done == 1) flag_aux_intr_done=0; // Clear 1st SW AUX_INTR Flag

    value = XVR_REG12;
    value = ((value & 0x1fff0000) >> 16);
    XVR_REG25 &= (~(1<<16));

    value_kcal_result = ((0x10a*0x100/value)&0x1ff);
    XVR_REG30 &= (~(0x1ff<<8));
    XVR_REG30 |= (value_kcal_result<<8);

    /* REG_APB3_UART_DATA = 0xBB; */
    //-------------------------------------------------------
}

void Setting_WDT_TIMER(u_int16 time) /* 1 tick = 0.512 ms */
{
	u_int32 value;
	value = 0x005A0000 | time ;
	p_APB0_WDT_BASE = value;
	value = 0x00A50000 | time ;
	p_APB0_WDT_BASE = value;
}

void delay_us(u_int32 num)
{
    //for 48M clock
    //100=12.5us
    //16=2us
    //8=1us
    //for 32.768 clock
    //8=1.6ms
    //16M clock
    //100=120us
    u_int32 i,j;
    for(i=0; i<num; i++)
        for(j=0; j<3; j++)
        {
            ;
        }
}

void xver_soft_reset(void)
{

    (*((volatile unsigned long *)BEKEN_xver_soft_reset_ADDR)) &= 0x7fffffff;
    delay_us(10);
     
    (*((volatile unsigned long *)BEKEN_xver_soft_reset_ADDR)) |= 0x80000000;
    delay_us(10);
    
    (*((volatile unsigned long *)BEKEN_xver_soft_reset_ADDR)) &= 0x7fffffff;
    delay_us(10);

    //Driver_wdt_feed_dog(0XFFFF);

    DEBUG_MSG(0x9a);
   
}





