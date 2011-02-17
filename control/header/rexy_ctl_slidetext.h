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
#define CTL_SLIDETEXT_STOP					CTL_SLIDETEXT_MOVE_STOP		/**< �����̵� ������� ��ũ�� ���� */		
#define CTL_SLIDETEXT_PAUSE					CTL_SLIDETEXT_MOVE_PAUSE	/**< �����̵� �Ͻ����� ��� ��ũ�� ����   */
#define CTL_SLIDETEXT_PLAY					CTL_SLIDETEXT_MOVE_PLAY 	/**< �����̵� ������ ��ũ�� ����  */

#define CTL_SLIDETEXT_FRONT					CTL_SLIDETEXT_START_FRONT	/**< �����̵� ������ ����(��) ��ũ�� ���� */
#define CTL_SLIDETEXT_END					CTL_SLIDETEXT_START_END		/**< �����̵� ������ ����(��) ��ũ�� ���� */
/**
* @}
* */


#define CTL_SLIDETEXT_DEFAULT_TYPE			CTL_SLIDETEXT_TYPE_LFRONT	/**< �����̵� ������ ���� (left -> front)���� */

#define CTL_SLIDETEXT_DEFAULT_X				0							/**< �ؽ�Ʈ ������ġ�� �⺻ X ��ǥ */
#define CTL_SLIDETEXT_DEFAULT_Y				0							/**< �ؽ�Ʈ ������ġ�� �⺻ Y ��ǥ */
#define CTL_SLIDETEXT_COLOR_FG				0x0000						/**< �ؽ�Ʈ �⺻ ���� */

#define CTL_SLIDETEXT_DEFAULT_TOP			0							/**< ��Ʈ�� �������� �⺻ X ��ǥ */
#define CTL_SLIDETEXT_DEFAULT_LEFT			0							/**< ��Ʈ�� �������� �⺻ Y �·� */
#define CTL_SLIDETEXT_DEFAULT_RIGHT			0							/**< ��Ʈ�� �������� �⺻ Width ���� */
#define CTL_SLIDETEXT_DEFAULT_BOTTOM		0							/**< ��Ʈ�� �������� �⺻ Height ���� */
#define CTL_SLIDETEXT_COLOR_BG				0xFFFF						/**< ��Ʈ�� �������� �⺻ ���� */	


#define CTL_SLIDETEXT_DEFAULT_TIMEOUT		1000						/**< �����̵� �⺻ ���͹� �ð� */
#define CTL_SLIDETEXT_DEFAULT_SSFLAG		CTL_SLIDETEXT_MOVE_STOP		/**< �����̵� �⺻ ����/����/�Ͻ����� �÷��� */	
#define CTL_SLIDETEXT_DEFAULT_FEFLAG		CTL_SLIDETEXT_FRONT			/**< �����̵� �⺻ ����(��/��)��ġ */	
#define CTL_SLIDETEXT_DEFAULT_MAXWIDTH		0							/**< �����̵带 �ϱ� ���� �⺻ ���ڿ� �ִ� ���� */
						
#define CTL_SLIDETEXT_DEFAULT_CURRWIDTH		0							/**< ���� ��Ʋ�� �������� Width ���� */
#define CTL_SLIDETEXT_DEFAULT_CURRMAXWIDTH	0							/**< ���� ���ڿ��� �ִ� ���� */
#define CTL_SLIDETEXT_DEFAULT_CURRX			0							/**< ���� �ؽ�Ʈ X ��ǥ �� */


/**
 * @defgroup ModeGroup Slide Text Play Mode
 * Slide Text Control Play Mode
 * @ingroup SlideTextCtlGroup
 * @{
 */

	enum 
	{
		CTL_SLIDETEXT_MOVE_STOP,		/**< �����̵带 �����. MODE�� PLAY�Ǵ� PAUSE ������ �� ����ȴ�.*/
		CTL_SLIDETEXT_MOVE_PAUSE,		/**< �����̵带 �Ͻ����� �Ѵ�. MODE�� PLAY ������ �� ����ȴ�. */
		CTL_SLIDETEXT_MOVE_PLAY,		/**< �����̵带 �����Ѵ�. MODE��  STOP�Ǵ� PAUSE ������ �� ����ȴ�.*/
	};

	enum 
	{
		CTL_SLIDETEXT_START_FRONT,		/**< �����̵带 �ؽ�Ʈ�� X �·ῡ�� ���� �Ѵ� */
		CTL_SLIDETEXT_START_END			/**< �����̵带 ��Ʈ�� �������� Width ���� ������ ���� �Ѵ� */
	};

	enum 
	{
		CTL_SLIDETEXT_TYPE_LFRONT,		/**< �����̵� ������ �������� �����̸� �����̵尡 ������ ó�� ��ġ���� �ݺ��Ѵ� */
		CTL_SLIDETEXT_TYPE_LROUND		/**< �����̵� ������ �������� �����̸� ��� ��ȯ �Ѵ� */
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
		CTL_SLIDETEXT_CMD_DO_DRAW	= 0x10	/**< ��Ʈ�� �����츦 �׸��� ���� ��ɾ� */
	};



	enum 
	{
		// Text Properties
		CTL_SLIDETEXT_CMD_SET_X = 0x0100,	/**< �ؽ�Ʈ�� X ��ǥ�� �����ϱ� ���� ��ɾ� */
		CTL_SLIDETEXT_CMD_SET_Y,			/**< �ؽ�Ʈ�� Y ��ǥ�� �����ϱ� ���� ��ɾ� */
		CTL_SLIDETEXT_CMD_SET_FGCOLOR,		/**< �ؽ�Ʈ�� ������ �����ϱ� ���� ��ɾ� */


		// Bg Properties
		CTL_SLIDETEXT_CMD_SET_TOP,			/**< ��Ʈ�� �������� X ������ �����ϱ� ���� ��ɾ� */
		CTL_SLIDETEXT_CMD_SET_LEFT,			/**< ��Ʈ�� �������� Y ������ �����ϱ� ���� ��ɾ� */
		CTL_SLIDETEXT_CMD_SET_RIGHT,		/**< ��Ʈ�� �������� Width ������ �����ϱ� ���� ��ɾ� */	
		CTL_SLIDETEXT_CMD_SET_BOTTOM,		/**< ��Ʈ�� �������� Height ������ �����ϱ� ���� ��ɾ� */
		CTL_SLIDETEXT_CMD_SET_BGCOLOR,		/**< */

		// slide Properties
		CTL_SLIDETEXT_CMD_DO_LFRONT,		/**< �����̵� ������ �������� �����̸� �����̵尡 ������ ó�� ��ġ���� �����ϱ� ���� ��ɾ�*/
		CTL_SLIDETEXT_CMD_DO_LROUND,		/**< �����̵� ������ �������� �����̸� ��� ��ȯ �ϱ� ���� ��ɾ� */		

		
		CTL_SLIDETEXT_CMD_SET_TIMEOUT,		/**< �����̵��� ���͹� �ð��� �����ϱ� ���� ��ɾ� */

		CTL_SLIDETEXT_CMD_DO_PLAY,			/**< �����̵带 �÷��� �ϱ� ���� ��ɾ� */
		CTL_SLIDETEXT_CMD_DO_STOP,			/**< �����̵带 ���� �ϱ� ���� ��ɾ� */
		CTL_SLIDETEXT_CMD_DO_PAUSE,			/**< �����̵带 �Ͻ����� �ϱ� ���� ��ɾ� */

		CTL_SLIDETEXT_CMD_GET_MODE,			/**< �����̵��� ���� MODE ���¸� ��� ���� ���� ��ɾ� */

		CTL_SLIDETEXT_CMD_DO_START_FRONT,	/**< �����̵��� ������ġ�� �ؽ�Ʈ�� X ��ǥ ���� ���� �Ѵ� */
		CTL_SLIDETEXT_CMD_DO_START_END,		/**< �����̵��� ������ġ�� Ŀ��Ʈ�� �������� Width ���� ������ ���� �Ѵ� */
	
		CTL_SLIDETEXT_CMD_SET_MAXWIDTH,		/**< �����̵��� ���ڿ��� ũ�⸦ �����ϱ� ���� ��ɾ� */
		CTL_SLIDETEXT_CMD_SET_DATA,			/**< �����̵��� ���ڿ��� �����ϱ� ���� ��ɾ� */
		//
		CTL_SLIDETEXT_CMD_SET_WNDRECT,		/**< ��Ʈ�� �������� ��ǥ�� ũ�� ������ �ѹ��� �����ϱ� ���� ��ɾ� */

		// callback function property
		CTL_SLIDETEXT_CMD_SET_CB_DRAW,		/**< ����� ȭ�� �׸��� �ݹ� �Լ� ����� ���� ��ɾ� */
		CTL_SLIDETEXT_CMD_SET_DB_KEYDOWN,	/**< ����� Ű ó�� �ݹ� �Լ� ����� ���� ��ɾ� */

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
typedef int (*CtlSlideTextDrawCB)(WND hWnd, COLORVAL nFgColor, COLORVAL nBgColor); /**< ����� �׸��� �ݹ� �Լ� ������ */
typedef int (*CtlSlideTextKeyDownCB)(WND hWnd, int wparam); /**< ����� Ű ó�� �ݹ� �Լ� ������ */

/**
* @}
* */


/*
 * �����̵� �ϰ��� �ϴ� �ؽ�Ʈ�� ��� ��ġ�ϸ� (x,y)
 * ���� ���̸� ������ ������, �ؽ�Ʈ ������ �����̸�, 
 * �ؽ�Ʈ�� ��ġ�� ���� ����, �ؽ�Ʈ�� Unicode ���� �ƴ�����
 * �Ӽ��� ������ �ִ�.
*/
	typedef struct _REXY_CTL_SLIDETEXT_INFO
	{
		rint16 nX;				/** X position     */
		rint16 nY;				/** Y position     */	
		COLORVAL nFgColor;		/** String Color   */
	} REXY_CTL_SLIDETEXT_INFO;

/* 
 *�����̵� �ϰ��� �ϴ� ��� ������ �����ϴ� �Ӽ��̸�,
 * ��� ������ �⺻ �Ӽ� ( top, left, right, bottom )�� ��� ���� 
 * ������ ������.
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
                              
