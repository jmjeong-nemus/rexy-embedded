/*   
 * * $Id: rexy_ctl_slidetext.h 491 2006-07-07 11:11:11Z wijang $
 * Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.    
 *   
 * This file is a part of Rexy Embedded.    
 *   
 */

#ifndef __REXY_CTL_SLIDETEXT_H__
#define __REXY_CTL_SLIDETEXT_H__

/** @file */

#include "rexy.h"
#include "rexy_ctl_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup SlideTextCtlGroup	Ctrl Slide Text Control
 * Ctrl Slide Text Control
 * @ingroup ControlGroup
 * @{
 */

/**
* @}
* */


/**
 * @defgroup MacroGroup Slide Text Default Option
 * Slidetext Control
 * @ingroup SlideTextCtlGroup
 * @{
 */
#define CTL_SLIDETEXT_STOP					CTL_SLIDETEXT_MOVE_STOP		/**< 슬라이드 정지모드 매크로 설정 */		
#define CTL_SLIDETEXT_PAUSE					CTL_SLIDETEXT_MOVE_PAUSE	/**< 슬라이드 일시정지 모드 매크로 설정   */
#define CTL_SLIDETEXT_PLAY					CTL_SLIDETEXT_MOVE_PLAY 	/**< 슬라이드 재생모드 매크로 설정  */

#define CTL_SLIDETEXT_FRONT					CTL_SLIDETEXT_START_FRONT	/**< 슬라이드 문자의 시작(앞) 매크로 설정 */
#define CTL_SLIDETEXT_END					CTL_SLIDETEXT_START_END		/**< 슬라이드 문자의 시작(뒤) 매크로 설정 */
/**
* @}
* */


#define CTL_SLIDETEXT_DEFAULT_TYPE			CTL_SLIDETEXT_TYPE_LFRONT	/**< 슬라이드 문자의 방향 (left -> front)설정 */

#define CTL_SLIDETEXT_DEFAULT_X				0							/**< 텍스트 시작위치의 기본 X 좌표 */
#define CTL_SLIDETEXT_DEFAULT_Y				0							/**< 텍스트 시작위치의 기본 Y 좌표 */
#define CTL_SLIDETEXT_COLOR_FG				0x0000						/**< 텍스트 기본 색상 */

#define CTL_SLIDETEXT_DEFAULT_TOP			0							/**< 컨트롤 윈도우의 기본 X 좌표 */
#define CTL_SLIDETEXT_DEFAULT_LEFT			0							/**< 컨트롤 윈도우의 기본 Y 좌료 */
#define CTL_SLIDETEXT_DEFAULT_RIGHT			0							/**< 컨트롤 윈도우의 기본 Width 영역 */
#define CTL_SLIDETEXT_DEFAULT_BOTTOM		0							/**< 컨트롤 윈도우의 기본 Height 영역 */
#define CTL_SLIDETEXT_COLOR_BG				0xFFFF						/**< 컨트롤 윈도우의 기본 색상 */	


#define CTL_SLIDETEXT_DEFAULT_TIMEOUT		1000						/**< 슬라이드 기본 인터벌 시간 */
#define CTL_SLIDETEXT_DEFAULT_SSFLAG		CTL_SLIDETEXT_MOVE_STOP		/**< 슬라이드 기본 시작/정지/일시정지 플래그 */	
#define CTL_SLIDETEXT_DEFAULT_FEFLAG		CTL_SLIDETEXT_FRONT			/**< 슬라이드 기본 시작(앞/뒤)위치 */	
#define CTL_SLIDETEXT_DEFAULT_MAXWIDTH		0							/**< 슬라이드를 하기 위한 기본 문자열 최대 길이 */
						
#define CTL_SLIDETEXT_DEFAULT_CURRWIDTH		0							/**< 현재 컨틀롤 윈도우의 Width 영역 */
#define CTL_SLIDETEXT_DEFAULT_CURRMAXWIDTH	0							/**< 현재 문자열의 최대 길이 */
#define CTL_SLIDETEXT_DEFAULT_CURRX			0							/**< 현재 텍스트 X 좌표 값 */


/**
 * @defgroup ModeGroup Slide Text Play Mode
 * Slide Text Control Play Mode
 * @ingroup SlideTextCtlGroup
 * @{
 */

	enum 
	{
		CTL_SLIDETEXT_MOVE_STOP,		/**< 슬라이드를 멈춘다. MODE가 PLAY또는 PAUSE 상태일 때 적용된다.*/
		CTL_SLIDETEXT_MOVE_PAUSE,		/**< 슬라이드를 일시정지 한다. MODE가 PLAY 상태일 때 적용된다. */
		CTL_SLIDETEXT_MOVE_PLAY,		/**< 슬라이드를 시작한다. MODE가  STOP또는 PAUSE 상태일 때 적용된다.*/
	};

	enum 
	{
		CTL_SLIDETEXT_START_FRONT,		/**< 슬라이드를 텍스트의 X 좌료에서 시작 한다 */
		CTL_SLIDETEXT_START_END			/**< 슬라이드를 컨트롤 윈도우의 Width 영역 끝에서 시작 한다 */
	};

	enum 
	{
		CTL_SLIDETEXT_TYPE_LFRONT,		/**< 슬라이드 방향을 왼쪽으로 움직이며 슬라이드가 끝나면 처음 위치에서 반복한다 */
		CTL_SLIDETEXT_TYPE_LROUND		/**< 슬라이드 방향을 왼쪽으로 움직이며 계속 순환 한다 */
	};

/**
* @}
* */

/**
 * @defgroup CmdGroup Slide Text Control Command
 * Slide Text Control Command
 * @ingroup SlideTextCtlGroup
 * @{
 */
	enum 
	{
		CTL_SLIDETEXT_CMD_DO_DRAW	= 0x10	/**< 컨트롤 윈도우를 그리기 위한 명령어 */
	};



	enum 
	{
		// Text Properties
		CTL_SLIDETEXT_CMD_SET_X = 0x0100,	/**< 텍스트의 X 좌표를 설정하기 위한 명령어 */
		CTL_SLIDETEXT_CMD_SET_Y,			/**< 텍스트의 Y 좌표를 설정하기 위한 명령어 */
		CTL_SLIDETEXT_CMD_SET_FGCOLOR,		/**< 텍스트의 색상을 설정하기 위한 명령어 */


		// Bg Properties
		CTL_SLIDETEXT_CMD_SET_TOP,			/**< 컨트롤 윈도우의 X 영역을 설정하기 위한 명령어 */
		CTL_SLIDETEXT_CMD_SET_LEFT,			/**< 컨트롤 윈도우의 Y 영역을 설정하기 위한 명령어 */
		CTL_SLIDETEXT_CMD_SET_RIGHT,		/**< 컨트롤 윈도우의 Width 영역을 설정하기 위한 명령어 */	
		CTL_SLIDETEXT_CMD_SET_BOTTOM,		/**< 컨트롤 윈도우의 Height 영역을 설정하기 위한 명령어 */
		CTL_SLIDETEXT_CMD_SET_BGCOLOR,		/**< */

		// slide Properties
		CTL_SLIDETEXT_CMD_DO_LFRONT,		/**< 슬라이드 방향을 왼쪽으로 움직이며 슬라이드가 끝나면 처음 위치에서 시작하기 위한 명령어*/
		CTL_SLIDETEXT_CMD_DO_LROUND,		/**< 슬라이드 방향을 왼쪽으로 움직이며 계속 순환 하기 위한 명령어 */		

		
		CTL_SLIDETEXT_CMD_SET_TIMEOUT,		/**< 슬라이드의 인터벌 시간을 설정하기 위한 명령어 */

		CTL_SLIDETEXT_CMD_DO_PLAY,			/**< 슬라이드를 플레이 하기 위한 명령어 */
		CTL_SLIDETEXT_CMD_DO_STOP,			/**< 슬라이드를 정지 하기 위한 명령어 */
		CTL_SLIDETEXT_CMD_DO_PAUSE,			/**< 슬라이드를 일시정지 하기 위한 명령어 */

		CTL_SLIDETEXT_CMD_GET_MODE,			/**< 슬라이드의 현재 MODE 상태를 얻어 오기 위한 명령어 */

		CTL_SLIDETEXT_CMD_DO_START_FRONT,	/**< 슬라이드의 시작위치를 텍스트의 X 좌표 에서 시작 한다 */
		CTL_SLIDETEXT_CMD_DO_START_END,		/**< 슬라이드의 시작위치를 커너트롤 윈도우의 Width 영역 끝에서 시작 한다 */
	
		CTL_SLIDETEXT_CMD_SET_MAXWIDTH,		/**< 슬라이드할 문자열의 크기를 설정하기 위한 명령어 */
		CTL_SLIDETEXT_CMD_SET_DATA,			/**< 슬라이드할 문자열을 설정하기 위한 명령어 */
		//
		CTL_SLIDETEXT_CMD_SET_WNDRECT,		/**< 컨트롤 윈도우의 좌표와 크기 정보를 한번에 설정하기 위한 명령어 */

		// callback function property
		CTL_SLIDETEXT_CMD_SET_CB_DRAW,		/**< 사용자 화면 그리기 콜백 함수 등록을 위한 명령어 */
		CTL_SLIDETEXT_CMD_SET_DB_KEYDOWN,	/**< 사용자 키 처리 콜백 함수 등록을 위한 명령어 */

		// end fo value
		CTL_SLIDETEXT_CMD_SET_LAST
	};
/**
* @}
* */

/**
 * @defgroup CBGroup Slide Text Callback API
 * Slide Text Control Callback API
 * @ingroup SlideTextCtlGroup
 * @{
 */
typedef int (*CtlSlideTextDrawCB)(WND hWnd, COLORVAL nFgColor, COLORVAL nBgColor); /**< 사용자 그리기 콜백 함수 포인터 */
typedef int (*CtlSlideTextKeyDownCB)(WND hWnd, int wparam); /**< 사용자 키 처리 콜백 함수 포인터 */

/**
* @}
* */


/*
 * 슬라이딩 하고자 하는 텍스트가 어디에 위치하며 (x,y)
 * 얼마의 길이를 가지고 있으며, 텍스트 생삭은 무엇이며, 
 * 텍스트가 위치한 곳은 어디며, 텍스트가 Unicode 인지 아닌지의
 * 속성을 가지고 있다.
*/
	typedef struct _REXY_CTL_SLIDETEXT_INFO
	{
		rint16 nX;				/** X position     */
		rint16 nY;				/** Y position     */	
		COLORVAL nFgColor;		/** String Color   */
	} REXY_CTL_SLIDETEXT_INFO;

/* 
 *슬라이딩 하고자 하는 배경 영역을 지정하는 속성이며,
 * 배경 영역의 기본 속성 ( top, left, right, bottom )과 배경 색상 
 * 정보를 가진다.
*/
 	typedef struct _REXY_CTL_SLIDETEXT_BG_INFO
  	{
		rint16 nTop;			/** Top info   */
		rint16 nLeft;			/** Left info  */
		rint16 nRight;			/** Right info */
		rint16 nBottom;			/** Bottom info */
		ruint16 nBgColor;		/** Bg Color */
  	} REXY_CTL_SLIDETEXT_BG_INFO;


	typedef struct _REXY_CTL_PRV_INFO
	{
		rint16 nCurrWidth;		/** Current text width */
		rint16 nCurrMaxWidth;
		rint16  nCurrX;			/** Current x positon */
		rint16	nPauseX;		
	} REXY_CTL_PRV_INFO;


	typedef struct _REXY_CTL_SLIDETEXT
	{
		ruint16 nType;					/** Slide info ( left, right, round ) */
		ruint16 nTimeout; 				/** Slide speed */
		ruint8  nSSFlag;				/** Slide start / stop flags */
		ruint8	nFEFlag;				/** Slide Front / End flags */
		ruint16 nMaxWidth;			
		void *pStr;						/** String pointer */

		CtlSlideTextDrawCB	pfnDraw;
		CtlSlideTextKeyDownCB pfnKeyDown;
		
		REXY_CTL_SLIDETEXT_INFO sText;
		REXY_CTL_SLIDETEXT_BG_INFO sBg;
		REXY_CTL_PRV_INFO sPrv;
	} REXY_CTL_SLIDETEXT;

	int CtlSlideTextProc(WND wnd, int nMsg, int wparam, int lparam);
	int CtlSlideTextCBProc(WND wnd, int nMsg, int wparam, int lparam, int bIsPre, int *pErr);
	int CtlSlideTextCmdProc(WND wnd, int nCommand,int wParam, int lParam, int *pErr);


#ifdef __cplusplus
}
#endif	// __cplusplus
#endif	// __REXY_CTL_SLIDETEXT_H__
                              
