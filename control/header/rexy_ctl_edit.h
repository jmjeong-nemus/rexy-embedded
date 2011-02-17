/*   
 * $Id: rexy_ctl_CtlEditor.h 491 2006-01-11 16:32:34Z madholic $
 * Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.    
 *   
 * This file is a part of Rexy Embedded.    
 */

/*************************************************************************
 * REXY/E Text Edit Control
 *
 *  기능: 
 *		+ 텍스트 편집
 *		+ Single Line/Multi Line 지원
 *		+ 스크롤 기능 지원
 *		+ Backspace, Delete 키 처리
 *	
 *  앞으로 추가될 기능
 *		+ 다양한 종류의 입력기와의 연동 고려
 ************************************************************************/

#ifndef __REXY_CTL_EDIT_H__
#define __REXY_CTL_EDIT_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @file */
	
#include "rexy.h"
#include "rexy_ctl_core.h"
#include "rexy_haldevice.h"

/**
 * @defgroup EditCtlGroup Ctrl Edit Control
 * Ctrl Edit Control
 * @ingroup ControlGroup
 * @{
 */

/**
 * @enum EditControlAttr
 * Edit Control을 설정하고 제어하기 위한 명령어 집합
 */
enum EditControlAttr{
	//Start offset 0x0100 for no reason.
	// 필드
	CTL_EDIT_CMD_GET_WND_RECT = 0x0100,	///< WIN RECT 정보 설정.
	CTL_EDIT_CMD_SET_WND_RECT,			///< WIN RECT 정보 설정.

    CTL_EDIT_CMD_GET_STRING,			///< 문자열
    CTL_EDIT_CMD_SET_STRING,			///< 문자열

    CTL_EDIT_CMD_GET_MAX_LEN,			///< 최대 길이
    CTL_EDIT_CMD_SET_MAX_LEN,			///< 최대 길이

	CTL_EDIT_CMD_GET_PASSWORD,			///< Password 입력모드 여부
	CTL_EDIT_CMD_SET_PASSWORD,			///< Password 입력모드 여부

	CTL_EDIT_CMD_GET_READONLY,			///< 읽기 전용 여부
	CTL_EDIT_CMD_SET_READONLY,			///< 읽기 전용 여부

    CTL_EDIT_CMD_GET_CURSOR_INDEX,		///< 문자열내의 커서의 위치
    CTL_EDIT_CMD_SET_CURSOR_INDEX,		///< 문자열내의 커서의 위치

	CTL_EDIT_CMD_GET_MULTILINE,			///< 싱글/멀티 라인 여부
	CTL_EDIT_CMD_SET_MULTILINE,			///< 싱글/멀티 라인 여부

	// 명령
    CTL_EDIT_CMD_DO_DRAW,				///< 화면 그리기

	// 콜백 함수
	CTL_EDIT_CMD_GET_CB_DRAW,			///< 화면 그리기 콜백 함수
	CTL_EDIT_CMD_SET_CB_DRAW,			///< 화면 그리기 콜백 함수

	CTL_EDIT_CMD_GET_CB_KEYDOWN,		///< 키 입력 처리 콜백 함수
	CTL_EDIT_CMD_SET_CB_KEYDOWN,		///< 키 입력 처리 콜백 함수

    CTL_EDIT_CMD_LAST					// Does nothing.
};

/** Drawing callback function pointer */
typedef int (*CtlEditCBDraw)(WND hCtlWnd, void *pString, int nLength);
/** Keydown callback function pointer */
typedef int (*CtlEditCBKeyDown)(WND hCtlWnd, int nKeyCode, void *pString, int *pLength);

/** @} */

int CtlEditProc(WND wnd, int nMsg, int wparam, int lparam);
int CtlEditCBProc(WND wnd, int nMsg, int wparam, int lparam, int bIsPre, int *pErr);
int CtlEditCmdProc(WND wnd, int nCmd, int wParam, int lParam, int *pErr);

#ifdef UNIT_TEST
void CtlEditTestUnit(void);
#endif  // UNIT_TEST

#ifdef __cplusplus
}
#endif	// __cplusplus
#endif	// __REXY_CTL_EDIT_H__

