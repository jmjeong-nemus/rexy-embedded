/*
 * * $Id: rexy_ctl_slidetext.c 491 2006-07-07 11:11:11Z wijang $
 * Copyright(c) 2008, Nemustech Inc, All Rights Reserved.    
 *   
 * This file is a part of Rexy Embedded.    
 *   
 */

/** @file */

/**
 * @addtogroup SlideTextCtlGroup Ctrl Slide Text Control
 * Ctrl Slide Text Control
 * @{
 */

#include "rexy_ctl_slidetext.h"

//#define TRACESELECT						TR_RexyControl
//#define TRACELEVEL						TR_LEVEL3

/**
* @brief		Create a new slide text control	structur		
* @param		wnd		Pointer to create a new control window
* @return		void
* @remarks		This function creates new text control window and allocates memory for text control structure
*				and initiate its default value
* @if HISTORY
*         20-July-2006 wijang Initiated
* @endif
*/
void PrvCtlSlideTextCreate(WND wnd)
{
    REXY_CTL_SLIDETEXT *pCtlSlideText;

    //Allocate memory for slidetext control structure.
    wnd->pData = MemAlloc(sizeof(REXY_CTL_SLIDETEXT));

    TRACE(TR_RexyControl, TR_LEVEL3, ("PrvCtlSlideTextCreate(hWnd=0x%x) Called\n", wnd));

    if(wnd->pData == NULL)
    {
        WinDestroyControl(wnd);
        wnd = NULL;
        TRACE(TR_RexyControl, TR_LEVEL3, ("PrvCtlSlideTextCreate(hWnd=0x%x) Return\n", wnd));
        return;
    }

    pCtlSlideText = (REXY_CTL_SLIDETEXT*)CtlGetStruct(wnd);

    //Initialize memory allocated - slidetext control.
    MemSet(pCtlSlideText, 0, sizeof(REXY_CTL_SLIDETEXT));

    //Set default values to list control structure.
    pCtlSlideText->sText.nX				= CTL_SLIDETEXT_DEFAULT_X;
    pCtlSlideText->sText.nY				= CTL_SLIDETEXT_DEFAULT_Y;
    pCtlSlideText->sText.nFgColor		= CTL_SLIDETEXT_COLOR_FG;

    pCtlSlideText->sBg.nLeft			= CTL_SLIDETEXT_DEFAULT_TOP;
    pCtlSlideText->sBg.nTop				= CTL_SLIDETEXT_DEFAULT_LEFT;
    pCtlSlideText->sBg.nRight			= CTL_SLIDETEXT_DEFAULT_RIGHT;
    pCtlSlideText->sBg.nBottom			= CTL_SLIDETEXT_DEFAULT_BOTTOM;
    pCtlSlideText->sBg.nBgColor			= CTL_SLIDETEXT_COLOR_BG;

    pCtlSlideText->nType				= CTL_SLIDETEXT_DEFAULT_TYPE;
    pCtlSlideText->nTimeout				= CTL_SLIDETEXT_DEFAULT_TIMEOUT;
    pCtlSlideText->nSSFlag				= CTL_SLIDETEXT_DEFAULT_SSFLAG;
    pCtlSlideText->nFEFlag				= CTL_SLIDETEXT_DEFAULT_FEFLAG;
    pCtlSlideText->nMaxWidth			= CTL_SLIDETEXT_DEFAULT_MAXWIDTH;

    pCtlSlideText->sPrv.nCurrWidth		= CTL_SLIDETEXT_DEFAULT_CURRWIDTH;
    pCtlSlideText->sPrv.nCurrMaxWidth	= CTL_SLIDETEXT_DEFAULT_CURRMAXWIDTH;
    pCtlSlideText->sPrv.nCurrX			= CTL_SLIDETEXT_DEFAULT_CURRX;


    TRACE(TR_RexyControl, TR_LEVEL3, ("PrvCtlSlideTextCreate() Return\n", 0));
    return;
}


/**
* @brief		Call back function called after creating list control.		
* @param[in]	wnd		Pointer to slide text control window
* @param[out]	pErr	Pointer to error value
* @return		void
* @remarks		Currently does nothing
* @if HISTORY
*         20-July-2006 wijang Initiated
* @endif
*/
void PrvCtlSlideTextCreatePostCB(WND wnd, int *pErr)
{
    TRACE(TR_RexyControl, TR_LEVEL3, ("PrvCtlSlideTextCreatePostCB(hWnd=0x%x, *pErr=%d) Called\n", wnd, *pErr));
    TRACE(TR_RexyControl, TR_LEVEL3, ("PrvCtlSlideTextCreatePostCB() Return\n", 0));
    return;
}

/**
* @brief		Text shift handler function
* @param[in]	wnd		Pointer to create a new control window
* @param[out]   pErr	Pointer to error value
* @return		void
* @remarks		shift a text to left side  or round after slide timer expire in its timeout value\n
*				divide into fore shifters\n
*				- left  to front
*				- left  to round
*				- right to left
*				- right to round
* @if HISTORY
*         20-July-2006 wijang Initiated
* @endif
*/
void PrvSlideCtlTextHandler (WND wnd, int *pErr)
{
    rint16 nCurrWidth;
    rect rtClient;

    REXY_CTL_SLIDETEXT *pCtlSlideText;

    TRACE(TR_RexyControl, TR_LEVEL3, ("PrvSlideCtlTextHandler(hWnd=0x%x, *pErr=%d) Called\n", wnd, *pErr));

    pCtlSlideText = (REXY_CTL_SLIDETEXT*)CtlGetStruct(wnd);
    CHECK_NULLPTR_VOID(pCtlSlideText, *pErr);


    nCurrWidth = FntGetTextWidth(&FntGlobalContext, pCtlSlideText->pStr, StrLen( pCtlSlideText->pStr));

    GdiLockScreen(wnd);

    switch (pCtlSlideText->nType)
    {
    case CTL_SLIDETEXT_TYPE_LFRONT:
        if (pCtlSlideText->nSSFlag == CTL_SLIDETEXT_MOVE_PLAY)
        {
            if ( pCtlSlideText->pfnDraw != NULL )
                (*pCtlSlideText->pfnDraw) (wnd, pCtlSlideText->sText.nFgColor, pCtlSlideText->sBg.nBgColor);

            switch ( pCtlSlideText->nFEFlag )
            {
            case CTL_SLIDETEXT_FRONT:
                if ( pCtlSlideText->sPrv.nCurrWidth >= 0)
                {
                    GdiTextOut(wnd, pCtlSlideText->pStr, StrLen(pCtlSlideText->pStr), (pCtlSlideText->sPrv.nCurrX += -1), pCtlSlideText->sText.nY);
                    pCtlSlideText->sPrv.nCurrWidth--;
                }
                else
                {
                    GdiSetRect(&rtClient, 0, 0, pCtlSlideText->sBg.nRight, pCtlSlideText->sBg.nBottom - pCtlSlideText->sBg.nTop );
                    GdiFillRect(wnd, &rtClient, pCtlSlideText->sBg.nBgColor);
                    GdiTextOut(wnd, pCtlSlideText->pStr, StrLen(pCtlSlideText->pStr), pCtlSlideText->sText.nX, pCtlSlideText->sText.nY);
                    pCtlSlideText->sPrv.nCurrWidth = nCurrWidth;
                    pCtlSlideText->sPrv.nCurrX = pCtlSlideText->sText.nX;
                }
                break;
            case CTL_SLIDETEXT_END:
                if ( pCtlSlideText->sPrv.nCurrWidth >= 0)
                {
                    GdiTextOut(wnd, pCtlSlideText->pStr, StrLen(pCtlSlideText->pStr), (pCtlSlideText->sPrv.nCurrX += -1), pCtlSlideText->sText.nY);
                    pCtlSlideText->sPrv.nCurrWidth--;
                }
                else
                {
                    GdiSetRect(&rtClient, 0, 0, pCtlSlideText->sBg.nRight, pCtlSlideText->sBg.nBottom - pCtlSlideText->sBg.nTop );
                    GdiFillRect(wnd, &rtClient, pCtlSlideText->sBg.nBgColor);
                    GdiTextOut(wnd, pCtlSlideText->pStr, StrLen(pCtlSlideText->pStr), pCtlSlideText->sBg.nRight, pCtlSlideText->sText.nY);
                    pCtlSlideText->sPrv.nCurrX = pCtlSlideText->sBg.nRight;

                    if ( nCurrWidth < pCtlSlideText->sBg.nRight )
                        pCtlSlideText->sPrv.nCurrWidth = nCurrWidth + ( pCtlSlideText->sBg.nRight - nCurrWidth);
                    else
                        pCtlSlideText->sPrv.nCurrWidth = nCurrWidth;
                }
                break;
            default:
                break;
            }
        }
        else if ( pCtlSlideText->nSSFlag == CTL_SLIDETEXT_MOVE_PAUSE)
        {
            if(pCtlSlideText->nTimeout != 0)
                UsrKillTimer(wnd, REXY_CTL_SLIDETEXT_TIMER);
			
			pCtlSlideText->sPrv.nPauseX = pCtlSlideText->sPrv.nCurrX; 
        }
        else
        {
            if(pCtlSlideText->nTimeout != 0)
                UsrKillTimer(wnd, REXY_CTL_SLIDETEXT_TIMER);
            
			GdiSetRect(&rtClient, 0, 0, pCtlSlideText->sBg.nRight, pCtlSlideText->sBg.nBottom - pCtlSlideText->sBg.nTop );
            if(pCtlSlideText->sBg.nBgColor)
                GdiFillRect(wnd, &rtClient, pCtlSlideText->sBg.nBgColor);

            GdiTextOut(wnd, pCtlSlideText->pStr, StrLen(pCtlSlideText->pStr), pCtlSlideText->sText.nX, pCtlSlideText->sText.nY);

            //CtlDraw(wnd, pErr);
        }
        break;

    case CTL_SLIDETEXT_TYPE_LROUND:
        if (pCtlSlideText->nSSFlag == CTL_SLIDETEXT_MOVE_PLAY)
        {
            if ( pCtlSlideText->pfnDraw != NULL )
                (*pCtlSlideText->pfnDraw) (wnd, pCtlSlideText->sText.nFgColor, pCtlSlideText->sBg.nBgColor);

            if ( pCtlSlideText->sPrv.nCurrWidth >= 0 )
            {
                GdiTextOut(wnd, pCtlSlideText->pStr, StrLen(pCtlSlideText->pStr), pCtlSlideText->sPrv.nCurrX += -1, pCtlSlideText->sText.nY);
                pCtlSlideText->sPrv.nCurrWidth--;
            }
            else
            {
                GdiSetRect(&rtClient, 0, 0, pCtlSlideText->sBg.nRight, pCtlSlideText->sBg.nBottom - pCtlSlideText->sBg.nTop );
                GdiFillRect(wnd, &rtClient, pCtlSlideText->sBg.nBgColor);
                GdiTextOut(wnd, pCtlSlideText->pStr, StrLen(pCtlSlideText->pStr), pCtlSlideText->sBg.nRight, pCtlSlideText->sText.nY);
                pCtlSlideText->sPrv.nCurrWidth = nCurrWidth + pCtlSlideText->sBg.nRight;
                pCtlSlideText->sPrv.nCurrX = pCtlSlideText->sBg.nRight;
            }
        }
        else if ( pCtlSlideText->nSSFlag == CTL_SLIDETEXT_MOVE_PAUSE)
        {
            if(pCtlSlideText->nTimeout != 0)
                UsrKillTimer(wnd, REXY_CTL_SLIDETEXT_TIMER);

			pCtlSlideText->sPrv.nPauseX = pCtlSlideText->sPrv.nCurrX; 
        }
        else
        {
            if(pCtlSlideText->nTimeout != 0)
                UsrKillTimer(wnd, REXY_CTL_SLIDETEXT_TIMER);

			GdiSetRect(&rtClient, 0, 0, pCtlSlideText->sBg.nRight, pCtlSlideText->sBg.nBottom - pCtlSlideText->sBg.nTop );
            if(pCtlSlideText->sBg.nBgColor)
                GdiFillRect(wnd, &rtClient, pCtlSlideText->sBg.nBgColor);

            GdiTextOut(wnd, pCtlSlideText->pStr, StrLen(pCtlSlideText->pStr), pCtlSlideText->sText.nX, pCtlSlideText->sText.nY);


          //  CtlDraw(wnd, pErr);
        }

        break;

//    case CTL_SLIDETEXT_TYPE_LRIGHT:
        // TODO : wijang
        break;

    default:
        break;
    }

	GdiUnlockScreen(wnd);

    TRACE(TR_RexyControl, TR_LEVEL3, ("PrvSlideCtlTextHandler() Return\n",0));
    return;

}


/**
* @brief		set slide text control for text properties.	
* @param[in]	pCtlSlideText		Pointer to main structure of slide text control.
* @param[in]	pCtlSlideTextInfo	Pointer to specified text info sturcture for slide text control.
* @param[out]	pErr	Pointer to error value
* @return		void
* @remarks		This functions set text slide control by specified slide text control text info structure's value.	
* @if HISTORY
*         20-July-2006 wijang Initiated
* @endif
*/

void PrvSlideCtlSetTextInfo (REXY_CTL_SLIDETEXT *pCtlSlideText, REXY_CTL_SLIDETEXT_INFO *pCtlSlideTextInfo, int *pErr)
{
    TRACE(TR_RexyControl, TR_LEVEL3, ("PrvSlideCtlSetTextInfo(pCtlSlideText=0x%x, pCtlSlideTextInfo=0x%x, *pErr=%d) Called\n", pCtlSlideText, pCtlSlideTextInfo, *pErr));

    pCtlSlideText->sText.nX			= pCtlSlideTextInfo->nX;
    pCtlSlideText->sText.nY			= pCtlSlideTextInfo->nY;
    pCtlSlideText->sText.nFgColor	= pCtlSlideTextInfo->nFgColor;

    TRACE(TR_RexyControl, TR_LEVEL3, ("PrvSlideCtlSetTextInfo() Return\n"));
    return;
}


/**
* @brief		set slide text control for window rectangle properties.
* @param[in]	pCtlSlideText		Pointer to main structure of slide text control.
* @param[in]    pCtlSlideTextBgInfo	Pointer to specified rect info structure for slide text control.
* @param[out]	pErr	Pointer to error value
* @return		void
* @remarks		this fuctions set text slide control by specified sldie text control rect info structure's value.
* @if HISTORY
*         20-July-2006 wijang Initiated
* @endif
*/
void PrvSlideCtlSetBgInfo (REXY_CTL_SLIDETEXT *pCtlSlideText, rect *pCtlSlideTextBgInfo, int *pErr)
{
    TRACE(TR_RexyControl, TR_LEVEL3, ("PrvSlideCtlSetBgInfo(pCtlSlideText=0x%x, pCtlSlideTextBgInfo=0x%x, *pErr=%d) Called\n", pCtlSlideText, pCtlSlideTextBgInfo, *pErr));

    pCtlSlideText->sBg.nLeft		=	pCtlSlideTextBgInfo->left;
    pCtlSlideText->sBg.nTop			=	pCtlSlideTextBgInfo->top;
    pCtlSlideText->sBg.nRight		=	pCtlSlideTextBgInfo->right;
    pCtlSlideText->sBg.nBottom		=	pCtlSlideTextBgInfo->bottom;

    TRACE(TR_RexyControl, TR_LEVEL3, ("PrvSlideCtlSetBgInfo() Return\n"));

    return;
}


/**
* @brief		Draw slide text control.
* @param[in]	wnd		Pointer to slide text control window
* @param[out]   pErr	Pointer to error value
* @return		void
* @remarks		This function draws default slide text sets slide timer.
* @if HISTORY
*         20-July-2006 wijang Initiated
* @endif
*/
void PrvCtlSlideTextDraw(WND wnd, int *pErr)
{
    rect rtClient;

    rint16 nCurrWidth;
    rint16 nCurrHeight;

    REXY_CTL_SLIDETEXT *pCtlSlideText;

    TRACE(TR_RexyControl, TR_LEVEL3, ("PrvCtlSlideTextDraw(hWnd=0x%x, *pErr=%d) Called\n", wnd, *pErr));

    pCtlSlideText = (REXY_CTL_SLIDETEXT*)CtlGetStruct(wnd);
    CHECK_NULLPTR_VOID(pCtlSlideText, *pErr);

    nCurrWidth = FntGetTextWidth(&FntGlobalContext, pCtlSlideText->pStr, StrLen( pCtlSlideText->pStr));
    nCurrHeight = pCtlSlideText->sBg.nBottom - pCtlSlideText->sBg.nTop;
    
	switch (pCtlSlideText->nType)
    {
    
	case CTL_SLIDETEXT_TYPE_LFRONT:
        if ( pCtlSlideText->nFEFlag == CTL_SLIDETEXT_FRONT )
        {
			pCtlSlideText->sPrv.nCurrMaxWidth = pCtlSlideText->nMaxWidth;
			pCtlSlideText->sPrv.nCurrX = pCtlSlideText->sText.nX;
			pCtlSlideText->sPrv.nCurrWidth = nCurrWidth;

        }
        else
        {
			pCtlSlideText->sPrv.nCurrMaxWidth = pCtlSlideText->nMaxWidth;
			pCtlSlideText->sPrv.nCurrX = pCtlSlideText->sBg.nRight;
			if ( nCurrWidth < pCtlSlideText->sBg.nRight )
				pCtlSlideText->sPrv.nCurrWidth = nCurrWidth + ( pCtlSlideText->sBg.nRight - nCurrWidth);
			else
				pCtlSlideText->sPrv.nCurrWidth = nCurrWidth;
        }
        break;
    
	case CTL_SLIDETEXT_TYPE_LROUND:
        if ( pCtlSlideText->nFEFlag == CTL_SLIDETEXT_FRONT )
        {
			pCtlSlideText->sPrv.nCurrMaxWidth = pCtlSlideText->nMaxWidth;
			pCtlSlideText->sPrv.nCurrX = pCtlSlideText->sText.nX;
			pCtlSlideText->sPrv.nCurrWidth = nCurrWidth;
        }
        else
        {
			pCtlSlideText->sPrv.nCurrMaxWidth = pCtlSlideText->nMaxWidth;
			pCtlSlideText->sPrv.nCurrX = pCtlSlideText->sBg.nRight;
			pCtlSlideText->sPrv.nCurrWidth = nCurrWidth +  pCtlSlideText->sBg.nRight;
        }
        break;
    default:
        break;
    }


    GdiLockScreen(wnd);
	
    if ( (pCtlSlideText->nSSFlag == CTL_SLIDETEXT_MOVE_PLAY) && (pCtlSlideText->sPrv.nCurrWidth > pCtlSlideText->nMaxWidth) )
    {
        if ( pCtlSlideText->nTimeout != 0 )
        {
            GdiSetRect(&rtClient, 0, 0, pCtlSlideText->sBg.nRight, nCurrHeight );
            if(pCtlSlideText->sBg.nBgColor)
                GdiFillRect(wnd, &rtClient, pCtlSlideText->sBg.nBgColor);

            UsrSetTimer(wnd, REXY_CTL_SLIDETEXT_TIMER, pCtlSlideText->nTimeout );

        }
    }
	else
	{
		GdiSetRect(&rtClient, 0, 0, pCtlSlideText->sBg.nRight, pCtlSlideText->sBg.nBottom - pCtlSlideText->sBg.nTop );
	    if(pCtlSlideText->sBg.nBgColor)
			GdiFillRect(wnd, &rtClient, pCtlSlideText->sBg.nBgColor);
		switch ( pCtlSlideText->nSSFlag )
		{
		case CTL_SLIDETEXT_MOVE_PLAY:
			pCtlSlideText->nSSFlag = CTL_SLIDETEXT_MOVE_STOP;
		case CTL_SLIDETEXT_MOVE_STOP:
			GdiTextOut(wnd, pCtlSlideText->pStr, StrLen(pCtlSlideText->pStr), pCtlSlideText->sText.nX, pCtlSlideText->sText.nY);
			break;
		case CTL_SLIDETEXT_MOVE_PAUSE:
			GdiTextOut(wnd, pCtlSlideText->pStr, StrLen(pCtlSlideText->pStr), pCtlSlideText->sPrv.nPauseX, pCtlSlideText->sText.nY);
			break;
		}
	}

	GdiUnlockScreen(wnd);

    TRACE(TR_RexyControl, TR_LEVEL3, ("PrvCtlSlideTextDraw() Return\n"));
    return;
}

/**
* @brief		Callback function called before drawing slide text control.		
* @param[in]	wnd		Pointer to slide text control window
* @param[out]	pErr	pErr	Pointer to error value
* @return		void
* @remarks		This function sets control window rectangle range and forground/background color.
* @if HISTORY
*         20-July-2006 wijang Initiated
* @endif
*/
void PrvCtlSlideTextDrawPreCB(WND wnd, int *pErr)
{
    REXY_CTL_SLIDETEXT *pCtlSlideText;

    TRACE(TR_RexyControl, TR_LEVEL3, ("PrvCtlSlideTextDrawPreCB(hWnd=0x%x, *pErr=%d) Called\n", wnd, *pErr));

    pCtlSlideText = (REXY_CTL_SLIDETEXT*)CtlGetStruct(wnd);
    CHECK_NULLPTR_VOID(pCtlSlideText, *pErr);

    CtlSetWNDRect(wnd,
                  pCtlSlideText->sBg.nLeft,
                  pCtlSlideText->sBg.nTop,
                  pCtlSlideText->sBg.nRight,
                  pCtlSlideText->sBg.nBottom,
                  pErr);

    CtlSetWNDColor(wnd, pCtlSlideText->sText.nFgColor, pCtlSlideText->sBg.nBgColor, pErr);

//	if ( pCtlSlideText->nSSFlag == CTL_SLIDETEXT_MOVE_PAUSE )
//		pCtlSlideText->nSSFlag = CTL_SLIDETEXT_MOVE_STOP;	
 
	TRACE(TR_RexyControl, TR_LEVEL3, ("PrvCtlSlideTextDrawPreCB() Return\n"));

    return;

}


/**
* @brief		Call back function called after creating slide text control.
* @param[in]	wnd		Pointer to slide text control window
* @param[out]	pErr	Pointer to error value
* @return		void
* @remarks		Currently does nothing.
* @if HISTORY
*         20-July-2006 wijang Initiated
* @endif
*/
void PrvCtlSlideTextDrawPostCB(WND wnd, int *pErr)
{
    TRACE(TR_RexyControl, TR_LEVEL3, ("PrvCtlSlideTextDrawPostCB(hWnd=0x%x, *pErr=%d) Called\n", wnd, *pErr));
    TRACE(TR_RexyControl, TR_LEVEL3, ("PrvCtlSlideTextDrawPostCB() Return\n"));
    return;

}


/**
* @brief		Handles popup control's common key down event. 
* @param[in]	wnd		Pointer to slide text control window
* @param[in]	wParam	key type
* @param	pErr
* @return		Returns a result value.
* @if HISTORY
*		  20-July-2006 wijang Initiated
* @endif
*/
int PrvCtlSlideTextKeyDown(WND wnd, int wParam, int *pErr)
{
    REXY_CTL_SLIDETEXT *pCtlSlideText;
	int ret;

    int bHandled = FALSE;
    
	TRACE(TR_RexyControl, TR_LEVEL3, ("PrvCtlSlideTextKeyDown(hWnd=0x%x, *pErr=%d) Called\n", wnd, *pErr));

    pCtlSlideText = (REXY_CTL_SLIDETEXT*)CtlGetStruct(wnd);

    if (pCtlSlideText->pfnKeyDown != NULL )
    {
        ret = (*pCtlSlideText->pfnKeyDown) (wnd, wParam);
        
		if ( ret )
			bHandled = TRUE;
		else
			bHandled = FALSE;
    }
    else
        bHandled = FALSE;


    TRACE(TR_RexyControl, TR_LEVEL3, ("PrvCtlSlideTextKeyDown() Return %d\n", 0));

    return bHandled;
}

/**
* @brief        Destroys created slide text control.
* @param[in]    wnd			Pointer to slide text control window
* @param[out]   pErr		Pointer to error value
* @retval       void
* @remarks      This function Free allocated memory for list control data.
* @if HISTORY
*         
* @endif
*/
void PrvCtlSlideTextDestroy(WND wnd, int *pErr)
{
    REXY_CTL_SLIDETEXT *pCtlSlideText;

    TRACE(TR_RexyControl, TR_LEVEL3, ("PrvCtlSlideTextDestroy(hWnd=0x%x, *pErr=%d) Called\n", wnd, *pErr));

    pCtlSlideText = (REXY_CTL_SLIDETEXT*)CtlGetStruct(wnd);
    CHECK_NULLPTR_VOID(pCtlSlideText, *pErr);

    // Kill slidetext timer
    if(pCtlSlideText->nTimeout != 0)
        UsrKillTimer(wnd, REXY_CTL_SLIDETEXT_TIMER);

    // Free slidetext
    MemFree(pCtlSlideText);
    wnd->pData = NULL;

    TRACE(TR_RexyControl, TR_LEVEL3, ("PrvCtlSlideTextDestroy() Return\n"));

    return;
}

/**
* @brief        Call back function called before destroying slide text control.
* @param[in]    wnd             Pointer to slide text control window
* @param[out]   pErr            Pointer to error value
* @retval       void
* @remarks      Currently does nothing.
* @if HISTORY
*		  20-July-2006 wijang Initiated
* @endif
*/
void PrvCtlSlideTextDestroyPreCB(WND wnd, int *pErr)
{
    TRACE(TR_RexyControl, TR_LEVEL3, ("PrvCtlSlideTextDestroyPreCB(hWnd=0x%x, *pErr=%d) Called\n", wnd, *pErr));
    TRACE(TR_RexyControl, TR_LEVEL3, ("PrvCtlSlideTextDestroyPreCB() Return\n"));
    return;
}

/**
* @brief        Main message handler of slide text control.
* @param[in]    wnd         Pointer to slide text control window
* @param[in]    nMsg        Message to handle
* @param[in]    wParam      wParam of message
* @param[in]    lParam      lParam of message
* @retval       RxErrNone   If nothing to handle or no error occurred
* @retval       wnd         If message was AWM_CREATE and control was successfully created
* @remarks      This function handles messages for slide text control.\n
*               This function is called by control core if some messages is needed to be handled by slide text control.
* @if HISTORY
*		  20-July-2006 wijang Initiated
* @endif
*/
int CtlSlideTextProc(WND wnd, int nMsg, int wParam, int lParam)
{
    int err;
    int bHandled = FALSE;

    TRACE(TR_RexyControl, TR_LEVEL3, ("CtlSlideTextProc(hWnd=0x%x, nMsg=%d, wParam=%d, lParam=%d) Called\n", wnd, nMsg, wParam, lParam));
    switch(nMsg)
    {
    case AWM_CREATE:
        PrvCtlSlideTextCreate(wnd);
        TRACE(TR_RexyControl, TR_LEVEL3, ("CtlSlideTextProc() Return %d\n", 0));
        return 0;

    case AWM_PAINT:
        PrvCtlSlideTextDraw(wnd, &err);
        TRACE(TR_RexyControl, TR_LEVEL3, ("CtlSlideTextProc() Return %d\n", 0));
        return 0;

    case AWM_KEYDOWN:
        TRACE(TR_RexyControl, TR_LEVEL3, ("CtlSlideTextProc() Return %d\n", 0));
        return PrvCtlSlideTextKeyDown(wnd, wParam, &err);


    case AWM_TIMER:
        PrvSlideCtlTextHandler(wnd, &err);
        TRACE(TR_RexyControl, TR_LEVEL3, ("CtlSlideTextProc() Return %d\n", 0));
        bHandled = TRUE;
        break;

    case AWM_DESTROY:
        PrvCtlSlideTextDestroy(wnd, &err);
        TRACE(TR_RexyControl, TR_LEVEL3, ("CtlSlideTextProc() Return %d\n", 0));
        return 0;
    }

    TRACE(TR_RexyControl, TR_LEVEL3, ("CtlSlideTextProc() Return %d\n", 0));

    return bHandled;
}

/**
* @brief        Callback message hanlder of slide text control.
* @param[in]    wnd         Pointer to slide text control window
* @param[in]    nMsg        Message to handle
* @param[in]    wParam      wParam of message
* @param[in]    lParam      lParam of message
* @param[in]    bIsPre      0 if functions is called for pre-callback\n
*                           Besides 0 if function called for post-callback
* @param[out]   pErr        Pointer to error value
* @retval       RxErrNone   If nothing to handle or no error occurred
* @remarks      This callback function called after handling some messages.
* @if HISTORY
*		  20-July-2006 wijang Initiated
* @endif
*/
int CtlSlideTextCBProc(WND wnd, int nMsg, int wParam, int lParam, int bIsPre, int *pErr)
{
    TRACE(TR_RexyControl, TR_LEVEL3, ("CtlSlideTextCBProc(hWnd=0x%x, nMsg=%d, wParam=%d, lParam=%d) Called\n", wnd, nMsg, wParam, lParam));
    switch(nMsg)
    {
    case AWM_CREATE:
        PrvCtlSlideTextCreatePostCB(wnd, pErr);
        TRACE(TR_RexyControl, TR_LEVEL3, ("CtlSlideTextCBProc() Return %d\n", 0));
        return 0;

    case AWM_DESTROY:
        PrvCtlSlideTextDestroyPreCB(wnd, pErr);
        TRACE(TR_RexyControl, TR_LEVEL3, ("CtlSlideTextCBProc() Return %d\n", 0));
        return 0;

    case AWM_PAINT:
        if(bIsPre)
            PrvCtlSlideTextDrawPreCB(wnd, pErr);
        else
            PrvCtlSlideTextDrawPostCB(wnd, pErr);

        TRACE(TR_RexyControl, TR_LEVEL3, ("CtlSlideTextCBProc() Return %d\n", 0));
        return 0;

    default:
        TRACE(TR_RexyControl, TR_LEVEL3, ("CtlSlideTextCBProc() Return %d\n", 0));
        return 0;
    }
}

/**
* @brief        Command handler of slide text control.
* @param[in]    wnd         Pointer to slide text control window
* @param[in]    nCommand		Command ID
* @param[in]    wParam		wParam of command
* @param[in]    lParam		lParam of command
* @param[out]   pErr		Pointer to error value
* @retval       Varies by command ID
* @remarks      This function commands to slide text control to do various functions.\n
*               By command ID slide text control set its inner member value or,
*               returns its inner member value to whom it called.\n
*               Also does some functions such as drawing by specified command ID.
* @if HISTORY
*		  20-July-2006 wijang Initiated
* @endif
*/
int CtlSlideTextCmdProc(WND wnd, int nCommand,int wParam, int lParam, int *pErr)
{
    REXY_CTL_SLIDETEXT *pCtlSlideText;

    TRACE(TR_RexyControl, TR_LEVEL3, ("CtlSlideTextCmdProc(hWnd=0x%x, nMsg=%d, wParam=%d, lParam=%d) Called\n", wnd, nCommand, wParam, lParam));

    pCtlSlideText = (REXY_CTL_SLIDETEXT*)CtlGetStruct(wnd);
    CHECK_NULLPTR(pCtlSlideText, 0, *pErr);

    switch(nCommand) {

        // Text related command
    case CTL_SLIDETEXT_CMD_SET_X:
        pCtlSlideText->sText.nX = (ruint16)wParam;
        break;
    case CTL_SLIDETEXT_CMD_SET_Y:
        pCtlSlideText->sText.nY = (ruint16)wParam;
        break;
    case CTL_SLIDETEXT_CMD_SET_FGCOLOR:
        pCtlSlideText->sText.nFgColor = (COLORVAL)wParam;
		GdiSetFgColor(wnd, (COLORVAL)wParam);
        break;

        // Bg related command
    case CTL_SLIDETEXT_CMD_SET_TOP:
        pCtlSlideText->sBg.nTop = (ruint16)wParam;
        break;
    case CTL_SLIDETEXT_CMD_SET_LEFT:
        pCtlSlideText->sBg.nLeft = (ruint16)wParam;
        break;
    case CTL_SLIDETEXT_CMD_SET_RIGHT:
        pCtlSlideText->sBg.nRight = (ruint16)wParam;
        break;
    case CTL_SLIDETEXT_CMD_SET_BOTTOM:
        pCtlSlideText->sBg.nBottom= (ruint16)wParam;
        break;
    case CTL_SLIDETEXT_CMD_SET_BGCOLOR:
        pCtlSlideText->sBg.nBgColor = (COLORVAL)wParam;
		GdiSetBkColor(wnd, (COLORVAL)wParam);
        break;

        // slide related command
    case CTL_SLIDETEXT_CMD_DO_LFRONT:
        pCtlSlideText->nType = (ruint16)CTL_SLIDETEXT_TYPE_LFRONT;
        break;
    case CTL_SLIDETEXT_CMD_DO_LROUND:
        pCtlSlideText->nType = (ruint16)CTL_SLIDETEXT_TYPE_LROUND;
        break;
    case CTL_SLIDETEXT_CMD_SET_TIMEOUT:
        pCtlSlideText->nTimeout = (ruint16)wParam;
        break;
    case CTL_SLIDETEXT_CMD_DO_PLAY:

		if ( pCtlSlideText->nSSFlag == 	CTL_SLIDETEXT_MOVE_PAUSE )
		{
			pCtlSlideText->nSSFlag = (ruint8)CTL_SLIDETEXT_MOVE_PLAY;
			pCtlSlideText->sPrv.nCurrX = pCtlSlideText->sPrv.nPauseX;
			UsrSetTimer(wnd, REXY_CTL_SLIDETEXT_TIMER, pCtlSlideText->nTimeout );

		}
		else
		{
			pCtlSlideText->nSSFlag = (ruint8)CTL_SLIDETEXT_MOVE_PLAY;
			CtlDraw(wnd, pErr);
		}
        
		break;
    case CTL_SLIDETEXT_CMD_DO_STOP:
//		if ( pCtlSlideText->nSSFlag == CTL_SLIDETEXT_MOVE_PLAY 
//			|| pCtlSlideText->nSSFlag == CTL_SLIDETEXT_MOVE_PAUSE )
			pCtlSlideText->nSSFlag = (ruint8)CTL_SLIDETEXT_MOVE_STOP; 
			CtlDraw(wnd, pErr);
        break;
    case CTL_SLIDETEXT_CMD_DO_PAUSE:
		if ( pCtlSlideText->nSSFlag == CTL_SLIDETEXT_MOVE_PLAY )
			pCtlSlideText->nSSFlag = (ruint8)CTL_SLIDETEXT_MOVE_PAUSE;
		else
			pCtlSlideText->nSSFlag = (ruint8)CTL_SLIDETEXT_MOVE_STOP;
        break;
	case CTL_SLIDETEXT_CMD_GET_MODE:
		return (int) pCtlSlideText->nSSFlag;

    case CTL_SLIDETEXT_CMD_DO_START_FRONT:
        pCtlSlideText->nFEFlag = (ruint8)CTL_SLIDETEXT_START_FRONT;
        break;
    case CTL_SLIDETEXT_CMD_DO_START_END:
        pCtlSlideText->nFEFlag = (ruint8)CTL_SLIDETEXT_START_END;
        break;
    case CTL_SLIDETEXT_CMD_SET_MAXWIDTH:
        pCtlSlideText->nMaxWidth = (ruint16)wParam;
        break;
    case CTL_SLIDETEXT_CMD_SET_DATA:
        pCtlSlideText->pStr = (void*)lParam;
        break;
    case CTL_SLIDETEXT_CMD_DO_DRAW:
        CtlDraw(wnd, pErr);
        break;

        // Text info related command
//    case CTL_SLIDETEXT_CMD_SET_TEXT_INFO:
//        PrvSlideCtlSetTextInfo( pCtlSlideText, (REXY_CTL_SLIDETEXT_INFO*)lParam, pErr );
//        break;

        // Bg info related command
    case CTL_SLIDETEXT_CMD_SET_WNDRECT:
        PrvSlideCtlSetBgInfo( pCtlSlideText, (rect*)lParam, pErr );
        break;

        // callback related command
    case CTL_SLIDETEXT_CMD_SET_CB_DRAW:
        pCtlSlideText->pfnDraw = (CtlSlideTextDrawCB)lParam;
        break;
    case CTL_SLIDETEXT_CMD_SET_DB_KEYDOWN:
        pCtlSlideText->pfnKeyDown = (CtlSlideTextKeyDownCB)lParam;
        break;
    }

    TRACE(TR_RexyControl, TR_LEVEL3, ("CtlSlideTextCmdProc() Return %d\n", 0));

    return 0;
}

/** @} */ // End of slidetext Control
