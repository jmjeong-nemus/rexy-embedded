/*   
 * * $Id: rexy_sysmsg.h 491 2006-01-11 16:32:34Z madholic $
 * Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.    
 *   
 * This file is a part of Rexy Embedded.    
 *   
 */

#ifndef __REXY_SYSMSG_H__
#define __REXY_SYSMSG_H__

/** @file */

int		MsgDefWindowProc (WND wnd, int mesg, int wparam, int lparam);
int		MsgDefMsgKeyDown(WND hWnd, int Msg, int wParam, int lParam);
int		MsgDefMsgDispUpdate(WND hWnd, int Msg, int wParam, int lParam);
int		MsgSystemTimer(pmsg msg);
int		MsgSystemKeyRepeat(pmsg msg);
int		MsgSystemKeyUp(pmsg msg);
int		MsgSystemKeyDown(pmsg msg);
int		MsgSystemKeyLong(pmsg msg);
int		MsgSystemUpdateTime(pmsg _msg);
int		MsgSystemEnableTimeDisp(pmsg _msg);
int		MsgSystemCheckAlarm(pmsg msg);
int		MsgSystemChangeIdleTimer(pmsg msg);
int		MsgSystemFolderChange(pmsg msg);

#endif
