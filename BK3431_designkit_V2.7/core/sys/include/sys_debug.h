#ifndef __PARTHUS_SYS_DEBUG__
#define __PARTHUS_SYS_DEBUG__

/***********************************************************************
 *
 * MODULE NAME:    sys_debug.h
 * PROJECT CODE:   BlueStream
 * MAINTAINER:     John Nelson
 * DATE:           Wed Jul 14 11:39:10 BST 1999
 *
 * SOURCE CONTROL: $Id $
 *
 * LICENCE:
 *    This source code is copyright (c) 1999-2004 Ceva Inc.
 *    All rights reserved.
 *
 * REVISION HISTORY:
 * 15 June 2001    All debug definitions separated from sys_features.h
 *
 * This file is to be used for debug selectors which must be off for
 * release.
 *
 ***********************************************************************/

/***********************************************************************
 * System Wide: Debug/Logging/Test Configurations
 **********************************************************************/

/*
 * Support endianess checks
 */
#ifndef LINUX
#define __LITTLE_ENDIAN 1234
#ifndef __BIG_ENDIAN
#define __BIG_ENDIAN    4321
#endif
#endif

/***********************************************************************
 * Debug Selector definitions (PRH_BS_DBG_<module> should be defined externally  
 **********************************************************************/
/***********************************************************************
 * General Debug Support
 **********************************************************************/
#ifndef NDEBUG
#include <assert.h>
#define SYSdebug_Assert(expr)    assert(expr)
#else
/*
 * Alternatively, can setup a local definition for test purposes
 */
#define SYSdebug_Assert(expr)
#endif
#endif
