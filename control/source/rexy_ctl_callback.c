/*
 * $Id: rexy_ctl_callback.c 491 2006-07-12 16:32:34Z venabili $
 * Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded.
 *
 */

#include "rexy.h"
#include "rexy_ctl_core.h"
#include "rexy_ctl_list.h"
#include "rexy_ctl_popup.h"
#include "rexy_ctl_slidetext.h"
#include "rexy_ctl_edit.h"
#include "rexy_ctl_anibmp.h"

CTL_PROC CtlGetCtlProc(int nType, int *pErr)
{
    CTL_PROC ctlProc = NULL;
    CHECK_NULLERR(pErr, NULL);

    *pErr = CTLERR_NONE;

    switch(nType)
    {
        // please add control procedure
    case CTL_TYPE_LIST:
        ctlProc = CtlListProc;
        break;

	case CTL_TYPE_POPUP:
        ctlProc = CtlPopupProc;
        break;

	case CTL_TYPE_SLIDETEXT:
		ctlProc = CtlSlideTextProc;
		break;

	case CTL_TYPE_ANIBMP:
		ctlProc = CtlAniBmpProc;
		break;

	case CTL_TYPE_EDIT:
		ctlProc = CtlEditProc;
		break;
    }

    return ctlProc;
}


rint32 CtlWinProc(WND wnd, int msg, int wparam, int lparam)
{
    int err = CTLERR_NONE;
    int handled = FALSE;
   
    // 생성과 소멸의 이벤트에는 특별히 관혀하지 않도록 한다 
    if( msg == AWM_CREATE ||
        msg == AWM_DESTROY )
        return FALSE;
    else
        handled = CtlProcessEvent(wnd, &err, msg, wparam, lparam);
   
    return handled;
}


CTL_CB_PROC CtlGetCtlCBProc(int nType, int *pErr)
{
    CTL_CB_PROC pfnProc = NULL;

    CHECK_NULLERR(pErr,NULL);

    *pErr = CTLERR_NONE;

    switch(nType)
    {
    case CTL_TYPE_LIST:
        pfnProc = CtlListCBProc;
        break;
	
	case CTL_TYPE_POPUP:
		pfnProc = CtlPopupCBProc;
		break;

	case CTL_TYPE_SLIDETEXT:
		pfnProc = CtlSlideTextCBProc;
		break;
	
    case CTL_TYPE_ANIBMP:
		pfnProc = CtlAniBmpCBProc;
		break;

	case CTL_TYPE_EDIT:
		pfnProc = CtlEditCBProc;
		break;
    }

    return pfnProc;
}

CTL_COMMAND_PROC CtlGetCtlCmdProc(int type, int *pErr)
{
    CTL_COMMAND_PROC pfnCtlComProc = NULL;
    CHECK_NULLERR(pErr,NULL); // ASSERT

    switch(type)
    {
    case CTL_TYPE_LIST:
        pfnCtlComProc = CtlListCmdProc;
        break;

	case CTL_TYPE_POPUP:
		pfnCtlComProc = CtlPopupCmdProc;
		break;

	case CTL_TYPE_SLIDETEXT:
		pfnCtlComProc = CtlSlideTextCmdProc;
		break;

	case CTL_TYPE_ANIBMP:
		pfnCtlComProc = CtlAniBmpCmdProc;
		break;

	case CTL_TYPE_EDIT:
		pfnCtlComProc = CtlEditCmdProc;
		break;
    }

    return pfnCtlComProc;
}

