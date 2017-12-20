//**********************************************************************//
//File          : BKPeriUnitAPI.c                                   //
//Author        : Jun Yu                                                //
//Corp          : Beken Co.Ltd(Shanghai)                                //
//Date          : 2013/01/28                                            //
//Description   : Application Interface Source File For Other Periphral //
//                Unit in BK Chip.                                  //
//**********************************************************************//

#include "BKAPI.h"
#include "debug.h"
#include "le_cts_app.h"
#include "sys_features.h"

//ADC
volatile u_int8 ADC_finish = FALSE;
volatile u_int8 ADC_captrue = FALSE;
static volatile u_int32 adc_config_value;

static u_int8 mem_buf[MEM_BUF_LEN];
t_sys_vars *g_sys_vars;

//----------------------------------------------------------------------
// Setup SAR-ADC Configurations
// ADC Mode configuration bit is used to enable SAR-ADC.
// In One-Step Mode (ADC Mode=0x1), ADC run for one convertion only.
// Re-config ADC Mode=0x1 to start ADC again
void Beken_ADC_Config(BKADCCONFIG_STRU *p_ADC_Config)
{
    adc_config_value  = 0x0 ; // Clear current ADC Configurations
    adc_config_value |= ((p_ADC_Config->mode       ) << BIT_ADC_MODE            ) ;
    adc_config_value |= ((p_ADC_Config->channel    ) << BIT_ADC_CHNL            ) ;
    adc_config_value |= ((p_ADC_Config->sample_rate) << BIT_ADC_SAMPLE_RATE     ) ;
    adc_config_value |= ((p_ADC_Config->settling   ) << BIT_ADC_WAIT_CLK_SETTING) ;
    adc_config_value |= ((p_ADC_Config->validcount ) << BIT_ADC_VALID_MODE      ) ;
    adc_config_value |= ((p_ADC_Config->clk_rate   ) << BIT_ADC_CLK_RATE        ) ;
    REG_APB7_ADC_CFG = adc_config_value ;
}
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// Initial GPIO, peripheral clock and IRQ for SAR-ADC. Enable SAR-ADC to process Input Data
void Beken_ADC_Initial(void)
{
    u_int8  channel_sel;

    channel_sel = adc_config_value>>3;
    // Enable GPIO-D for SAR-ADC Detection
    //GPIO-D[7:0] UART 2nd func enable. Output disable. Input disable.
    //GPIO-D[7:0] No Pull-Up. No Pull-Down.
    //REG_APB5_GPIOD_CFG  = 0x0000FF00;//for ADC input
	  
    //REG_APB5_GPIOD_DATA = 0x0000FFFF;

    REG_APB5_GPIOD_CFG &= ~(1<<channel_sel);
    REG_APB5_GPIOD_CFG |= (1<<channel_sel)<<8;
    REG_APB5_GPIOD_CFG &= ~(1<<channel_sel<<16);
    REG_APB5_GPIOD_CFG &= ~(1<<channel_sel<<24);

    REG_APB5_GPIOD_DATA &= ~(1<<channel_sel<<16);
}
//----------------------------------------------------------------------

// Function to set ADC Mode only. Used for software-control mode or one-step mode
void Beken_ADC_Mode_Config(u_int8 adc_mode)
{
    REG_APB7_ADC_CFG &= (~SET_ADC_MODE) ;
    REG_APB7_ADC_CFG |= (adc_mode << BIT_ADC_MODE);
}
//----------------------------------------------------------------------

void Beken_ADC_PowerUP(void)
{
    REG_AHB0_ICU_ADCCLKCON    = 0x0 ;                //ADC clock enable
    REG_AHB0_ICU_INT_ENABLE  |= INT_STATUS_ADC_bit;  //IRQ ADC interrupt enable
}
void BKADC_CLK_PowerDown(void)
{
    REG_ICU_ADCCLKCON  = 1;                                //ADC clock disable
    REG_APB5_GPIOD_CFG  = 0xF708FFFF ; /* GPIO-D(P3.3),pull high */
    REG_APB5_GPIOD_DATA |= (0x0C << BIT_GPIO_INPUT_EN);  // GPIO-D[2],[3] are input GPIO-D[2]---> RF_test_enable,GPIO-D[3]--> button
    //REG_AHB0_ICU_ANALOG0_PWD = 0xFFFFFFFF;  //At initialization, force pwdCB-Bias=1
    //REG_AHB0_ICU_ANALOG0_PWD = 0xFFFFFFE0;
}

void BKADC_INT_ENABLE(void)
{
    REG_ICU_INT_ENABLE |= INT_STATUS_ADC_bit;            //interrupt enable
}

void BKADC_INT_DISABLE(void)
{
    REG_ICU_INT_ENABLE &= ~INT_STATUS_ADC_bit;            //interrupt disable
}

void BKADC_ENABLE(void)
{
    unsigned int value = REG_ADC_CFG;
    value |= ADC_ENABLE;
    REG_ADC_CFG = value;
}
void BKADC_DISABLE(void)
{
    REG_ADC_CFG = 0;
}

//********************************************************//
//Function: BK ADC Interrupt Service
//Parameter:
//          None
//********************************************************//
void BKADCIntService(void)
{
    ADC_finish = TRUE;
    Save_Battery_level();
    REG_APB7_ADC_CFG |= SET_ADC_INT_CLEAR;
}

void read_adc_start(void)
{
    BKADCCONFIG_STRU adc_config_parameter;

  //adc_config_parameter.mode        = 0x03; //Continual Mode 
    adc_config_parameter.mode        = 0x02; //Software-controlled ADC mode 
    adc_config_parameter.channel     = 0x01; //Test Channel 1 GPIOD_Pin1 Battery Detect pin
    adc_config_parameter.sample_rate = 0x01;
    adc_config_parameter.settling    = 0x01;
    adc_config_parameter.validcount  = 0x02;
    adc_config_parameter.clk_rate    = 0x03;

    Beken_ADC_PowerUP();
    Beken_ADC_Config(&adc_config_parameter);
    Beken_ADC_Initial(); 
}

void Power_ON_Detect_Battery(void)
{
    u_int8 Battery_Capacity;

    read_adc_start();

    while(ADC_finish == FALSE); //INT mode
    ADC_finish = TRUE;

//    BKADC_DISABLE();/* /*  revision 335 change */ this line let chip can not make ADC int  */
    BKADC_CLK_PowerDown();
    BKADC_INT_DISABLE();

    Battery_Capacity = Battery_level_to_Percentage(Get_Battery());
    Save_Power_on_Battery(Battery_Capacity);
}

//********************************************************//
//Function: BK PWM Initial
//Parameter:
//          index: PWM number
//                              PT0_MODE_BIT: pwm0
//                              PT1_MODE_BIT: pwm1
//                              PT2_MODE_BIT: pwm2
//                              PT3_MODE_BIT: pwm3
//                              PT4_MODE_BIT: pwm4
//          type: set PWM or TIMER
//                              BK_TIMER_MODE: timer with PWM interrupt
//                              BK_PWM_MODE  : PWM output GPIO output
//                      counter:
//                              Timer Mode:  When count to value=counter, output interrupt
//                              PWM Mode  :  When count to value=counter, flip output on GPIO
//********
/************************************************/
void BKPWMInit(unsigned int index, unsigned char type, unsigned int counter)
{
    if(type == BK_TIMER_MODE)
    {
        switch(index)
        {
        case PT0_MODE_BIT:
            {
                REG_APB1_PWM_0 =  counter<<BIT_TIMER_MODE_START;
                REG_AHB0_ICU_INT_ENABLE |= INT_STATUS_PWM0_bit;
                REG_APB1_PWM_CFG        &= (~(1<<PT0_MODE_BIT));
                break;
            }
        case PT1_MODE_BIT:
            {
                REG_APB1_PWM_1 =  counter<<BIT_TIMER_MODE_START;
                REG_AHB0_ICU_INT_ENABLE |= INT_STATUS_PWM1_bit;
                REG_APB1_PWM_CFG        &= (~(1<<PT1_MODE_BIT));
                break;
            }
        case PT2_MODE_BIT:
            {
                REG_APB1_PWM_2 =  counter<<BIT_TIMER_MODE_START;
                REG_AHB0_ICU_INT_ENABLE |= INT_STATUS_PWM2_bit;
                REG_APB1_PWM_CFG        &= (~(1<<PT2_MODE_BIT));
                break;
            }
        case PT3_MODE_BIT:
            {
                REG_APB1_PWM_3 =  counter<<BIT_TIMER_MODE_START;
                REG_AHB0_ICU_INT_ENABLE |= INT_STATUS_PWM3_bit;
                REG_APB1_PWM_CFG        &= (~(1<<PT3_MODE_BIT));
                break;
            }
        case PT4_MODE_BIT:
            {
                REG_APB1_PWM_4 =  counter<<BIT_TIMER_MODE_START;
                REG_AHB0_ICU_INT_ENABLE |= INT_STATUS_PWM4_bit;
                REG_APB1_PWM_CFG        &= (~(1<<PT4_MODE_BIT));
                break;
            }
        default : break;
        }
    }
    else if(type == BK_PWM_MODE)
    {
        switch(index)
        {
        case PT0_MODE_BIT:
            {
                REG_APB1_PWM_0    = (counter<<BIT_TIMER_MODE_START) + (counter>>1);
                REG_APB1_PWM_CFG |= (0x1 << PT0_MODE_BIT);
                break;
            }
        case PT1_MODE_BIT:
            {
                REG_APB1_PWM_1    = (counter<<BIT_TIMER_MODE_START) + (counter>>1);
                REG_APB1_PWM_CFG |= (0x1 << PT1_MODE_BIT);
                break;
            }
        case PT2_MODE_BIT:
            {
                REG_APB1_PWM_2    = (counter<<BIT_TIMER_MODE_START) + (counter>>1);
                REG_APB1_PWM_CFG |= (0x1 << PT2_MODE_BIT);
                break;
            }
        case PT3_MODE_BIT:
            {
                REG_APB1_PWM_3    = (counter<<BIT_TIMER_MODE_START) + (counter>>1);
                REG_APB1_PWM_CFG |= (0x1 << PT3_MODE_BIT);
                break;
            }
        case PT4_MODE_BIT:
            {
                REG_APB1_PWM_4    = (counter<<BIT_TIMER_MODE_START) + (counter>>1);
                REG_APB1_PWM_CFG |= (0x1 << PT4_MODE_BIT);
                break;
            }
        default : break;
        }
				#ifndef ANA_TEST_ON
        //REG_APB5_GPIOB_CFG  &= ~(1<<index);                       //GPIO pwm0-4 2nd func enable
       // REG_APB5_GPIOB_DATA &= ~(1<<(index+BIT_GPIO_INPUT_EN)) ;  // Clear GPIO pwm0-4 input enable
				#endif
    }
    Beken_PWM_ENABLE();
    //REG_APB1_PWM_CFG    |=  (1<<PT_ENABLE_BIT);     // Enable PWM
}
//------------------------------------------------
// Setup PWM clock division register
// Parameter:
//     pwm_clk_div==0 -> Divide PWM clock by 1
//     pwm_clk_div==1 -> Divide PWM clock by 2
//     ......
//     pwm_clk_div==15-> Divide PWM clock by 16
//------------------------------------------------
void Beken_PWM_CLK_Division(unsigned char pwm_clk_div)
{
    REG_APB1_PWM_CFG &= (~SET_PT_CLK_DIV) ;
    REG_APB1_PWM_CFG  = (pwm_clk_div << PT_CLK_DIV_BIT) ;
}

void Beken_PWM_CLK_PowerUp(void)
{
    REG_AHB0_ICU_PWMCLKCON &= 0x0 ;    //PWM clock enable
}

void Beken_PWM_CLK_PowerDown(void)
{
    REG_AHB0_ICU_PWMCLKCON |= 0x1 ;    //PWM clock disable
}

void Beken_PWM_ENABLE(void)
{
    REG_APB1_PWM_CFG    |=  (1<<PT_ENABLE_BIT);     // Enable PWM
}

void Beken_PWM_DISABLE(void)
{
    REG_APB1_PWM_CFG    &=  ~(1<<PT_ENABLE_BIT);     // Enable PWM
}
//********************************************************//
//Function: BK PWM Interrupt service
//Parameter:
//          None
//********************************************************//
void BKPWMIntService(unsigned char PwmIdx)
{
#ifdef BEKEN_PWM_MODULE_TEST
    if((PwmIdx>=0) && (PwmIdx<=4))
        timer_cnt[PwmIdx]++;
#endif
    switch(PwmIdx)
    {
    case 0x00:  //add user code
        REG_PWM_CFG |= PT0_INT_FLAG;
        break;
    case 0x01:  //add user code
        REG_PWM_CFG |= PT1_INT_FLAG;
        break;
    case 0x02:  //add user code
        REG_PWM_CFG |= PT2_INT_FLAG;
        break;
    case 0x03:  //add user code
        REG_PWM_CFG |= PT3_INT_FLAG;
        break;
    case 0x04:  //add user code
        REG_PWM_CFG |= PT4_INT_FLAG;
        break;
    default:
        break;
    }
}

/// NVR space
void erase_nvr(u_int32 addr) {
	REG_AHB3_MFC_WP0 = 0xa5;	/**< W/P disable */
	REG_AHB3_MFC_WP1 = 0xc3;
    REG_AHB3_MFC_KEYWORD = 0x58a9;
    REG_AHB3_MFC_KEYWORD = 0xa958;
    REG_AHB3_MFC_ADDR = addr;
    REG_AHB3_MFC_CONTROL = MFC_CONTROL_SECT_ERASE | MFC_CONTROL_NVR_SPACE | 0x01;     //start erase
	while(REG_AHB3_MFC_CONTROL&1);
	REG_AHB3_MFC_WP0 = 0;	/**< restor W/P */
	REG_AHB3_MFC_WP1 = 0;
}

void read_nvr(u_int32 addr, u_int32 *pData, u_int32 len) {
    REG_AHB3_MFC_WP0 = 0xa5;
    REG_AHB3_MFC_WP1 = 0xc3;
    while (len--) {
        REG_AHB3_MFC_KEYWORD = 0X58A9;
        REG_AHB3_MFC_KEYWORD = 0XA958;
        REG_AHB3_MFC_ADDR = addr++; 
        REG_AHB3_MFC_CONTROL = MFC_CONTROL_READ | MFC_CONTROL_NVR_SPACE | 0x01;     //start write
		while(REG_AHB3_MFC_CONTROL&1);
        *pData++ = REG_AHB3_MFC_DATA;
    }
    REG_AHB3_MFC_WP0 = 0;
    REG_AHB3_MFC_WP1 = 0;
}

void write_nvr(u_int32 addr, u_int32 *pData, u_int32 len) {
    REG_AHB3_MFC_WP0 = 0xa5;
    REG_AHB3_MFC_WP1 = 0xc3;
    while (len--) {
        REG_AHB3_MFC_KEYWORD = 0X58A9;
        REG_AHB3_MFC_KEYWORD = 0XA958;
        REG_AHB3_MFC_ADDR = addr++; 
        REG_AHB3_MFC_DATA = *pData++;
        REG_AHB3_MFC_CONTROL = MFC_CONTROL_WRITE | MFC_CONTROL_NVR_SPACE | 0x01;     //start write
		while(REG_AHB3_MFC_CONTROL&1);
    }
    REG_AHB3_MFC_WP0 = 0;
    REG_AHB3_MFC_WP1 = 0;
}

void FlashStorageInit(void)
{
    u_int8 i;
    u_int32      flash_addr;
    u_int32     temp;
    
    REG_AHB3_MFC_WP0 = 0xa5;
    REG_AHB3_MFC_WP1 = 0xc3;

    flash_addr = MEM_START_ADDR;

    for(i=0; i<(MEM_BUF_LEN/4); i++)
    {
        REG_AHB3_MFC_KEYWORD = 0x58a9;
        REG_AHB3_MFC_KEYWORD = 0xa958;

        REG_AHB3_MFC_ADDR = flash_addr;
        REG_AHB3_MFC_CONTROL = MFC_CONTROL_READ |MFC_CONTROL_MAIN_SPACE | 0x01;
        while(REG_AHB3_MFC_CONTROL&0x01);   //wait read finish

        temp = REG_AHB3_MFC_DATA;

        memcpy(&mem_buf[i*4], &temp, 4);



        flash_addr++;
    }
    g_sys_vars = (t_sys_vars*)mem_buf;

}

