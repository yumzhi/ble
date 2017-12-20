/******************************************************************************
 * MODULE NAME:    hw_jalapeno.h
 * PROJECT CODE:   BlueStream
 * DESCRIPTION:    None
 * MAINTAINER:     Ivan Griffin
 * CREATION DATE:  7 July 1999
 *
 * SOURCE CONTROL: $Id: hw_jalapeno.h,v 1.1 2012/05/09 00:09:52 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 1999-2004 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *     7 July 1999	Jalapeno Revision 1.0, 30 June 1999
 *    30 July 1999      Jalapeno Revision 2.4, 14 July 1999
 *    07 Oct  1999      Jalapeno Revision (jal006v2.2)
 *    22 Nov  1999      Jalapeno Revision (jal006v2.41)
 *                       - partial update (registers up to 0x60)
 *
 ******************************************************************************/


#ifndef _PARTHUS_HW_JALAPENO_
#define _PARTHUS_HW_JALAPENO_

#include "sys_config.h"
#include "hw_jal_defs.h"                /* All jalapeno definitions     */
#include "hw_le_jal_defs.h"
/************************************************************************
*
* Define the constants and functions which are required to define a
* hardware independent interface to an RF device (from lmx3162).
*
*************************************************************************/
/*  Serial Configuration Register Parameters - Initial Configuration    */

#define RADIO_ADDR_LENGTH  0  /* Address field length when accessing reg*/
#define RADIO_PKT_LENGTH  20  /* Data field length when of accessing reg*/
#define RADIO_CLK_POL      0  /* Enable clock inversion on serial port  */
#define RADIO_DATA_POL     0  /* Enable data inversion on serial port   */
#define RADIO_NUM          0  /* Device number on serial port           */
#define RADIO_CLK_HIGH     1  /* High time for clock on serial port     */
#define RADIO_CLK_LOW      1  /* Low time for clock on serial port      */
#define RADIO_CLK_BYP      0  /* Clock bypass enable, no clock division */
#define RADIO_LE_SRC1      0  /* Load Enable Source for external radio  */
        		      /* 0 : Finite State Machine Serial Enable */
        		      /* 1 : RF_CTRL[0] Programmed Serial Enable*/
#define RADIO_LE_INV       1  /* Conditional Load Enable Inversion      */
        		      /* 0 : no inversion of ser_enb signal     */
        		      /* 1 : inversion of ser_enb signal        */

/*  Serial Configuration Register Parameters - Hopping Configuration    */

#define RADIO_LE_SRC2      1  /* Load Enable Source for external radio  */
        		      /* 0 : Finite State Machine Serial Enable */
        		      /* 1 : RF_CTRL[0] Programmed Serial Enable */

/****************************************************************
*
*  Define the structure used for accessing a Jalapeno device
*
****************************************************************/
#ifndef SER_DEFINED
  #define SER_DEFINED
  #define SER_NUM_SER_PORTS 4

typedef struct ser_port_struct {
  u_int32 addr_len;
  u_int32 pkt_len;
  u_int32 clk_pol;
  u_int32 data_pol;
  u_int32 clk_low;
  u_int32 clk_high;
  u_int32 clk_byp;
  u_int32 le_inv;
  u_int32 le_src;
} ser_port_type;

#endif

/*
 This structure is redundant.  Use the following to obtain tx/rx acl addresses:
       HW_get_tx_acl_buf_addr(), HW_get_rx_acl_buf_addr()

typedef struct jal_struct {
  volatile u_int8 *addr;
  volatile u_int8 *tx_acl_buf;
  volatile u_int8 *rx_acl_buf;
  ser_port_type ser_port[SER_NUM_SER_PORTS];
} jalapeno;
*/
/*
 * Declare function used for defining address space for jalapeno
 */
/*
void init_jal_board_adr(jalapeno *);
 */

/*
 * Types specific to Jalapeno device only
 */
typedef enum {RX_DISABLE=0, RX_NORMAL=1, RX_TWO_WINS=2, RX_FULL_WIN=3} t_rx_mode;


#endif
