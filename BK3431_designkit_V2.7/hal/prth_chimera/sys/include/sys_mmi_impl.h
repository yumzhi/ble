#ifndef _PARTHUS_SYS_MMI_IMPL_
#define _PARTHUS_SYS_MMI_IMPL_

/***********************************************************************
 *
 * MODULE NAME:    sys_mmi.h
 * PROJECT CODE:   BlueStream
 * DESCRIPTION:    generic debug event man-machine feedback
 * MAINTAINER:     Ivan Griffin
 * CREATION DATE:  20 September 2000
 *
 * SOURCE CONTROL: $Id: sys_mmi_impl.h,v 1.4 2004/07/07 14:30:51 namarad Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2004 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    20 Sept 2000 - IG  - initial version
 *
 *    
 ***********************************************************************/

#include "sys_config.h"
#include "sys_types.h"

typedef enum {
	MMI_IDLE = 0x00,
	MMI_POWER_ON_STATE,
	MMI_ADVERTISING_STATE,
	MMI_CONNECTION_SUCCESS_STATE,
	MMI_DISCONNECTED_STATE,
	MMI_LOST_CONNECTION_STATE,
	MMI_DEEPSLEEP_STATE,
	MMI_SEND_REPORT_STATE,
	ALERT_LEVEL_SETTING_STATE,
	
}MMI_STATE_t;
typedef enum {
	MMI_START,
	MMI_ACTIVE,
	MMI_NEXT_PROC,
	MMI_END
}MMI_SUB_STATE_t;
typedef void (*MMI_SUB_FUNCTION)(void);
typedef struct t_MMI_SUB_FUNC {
	MMI_SUB_FUNCTION MMI_sub_func;
	u_int16 time;//unit 312.5us
} t_MMI_SUB_FUNC;
void MMI_init(void);
void POWER_ON_MMI(void);
void ADVERTISING_MMI(void);
void CONNECTION_SUCCESS_MMI(void);
void DISCONNECTED_MMI(void);
void LOST_CONNECTION_MMI(void);
void SEND_DATA_MMI(void);
void DEEPSLEEP_MMI(void);
void ALERT_LEVEL_SETTING_MMI(void);
void DUMMY_FUNC(void);
void Set_MMI_State(MMI_STATE_t value);
void MMI_main(void);
MMI_STATE_t Get_MMI_State(void);
void Set_MMI_Cycle_time(u_int16 value);
void LED1_ON(void);
void LED0_ON(void);
//void LED_OFF(void);
void Set_current_button_state(u_int8 value);// 1 : not press, 0 : press
void Set_last_button_state(u_int8 value);// 1 : not press, 0 : press
#ifdef ANA_TEST_ON
#define MMI_ON 0//used LED and buzzer function
#define MMI_IO_ENABLE 0			/**< Deep sleep */
#define ADC_ON 0 //used battery level detect function
#define BUZZER_ENABLE 0
#else
#define MMI_ON 1//used LED and buzzer function
#define MMI_IO_ENABLE 0			/**< Deep sleep */
#define ADC_ON 0 //used battery level detect function
#define BUZZER_ENABLE 0
#endif
#endif
