#ifndef _PARTHUS_HW_LE_LC_
#define _PARTHUS_HW_LE_LC_

#include "sys_types.h"
#include "sys_config.h"
#include "sys_features.h"
#include "hw_macro_defs.h"

#include "hw_jalapeno.h"                   /*Dedicated to hardware platform */

void HWlelc_Initialise(void);

/****************************************************************
*
*  Declare the functions used in accessing the Jalapeno
*  common control registers
*
****************************************************************/

__INLINE__ void HWle_set_le_mode(void);
__INLINE__ void HWle_clear_le_mode(void);//merge CEVA 0517

__INLINE__ u_int HWle_get_le_mode(void);

#if 0
__INLINE__ void HWle_set_slave_mode(const u_int);
__INLINE__ u_int HWle_get_slave_mode(void);
#endif

__INLINE__ void HWle_set_master_mode(void);
__INLINE__ void HWle_clear_master_mode(void);//merge CEVA 0517

__INLINE__ u_int HWle_get_master_mode(void);

__INLINE__ void HWle_set_adv_state(void);
__INLINE__ void HWle_clear_adv_state(void);//merge CEVA 0517

__INLINE__ u_int HWle_get_adv_state(void);

__INLINE__ void HWle_set_tx_enable(void);
__INLINE__ void HWle_clear_tx_enable(void);//merge CEVA 0517

__INLINE__ u_int HWle_get_tx_enable(void);

__INLINE__ void HWle_set_scan_state(const u_int);
__INLINE__ u_int HWle_get_scan_state(void);

__INLINE__ void HWle_set_tifs_default(void);
__INLINE__ void HWle_clear_tifs_default(void);//merge CEVA 0517
__INLINE__ u_int HWle_get_tifs_default(void);

__INLINE__ void HWle_set_tifs_abort(void);
__INLINE__ void HWle_clear_tifs_abort(void);//merge CEVA 0517

__INLINE__ u_int HWle_get_tifs_abort(void);


__INLINE__ void HWle_set_tifs_delay(const u_int);
__INLINE__ u_int HWle_get_tifs_delay(void);


__INLINE__ void HWle_set_tifs_default(void);
__INLINE__ void HWle_clear_tifs_default(void);//merge CEVA 0517
__INLINE__ u_int HWle_get_tifs_default(void);

__INLINE__ void HW_set_pd_addr(const t_bd_addr *);
__INLINE__ void HW_set_rd_addr(const t_bd_addr *);


__INLINE__ void HWle_set_acc_addr(const u_int);
__INLINE__ u_int HWle_get_acc_addr(void);



__INLINE__ void HWle_set_le_preamble(const u_int);
__INLINE__ u_int HWle_get_le_preamble(void);

__INLINE__ void HWle_set_crc_init(const u_int);
__INLINE__ u_int HWle_get_crc_init(void);

// Tranmist Control Functions

__INLINE__ void HWle_set_tx_pdu_type(const u_int);
__INLINE__ u_int HWle_get_tx_pdu_type(void);

__INLINE__ void HWle_set_tx_rfu1_adv(const u_int);
__INLINE__ u_int HWle_get_tx_rfu1_adv(void);

__INLINE__ void HWle_set_tx_tx_add(const u_int);
__INLINE__ u_int HWle_get_tx_tx_add(void);

__INLINE__ void HWle_set_tx_rx_add(const u_int);
__INLINE__ u_int HWle_get_tx_rx_add(void);

__INLINE__ void HWle_set_tx_length_adv(const u_int);
__INLINE__ u_int HWle_get_tx_length_adv(void);

__INLINE__ void HWle_set_tx_rfu2_adv(const u_int);
__INLINE__ u_int HWle_get_tx_rfu2_adv(void);

__INLINE__ void HWle_set_tx_nesn(const u_int);
__INLINE__ u_int HWle_get_tx_nesn(void);

__INLINE__ void HWle_set_tx_sn(const u_int);
__INLINE__ u_int HWle_get_tx_sn(void);

__INLINE__ void HWle_set_tx_md(void);
__INLINE__ void HWle_clear_tx_md(void);//merge CEVA 0517

__INLINE__ u_int HWle_get_tx_md(void);

__INLINE__ void HWle_set_tx_length_data(const u_int);
__INLINE__ u_int HWle_get_tx_length_data(void);


// Revice Control Functions

__INLINE__ void HWle_set_rx_le_preamble(const u_int);
__INLINE__ u_int HWle_get_rx_le_preamble(void);

__INLINE__ void HWle_set_rx_crc_init(const u_int);
__INLINE__ u_int HWle_get_rx_crc_init(void);

__INLINE__ void HWle_set_rx_pdu_type(const u_int);
__INLINE__ u_int HWle_get_rx_pdu_type(void);

__INLINE__ void HWle_set_rx_rfu1_adv(const u_int);
__INLINE__ u_int HWle_get_rx_rfu1_adv(void);

__INLINE__ void HWle_set_rx_tx_add(const u_int);
__INLINE__ u_int HWle_get_rx_tx_add(void);

__INLINE__ void HWle_set_rx_rx_add(const u_int);
__INLINE__ u_int HWle_get_rx_rx_add(void);

__INLINE__ void HWle_set_tx_length_adv(const u_int);
__INLINE__ u_int HWle_get_tx_length_adv(void);

__INLINE__ void HWle_set_rx_rfu2_adv(const u_int);
__INLINE__ u_int HWle_get_rx_rfu2_adv(void);

__INLINE__ void HWle_set_rx_nesn(const u_int);
__INLINE__ u_int HWle_get_rx_nesn(void);

__INLINE__ void HWle_set_rx_sn(const u_int);
__INLINE__ u_int HWle_get_rx_sn(void);

__INLINE__ void HWle_set_rx_md(const u_int);
__INLINE__ u_int HWle_get_rx_md(void);

__INLINE__ void HWle_set_tx_length_data(const u_int);
__INLINE__ u_int HWle_get_tx_length_data(void);


__INLINE__ void HWle_set_tx_llid(const u_int llid);
__INLINE__ u_int HWle_get_tx_llid(void);

//__INLINE__ void HWle_set_tx_llid(const u_int llid);
__INLINE__ u_int HWle_get_rx_llid(void);


__INLINE__ void HWle_set_auxle1_timer(const u_int16);
__INLINE__ u_int16 HWle_get_auxle1_timer(void);

__INLINE__ void HWle_set_auxle2_timer(const u_int16);
__INLINE__ u_int16 HWle_get_auxle2_timer(void);

__INLINE__ void HWle_set_rx_mic(const u_int);
__INLINE__ u_int HWle_get_rx_mic(void);

__INLINE__ void HWle_set_tx_mic(const u_int);
__INLINE__ u_int HWle_get_tx_mic(void);

__INLINE__ void HWle_set_rx_mic_status(const u_int);
__INLINE__ u_int HWle_get_rx_mic_status(void);

__INLINE__ void HWle_set_delay_search_win(const u_int );
__INLINE__ u_int HWle_get_delay_search_win(void);

__INLINE__ void HWle_set_whitening_enable(void);
__INLINE__ void HWle_clear_whitening_enable(void);//merge CEVA 0517
__INLINE__ u_int HWle_get_whitening_enable(void);

__INLINE__ void HWle_set_whitening_init(const u_int );
__INLINE__ u_int HWle_get_whitening_init(void);

__INLINE__ u_int HWle_get_crc_err(void);

__INLINE__ void HWle_set_le_spi_only(void);
__INLINE__ void HWle_clear_le_spi_only(void);//merge CEVA 0517
__INLINE__ u_int HWle_get_le_spi_only(void);
__INLINE__ void HWle_set_win_offset_interval_in_tx_buffer(u_int16 offset,u_int16 interval);
__INLINE__ void HWle_set_advertiser_address_in_tx_buffer(const u_int8* pAddr);

__INLINE__ void HWle_set_advertiser_addr1_in_tx_buffer(u_int32 val);
__INLINE__ void HWle_set_advertiser_addr2_in_tx_buffer(u_int32 val);
#if (PRH_BS_CFG_SYS_LE_DUAL_MODE == 1)
#define HWle_get_tx_acl_buf_addr()  0x80006200 // JAL_LE_TX_ACL_BASE_ADDR
#define HWle_get_rx_acl_buf_addr()  0x80006400 // JAL_LE_RX_ACL_BASE_ADDR              (0x00000400 + JAL_LE_BASE_ADDR)
#else // SINGLE_MODE
#define HWle_get_tx_acl_buf_addr()  (0x008016A0 + 0x0)//BK3431 addr
#define HWle_get_rx_acl_buf_addr()  (0x008016D0 + 0x0)//BK3431 addr
#endif

#define HWle_get_aes_key_addr()     JAL_LE_AES_KEY_ADDR
#define HWle_get_aes_iv_addr()      JAL_LE_AES_IV_ADDR
#define HEle_get_aes_pkt_counter_addr() JAL_LE_AES_PKT_COUNTER_ADDR
#define HWle_get_aes_data_addr()    JAL_LE_AES_BUFFER_ADDR
#ifdef __USE_INLINES__
#include "hw_le_lc_impl.h"
#endif

#endif
