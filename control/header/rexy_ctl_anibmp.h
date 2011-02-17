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
 * Animate bitmap control�� ��� ����� ����.
 */
enum PlayMode
{
	CTL_ANIBMP_PLAY_NORMAL,				///< ������ 1ȸ ���
	CTL_ANIBMP_PLAY_ROTATE,				///< ������ ��ȯ
	CTL_ANIBMP_PLAY_REVERSE,			///< ������ 1ȸ ���
	CTL_ANIBMP_PLAY_REVERSE_ROTATE,		///< ������ ��ȯ
	CTL_ANIBMP_PLAY_RANDOM				///< ������ ���
};

/**
 * @enum Command
 * Animate Bitmap Control�� �����ϰ� �����ϱ� ���� ��ɾ� ����
 */
enum Command
{
	CTL_ANIBMP_CMD_SET_LEFT,			///< ��Ʈ�� �����쿡�� �̹����� ��µ� X��ǥ�� ����
	CTL_ANIBMP_CMD_SET_TOP,				///< ��Ʈ�� �����쿡�� �̹����� ��µ� Y��ǥ�� ����
	CTL_ANIBMP_CMD_SET_WIDTH,			///< �̹����� ��µ� ������ �� ����
	CTL_ANIBMP_CMD_SET_HEIGHT,			///< �̹����� ��µ� ������ ���� ����
	CTL_ANIBMP_CMD_SET_BGCOLOR,			///< ��Ʈ�� �������� ���� ����

	CTL_ANIBMP_CMD_SET_FRAME_NO,		///< ��ü �������� �� ����
	CTL_ANIBMP_CMD_SET_P_IDS,			///< �̹��� �迭 ������
	CTL_ANIBMP_CMD_SET_PLAYTYPE,		///< ������ ����
	CTL_ANIBMP_CMD_SET_TIMEOUT,			///< ������ �̹����� ȭ�鿡 �������� �ð� ����
	CTL_ANIBMP_CMD_SET_P_TIMEOUT,		///< �� �̹����� ȭ�鿡 �������� �ð� ����

	CTL_ANIBMP_CMD_SET_WNDRECT,			///< ��Ʈ�� �������� ��ǥ �� ���� ����
	CTL_ANIBMP_CMD_SET_CB_KEYDOWN,		///< Key Down Callback �Լ� ���
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

// Animation Bitmap Control Main ����ü
typedef struct
{
	ruint16	nIdx;
	bool	bIsTimer;
	CtlAniBmpKeyDownCB pfnKeyDown;

	rint16 nLeft;				///< �����쿡�� �̹����� ��µ� ������ x��ǥ
	rint16 nTop;				///< �����쿡�� �̹����� ��µ� ������ y��ǥ
	ruint16 nWidth;				///< �����쿡�� �̹����� ��µ� ��
	ruint16 nHeight;			///< �����쿡�� �̹����� ��µ� ����
	COLORVAL bgColor;			///< �������� ������

	ruint16	nFrameNo;			///< ����� ��ü �̹��� ��
	int*	pIDs;				///< �̹����� ID�� ������ �ִ� �迭�� ����Ű�� ������
	int		nPlayType;			///< ��� ���
	int		nTimeout;			///< ������ �̹����� ȭ�鿡 �������� �ð�(milli second)
	int*	pTimeout;			///< �� �̹����� �������� �ð����� ������ �迭�� ����Ű�� ������
} REXY_CTL_ANIBMP;

// Procedure
int CtlAniBmpProc(WND wnd, int nMsg, int wparam, int lparam);
int CtlAniBmpCBProc(WND wnd, int nMsg, int wparam, int lparam, int bIsPre, int *pErr);
int CtlAniBmpCmdProc(WND wnd, int nCommand,int wParam, int lParam, int *pErr);

#ifdef __cplusplus
}
#endif	// __cplusplus
#endif	// __REXY_CTL_ANIBMP_H__
                              
