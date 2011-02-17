/*   
 * * $Id: rexy_ctl_anibmp.h 2006-07-10 19:40 hakeya $
 * Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.    
 *   
 * This file is a part of Rexy Embedded.    
 *   
 */

#ifndef __REXY_CTL_ANIBMP_H__
#define __REXY_CTL_ANIBMP_H__

/** @file */

#include "rexy.h"
#include "rexy_ctl_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup AniBmpCtlGroup Ctrl Animate Bitmap Control
 * Ctrl Animate Bitmap Control
 * @ingroup ControlGroup
 * @{
 */

/**
 * @def CTL_ANIBMP_DEFAULT_TIMEOUT
 *	Default Timeout(=500)
 * @def CTL_ANIBMP_DEFAULT_TOP
 *	Default Y Position(=0)
 * @def CTL_ANIBMP_DEFAULT_LEFT
 *	Default X Position(=0)
 * @def CTL_ANIBMP_DEFAULT_WIDTH
 *	Default Width of area to display image(=Screen Width)
 * @def CTL_ANIBMP_DEFAULT_HEIGHT
 *	Default Height of area to display image(=Screen Height)
 * @def CTL_ANIBMP_DEFAULT_BGCOLOR
 *	Default Background Color of Control Window(WHITE)
 * @def CTL_ANIBMP_DEFAULT_PLAY_MODE
 *	Default Play Mode(CTL_ANIBMP_PLAY_NORMAL)
 */
#define CTL_ANIBMP_DEFAULT_TIMEOUT		500
#define CTL_ANIBMP_DEFAULT_TOP			0
#define CTL_ANIBMP_DEFAULT_LEFT			0
#define CTL_ANIBMP_DEFAULT_WIDTH		FtrGetSystemFeature(FEATURE_LCD_WIDTH)
#define CTL_ANIBMP_DEFAULT_HEIGHT		FtrGetSystemFeature(FEATURE_LCD_HEIGHT)
#define	CTL_ANIBMP_DEFAULT_BGCOLOR		COLOR_WHITE

#define CTL_ANIBMP_DEFAULT_PLAY_MODE	CTL_ANIBMP_PLAY_NORMAL


/**
 * @enum PlayMode
 * Animate bitmap control의 재생 방법을 정의.
 */
enum PlayMode
{
	CTL_ANIBMP_PLAY_NORMAL,				///< 정방향 1회 재생
	CTL_ANIBMP_PLAY_ROTATE,				///< 정방향 순환
	CTL_ANIBMP_PLAY_REVERSE,			///< 역방향 1회 재생
	CTL_ANIBMP_PLAY_REVERSE_ROTATE,		///< 역방향 순환
	CTL_ANIBMP_PLAY_RANDOM				///< 무작위 재생
};

/**
 * @enum Command
 * Animate Bitmap Control을 설정하고 제어하기 위한 명령어 집합
 */
enum Command
{
	CTL_ANIBMP_CMD_SET_LEFT,			///< 콘트롤 윈도우에서 이미지가 출력될 X좌표를 설정
	CTL_ANIBMP_CMD_SET_TOP,				///< 콘트롤 윈도우에서 이미지가 출력될 Y좌표를 설정
	CTL_ANIBMP_CMD_SET_WIDTH,			///< 이미지가 출력될 영역의 폭 설정
	CTL_ANIBMP_CMD_SET_HEIGHT,			///< 이미지가 출력될 영역의 높이 설정
	CTL_ANIBMP_CMD_SET_BGCOLOR,			///< 콘트롤 윈도우의 배경색 설정

	CTL_ANIBMP_CMD_SET_FRAME_NO,		///< 전체 프래임의 수 설정
	CTL_ANIBMP_CMD_SET_P_IDS,			///< 이미지 배열 포인터
	CTL_ANIBMP_CMD_SET_PLAYTYPE,		///< 재생모드 설정
	CTL_ANIBMP_CMD_SET_TIMEOUT,			///< 한장의 이미지가 화면에 보여지는 시간 설정
	CTL_ANIBMP_CMD_SET_P_TIMEOUT,		///< 각 이미지가 화면에 보여지는 시간 설정

	CTL_ANIBMP_CMD_SET_WNDRECT,			///< 콘트롤 윈도우의 좌표 및 영역 설정
	CTL_ANIBMP_CMD_SET_CB_KEYDOWN,		///< Key Down Callback 함수 등록
	CTL_ANIBMP_CMD_START_STOP,			///< Play/Stop Animation(Start/Stop Timer)

	CTL_ANIBMP_CMD_LAST
};


// Animation Bitmap KeyDown Callback Function. 2006-07-20 added by hakeya
// This function may be fill at application.
/** @fn typedef int (*CtlAniBmpKeyDownCB)(WND hWnd, int wparam, int *pErr)
 * @brief	User Callback function to process key down event at anibmp control
 * @param[in]	hWnd	Window Handle
 * @param[in]	wparam	Key Code
 * @param[out]	pErr	Error Value
 * @remark	This function may be fill at application.\n
 *	User can use Control core function and Command for Anibmp Control in this function.\n
 */
typedef int (*CtlAniBmpKeyDownCB)(WND hWnd, int wparam, int *pErr);

/** @} */ // end of anibmpctl group

// Animation Bitmap Control Main 구조체
typedef struct
{
	ruint16	nIdx;
	bool	bIsTimer;
	CtlAniBmpKeyDownCB pfnKeyDown;

	rint16 nLeft;				///< 윈도우에서 이미지가 출력될 영역의 x좌표
	rint16 nTop;				///< 윈도우에서 이미지가 출력될 영역의 y좌표
	ruint16 nWidth;				///< 윈도우에서 이미지가 출력될 폭
	ruint16 nHeight;			///< 윈도우에서 이미지가 출력될 높이
	COLORVAL bgColor;			///< 윈도우의 바탕색

	ruint16	nFrameNo;			///< 재생될 전체 이미지 수
	int*	pIDs;				///< 이미지의 ID를 가지고 있는 배열을 가리키는 포인터
	int		nPlayType;			///< 재생 모드
	int		nTimeout;			///< 한장의 이미지가 화면에 보여지는 시간(milli second)
	int*	pTimeout;			///< 각 이미지가 보여지는 시간값을 가지는 배열을 가리키는 포인터
} REXY_CTL_ANIBMP;

// Procedure
int CtlAniBmpProc(WND wnd, int nMsg, int wparam, int lparam);
int CtlAniBmpCBProc(WND wnd, int nMsg, int wparam, int lparam, int bIsPre, int *pErr);
int CtlAniBmpCmdProc(WND wnd, int nCommand,int wParam, int lParam, int *pErr);

#ifdef __cplusplus
}
#endif	// __cplusplus
#endif	// __REXY_CTL_ANIBMP_H__
                              
