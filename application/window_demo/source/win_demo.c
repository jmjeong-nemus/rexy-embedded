/* 
* $Id: win_demo.c 491 2006-07-04 15:03:34Z venabili $
* Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.    
*   
* This file is a part of Rexy
*   
*/

#include "rexy.h"
#include "rexy_haldevice.h"

#include "../header/win_demo.h"
#include "rexy_ctl_core.h"
#include "rexy_ctl_list.h"
#include "rexy_ctl_popup.h"
#include "application.h"

extern WND MainWndHandle;

#define WINDEMO_TIMER	10000
#define WINDEMO_CHILD_TIMER	11000

#define CHILD_WIDTH		100
#define CHILD_HEIGHT	40
#define MAX_CHILD		3
#define MAX_WINDOW		3

#define WINDEMO_MENU	0
#define WINDEMO_WINDOW	1
#define WINDEMO_CONTROL	2

static unsigned short gChildBGColor[3] = { COLOR_YELLOW, COLOR_GREEN, COLOR_BLUE};

static rint32 PrvWinDemoKeyEvent(WND wndd, rint32 wparam, rint32 lparam);
static rint32 PrvWinTestProc(WND wnd, int msg, int wParam, int lParam);
static rint32 PrvWinTestCtrlProc(WND wnd, int msg, int wparam, int lparam);
static rint32 PrvWinDemoTimer(WND wnd, rint32 wparam, rint32 lparam);
static rint32 PrvWinDemoChildTimer(WND wnd, rint32 wparam, rint32 lparam);

static void PrvWinDemoPaint(WND wnd);
static void PrvWinDemoChildPaint(WND wnd);
static void PrvWinDemoCurrentWindow(WND wnd);

static int PrvWinDemoWindowProc(WND wnd, int msg, int wparam, int lparam);

static rint32 winDemoState;
static rint32 winDemoTimerCount;
static rint32 winDemoChildTimerCount;

static rint32 childCountWindow, childCountControl, ctrlID;
static WND winPanel;
static WND childPanel[3] = {0, 0, 0};

static rint32 childTopOffset[3] = { 0, 50, 100};
static rint32 childTimerCount[MAX_CHILD];
static rint32 childTimer[MAX_CHILD];
static rint32 windowTimerCount[MAX_WINDOW];
static rint32 windowTimer[MAX_WINDOW];

static void PrvWinDemoPaint(WND wnd)
{
	WND w;
	char tmp[20];
	GdiLockScreen(wnd);
	GdiClrScreen(wnd, COLOR_WHITE);
	PrvWinDemoCurrentWindow(wnd);
	if (winDemoState == WINDEMO_MENU) 
	{
		GdiTextOut(wnd, "1 : Window Test", StrLen("1 : Window Test"), 10, 20);
		GdiTextOut(wnd, "2 : Control Test", StrLen("2 : Control Test"), 10, 35);
		GdiTextOut(wnd, "9 : End Program", StrLen("9 : End Program"), 10, 50);
	}
	if (winDemoState == WINDEMO_WINDOW) 
	{
		GdiTextOut(wnd, "1 : Create new child window", StrLen("1 : Create new child window"), 10, 20);
		GdiTextOut(wnd, "2 : Destroy child window", StrLen("2 : Destroy child window"), 10, 35);
		GdiTextOut(wnd, "3 : Hide child window", StrLen("3 : Hide child window"), 10, 50);
		GdiTextOut(wnd, "4 : Show child window", StrLen("4 : Show child window"), 10, 65);
		GdiTextOut(wnd, "8 : Update Screen", StrLen("8 : Update Screen"), 10, 80);
		GdiTextOut(wnd, "9 : Up to Menu", StrLen("9 : Up to Menu"), 10, 95);
	}
	if (winDemoState == WINDEMO_CONTROL) 
	{
		GdiTextOut(wnd, "1 : Create new child control", StrLen("1 : Create new child control"), 10, 20);
		GdiTextOut(wnd, "2 : Destroy child control", StrLen("2 : Destroy child control"), 10, 35);
		GdiTextOut(wnd, "3 : Show Up child control", StrLen("3 : Show Up child control"), 10, 50);
		GdiTextOut(wnd, "4 : Show Down child control", StrLen("4 : Show Down child control"), 10, 65);
		GdiTextOut(wnd, "8 : Update Screen", StrLen("8 : Update Screen"), 10, 80);
		GdiTextOut(wnd, "9 : Up to Menu", StrLen("9 : Up to Menu"), 10, 95);
		w = WinGetFirstControl(wnd, 0);
		while(w)
		{
			WinShowWindow(w, SHOW_PANEL);
			w = WinGetNextControl(wnd, w, 0);
		}
	}
	StrPrintF(tmp, "Timer Count : %d", winDemoTimerCount);
	GdiTextOut(wnd, tmp, StrLen(tmp), 10, 300);

	GdiUnlockScreen(wnd);
}


static void PrvWinDemoCurrentWindow(WND wnd)
{
	char tmp[12];
	//WND c;
	//c = WinGetCurrentWindow();
	StrPrintF(tmp, "0x%X", wnd);
	GdiTextOut(wnd, tmp, StrLen(tmp), 10, 5 );
}

static void PrvWinDemoChildPaint(WND wnd)
{
	char tmp[20];

	if (wnd->window_visible == WND_INVISIBLE) return;
	GdiLockScreen(wnd);
	GdiClrScreen(wnd, wnd->bg_color);
	PrvWinDemoCurrentWindow(wnd);
	StrPrintF(tmp, "child %d %02d ", wnd->id, childTimerCount[wnd->id]);
	GdiTextOut(wnd, tmp, StrLen(tmp), 10, 20);
	GdiUnlockScreen(wnd);
}

static rint32 PrvWinTestCtrlProc(WND wnd, int msg, int wparam, int lparam)
{
    switch(msg)
    {
    case AWM_CREATE:
		GdiTextOut(wnd, "Control", StrLen("Control"), 10, 30);
        break;

    case AWM_PAINT:
        break;
	case AWM_TIMER:
		break;
    default:
        break;
    }

    return 0;
}

static rint32 GetEmptyCtlID()
{
	int i;
	for (i=0; i<MAX_CHILD; i++)
	{
		if (childTimer[i] == 0)
		{
			childTimer[i] = 1;
			return i;
		}
	}
	return -1;
}

static rint32 GetEmptyWindowID()
{
	int i;
	for (i=0; i<MAX_WINDOW; i++)
	{
		if (windowTimer[i] == 0)
		{
			windowTimer[i] = 1;
			return i;
		}
	}
	return -1;
}

static rint32 PrvWinTestProc(WND wnd, int msg, int wparam, int lparam)
{
	int bHandled = FALSE;
	int i;
    switch(msg)
    {
    case AWM_CREATE:
		wnd->bg_color = gChildBGColor[wnd->id%3];
		UsrSetTimer(wnd, WINDEMO_CHILD_TIMER + wnd->id, 200 * (wnd->id + 1));
		PrvWinDemoChildPaint(wnd);
		bHandled = TRUE;
        break;
	case AWM_DESTROY:
		UsrKillTimer(wnd, WINDEMO_CHILD_TIMER + wnd->id);
		childTimer[wnd->id] = 0;
		childTimerCount[wnd->id] = 0;
		bHandled = TRUE;
		break;
    case AWM_PAINT:
		if (winDemoState == WINDEMO_WINDOW)
		{
			for (i=0; i<3; i++)
			{
			//	if (childPanel[i]) WinShowWindow(childPanel[i], SHOW_PANEL);
			}
		}
		if (winDemoState == WINDEMO_CONTROL)
		{
			//w = wnd->childwin;
			//while(w)
			//{
			//	WinUpdateWindow(w, SHOW_PANEL);
			//	w = w->nextwin;
			//}
		}
		PrvWinDemoChildPaint(wnd);
		bHandled = TRUE;
        break;
	case AWM_SETFOCUS:
		wnd->bg_color = COLOR_RED;
		//PrvWinDemoChildPaint(wnd);
		WinUpdateWindow(wnd->parentwin, SHOW_PANEL);
		bHandled = TRUE;
		break;

	case AWM_KILLFOCUS:
		wnd->bg_color = gChildBGColor[wnd->id%3]; //COLOR_BLUE;
		//PrvWinDemoChildPaint(wnd);
		WinUpdateWindow(wnd->parentwin, SHOW_PANEL);
		bHandled = TRUE;
		break;

    case AWM_KEYDOWN:
		if (WinGetSelection() != wnd) return bHandled;
		switch(wparam)
		{
		case VK_PAD_1:
			//GdiClrScreen(wnd, COLOR_GREEN);
			break;
		case VK_PAD_2:
			//GdiClrScreen(wnd, COLOR_YELLOW);
			break;
		case VK_PAD_3: // z-order up
			WinSetShowUpControl(wnd);
			break;
		case VK_PAD_4: // z-order down
			WinSetShowDownControl(wnd);
			break;
		case VK_USER_UP:
//			if (wnd->frame.top >=30) {
				WinSetWindowPos(wnd, wnd->frame.left, wnd->frame.top - 10, CHILD_WIDTH, CHILD_HEIGHT, SHOW_PANEL);
//			}
			
			break;
		case VK_USER_DOWN:
//			if (wnd->frame.top <= 260) {
				WinSetWindowPos(wnd, wnd->frame.left, wnd->frame.top + 10, CHILD_WIDTH, CHILD_HEIGHT, SHOW_PANEL);
//			}
			
			break;
		case VK_USER_LEFT:
//			if (wnd->frame.left >= 20) {
				WinSetWindowPos(wnd, wnd->frame.left - 10, wnd->frame.top, CHILD_WIDTH, CHILD_HEIGHT, SHOW_PANEL);
//			}
			break;
		case VK_USER_RIGHT:
//			if (wnd->frame.left <= 120) {
				WinSetWindowPos(wnd, wnd->frame.left + 10, wnd->frame.top, CHILD_WIDTH, CHILD_HEIGHT, SHOW_PANEL);
//			}
			break;
		case VK_PAD_9:
			WinSetSelection(wnd->parentwin);
		break;
		}
		bHandled = TRUE;
        break;
	case AWM_TIMER:
		PrvWinDemoChildTimer(wnd, wparam, lparam);
		bHandled = TRUE;
		break;

    default:
        break;
    }

    return bHandled;
}

static rint32 PrvWinDemoKeyEventWindow(WND wnd, rint32 wparam, rint32 lparam)
{
	char tmp[12];
	rect ctrl_rect;
	int i;
	
	switch(wparam)
	{
	case VK_PAD_1:
		if (childCountWindow < 3)
		{
			
			ctrl_rect.top = 20 + childCountWindow*50;
			ctrl_rect.left = 20;
			ctrl_rect.bottom = ctrl_rect.top + CHILD_HEIGHT; 
			ctrl_rect.right = ctrl_rect.left + CHILD_WIDTH;
			childPanel[childCountWindow] = WinCreateWindow(PrvWinTestProc, ctrl_rect.left, ctrl_rect.top, CHILD_WIDTH, CHILD_HEIGHT, WND_PANEL);

			childCountWindow++;
		}
		break;
	case VK_PAD_2:
		WinUpdateAllWindow();
		winPanel = NULL;
		break;
	case VK_PAD_3:
		//WinShowWindow(childPanel[0], HIDE_PANEL);
		break;
	case VK_PAD_4:
		//WinShowWindow(childPanel[0], SHOW_PANEL);
		break;
	case VK_PAD_5:
		if (childPanel[0])
		{
			WinSetSelection(childPanel[0]);
		}
		StrPrintF(tmp, "0x%X ", WinGetSelection());
		GdiTextOut(wnd, tmp, StrLen(tmp), 10, 290);
		break;
	case VK_USER_UP:
		break;
	case VK_USER_DOWN:
		break;
	case VK_USER_LEFT:
		break;
	case VK_USER_RIGHT:
		break;
	case VK_PAD_8:
		WinUpdateAllWindow();
		break;
	case VK_PAD_9:
		winDemoState = WINDEMO_MENU;
		for (i=0; i<3; i++)
		{
			if (childPanel[i]) WinShowWindow(childPanel[i], HIDE_PANEL);
		}
		PrvWinDemoPaint(wnd);
	break;
	}
	return 0;
}


static rint32 PrvWinDemoKeyEventControl(WND wnd, rint32 wparam, rint32 lparam)
{
	int bHandled = FALSE;
	rint32 offset;
	rect ctrl_rect;
	WND w;
	if (wparam == VK_USER_TAB)
	{
		w = wnd->childwin;
		if (winPanel == NULL)
		{
			winPanel = WinGetFirstControl(wnd, 0);
		}
		else
		{
			if (winPanel->nextwin != NULL) winPanel = winPanel->nextwin;
			else winPanel = WinGetFirstControl(wnd, 0);
		}
		if (winPanel) WinSetSelection(winPanel);
		//WinSetHiliteControl(wnd, winPanel);
		bHandled = TRUE;
		return bHandled;
	}

	if (WinGetSelection() != wnd) return 0;
	switch(wparam)
	{
	case VK_PAD_1:
		if (childCountControl < 3)
		{
			if (wnd->childwin == NULL || childCountControl == 0) offset = 0;
			else
			{
				offset = WinGetLastControl(wnd, 0)->frame.top;
				if (offset > 100) offset = 0;
				else offset += 50;
			}
			
			ctrl_rect.top = 20 + offset;
			ctrl_rect.left = 20;
			ctrl_rect.bottom = ctrl_rect.top + CHILD_HEIGHT; 
			ctrl_rect.right = ctrl_rect.left + CHILD_WIDTH;
			ctrlID = GetEmptyCtlID();
			if (ctrlID == -1 ) ctrlID = 4;
			w = WinAddControl(wnd, WND_USERCONTROL, ctrlID, AWS_VISIBLE, &ctrl_rect, PrvWinTestProc);
			//WinShowWindow(w, SHOW_PANEL);
			childCountControl++;
		}
		bHandled = TRUE;
		break;
	case VK_PAD_2:
		//break;
		if (wnd->childwin) {
			WinDestroyControl(WinGetFirstControl(wnd, 0));
			childCountControl--;
		}
		//WinUpdateAllWindow();
		WinUpdateWindow(wnd, ERASE_BKGND);
		winPanel = NULL;
		bHandled = TRUE;
		break;
	case VK_PAD_3:
		break;
	case VK_PAD_4:
		break;
	case VK_PAD_5:
		if (wnd->childwin)
		{
			WinSetSelection(wnd->childwin);
		}
		bHandled = TRUE;
		break;
	case VK_PAD_6:
		{
			//char tmp[12];
			//StrPrintF(tmp, "0x%X 0x%X", WinGetSelection(), wnd->window_type);
			//GdiTextOut(wnd, tmp, StrLen(tmp), 10, 300);
		}
		break;
	case VK_USER_UP:
		if (winPanel == NULL)
		{
			winPanel = WinGetFirstControl(wnd, 0);
		}
		else
		{
			if (winPanel->prevwin != NULL) winPanel = winPanel->prevwin;
		}

		WinSetSelection(winPanel);
		WinSetHiliteControl(wnd, winPanel);
		bHandled = TRUE;
		break;
	case VK_USER_DOWN:
		if (winPanel == NULL)
		{
			winPanel = WinGetFirstControl(wnd, 0);
		}
		else
		{
			if (winPanel->nextwin != NULL) winPanel = winPanel->nextwin;
		}
			
		WinSetSelection(winPanel);
		WinSetHiliteControl(wnd, winPanel);
		bHandled = TRUE;
		break;
	case VK_USER_LEFT:
		break;
	case VK_USER_RIGHT:
		break;
	case VK_PAD_8:
		WinUpdateWindow(wnd, ERASE_BKGND);
		break;
	case VK_PAD_9:
		winDemoState = WINDEMO_MENU;
		w = wnd->childwin;
		while(w)
		{
			WinShowWindow(w, HIDE_PANEL);
			w = w->nextwin;
		}
		PrvWinDemoPaint(wnd);
		bHandled = TRUE;
		break;
	}
	return bHandled;
}


static rint32 PrvWinDemoKeyEvent(WND wnd, rint32 wparam, rint32 lparam)
{
	switch(wparam)
	{
	case VK_PAD_1:
		winDemoState = WINDEMO_WINDOW;
		PrvWinDemoPaint(wnd);
		break;
	case VK_PAD_2:
		winDemoState = WINDEMO_CONTROL;
		//WinUpdateWindow(wnd, SHOW_PANEL);
		PrvWinDemoPaint(wnd);
		break;
	case VK_PAD_8:
		WinUpdateWindow(wnd, ERASE_BKGND);
		break;
	case VK_PAD_9:
		StaGotoState(wnd, STATE_ID_ENTRY, MSG_DESTROY, MSG_CREATE );
	break;
	}
	return 0;
}

static rint32 PrvWinDemoTimer(WND wnd, rint32 wparam, rint32 lparam)
{
	char tmp[20];
	if (winDemoTimerCount == 1000) winDemoTimerCount = -1;
	StrPrintF(tmp, "Timer Count : %d", ++winDemoTimerCount);
	GdiLockScreen(wnd);
	GdiTextOut(wnd, tmp, StrLen(tmp), 10, 300);
	GdiUnlockScreen(wnd);
	return 0;
}

static rint32 PrvWinDemoChildTimer(WND wnd, rint32 wparam, rint32 lparam)
{
	if (wnd->window_visible == WND_INVISIBLE) {
		childTimerCount[wnd->id] += 1;
		return -1;
	}
	if (childTimerCount[wnd->id] == 50) childTimerCount[wnd->id] = 0;
	else childTimerCount[wnd->id]++;

	MsgPostMessage(wnd, AWM_PAINT, 0, 0);
	return 0;
}

int	WinDemoProc(WND wnd, int msg, int wparam, int lparam)
{
	int i;
	int	ScreenWidth,ScreenHeight;
	int	bHandled=FALSE;
	WND	WinDemoWindowH;
	switch ( msg )
	{
		case AWM_CREATE:

			GdiGetScreenInfo( &ScreenWidth, &ScreenHeight ); // We are going to use whole screen
			WinDemoWindowH = WinCreateWindow( PrvWinDemoWindowProc, 0, 0, ScreenWidth, ScreenHeight, WND_PANEL );
			WinSetSelection( WinDemoWindowH );

			winDemoState = WINDEMO_MENU;
			for (i=0; i<MAX_CHILD; i++)
			{
				childTimerCount[i] = 0;
				childTimer[i] = 0;
			}
			winDemoTimerCount = 0;
			childCountWindow = 0; childCountControl = 0; ctrlID = 0;
			winPanel = NULL;
			GdiClrScreen(wnd, COLOR_WHITE);
			UsrSetTimer(wnd, WINDEMO_TIMER, 1000);
			PrvWinDemoPaint(wnd);
			bHandled = TRUE;
			break;
		
		case AWM_DESTROY:

			if (wnd->childwin)
			{
				WinDestroyAllControl(wnd);
			}
			for (i=0; i<3; i++)
			{
				if (childPanel[i]) WinDestroyWindow(childPanel[i]);
			}
			UsrKillTimer(wnd, WINDEMO_TIMER);
			WinDestroyWindow(wnd);
			bHandled = TRUE;
			break;
	}
	return bHandled;
}

static int PrvWinDemoWindowProc(WND wnd, int msg, int wparam, int lparam)
{
	int	bHandled=FALSE;
    switch(msg)
    {
    case AWM_PAINT:
		PrvWinDemoPaint(wnd);
		bHandled = TRUE;
        break;

    case AWM_KEYDOWN:
		switch(winDemoState)
		{
		case WINDEMO_MENU:
			bHandled = PrvWinDemoKeyEvent(wnd, wparam, lparam);
			break;
		case WINDEMO_WINDOW:
			bHandled = PrvWinDemoKeyEventWindow(wnd, wparam, lparam);
			break;
		case WINDEMO_CONTROL:
			bHandled = PrvWinDemoKeyEventControl(wnd, wparam, lparam);
			break;
		}
        break;
	case AWM_TIMER:
		PrvWinDemoTimer(wnd, wparam, lparam);
		bHandled = TRUE;
		break;
    default:
		break;
    }

    return bHandled;
}

