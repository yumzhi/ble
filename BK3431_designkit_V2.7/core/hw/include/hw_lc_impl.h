#ifndef _PARTHUS_HW_LSLC_IMPL_
#define _PARTHUS_HW_LSLC_IMPL_

/*
 * MODULE NAME:    hw_lc_impl.h
 * PROJECT CODE:    BlueStream
 * DESCRIPTION:    Hardware Access Functions
 * MAINTAINER:     Ivan Griffin
 * DATE:           1 Jun 1999
 *
 * SOURCE CONTROL: $Id: hw_lc_impl.h,v 1.1 2012/05/09 00:09:52 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 1999-2004 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    1 June 1999 -   jn       - Initial Version V0.9
 *    14 July 1999 -   ig       - Moved macros to inline functions
 *
 */

#include "sys_config.h"
#include "hw_memcpy.h"
#include "hw_lc.h"    
#include "hw_delay.h"
#include "sys_features.h"

#if (PRAGMA_INLINE==1)
#pragma inline(\
HW_set_bd_addr,\
HW_set_bd_addr_via_uap_lap_nap,\
HW_set_uap_lap,\
HW_set_nap,\
HW_get_uap_lap,\
HW_get_nap,\
HW_get_bd_addr_Ex,\
HW_set_bt_clk,\
HW_set_bt_clk_offset,\
HW_get_bt_clk,\
HW_Get_Bt_Clk_Avoid_Race,\
HW_set_native_clk,\
HW_get_native_clk,\
HW_Get_Native_Clk_Avoid_Race,\
HW_set_sleep_status,\
HW_get_sleep_status,\
HW_set_use_lf,\
HW_get_use_lf,\
HW_set_slave,\
HW_get_slave,\
HW_set_pkd_intr_mask,\
HW_get_pkd_intr_mask,\
HW_set_pkd_rx_hdr_intr_mask,\
HW_get_pkd_rx_hdr_intr_mask,\
HW_set_pka_intr_mask,\
HW_get_pka_intr_mask,\
HW_set_no_pkt_rcvd_intr_mask,\
HW_get_no_pkt_rcvd_intr_mask,\
HW_set_sync_det_intr_mask,\
HW_get_sync_det_intr_mask,\
HW_set_tim_intr_mask,\
HW_get_tim_intr_mask,\
HW_set_aux_tim_intr_mask,\
HW_get_aux_tim_intr_mask,\
HW_set_pkd_intr_clr,\
HW_set_pkd_rx_hdr_intr_clr,\
HW_set_pka_intr_clr,\
HW_set_no_pkt_rcvd_intr_clr,\
HW_set_sync_det_intr_clr,\
HW_set_tim_intr_clr,\
HW_set_aux_tim_intr_clr,\
HW_get_no_pkt_rcvd_intr_clr,\
HW_get_sync_det_intr_clr,\
HW_get_pkd_intr,\
HW_get_pkd_rx_hdr_intr,\
HW_get_pka_intr,\
HW_get_no_pkt_rcvd_intr,\
HW_get_sync_det_intr,\
HW_get_tim_intr,\
HW_get_aux_tim_intr,\
HW_set_rx_mode,\
HW_get_rx_mode,\
HW_set_tx_mode,\
HW_get_tx_mode,\
HW_set_rx_buf,\
HW_get_rx_buf,\
HW_set_win_ext,\
HW_get_win_ext,\
HW_get_hec_err,\
HW_get_crc_err,\
HW_get_rx_hdr,\
HW_set_ser_cfg,\
HW_get_ser_cfg,\
HW_set_ser_data,\
HW_get_ser_data,\
HW_set_aux_timer,\
HW_get_aux_timer,\
HW_set_tx_delay,\
HW_get_tx_delay,\
HW_set_rx_delay,\
HW_get_rx_delay,\
HW_set_write_absolute_bt_clk,\
HW_set_delayed_bt_clk_update,\
HW_get_minor_revision,\
HW_get_major_revision,\
HWjal_Set_Rst_Code,\
HWjal_Get_Rst_Code
)



#if (BUILD_TYPE==UNIT_TEST_BUILD)
#pragma inline(HW_get_pkd_intr_clr,\
HW_get_pkd_rx_hdr_intr_clr,\
HW_get_pka_intr_clr,\
HW_get_tim_intr_clr,\
HW_get_aux_tim_intr_clr,\
HW_get_gio_intr_clr0,\
HW_get_gio_intr_clr1,\
HW_get_gio_intr_clr2,\
HW_get_gio_intr_clr3,\
HW_set_pkd_intr,\
HW_set_pkd_rx_hdr_intr,\
HW_set_pka_intr,\
HW_set_no_pkt_rcvd_intr,\
HW_set_sync_det_intr,\
HW_set_tim_intr,\
HW_set_aux_tim_intr,\
HW_set_gio_intr0,\
HW_set_gio_intr1,\
HW_set_gio_intr2,\
HW_set_gio_intr3,\
HW_set_tx0_over,\
HW_set_tx0_under,\
HW_set_tx1_over,\
HW_set_tx1_under,\
HW_set_tx2_over,\
HW_set_tx2_under,\
HW_set_rx_len,\
HW_set_rx_type,\
HW_set_rx_flow,\
HW_set_rx_arqn,\
HW_set_rx_seqn,\
HW_set_rx_am_addr,\
HW_set_rx_l_ch,\
HW_set_rx_p_flow,\
HW_set_rx_pkt,\
HW_set_hec_err,\
HW_set_crc_err,\
HW_set_rx_hdr,\
HW_set_rx0_over,\
HW_set_rx0_under,\
HW_set_rx1_over,\
HW_set_rx1_under,\
HW_set_rx2_over,\
HW_set_rx2_under,\
HW_set_rst_code,\
HW_set_err_cnt,\
HW_set_gio_status)
#endif /*#if (BUILD_TYPE==UNIT_TEST_BUILD)*/

#endif

#if (PRH_BS_DEV_USE_CACHED_BT_CLOCKS==1)
#include "hw_register.h"
#include "hw_jal_defs.h"
extern mHWreg_Create_Cache_Register(JAL_NATIVE_CLK);
extern mHWreg_Create_Cache_Register(JAL_BT_CLK_OFFSET);
#endif


/***********************************************************************
*
* This section define all the functions which write to and read
* the common control registers
*
************************************************************************/

/* Set Bluetooth Device Address */
__INLINE__ void HW_set_bd_addr(const t_bd_addr *p_bda)
{
#ifdef HW_DATA32
    /*
     * Write to hardware using only 32 bit words.
     */
    u_int32 bd_U32x2[2];
    BDADDR_Convert_to_U32x2(p_bda, bd_U32x2);
    mSetHWEntry64(JAL_BD_ADDR, bd_U32x2);
#else
    /*
     * Direct byte move
     */
    BDADDR_Get_Byte_Array_Ex(bda, (u_int8*)JAL_BD_ADDR_ADDR);
#endif

}

/*
 * Set BD_ADDR using uap_lap and nap fields (efficient).
 */
__INLINE__ void HW_set_bd_addr_via_uap_lap_nap(t_uap_lap uap_lap, t_nap nap)
{
    mSetHWEntry32(JAL_UAP_LAP, uap_lap);
    mSetHWEntry32(JAL_NAP, nap);
}

/*
 * Get/Set BD_ADDR using separate access to JAL_LAP_UAP JAL_NAP registers
 */
__INLINE__ void HW_set_uap_lap(t_uap_lap uap_lap)
                                        { mSetHWEntry32(JAL_UAP_LAP, uap_lap); }
__INLINE__ void HW_set_nap(t_nap nap)   { mSetHWEntry32(JAL_NAP, nap); }
__INLINE__ u_int32 HW_get_uap_lap(void) { return mGetHWEntry32(JAL_UAP_LAP); }
__INLINE__ u_int32 HW_get_nap(void)     { return mGetHWEntry32(JAL_NAP); }

/*
 * Get Bluetooth Device Address
 */
__INLINE__ void HW_get_bd_addr_Ex(t_bd_addr *p_bda)
{
#ifdef HW_DATA32
    /*
     * Write to hardware using only 32 bit words.
     */
    u_int32 bd_32[2];
    mGetHWEntry64_Ex(JAL_BD_ADDR, bd_32);
    BDADDR_Set_LAP_UAP_NAP(p_bda,
            bd_32[0]&0xFFFFFF, (t_uap) (bd_32[0]>>24), bd_32[1]);
#else
    /*
     * Direct byte read
     */
    BDADDR_Assign_from_Byte_Array(bda, (const u_int8*)JAL_BD_ADDR_ADDR);
#endif
}




/* Get Native Clock */
__INLINE__ t_clock HW_get_native_clk(void)
{
#if (PRH_BS_DEV_USE_CACHED_BT_CLOCKS==1)
    mHWreg_Load_Cache_Register(JAL_NATIVE_CLK);
    return mHWreg_Get_Cache_Register(JAL_NATIVE_CLK);
#else
    return mGetHWEntry32(JAL_NATIVE_CLK);
#endif
}

/* Get Bluetooth Clock */
__INLINE__ t_clock HW_get_bt_clk(void)
{
#if (BUILD_TYPE!=UNIT_TEST_BUILD)
#if (PRH_BS_DEV_USE_CACHED_BT_CLOCKS==1)
    mHWreg_Load_Cache_Register(JAL_BT_CLK_OFFSET);
    return mHWreg_Get_Cache_Register(JAL_BT_CLK_OFFSET);
#else
    return mGetHWEntry32(JAL_BT_CLK_OFFSET);
#endif
#else
    register t_clock return_code;

    if ( HW_get_slave() )
        return_code =  mGetHWEntry32(JAL_BT_CLK_OFFSET);
    else  /* If master then native clock == bt clock */
        return_code =  HW_get_native_clk();          

    return (return_code);

#endif /*(BUILD_TYPE!=UNIT_TEST_BUILD)*/
}

__INLINE__ t_clock HW_Get_Bt_Clk_Avoid_Race(void)
{
    t_clock piconet_clk;

#if (PRH_BS_DEV_USE_CACHED_BT_CLOCKS==1)
    /* only read from HW when cached value is invalidated as zero */
    piconet_clk = mHWreg_Get_Cache_Register(JAL_BT_CLK_OFFSET);
    if(0 == piconet_clk)
#endif
    {
        piconet_clk = HW_get_bt_clk();
    
        {
            if (HW_get_bt_clk() != piconet_clk)
            {
                piconet_clk = HW_get_bt_clk(); /* re-read, to ensure value has
                                                * settled and is sane! */
            }
        }
    }
    
    return piconet_clk;
}


/*
 * Set directly the BT clk offset.
 */
__INLINE__ void HW_set_bt_clk_offset(t_clock bt_clk_offset)
{
#if (BUILD_TYPE==UNIT_TEST_BUILD)
    /*
     * Emulate hardware by adding current clock value to offset
     */
    bt_clk_offset += (HW_get_add_bt_clk_relative()?HW_get_bt_clk():HW_get_native_clk()) & 0x0FFFFFFC;
    bt_clk_offset &= 0x0FFFFFFC;
#endif

    mSetHWEntry32(JAL_BT_CLK_OFFSET, bt_clk_offset);

#if (PRH_BS_DEV_USE_CACHED_BT_CLOCKS==1)
    /* invalidate cached piconet clock */
    mHWreg_Clear_Cache_Register(JAL_BT_CLK_OFFSET);
#endif
}

/* Set Native Clock */
__INLINE__ void HW_set_native_clk(const t_clock clkn)
{
#if (PRH_BS_DEV_USE_CACHED_BT_CLOCKS==1)
    mHWreg_Assign_Cache_Register(JAL_NATIVE_CLK,clkn);
    mHWreg_Store_Cache_Register(JAL_NATIVE_CLK);
    /* invalidate cached piconet clock */
    mHWreg_Clear_Cache_Register(JAL_BT_CLK_OFFSET);
#else
      mSetHWEntry32(JAL_NATIVE_CLK, clkn);
#endif
}
__INLINE__ u_int8 HW_Get_Native_Clk_Avoid_Race_last2bits(void)
{
    t_clock native_clk;
    native_clk = HW_Get_Native_Clk_Avoid_Race();
    return (native_clk&0x00000003);
}
__INLINE__ t_clock HW_Get_Native_Clk_Avoid_Race(void)
{
    t_clock native_clk;

#if (PRH_BS_DEV_USE_CACHED_BT_CLOCKS==1)
    /* only read from HW when cached value is invalidated as zero */
    native_clk = mHWreg_Get_Cache_Register(JAL_NATIVE_CLK);
    if(0 == native_clk)
#endif
    {
        native_clk = HW_get_native_clk();
    
        HWdelay_Wait_For_us(1);
        {
            if (HW_get_native_clk() != native_clk)
            {
                native_clk = HW_get_native_clk(); /* re-read, to ensure value has
                                                   * settled and is sane! */
            }
        }
    }

    return native_clk;
}


/* Set Intraslot Offset */
#if (PRH_BS_CFG_SYS_LE_DUAL_MODE==1)
__INLINE__ void HW_set_intraslot_offset(const u_int32 offset) {  mSetHWEntry32(JAL_INTRASLOT_OFFSET, offset); }
/* Set Slave Mode */
__INLINE__ void HW_set_slave(const u_int slave) { mSetHWEntry(JAL_SLAVE, slave); }
#else
__INLINE__ void HW_set_intraslot_offset(const u_int32 offset) { ; }
__INLINE__ void HW_set_slave(const u_int slave) { ; }
#endif
/* Set Use LF */
__INLINE__ void HW_set_use_lf(const u_int use_lf) { mSetHWEntry(JAL_USE_LF, use_lf); }
/* Get Use LF */
__INLINE__ u_int HW_get_use_lf(void) { return mGetHWEntry(JAL_USE_LF); }

/* Set Use LF */
__INLINE__ void HW_set_sleep_status(const u_int use_lf) { mSetHWEntry(JAL_SLEEP_STATUS, use_lf); }
/* Get Use LF */
__INLINE__ u_int HW_get_sleep_status(void) { return mGetHWEntry(JAL_SLEEP_STATUS); }

/* Set Test Radio bit */
__INLINE__ void HW_set_test_radio(const u_int test_radio) { mSetHWEntry(JAL_TEST_RADIO, test_radio); }
/* Get Test Radio bit */
__INLINE__ u_int HW_get_test_radio(void) { return mGetHWEntry( JAL_TEST_RADIO ); }

/* Set Timer interrupt mask */
__INLINE__ void HW_set_tim_intr_mask(const u_int value) { mSetHWEntry(JAL_TIM_INTR_MSK, value);
 }
/* Get Timer interrupt mask */
__INLINE__ u_int HW_get_tim_intr_mask(void) { return mGetHWEntry(JAL_TIM_INTR_MSK); }

/* Set Packet interrupt mask */
__INLINE__ void HW_set_pkd_intr_mask(const u_int value) { mSetHWEntry(JAL_PKD_INTR_MSK, value); }
/* Get Packet interrupt mask */
__INLINE__ u_int HW_get_pkd_intr_mask(void) { return mGetHWEntry(JAL_PKD_INTR_MSK); }

/* Set AUX Timer interrupt mask */
__INLINE__ void HW_set_aux_tim_intr_mask(const u_int value) { mSetHWEntry(JAL_AUX_TIM_INTR_MSK, value); }
/* Get AUX Timer interrupt mask */
__INLINE__ u_int HW_get_aux_tim_intr_mask(void) { return mGetHWEntry(JAL_AUX_TIM_INTR_MSK); }

/* Set PKA interrupt mask */
__INLINE__ void HW_set_pka_intr_mask(const u_int value) { mSetHWEntry(JAL_PKA_INTR_MSK, value); }
/* Get PKA Interrupt mask */
__INLINE__ u_int HW_get_pka_intr_mask(void) { return mGetHWEntry(JAL_PKA_INTR_MSK); }

__INLINE__ void  HW_set_pkd_rx_hdr_intr_mask(const u_int value) { mSetHWEntry(JAL_PKD_RX_HDR_INTR_MSK, value); }
__INLINE__ u_int HW_get_pkd_rx_hdr_intr_mask(void) { return mGetHWEntry(JAL_PKD_RX_HDR_INTR_MSK); }

__INLINE__ void HW_set_aes_intr_mask(const u_int value) { mSetHWEntry(JAL_AES_INTR_MSK, value); }
/* Clear Timer interrupt Clear */
__INLINE__ void HW_set_tim_intr_clr(const u_int value) { mSetHWEntry(JAL_TIM_INTR_CLR, value); }

/* Set Pkd Interrupt Clear */
__INLINE__ void HW_set_pkd_intr_clr(const u_int value) { mSetHWEntry(JAL_PKD_INTR_CLR, value); }

/* Clear AUX Timer interrupt Clear */
__INLINE__ void HW_set_aux_tim_intr_clr(const u_int value) { mSetHWEntry(JAL_AUX_TIM_INTR_CLR, value); }

/* Set Pka Interrupt Clear */
__INLINE__ void HW_set_pka_intr_clr(const u_int value) { mSetHWEntry(JAL_PKA_INTR_CLR, value); }

/* Set Pkd RX HDR Interrupt Clear */
__INLINE__ void HW_set_pkd_rx_hdr_intr_clr(const u_int value) { mSetHWEntry(JAL_PKD_RX_HDR_INTR_CLR, value); }

__INLINE__ void HW_set_aes_intr_clr(const u_int value) { mSetHWEntry(JAL_AES_INTR_CLR, value); }
/*
 * COMMON STATUS REGISTERS
 */
/* Get Timer Interrupt */
__INLINE__ u_int HW_get_tim_intr(void) { return mGetHWEntry(JAL_TIM_INTR); }

/* Get Pkd Interrupt */
__INLINE__ u_int HW_get_pkd_intr(void) { return mGetHWEntry(JAL_PKD_INTR); }

/* Get AUX Timer Interrupt */
__INLINE__ u_int HW_get_aux_tim_intr(void) { return mGetHWEntry(JAL_AUX_TIM_INTR); }

/* Get Pka Interrupt */
__INLINE__ u_int HW_get_pka_intr(void) { return mGetHWEntry(JAL_PKA_INTR); }

/* Get Pkd RX HDR Interrupt */
__INLINE__ u_int HW_get_pkd_rx_hdr_intr(void) { return mGetHWEntry(JAL_PKD_RX_HDR_INTR); }



/*
 * RECEIVE CONTROL REGISTERS
 */
/* Set RX Mode Bits */
__INLINE__ void HW_set_rx_mode(const u_int mode) { 
    mSetHWEntry(JAL_RX_MODE, mode); 
}
/* Get RX Mode Bits */
__INLINE__ u_int HW_get_rx_mode(void) { return mGetHWEntry(JAL_RX_MODE); }


/* Set TX Mode Bits */
//__INLINE__ void HW_set_tx_mode(const u_int mode)  { mSetHWEntry(JAL_TX_MODE, mode); }
__INLINE__ void HW_set_tx_mode(const u_int mode)  { ; }
/* Get TX Mode Bits */
//__INLINE__ u_int HW_get_tx_mode(void) { return mGetHWEntry(JAL_TX_MODE); }
/* Set Window Extension */
__INLINE__ void   HW_set_win_ext(const u_int ext) { mSetHWEntry(JAL_WIN_EXT, ext); }
/* Get Window Extension */
__INLINE__ u_int32 HW_get_win_ext(void) { return mGetHWEntry(JAL_WIN_EXT); }
/*
 * GIO CONTROL REGISTERS
 */


/*
 * SERIAL REGISTERS
 */
/* Set Serial configuration */
__INLINE__ void HW_set_ser_cfg(const u_int ser_cfg) { mSetHWEntry(JAL_SER_CFG, ser_cfg); }
/* Get Serial Configuration */
__INLINE__ u_int HW_get_ser_cfg(void) { return mGetHWEntry(JAL_SER_CFG); }

/*
 * AUXILLIARY TIMER REGISTERS
 */
/* Set Aux Timer */
__INLINE__ void HW_set_aux_timer(const u_int aux_timer) { mSetHWEntry(JAL_AUX_TIMER, aux_timer); }
/* Get Aux Timer */
__INLINE__ u_int HW_get_aux_timer(void) { return mGetHWEntry(JAL_AUX_TIMER); }


/*
 * TRANSMIT and RECEIVE DELAY REGISTERS
 */
/* Set TX Delay*/
__INLINE__ void HW_set_tx_delay(const u_int value) { mSetHWEntry(JAL_TX_DELAY, value); }
/* Get TX Delay */
__INLINE__ u_int HW_get_tx_delay(void) { return mGetHWEntry(JAL_TX_DELAY); }

/* Set RX Delay */
__INLINE__ void HW_set_rx_delay(const u_int value) { mSetHWEntry(JAL_RX_DELAY, value); }
/* Get RX Delay */
__INLINE__ u_int HW_get_rx_delay(void) { return mGetHWEntry(JAL_RX_DELAY); }

/*
 * BT CLOCK CONTROL REGISTERS
 */
__INLINE__ void HW_set_add_bt_clk_relative(const u_int value) { mSetHWEntry(JAL_ADD_BT_CLK_RELATIVE, value); }
__INLINE__ u_int HW_get_add_bt_clk_relative(void) { return mGetHWEntry(JAL_ADD_BT_CLK_RELATIVE); }
__INLINE__ void HW_set_write_absolute_bt_clk(const u_int value) { mSetHWEntry(JAL_WRITE_ABSOLUTE_BT_CLK, value); }

/*
 * REVISION CONTROL NUMBERS
 */
__INLINE__ u_int HW_get_minor_revision(void) { return mGetHWEntry(JAL_MINOR_REVISION); }
__INLINE__ u_int HW_get_major_revision(void) { return mGetHWEntry(JAL_MAJOR_REVISION); }

/*
 * RESET CONTROLS
 */
/* Set RST Code */
__INLINE__ void HWjal_Set_Rst_Code(const u_int rst_code) { mSetHWEntry(JAL_RST_CODE, rst_code); }


/*
 * AUXILLIARY FUNCTIONS
 */

__INLINE__ void HW_set_use_hab_crl1(const u_int value) { mSetHWEntry(JAL_USE_HAB_CTRL,value); }

__INLINE__ u_int HW_get_spi_now_conflict(void) { return mGetHWEntry(JAL_SPI_NOW_CONFLICT); }
__INLINE__ void HW_set_spi_now_conflict_clr(const u_int value) {mSetHWEntry(JAL_SPI_NOW_CONFLICT_CLR, value); }
__INLINE__ void HW_set_ser0_wr_clr(const u_int value) {mSetHWEntry(JAL_SER0_WR_CLR, value); }


__INLINE__ u_int HW_get_no_pkt_rcvd_intr_clr(void) { return mGetHWEntry(JAL_NO_PKT_RCVD_INTR_CLR); }

__INLINE__ u_int HW_get_no_pkt_rcvd_intr(void) { return mGetHWEntry(JAL_NO_PKT_RCVD_INTR); }
__INLINE__ void HW_set_no_pkt_rcvd_intr_clr(const u_int value) {mSetHWEntry(JAL_NO_PKT_RCVD_INTR_CLR, value); }

__INLINE__ void HW_set_no_pkt_rcvd_intr_mask(const u_int value) {mSetHWEntry(JAL_NO_PKT_RCVD_INTR_MSK, value); }
__INLINE__ u_int HW_get_no_pkt_rcvd_intr_mask(void) { return mGetHWEntry(JAL_NO_PKT_RCVD_INTR_MSK); }
 
__INLINE__ u_int HW_get_sync_det_intr_clr(void) { return mGetHWEntry(JAL_SYNC_DET_INTR_CLR); }

__INLINE__ u_int HW_get_sync_det_intr(void) { return mGetHWEntry(JAL_SYNC_DET_INTR); }
__INLINE__ void HW_set_sync_det_intr_clr(const u_int value) {mSetHWEntry(JAL_SYNC_DET_INTR_CLR, value); }

__INLINE__ void HW_set_sync_det_intr_mask(const u_int value) {mSetHWEntry(JAL_SYNC_DET_INTR_MSK, value); }
__INLINE__ u_int HW_get_sync_det_intr_mask(void) { return mGetHWEntry(JAL_SYNC_DET_INTR_MSK); }

__INLINE__ u_int HW_get_native_bit_count(void) { return mGetHWEntry(JAL_NAT_BIT_CNT); }


__INLINE__ void  HW_set_ser_time_early(const u_int value)  {mSetHWEntry(JAL_SER_TIME_EARLY,value); }
__INLINE__ void  HW_set_ser_time_late(const u_int value)  {mSetHWEntry(JAL_SER_TIME_LATE,value); }

#endif
