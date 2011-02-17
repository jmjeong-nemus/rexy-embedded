/*   
 * * $Id: rexy_ctl_popup.c 491 2006-01-11 16:32:34Z madholic $
 * Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.    
 *   
 * This file is a part of Rexy Embedded.    
 *   
 */

#ifndef __REXY_STATE_H__
#define __REXY_STATE_H__


/** @file */

#define MAX_STATE_NUM 100	// Max num of applications

typedef int StateIDType;

bool	StaGotoState(WND hwnd, StateIDType newState, MsgType MsgForPrevApp, MsgType MsgForNewApp );
RxErr	StaSetEventHandler(StateIDType stateId,WINPROC EventHandler);
WINPROC StaGetCurrentProc(void);
StateIDType StaGetCurrentStateID(void);

#endif

//__REXY_STATE_H__

