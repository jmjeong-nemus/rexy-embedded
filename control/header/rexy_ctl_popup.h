/*   
 * $Id: rexy_ctl_popup.h 491 2006-01-11 16:32:34Z madholic $
 * Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.    
 *   
 * This file is a part of Rexy Embedded.    
 *   
 */

#ifndef __REXY_CTL_POP_H__
#define __REXY_CTL_POP_H__

/** @file */

#include "rexy.h"
#include "rexy_ctl_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup PopupCtlGroup Ctrl Popup Control
 * Ctrl Popup Control
 * @ingroup ControlGroup
 * @{
 */

#define CTL_POPUP_BASE		CTLERR_BASE + 0x0100
#define	CTL_POPUP_ERR_NONE	RxErrNone
#define CTL_POPUP_EXIT		CTL_POPUP_BASE + 1

#define CTL_POPUP_DEFAULT_TIMEOUT	2000
#define CTL_POPUP_DEFAULT_TOP		CLIENT2_TOP + 20
#define CTL_POPUP_DEFAULT_LEFT		CLIENT_LEFT + 20
#define CTL_POPUP_DEFAULT_RIGHT		CLIENT_RIGHT - 20
#define CTL_POPUP_DEFAULT_BOTTOM	CLIENT_BOTTOM - 20
#define CTL_POPUP_DEFAULT_BTN_WIDTH		50
#define CTL_POPUP_DEFAULT_BTN_HEIGHT	30
#define CTL_POPUP_DEFAULT_FONT		FntIDAlpha

#define CTL_POPUP_COLOR_FG			COLOR_BLUE
#define CTL_POPUP_COLOR_BG			COLOR_WHITE
#define CTL_POPUP_COLOR_BOX			COLOR_MAGENTA
#define CTL_POPUP_COLOR_BOX2		MAKERGB(64, 64, 64)

#define POPUP_DEFAULT_INTERVAL		2000

	/**
	 * @enum PopupControlType
	 * Popup Control의 타입
	 */
	enum PopupControlType{
		CTL_POPUP_CONFIRM_TYPE,			///< confirm  type
		CTL_POPUP_ALERT_TYPE,			///< alert	  type
		CTL_POPUP_QUESTION_TYPE,		///< question type
		CTL_POPUP_PROGRESS_TYPE,		///< progress type
		CTL_POPUP_CUSTOM_TYPE			///< custom   type
	};

	/**
	 * @enum PopupControlAttr
	 * Popup Control을 설정하고 제어하기 위한 명령어 집합
	 */
	enum PopupControlAttr{
		CTL_POPUP_CMD_SET_TYPE,			///< Set control style
		CTL_POPUP_CMD_SET_RECT,			///< Set the window rectangle of the popup control
		CTL_POPUP_CMD_SET_FGCOLOR,		///< Set foreground color
		CTL_POPUP_CMD_SET_BGCOLOR,		///< Set background color
		CTL_POPUP_CMD_SET_BOXCOLOR,		///< Set bound rectangle color
		CTL_POPUP_CMD_SET_TIMEOUT,		///< Set timeout setting

		CTL_POPUP_CMD_GET_RECT,			///< Get control's rectangle
		CTL_POPUP_CMD_SET_STR,			///< Set control's string
		CTL_POPUP_CMD_GET_STR,			///< Get control's string
		CTL_POPUP_CMD_SET_ID,			///< Set control's id
		CTL_POPUP_CMD_GET_ID,			///< Get control's id

		// added by wijang
		CTL_POPUP_CMD_DO_DRAW,			///< Execute control drawing totally
		
		CTL_POPUP_CMD_SET_CB_DRAW,		///< Set draw callback function pointer
		CTL_POPUP_CMD_SET_CB_KEYDOWN,	///< Set key down callback function pointer
		
		CTL_POPUP_CMD_LAST				// Do nothing
	};

	enum PopupCloseReason{
		CTL_POPUP_OK = 0,
		CTL_POPUP_CANCEL,
		CTL_POPUP_YES,
		CTL_POPUP_NO,
		CTL_POPUP_TIMER,
		CTL_POPUP_SYSTEM,
	};

#define CTL_MSG_POPUP_CLOSE CTL_MSG_POPUP
	
// added by wijang
/** Drawing Callback function prototype */
typedef int (*CtlPopupDrawCB)(WND hWnd);
/** Keydown callback function prototype */
typedef int (*CtlPopupKeyDownCB)(WND hWnd, int wparam);

/** @} */ // end of popupctl group

	typedef struct
	{
		int nType;
		rect win_rect;
		
		COLORVAL fgColor;
		COLORVAL bgColor;
		COLORVAL boxColor;
		WORD* pScreenSaver;		// popup이 그려지기 이전의 화면 dump
		int ePopupType;
		int nTimeout;
		int	nId;
		int nReason;
		char *pStr; // popup string

		CtlPopupDrawCB	pfnDraw;
		CtlPopupKeyDownCB pfnKeyDown;
	} REXY_CTL_POPUP;


	int CtlPopupProc(WND wnd, int nMsg, int wparam, int lparam);
	int CtlPopupCBProc(WND wnd, int nMsg, int wparam, int lparam, int bIsPre, int *pErr);
	int CtlPopupCmdProc(WND wnd, int nCommand,int wParam, int lParam, int *pErr);


#ifdef __cplusplus
}
#endif	// __cplusplus
#endif	// __REXY_CTL_POP_H__
                              
