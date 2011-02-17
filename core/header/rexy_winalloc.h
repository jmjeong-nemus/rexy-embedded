/*
 * $Id: rexy_winalloc.h 7590 2005-09-09 14:16:11Z klenui $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 *	REXY에서 제공하는 메모리 관리 루틴을 위한 함수들이 
 *	선언되어 있다. 
 *
 */

#ifndef __REXY_WINALLOC_H__
#define __REXY_WINALLOC_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @file */

#include "rexy_type.h"

#define MAX_WND_STORAGE	30

    void InitWNDFreeStructures(void);
    void DelWNDFreeStructures(void);
    WND malloc_w( int size );
    void free_w( WND wnd );

/** @} */

#ifdef __cplusplus
}
#endif	// __cplusplus

#endif	// __REXY_WINALLOC_H__



