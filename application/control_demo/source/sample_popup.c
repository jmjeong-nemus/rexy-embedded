/* 
* $Id: sample_pop.c 491 2006-01-11 00:26:34Z madholic $
* Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.    
*   
* This file is a part of Rexy Embedded
*   
*/

#include "rexy.h"
#include "rexy_haldevice.h"

#include "../header/sample_popup.h"
#include "rexy_ctl_popup.h"
#include "application.h"

#define SAMPLE_POPUP_ID 1

#define CTL_POPUP_STRING	"Press 0, Popup Test"
#define CTL_POPUP_POPUP1	"Popup #1"
#define CTL_POPUP_POPUP2	"Popup #2"
#define CTL_POPUP_POPUP3	"Popup #3"
#define CTL_POPUP_STRING1	"Press 1, close popup"
#define CTL_POPUP_STRING2	"Press 2, next popup"
#define CTL_POPUP_STRING3	"Press 3, prev popup"
#define CTL_POPUP_STRING4	"Press 4, new popup"
#define CTL_POPUP_STRING5	"Press 5, close popup"
#define CTL_POPUP_STRING6	"This Popup will be "
#define CTL_POPUP_STRING7	"closedafter 7 seconds"
#define CTL_POPUP_STRING8	"automatically."

#define CTL_POPUP_INFO_00	"Press 0, Close Popup"
#define CTL_POPUP_INFO_01	"Press 1, Show Confirm	Popup"
#define CTL_POPUP_INFO_02	"Press 2, Show Question Popup"
#define CTL_POPUP_INFO_03	"Press 3, Show Progress Popup"
#define CTL_POPUP_INFO_09	"Press 9, Quit"

WND gpMainWnd = NULL;

WND gpWND1 = NULL;
WND gpWND2 = NULL;
WND gpWND3 = NULL;

WND g_wndConfirmPopup = NULL;
WND g_wndQuestionPopup = NULL;
WND g_wndProgressPopup = NULL;

bool g_bPopupCreated = FALSE;

RxBITMAP g_popBmp;
bool	g_IsStart;
bool	g_IsFirstPop;
bool	g_IsSecondPop;
bool	g_IsThirdPop;


WND  PrvSamplePopupGetWND(void);
void PrvSamplePopupSetWND(WND wnd);
void PrvSamplePopupInitMainWnd(WND wnd);
void PrvSamplePopupDestoryMainWnd(WND wnd);
void PrvSampePopupMainWinProc(WND wnd,int msg, int wparam, int lparam);
void PrvSamplePopupDrawMainMenu (WND wnd);
int PrvSamplePopupProcessTimer(WND wnd, int msg, int wparam, int lparam);
int PrvSamplePopupProcessKey(WND wnd, int msg, int wparam, int lparam);
WND PrvSamplePopupCreateConfirmPopup(WND wnd, char *str);
WND PrvSamplePopupCreateQuestionPopup(WND wnd, char *str);
WND PrvSamplePopupCreateProgressPopup(WND wnd, char *str);

void PrvSamplePopupDrawPopup(WND wnd);
void CloseConfirmPopup(void);
int PrvSamplePopupCB(WND wnd, int msg, int wparam, int lparam, int isPre, int *pErr);

void PrvSamplePopupDrawMainMenu (WND wnd)
{
	GdiLockScreen(wnd);
	GdiClrScreen(wnd, COLOR_WHITE);
	GdiTextOut(wnd, CTL_POPUP_INFO_01, StrLen(CTL_POPUP_INFO_01), 10, 10);
	GdiTextOut(wnd, CTL_POPUP_INFO_02, StrLen(CTL_POPUP_INFO_02), 10, 30);
	GdiTextOut(wnd, CTL_POPUP_INFO_03, StrLen(CTL_POPUP_INFO_03), 10, 50);
	GdiTextOut(wnd, CTL_POPUP_INFO_09, StrLen(CTL_POPUP_INFO_09), 10, 70);
	GdiUnlockScreen(wnd);
}

int PopDrawPostCB1(WND wnd)
{
	GdiLockScreen(wnd);
	GdiTextOut(wnd, CTL_POPUP_STRING1, StrLen(CTL_POPUP_STRING1), 30, 30);
	GdiTextOut(wnd, CTL_POPUP_STRING2, StrLen(CTL_POPUP_STRING2), 30, 60);
	GdiUnlockScreen(wnd);

	return 0;
}

int PopDrawPostCB2(WND wnd)
{
	GdiLockScreen(wnd);

	GdiTextOut(gpWND2, CTL_POPUP_STRING1, StrLen(CTL_POPUP_STRING1), 30, 40);
	GdiTextOut(gpWND2, CTL_POPUP_STRING4, StrLen(CTL_POPUP_STRING4), 30, 90);
	GdiTextOut(gpWND2, CTL_POPUP_STRING6, StrLen(CTL_POPUP_STRING6), 30, 120);
	GdiTextOut(gpWND2, CTL_POPUP_STRING7, StrLen(CTL_POPUP_STRING7), 30, 135);
	GdiTextOut(gpWND2, CTL_POPUP_STRING8, StrLen(CTL_POPUP_STRING8), 30, 150);
	GdiUnlockScreen(wnd);
	
	g_IsStart = TRUE;

	return 0;
}

int PopDrawPostCB3(WND wnd)
{
	REXY_CTL_POPUP *pPop;
	COLORVAL oldColor;
	
	pPop = (REXY_CTL_POPUP*)CtlGetStruct(wnd);
	
	RscLoadBitmap(MGL_preMM_Alarm01_I, &g_popBmp);
	
	GdiLockScreen(wnd);
	GdiDrawBitmap(wnd, &g_popBmp, 0, 0, pPop->win_rect.left+1, pPop->win_rect.top+1, 
				  pPop->win_rect.right - pPop->win_rect.left-1, pPop->win_rect.bottom - pPop->win_rect.top-1);
	oldColor = GdiSetBkColor(wnd, COLOR_TRANSPARENT);
	GdiSetFgColor(wnd, COLOR_BLACK);
	GdiTextOut(wnd, CTL_POPUP_STRING5, StrLen(CTL_POPUP_STRING5), 35, 35);
	GdiUnlockScreen(wnd);

	return 0;
}

void PopupCreateCallback(WND wnd)
{


}

void PopupDrawCallback(WND wnd)
{
	GdiLockScreen(wnd);
	GdiTextOut(wnd, CTL_POPUP_STRING1, StrLen(CTL_POPUP_INFO_00), 30, 30);
	GdiUnlockScreen(wnd);
}



WND PrvSamplePopupCreateConfirmPopup(WND wnd, char *str)
{
	int err;
    WND ctlWnd;

	// Create popup control
	ctlWnd = CtlCreate(wnd, CTL_TYPE_POPUP, NULL, NULL, &err);

	// Set popup property

	CtlSeti(ctlWnd, CTL_POPUP_CMD_SET_TYPE, CTL_POPUP_CONFIRM_TYPE, &err);
	CtlSeti(ctlWnd, CTL_POPUP_CMD_SET_ID, SAMPLE_POPUP_ID, &err);
	CtlSetPtr(ctlWnd,CTL_POPUP_CMD_SET_STR, str, &err);

    return ctlWnd;

}


WND PrvSamplePopupCreateQuestionPopup(WND wnd, char *str)
{
	int err;
    WND ctlWnd;

	// Create popup control
	 ctlWnd = CtlCreate(wnd, CTL_TYPE_POPUP, NULL, PrvSamplePopupCB, &err);

	// Set popup property

	CtlSeti(ctlWnd, CTL_POPUP_CMD_SET_TYPE, CTL_POPUP_QUESTION_TYPE, &err);
	CtlSeti(ctlWnd, CTL_POPUP_CMD_SET_ID, SAMPLE_POPUP_ID+1, &err);
	CtlSetPtr(ctlWnd,CTL_POPUP_CMD_SET_STR, str, &err);

    return ctlWnd;
}


WND PrvSamplePopupCreateProgressPopup(WND wnd, char *str)
{
	int err;
    WND ctlWnd;

	// Create popup control
	 ctlWnd = CtlCreate(wnd, CTL_TYPE_POPUP, NULL, NULL, &err);

	// Set popup property

	CtlSeti(ctlWnd, CTL_POPUP_CMD_SET_TYPE, CTL_POPUP_PROGRESS_TYPE, &err);
	CtlSeti(ctlWnd, CTL_POPUP_CMD_SET_ID, SAMPLE_POPUP_ID+2, &err);
	CtlSeti(ctlWnd, CTL_POPUP_CMD_SET_TIMEOUT, 3000, &err);
	CtlSetPtr(ctlWnd,CTL_POPUP_CMD_SET_STR, str, &err);

    return ctlWnd;
}


int PrvSamplePopupCB(WND wnd, int msg, int wparam, int lparam, int isPre, int *pErr)
{
    static COLORVAL fcolor, bkcolor, boxcolor;

    if(isPre)
    {
        if(msg == AWM_PAINT)
        {
	        CtlSeti(wnd, CTL_POPUP_CMD_SET_FGCOLOR, COLOR_BLUE, pErr);
	        CtlSeti(wnd, CTL_POPUP_CMD_SET_BGCOLOR, COLOR_GRAY, pErr);
	        CtlSeti(wnd, CTL_POPUP_CMD_SET_BOXCOLOR, COLOR_ORANGE, pErr);
        }
    }

    // 다음 이벤트 처리를 위해 FALSE를 리턴 해야만 한다 
    return FALSE;
}

void PrvSamplePopupDrawPopup(WND wnd)
{
	int err;

	CtlDo(wnd, CTL_POPUP_CMD_DO_DRAW, 0, 0, &err);
}

void CreateQuestionPopup(WND wnd)
{

}

void CreateProgressPopup(WND wnd)
{

}

void CreateSamplePop1(WND hWnd)
{
	int err;
	WND wnd;
	rect rtWin;

	// Create popup control
	wnd = CtlCreate(hWnd, CTL_TYPE_POPUP, NULL, NULL, &err);

	// Set popup property, Draw popup
	gpWND1 = wnd;

	GdiSetRect(&rtWin, 0, 0, 200, 150);
	CtlSetPtr(gpWND1, CTL_POPUP_CMD_SET_RECT, (void*)&rtWin, &err);

	CtlSeti(gpWND1, CTL_POPUP_CMD_SET_FGCOLOR, COLOR_BLUE, &err);
	CtlSeti(gpWND1, CTL_POPUP_CMD_SET_BGCOLOR, COLOR_GRAY, &err);
	CtlSeti(gpWND1, CTL_POPUP_CMD_SET_BOXCOLOR, COLOR_ORANGE, &err);

	CtlSetPtr(gpWND1, CTL_POPUP_CMD_SET_CB_DRAW, (void*)PopDrawPostCB1, &err);
}

void CreateSamplePop2(WND hWnd)
{
	int err;
	WND wnd;
	rect rtWin;

	// Create popup control
	wnd = CtlCreate(hWnd, CTL_TYPE_POPUP, NULL, NULL, &err);

	// Set popup property, Draw popup
	gpWND2 = wnd;

	GdiSetRect(&rtWin, 10, 10, 220, 200);
	CtlSetPtr(gpWND2, CTL_POPUP_CMD_SET_RECT, (void*)&rtWin, &err);
	
	CtlSeti(gpWND2, CTL_POPUP_CMD_SET_TYPE, CTL_POPUP_CONFIRM_TYPE, &err);
	CtlSeti(gpWND2, CTL_POPUP_CMD_SET_FGCOLOR, COLOR_BLUE, &err);
	CtlSeti(gpWND2, CTL_POPUP_CMD_SET_BGCOLOR, COLOR_GRAY, &err);
	CtlSeti(gpWND2, CTL_POPUP_CMD_SET_BOXCOLOR, COLOR_ORANGE, &err);
	
	CtlSetPtr(gpWND2, CTL_POPUP_CMD_SET_CB_DRAW, (void*)PopDrawPostCB2, &err);
}

void CreateSamplePop3(WND hWnd)
{
	int err;
	WND wnd;
	rect rtWin;

	// Create popup control
	wnd = CtlCreate(hWnd, CTL_TYPE_POPUP, NULL, NULL, &err);

	// Set popup property, Draw popup
	gpWND3 = wnd;

	GdiSetRect(&rtWin, 30, 30, 230, 190);
	CtlSetPtr(gpWND3, CTL_POPUP_CMD_SET_RECT, (void*)&rtWin, &err);
	
	CtlSeti(gpWND3, CTL_POPUP_CMD_SET_TYPE, CTL_POPUP_CONFIRM_TYPE, &err);
	CtlSeti(gpWND3, CTL_POPUP_CMD_SET_FGCOLOR, COLOR_BLUE, &err);
	CtlSeti(gpWND3, CTL_POPUP_CMD_SET_BGCOLOR, COLOR_GRAY, &err);
	CtlSeti(gpWND3, CTL_POPUP_CMD_SET_BOXCOLOR, COLOR_ORANGE, &err);
	
	CtlSetPtr(gpWND3, CTL_POPUP_CMD_SET_CB_DRAW, (void*)PopDrawPostCB3, &err);
}

void CloseConfirmPopup(void)
{
	int err;

	if(g_wndConfirmPopup != NULL)
	{
		CtlDestroy(g_wndConfirmPopup, &err);
		g_wndConfirmPopup = NULL;
		g_bPopupCreated = FALSE;
	}
}

void CloseQuestionPopup()
{

}

void CloseProgressPopup()
{

}

void CloseSamplePop1()
{
	int err;

	g_IsFirstPop = FALSE;

	if(gpWND1 != NULL)
	{
		CtlDestroy(gpWND1, &err);
		gpWND1 = NULL;
		g_bPopupCreated = FALSE;
	}
}

void CloseSamplePop2()
{
	int err;

	if(gpWND2 != NULL)
	{
		CtlDestroy(gpWND2, &err);
		gpWND2 = NULL;
	}
}

void CloseSamplePop3()
{
	int err;

	if(gpWND3 != NULL)
	{
		CtlDestroy(gpWND3, &err);
		gpWND3 = NULL;
	}
}

int PrvSamplePopupProc(WND wnd, int msg, int wparam, int lparam)
{
    int bHandled = FALSE;

	// Application Event Processing
	if(msg == AWM_CREATE || msg == AWM_DESTROY)
	{
		PrvSampePopupMainWinProc(wnd,msg,wparam,lparam);
		return TRUE;
	}
	
	if(WinGetSelection()!=PrvSamplePopupGetWND()) // Current Window가 Main인 경우만 처리 
		return FALSE;

	switch(msg)
	{
		case CTL_MSG_POPUP_CLOSE: // Popup이 닫혔을때 발생하는 이벤트 
			if(wparam == SAMPLE_POPUP_ID)
			{
				int i = 0;
				i = 1;
				// Sample Popup Close
                // lparam <== CTL_POPUP_OK
			}
            else
            if(wparam == SAMPLE_POPUP_ID+1)
            {
                int i = 0;
                i = lparam;
                // lparam <== CTL_POPUP_YES
                // lparam <== CTL_POPUP_NO
            }
			else
			if(wparam == SAMPLE_POPUP_ID+2)
			{
			}
		break;
				
		case AWM_PAINT:
			PrvSamplePopupDrawMainMenu(wnd);
				bHandled = TRUE;
			break;

		case AWM_KEYDOWN:
			bHandled = PrvSamplePopupProcessKey(wnd,msg,wparam,lparam);
			break;
				
		case AWM_TIMER:
			bHandled = PrvSamplePopupProcessTimer(wnd,msg,wparam,lparam);
			break;
	}
				
	return bHandled;
}



int PrvSamplePopupProcessTimer(WND wnd, int msg, int wparam, int lparam)
{
	return 0;
}

int PrvSamplePopupProcessKey(WND wnd, int msg, int wparam, int lparam)
{
	int handled = FALSE;
				
	switch(wparam) 
	{
		case VK_PAD_1:
			g_wndConfirmPopup = PrvSamplePopupCreateConfirmPopup(wnd,"Confirm Type Popup Test 1234 5678 ");
			PrvSamplePopupDrawPopup(g_wndConfirmPopup);
			handled = TRUE;
			break;
        case VK_PAD_2:
            g_wndQuestionPopup = PrvSamplePopupCreateQuestionPopup(wnd,"Question Type Popup Test Test Test");
			PrvSamplePopupDrawPopup(g_wndQuestionPopup);
			handled = TRUE;
			break;
		case VK_PAD_3:
			g_wndProgressPopup = PrvSamplePopupCreateProgressPopup(wnd, "Progress Type Popup Test  Timeout : 3 Sec");
			PrvSamplePopupDrawPopup(g_wndProgressPopup);
			handled = TRUE;
			break;


	}

	return handled;
}


void PrvSampleDestoryWin(WND wnd)
{

}			


void PrvSampePopupMainWinProc(WND wnd,int msg, int wparam, int lparam)
{
	if(msg == AWM_CREATE)
		PrvSamplePopupInitMainWnd(wnd);
	else
	if(msg == AWM_DESTROY)
		PrvSamplePopupDestoryMainWnd(wnd);
}



void PrvSamplePopupInitMainWnd(WND wnd)
{

}

void PrvSamplePopupDestoryMainWnd(WND wnd)
{
	int err = CTLERR_NONE;
	if (gpWND1) CtlDestroy(gpWND1, &err);
	if (gpWND2) CtlDestroy(gpWND2, &err);
}


WND PrvSamplePopupGetWND(void)
{
	return gpMainWnd; 
}

void PrvSamplePopupSetWND(WND wnd)
{
	gpMainWnd = wnd; 
}


int SamplePopupProc(WND wnd, int msg, int wparam, int lparam)
{
	WND newWin;
	int ScreenWidth, ScreenHeight;
	int bHandled = FALSE;
    switch(msg)
    {
    case AWM_CREATE:
		GdiGetScreenInfo( &ScreenWidth, &ScreenHeight );
		newWin = WinCreateWindow(PrvSamplePopupProc, 0, 0, ScreenWidth, ScreenHeight, WND_PANEL);
		ASSERT(newWin);
		PrvSamplePopupSetWND(newWin);
		WinUpdateWindow(newWin, ERASE_BKGND);
		bHandled = TRUE;
        break;
	case AWM_DESTROY:
		WinDestroyWindow(wnd);
		bHandled = TRUE;
		break;
	}
    return bHandled;
}
