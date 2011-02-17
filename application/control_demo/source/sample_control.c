/* 
* $Id: sample_list.c 491 2006-07-14 00:26:34Z venabili $
* Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.    
*   
* This file is a part of Rexy Embedded
*   
*/

#include "rexy.h"
#include "rexy_haldevice.h"

#include "../header/sample_control.h"
#include "application.h"

static char* ListString[] =
{
    "1. List Control Demo",
    "2. Popup Control Demo",
    "3. Edit Control Demo",
    "4. Sliding Text Control Demo",
    "5. Animation Bitmap Control Demo",
    "9. Exit Control Demo",
};
static int ListStringCount = sizeof(ListString) / sizeof(char*);

static rint32 PrvSamCtlKeyEvent(WND wnd, rint32 wparam, rint32 lparam);
static void PrvSamCtlPaint(WND wnd);

static rint32 PrvSamCtlKeyEvent(WND wnd, rint32 wparam, rint32 lparam)
{
	switch(wparam)
	{
	case VK_PAD_1:
		StaGotoState(wnd, STATE_ID_CTL_LIST_TEST, MSG_DESTROY, MSG_CREATE);			
		break;
	case VK_PAD_2:
		StaGotoState(wnd, STATE_ID_CTL_POPUP_TEST, MSG_DESTROY, MSG_CREATE);			
		break;
	case VK_PAD_3:
		StaGotoState(wnd, STATE_ID_CTL_EDIT_TEST, MSG_DESTROY, MSG_CREATE);			
		break;
	case VK_PAD_4:
		StaGotoState(wnd, STATE_ID_CTL_SLIDETEXT_TEST, MSG_DESTROY, MSG_CREATE);			
		break;
	case VK_PAD_5:
		StaGotoState(wnd, STATE_ID_CTL_ANIBMP_TEST, MSG_DESTROY, MSG_CREATE);			
		break;

	case VK_PAD_9:
		StaGotoState(wnd, STATE_ID_ENTRY, MSG_DESTROY, MSG_CREATE);			
		break;

	}
	return 0;
}

static void PrvSamCtlPaint(WND wnd)
{
	int i;

	GdiLockScreen(wnd);
	GdiClrScreen(wnd, COLOR_WHITE);

	for (i = 0; i < ListStringCount; i++)
		GdiTextOut(wnd, ListString[i], StrLen(ListString[i]), 10, (rint16)(10 + 15 * i));

	GdiUnlockScreen(wnd);
}

rint32 PrvSampleControlProc(WND wnd, int msg, int wparam, int lparam)
{
	int bHandled = FALSE;
    switch(msg)
    {
    case AWM_CREATE:
		PrvSamCtlPaint(wnd);
		bHandled = TRUE;
        break;
    case AWM_DESTROY:
		bHandled = TRUE;
        break;

    case AWM_PAINT:
		PrvSamCtlPaint(wnd);
		bHandled = TRUE;
        break;

    case AWM_KEYDOWN:
		PrvSamCtlKeyEvent(wnd, wparam, lparam);
		bHandled = TRUE;
        break;
    default:
        break;
    }

    return bHandled;
}

rint32 SampleControlProc(WND wnd, int msg, int wparam, int lparam)
{
	int ScreenWidth, ScreenHeight;
	WND SamCtrlWnd;
	int bHandled = FALSE;
    switch(msg)
    {
    case AWM_CREATE:
		GdiGetScreenInfo( &ScreenWidth, &ScreenHeight ); // We are going to use whole screen
		SamCtrlWnd = WinCreateWindow(PrvSampleControlProc, 0, 0, ScreenWidth, ScreenHeight, WND_PANEL);
		bHandled = TRUE;
        break;

    case AWM_DESTROY:
		WinDestroyWindow(wnd);
		bHandled = TRUE;
        break;

    default:
        break;
    }

    return bHandled;
}

