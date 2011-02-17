/*   
 * $Id: rexy_ctl_core.h 491 2006-01-11 16:32:34Z madholic $
 * Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.    
 *   
 * This file is a part of Rexy Embedded.    
 *   
 */

#ifndef __REXY_CTL_CORE_H__
#define __REXY_CTL_CORE_H__

/** @file */

#include "rexy_error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup ControlCoreGroup Ctrl Control Core 
 * Ctrl Control Core
 * @ingroup ControlGroup
 * @{
 */


     //	Control Core Callback function
    	
    //	@param wnd control window pointer
    //	@param nMsg system message
	//	@param nCommand control command
    //	@param wParam wParam
    //	@param lParam lParam
    //	@param bIsPre TRUE : pre callback, FALSE : post callback
    // */
	

	/** Main message handler */
    typedef rint32 (*CTL_PROC) (WND wnd, rint32 nMsg, rint32 wparam, rint32 lparam);
	/** Callback message handler */
    typedef rint32 (*CTL_CB_PROC) (WND wnd, rint32 nMsg, rint32 wparam, rint32 lparam, rint32 bIsPre, rint32 *pErr);
	/** Command handler */
    typedef rint32 (*CTL_COMMAND_PROC) (WND wnd, rint32 nCommand,rint32 wParam, rint32 lParam, rint32 *pErr);



    /* Function Prototype */
    rint32 CtlWinProc(WND wnd, rint32 msg, rint32 wparam, rint32 lparam);
	WND		CtlCreate(WND wnd, rint32 nType, rect *location, CTL_CB_PROC pfnProc, rint32 *pErr);
	WND		CtlCreateEx(WND wnd, rint32 nType, rint32 id, rint32 style, rect *location, CTL_CB_PROC pfnProc, rint32 *pErr);
    void	CtlDestroy(WND wnd, rint32 *pErr);
    void	CtlDraw(WND wnd, rint32 *pErr); // AWM_PAINT
    void	CtlLoad(WND wnd, rint32 *pErr); // AWM_LOAD
    void	CtlSave(WND wnd, rint32 *pErr); // AWM_SAVE
	void	CtlKeyDown(WND wnd, rint32 *pErr);	// AWM_KEYDOWN
    rint32 	CtlProcessEvent(WND wnd, rint32 *pErr,rint32 nMsg,rint32 iwParam,rint32 ilParam);

    void	CtlSetPtr(WND wnd, rint32 nCommand,void * pValue, rint32 *pErr);
    void*	CtlGetPtr(WND wnd, rint32 nCommand, rint32 *pErr);
    
	void	CtlSeti(WND wnd, rint32 nCommand,rint32 iValue,rint32 *pErr);
    rint32		CtlGeti(WND wnd, rint32 nCommand, rint32 *pErr);
	
	void	CtlSetiPos(WND wnd, rint32 nCommand, rint32 iPos, rint32 iValue, rint32 *pErr);
	rint32		CtlGetiPos(WND wnd, rint32 nCommand, rint32 iPos, rint32 *pErr);

	void	CtlSetiPtr(WND wnd, rint32 nCommand, rint32 iPos, void* pValue, rint32 *pErr);
	void*	CtlGetiPtr(WND wnd, rint32 nCommand, rint32 iPos, rint32 *pErr);

    rint32		CtlDo(WND wnd, rint32 nCommand, rint32 wParam, rint32 lParam, rint32 *pErr);

    // Window Function
    void	CtlSetWNDRect(WND wnd, rint32 x, rint32 y, rint32 w_width, rint32 w_height, rint32 *pErr);
    void	CtlSetWNDColor(WND wnd, rint32 fgColor, rint32 bkColor, rint32 *pErr);

	// 
	void*	CtlGetStruct(WND wnd);

	// Error Setting
	void	CtlSetLastError(rint32 pErr);
	rint32		CtlGetLastError(void);



/** @} */

#define CTL_TYPE_LIST				WND_USERCONTROL + 0x00		///< List Control
#define CTL_TYPE_POPUP				WND_USERCONTROL + 0x01		///< Popup Control
#define CTL_TYPE_EDIT				WND_USERCONTROL + 0x02		///< Edit Control
#define CTL_TYPE_SLIDETEXT			WND_USERCONTROL + 0x03		///< Sliding Text Control
#define CTL_TYPE_ANIBMP				WND_USERCONTROL + 0x04		///< Animated Bitmap Control


#define CTL_MSG_START				AWM_USER_CONTROL
#define CTL_MSG_LIST				CTL_MSG_START
#define CTL_MSG_POPUP				CTL_MSG_START + 100
#define CTL_MSG_EDIT				CTL_MSG_START + 200
#define CTL_MSG_SLIDETEXT			CTL_MSG_START + 300
#define CTL_MSG_ANIBMP				CTL_MSG_START + 400

#define CTLERR_NONE				RxErrNone
#define CTLERR_WND				CTLERR_BASE + 1
#define CTLERR_NULLPTR			CTLERR_BASE + 2
#define CTLERR_CREATE			CTLERR_BASE + 3 // Control Procedure AWM_CREATE ERROR
#define CTLERR_GETCTLPROC       CTLERR_BASE + 4
#define CTLERR_GETI             CTLERR_BASE + 5
#define CTLERR_SETI             CTLERR_BASE + 6
#define CTLERR_GETPTR           CTLERR_BASE + 7
#define CTLERR_SETPTR           CTLERR_BASE + 8

    void CtlDebugPuts(const char *szFile, rint32 nLine, const char *szStr);
    void CtlDebugStr(const char *szStr);
    char *CtlGetErrStr(rint32 nErr);

#define CTL_DEBUG
//#define UNIT_TEST

#ifdef CTL_DEBUG

#define CTL_DEBUG_STR(s) \
        CtlDebugStr(s)

#define CTL_DEBUG_ERROUT(f,l,s) \
        CtlDebugPuts(f,l,s)
#else	// CTL_DEBUG

#define CTL_DEBUG_ERROUT(f,l,s) 
#define CTL_DEBUG_STR(s)

#endif	// CTL_DEBUG


#ifndef CTL_DEBUG

#define CHECK_ERR_IF_EQUAL(e,v,r) if(e == v) return r;
#define CHECK_ERR_IF_NOT_EQUAL(e,v,r) if(e != v) return r;

#define CHECK_ERR(e,v,r) CHECK_ERR_IF_NOT_EQUAL(e,v,r)
#define CHECK_ERR_VOID(e,v) if(e != v) return ;

#define CHECK_ERR_IF_FALSE(c,r) \
        do{ if(!c) return r; } while(0);

#define CHECK_ERR_IF_TRUE(c,r) \
        do{ if(c) return r; } while(0);

#define CHECK_ERR_IF_FALSE_VOID(c,r) \
        do{ if(!c) return ; } while(0);

#define CHECK_ERR_IF_TRUE_VOID(c,r) \
        do{ if(c) return ; } while(0);


#define CHECK_NULLERR(v,r) if(v==NULL) return r;
#define CHECK_NULLERR_VOID(v) if(v==NULL) return;

#define CHECK_NULLPTR(v,r,e) if(v==NULL) { e=CTLERR_NULLPTR;  return r;}
#define CHECK_NULLPTR_VOID(v,e) if(v==NULL){ e=CTLERR_NULLPTR; return; }


#else	// CTL_DEBUG

#define CHECK_ERR_IF_EQUAL(e,v,r) \
        if(e == v) \
        { \
            CTL_DEBUG_ERROUT(__FILE__,__LINE__, #e "==" #v "\n"); \
            return r; \
        } \

#define CHECK_ERR_IF_NOT_EQUAL(e,v,r) \
        if(e != v) \
        { \
            CTL_DEBUG_ERROUT(__FILE__,__LINE__, #e "!=" #v "\n"); \
            return r; \
        } \

#define CHECK_ERR(e,v,r) CHECK_ERR_IF_NOT_EQUAL(e,v,r)

#define CHECK_ERR_VOID(e,v) if(e != v) return ;

#define CHECK_ERR_IF_FALSE(c,r) \
        do{ if(!c) return r; } while(0);

#define CHECK_ERR_IF_TRUE(c,r) \
        do{ if(c) return r; } while(0);

#define CHECK_ERR_IF_FALSE_VOID(c) \
        do{ if(!c) return ; } while(0);

#define CHECK_ERR_IF_TRUE_VOID(c) \
        do{ if(c) return ; } while(0);


#define CHECK_NULLERR(v,r) if(v==NULL) return r;
#define CHECK_NULLERR_VOID(v) if(v==NULL) return;

#define CHECK_NULLPTR(v,r,e) if(v==NULL) { e=CTLERR_NULLPTR;  return r;}
#define CHECK_NULLPTR_VOID(v,e) if(v==NULL){ e=CTLERR_NULLPTR; return; }

#endif	// CTL_DEBUG



#ifdef __cplusplus
}
#endif	// __cplusplus
#endif	// __REXY_CTL_CORE_H__
