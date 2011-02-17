/*
* $Id: rexy_state.c 594 2005-02-24 13:13:55Z klenui $
* Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
*
* This file is a part of Rexy Embedded 
*
*	pict routine.
*
*/

/** @file */

/**
 * @defgroup logic_rexy_state Logic State
 * These APIs are associated with Pict of Logic
 * @ingroup LogicGroup
 * @{
 */

#include <stdarg.h>

#include "rexy.h"

static WINPROC StateEventHandler[MAX_STATE_NUM];
static StateIDType gStateID = -1;

/** 
 * @brief	Change state and run an application
 * @ifdef HISTORY
 *			19-Jan-2006 Joshua revised to support message delivery
 *			07-Jun-2006	Joshua updated to support wparam, lparam
 *			26-Jul-2006	Joshua updated to handle null entry proc situation
 * @endif
 */
bool StaGotoState( WND hwnd, StateIDType newState, MsgType MsgForPrevApp, MsgType MsgForNewApp )
{
	WINPROC StateEntryProc;

	if ( newState < 0 || newState >= MAX_STATE_NUM ) 
	{
		return FALSE;
	}
	else 
	{
		StateEntryProc = StaGetCurrentProc();

		if ( StateEntryProc ) 
		{
			( StateEntryProc )( hwnd, MsgForPrevApp.wmsg, MsgForPrevApp.wparam, MsgForPrevApp.lparam );
		}

		MemDumpLeakInfo();

		gStateID = newState;
		StateEntryProc = StaGetCurrentProc();
		if ( StateEntryProc ) 
		{
			( StateEntryProc )( hwnd, MsgForNewApp.wmsg, MsgForNewApp.wparam, MsgForNewApp.lparam );
		}
		else TrcPrint("[Error] Failed to find new state entry procedure\n");
	}
	return TRUE;
}

/** 
 * @anchor StaSetEventHandler
 * @brief	Set event handler of a state
 * @param	stateId
 * @param	EventHandler
 * @retval	RxErrNone for success
 * @retval	RxErrStaClass | enInvalidParam	on failure
 * @retval	RxErrStaClass | enAlreadyExist	if there's a winproc already matching stateId
 * @remark	'enAlreadyExist' is for debugging purpose. Note that It doesn't mean failure.
 * @ifdef HISTORY
 *		15-May-2006	Joshua Created
 * @endif
 */
RxErr StaSetEventHandler(StateIDType stateId,WINPROC EventHandler)
{
	RxErr result = RxErrNone;

	if ( stateId >= MAX_STATE_NUM || EventHandler == NULL )
	{
		result = ( RxErrStaClass | enInvalidParam );
	}
	else
	{
		if ( StateEventHandler[stateId] ) 
			result = ( RxErrStaClass | enAlreadyExist );

		StateEventHandler[stateId] = EventHandler;

	}
	return result;
}
/** StaGetCurrentProc
*
* @return Return function pointer
* @ifdef HISTORY
*		26-Jul-2006 Joshua	support -1 value for gStateID 
* @endif
*/
WINPROC StaGetCurrentProc(void)
{
	if ( gStateID == -1 ) return NULL;
	return StateEventHandler[gStateID];
}


/**
 * @anchor	StaGetCurrentStateID
 * @ifdef	HISTORY
 *		24-Aug-2006	Joshua created
 * @endif
 */
StateIDType StaGetCurrentStateID(void)
{
	return gStateID;
}



/** @} */ // End of Logic State
