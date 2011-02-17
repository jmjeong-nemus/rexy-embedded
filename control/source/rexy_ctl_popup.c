/*   
 * * $Id: rexy_ctl_popup.c 491 2006-01-11 16:32:34Z madholic $
 * Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.    
 *   
 * This file is a part of Rexy Embedded.    
 *   
 */

/** @file */

/**
 * @addtogroup PopupCtlGroup Ctrl Popup Control
 * Popup Control
 * @{
 */

#include "rexy_ctl_popup.h"


	
#define POPUP_STR_CONFIRM "Press \'0\' Key"
#define POPUP_STR_QUESTION "YES[\'0\'] / NO[\'1\']"
#define POPUP_STR_PROGRESS "Press any Key"


WND gpPopup = NULL;

void PrvPopupSetRect(REXY_CTL_POPUP* pCtlPopup, rect* pRect, int* pErr);
void PrvPopupSetWnd(WND wnd, int* pErr);
void PrvPopupSetType(REXY_CTL_POPUP* pCtlPopup, int type , int *pErr);

void CtlPopupDrawString(WND wnd, REXY_CTL_POPUP *pPop);
void CtlPopupDrawConfirm(WND wnd, REXY_CTL_POPUP *pPop);
void CtlPopupDrawQuestion(WND wnd, REXY_CTL_POPUP *pPop);
void CtlPopupDrawProgress(WND wnd, REXY_CTL_POPUP *pPop);
int CtlPopupProcessDefaultKeydown(WND wnd, int wparam);

/**
* @brief		Create popup control. When Popup Procedure receive AWM_CREATE message, calls this fuction.
* @param[in]	wnd     Pointer to control window
* @return		If succeeds allocating memory for popup control structure, return the pointer to it, else NULL.
* @remarks		This function allocates memory for popup control structure and saves current screen and sets control's chain.
* @if HISTORY
*         10-Jan-2006 namgung Initiated
* @endif
*/
void CtlPopupCreate(WND wnd)
{
	REXY_CTL_POPUP *pPop;
	int screen_size=0;

    //Allocate memory for popup control structure.
    wnd->pData = MemAlloc(sizeof(REXY_CTL_POPUP));
    if(wnd->pData == NULL)
    {
		WinDestroyControl(wnd);
		wnd = NULL;
        return ;
    }

	pPop = (REXY_CTL_POPUP *)CtlGetStruct(wnd);

    //Initialize memory allocated - popup control.
    MemSet(pPop, 0, sizeof(REXY_CTL_POPUP));

	// Get screen size
	screen_size = GdiScreenGetMemSize();
	if(screen_size==0)
		return;

	// Alloc screen saver
	pPop->pScreenSaver = MemAlloc(screen_size);
	if(pPop->pScreenSaver==NULL)
		return;
	
	// Save screen before drawing popup
    MemCopy(pPop->pScreenSaver, GdiGetScreenPtr(), screen_size);

	pPop->win_rect.left = CTL_POPUP_DEFAULT_LEFT;
	pPop->win_rect.top = CTL_POPUP_DEFAULT_TOP;
	pPop->win_rect.right = CTL_POPUP_DEFAULT_RIGHT;
	pPop->win_rect.bottom = CTL_POPUP_DEFAULT_BOTTOM;

	pPop->bgColor = CTL_POPUP_COLOR_BG;
	pPop->boxColor = CTL_POPUP_COLOR_BOX;
	pPop->nTimeout = 0;
	pPop->pStr = NULL;

	WinSetSelection( wnd );
	
	//pPop->nTimeout = POPUP_DEFAULT_INTERVAL;
}


/**
* @brief		Callback function called after creating popup control.
* @param[in]	wnd     Pointer to control window
* @param[out]	*pErr	Pointer to error value
* @return		void
* @remarks		Currently does nothing.
* @if HISTORY
*         10-Jan-2006 namgung Initiated
* @endif
*/
void CtlPopupCreatePostCB(WND wnd, int *pErr)
{

}


/**
* @brief		Draw popup control. When Popup Procedure receive AWM_PAINT message, calls this fuction.
* @param[in]	wnd     Pointer to control window
* @param[out]   pErr    Pointer to error value
* @return		void
* @remarks		This function draws default popup box and sets popup timer.
* @if HISTORY
*         10-Jan-2006 namgung Initiated
*         05-Jul-2006 yich 사용 안하는 변수 주석 처리
* @endif
*/
void CtlPopupDraw(WND wnd, int *pErr)
{
	COLORVAL old_fg;
	REXY_CTL_POPUP *pPop;

	pPop = (REXY_CTL_POPUP*)CtlGetStruct(wnd);
	CHECK_NULLPTR_VOID(pPop, *pErr);

	GdiLockScreen(wnd);

	if(pPop->bgColor)
		GdiFillRect(wnd, &pPop->win_rect, pPop->bgColor);

	old_fg = GdiSetFgColor(wnd, pPop->boxColor);

    GdiRectangle(wnd, &pPop->win_rect);

    GdiSetFgColor(wnd, old_fg);

	// Defaul
	if(pPop->nType==CTL_POPUP_CONFIRM_TYPE)
	{
		CtlPopupDrawConfirm(wnd,pPop); 
	}
    else
   	if(pPop->nType==CTL_POPUP_QUESTION_TYPE)
	{
		CtlPopupDrawQuestion(wnd,pPop); 
	}
	else
	if(pPop->nType==CTL_POPUP_PROGRESS_TYPE)
	{
		CtlPopupDrawProgress(wnd,pPop);
	}

	GdiUnlockScreen(wnd);

	if(pPop->nTimeout != 0)
		UsrSetTimer(wnd, REXY_POPUPDLG_TIMER, pPop->nTimeout);

	//if(pPop->pfnDraw != NULL )
	//	(*pPop->pfnDraw)(wnd);
}

void CtlPopupDrawConfirm(WND wnd, REXY_CTL_POPUP *pPop)
{

	rect rtClient;
	GdiSetRect(&rtClient, pPop->win_rect.left, pPop->win_rect.top, pPop->win_rect.right, pPop->win_rect.bottom);
	
	if(pPop->pStr != NULL)
		CtlPopupDrawString(wnd,pPop);

	GdiDrawTextOut(wnd,rtClient.left,rtClient.bottom - (FntGetTextHeight(&FntGlobalContext) * 2),
					rtClient.right - rtClient.left, POPUP_STR_CONFIRM, StrLen(POPUP_STR_CONFIRM),ALIGN_CENTER);

}

void CtlPopupDrawQuestion(WND wnd, REXY_CTL_POPUP *pPop)
{

	rect rtClient;
	GdiSetRect(&rtClient, pPop->win_rect.left, pPop->win_rect.top, pPop->win_rect.right, pPop->win_rect.bottom);
	
	if(pPop->pStr != NULL)
		CtlPopupDrawString(wnd,pPop);

	GdiDrawTextOut(wnd,rtClient.left,rtClient.bottom - (FntGetTextHeight(&FntGlobalContext) * 2),
					rtClient.right - rtClient.left, POPUP_STR_QUESTION, StrLen(POPUP_STR_QUESTION),ALIGN_CENTER);

}

void CtlPopupDrawProgress(WND wnd, REXY_CTL_POPUP *pPop)
{

	rect rtClient;
	GdiSetRect(&rtClient, pPop->win_rect.left, pPop->win_rect.top, pPop->win_rect.right, pPop->win_rect.bottom);
	
	if(pPop->pStr != NULL)
		CtlPopupDrawString(wnd,pPop);

	GdiDrawTextOut(wnd,rtClient.left,rtClient.bottom - (FntGetTextHeight(&FntGlobalContext) * 2),
					rtClient.right - rtClient.left, POPUP_STR_PROGRESS, StrLen(POPUP_STR_PROGRESS),ALIGN_CENTER);

}
						
void CtlPopupDrawString(WND wnd, REXY_CTL_POPUP *pPop)
{
    int err;

    CHECK_NULLPTR_VOID(pPop,err);
    CHECK_NULLPTR_VOID(pPop->pStr,err); 

    {
        COLORVAL fgColor;
        fgColor = GdiSetFgColor(wnd,pPop->fgColor);

	    GdiLockScreen(wnd);
	    GdiDrawTextOutLF(wnd, pPop->win_rect.left+1, pPop->win_rect.top+1, 
					     pPop->win_rect.right - pPop->win_rect.left , pPop->pStr,
					     StrLen(pPop->pStr),ALIGN_CENTER);
	    GdiUnlockScreen(wnd);
        GdiSetFgColor(wnd, fgColor);
    }
}



/**
* @brief		Callback function called before drawing popup control.
* @param[in]	wnd     Pointer to control window
* @param[out]	pErr	Pointer to error value.
* @return		void
* @remarks		This function sets window rectangle.
* @if HISTORY
*         10-Jan-2006 namgung Initiated
*         05-Jul-2006 yich 화면 크기를 설정을 상수에서 함수로 변경
* @endif
*/
void CtlPopupDrawPreCB(WND wnd, int *pErr)
{
	REXY_CTL_POPUP *pPop;

	pPop = (REXY_CTL_POPUP*)CtlGetStruct(wnd);
	CHECK_NULLPTR_VOID(pPop, *pErr);

	CtlSetWNDRect(wnd, 
					0, 
					0, 
					FtrGetSystemFeature(FEATURE_LCD_WIDTH), 
					FtrGetSystemFeature(FEATURE_LCD_HEIGHT), 
					pErr);
	CtlSetWNDColor(wnd, pPop->fgColor, pPop->bgColor, pErr);
	
}


/**
* @brief		Callback function called after drawing popup control.
* @param[in]	wnd     Pointer to control window
* @param[out]	pErr	Pointer to error value
* @return		void
* @if HISTORY
*         10-Jan-2006 namgung Initiated
* @endif
*/
void CtlPopupDrawPostCB(WND wnd, int *pErr)
{
	
}


/**
* @brief		Handles popup control's common key down event. 
* @param[in]	wnd     Pointer to control window
* @param[in]	wparam	key type
* @return		Returns a result value.
* @if HISTORY
*         10-Jan-2006 namgung Initiated
* @endif
*/
int CtlPopupKeydown(WND wnd, int wparam)
{
	REXY_CTL_POPUP *pPop;
	
	int ret;

	int bHandled;

	pPop = (REXY_CTL_POPUP*)CtlGetStruct(wnd);

	if (pPop->pfnKeyDown != NULL)
	{
		ret = (*pPop->pfnKeyDown)(wnd, wparam);
		
		if (ret)
			bHandled = TRUE;
		else
			bHandled = FALSE;
	}
	else
		bHandled = FALSE;

	// Default 처리 루틴 
	if(bHandled == FALSE)
		bHandled = CtlPopupProcessDefaultKeydown(wnd, wparam);

	

	return bHandled;

}

/**
* @brief		Destroy created popup control. 
* @param[in]	wnd     Pointer to control window
* @param[in]	wparam	key type
* @return		handled (TRUE/FALSE)
*/
int CtlPopupProcessDefaultKeydown(WND wnd, int wparam)
{
	REXY_CTL_POPUP *pPop;
	int err;
	int bHandled = FALSE;

	pPop = (REXY_CTL_POPUP*)CtlGetStruct(wnd);

    switch(pPop->nType)
    {
        case CTL_POPUP_CONFIRM_TYPE:
		    if(wparam == VK_PAD_0) // Defalut Confirm Type은 0을 누르면 닫힌다 
		    {
			    pPop->nReason = CTL_POPUP_OK;
			    CtlDestroy(wnd,&err);
			    bHandled = TRUE; 
		    }
            break;
        case CTL_POPUP_QUESTION_TYPE:
            if(wparam == VK_PAD_0) // Defalut Confirm Type은 0을 누르면 닫힌다 
		    {
			    pPop->nReason = CTL_POPUP_YES;
			    CtlDestroy(wnd,&err);
			    bHandled = TRUE; 
		    }
            else
            if(wparam == VK_PAD_1) // Defalut Confirm Type은 0을 누르면 닫힌다 
		    {
			    pPop->nReason = CTL_POPUP_NO;
			    CtlDestroy(wnd,&err);
			    bHandled = TRUE; 
		    }
			break;
		case CTL_POPUP_PROGRESS_TYPE:
			pPop->nReason = CTL_POPUP_CANCEL;
			CtlDestroy(wnd, &err);
			bHandled = TRUE;
			break;
    }

	return bHandled;
}

/**
* @brief		Destroy created popup control. 
* @param[in]	wnd     Pointer to control window
* @param[out]	pErr	Pointer to error value
* @return		void
*/
void CtlPopupDestroy(WND wnd, int *pErr)
{
	REXY_CTL_POPUP *pPop;
	//REXY_CTL *pNextCtl;
	WND pNextCtl;
	int err;
	REXY_CTL_POPUP *pNextPop;

    pPop = (REXY_CTL_POPUP*)CtlGetStruct(wnd);
	CHECK_NULLPTR_VOID(pPop, *pErr);

	GdiLockScreen(wnd);

	// Restore screen
	MemCopy(GdiGetScreenPtr(), pPop->pScreenSaver, GdiScreenGetMemSize());
	//GdiWinRestoreBits();

	// Redraw next control
	pNextCtl = wnd->nextwin;
	if(pNextCtl)
	{
		pNextPop = (REXY_CTL_POPUP*)CtlGetStruct(pNextCtl);
		MemCopy(pNextPop->pScreenSaver, pPop->pScreenSaver, GdiScreenGetMemSize());
	}

	while (pNextCtl) {
		CtlDraw(pNextCtl, &err);
		pNextCtl = pNextCtl->nextwin;
	};

	if(wnd->prevwin)
	{
		if(wnd->nextwin)
		{
			wnd->nextwin->prevwin = wnd->prevwin;
			wnd->prevwin->nextwin = wnd->nextwin;
		}
		else
			wnd->prevwin->nextwin = NULL;
	}

	GdiUnlockScreen(wnd);

	// Kill popup timer
	if(pPop->nTimeout != 0)
		UsrKillTimer(wnd, REXY_POPUPDLG_TIMER);

	// Free popup
//	MemFree(pPop->pScreenSaver);
    MemFree(pPop);
    pPop = NULL;

}


/** 
* @brief        Call back function called before destroying popup control.
* @param[in]    wnd     Pointer to control window
* @param[out]   pErr            Pointer to error value
* @remarks      Currently does nothing.
* @if HISTORY
*         10-Jan-2006 namgung Initiated
* @endif
*/
void CtlPopupDestroyPreCB(WND wnd, int *pErr)
{
	REXY_CTL_POPUP *pPop;
	pPop = (REXY_CTL_POPUP*)CtlGetStruct(wnd);
	CHECK_NULLPTR_VOID(pPop, *pErr);
	MsgPostMessage(wnd->parentwin, CTL_MSG_POPUP_CLOSE, pPop->nId,pPop->nReason);
}


/** 
* @brief        Event loop for modal popup control.
* @param[in]	wnd		window handle
* @param[out]   pErr    Pointer to error value
* @return		Returns exit value
* @remarks      
* @if HISTORY
*         10-Jan-2006 namgung Initiated
* @endif
*/
int CtlPopupEventloop(WND wnd, int *pErr)
{
	msg nMsg;

	pErr = NULL;

	 while(MsgGetMessageCtrl(wnd, &nMsg, (int)CtlPopupEventloop))
    {
        int ret;
        ret = MsgDispatchSystemMessage(&nMsg);
        if( !ret )
        {
            MsgDispatchMessage(&nMsg);
        }
		
		// Deliver event to control core
		if(nMsg.wmsg != AWM_CREATE && nMsg.wmsg != AWM_DESTROY)
		{
			 CtlProcessEvent(wnd, pErr, nMsg.wmsg, nMsg.wparam, nMsg.lparam);
			 if(*pErr == CTL_POPUP_EXIT)
				 return 0;
		}

    }
	 return 0;
}


/** 
* @brief        Main message handler of popup control.
* @param[in]    wnd			Pointer to control window
* @param[in]    msg        Message to handle
* @param[in]    wparam      wparam of message
* @param[in]    lparam      lparam of message
* @retval       If nothing to handle or no error occurred, return 0
* @retval       REXY_CTL*   If message was AWM_CREATE and control was successfully created
* @remarks      This function handles messages for popup control.\n
*               This function is called by control core if some messages is needed to be handled by popup control.
* @if HISTORY
*         10-Jan-2006 namgung Initiated
* @endif
*/
int CtlPopupProc(WND wnd, int msg, int wparam, int lparam)
{
	int pErr;
    
	int bHandled = FALSE;

    switch(msg)
    {
    case AWM_CREATE:
		//Allocate memory for popup control structure.
		CtlPopupCreate(wnd);
        break;
	case AWM_DESTROY:
		CtlPopupDestroy(wnd, &pErr);
		break;
    case AWM_PAINT:
		CtlPopupDraw(wnd, &pErr);
        break;
	case AWM_SETFOCUS:
		break;
	case AWM_KILLFOCUS:
		break;
    case AWM_KEYDOWN:
		CtlPopupKeydown(wnd, wparam);
		bHandled = TRUE;
       break;
	case AWM_TIMER:
		//CtlPopupDestroy(wnd, &pErr);
		CtlDestroy(wnd, &pErr);
		bHandled = TRUE;
		break;

    default:
		CtlPopupEventloop(wnd, &pErr);
        break;
    }

    return bHandled;
}


/** 
* @brief        Callback message hanlder of popup control.
* @param[in]    wnd			Pointer to control window
* @param[in]    nMsg        Message to handle
* @param[in]    wparam      wparam of message
* @param[in]    lparam      lparam of message
* @param[in]    bIsPre      0 if functions is called for pre-callback\n
*                           Besides 0 if function called for post-callback
* @param[out]   pErr        Pointer to error value
* @return       If nothing to handle or no error occurred, returns 0.
* @remarks      This callback function called after handling some messages.
* @if HISTORY
*         10-Jan-2006 namgung Initiated
* @endif
*/
int CtlPopupCBProc(WND wnd, int nMsg, int wparam, int lparam, int bIsPre, int *pErr)
{
    switch(nMsg)
    {
    case AWM_CREATE:
        CtlPopupCreatePostCB(wnd, pErr);
        return 0;

    case AWM_DESTROY:
        CtlPopupDestroyPreCB(wnd, pErr);
        return 0;

	case AWM_PAINT:
		if(bIsPre)
			CtlPopupDrawPreCB(wnd, pErr);
		else
			CtlPopupDrawPostCB(wnd, pErr);
		return 0;

    default:
        return 0;
    }
}


/** 
* @brief        Command handler of popup control.
* @param[in]    wnd			Pointer to control window
* @param[in]    nCommand    Command ID
* @param[in]    wparam		wparam of command
* @param[in]    lparam		lparam of command
* @param[out]   pErr		Pointer to error value
* @return       Varies by command ID
* @remarks      This function commands to popup control to do various functions.
*               By command ID popup control set its inner member value or,
*               returns its inner member value to whom it called.
*               Also does some functions such as drawing by specified command ID.
* @if HISTORY
*         10-Jan-2006 namgung Initiated
* @endif
*/
int CtlPopupCmdProc(WND wnd, int nCommand,int wparam, int lparam, int *pErr)
{
	REXY_CTL_POPUP *pPop;

	pPop = (REXY_CTL_POPUP*)CtlGetStruct(wnd);
	CHECK_NULLPTR(pPop, 0, *pErr);

	switch(nCommand) {
	case CTL_POPUP_CMD_SET_TYPE:
        PrvPopupSetType(pPop,wparam,pErr);
		pPop->nType = wparam;
		break;
	case CTL_POPUP_CMD_SET_RECT:
		PrvPopupSetRect(pPop, (rect*)lparam, pErr);
		PrvPopupSetWnd(wnd, pErr);
		break;
	case CTL_POPUP_CMD_SET_FGCOLOR:
		pPop->fgColor = wparam;
		break;
	case CTL_POPUP_CMD_SET_BGCOLOR:
		pPop->bgColor = wparam;
		break;
	case CTL_POPUP_CMD_SET_BOXCOLOR:
		pPop->boxColor = wparam;
		break;
	case CTL_POPUP_CMD_SET_TIMEOUT:
		pPop->nTimeout = wparam;
		break;
	case CTL_POPUP_CMD_GET_RECT:
		return (int)&pPop->win_rect;
	case CTL_POPUP_CMD_SET_STR:
		if(lparam == 0)
			*pErr = CTLERR_NULLPTR;
		else
			pPop->pStr = (char *)lparam;
		break;
	case CTL_POPUP_CMD_GET_STR:
		return (int)pPop->pStr;

	case CTL_POPUP_CMD_SET_ID:
		pPop->nId = wparam;
		break;


	case CTL_POPUP_CMD_DO_DRAW:
		CtlDraw(wnd, pErr);
		break;
	case CTL_POPUP_CMD_SET_CB_DRAW:
		pPop->pfnDraw = (CtlPopupDrawCB)lparam;
		break;
	case CTL_POPUP_CMD_SET_CB_KEYDOWN:
		pPop->pfnKeyDown = (CtlPopupKeyDownCB)lparam;
		break;
	}

	return 0;
}

void PrvPopupSetType(REXY_CTL_POPUP* pCtlPopup, int type , int *pErr)
{
    CHECK_NULLPTR_VOID(pCtlPopup, *pErr);    
    switch(type)
    {
        case CTL_POPUP_QUESTION_TYPE:
        case CTL_POPUP_CONFIRM_TYPE:
            //  Default Color 설정 
            pCtlPopup->bgColor = CTL_POPUP_COLOR_BG;
            pCtlPopup->fgColor = CTL_POPUP_COLOR_FG;
            pCtlPopup->boxColor = CTL_POPUP_COLOR_BOX;
            break;
    }
}

void PrvPopupSetRect(REXY_CTL_POPUP* pCtlPopup, rect* pRect, int* pErr)
{
	pCtlPopup->win_rect.left = pRect->left;
	pCtlPopup->win_rect.top = pRect->top;
	pCtlPopup->win_rect.right = pRect->right;
	pCtlPopup->win_rect.bottom = pRect->bottom;
}

void PrvPopupSetWnd(WND wnd, int* pErr)
{
	REXY_CTL_POPUP* pPopup = (REXY_CTL_POPUP*)CtlGetStruct(wnd);

    CtlSetWNDRect(wnd, pPopup->win_rect.left, pPopup->win_rect.top, 
			      (pPopup->win_rect.right - pPopup->win_rect.left),
				  (pPopup->win_rect.bottom - pPopup->win_rect.top), 
				  pErr);
}

/****************************************************************************/
/*  Unit Test Module                                                        */
/****************************************************************************/
#ifdef UNIT_TEST
#define UNIT_TEST_ERR(err, n, str)  if(err != RxErrNone) \
                                    { TrcPrintF("UNIT TEST #%d : %s : FAILED\n", n, str); return; } \
                                    else \
                                    { TrcPrintF("UNIT TEST #%d : %s : OK\n", n, str); n++; }

#define UNIT_TEST_TYPE		CTL_POPUP_CONFIRM_TYPE
#define UNIT_TEST_TOP		20
#define UNIT_TEST_LEFT		20
#define UNIT_TEST_RIGHT		200
#define UNIT_TEST_BOTTOM	200
#define UNIT_TEST_FGCOLOR	COLOR_BLACK
#define UNIT_TEST_BGCOLOR	CTL_POPUP_COLOR_BG
#define UNIT_TEST_BOXCOLOR	CTL_POPUP_COLOR_BOX
#define UNIT_TEST_TIMEOUT	0

void CtlPopupUnitTest(WND hWnd)
{
    WND wnd;
    int err;

    int nTestNum = 1;

    //Create
    wnd = CtlCreate(hWnd, CTL_TYPE_POPUP, 0, NULL, &err);
    UNIT_TEST_ERR(err, nTestNum, "Create");

    //Set : popup type
    CtlSeti(wnd, CTL_POPUP_CMD_SET_TYPE, UNIT_TEST_TYPE, &err);
    UNIT_TEST_ERR(err, nTestNum, "Set : popup type");

    //Set : top
    CtlSeti(wnd, CTL_POPUP_CMD_SET_TOP, UNIT_TEST_TOP, &err);
    UNIT_TEST_ERR(err, nTestNum, "Set : top");

    //Set : left
    CtlSeti(wnd, CTL_POPUP_CMD_SET_LEFT, UNIT_TEST_LEFT, &err);
    UNIT_TEST_ERR(err, nTestNum, "Set : left");

    //Set : right
    CtlSeti(wnd, CTL_POPUP_CMD_SET_RIGHT, UNIT_TEST_RIGHT, &err);
    UNIT_TEST_ERR(err, nTestNum, "Set : right");

    //Set : bottom
    CtlSeti(wnd, CTL_POPUP_CMD_SET_BOTTOM, UNIT_TEST_BOTTOM, &err);
    UNIT_TEST_ERR(err, nTestNum, "Set : bottom");

	//Set : foreground color
    CtlSeti(wnd, CTL_POPUP_CMD_SET_FGCOLOR, UNIT_TEST_FGCOLOR, &err);
    UNIT_TEST_ERR(err, nTestNum, "Set : foreground color");

	//Set : background color
    CtlSeti(wnd, CTL_POPUP_CMD_SET_BGCOLOR, UNIT_TEST_BGCOLOR, &err);
    UNIT_TEST_ERR(err, nTestNum, "Set : background color");

	//Set : box color
    CtlSeti(wnd, CTL_POPUP_CMD_SET_BOXCOLOR, UNIT_TEST_BOXCOLOR, &err);
    UNIT_TEST_ERR(err, nTestNum, "Set : box color");

	//Set : time out
    CtlSeti(wnd, CTL_POPUP_CMD_SET_TIMEOUT, UNIT_TEST_TIMEOUT, &err);
    UNIT_TEST_ERR(err, nTestNum, "Set : time out");

    //Destroy
    CtlDestroy(wnd, &err);
    UNIT_TEST_ERR(err, nTestNum, "Destroy");
}
#endif

/** @} */ // End of Popup Control
