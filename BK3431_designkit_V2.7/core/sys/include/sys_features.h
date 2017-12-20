#ifndef __PARTHUS_SYS_FEATURES__
#define __PARTHUS_SYS_FEATURES__

/***********************************************************************
 *
 * MODULE NAME:    sys_features.h
 * PROJECT CODE:   BlueStream
 * MAINTAINER:     Tom Kerwick
 * DATE:           Wed Jul 14 11:39:10 BST 1999
 *
 * SOURCE CONTROL: $Id: sys_features.h,v 1.2 2012/05/14 23:14:15 garyf Exp $ 
 *
 * LICENCE:
 *    This source code is copyright (c) 1999-2004 Ceva Inc.
 *    All rights reserved.
 *
 * REVISION HISTORY:
 *
 * This file is to be used for grouping features, both 
 * LMP and Parthus extended features.
 *
 ***********************************************************************/
#ifndef PRH_BS_FEATURES_DEFINITION_FILE_EXISTS


#ifndef PRH_BS_CFG_LOCAL_NAME
#define PRH_BS_CFG_LOCAL_NAME        "Ceva XpertBlue"
#endif

#define PRH_BS_CFG_LMP_VERSION        6
#define PRH_BS_CFG_LMP_SUBVERSION     0x0250

#define PRH_BS_CFG_HCI_VERSION        PRH_BS_CFG_LMP_VERSION
#define PRH_BS_CFG_HCI_REVISION       PRH_BS_CFG_LMP_SUBVERSION

#ifndef PRH_BS_CFG_MANUFACTURER_NAME
#define PRH_BS_CFG_MANUFACTURER_NAME  0x000D /* TI comp ID */ /* 0x000E  // CEVA comp ID */

#endif

#endif /* #ifndef PRH_BS_FEATURES_DEFINITION_FILE_EXISTS */

#define FLASH_BUILD                     1
#define UNIT_TEST_BUILD                 2
#define DEBUG_BUILD                     3
#define COMBINED_BUILD                  6
#define GENERIC_BUILD                   7

 
/*
 * Host Controller Configuration Options that are normally fixed for normal
 * HC Builds
 */
#define NDEBUG 0
#define STATIC static

//#define PRH_BS_CFG_REMOVE_UART           0
#define PRH_BS_CFG_UART_FOR_SENSOR_ONLY    0//merge CEVA 0517
#define PRH_BS_CFG_EMBEDDED_PULSE_SENSOR   0//merge CEVA 0517
#define PRH_BS_CFG_HACK_FOR_SAMSUNG_SIII_INTEROPERABILITY   0 //merge CEVA 0517 // Beware enabling this will disable inter with other devices

#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY == 1)//merge CEVA 0517
#define TRA_HCIT_UART_DEFAULT_BAUD_RATE 9600//merge CEVA 0517
#else
#define TRA_HCIT_UART_DEFAULT_BAUD_RATE /*230400*/ 115200//merge CEVA 0517
#endif

/***************************************************
 *
 * Enable/Disable the Host Layers and Applications
 *==================================================
 ***************************************************/

#define PRH_BS_CFG_SYS_EMBEDDED_PROFILES 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_SMP_INCLUDED 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_ATT_INCLUDED 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_GATT_INCLUDED 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_GAP_INCLUDED 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_L2CAP_INCLUDED 1//merge CEVA 0517
//#define PRH_BS_CFG_SYS_LE_UART_INCLUDED 0

/***************************************************
 * GATT SERVER BUILD/CONFIGURATION OPTIONS
 ***************************************************/

#if (PRH_BS_CFG_SYS_LE_GATT_INCLUDED==1)//merge CEVA 0517
/*
 * GATT Build options
 */

#define PRH_BS_CFG_SYS_LE_GATT_SERVER_SUPPORTED 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_GATT_CLIENT_SUPPORTED 1//merge CEVA 0517

#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==1)//merge CEVA 0517
#define PRH_BS_CFG_SYS_GATT_DELIVER_EVENTS_VIA_HCI 0//merge CEVA 0517
#define PRH_BS_CFG_SYS_GATT_ACCEPT_COMMANDS_VIA_HCI 0//merge CEVA 0517
#else//merge CEVA 0517
// GF Change 21 March 2013
#define PRH_BS_CFG_SYS_GATT_DELIVER_EVENTS_VIA_HCI 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_GATT_ACCEPT_COMMANDS_VIA_HCI 1//merge CEVA 0517
#endif//merge CEVA 0517

#define PRH_BS_CFG_SYS_LE_SERVER_SUPPORTS_128_BIT_UUIDS 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_SERVER_PTS_LARGE_DATABASE_1 0//merge CEVA 0517

#define PRH_BS_CFG_SYS_LE_SERVER_SUPPORTS_EXTENDED_PROPERTIES_DESCRIPTOR 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_SERVER_SUPPORTS_CLIENT_CHARACTERISTIC_CONFIGURATION_DESCRIPTOR 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_SERVER_SUPPORTS_USER_DESCRIPTION_DESCRIPTOR 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_SERVER_SUPPORTS_CHARACTERISTIC_FORMAT_DESCRIPTOR 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_SERVER_SUPPORTS_REPORT_REFERENCE_DESCRIPTOR 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_SERVER_SUPPORTS_EXTERNAL_REPORT_REFERENCE_DESCRIPTOR 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_SERVER_SUPPORTS_INCLUDED_SERVICES 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_SERVER_SUPPORTS_LOCAL_MODIFICATION_OF_PERMISSIONS 1//merge CEVA 0517

#define PRH_BS_CFG_SYS_LE_CLIENT_READ_BLOB_SUPPORTED  1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_SERVER_READ_BLOB_SUPPORTED  1//merge CEVA 0517

#define PRH_BS_CFG_SYS_LE_CLIENT_SUPPORTS_DISCOVERY_BY_UUID 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_CLIENT_SUPPORTS_128_BIT_UUIDS  0//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_CLIENT_SUPPORTS_READ_MULTIPLE 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_SERVER_SUPPORTS_READ_MULTIPLE 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_CLIENT_SUPPORTS_PREPARE_WRITE 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_SERVER_SUPPORTS_PREPARE_WRITE 1//merge CEVA 0517

/*
 * GATT CONFIGURATION OPTIONS
 */

#define PRH_BS_CFG_SYS_LE_CLIENT_GATT_MAX_LONG_WRITE  50//merge CEVA 0517

#endif  // Of GATT Options

/************************************************************
 * SMP BUILD OPTIONS
 ************************************************************/
#if (PRH_BS_CFG_SYS_LE_SMP_INCLUDED==1)//merge CEVA 0517
#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==1)//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_SMP_DELIVER_EVENTS_VIA_HCI 0//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_SMP_ACCEPT_COMMANDS_VIA_HCI 0//merge CEVA 0517
#else
#define PRH_BS_CFG_SYS_LE_SMP_DELIVER_EVENTS_VIA_HCI 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_SMP_ACCEPT_COMMANDS_VIA_HCI 1//merge CEVA 0517
#endif
// Key Hierarchy Determines if the Periheral will used Key Hierarchy or Security DB
// to get the LTK given and EDIV and Random.

#define PRH_BS_CFG_SYS_LE_SMP_PERIPHERAL_SUPPORTS_KEY_HIERARCHY 1//merge CEVA 0517

#define PRH_BS_CFG_SYS_LE_SMP_SUPPORTS_OOB 0//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_SMP_SUPPORTS_PASSKEY 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_SMP_API_TO_CONFIGURE_SECURITY 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_SMP_IRK_SUPPORTED 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_SMP_DATA_SIGNING 0//merge CEVA 0517

#endif  // Of SMP Build Options

/************************************************************
 * L2CAP BUILD OPTIONS
 ************************************************************/
#if (PRH_BS_CFG_SYS_LE_L2CAP_INCLUDED==1)//merge CEVA 0517
#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==0)//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_L2CAP_DELIVER_EVENTS_VIA_HCI 0//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_L2CAP_ACCEPT_COMMANDS_VIA_HCI 0//merge CEVA 0517
#else
#define PRH_BS_CFG_SYS_LE_L2CAP_DELIVER_EVENTS_VIA_HCI 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_L2CAP_ACCEPT_COMMANDS_VIA_HCI 1//merge CEVA 0517
#endif
#endif // Of L2CAP options

/************************************************************
 * GAP BUILD OPTIONS
 ************************************************************/
#if (PRH_BS_CFG_SYS_LE_GAP_INCLUDED==1)//merge CEVA 0517
#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY==0)//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_GAP_DELIVER_EVENTS_VIA_HCI 0//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_GAP_ACCEPT_COMMANDS_VIA_HCI 0//merge CEVA 0517
#else
#define PRH_BS_CFG_SYS_LE_GAP_DELIVER_EVENTS_VIA_HCI 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_GAP_ACCEPT_COMMANDS_VIA_HCI 1//merge CEVA 0517
#endif
#define PRH_BS_CFG_SYS_LE_GAP_PRIVACY_SUPPORTED 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_GAP_RECONNECTION_ADDRESS_SUPPORTED 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_GAP_AUTO_WRITE_RECONNECTION_ADDRESS 0//merge CEVA 0517

#endif // Of GAP Options

/************************************************************
 * Generic Device Configuration Options
 * These have significant bearing of Program code size
 ************************************************************///merge CEVA 0517
#define PRH_BS_CFG_SYS_ENCRYPTION_SUPPORTED 1
#define PRH_BS_CFG_SYS_LE_BROADCAST_DEVICE  1
#define PRH_BS_CFG_SYS_LE_OBSERVER_DEVICE   0
#define PRH_BS_CFG_SYS_LE_CENTRAL_DEVICE    0
#define PRH_BS_CFG_SYS_LE_PERIPHERAL_DEVICE 1

//merge CEVA 0517 remove #define PRH_BS_CFG_SYS_LE_SINGLE_MODE 1
//merge CEVA 0517 remove #define PRH_BS_CFG_SYS_LE_DUAL_MODE 0

//merge CEVA 0517 remove #define PRH_BS_CFG_SYS_LE_SMP_INCLUDED 0
//merge CEVA 0517 remove #define PRH_BS_CFG_SYS_LE_ATT_INCLUDED 0
//merge CEVA 0517 remove #define PRH_BS_CFG_SYS_LE_GATT_INCLUDED 0
//merge CEVA 0517 remove #define PRH_BS_CFG_SYS_LE_GAP_INCLUDED 0

//merge CEVA 0517 remove #define PRH_BS_CFG_SYS_LE_12MHZ_OPTIMISATION 0
//merge CEVA 0517 remove #define PRH_BS_CFG_SYS_LE_DISABLE_CRC_CHECK 0
/***********************************************************
 * DEGUG :- Tracing options and Extended HCI
 ***********************************************************/

#define  PRH_BS_CFG_SYS_EXTENDED_HCI_COMMANDS_SUPPORTED 1

#define PRH_BS_CFG_SYS_TRACE_LE_DEVICE_STATE_VIA_HCI_SUPPORTED 0//merge CEVA 0517
#define PRH_BS_CFG_SYS_TRACE_LLDATA_VIA_HCI_SUPPORTED 0//merge CEVA 0517
#define PRH_BS_CFG_SYS_TRACE_LLC_VIA_HCI_SUPPORTED 0// reduce memory space//CEVA 0517 org 1//merge CEVA 0517 
#define PRH_BS_CFG_SYS_TRACE_ADV_VIA_HCI_SUPPORTED 0// reduce memory space//CEVA 0517 org 1//merge CEVA 0517 
#define PRH_BS_CFG_SYS_TRACE_SMP_VIA_HCI_SUPPORTED 0// reduce memory space//CEVA 0517 org 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_TRACE_ATT_VIA_HCI_SUPPORTED 0// reduce memory space//CEVA 0517 org 1 //merge CEVA 0517
#define PRH_BS_CFG_SYS_TRACE_L2CAP_VIA_HCI_SUPPORTED 0//merge CEVA 0517

#define PRH_BS_CFG_SYS_TESTING_AGAINST_PTS4_5 0//merge CEVA 0517

/*******************************************************
 * Low Energy - Host Controller Configuration Options
 * [ have no impact on host ]
 ******************************************************/
/* Configuration Options */
#define PRH_CFG_MAX_ADV_DIRECT_DUPLICATE_ENTRIES 3//merge CEVA 0517
#define PRH_CFG_MAX_ADV_IND_DUPLICATE_ENTRIES 3//merge CEVA 0517
#define PRH_CFG_MAX_ADV_SCAN_IND_DUPLICATE_ENTRIES 3//merge CEVA 0517
#define PRH_CFG_MAX_ADV_NONCONN_IND_DUPLICATE_ENTRIES 3//merge CEVA 0517
#define PRH_CFG_MAX_SCAN_RSP_DUPLICATE_ENTRIES 3//merge CEVA 0517

#define PRH_BS_CFG_MASTER_SLEEP_CLOCK_ACCURACY 2
#define PRH_BS_CFG_DEFAULT_TRANSMIT_WINDOW_OFFSET   4
#define PRH_BS_CFG_DEFAULT_TRANSMIT_WINDOW_SIZE 4

#define PRH_BS_CFG_SYS_MAX_WHITE_LIST_ENTRIES 0x04//merge CEVA 0517
#define PRH_BS_CFG_LE_MAX_DEPTH_ADVERTISING_ARRAY 3//merge CEVA 0517
#define PRH_BS_CFG_LE_CONN_HANDLE_OFFSET 0x10
#define PRH_BS_CFG_LE_FIX_PLL_ISSUE 1

// These are the Default value in the BT 4.0 specification
// Should not be modified
#define LE_DEFAULT_SCAN_INTERVAL 0x10
#define LE_DEFAULT_SCAN_WINDOW 0x10

/*
 * Host Controll Build options//merge CEVA 0517
 */
#define PRH_BS_CFG_SYS_LE_SINGLE_MODE 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_DUAL_MODE 0//merge CEVA 0517

#define PRH_BS_CFG_SYS_LE_12MHZ_OPTIMISATION 0//merge CEVA 0517
#define PRH_BS_CFG_SYS_LE_DISABLE_CRC_CHECK 0//merge CEVA 0517


/* Build options for LE TESTMODE */
#define PRH_BS_CFG_SYS_TESTMODE_LE_TX_PSEUDO_RANDOM_15_BIT 1//merge CEVA 0517
#define PRH_BS_CFG_SYS_TESTMODE_LE_TX_PSEUDO_RANDOM_9_BIT 1//merge CEVA 0517



/********************************************************************
 * Host Controller Configuration Options that are variable but proven
 ********************************************************************/
#define TABASCO_OLD_SER_BLK_SUPPORTED 1

//*************************************************************************
// Legacy BT Classic Defines - set to Zero allways - DONT MODIFY for Single Mode
//******************************************************************************
#define PRH_BS_CFG_SYS_FLEXIBLE_DATA_BUFFER_SIZES_SUPPORTED 0
#define PRH_BS_CFG_SYS_SYN_VIA_HCI_SUPPORTED 0
#define PRH_BS_DEV_BIG_ENDIAN_AND_DIRECT_BASEBAND_MOVE 0
#define PRH_BS_CFG_SYS_LMP_EDR_SUPPORTED 0
#define PRH_BS_DEV_USE_CACHED_BT_CLOCKS 0
#define PRH_BS_CFG_SYS_LMP_RSSI_SUPPORTED 0
#define PRH_BS_CFG_SYS_ENHANCED_POWER_CONTROL_SUPPORTED 0
#define PRH_BS_DEV_USE_DELAYED_SERVICE_ROUTINES 0
#if (PRH_BS_CFG_UART_FOR_SENSOR_ONLY == 1)//merge CEVA 0517
#define PRH_BS_DEV_PROCESS_UART_RX_DATA_IN_IRQ_CONTEXT 0
#else//merge CEVA 0517
#define PRH_BS_DEV_PROCESS_UART_RX_DATA_IN_IRQ_CONTEXT 1//merge CEVA 0517
#endif//merge CEVA 0517
#define PRH_BS_DEV_UART_FIQ_ALWAYS_ENABLED 1    //charles modify
#define PRH_BS_CFG_SYS_TRACE_VIA_HCI_SUPPORTED 0
#define PRH_BS_CFG_SYS_TRACE_LC_VIA_HCI_SUPPORTED 0
#define PRH_BS_CFG_SYS_TRACE_BB_VIA_HCI_SUPPORTED 0
#define PRH_BS_CFG_SYS_TRACE_LMP_VIA_HCI_SUPPORTED 0
#define PRH_BS_CFG_SYS_TRACE_RADIO_VIA_HCI_SUPPORTED 0
#define PRH_BS_CFG_SYS_TRACE_INTR_VIA_HCI_SUPPORTED 0

#define PRH_BS_DBG_LC_LSLC_STAT 0
#define PRH_BS_DEV_SERVICE_TRANSPORT_DURING_SPINS_SUPPORTED 0
#define TRA_HCIT_COMBINED_SUPPORTED 0
#define PRH_BS_DBG_LC_LOG 0
#define PRH_BS_DBG_LC_LSLC_PKT 0
#define PRH_BS_DBG_LC_LSLC_IRQ 0
#define PRTH_BS_DBG_BB_PACKET_LOG_TYPE  0
#define PRH_BS_DEV_WIRED_LINK 0
#define PRH_BS_DBG_LC 0
#define PRH_BS_DBG_PERCENT_CE_SUCCESS_ON_7SEGMENT 0//merge CEVA 0517
// End if Legacy #defines
//*************************************************************************
/*
 * Major configuration parameters for Queues
 *
 * Please note: 
 * 1. ACL_IN_PACKET_LENGTH and ACL_OUT_PACKET_LENGTH will be aligned
 *    to ensure that PRH_BS_CFG_SYS_ACL_BUFFER_ALIGNMENT due to buffer 
 *    alignment constraints
 *    IN:  Radio Baseband to Host        OUT: Host to Radio Baseband
 * 2. The PRH_BS_CFG_SYS_ACL_SEGMENT_LENGTH_ALIGNMENT must be 1, 2 or 4 bytes
 *    and determines what the segment alignment for each segmented packet.
 *    Penalty is time required for tx queue-> baseband copy.
 */
#define PRH_BS_CFG_SYS_ACL_BUFFER_ALIGNMENT         16    /* Bytes  */
#define PRH_BS_CFG_SYS_ACL_SEGMENT_LENGTH_ALIGNMENT  4    /* Bytes */

#define PRH_BS_CFG_SYS_ACL_IN_PACKET_LENGTH    MIN_L2CAP_MTU   /* Bytes  */
#define PRH_BS_CFG_SYS_ACL_OUT_PACKET_LENGTH   MIN_L2CAP_MTU   /* Bytes  */


#define PRH_BS_CFG_SYS_SIZEOF_HCI_COMMAND_HEAP 0x80// reduce memory space//CEVA 0517 org 0x100 /* Bytes  *///merge CEVA 0517
#define PRH_BS_CFG_SYS_MAX_HCI_COMMANDS        2 // reduce memory space//CEVA 0517 org 4//merge CEVA 0517

#define PRH_BS_CFG_SYS_SIZEOF_HCI_EVENT_HEAP   0x80// reduce memory space//CEVA 0517 org 0x200 /* Bytes  *///merge CEVA 0517
#define PRH_BS_CFG_SYS_MAX_HCI_EVENTS          6// reduce memory space//CEVA 0517 org 10//merge CEVA 0517

#define PRH_BS_CFG_SYS_SIZEOF_LMP_IN_HEAP      0x80  /* Bytes  *///merge CEVA 0517
#define PRH_BS_CFG_SYS_MAX_LMP_IN_MESSAGES     4//merge CEVA 0517
#define PRH_BS_CFG_SYS_SIZEOF_LMP_OUT_HEAP     0x80  /* Bytes  *///merge CEVA 0517
#define PRH_BS_CFG_SYS_MAX_LMP_OUT_MESSAGES    4//merge CEVA 0517

#define PRH_BS_CFG_SYS_NUM_IN_ACL_PACKETS      5     /* Buffers*///merge CEVA 0517
#define PRH_BS_CFG_SYS_NUM_OUT_ACL_PACKETS     5    /* Buffers*///merge CEVA 0517


/*
 * Major configuration parameters for Piconet Devices
 */

#if (PRH_BS_CFG_SYS_LE_PERIPHERAL_DEVICE==1) && (PRH_BS_CFG_SYS_LE_CENTRAL_DEVICE!=1)
#define PRH_MAX_DEVICES_SUPPORTED                     2//merge CEVA 0517
#elif (PRH_BS_CFG_SYS_LE_CENTRAL_DEVICE==1)
#define PRH_MAX_DEVICES_SUPPORTED                    1//charles modify 20131205 reduce memory space //CEVA 0517 org 3//merge CEVA 0517
#else
#define PRH_MAX_DEVICES_SUPPORTED                     0
#endif
/*
 * MAX_ACTIVE_DEVICE_LINKS, 
 */
#define PRH_BS_CFG_SYS_MAX_DEVICE_CONTAINERS (PRH_MAX_DEVICES_SUPPORTED) 
#define MAX_ACTIVE_DEVICE_LINKS         PRH_BS_CFG_SYS_MAX_DEVICE_CONTAINERS
/*
 * Conditional Definitions
 */

#define PRH_BS_CFG_SYS_LOW_POWER_MODE_SUPPORTED 1
#define PRH_BS_CFG_SYS_CPU_NO_POWER_DOWN        0

#if (PRH_BS_CFG_SYS_LOW_POWER_MODE_SUPPORTED==1)
/*
 * Note:
 * The ability of a platform to support sleep functionality is dependent on 
 * the availability of a low frequency oscilalator (LFO).
 * 
 * The availability of this LFO is indicated by :
 *  SYS_LF_OSCILLATOR_PRESENT     (hal/___platform___/sys/include/"sys_hal_features.h" )
 */
#define PRH_BS_CFG_SYS_SLEEP_MASTER_SUPPORTED 1
#define PRH_BS_CFG_SYS_SLEEP_SLAVE_SUPPORTED 1
#define PRH_BS_CFG_SYS_SLEEP_IN_STANDBY_SUPPORTED 1
#else
#define PRH_BS_CFG_SYS_SLEEP_MASTER_SUPPORTED 0
#define PRH_BS_CFG_SYS_SLEEP_SLAVE_SUPPORTED 0
#define PRH_BS_CFG_SYS_SLEEP_IN_STANDBY_SUPPORTED 0
#endif


#define PRH_BS_CFG_SYS_HW_INDIVIDUAL_TIM_INTR_CLR 1


#if (PRH_BS_CFG_SYS_TRACE_LLC_VIA_HCI_SUPPORTED==1)	
/*
 * Must increase event queues to support LLC logging on HCI Event Queue
 * Size necessary to support testing of full length Remote Name Requests
 */
    #undef PRH_BS_CFG_SYS_SIZEOF_HCI_EVENT_HEAP
    #undef PRH_BS_CFG_SYS_MAX_HCI_EVENTS
    #define PRH_BS_CFG_SYS_SIZEOF_HCI_EVENT_HEAP   0x240//merge CEVA 0517 /* Bytes  */
    #define PRH_BS_CFG_SYS_MAX_HCI_EVENTS          16
#endif

#if (PRH_BS_CFG_SYS_TRACE_ADV_VIA_HCI_SUPPORTED==1)
/*
 * Must increase event queues to support ADV logging on HCI Event Queue
 * Size necessary to support testing of full length ADV messages
 */
    #undef PRH_BS_CFG_SYS_SIZEOF_HCI_EVENT_HEAP
    #undef PRH_BS_CFG_SYS_MAX_HCI_EVENTS
    #define PRH_BS_CFG_SYS_SIZEOF_HCI_EVENT_HEAP   0x500 /* Bytes  */
    #define PRH_BS_CFG_SYS_MAX_HCI_EVENTS          32

#endif

//merge CEVA 0517 remove  #define DEFAULT_MAX_NUM_LE_LINKS   PRH_MAX_DEVICES_SUPPORTED
//Note : current total stack : 1344 bytes
//charles modify 20131205 reduce memory size
#undef PRH_BS_CFG_SYS_GATT_DELIVER_EVENTS_VIA_HCI
#undef PRH_BS_CFG_SYS_GATT_ACCEPT_COMMANDS_VIA_HCI
#define PRH_BS_CFG_SYS_GATT_DELIVER_EVENTS_VIA_HCI 0
#define PRH_BS_CFG_SYS_GATT_ACCEPT_COMMANDS_VIA_HCI 0
#undef PRH_BS_CFG_SYS_LE_SMP_DELIVER_EVENTS_VIA_HCI
#undef PRH_BS_CFG_SYS_LE_SMP_ACCEPT_COMMANDS_VIA_HCI
#define PRH_BS_CFG_SYS_LE_SMP_DELIVER_EVENTS_VIA_HCI 0
#define PRH_BS_CFG_SYS_LE_SMP_ACCEPT_COMMANDS_VIA_HCI 0
#define IDLE_MODE  1
#define FORCE_SLEEP //every finish connection event force go to sleep 
//#define DELAY_SLEEP  //have data in Queue,delay go to cpu power down procedure
//#define GPIO_DEBUG  //mark mean no use GPIO debug
//#define BB_TEST_PIN_EN 1		/**< Enable test pin */  //frank 160617
//#define ANA_TEST_ON    
//charles add define
#define SLEEP_WITH_32K
#define UART_ENABLE
#define NEW_MMI_BUTTON
#define LPO_CLK_SOURCE_32000HZ
//#define LPO_CLK_SOURCE_32768HZ

//#define SEL_LPO_TYPE_XTAL
#define SEL_LPO_TYPE_16M_DIV

//#define CONNECT_INTERVAL_600MS
//#define CONNECT_INTERVAL_300MS
#define CONNECT_INTERVAL_290MS
//#define CONNECT_INTERVAL_200MS
//#define CONNECT_INTERVAL_100MS
//#define CONNECT_INTERVAL_50MS
//#define CONNECT_INTERVAL_180MS /*  revision 335 change */
#define MIN_AUX_VALULE    25
#define AUTO_ADVERTISING 1
//#define FIX_ROM_ADDRESS
#define ENABLE_WDT
#define VERSION_D_CHIP
#define DISABLE_LOW_POWER_CONNECT
//#define PER_TEST
//#define ATT_CNT_TEST
//#define AUX_CALIBRATE /* Enable no packet receive rate 5/932 = 0.5 %,Disable 35/887 = 3.9 % */
//#define JAL_BASE_ADDR 0x0F26000 

#define STORE_BTADDR_FLASH   1 //Kevin Add @ 20171220
#define ADD_CHANGE_BTNAME      //Kevin Add @ 20171220

#endif
