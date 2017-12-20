#ifndef _PARTHUS_HW_LE_LC_IMPL_
#define _PARTHUS_HW_LE_LC_IMPL_


/*
 * MODULE NAME:    hw_le_lc_impl.h
 * PROJECT CODE:    BlueStream
 * DESCRIPTION:    LE Hardware Access Functions
 * MAINTAINER:     Gary Fleming
 * DATE:           July 2011
 *
 * SOURCE CONTROL: $Id: hw_le_lc_impl.h,v 1.1 2012/05/09 00:09:53 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 1999-2004 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 */

#include "sys_types.h"
#include "sys_config.h"
#include "hw_memcpy.h"
#include "hw_le_lc.h"    
#include "hw_delay.h"
#include "hw_register.h"
#include "hw_macro_defs.h"
#include "BKAPI.h"
void LE_delay_time(volatile int time);
//#include "hw_le_jal_defs.h"

#if (PRAGMA_INLINE==1)
#pragma inline(\
HWle_set_le_mode,\
HWle_clear_le_mode, \//merge CEVA 0517
HWle_get_le_mode,\
HWle_set_address_type,\
HWle_get_address_type,\
HWle_set_slave_mode,\
HWle_get_slave_mode,\
HWle_set_adv_state,\
HWle_clear_adv_state,\//merge CEVA 0517
HWle_get_adv_state,\
HWle_set_tx_rx_enable,\
HWle_get_tx_rx_enable,\
HWle_set_scan_state,\
HWle_get_scan_state,\
HWle_set_tifs_default,\
HWle_clear_tifs_default,\//merge CEVA 0517
HWle_get_tifs_default,\
HWle_set_tifs_abort,\
HWle_clear_tifs_abort,\//merge CEVA 0517
HWle_get_tifs_abort,\
HWle_set_tifs_delay,\
HWle_get_tifs_delay,\
HWle_set_tifs_default,\
HWle_clear_tifs_default,\//merge CEVA 0517
HWle_get_tifs_default,\
HWle_set_pd_addr,\
HWle_set_rd_addr,\
HWle_set_acc_addr,\
HWle_get_acc_addr,\
HWle_set_le_preamble,\
HWle_get_le_preamble,\
HWle_set_crc_init,\
HWle_get_crc_init,\

HWle_set_tx_pdu_type,\
HWle_get_tx_pdu_type,\
HWle_set_tx_rfu1_adv,\
HWle_get_tx_rfu1_adv,\
HWle_set_tx_tx_add,\
HWle_get_tx_tx_add,\
HWle_set_tx_rx_add,\
HWle_get_tx_rx_add,\
HWle_set_tx_tx_length_adv,\
HWle_get_tx_tx_length_adv,\
HWle_set_tx_rfu1_adv,\
HWle_get_tx_rfu1_adv,\
HWle_set_tx_nesn,\
HWle_get_tx_nesn,\
HWle_set_tx_sn,\
HWle_get_tx_sn,\
HWle_set_tx_md,\
HWle_clear_tx_md,\//merge CEVA 0517
HWle_get_tx_md,\
HWle_set_tx_length_data,\
HWle_get_tx_length_data,\
//XX
HWle_set_rx_pdu_type,\
HWle_get_rx_pdu_type,\
HWle_set_rx_rfu1_adv,\
HWle_get_rx_rfu1_adv,\
HWle_set_rx_tx_add,\
HWle_get_rx_tx_add,\
HWle_set_rx_rx_add,\
HWle_get_rx_rx_add,\
HWle_set_rx_tx_length_adv,\
HWle_get_rx_tx_length_adv,\
HWle_set_rx_rfu1_adv,\
HWle_get_rx_rfu1_adv,\
HWle_set_rx_nesn,\
HWle_get_rx_nesn,\
HWle_set_rx_sn,\
HWle_get_rx_sn,\
HWle_set_rx_md,\
HWle_get_rx_md,\
HWle_set_rx_length_data,\
HWle_get_rx_length_data,\

HWle_set_auxle1_timer,\
HWle_get_auxle1_timer,\
HWle_set_auxle2_timer,\
HWle_get_auxle2_timer,\
HWle_set_rx_mic_status,\
HWle_get_rx_mic_status,\
HWle_set_whitening_enable,\
HWle_clear_whitening_enable,\//merge CEVA 0517
HWle_get_whitening_enable,\
HWle_set_delay_search_win,\
HWle_get_delay_search_win,\
HWle_set_whitening_init,\
HWle_get_whitening_init);
#endif

__INLINE__ void HWle_set_le_mode()              { mSetHWBit(JAL_LE_MODE); }//merge CEVA 0517
__INLINE__ void HWle_clear_le_mode()              { mClearHWBit(JAL_LE_MODE); }//merge CEVA 0517

__INLINE__ u_int HWle_get_le_mode(void)                            {return mGetHWEntry(JAL_LE_MODE);  }

__INLINE__ void HWle_set_address_type(const u_int add_type)        { mSetHWEntry(JAL_LE_ADDR_TYPE,add_type); }
__INLINE__ u_int HWle_get_address_type(void)                       { return mGetHWEntry(JAL_LE_ADDR_TYPE);  }

#if 0
__INLINE__ void HWle_set_slave_mode(const u_int slave_mode)        {mSetHWEntry(JAL_LE_SLAVE_MODE,slave_mode);}
__INLINE__ u_int HWle_get_slave_mode(void)                         {return mGetHWEntry(JAL_LE_SLAVE_MODE);   }
#endif

__INLINE__ void HWle_set_master_mode()        {mSetHWBit(JAL_LE_MASTER_MODE);}//merge CEVA 0517
__INLINE__ void HWle_clear_master_mode()        {mClearHWBit(JAL_LE_MASTER_MODE);}//merge CEVA 0517


__INLINE__ u_int HWle_get_master_mode(void)                         {return mGetHWEntry(JAL_LE_MASTER_MODE);   }


__INLINE__ void HWle_set_adv_state()          {mSetHWBit(JAL_LE_ADV_STATE);} //merge CEVA 0517                   
__INLINE__ void HWle_clear_adv_state()          {mClearHWBit(JAL_LE_ADV_STATE);}   //merge CEVA 0517                 


__INLINE__ u_int HWle_get_adv_state(void)                          {return mGetHWEntry(JAL_LE_ADV_STATE);  }

__INLINE__ void HWle_set_tx_enable()       {mSetHWBit(JAL_LE_TX_ENABLE);}//merge CEVA 0517
__INLINE__ void HWle_clear_tx_enable()       {mClearHWBit(JAL_LE_TX_ENABLE);}//merge CEVA 0517

__INLINE__ u_int HWle_get_tx_enable(void)                          {return mGetHWEntry(JAL_LE_TX_ENABLE);  }

__INLINE__ void HWle_set_scan_state(const u_int scan_state)        { return; }
__INLINE__ u_int HWle_get_scan_state(void)                         { return 0;}

__INLINE__ void HWle_set_tifs_default()    {mSetHWBit(JAL_LE_TIFS_DEFAULT);} //merge CEVA 0517                   
__INLINE__ void HWle_clear_tifs_default()    {mClearHWBit(JAL_LE_TIFS_DEFAULT);} //merge CEVA 0517                   

__INLINE__ u_int HWle_get_tifs_default(void)                       {return mGetHWEntry(JAL_LE_TIFS_DEFAULT);  }

__INLINE__ void HWle_set_tifs_abort()        {mSetHWBit(JAL_LE_TIFS_ABORT);} //merge CEVA 0517                   
__INLINE__ void HWle_clear_tifs_abort()        {mClearHWBit(JAL_LE_TIFS_ABORT);}  //merge CEVA 0517                  

__INLINE__ u_int HWle_get_tifs_abort(void)                         {return mGetHWEntry(JAL_LE_TIFS_ABORT);  }

__INLINE__ void HWle_abort_tifs_count(void)        {    mSetHWEntry(JAL_LE_TIFS_ABORT,1);
                                                        LE_delay_time(1);
														mSetHWEntry(JAL_LE_TIFS_ABORT,0);}

__INLINE__ void HWle_set_tifs_delay(const u_int tifs_delay)        {mSetHWEntry(JAL_LE_TIFS_DELAY,tifs_delay);}                    
__INLINE__ u_int HWle_get_tifs_delay(void)                         {return mGetHWEntry(JAL_LE_TIFS_DELAY);  }


__INLINE__ void HWle_set_le_spi_only()        {mSetHWBit(JAL_LE_SPI_LE_ONLY);}//merge CEVA 0517
__INLINE__ void HWle_clear_le_spi_only()      {mClearHWBit(JAL_LE_SPI_LE_ONLY);}//merge CEVA 0517

__INLINE__ u_int HWle_get_le_spi_only(void)                        {return mGetHWEntry(JAL_LE_SPI_LE_ONLY);  }



__INLINE__ void HW_set_pd_addr(const t_bd_addr *p_bda)
{
    /*
     * Write to hardware using only 32 bit words.
     */
    u_int32 bd_U32x2[2];
    BDADDR_Convert_to_U32x2(p_bda, bd_U32x2);
    mSetHWEntry64(JAL_LE_PD, bd_U32x2);

}


__INLINE__ void HW_set_rd_addr(const t_bd_addr *p_bda)
{
    u_int32 bd_U32x2[2];
    BDADDR_Convert_to_U32x2(p_bda, bd_U32x2);
    mSetHWEntry64(JAL_LE_RD, bd_U32x2);
}

#if 0 // debug only
__INLINE__ u_int16 HWle_Read_Rx_Header(void)
{
	return (mGetHWEntry(JAL_LE_RX_HEADER) & 0x0000FFFF);
}
#endif
__INLINE__ void HWle_set_acc_addr(const u_int acc_addr)                    {mSetHWEntry(JAL_LE_ACC_ADDR,acc_addr);} 
__INLINE__ u_int HWle_get_acc_addr(void)                                   {return mGetHWEntry(JAL_LE_ACC_ADDR);  }

__INLINE__ void HWle_set_crc_init(const u_int crc_init)                    {mSetHWEntry(JAL_LE_CRC_INIT,crc_init);} 
__INLINE__ u_int HWle_get_crc_init(void)                                   {return mGetHWEntry(JAL_LE_CRC_INIT);  }

#if 1 // TO DO -- TODO Not Complete - for Single Mode
// Test Mode Transmit Header
__INLINE__ void HWle_set_tx_test_pdu_rfu1(const u_int rfu1 )                  {mSetHWEntry(JAL_LE_TX_TEST_PDU_RFU1,rfu1);    }
__INLINE__ u_int HWle_get_tx_test_pdu_rfu1(void)                              {return mGetHWEntry(JAL_LE_TX_TEST_PDU_RFU1);        }

__INLINE__ void HWle_set_tx_test_length(const u_int length)					  {mSetHWEntry(JAL_LE_TX_TEST_PDU_LEN,length);    } 
__INLINE__ u_int HWle_get_tx_test_length(void)								  {return mGetHWEntry(JAL_LE_TX_TEST_PDU_LEN);        }

__INLINE__ void HWle_set_tx_test_pdu_rfu2(const u_int rfu2 )                  {mSetHWEntry(JAL_LE_TX_TEST_PDU_RFU2,rfu2);    }
__INLINE__ u_int HWle_get_tx_test_pdu_rfu2(void)                              {return mGetHWEntry(JAL_LE_TX_TEST_PDU_RFU2);        }

__INLINE__ void HWle_set_tx_test_pdu_type(const u_int pdu_type)               {mSetHWEntry(JAL_LE_TX_TEST_PDU_TYPE,pdu_type);}
__INLINE__ u_int HWle_get_tx_test_pdu_type(void)                              {return mGetHWEntry(JAL_LE_TX_TEST_PDU_TYPE);  }

#else

__INLINE__ void HWle_set_tx_test_pdu_rfu1(const u_int rfu1 )                  {;    }
__INLINE__ u_int HWle_get_tx_test_pdu_rfu1(void)                              {return 0;        }

__INLINE__ void HWle_set_tx_test_length(const u_int length)					  {;    }
__INLINE__ u_int HWle_get_tx_test_length(void)								  {return 0;        }

__INLINE__ void HWle_set_tx_test_pdu_rfu2(const u_int rfu2 )                  {;    }
__INLINE__ u_int HWle_get_tx_test_pdu_rfu2(void)                              {return 0;        }

__INLINE__ void HWle_set_tx_test_pdu_type(const u_int pdu_type)               {;}
__INLINE__ u_int HWle_get_tx_test_pdu_type(void)                              {return 0;  }



#endif
// TransMit Control Registers
__INLINE__ void HWle_set_tx_pdu_type(const u_int pdu_type)                    {mSetHWEntry(JAL_LE_TX_PDU_TYPE,pdu_type);}
__INLINE__ u_int HWle_get_tx_pdu_type(void)                                   {return mGetHWEntry(JAL_LE_TX_PDU_TYPE);  }

__INLINE__ void HWle_set_tx_rfu1_adv(const u_int rfu1 )                       {mSetHWEntry(JAL_LE_TX_RFU1_ADV,rfu1);    }
__INLINE__ u_int HWle_get_tx_rfu1_adv(void)                                   {return mGetHWEntry(JAL_LE_TX_RFU1_ADV);        }

__INLINE__ void HWle_set_tx_tx_add(const u_int tx_add)                        {mSetHWEntry(JAL_LE_TX_TX_ADD,tx_add);    }
__INLINE__ u_int HWle_get_tx_tx_add(void)                                     {return mGetHWEntry(JAL_LE_TX_TX_ADD);    }

__INLINE__ void HWle_set_tx_rx_add(const u_int rx_add)                        {mSetHWEntry(JAL_LE_TX_RX_ADD,rx_add);    }
__INLINE__ u_int HWle_get_tx_rx_add(void)                                     {return mGetHWEntry(JAL_LE_TX_RX_ADD);    }

__INLINE__ void HWle_set_tx_length_adv(const u_int length_adv)             {mSetHWEntry(JAL_LE_TX_LENGTH_ADV,length_adv);    } 
__INLINE__ u_int HWle_get_tx_length_adv(void)                              {return mGetHWEntry(JAL_LE_TX_LENGTH_ADV);        }

__INLINE__ void HWle_set_tx_rfu2_adv(const u_int rfu2 )                       {mSetHWEntry(JAL_LE_TX_RFU2_ADV,rfu2);    }
__INLINE__ u_int HWle_get_tx_rfu2_adv(void)                                   {return mGetHWEntry(JAL_LE_TX_RFU2_ADV);        }

__INLINE__ void HWle_set_tx_adv_header(u_int8 type, u_int8 tx_add, u_int8 rx_add, u_int8 length)
{
	mSetHWEntry(JAL_LE_TX_ADV_HEADER,(type + (tx_add << JAL_LE_TX_TX_ADD_SHFT) + (rx_add << JAL_LE_TX_RX_ADD_SHFT) + (length << JAL_LE_TX_LENGTH_ADV_SHFT)));
}

__INLINE__ void HWle_set_win_size_in_tx_buffer(u_int8 win_size)              {mSetHWEntry(JAL_LE_TX_BUFF_WIN_SIZE,win_size); }
__INLINE__ void HWle_set_win_offset_in_tx_buffer(u_int8 win_offset)          {mSetHWEntry(JAL_LE_TX_BUFF_WIN_OFFSET,win_offset); }

__INLINE__ void HWle_set_win_offset_interval_in_tx_buffer(u_int16 offset,u_int16 interval)
{
	*((u_int32 volatile*)JAL_LE_TX_BUFF_WIN_OFFSET_ADDR) = (u_int32)offset + (u_int32)(interval << 16);
}
__INLINE__ void HWle_set_advertiser_addr1_in_tx_buffer(u_int32 val)
{
	*((u_int32 volatile*)JAL_LE_TX_BUF_ADDR1_ADDR ) = val;
}

__INLINE__ void HWle_set_advertiser_addr2_in_tx_buffer(u_int32 val)
{
	*((u_int32 volatile*)JAL_LE_TX_BUF_ADDR2_ADDR ) = val;
}

__INLINE__ void HWle_set_advertiser_address_in_tx_buffer(const u_int8* p_pdu)
{
	u_int32 curr_contents = *((u_int32 volatile*)(JAL_LE_TX_BUF_ADDR1_ADDR)) & 0x0000FFFF;

	//curr_contents = 0x00001f1f;
	*((u_int32 volatile*)JAL_LE_TX_BUF_ADDR1_ADDR ) = curr_contents  +
			((u_int32)((*(p_pdu))<<16) +  (u_int32)((*(p_pdu+1)) << 24)) ;

}
__INLINE__ u_int HWle_get_tx_adv_header(void)                                 {return mGetHWEntry(JAL_LE_TX_ADV_HEADER); }
__INLINE__ u_int HWle_get_tx_data_header(void)                                 {return mGetHWEntry(JAL_LE_TX_DATA_HEADER); }

__INLINE__ u_int HWle_get_rx_adv_header(void)                                 {return mGetHWEntry(JAL_LE_RX_HEADER); }
__INLINE__ u_int HWle_get_rx_data_header(void)                                 {return mGetHWEntry(JAL_LE_RX_HEADER); }


__INLINE__ void HWle_set_tx_llid(const u_int llid)                            {mSetHWEntry(JAL_LE_TX_LLID,llid);    }
__INLINE__ u_int HWle_get_tx_llid(void)                                       {return mGetHWEntry(JAL_LE_TX_LLID);  }


__INLINE__ void HWle_set_tx_nesn(const u_int nesn)                            {mSetHWEntry(JAL_LE_TX_NESN,nesn);    }
__INLINE__ u_int HWle_get_tx_nesn(void)                                       {return mGetHWEntry(JAL_LE_TX_NESN);  }

__INLINE__ void HWle_set_tx_sn(const u_int sn)                                {mSetHWEntry(JAL_LE_TX_SN,sn);        }
__INLINE__ u_int HWle_get_tx_sn(void)                                         {return mGetHWEntry(JAL_LE_TX_SN);    }

__INLINE__ void HWle_set_tx_md()                                {mSetHWBit(JAL_LE_TX_MD);                              }//merge CEVA 0517
__INLINE__ void HWle_clear_tx_md( )                                {mClearHWBit(JAL_LE_TX_MD);                              }//merge CEVA 0517

__INLINE__ u_int HWle_get_tx_md(void)                                         {return mGetHWEntry(JAL_LE_TX_MD);                          }

__INLINE__ void HWle_set_tx_length_data(const u_int tx_length_data)        {mSetHWEntry(JAL_LE_TX_LENGTH_DATA,tx_length_data);      } 
__INLINE__ u_int HWle_get_tx_length_data(void)                             {return mGetHWEntry(JAL_LE_TX_LENGTH_DATA);              }

// REceive Control Registers

__INLINE__ void HWle_set_rx_pdu_type(const u_int pdu_type)                    {mSetHWEntry(JAL_LE_RX_PDU_TYPE,pdu_type);}
__INLINE__ u_int HWle_get_rx_pdu_type(void)                                   {return mGetHWEntry(JAL_LE_RX_PDU_TYPE);  }

__INLINE__ void HWle_set_rx_rfu1_adv(const u_int rfu1 )                       {mSetHWEntry(JAL_LE_RX_RFU1_ADV,rfu1);    }
__INLINE__ u_int HWle_get_rx_rfu1_adv(void)                                   {return mGetHWEntry(JAL_LE_RX_RFU1_ADV);        }

__INLINE__ void HWle_set_rx_tx_add(const u_int tx_add)                        {mSetHWEntry(JAL_LE_RX_TX_ADD,tx_add);    }
__INLINE__ u_int HWle_get_rx_tx_add(void)                                     {return mGetHWEntry(JAL_LE_RX_TX_ADD);    }

__INLINE__ void HWle_set_rx_rx_add(const u_int rx_add)                        {mSetHWEntry(JAL_LE_RX_RX_ADD,rx_add);    }
__INLINE__ u_int HWle_get_rx_rx_add(void)                                     {return mGetHWEntry(JAL_LE_RX_RX_ADD);    }

__INLINE__ void HWle_set_rx_length_adv(const u_int length_adv)             {mSetHWEntry(JAL_LE_RX_LENGTH_ADV,length_adv);    }
__INLINE__ u_int HWle_get_rx_length_adv(void)                              {return mGetHWEntry(JAL_LE_RX_LENGTH_ADV);        }

__INLINE__ void HWle_set_rx_rfu2_adv(const u_int rfu2 )                       {mSetHWEntry(JAL_LE_RX_RFU2_ADV,rfu2);    }
__INLINE__ u_int HWle_get_rx_rfu2_adv(void)                                   {return mGetHWEntry(JAL_LE_RX_RFU2_ADV);        }

//__INLINE__ void HWle_set_tx_llid(const u_int llid)                            {mSetHWEntry(JAL_LE_TX_LLID,llid);    }
__INLINE__ u_int HWle_get_rx_llid(void)                                       {return mGetHWEntry(JAL_LE_RX_LLID);  }

__INLINE__ void HWle_set_rx_nesn(const u_int nesn)                            {mSetHWEntry(JAL_LE_RX_NESN,nesn);    }
__INLINE__ u_int HWle_get_rx_nesn(void)                                       {return mGetHWEntry(JAL_LE_RX_NESN);  }

__INLINE__ void HWle_set_rx_sn(const u_int sn)                                {mSetHWEntry(JAL_LE_RX_SN,sn);        }
__INLINE__ u_int HWle_get_rx_sn(void)                                         {return mGetHWEntry(JAL_LE_RX_SN);    }

__INLINE__ void HWle_set_rx_md(const u_int md)                                {mSetHWEntry(JAL_LE_RX_MD,md);                              }
__INLINE__ u_int HWle_get_rx_md(void)                                         {return mGetHWEntry(JAL_LE_RX_MD);                          }

__INLINE__ void HWle_set_rx_length_data(const u_int tx_length_data)        {mSetHWEntry(JAL_LE_RX_LENGTH_DATA,tx_length_data);      }
__INLINE__ u_int HWle_get_rx_length_data(void)                             {return mGetHWEntry(JAL_LE_RX_LENGTH_DATA);              }

__INLINE__ u_int HWle_get_crc_err(void)                                  {return mGetHWEntry(JAL_LE_CRC_ERR);              }


//
__INLINE__ void HWle_set_auxle1_timer(const u_int16 auxle1_timer)          {mSetHWEntry(JAL_LE_AUXLE1_TIMER,auxle1_timer);          } 
__INLINE__ u_int16 HWle_get_auxle1_timer(void)                             {return mGetHWEntry(JAL_LE_AUXLE1_TIMER);                }

__INLINE__ void HWle_set_auxle2_timer(const u_int16 auxle2_timer)          {mSetHWEntry(JAL_LE_AUXLE2_TIMER,auxle2_timer);          } 
__INLINE__ u_int16 HWle_get_auxle2_timer(void)                             {return mGetHWEntry(JAL_LE_AUXLE2_TIMER);                }

__INLINE__ void HWle_set_rx_mic_status(const u_int rx_mic_status)          {mSetHWEntry(JAL_LE_RX_MIC_STATUS,rx_mic_status);        } 
__INLINE__ u_int HWle_get_rx_mic_status(void)                              {return mGetHWEntry(JAL_LE_RX_MIC_STATUS);               }


__INLINE__ void HWle_set_delay_search_win(const u_int win_delay)       {mSetHWEntry(JAL_LE_DELAY_SEARCH_WIN,win_delay);     }
__INLINE__ u_int HWle_get_delay_search_win(void)                           {return mGetHWEntry(JAL_LE_DELAY_SEARCH_WIN);            }


__INLINE__ void HWle_set_whitening_enable()       {mSetHWBit(JAL_LE_WHITENING_ENABLE);     }//merge CEVA 0517
__INLINE__ void HWle_clear_whitening_enable()       {mClearHWBit(JAL_LE_WHITENING_ENABLE);     }//merge CEVA 0517

__INLINE__ u_int HWle_get_whitening_enable(void)                           {return mGetHWEntry(JAL_LE_WHITENING_ENABLE);            }

__INLINE__ void HWle_set_whitening_init(const u_int whiten_init)		   {mSetHWEntry(JAL_LE_WHITENING_INIT,whiten_init);         }
__INLINE__ u_int HWle_get_whitening_init(void)                             {return mGetHWEntry(JAL_LE_WHITENING_INIT);              }


__INLINE__ void HWle_set_aes_mode(const u_int aes_mode)		   				{mSetHWEntry(JAL_LE_AES_MODE,aes_mode);         }
__INLINE__ u_int HWle_get_aes_mode(void)                             		{return mGetHWEntry(JAL_LE_AES_MODE);              }

__INLINE__ void HWle_set_aes_enable()		   			{mSetHWBit(JAL_LE_AES_ENABLE);         }//merge CEVA 0517
__INLINE__ void HWle_clear_aes_enable()		   			{mClearHWBit(JAL_LE_AES_ENABLE);         }//merge CEVA 0517

__INLINE__ u_int HWle_get_aes_enable(void)                             		{return mGetHWEntry(JAL_LE_AES_ENABLE);              }

__INLINE__ void HWle_set_aes_start()		   			{mSetHWBit(JAL_LE_AES_START);         }//merge CEVA 0517

__INLINE__ u_int HWle_get_aes_start(void)                             		{return mGetHWEntry(JAL_LE_AES_START);              }

__INLINE__ void HWle_set_aes_data_ready()	  		{mSetHWBit(JAL_LE_AES_DATA_READY);      }//merge CEVA 0517

__INLINE__ u_int HWle_get_aes_data_ready(void)                             	{return mGetHWEntry(JAL_LE_AES_DATA_READY);              }

__INLINE__ void HWle_set_aes_pkt_length(const u_int aes_pkt_length)	  		{mSetHWEntry(JAL_LE_AES_PKT_LENGTH,aes_pkt_length);      }
__INLINE__ u_int HWle_get_aes_pkt_length(void)                             	{return mGetHWEntry(JAL_LE_AES_PKT_LENGTH);              }

__INLINE__ void HWle_set_aes_llid(const u_int aes_llid)	  				{mSetHWEntry(JAL_LE_AES_PKT_LLID,aes_llid);      }
__INLINE__ u_int HWle_get_aes_llid(void)                             	{return mGetHWEntry(JAL_LE_AES_PKT_LLID);              }

//__INLINE__ void HWle_set_aes_mic(const u_int aes_mic)	  				{mSetHWEntry(JAL_LE_AES_MIC,aes_mic);      }
//__INLINE__ u_int HWle_get_aes_mic(void)                             	{return mGetHWEntry(JAL_LE_AES_MIC);              }

__INLINE__ void HWle_set_aes_mic_status(const u_int aes_mic_status)	    	{mSetHWEntry(JAL_LE_AES_RX_MIC_STATUS,aes_mic_status);      }
__INLINE__ u_int HWle_get_aes_mic_status(void)                             	{return mGetHWEntry(JAL_LE_AES_RX_MIC_STATUS);              }

__INLINE__ void HWle_set_aes_active(const u_int aes_active)		    	{mSetHWEntry(JAL_LE_AES_ACTIVE,aes_active);      }
__INLINE__ u_int HWle_get_aes_active(void)                             	{return mGetHWEntry(JAL_LE_AES_ACTIVE);              }

__INLINE__ void HWle_set_aes_finished(const u_int aes_active)		    	{mSetHWEntry(JAL_LE_AES_FINISHED,aes_active);      }
__INLINE__ u_int HWle_get_aes_finished(void)                             	{return mGetHWEntry(JAL_LE_AES_FINISHED);              }

__INLINE__ void HWle_set_aes_pktcntr_byte0(const u_int byte0)		{ mSetHWEntry(JAL_LE_AES_PKTCNT1_B0,byte0);}

__INLINE__ void HWle_set_aes_pktcntr_byte1(const u_int byte1)      { mSetHWEntry(JAL_LE_AES_PKTCNT1_B1,byte1);}

__INLINE__ void HWle_set_aes_pktcntr_byte2(const u_int byte2)      { mSetHWEntry(JAL_LE_AES_PKTCNT1_B2,byte2);}

__INLINE__ void HWle_set_aes_pktcntr_byte3(const u_int byte3)      { mSetHWEntry(JAL_LE_AES_PKTCNT1_B3,byte3);}

__INLINE__ void HWle_set_aes_pktcntr_byte4(const u_int byte4)      { mSetHWEntry(JAL_LE_AES_PKTCNT1_B4,byte4);}


__INLINE__ u_int HWle_get_aes_pktcntr_byte0(void)		{ return mGetHWEntry(JAL_LE_AES_PKTCNT1_B0);}

__INLINE__ u_int HWle_get_aes_pktcntr_byte1(void)      { return mGetHWEntry(JAL_LE_AES_PKTCNT1_B1);}

__INLINE__ u_int HWle_get_aes_pktcntr_byte2(void)      { return mGetHWEntry(JAL_LE_AES_PKTCNT1_B2);}

__INLINE__ u_int HWle_get_aes_pktcntr_byte3(void)      { return mGetHWEntry(JAL_LE_AES_PKTCNT1_B3);}

__INLINE__ u_int HWle_get_aes_pktcntr_byte4(void)      { return mGetHWEntry(JAL_LE_AES_PKTCNT1_B4);}

__INLINE__ void HWle_set_aes_pkt_cntr1(const u_int low_pkt_ctr)
{
#if 0
	const u_int8 byte0 = (u_int8) ( low_pkt_ctr & 0x000000FF);
	const u_int8 byte1 = (u_int8)(( low_pkt_ctr & 0x0000FF00) >> 8);
	const u_int8 byte2 = (u_int8)(( low_pkt_ctr & 0x00FF0000) >> 16);
	const u_int8 byte3 = (u_int8)(( low_pkt_ctr & 0xFF000000) >> 24);

	HWle_set_aes_pkt_cntr_byte0(byte0);
	HWle_set_aes_pkt_cntr_byte1(byte1);
	HWle_set_aes_pkt_cntr_byte2(byte2);
	HWle_set_aes_pkt_cntr_byte3(byte3);
#endif
	//mSetHWEntry(JAL_LE_AES_PKT_COUNTER1,low_pkt_ctr);
}

//__INLINE__ u_int HWle_get_aes_pkt_cntr1(void)                             	{return mGetHWEntry(JAL_LE_AES_PKT_COUNTER1);              }

//__INLINE__ void HWle_set_aes_pkt_cntr2(const u_int high_pkt_ctr)		    	   {mSetHWEntry(JAL_LE_AES_PKT_COUNTER2,high_pkt_ctr);      }
//__INLINE__ u_int HWle_get_aes_pkt_cntr2(void)                             	{return mGetHWEntry(JAL_LE_AES_PKT_COUNTER2);              }

__INLINE__ void HWle_write_data_to_aes(void* p_plaintext,u_int8 len)
{
	hw_memcpy32((void*)(JAL_LE_AES_BUFFER_ADDR),(void*)p_plaintext,len);
}



__INLINE__ void HWle_read_data_from_aes(void* data,u_int8 len)
{
	hw_memcpy32((void*)data, (void*)(JAL_LE_AES_BUFFER_ADDR),len);
}



#endif

