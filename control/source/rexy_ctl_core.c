/*
 * $Id: rexy_ctl_core.c 491 2006-01-11 16:32:34Z madholic $
 * Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded.
 *
 */

/** @file */

/**
 * @addtogroup ControlCoreGroup Ctrl Control Core 
 * Ctrl Control Core
 * @{
 */

#include "rexy.h"
#include "rexy_ctl_core.h"
#include "rexy_ctl_edit.h"

rint32 gCtlLastError;

extern CTL_PROC		    CtlGetCtlProc(rint32 nType, rint32 *pErr);
extern CTL_CB_PROC	    CtlGetCtlCBProc(rint32 nType, rint32 *pErr);
extern CTL_COMMAND_PROC CtlGetCtlCmdProc(rint32 type, rint32 *pErr);

/** 
 * @brief		create control
 * @param[in]	wnd			parent window
 * @param[in]	nType		control type
 * @param[in]	location	information of control rectangle
 * @param[in]	pfnProc		callback function
 * @param[out]	pErr		error pointer
 * @retval		new created control window pointer		if function succeeds.
 * @retval		NULL									if function fails.
 * @remarks		when control is created, do not call pre callback function
 * @see
 * @if HISTORY
 *				07-Jul-2006 venabili updated 
 * @endif
 */
WND CtlCreate(WND wnd, rint32 nType, rect *location, CTL_CB_PROC pfnProc, rint32 *pErr)
{
	return CtlCreateEx(wnd, nType, 0, 0, location, pfnProc, pErr);
}

WND CtlCreateEx(WND wnd, rint32 nType, rint32 id, rint32 style, rect *location, CTL_CB_PROC pfnProc, rint32 *pErr)
{
    WND new_wnd;
    CTL_PROC ctlProc;
    CTL_CB_PROC pfnCtlCBProc = NULL;

    // check Error Var
    CHECK_NULLERR(pErr,NULL);

    *pErr = CTLERR_NONE;


    CHECK_ERR(*pErr,CTLERR_NONE, NULL);

    // get control proc
    ctlProc = CtlGetCtlProc(nType, pErr);

    if(ctlProc == NULL)
        return NULL;

	// Create new control structure
	new_wnd = WinAddControl(wnd, nType, id, style, location, CtlWinProc);
    //new_wnd = WinCreateWindow(wnd, nType, id, style, location, CtlWinProc);

    if(new_wnd == NULL)
    {
        *pErr = CTLERR_CREATE;
        return NULL;
    }

    ctlProc(new_wnd, AWM_CREATE, 0, 0);

    new_wnd->pfnProc = (void*)pfnProc;
    pfnCtlCBProc = CtlGetCtlCBProc(nType, pErr);

    if(pfnCtlCBProc)
        pfnCtlCBProc(new_wnd,AWM_CREATE,0,0,FALSE,pErr);//post callback;


    CHECK_ERR(*pErr,CTLERR_NONE, NULL);

    if(pfnProc)
        pfnProc(new_wnd,AWM_CREATE,0,0,FALSE,pErr);//post appl callback;

    CHECK_ERR(*pErr,CTLERR_NONE, NULL);

	return new_wnd;
}


/** 
 * @brief		destroy control
 * @param[in]	wnd		control instance window pointer to destroy
 * @param[out]	pErr		error pointer
 * @remarks		when control is destroyed, do not call post callback function
 * @see
 * @if HISTORY
 *				07-Jul-2006 venabili updated 
 * @endif
 */
void CtlDestroy(WND wnd, rint32 *pErr)
{
    CTL_PROC ctlProc;
    CTL_CB_PROC pfnCtlCBProc = NULL;
    CTL_CB_PROC pfnProc = NULL;

    CHECK_NULLERR_VOID(pErr);

    *pErr = CTLERR_NONE;

    CHECK_NULLPTR_VOID(wnd->pData,*pErr);

	// Application Callback Message Handler
    if(wnd->pfnProc)
    {
        pfnProc = (CTL_CB_PROC)(wnd->pfnProc);
        *pErr = pfnProc(wnd,AWM_DESTROY,0,0,TRUE,pErr);//pre appl callback;
    }

    CHECK_ERR_VOID(*pErr,CTLERR_NONE);

	// Control Callback Message Handler
    pfnCtlCBProc = CtlGetCtlCBProc(wnd->window_type, pErr);

    if(pfnCtlCBProc)
        pfnCtlCBProc(wnd,AWM_DESTROY,0,0,TRUE,pErr);//pre callback;
    CHECK_ERR_VOID(*pErr,CTLERR_NONE);

    // get control proc
    ctlProc = CtlGetCtlProc(wnd->window_type, pErr);
  
    CHECK_ERR_VOID(*pErr,CTLERR_NONE);
    CHECK_NULLPTR_VOID(ctlProc,*pErr);

    ctlProc(wnd, AWM_DESTROY, 0, 0);
	WinDestroyControl(wnd);

    //free_w(wnd);
}

/** 
 * @brief		draw control
 * @param[in]	wnd			control window pointer
 * @param[out]	pErr		error pointer
 * @remarks		
 * @see
 */
void CtlDraw(WND wnd, rint32 *pErr)
{
    CtlProcessEvent(wnd,pErr,AWM_PAINT,0,0);
}

/** 
 * @brief		save control
 * @param[in]	wnd			control window pointer
 * @param[out]	pErr		error pointer
 * @remarks		
 * @see
 */
void CtlSave(WND wnd, rint32 *pErr)
{
    CtlProcessEvent(wnd,pErr,AWM_SAVE,0,0);
}

/** 
 * @brief		load control
 * @param[in]	wnd			control window pointer
 * @param[out]	pErr		error pointer
 * @remarks		
 * @see
 */
void CtlLoad(WND wnd, rint32 *pErr)
{
    CtlProcessEvent(wnd,pErr,AWM_LOAD,0,0);
}

/** 
 * @brief		process key down for each control
 * @param[in]	wnd			control window pointer
 * @param[out]	pErr		error pointer
 * @remarks		
 * @see
 */
void CtlKeyDown(WND wnd, rint32 *pErr)
{
    CtlProcessEvent(wnd, pErr, AWM_KEYDOWN, 0, 0);
}

/** 
 * @brief		control event processing
 * @param[in]	wnd			control window pointer
 * @param[out]	pErr		error pointer
 * @param[in]	nMsg
 * @param[in]	iwParam
 * @param[in]	ilParam
 * @remarks		
 * @see
 */
rint32 CtlProcessEvent(WND wnd, rint32 *pErr,rint32 nMsg,rint32 iwParam,rint32 ilParam)
{
    CTL_PROC ctlProc;
    CTL_CB_PROC pfnProc;
    CTL_CB_PROC pfnCtlCBProc = NULL;
    rint32 handled = FALSE;

    CHECK_NULLERR_VOID(pErr);

    *pErr = CTLERR_NONE;

    CHECK_NULLPTR_VOID(wnd,*pErr);

    pfnCtlCBProc = CtlGetCtlCBProc(wnd->window_type, pErr);

    if(wnd->pfnProc)
    {
        pfnProc = (CTL_CB_PROC)wnd->pfnProc;
        pfnProc(wnd,nMsg,iwParam,ilParam,TRUE,pErr);//pre appl callback;
    }
    CHECK_ERR_VOID(*pErr,CTLERR_NONE);

    if(pfnCtlCBProc)
        pfnCtlCBProc(wnd,nMsg,iwParam,ilParam,TRUE,pErr);//pre ctl callback;
    CHECK_ERR_VOID(*pErr,CTLERR_NONE);

    // get control proc
    ctlProc = CtlGetCtlProc(wnd->window_type, pErr);
    CHECK_ERR_VOID(*pErr,CTLERR_NONE);
    CHECK_NULLPTR_VOID(ctlProc,*pErr);

    //*pErr = ctlProc(wnd,nMsg,iwParam,ilParam,pErr);
    handled = ctlProc(wnd,nMsg,iwParam,ilParam);
    CHECK_ERR_VOID(*pErr,CTLERR_NONE);

    if(pfnCtlCBProc)
        pfnCtlCBProc(wnd,nMsg,iwParam,ilParam,FALSE,pErr);//post ctl callback;
    CHECK_ERR_VOID(*pErr,CTLERR_NONE);

    if(wnd->pfnProc)
        pfnProc(wnd,nMsg,iwParam,ilParam,FALSE,pErr);//post appl callback;
    CHECK_ERR_VOID(*pErr,CTLERR_NONE);

    return handled;
}

/** 
 * @brief		do control command process
 * @param[in]	wnd			control window pointer
 * @param[in]	nCommand	control command
 * @param[in]	wParam		
 * @param[in]	lParam		
 * @param[out]	pErr		error pointer
 * @remarks		
 * @see
 */
rint32 CtlDo(WND wnd, rint32 nCommand, rint32 wParam, rint32 lParam, rint32 *pErr)
{
    CTL_COMMAND_PROC pfnCtlCmdProc = NULL;
    rint32 iValue = 0;

    CHECK_NULLERR(pErr,0); // ASSERT
    *pErr = CTLERR_NONE;

    CHECK_NULLPTR(wnd->pData,0,*pErr); // NULL pData Check

    pfnCtlCmdProc = CtlGetCtlCmdProc(wnd->window_type,pErr);
    CHECK_ERR(*pErr,CTLERR_NONE,0);

    if(pfnCtlCmdProc)
    {
        iValue = pfnCtlCmdProc(wnd,nCommand,wParam,lParam,pErr);
    }
    else
    {
        *pErr = CTLERR_GETCTLPROC;
        return 0;
    }

    CHECK_ERR(*pErr,CTLERR_NONE,0);

    return iValue;
}

/** 
 * @brief		get control property value
 * @param[in]	wnd			control window pointer
 * @param[in]	nCommand	control command
 * @param[out]	pErr		error pointer
 * @remarks		
 * @see
 */
rint32 CtlGeti(WND wnd, rint32 nCommand, rint32 *pErr)
{
    CTL_COMMAND_PROC pfnCtlCmdProc = NULL;
    rint32 iValue = 0;

    CHECK_NULLERR(pErr,0); // ASSERT
    *pErr = CTLERR_NONE;

    CHECK_NULLPTR(wnd,0,*pErr); // NULL pCtl Check

    pfnCtlCmdProc = CtlGetCtlCmdProc(wnd->window_type,pErr);
    CHECK_ERR(*pErr,CTLERR_NONE,0);

    if(pfnCtlCmdProc)
    {
        iValue = pfnCtlCmdProc(wnd,nCommand,0,0,pErr);

#ifdef CTL_VAL_CHECK // validation check
        if(CtlIsValidInt(wnd,nCommand,iValue,pErr) == FALSE)
        {
            *pErr = CTLERR_GETI;
            return 0;
        }
#endif

    }
    else
    {
        *pErr = CTLERR_GETCTLPROC;
        return 0;
    }

    CHECK_ERR(*pErr,CTLERR_NONE,0);

    return iValue;
}

/** 
* @brief        Get control property value by position value
* 
* @param[in]    wnd			The control window pointer
* @param[in]	nCommand	control command
* @param[in]    iPos		The position value
* @param[out]   pErr        Pointer to error value
* 
* @retval       rint32			return property value by the passed position
* @remarks      This function get the property value using position value
*
* @if HISTORY
*         18-Sep-2006 madholic Initiated
* @endif
*/
rint32 CtlGetiPos(WND wnd, rint32 nCommand, rint32 iPos, rint32 *pErr)
{
    CTL_COMMAND_PROC pfnCtlCmdProc = NULL;
    rint32 iValue = 0;

    CHECK_NULLERR(pErr,0); // ASSERT
    *pErr = CTLERR_NONE;

    CHECK_NULLPTR(wnd,0,*pErr); // NULL pCtl Check

    pfnCtlCmdProc = CtlGetCtlCmdProc(wnd->window_type,pErr);
    CHECK_ERR(*pErr,CTLERR_NONE,0);

    if(pfnCtlCmdProc)
    {
        iValue = pfnCtlCmdProc(wnd,nCommand, iPos, 0, pErr);
    }
    else
    {
        *pErr = CTLERR_GETCTLPROC;
        return 0;
    }

    CHECK_ERR(*pErr,CTLERR_NONE,0);

    return iValue;
}


/** 
* @brief        Set the control property with position value
* 
* @param[in]    wnd			The control window pointer
* @param[in]	nCommand	control command
* @param[in]    iPos		The position value
* @param[in]    iValue		The setting value
* @param[out]   pErr        Pointer to error value
* 
* @retval       void
* @remarks      This function set the property value by the position value
*
* @if HISTORY
*         18-Sep-2006 madholic Initiated
* @endif
*/
void CtlSetiPos(WND wnd, rint32 nCommand, rint32 iPos, rint32 iValue,rint32 *pErr)
{
    CTL_COMMAND_PROC pfnCtlCmdProc = NULL;

    CHECK_NULLERR_VOID(pErr); // ASSERT
    *pErr = CTLERR_NONE;

    CHECK_NULLPTR_VOID(wnd->pData,*pErr); // NULL pCtl Check

    pfnCtlCmdProc = CtlGetCtlCmdProc(wnd->window_type,pErr);
    CHECK_ERR_VOID(*pErr,CTLERR_NONE);

    if(pfnCtlCmdProc)
    {
        pfnCtlCmdProc(wnd,nCommand, iPos, iValue, pErr);
    }
    else
    {
        *pErr = CTLERR_GETCTLPROC;
    }
}

/** 
* @brief        Get the pointer of the control property value.
* 
* @param[in]    wnd			The control window pointer
* @param[in]	nCommand	The control command
* @param[in]    iPos		The position value
* @param[out]   pErr        Pointer to error value
* 
* @retval       void*		The pointer of the control property value		
* @remarks      This function get the property pointer with position value
*
* @if HISTORY
*         18-Sep-2006 madholic Initiated
* @endif
*/
void* CtlGetiPtr(WND wnd, rint32 nCommand, rint32 iPos, rint32 *pErr)
{
    CTL_COMMAND_PROC pfnCtlCmdProc = NULL;
    void * pValue = NULL;

    CHECK_NULLERR(pErr, NULL); // ASSERT
    *pErr = CTLERR_NONE;

    CHECK_NULLPTR(wnd, NULL, *pErr); // NULL pCtl Check

    pfnCtlCmdProc = CtlGetCtlCmdProc(wnd->window_type,pErr);
    CHECK_ERR(*pErr,CTLERR_NONE,NULL);

    if(pfnCtlCmdProc)
    {
        pValue = (void *)pfnCtlCmdProc(wnd, nCommand, iPos, 0, pErr);
    }
    else
    {
        *pErr = CTLERR_GETCTLPROC;
        return NULL;
    }

    CHECK_ERR(*pErr, CTLERR_NONE,NULL);

    return pValue;
}

/** 
* @brief        Set the pointer of the control property value.
* 
* @param[in]    wnd			The control window pointer
* @param[in]	nCommand	control command
* @param[in]    iPos		The position value
* @param[in]    pValue		The setting value
* @param[out]   pErr        Pointer to error value
* 
* @retval       void
* @remarks      This function set property with integer and pointer value
*
* @if HISTORY
*         18-Sep-2006 madholic Initiated
* @endif
*/
void CtlSetiPtr(WND wnd, rint32 nCommand, rint32 iPos, void* pValue, rint32 *pErr)
{
    CTL_COMMAND_PROC pfnCtlCmdProc = NULL;
//    rint32 iRvalue = 0;

    CHECK_NULLERR_VOID(pErr); // ASSERT
    *pErr = CTLERR_NONE;

    CHECK_NULLPTR_VOID(wnd,*pErr); // NULL pCtl Check

    pfnCtlCmdProc = CtlGetCtlCmdProc(wnd->window_type,pErr);
    CHECK_ERR_VOID(*pErr,CTLERR_NONE);

    if(pfnCtlCmdProc)
    {
        pfnCtlCmdProc(wnd,nCommand, iPos, (rint32)pValue, pErr);

#ifdef CTL_VAL_CHECK

        if(CtlIsValidPtr(wnd,nCommand,pValue,pErr) == FALSE)
        {
            *pErr = CTLERR_SETPTR;
            return ;
        }
#endif

    }
    else
    {
        *pErr = CTLERR_GETCTLPROC;
    }
}


/** 
 * @brief		set control property value
 * @param[in]	wnd			control window pointer
 * @param[in]	nCommand	control command
 * @param[in]	iValue		data to set
 * @param[out]	pErr		error pointer
 * @remarks		
 * @see
 */
void CtlSeti(WND wnd, rint32 nCommand,rint32 iValue,rint32 *pErr)
{
    CTL_COMMAND_PROC pfnCtlCmdProc = NULL;

    CHECK_NULLERR_VOID(pErr); // ASSERT
    *pErr = CTLERR_NONE;

    CHECK_NULLPTR_VOID(wnd->pData,*pErr); // NULL pCtl Check

    pfnCtlCmdProc = CtlGetCtlCmdProc(wnd->window_type,pErr);
    CHECK_ERR_VOID(*pErr,CTLERR_NONE);

    if(pfnCtlCmdProc)
    {

#ifdef CTL_VAL_CHECK
        if(CtlIsValidInt(wnd->pData,nCommand,iValue,pErr) == FALSE)
        {
            *pErr = CTLERR_SETI;
            return 0;
        }
#endif
        pfnCtlCmdProc(wnd,nCommand,iValue,0,pErr);
    }
    else
    {
        *pErr = CTLERR_GETCTLPROC;
    }
}

/** 
 * @brief		get control property value pointer (function, data etc.)
 * @param[in]	wnd			control window pointer
 * @param[in]	nCommand	control command
 * @param[out]	pErr		error pointer
 * @remarks		
 * @see
 */
void * CtlGetPtr(WND wnd, rint32 nCommand, rint32 *pErr)
{
    CTL_COMMAND_PROC pfnCtlCmdProc = NULL;
    void * pValue = NULL;

    CHECK_NULLERR(pErr,NULL); // ASSERT
    *pErr = CTLERR_NONE;

    CHECK_NULLPTR(wnd,NULL,*pErr); // NULL pCtl Check

    pfnCtlCmdProc = CtlGetCtlCmdProc(wnd->window_type,pErr);
    CHECK_ERR(*pErr,CTLERR_NONE,NULL);

    if(pfnCtlCmdProc)
    {
        pValue = (void *)pfnCtlCmdProc(wnd,nCommand,0,0,pErr);
#ifdef CTL_VAL_CHECK

        if(CtlIsValidPtr(wnd,nCommand,pValue,pErr) == FALSE)
        {
            *pErr = CTLERR_GETPTR;
            return NULL;
        }
#endif

    }
    else
    {
        *pErr = CTLERR_GETCTLPROC;
        return NULL;
    }

    CHECK_ERR(*pErr,CTLERR_NONE,NULL);

    return pValue;
}

/** 
 * @brief		get the control's structure pointer
 * @param[in]	wnd			control window pointer
 * @remarks		
 * @see
 */
void* CtlGetStruct(WND wnd)
{
	return (wnd->pData);
}


bool CtlIsValidPtr(WND wnd, rint32 nCommand, void *pValue, rint32 *pErr)
{

    return TRUE;
}


bool CtlIsValidInt(WND wnd, rint32 nCommand, rint32 iValue, rint32 *pErr)
{

    return TRUE;
}

/** 
 * @brief		set control property value pointer (function, data etc.)
 * @param[in]	wnd			control window pointer
 * @param[in]	nCommand	control command
 * @param[in]	pValue		data pointer to set
 * @param[out]	pErr		error pointer
 * @remarks		
 * @see
 */
void CtlSetPtr(WND wnd, rint32 nCommand,void * pValue,rint32 *pErr)
{
    CTL_COMMAND_PROC pfnCtlCmdProc = NULL;
//    rint32 iRvalue = 0;

    CHECK_NULLERR_VOID(pErr); // ASSERT
    *pErr = CTLERR_NONE;

    CHECK_NULLPTR_VOID(wnd,*pErr); // NULL pCtl Check

    pfnCtlCmdProc = CtlGetCtlCmdProc(wnd->window_type,pErr);
    CHECK_ERR_VOID(*pErr,CTLERR_NONE);

    if(pfnCtlCmdProc)
    {
        pfnCtlCmdProc(wnd,nCommand, 0, (rint32)pValue, pErr);

#ifdef CTL_VAL_CHECK

        if(CtlIsValidPtr(wnd,nCommand,pValue,pErr) == FALSE)
        {
            *pErr = CTLERR_SETPTR;
            return ;
        }
#endif

    }
    else
    {
        *pErr = CTLERR_GETCTLPROC;
    }
}

/** 
 * @brief		set control window position and size
 * @param[in]	wnd			control window pointer
 * @param[in]	x			left position of control frame
 * @param[in]	y			top position of control frame
 * @param[in]	w_width		width of control frame
 * @param[in]	w_height	height of control frame
 * @param[out]	pErr		error pointer
 * @remarks		
 * @see
 */
void CtlSetWNDRect(WND wnd, rint32 x, rint32 y, rint32 w_width, rint32 w_height,rint32 *pErr)
{
    rect clipping;
    rint32 width=0, height=0;

    CHECK_NULLERR_VOID(pErr);

    *pErr = CTLERR_NONE;
    CHECK_NULLPTR_VOID(wnd,*pErr);
    //CHECK_ERR_IF_FALSE_VOID(x>0 && y>0 && w_width > 0 && w_height >0);

    wnd->frame.left = x;
    wnd->frame.right = x + w_width - 1;
    wnd->frame.top = y;
    wnd->frame.bottom = y + w_height - 1;

    GdiGetScreenInfo(&width, &height);
    clipping.left = 0;
    clipping.right = width - 1;
    clipping.top = 0;
    clipping.bottom = height - 1;

    GdiIntersectRect(&(wnd->clip), &(wnd->frame), &clipping);

    //wnd->window_type = WND_PANEL;
    wnd->dev = NULL;
    wnd->window_visible = WND_VISIBLE;
}

/** 
 * @brief		set control window color
 * @param[in]	wnd			control window pointer
 * @param[in]	fgColor		control window foreground color
 * @param[in]	bkColor		control window background color
 * @param[out]	pErr		error pointer
 * @remarks		
 * @see
 */
void CtlSetWNDColor(WND wnd, rint32 fgColor, rint32 bkColor, rint32 *pErr)
{
    CHECK_NULLERR_VOID(pErr);
    CHECK_NULLPTR_VOID(wnd,*pErr);

    wnd->fg_color = RGBVALTODEV(fgColor);
    wnd->bg_color = RGBVALTODEV(bkColor);
}



void CtlDebugPuts(const char *szFile, rint32 nLine, const char *szStr)
{
    char buff[128] = {0,};
    if(szStr==NULL||*szStr=='\0')
        return;

    TrcPrintF(buff,"[CTLERR] %s : %d : %s",szFile, nLine,szStr);

#ifdef WIN32
    OutputDebugString((LPCTSTR)buff); //Beep(750,300);
#endif
}


void CtlDebugStr(const char *szStr)
{
    char buff[128] = {0,};
    if(szStr==NULL||*szStr=='\0')
        return;

    TrcPrintF(buff,"[CTL] %s \n",szStr);

#ifdef WIN32
    OutputDebugString((LPCTSTR)buff); //Beep(750,300);
#endif

}

char *CtlGetErrStr(rint32 nErr)
{
    static char szErr[16] = {0,};

    switch(nErr)
    {
    case CTLERR_NONE:
        StrCpy(szErr,"CTLERR_NONE");
        break;
    case CTLERR_WND:
        StrCpy(szErr,"CTLERR_WND");
        break;
    case CTLERR_NULLPTR:
        StrCpy(szErr,"CTLERR_NULLPTR");
        break;
    case CTLERR_CREATE:
        StrCpy(szErr,"CTLERR_CREATE");
        break;
    default:
        StrCpy(szErr,"CTLERR_UNKNOWEN");
    }

    return szErr;
}

void CtlSetLastError(rint32 pErr)
{
	gCtlLastError = pErr;
}

rint32 CtlGetLastError(void)
{
	return gCtlLastError;
}


#ifdef UNIT_TEST

rint32 CoreCb(WND wnd,rint32 nMsg, rint32 iwParam,rint32 ilParam,rint32 isPre);

void CtlCoreUnitTest(WND hWnd)
{
	WND wnd;
    rint32 nErr = CTLERR_NONE;
    wnd = CtlCreate(hWnd, CTL_TYPE_LIST, NULL, (CTL_CB_PROC)CoreCb,&nErr);

    if(nErr != CTLERR_NONE)
    {
        CTL_DEBUG_STR("CtlCreate error");
        return;
    }
    else
    {
        CTL_DEBUG_STR("CtlCreate OK");
    }

    CtlDraw(wnd,&nErr);
    if(nErr != CTLERR_NONE)
    {
        CTL_DEBUG_STR("CtlDraw error");
        return;
    }
    else
    {
        CTL_DEBUG_STR("CtlDraw OK");
    }

    CtlDestroy(wnd,&nErr);
    if(nErr != CTLERR_NONE)
    {
        CTL_DEBUG_STR("CtlDestroy error");
        return;
    }
    else
    {
        CTL_DEBUG_STR("CtlDestroy OK");
    }
}


rint32 CoreCb(WND wnd,rint32 nMsg, rint32 iwParam,rint32 ilParam,rint32 isPre)
{
    if(wnd==NULL && nMsg != AWM_CREATE)
    {
        CTL_DEBUG_STR("application callback");
        return CTLERR_NULLPTR;
    }

    return CTLERR_NONE;
}

#endif

/** @} */ // End of Control Core
