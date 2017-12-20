#ifndef _PARTHUS_BT_INIT
#define _PARTHUS_BT_INIT

/***********************************************************************
 *
 * MODULE NAME:    bt_init.h
 * PROJECT CODE:    BlueStream
 * DESCRIPTION:    Intialise the entire BT Stack
 * MAINTAINER:     Ivan Griffin
 * CREATION DATE:  18 February 2000
 *
 * SOURCE CONTROL: $Id: bt_init.h,v 1.1 2012/05/09 00:09:30 garyf Exp $
 *
 * LICENSE:
 *     This source code is copyright (c) 2000-2004 Ceva Inc.
 *     All rights reserved.
 *
 * REVISION HISTORY:
 *    18.Feb.2000 -   IG       - first version
 *
 *
 * ISSUES:
 *    
 ***********************************************************************/


void BT_Initialise(void);
void BT_Initialise_Ex(t_SYS_Fn_Ptr callback);

#endif
