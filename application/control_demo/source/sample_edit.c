/* 
* $Id: sample_list.c 491 2006-07-13 00:26:34Z yich $
* Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.    
*   
* This file is a part of Rexy Embedded
*   
*/

#include "rexy.h"
#include "rexy_ctl_core.h"
#include "rexy_ctl_edit.h"
#include "application.h"
#include "../header/sample_edit.h"

//
//
//
static WND PrvSampleEditCreate(WND hWnd)
{
    int nErr;
	WND hCtlWnd;
	char *sampleTxt = "Nemustech RexyE(TM)\nEdit control test\nHello world!!!\n";
    rect sWndRect = {20, 20, 220, 160};

    hCtlWnd = CtlCreate(hWnd, CTL_TYPE_EDIT, 0, NULL, &nErr);
	if (hCtlWnd == NULL)
		return NULL;

    CtlSetPtr(hCtlWnd, CTL_EDIT_CMD_SET_WND_RECT, (void*)&sWndRect, &nErr);
	CtlSetPtr(hCtlWnd, CTL_EDIT_CMD_SET_STRING, (void*)sampleTxt, &nErr);
	CtlSeti(hCtlWnd, CTL_EDIT_CMD_SET_MULTILINE, TRUE, &nErr);

	return hCtlWnd;
}

static void PrvSampleEditDraw(WND hCtlWnd)
{
    int nErr;

    CtlDo(hCtlWnd, CTL_EDIT_CMD_DO_DRAW, 0, 0, &nErr);
}

static int PrvSampleEditDestroy(WND hCtlWnd, int nMsg, int wParam, int lParam)
{
	int nErr;

	CtlDestroy(	hCtlWnd, &nErr);

	return 0;
}


static int PrvEditMainProc (WND hWnd, int nMsg, int wParam, int lParam)
{

	WND hCtlWnd;

	int bHandled = FALSE;
	
	switch(nMsg)
	{
	case AWM_CREATE:
		GdiClrScreen(hWnd, COLOR_MAGENTA);
		hCtlWnd = PrvSampleEditCreate(hWnd);
		PrvSampleEditDraw(hCtlWnd);
		break;
	
	case AWM_DESTROY:
		break;
	
	case AWM_PAINT:
		break;

	case AWM_KEYDOWN:
		switch(wParam)
		{
		case VK_PAD_9:
			StaGotoState(hWnd, STATE_ID_CTL_TEST, MSG_DESTROY, MSG_CREATE);
			bHandled = TRUE;
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}

    return bHandled;
}		


/** 
 * @brief       Edit control testing program.
 * @param[in]   hWnd	Window handle for application
 * @param[in]	nMsg	Message that be processed	
 * @param[in]	wParam	Message Parameter 1
 * @param[in]	lParam	Message Parameter 2
 * @return		
 * @if HISTORY
 *				13-Jun-2006 yich Initiated
 * @endif
 */

int SampleEditProc(WND wnd, int nMsg, int wParam, int lParam)
{
	WND hCtlWnd;

	int	ScreenWidth, ScreenHeight;	

	int bHandled = FALSE;

    switch(nMsg)
    {
    case AWM_CREATE:
		// Create Window for this application
		GdiGetScreenInfo( &ScreenWidth, &ScreenHeight ); // We are going to use whole screen
		hCtlWnd		= WinCreateWindow( PrvEditMainProc, 0, 0, ScreenWidth, ScreenHeight, WND_PANEL );
		
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
