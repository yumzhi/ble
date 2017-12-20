/***********************************************************************
 *
 * MODULE NAME:    sys_init.c
 * PROJECT CODE:   BlueStream
 * DESCRIPTION:    hal specific init code
 * MAINTAINER:     Ivan Griffin
 * CREATION DATE:  21 September 2001
 *
 * SOURCE CONTROL: $Id: 
 *
 * LICENSE:
 *     This source code is copyright (c) 2001-2004 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    21 Sept 2001 - RGB  - initial version
 *
 *    
 ***********************************************************************/

#include "sys_config.h"
#include "sys_types.h"

#include "sys_init.h"

#include "hw_lc.h"
#include "hw_radio.h"

#include "hw_register.h"
#include "hw_hab_defs.h"
#include "hw_habanero.h"

#ifdef _VIRTUAL_HOST_
extern u_int32 gTXPacketCount;
extern u_int8  gFirstVaild_link_id;
extern u_int32 gRXPacketCount;
#endif
void SYSinit_Initialise(void)
{
#if (BUILD_TYPE!=FLASH_BUILD)
    /*
     * if a ROM build, this is called from startup assembly -- otherwise
     * need to hook it in here...
     *
     * halts system if problem with endianness, or SRAM 
     */
    /* SYSpost_Test(); */
#endif
    /* Setup the Tabasco Clock Divider (40MHz / 5 => 8MHz) */
    {
        //mHWreg_Create_Cache_Register(HAB_RF_MUX_CLK_DIV_REG); //no used BK3431 ,HAB_RF_MUX_CLK_DIV_REG already re-design in ICU part
        //mHWreg_Load_Cache_Register(HAB_RF_MUX_CLK_DIV_REG);   //no used BK3431 ,HAB_RF_MUX_CLK_DIV_REG already re-design in ICU part
        //mHWreg_Assign_Cache_Field(HAB_RF_MUX_CLK_DIV_REG, HAB_CLK_DIVIDER, 5); //no used BK3431 ,HAB_CLK_DIVIDER already re-design in ICU part
        //mHWreg_Store_Cache_Register(HAB_RF_MUX_CLK_DIV_REG);//no used BK3431 ,HAB_CLK_DIVIDER already re-design in ICU part
    }
    #ifdef _VIRTUAL_HOST_
    gTXPacketCount = 0;
    gRXPacketCount = 0;
    gFirstVaild_link_id = 0x0f;
    #endif
}

