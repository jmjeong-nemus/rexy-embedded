/*   
 * * $Id: rexy_ctl_anibmp.c 2006-07-10 18:50 Hakeya $
 * Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.    
 *   
 * This file is a part of Rexy Embedded.    
 *   
 */

/** @file */

/** @addtogroup AniBmpCtlGroup Ctrl Animate Bitmap Control
 * This is the Animate Bitmap Control
 * @{
 */
#include "rexy_haldevice.h"
#include "rexy_ctl_anibmp.h"

//#define TRACELEVEL	TR_LEVEL1
//#define TRACESELECT	TR_RexyControl

/**
* @brief		Create Animation Bitmap control. When Animation Bitmap Procedure receive AWM_CREATE message, calls this fuction.
* @param[in]	wnd		Window handle
* @param[out]	pErr	Pointer for error value
* @return		void
* @remarks		윈도우 핸들의 pData에 콘트롤 구조체를 할당하고 초기화.
*				
* @if HISTORY
*		 19-JUL-2006 hakeya Initiated
* @endif
*/
void PrvCtlAniBmpCreate(WND wnd, int *pErr)
{
	REXY_CTL_ANIBMP *pAniBmpCtl;

	TRACE(TR_RexyControl, TR_LEVEL3, ("+++PrvCtlAniBmpCreate+++\n"));

	//Allocate memory for anibmp control structure.
	TRACE(TR_RexyControl, TR_LEVEL2, ("Allocate memory for REXY_CTL_ANIBMP\n"));
    wnd->pData = MemAlloc(sizeof(REXY_CTL_ANIBMP));
    if(wnd->pData == NULL)
    {
		TRACE(TR_RexyControl, TR_LEVEL2, ("Failed to allocate memory for REXY_CTL_ANIBMP\n"));
		WinDestroyControl(wnd);
		wnd = NULL;
        return ;
    }

	pAniBmpCtl = (REXY_CTL_ANIBMP *)CtlGetStruct(wnd);

    //Initialize memory allocated - popup control.
	TRACE(TR_RexyControl, TR_LEVEL2, ("Initialize memory\n"));
    MemSet(pAniBmpCtl, 0, sizeof(REXY_CTL_ANIBMP));

	// Set default property value
	TRACE(TR_RexyControl, TR_LEVEL2, ("Initialize properties\n"));
	pAniBmpCtl->nIdx = 0;
	pAniBmpCtl->nLeft = CTL_ANIBMP_DEFAULT_LEFT;
	pAniBmpCtl->nTop = CTL_ANIBMP_DEFAULT_TOP;
	pAniBmpCtl->nWidth = CTL_ANIBMP_DEFAULT_WIDTH;
	pAniBmpCtl->nHeight = CTL_ANIBMP_DEFAULT_HEIGHT;
	pAniBmpCtl->bgColor = CTL_ANIBMP_DEFAULT_BGCOLOR;

	pAniBmpCtl->nFrameNo = 0;
	pAniBmpCtl->pIDs = NULL;
	pAniBmpCtl->nPlayType = CTL_ANIBMP_DEFAULT_PLAY_MODE;
	pAniBmpCtl->nTimeout = 500;
	pAniBmpCtl->pTimeout = NULL;

	pAniBmpCtl->bIsTimer = FALSE;

	TRACE(TR_RexyControl, TR_LEVEL2, ("Initialize Done..\n"));

	TRACE(TR_RexyControl, TR_LEVEL3, ("---PrvCtlAniBmpCreate---\n"));
}

/**
* @brief		Load and Draw Bitmap Image. This function called by Timer handle function. Also, when Animation Bitmap Procedure receive AWM_PAINT message, calls this function.
* @param[in]	wnd		window handle
* @param[out]	pErr	Pointer for error value
* @return		void
* @remarks
*
* @if HISTORY
*		 19-JUL-2006 hakeya Initiated
* @endif
*/
void PrvCtlAniBmpDraw(WND wnd, int *pErr)
{
	RxBITMAP rxBmp;
	REXY_CTL_ANIBMP	*pAniBmpCtl;

	TRACE(TR_RexyControl, TR_LEVEL3, ("+++PrvCtlAniBmpDraw+++\n"));

	pAniBmpCtl = (REXY_CTL_ANIBMP *)CtlGetStruct(wnd);
	CHECK_NULLPTR_VOID(pAniBmpCtl, *pErr);	

	// load and draw bitmap image
	RscLoadBitmap(pAniBmpCtl->pIDs[pAniBmpCtl->nIdx], &rxBmp);

	GdiLockScreen(wnd);
  	
	GdiClrScreen(wnd, pAniBmpCtl->bgColor);
//	GdiSetBkColor(wnd, pAniBmpCtl->bgColor);

//	GdiDrawBitmap(wnd, &rxBmp, 0, 0, pAniBmpCtl->nLeft, pAniBmpCtl->nTop, pAniBmpCtl->nWidth, pAniBmpCtl->nHeight);
	GdiDrawBitmap(wnd, &rxBmp, 0, 0, 0, 0, pAniBmpCtl->nWidth, pAniBmpCtl->nHeight);
	TRACE(TR_RexyControl, TR_LEVEL2, ("Draw Done...\n"));

	GdiUnlockScreen(wnd);

	TRACE(TR_RexyControl, TR_LEVEL3, ("---PrvCtlAniBmpDraw---\n"));
}

/**
* @brief		Timer Event Handler. When Animation Bitmap Procedure receive AWM_TIMER message, calls this function.
* @param[in]	wnd		window handle
* @param[out]	pErr	Pointer for error value
* @return		void
* @remarks		Bitmap image를 출력하고 타이머를 시작. 다음 출력될 Index값 Set.
*
* @if HISTORY
*		 19-JUL-2006 hakeya Initiated
* @endif
*/
void PrvCtlAniBmpTimerHandler(WND wnd, int *pErr)
{
	REXY_CTL_ANIBMP *pAniBmpCtl;

	TRACE(TR_RexyControl, TR_LEVEL3, ("+++PrvCtlAniBmpTimerhandler+++\n"));

	pAniBmpCtl = (REXY_CTL_ANIBMP *)CtlGetStruct(wnd);
	CHECK_NULLPTR_VOID(pAniBmpCtl, *pErr);
	
	// Draw current image
//	CtlDraw(wnd, pErr);
	PrvCtlAniBmpDraw(wnd, pErr);

	// 타이머 설정. timeout이 0이면 배열에 있는 각각의 timeout값 사용.
	if(pAniBmpCtl->nTimeout == 0)
	{
		if(pAniBmpCtl->bIsTimer == TRUE)
			UsrKillTimer(wnd, REXY_CTL_ANIBMP_TIMER);

		UsrSetTimer(wnd, REXY_CTL_ANIBMP_TIMER, pAniBmpCtl->pTimeout[pAniBmpCtl->nIdx]);
		pAniBmpCtl->bIsTimer=TRUE;
		TRACE(TR_RexyControl, TR_LEVEL2, ("Timer is set(Refer Array)\n"));
	}
	else if(pAniBmpCtl->bIsTimer==FALSE && pAniBmpCtl->nTimeout>0)
	{
		UsrSetTimer(wnd, REXY_CTL_ANIBMP_TIMER, pAniBmpCtl->nTimeout);
		pAniBmpCtl->bIsTimer=TRUE;
		TRACE(TR_RexyControl, TR_LEVEL2, ("Timer is set(nTimeout)\n"));
	}

	// 재생 모드에 따른 다음 이미지 선택.
	switch(pAniBmpCtl->nPlayType)
	{
	case CTL_ANIBMP_PLAY_NORMAL:
		if(pAniBmpCtl->nIdx < pAniBmpCtl->nFrameNo-1)
			pAniBmpCtl->nIdx++;
		break;

	case CTL_ANIBMP_PLAY_ROTATE:
		pAniBmpCtl->nIdx++;
		pAniBmpCtl->nIdx = pAniBmpCtl->nIdx % pAniBmpCtl->nFrameNo;
		break;

	case CTL_ANIBMP_PLAY_REVERSE:
		if(pAniBmpCtl->nIdx > 0)
			pAniBmpCtl->nIdx--;
		break;

	case CTL_ANIBMP_PLAY_REVERSE_ROTATE:
		if(pAniBmpCtl->nIdx == 0)
			pAniBmpCtl->nIdx = pAniBmpCtl->nFrameNo-1;
		else
			pAniBmpCtl->nIdx--;
		break;

	case CTL_ANIBMP_PLAY_RANDOM:
		pAniBmpCtl->nIdx = rand() % pAniBmpCtl->nFrameNo;
		break;
	}
	TRACE(TR_RexyControl, TR_LEVEL3, ("---PrvCtlAniBmpTimerHandler---\n"));
}

/**
* @brief		Destroy created Animation bitmap control.
* @param[in]	wnd		window handle
* @param[out]	pErr	Pointer for error value
* @return		void
* @remarks		
* @if HISTORY
*		 19-JUL-2006 hakeya Initiated
* @endif
*/
void PrvCtlAniBmpDestroy(WND wnd, int *pErr)
{
	REXY_CTL_ANIBMP *pAniBmpCtl;

	TRACE(TR_RexyControl, TR_LEVEL3, ("+++PrvCtlAniBmpDestroy+++\n"));

    pAniBmpCtl = (REXY_CTL_ANIBMP *)CtlGetStruct(wnd);
	CHECK_NULLPTR_VOID(pAniBmpCtl, *pErr);

	// Kill timer
	if(pAniBmpCtl->bIsTimer == TRUE)
	{
		UsrKillTimer(wnd, REXY_CTL_ANIBMP_TIMER);
		pAniBmpCtl->bIsTimer = FALSE;
	}

	// Free 
    MemFree(pAniBmpCtl);
    pAniBmpCtl = NULL;

	TRACE(TR_RexyControl, TR_LEVEL3, ("---PrvCtlAniBmpDestroy---\n"));
}


/**
* @brief		Main message handler of Animation bitmap control.
* @param[in]	wnd		Window handle.
* @param[in]	nMsg	Message to handle
* @param[in]	wparam	wparam of message
* @param[in]	lparam	lparam of message
* @return		0.
* @remarks	
*
* @if HISTORY
*		 19-JUL-2006 hakeya Initiated
* @endif
*/
int CtlAniBmpProc(WND wnd, int nMsg, int wparam, int lparam)
{
	int err;
//	int nKey=0;

	REXY_CTL_ANIBMP	*pAniBmpCtl;

	TRACE(TR_RexyControl, TR_LEVEL3, ("+++CtlAniBmpProc+++\n"));

	pAniBmpCtl = (REXY_CTL_ANIBMP*)CtlGetStruct(wnd);

	switch(nMsg)
	{
	case AWM_CREATE:
		PrvCtlAniBmpCreate(wnd, &err);
		break;

	case AWM_PAINT:
		PrvCtlAniBmpDraw(wnd, &err);
		break;

	case AWM_KEYDOWN:
		if (pAniBmpCtl->pfnKeyDown != NULL )
		{
			// 유저의 콜백 함수에서 리턴값에 따라 메시지 패스 여부 결정..
			if((*pAniBmpCtl->pfnKeyDown) (wnd, wparam, &err))
				return TRUE;
		}
		return FALSE;

	case AWM_TIMER:
//		if(wparam == REXY_CTL_ANIBMP_TIMER)
			if(wnd)	PrvCtlAniBmpTimerHandler(wnd, &err);
		break;

	case AWM_DESTROY:
		PrvCtlAniBmpDestroy(wnd, &err);
		break;
    }

	TRACE(TR_RexyControl, TR_LEVEL3, ("---CtlAniBmpProc---\n"));
    return 0;
} 

/**
* @brief		Callback message handler of animation bitmap conrtol.
* @param[in]	wnd		Window handle
* @param[in]	nMsg	Message to handle
* @param[in]	wparam	wparam of message
* @param[in]	lparam	lparam of message
* @param[in]	bIsPre	Use to distinguish pre/post-callback.
* @param[out]	pErr	Pointer for error value
* @return		0.
* @remarks		message 처리 함수가 호출되기 전/후에 필요한 기능 구현 가능.
* @if HISTORY
*		 19-JUL-2006 hakeya Initiated
* @endif
*/
int CtlAniBmpCBProc(WND wnd, int nMsg, int wparam, int lparam, int bIsPre, int *pErr)
{
	TRACE(TR_RexyControl, TR_LEVEL3, ("+++CtlAniBmpCBProc+++\n"));

	TRACE(TR_RexyControl, TR_LEVEL3, ("---CtlAniBmpCBProc---\n"));
	return 0;
}

/**
* @brief		Command handler of animation bitmap control.
* @param[in]	wnd			Window handle
* @param[in]	nCommand	Command ID
* @param[in]	wparam		wparam of command
* @param[in]	lparam		lparam of command
* @param[out]	pErr		Pointer for error value
* @return		0.
* @remarks		
* @if HISTORY
*		 19-JUL-2006 hakeya Initiated
* @endif
*/
int CtlAniBmpCmdProc(WND wnd, int nCommand,int wparam, int lparam, int *pErr)
{
	REXY_CTL_ANIBMP *pAniBmpCtl;

	TRACE(TR_RexyControl, TR_LEVEL3, ("+++CtlAniBmpCmdProc+++\n"));

	pAniBmpCtl = (REXY_CTL_ANIBMP *)CtlGetStruct(wnd);
	CHECK_NULLPTR(pAniBmpCtl, 0, *pErr);

	switch(nCommand) {
	case CTL_ANIBMP_CMD_SET_LEFT :
		pAniBmpCtl->nLeft = wparam;
		break;

	case CTL_ANIBMP_CMD_SET_TOP :
		pAniBmpCtl->nTop = wparam;
		break;

	case CTL_ANIBMP_CMD_SET_WIDTH:
		pAniBmpCtl->nWidth = wparam;
		break;

	case CTL_ANIBMP_CMD_SET_HEIGHT:
		pAniBmpCtl->nHeight = wparam;
		break;

	case CTL_ANIBMP_CMD_SET_BGCOLOR:
		pAniBmpCtl->bgColor = wparam;

	case CTL_ANIBMP_CMD_SET_FRAME_NO:
		pAniBmpCtl->nFrameNo = wparam;
		break;

	case CTL_ANIBMP_CMD_SET_P_IDS:
		pAniBmpCtl->pIDs = (int*)lparam;
		break;

	case CTL_ANIBMP_CMD_SET_PLAYTYPE:
		pAniBmpCtl->nPlayType = wparam;

		// 재생 모드에 따라 처음 디스플레이 될 이미지 선택
		switch(pAniBmpCtl->nPlayType)
		{
		case CTL_ANIBMP_PLAY_NORMAL:
		case CTL_ANIBMP_PLAY_ROTATE:
			pAniBmpCtl->nIdx = 0;
			break;

		case CTL_ANIBMP_PLAY_REVERSE:
		case CTL_ANIBMP_PLAY_REVERSE_ROTATE:
			pAniBmpCtl->nIdx = pAniBmpCtl->nFrameNo -1;
			break;

		case CTL_ANIBMP_PLAY_RANDOM:
			pAniBmpCtl->nIdx = rand() % pAniBmpCtl->nFrameNo;
			break;
		}
		break;

	case CTL_ANIBMP_CMD_SET_TIMEOUT:
		pAniBmpCtl->nTimeout = wparam;
		break;

	case CTL_ANIBMP_CMD_SET_P_TIMEOUT:
		pAniBmpCtl->pTimeout = (int*)lparam;
		break;

	case CTL_ANIBMP_CMD_SET_WNDRECT:
		// 기존의 영역을 설정한 바탕색으로 채움.
		//GdiClrScreen(wnd, pAniBmpCtl->bgColor);
		CtlSetWNDRect(wnd, ((rect*)lparam)->left, ((rect*)lparam)->top, ((rect*)lparam)->right - ((rect*)lparam)->left + 1, ((rect*)lparam)->bottom - ((rect*)lparam)->top + 1, pErr);
		break;

	case CTL_ANIBMP_CMD_SET_CB_KEYDOWN:
		pAniBmpCtl->pfnKeyDown = (CtlAniBmpKeyDownCB)lparam;

	case CTL_ANIBMP_CMD_START_STOP:
		if(wparam)
		{
			if(pAniBmpCtl->bIsTimer == TRUE)
				break;

			if(pAniBmpCtl->nTimeout==0 && pAniBmpCtl->pTimeout==NULL)
				break;

			PrvCtlAniBmpTimerHandler(wnd, pErr);
				break;
		}
		else
		{
			if(pAniBmpCtl->bIsTimer == TRUE)
			{
				UsrKillTimer(wnd, REXY_CTL_ANIBMP_TIMER);
				pAniBmpCtl->bIsTimer = FALSE;
				pAniBmpCtl->nIdx = 0;
			}
		}
  }
	TRACE(TR_RexyControl, TR_LEVEL3, ("---CtlAniBmpComProc---\n"));
	return 0;
}

/** @} */ // end AniBmpCtl Group
