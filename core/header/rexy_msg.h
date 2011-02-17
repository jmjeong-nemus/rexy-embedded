/*
 * $Id: rexy_msg.h 5441 2005-07-21 04:31:49Z klenui $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 * Device dependent한 함수 및 상수가 상수가 정의되어 있다.
 *
 */

#ifndef __REXY_MSG_H__
#define __REXY_MSG_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	int	wmsg;
	int	wparam;
	int lparam;
} MsgType;

    /** @file */

    int		MsgPeekMessage(WND panel, pmsg msg);
    int		MsgGetMessage(WND panel, pmsg msg);
    int		MsgGetMessageCtrl(WND panel, pmsg msg, int func_ptr);
    int		MsgDispatchSystemMessage(pmsg msg);
    int		MsgKeyPadDispatch(int wparam, int lparam);
    int		MsgDispatchMessage(pmsg msg);
    int		MsgSendMessage(WND panel, int Msg, int wParam, int lParam);
    void	MsgPostMessage(WND panel, int Msg, int wParam, int lParam);
    void	MsgAddMessageToHead(WND panel, int Msg, int wParam, int lParam);
    bool	MsgIsSystemMessage(int msg, int wParam, int lParam);
    void	MsgRestoreMessage(WND panel, int Msg, int wParam, int lParam);
    void	MsgRemoveMessage(WND panel);
    void	MsgPostQuitMessage(int exit_num);
    int		MsgDefWindowProc (WND wnd, int mesg, int wparam, int lparam);
    void	MsgChangeEndKey(WND hWnd, int newmsg);
    int		MsgGetNewMsg(void);
    int		MsgGetNewEndMsgFlag(void);
    void	MsgResetNewEndMsgFlag(void);
    int		MsgRouteMessage( WND wnd, pmsg msg );
	pmsg	MsgGetChildMsg( void );
	bool	MsgSetChildMsg( pmsg child_msg );
	
#ifdef __cplusplus
}
#endif	// __cplusplus

#endif	// __REXY_MSG_H__
