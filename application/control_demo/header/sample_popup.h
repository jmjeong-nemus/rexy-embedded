/* 
* $Id: sample_pop.h 491 2006-01-11 00:26:34Z madholic $
* Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.    
*   
* This file is a part of Rexy Embedded
*   
*/

#ifndef __SAMPLE_POPUP_H__
#define __SAMPLE_POPUP_H__

#include "rexy.h"


#ifdef __cplusplus
extern "C" {
#endif

    int PopSampleProc(WND wnd, int msg, int wparam, int lparam);
	void PrvSampleDestoryWin(WND wnd);

#ifdef __cplusplus
}
#endif	// __cplusplus
#endif	// __SAMPLE_POPUP_H__
