/*   
 * $Id: rexy_ctl_CtlEditor.h 491 2006-01-11 16:32:34Z madholic $
 * Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.    
 *   
 * This file is a part of Rexy Embedded.    
 */

/*************************************************************************
 * REXY/E Text Edit Control
 *
 *  ���: 
 *		+ �ؽ�Ʈ ����
 *		+ Single Line/Multi Line ����
 *		+ ��ũ�� ��� ����
 *		+ Backspace, Delete Ű ó��
 *	
 *  ������ �߰��� ���
 *		+ �پ��� ������ �Է±���� ���� ���
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
 * Edit Control�� �����ϰ� �����ϱ� ���� ��ɾ� ����
 */
enum EditControlAttr{
	//Start offset 0x0100 for no reason.
	// �ʵ�
	CTL_EDIT_CMD_GET_WND_RECT = 0x0100,	///< WIN RECT ���� ����.
	CTL_EDIT_CMD_SET_WND_RECT,			///< WIN RECT ���� ����.

    CTL_EDIT_CMD_GET_STRING,			///< ���ڿ�
    CTL_EDIT_CMD_SET_STRING,			///< ���ڿ�

    CTL_EDIT_CMD_GET_MAX_LEN,			///< �ִ� ����
    CTL_EDIT_CMD_SET_MAX_LEN,			///< �ִ� ����

	CTL_EDIT_CMD_GET_PASSWORD,			///< Password �Է¸�� ����
	CTL_EDIT_CMD_SET_PASSWORD,			///< Password �Է¸�� ����

	CTL_EDIT_CMD_GET_READONLY,			///< �б� ���� ����
	CTL_EDIT_CMD_SET_READONLY,			///< �б� ���� ����

    CTL_EDIT_CMD_GET_CURSOR_INDEX,		///< ���ڿ����� Ŀ���� ��ġ
    CTL_EDIT_CMD_SET_CURSOR_INDEX,		///< ���ڿ����� Ŀ���� ��ġ

	CTL_EDIT_CMD_GET_MULTILINE,			///< �̱�/��Ƽ ���� ����
	CTL_EDIT_CMD_SET_MULTILINE,			///< �̱�/��Ƽ ���� ����

	// ���
    CTL_EDIT_CMD_DO_DRAW,				///< ȭ�� �׸���

	// �ݹ� �Լ�
	CTL_EDIT_CMD_GET_CB_DRAW,			///< ȭ�� �׸��� �ݹ� �Լ�
	CTL_EDIT_CMD_SET_CB_DRAW,			///< ȭ�� �׸��� �ݹ� �Լ�

	CTL_EDIT_CMD_GET_CB_KEYDOWN,		///< Ű �Է� ó�� �ݹ� �Լ�
	CTL_EDIT_CMD_SET_CB_KEYDOWN,		///< Ű �Է� ó�� �ݹ� �Լ�

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

