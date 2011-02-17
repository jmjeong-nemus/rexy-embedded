/* 
* $Id$
* Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.    
*   
* This file is a part of Biorhythm application
*   
*/

#ifndef __BIORHYTHM_H__
#define __BIORHYTHM_H__

#include "rexy.h"

#ifdef __cplusplus
extern "C" {
#endif
	//biorhythm structure
	typedef struct
	{
		rint16 nIntellectual;
		rint16 nPhysical;
		rint16 nEmotional;
	} BiorhythmType;

	typedef enum 
	{
		BIO_BIRTHDAYINPUT_FORM = 1,
		BIO_GRAPHDRAW_FORM,

		BIO_LAST_FORM
	} BiorhythmFormList;

	typedef struct
	{
		BiorhythmFormList eFormId;
		void (*pfnFormHandler)(WND hWnd, rint32 msg, rint32 wParam, rint32 lParam);
	} BiorhythmFormHandle;

	typedef struct  
	{
		int nLocation;
		int nInYear;
		int nInMonth;
		int nInDay;
		int nSelectYear;
		int nSelectMonth;
		int nSelectDay;
		int nCurrFormID;
		int nFocus;
		int nScreenWidth;
		int nScreenHeight;
		
		BiorhythmType Biorhythm[32];
	} BiorhythmGlobalsType, *BiorhythmGlobalsPtr;
	
    rint32 BiorythmProc(WND hWnd, rint32 msg, rint32 wParam, rint32 lParam);

#ifdef __cplusplus
}
#endif	// __cplusplus
#endif	// __BIORYTHM_H__
