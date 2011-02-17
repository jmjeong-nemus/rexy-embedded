/*
* $Id: rexy_msg.c 435 2005-02-16 08:25:12Z longdal $
* Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
*
* This file is a part of Rexy Embedded 
*
*/

/** @file */

/**
* @defgroup msgmgr Core Messaging and Event Managing Service
* Message & Event API
* @ingroup RexyECoreGroup
* @{
*/

#include "rexy.h"

#define DPRINT(x)

/*======================================================================*/
/* Private								*/
/*======================================================================*/
static int	PrvCheckQueueMsg(WND panel, pmsg msg, queue_obj * msg_queue);
static int	PrvGetMessage(WND panel, pmsg msg, int func_ptr);

/*======================================================================*/
/* Module Variables							*/
/*======================================================================*/
static int newEndMsg = 0;
static int newEndMsgFlag = 0;
unsigned int g_prev_klight_time = 0;
static msg	g_child_msg;

extern WND MainWndHandle;
//To handle system Timer Message
extern ruint32	g_CurTimerGCD;
/*======================================================================*/
/* Implementation							*/
/*======================================================================*/

/**
* @brief		Retrieve message from message queue.
* @param[in]	panel	Panel window handle.
* @param[in]	msg		Message structure pointer.
* @retval		1		If message is peeked successfully
* @retval		0		If message isn't peeked successfully
* @remarks		If no message is in the queue, return immediately.
*			
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
int MsgPeekMessage(WND panel, pmsg msg)
{
    int ret;
    ret = -1;

    if ( PrvCheckQueueMsg(panel, msg, &g_process_q.norm) > 0 )
    {
        ret = 1;
    }
    else if ( PrvCheckQueueMsg(panel, msg, &g_process_q.timer) > 0 )
    {
		//TrcPrint("Event in Timer queue found\n");
        ret = 1;
    }
    else if ( PrvCheckQueueMsg(panel, msg, &g_process_q.paint) > 0 )
    {
        ret =  1;
    }

    return ret;
}

/**
* @brief		Get message from message queue.
* @param[in]	panel	Panel window handle.
* @param[in]	msg		Message structure pointer.
* @retval		0		If retrieved message is WM_QUIT
* @retval		1		If retrieved message is in case of all message except for WM_QUIT
* @remarks		If there is no message in queue, operation is blocked.
*	
* @if HISTORY
*				06-Jan-2006 madholic	Initiated 
*				09-Jan-2006	joshua		Added system timer message handling
* @endif
*/
int MsgGetMessage(WND panel, pmsg msg)
{
    int ret;
    MemSet( msg, 0, sizeof(msg) );

    ret = PrvGetMessage(panel, msg, (int)StaGetCurrentProc());

    return ret;
}

/**
* @brief		
* @param[in]	panel .	
* @param[in]	msg .
* @param[in]	func_ptr .
* @return		
*		
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
int MsgGetMessageCtrl(WND panel, pmsg msg, int func_ptr)
{
    return PrvGetMessage(panel, msg, func_ptr);
}

/**
* @brief		Dispatch the message to the system panel.
* @param[in]	msg		Message structure pointer.
* @retval		1		If message is dispatched successfully
* @retval		0		If message dispatching is failed
*			
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
int MsgDispatchSystemMessage(pmsg msg)
{
    if ( msg->window == (int)SYSTEM_PANEL )
    {
        // Dispatch some system function
        TRACE(TR_SystemEvent, TR_LEVEL1, ("AS_DispatchSystemMessage: SYSTEM_PANEL\n"));
        return 1;
    }
    else
    {
        TRACE(TR_SystemEvent, TR_LEVEL1, ("AS_DispatchSystemMessage: Normal\n"));
        switch ( msg->wmsg )
        {
        case AWM_TIMER:
            return MsgSystemTimer(msg);

        case AWM_KEYLONG:
            return MsgSystemKeyLong(msg);

        case AWM_KEYDOWN:
            return MsgSystemKeyDown(msg);

        case AWM_KEYREPEAT:
            return MsgSystemKeyRepeat(msg);

        case AWM_KEYUP:
            return MsgSystemKeyUp(msg);


        default:
            break;
        }
    }

    return 0;

}

/**
* @brief		Send the AWM_KEYDOWN message to the window. 
* @param[in]	wparam		Message dependent data.
* @param[in]	lparam		Message dependent data.
* @return		Depends on the message and window procedure.
* @remarks		It invokes the window procedure directly.
*			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
int MsgKeyPadDispatch(int wparam, int lparam)
{
    return MsgSendMessage((WND)MainWndHandle, AWM_KEYDOWN, wparam, lparam);
}

/**
* @brief		Send the message to appropriate window procedure.
* @param[in]	msg		Message structure pointer.
* @return		Depends on the message and window procedure.
*			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
int MsgDispatchMessage(pmsg msg)
{
    TRACE(TR_SystemEvent, TR_LEVEL1, ("AS_DispatchMessage Sending %x msg\n", msg->wmsg));
    return MsgSendMessage((WND)msg->window, msg->wmsg, msg->wparam, msg->lparam);
}

extern void WinSetCurrentWindow(WND panel); // refer to rexy_win.c

/**
* @brief		Send the message to the window.
* @param[in]	panel	Window handle.
* @param[in]	Msg		Message type.
* @param[in]	wParam	Message dependent data.
* @param[in]	lParam	Message dependent data.
* @return		Depends on the message and window procedure.
* @remarks		Gets blocked until sent message is processed. Process message by calling window procedure directly.
*			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
*				09-Jan-2008 joshua	 update warning message
* @endif
*/
int MsgSendMessage(WND panel, int Msg, int wParam, int lParam)
{
	ASSERT( panel );

    if ( panel )
	{
		if ( panel->WndProc )
		{
			TRACE(TR_SystemEvent, TR_LEVEL1, ("MsgSendMessage Sending %x msg to %lx\n", Msg, panel));
			return panel->WndProc(panel, Msg, wParam, lParam);
		}
		else
		{
			TrcPrintF("[Warning] Message %x has been vaporized since window panel %d has no event handler\n", Msg, panel);
		}
	}
	else
	{
		// Should not reach here
		TrcPrint( "[Error] Message %d sent to invalid window\n" );
	}
    return -1;
}

/**
* @brief		Send message to the message queue. 
* @param[in]	panel	Window handle.
* @param[in]	Msg		Message type.
* @param[in]	wParam	Message dependent data.
* @param[in]	lParam	Message dependent data.
* @return		void
* @remarks		Puts message in the message queue and return immediately.
*			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
void MsgPostMessage(WND panel, int Msg, int wParam, int lParam)
{
    msg_obj * new_msg;

    new_msg = MgqMalloc(sizeof(msg_obj));
    if ( new_msg == NULL )
    {
        return;
    }

    new_msg->window = (int)panel;
    new_msg->wmsg = Msg;
    new_msg->wparam = wParam;
    new_msg->lparam = lParam;

    switch ( new_msg->wmsg )
    {
    case AWM_PAINT :
    case AWM_ERASEBKGND:
        LckLock(LOCK_ID_MSGQ);
        MgqPushMsgDirect(&g_process_q.paint, new_msg);
        LckUnlock(LOCK_ID_MSGQ);
        break;

    case AWM_UPDATECONTROL:
        LckLock(LOCK_ID_MSGQ);
        MgqPushMsgDirect(&g_process_q.paint, new_msg);
        LckUnlock(LOCK_ID_MSGQ);
        break;

    case AWM_TIMER :
        LckLock(LOCK_ID_MSGQ);
        MgqPushMsgDirect(&g_process_q.timer, new_msg);
        LckUnlock(LOCK_ID_MSGQ);
        break;

    default :
        LckLock(LOCK_ID_MSGQ);
        MgqPushMsgDirect(&g_process_q.norm, new_msg);
        LckUnlock(LOCK_ID_MSGQ);
        break;
    }

    TRACE(TR_SystemEvent, TR_LEVEL1, ("AS_PostMessage Posting %x msg\n", new_msg->wmsg));
    return;
}

#ifndef USE_JOB_LIST
void MsgAddMessageToHead(WND panel, int Msg, int wParam, int lParam)
{
    msg_obj * new_msg;

    new_msg = MgqMalloc(sizeof(msg_obj));
    if ( new_msg == NULL )
    {
        return;
    }

    new_msg->window = (int)panel;
    new_msg->wmsg = Msg;
    new_msg->wparam = wParam;
    new_msg->lparam = lParam;

    LckLock(LOCK_ID_MSGQ);
    MgqPushMsgAhead(&g_process_q.norm, new_msg);
    LckUnlock(LOCK_ID_MSGQ);

    TRACE(TR_SystemEvent, TR_LEVEL1, ("AS_AddMessageToHead Posting %x msg\n", new_msg->wmsg));
    return;
}
#endif

bool MsgIsSystemMessage(int msg, int wParam, int lParam)
{
    return FALSE;
}

/**
* @brief		
* @param[in]	panel	Window handle.
* @param[in]	Msg		Message type.
* @param[in]	wParam	Message dependent data.
* @param[in]	lParam	Message dependent data.
* @return		void
* 			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
void MsgRestoreMessage(WND panel, int Msg, int wParam, int lParam)
{
    msg_obj * new_msg;

    new_msg = MgqMalloc(sizeof(msg_obj));
    if ( new_msg == NULL )
    {
        return;
    }

    new_msg->window = (int)panel;
    new_msg->wmsg = Msg;
    new_msg->wparam = wParam;
    new_msg->lparam = lParam;

    LckLock(LOCK_ID_MSGQ);
    MgqPushMsgAhead(&g_process_q.norm, new_msg);
    LckUnlock(LOCK_ID_MSGQ);
    return;
}

/**
* @brief		Remove all messages in message queue of specified panel.
* @param[in]	panel		Window handle to remove the message.
* @return		void
* 			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
void MsgRemoveMessage(WND panel)
{
    LckLock(LOCK_ID_MSGQ);
    MgqRmQMsg(&g_process_q.norm, (int)panel);
    MgqRmQMsg(&g_process_q.paint, (int)panel);
    MgqRmQMsg(&g_process_q.timer, (int)panel);
    LckUnlock(LOCK_ID_MSGQ);
}

/**
* @brief		Send WM_QUIT message. Causes the termination of AS_WinMain loop.
* @param[in]	exit_num		Program exit Code.
* @return		void
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
void MsgPostQuitMessage(int exit_num)
{
	// edit by venabili
    //MsgPostMessage(NULL, AWM_QUIT, exit_num, 0);
    MsgPostMessage(WinGetSelection(), AWM_QUIT, exit_num, 0);
}

/**
* @brief		Default message procedure.
* @param[in]	wnd		Window handle.
* @param[in]	mesg	Message type.
* @param[in]	wparam	Message dependent data.
* @param[in]	lparam	Message dependent data.
* @return		Depends on the message type.
* @remarks		It is identical to the default window procedure of Win32.
*			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
int MsgDefWindowProc (WND wnd, int mesg, int wparam, int lparam)
{
    if ( wnd == NULL )
        return -1;

    switch ( mesg )
    {
    case AWM_UPDATECONTROL :
        WinUpdateAllControl(wnd, ERASE_BKGND | NO_REDRAW);
        break;

    case AWM_KEYDOWN:
        MsgDefMsgKeyDown(wnd, mesg, wparam, lparam);
        break;

    case AWM_DISPUPDATE:
        MsgDefMsgDispUpdate(wnd, mesg, wparam, lparam);
        break;

    default :
        break;
    }

    return 0;
}

int MsgDefMsgKeyDown(WND hWnd, int Msg, int wParam, int lParam)
{
    //	StateParam info;

    //	if (wParam == VK_USER_END)
    //	{
    //		AS_MemSet(&info, 0, sizeof(info));
    //		AS_BackStateEx(hWnd, info, IDA_SYSTEM, EXIT_BY_END);
    //	}

    return 0;
}

int MsgDefMsgDispUpdate(WND hWnd, int Msg, int wParam, int lParam)
{
    return 0;
}

/**
* @brief		Change End Key Event
* @param[in]	hWnd		Window handle.
* @param[in]	newmsg	new Endkey Message.
* @return		void
*		
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
void MsgChangeEndKey(WND hWnd, int newmsg)
{
    newEndMsg = newmsg;
    newEndMsgFlag = TRUE;
}


int MsggetNewMsg(void)
{
    return newEndMsg;
}

int MsggetNewEndMsgFlag(void)
{
    return newEndMsgFlag;
}

void MsgResetNewEndMsgFlag(void)
{

}
// Updated by Joshua on 2008-01-21 Mon??PM  5:57
static int PrvGetMessage(WND panel, pmsg mesg, int func_ptr)
{
     int ret = 0;
     msg ralmsg;

     ret = MsgPeekMessage(panel, mesg);

     if ( ret > 0 )
     {
	  if ( mesg->wmsg == AWM_QUIT ) 
	  {
	       ret = 0 ; // Termination
	  }
	  else 
	  {
	      ralmsg.window = (int)panel;
	      RalEventGet(&ralmsg,1);

	      if ( ralmsg.wmsg != AWM_NONE )
	      {
		   MsgPostMessage(panel,ralmsg.wmsg,ralmsg.wparam,ralmsg.lparam);
	      }
	  }
     }
     else
     {
	  do
	  {
	       mesg->window = (int)panel;
	       ret = RalEventGet(mesg, 0);
	  } while ( mesg->wmsg == AWM_NONE );
     }


    if (mesg->wmsg == AWM_TIMER && mesg->wparam == HAL_TIMER_ID )
    {
	 UsrHandleKernelTimerMessage();
	 MsgPeekMessage( panel, mesg );
    }
    return ret;
}

static int PrvCheckQueueMsg(WND panel, pmsg msg, queue_obj * msg_queue)
{
    msg_obj * prev_msg;
    msg_obj * queue_msg;

    prev_msg = NULL;
    queue_msg = msg_queue->m_head;


    if ( queue_msg != NULL )
    {
        msg->window = queue_msg->window;
        msg->lparam = queue_msg->lparam;
        msg->wparam = queue_msg->wparam;
        msg->wmsg = queue_msg->wmsg;
        MgqDelMsg(msg_queue, queue_msg, prev_msg);
        return 1;
    }
	return 0;


	/*
    if ( panel == NULL )
    {
        if ( queue_msg != NULL )
        {
            if(queue_msg->window == (int)NULL)
                msg->window = (int)WinGetSelection();
            else
                msg->window = queue_msg->window;
            msg->lparam = queue_msg->lparam;
            msg->wparam = queue_msg->wparam;
            msg->wmsg = queue_msg->wmsg;
            MgqDelMsg(msg_queue, queue_msg, prev_msg);
            return 1;
        }
    }
    else
    {
        while ( queue_msg != NULL )
        {
            if ( queue_msg->window == (int)panel )
            {
                if(queue_msg->window == (int)NULL)
                    msg->window = (int)WinGetSelection();
                else
                    msg->window = queue_msg->window;
                msg->lparam = queue_msg->lparam;
                msg->wparam = queue_msg->wparam;
                msg->wmsg = queue_msg->wmsg;
                MgqDelMsg(msg_queue, queue_msg, prev_msg);
                return 1;
            }
			// add by venabili for timer event between parent and child
			else if (queue_msg->wmsg == AWM_TIMER)
			{
				msg->window = queue_msg->window;
				msg->lparam = queue_msg->lparam;
				msg->wparam = queue_msg->wparam;
				msg->wmsg = queue_msg->wmsg;
                MgqDelMsg(msg_queue, queue_msg, prev_msg);
                return 1;
			}
            else
            {
                prev_msg = queue_msg;
                queue_msg = queue_msg->m_next;
            }
        }
    }

    return 0; */
}


/**
* @brief		Routing message
* @param[in]	wnd		Top level window handle to routing
* @param[in]	msg		event message to routing
* @return		Depends on the message treat or not.
*		
* @if HISTORY
*				19-Jul-2006 venabili Initiated
*				26-Jul-2006	Joshua	 Revised to route message in two way.
*									 ( For the entry point of matching state, and its subsidiary windows ) 
*									 API name renamed 
*               26-Oct-2006 longdal change routing message order that from parent first change to child first chance
*				22-Jan-2008 Joshua	Revised to route message from parent to child using AWM_CHILDMSG, no direct dispatch, 
*									no exception, no sibling window dispatch.
* @endif
*/
int MsgRouteMessage( WND wnd, pmsg msg )
{
	int wmsg;
	int ret;
	WND w;

	w = wnd;
	ret = 0;
    
	while ( w )
	{
		if ( w == msg->window )
		{
			wmsg = msg->wmsg;
		}
		else
		{
			wmsg = AWM_CHILDMSG;
			MsgSetChildMsg( msg );
		}

		ret = MsgSendMessage(w, wmsg, msg->wparam, msg->lparam);

		if ( ret )
		{
			break;
		}
		
		w = w->childwin;
	}
	return ret;
}
pmsg MsgGetChildMsg( void )
{
	return &g_child_msg;
}
// System use only
bool MsgSetChildMsg( pmsg child_msg )
{
	MemCopy( &g_child_msg, child_msg, sizeof( pmsg ));		
}
/** @} */ // End of Message
