/* 
* $Id: sample_anibmp.c 491 2006-07-18 hakeya $
* Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.    
*   
* This file is a part of Rexy Embedded
*   
*/

#include "rexy.h"
#include "rexy_haldevice.h"

#include "../header/sample_anibmp.h"
#include "rexy_ctl_anibmp.h"
#include "application.h"

#define NUM_OF_BMP	12

#define ANIBMP_MENU_STR0		"0 - Start/Stop Animation"
#define ANIBMP_MENU_STR1		"1 - Create Controls"
#define ANIBMP_MENU_STR2		"2 - Destroy Controls"
#define ANIBMP_MENU_STR3		"3 - Same timeout(>0)"
#define ANIBMP_MENU_STR4		"4 - Same timeout(=0)"
#define ANIBMP_MENU_STR5		"5 - Different timeout"

#define ANIBMP_MENU_STR8		"8 - Clear Screen"
#define ANIBMP_MENU_STR9		"9 - Application Terminate"


// To Do
// This function will be modified, after complete control core implementation.
int KeyDownSampleCallback(WND hWnd, int wparam, int *pErr)
{
	int bHandled=FALSE;

	switch(wparam)
	{
	case VK_USER_UP:
		CtlSeti(hWnd, CTL_ANIBMP_CMD_START_STOP, FALSE, pErr);
		bHandled = TRUE;
		
	default :
		break;
	}
	return bHandled;
}


// 텍스트 메뉴 출력
void PrvDrawMenu(WND wnd, bool bIsSetValue)
{
	GdiLockScreen(wnd);
	GdiClrScreen(wnd, COLOR_WHITE);

	if(bIsSetValue)
		GdiTextOut(wnd, ANIBMP_MENU_STR0, strlen(ANIBMP_MENU_STR0), 10, 10);
	GdiTextOut(wnd, ANIBMP_MENU_STR9, strlen(ANIBMP_MENU_STR9), 10, 25);
	GdiTextOut(wnd, ANIBMP_MENU_STR8, strlen(ANIBMP_MENU_STR8), 10, 40);

	GdiTextOut(wnd, ANIBMP_MENU_STR1, strlen(ANIBMP_MENU_STR1), 10, 60);
	GdiTextOut(wnd, ANIBMP_MENU_STR2, strlen(ANIBMP_MENU_STR2), 10, 75);

	GdiTextOut(wnd, ANIBMP_MENU_STR3, strlen(ANIBMP_MENU_STR3), 10, 90);
	GdiTextOut(wnd, ANIBMP_MENU_STR4, strlen(ANIBMP_MENU_STR4), 10, 105);
	GdiTextOut(wnd, ANIBMP_MENU_STR5, strlen(ANIBMP_MENU_STR5), 10, 120);
//	GdiTextOut(wnd, ANIBMP_MENU_STR6, strlen(ANIBMP_MENU_STR6), 10, 135);
//	GdiTextOut(wnd, ANIBMP_MENU_STR7, strlen(ANIBMP_MENU_STR7), 10, 150);

	GdiUnlockScreen(wnd);
}

// 콘트롤 윈도우를 생성하고 생성된 윈도우 핸들 반환
WND PrvCreateAniBmp(WND hWnd, rect *wndRect, int *pErr)
{
	static WND wnd;

	// Create control
    wnd = CtlCreate(hWnd, CTL_TYPE_ANIBMP, wndRect, NULL, pErr);

	return wnd;
}

// CtlSeti, CtlSetPtr을 사용하여 control Setting
void PrvSetAniBmp(WND hWnd, int type, int timeout, int *pErr)
{
	int i;
	static int aTimeout[NUM_OF_BMP];
	static int aBmpIDs[NUM_OF_BMP];

	for(i=0; i<NUM_OF_BMP; i++)
	{
		aBmpIDs[i] = MGL_icon_01_01_I + i * 4;
		aTimeout[i] = 100 + i * 50;
	}

	if(!hWnd)	return;

	CtlSeti(hWnd, CTL_ANIBMP_CMD_SET_LEFT, 3, pErr);
	CtlSeti(hWnd, CTL_ANIBMP_CMD_SET_TOP, 4, pErr);
	CtlSeti(hWnd, CTL_ANIBMP_CMD_SET_WIDTH, 72, pErr);
	CtlSeti(hWnd, CTL_ANIBMP_CMD_SET_HEIGHT, 70, pErr);
	CtlSeti(hWnd, CTL_ANIBMP_CMD_SET_BGCOLOR, COLOR_BLACK, pErr);

	CtlSeti(hWnd, CTL_ANIBMP_CMD_SET_FRAME_NO, sizeof(aBmpIDs)/sizeof(int), pErr);
	CtlSetPtr(hWnd, CTL_ANIBMP_CMD_SET_P_IDS, aBmpIDs, pErr);
	CtlSeti(hWnd, CTL_ANIBMP_CMD_SET_PLAYTYPE, type, pErr);
	CtlSeti(hWnd, CTL_ANIBMP_CMD_SET_TIMEOUT, timeout, pErr);
	CtlSetPtr(hWnd, CTL_ANIBMP_CMD_SET_P_TIMEOUT, aTimeout, pErr);
	CtlSetPtr(hWnd, CTL_ANIBMP_CMD_SET_CB_KEYDOWN, (void*)KeyDownSampleCallback, pErr);
}

// Timer 시작
void PrvStartAnimation(WND hWnd, int *pErr)
{ 
	CtlSeti(hWnd, CTL_ANIBMP_CMD_START_STOP, TRUE, pErr);
}

void PrvStopAnimation(WND hWnd, int *pErr)
{
	CtlSeti(hWnd, CTL_ANIBMP_CMD_START_STOP, FALSE, pErr);
}

void PrvStartStopAni(WND hWnd, bool *bIsStart, int *pErr)
{
	if(*bIsStart == FALSE)
	{
		PrvStartAnimation(hWnd, pErr);
		*bIsStart = TRUE;
	}
	else
	{
		PrvStopAnimation(hWnd, pErr);
		*bIsStart = FALSE;
	}
}

// Application Message Handler
int PrvSampleAppProc(WND hWnd, int msg, int wparam, int lparam)
{
	static WND wndLT=NULL;
	static WND wndRT=NULL;
	static WND wndCM=NULL;
	static WND wndLB=NULL;
	static WND wndRB=NULL;

	rect Rect;

	static bool bIsSetValue = FALSE;
	static bool bIsStart[5];
	
	int i, err=0;
	int bHandled=FALSE;
		
	switch(msg)
	{
	case AWM_KEYDOWN:
		if(!bIsStart[0])		GdiClrScreen(hWnd, COLOR_WHITE);

		switch(wparam)
		{
		case VK_PAD_1:
			for(i=0; i<5; i++)			bIsStart[i] = FALSE;

			// Create Left-Top Window(Control)
			SET_RECT(&Rect, 0, 0, 80, 80);
			wndLT = PrvCreateAniBmp(hWnd, &Rect, &err);
			// Create Right-Top window(Control)
			SET_RECT(&Rect, 159, 0, 239, 80);
			wndRT = PrvCreateAniBmp(hWnd, &Rect, &err);
			// Create Center-Middle window(Control)
			SET_RECT(&Rect, 79, 119, 159, 199);
			wndCM = PrvCreateAniBmp(hWnd, &Rect, &err);
			// Create Left_Bottom window(Control)
			SET_RECT(&Rect, 0, 239, 80, 319);
			wndLB = PrvCreateAniBmp(hWnd, &Rect, &err);
			// Create Right-Bottom window(Control)
			SET_RECT(&Rect, 159, 239, 239, 319);
			wndRB = PrvCreateAniBmp(hWnd, &Rect, &err);

			PrvDrawMenu(hWnd, bIsSetValue);
			bHandled = TRUE;
			break;

		case VK_PAD_2:
			CtlDestroy(wndLT, &err);
			CtlDestroy(wndRT, &err);
			CtlDestroy(wndCM, &err);
			CtlDestroy(wndLB, &err);
			CtlDestroy(wndRB, &err);

			for(i=0; i<5; i++)
				bIsStart[i] = FALSE;
			
			bIsSetValue = FALSE;
			bHandled = TRUE;
			break;

		case VK_PAD_3:	// each play mode has same timeout value.
			PrvSetAniBmp(wndLT, CTL_ANIBMP_PLAY_NORMAL, 300, &err);
			PrvSetAniBmp(wndRT, CTL_ANIBMP_PLAY_ROTATE, 300, &err);
			PrvSetAniBmp(wndCM, CTL_ANIBMP_PLAY_RANDOM, 300, &err);
			PrvSetAniBmp(wndLB, CTL_ANIBMP_PLAY_REVERSE_ROTATE, 300, &err);
			PrvSetAniBmp(wndRB, CTL_ANIBMP_PLAY_REVERSE, 300, &err);

			bIsSetValue = TRUE;

			for(i=0; i<5; i++)
				if(bIsStart[i])	break;

			PrvDrawMenu(hWnd, bIsSetValue);
			GdiTextOut(hWnd, "3 Key pressed..", 16, 10, 200);
			bHandled = TRUE;
			break;

		case VK_PAD_4:	// each play mode has same timeout value(=0).
			PrvSetAniBmp(wndLT, CTL_ANIBMP_PLAY_NORMAL, 0, &err);
			PrvSetAniBmp(wndRT, CTL_ANIBMP_PLAY_ROTATE, 0, &err);
			PrvSetAniBmp(wndCM, CTL_ANIBMP_PLAY_RANDOM, 0, &err);
			PrvSetAniBmp(wndLB, CTL_ANIBMP_PLAY_REVERSE_ROTATE, 0, &err);
			PrvSetAniBmp(wndRB, CTL_ANIBMP_PLAY_REVERSE, 0, &err);

			bIsSetValue = TRUE;

			for(i=0; i<5; i++)
				if(bIsStart[i])	break;

			PrvDrawMenu(hWnd, bIsSetValue);
			GdiTextOut(hWnd, "4 Key pressed..", 16, 10, 200);
			bHandled = TRUE;
			break;

		case VK_PAD_5:	// each play mode has different timeout value.
			PrvSetAniBmp(wndLT, CTL_ANIBMP_PLAY_NORMAL, 300, &err);
			PrvSetAniBmp(wndRT, CTL_ANIBMP_PLAY_ROTATE, 1000, &err);
			PrvSetAniBmp(wndCM, CTL_ANIBMP_PLAY_RANDOM, 2000, &err);
			PrvSetAniBmp(wndLB, CTL_ANIBMP_PLAY_REVERSE_ROTATE, 1700, &err);
			PrvSetAniBmp(wndRB, CTL_ANIBMP_PLAY_REVERSE, 1500, &err);

			bIsSetValue = TRUE;

			for(i=0; i<5; i++)
				if(bIsStart[i])	break;

			PrvDrawMenu(hWnd, bIsSetValue);
			GdiTextOut(hWnd, "5 Key pressed..", 16, 10, 200);
			bHandled = TRUE;
			break;

		case VK_PAD_6:
			SET_RECT(&Rect, 50, 50, 150, 150);
			CtlSetPtr(wndCM, CTL_ANIBMP_CMD_SET_WNDRECT, &Rect, &err);
			bHandled = TRUE;
			break;

		case VK_PAD_7:
			for(i=0; i<5; i++)
				if(bIsStart[i])	break;

			PrvDrawMenu(hWnd, bIsSetValue);
			GdiTextOut(hWnd, "Warning!! Invalide Key!", 24, 10, 200);
			bHandled = TRUE;
			break;

		case VK_PAD_8:
			GdiClrScreen(hWnd, COLOR_WHITE);
			PrvDrawMenu(hWnd, bIsSetValue);
			bHandled = TRUE;
			break;

		case VK_PAD_9:
			for(i=0; i<5; i++)			bIsStart[i] = FALSE;
			bIsSetValue = FALSE;

			StaGotoState(hWnd, STATE_ID_CTL_TEST, MSG_DESTROY, MSG_CREATE);
			bHandled = TRUE;
			break;

		case VK_PAD_0:	// start/stop all
			if(bIsSetValue==FALSE)
			{
				GdiTextOut(hWnd, "Warning!! Invalide Key!", 24, 10, 200);
				break;
			}

			PrvStartStopAni(wndLT, &bIsStart[0], &err);
			PrvStartStopAni(wndRT, &bIsStart[1], &err);
			PrvStartStopAni(wndCM, &bIsStart[2], &err);
			PrvStartStopAni(wndLB, &bIsStart[3], &err);
			PrvStartStopAni(wndRB, &bIsStart[4], &err);
			bHandled = TRUE;
			break;
		}
		break;
	}
	return bHandled;
}


// Application Message Handler
int SampleAniBmpGateProc(WND wnd, int msg, int wparam, int lparam)
{
	WND SampleAniBmpMainWindow;
	int	ScreenWidth;
	int ScreenHeight;
	int bHandled = FALSE;

	switch ( msg )
	{
	case AWM_CREATE:
			// Create Window for this application
			GdiGetScreenInfo( &ScreenWidth, &ScreenHeight ); // We are going to use whole screen
			SampleAniBmpMainWindow = WinCreateWindow(PrvSampleAppProc, 0, 0, ScreenWidth, ScreenHeight, WND_PANEL );

			// Do something for your application's initialization


			PrvDrawMenu(wnd, FALSE);
			
			bHandled = TRUE;
			break;
			
		case AWM_DESTROY:
			
			// Do something to close this app
			GdiSetBkColor(wnd,COLOR_WHITE);
			GdiSetFgColor(wnd,COLOR_BLACK);

			// Close all window that are created by this application
			WinDestroyWindow(wnd);
			bHandled = TRUE;	// End this application
			break;
	}

	return bHandled;
}
