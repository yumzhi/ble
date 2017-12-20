#ifndef _CEVA_LE_TEST__
#define _CEVA_LE_TEST__

/*********************************************************************
 * MODULE NAME:     le_test.h
 * PROJECT CODE:    BlueStream
 * DESCRIPTION:		The header file for test mode.
 * MAINTAINER:      Nicola Lenihan
 * DATE:            23-02-12        
 * SOURCE CONTROL:  
 *
 * LICENSE:
 *     This source code is copyright (c) 1999-2012 Ceva Inc.
 *     All rights reserved.
 *
 *********************************************************************/
#define MAX_TEST_DATA_SIZE 0x0A
#define TEST_MODE_ACCESS_ADDRESS 0x71764129 

// LE Test Mode States
#define LE_TEST_MODE_INACTIVE			  0x00
#define LE_TEST_MODE_ACTIVE				  0x01

// LE Test Mode Substates
#define LE_W4_PRE_TEST_TX				0x00
#define LE_W4_TEST_TX					0x01
#define	LE_W4_N_x_T2					0x02
#define LE_W4_T2_PRE_RX					0x03
#define LE_W4_TEST_RX					0x04
//#define LE_W4_NX_TEST_RX				0x05

typedef enum
{
    LE_TX_Psuedo_Random_9_bit               = 0, 
	LE_TX_11110000_Pattern					= 1,
	LE_TX_10_Pattern						= 2,
	LE_TX_Psuedo_Random_15_bit              = 3, 
	LE_TX_1_Pattern							= 4, 
    LE_TX_0_Pattern							= 5, 
    LE_TX_00001111_Pattern					= 6,
    LE_TX_01_Pattern						= 7
} t_letestScenario; 

/*
 * Function prototypes
 */
t_error LEtest_Receiver_Test(u_int8 rx_freq);
t_error LEtest_Transmitter_Test(u_int8 tx_freq, u_int8 len_of_test_data,
								u_int8 packet_payload);
u_int16 LEtest_Test_End(void);
t_error LEtest_Ctrl_Handle_Test_Mode_Event(u_int8 IRQ_Type);
void LEtest_Test_Event_Begin(void);

#endif
