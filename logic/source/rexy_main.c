/*
 * $Id: rexy_main.c 594 2005-02-24 13:13:55Z quann $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 *	main routine
 *
 */

/** @file */

/**
 * @defgroup logic_rexy_main Logic Main
 * These APIs are associated with Main of the Logic
 * @ingroup LogicGroup
 * @{
 */

#include "rexy.h"
#ifndef WIN32
#include "../../hal/linux_embedded/fbScreen.h"
#endif

WND MainWndHandle;
void Rexy_WinMain(void);

// core library build time log
const char g_rexy_core_build_date[] = __DATE__;
const char g_rexy_core_build_time[] = __TIME__;

static void InitStructures(void);
static void CloseStructures(void);

static int InitDevices(void);

static void ReleaseStructures(void);


extern void AppInit(void);		// application initialization


/** Start Rexy.
*
* @return Return 0
*/
int StartRexy(void)
{
    // just initialization of g_scr_dev. no allocation.
    if ( InitDevices() < 0 )
        return 0;

    InitStructures();
    Rexy_WinMain();

    return 0;
}

static void InitStructures(void)
{
    extern tm_msg g_tm_schedule[MAX_TIMER_NUM];
    // static linked list ?	
    InitWNDFreeStructures();
    MemSet(&g_tm_schedule, 0, sizeof(g_tm_schedule));
    LckInit();
    return;
}

static void CloseStructures(void)
{
}


static void ReleaseStructures(void)
{
    DelWNDFreeStructures();
    MgqDelMsgFreeStructures();

    return;
}

static int InitDevices(void)
{
#ifndef WIN32
	fbInit();
#endif
    return 0;
}

static void CloseDevices(void)
{
#ifndef WIN32
	fbClose();
#endif
    return;
}

/** End Rexy.
*
*/
void EndRexy(void)
{
    CloseStructures();
    // in fact, this function does nothing
    ReleaseStructures();
    CloseDevices();
    return;
}

//static void DisplayShutdownAnimation(void)
//{
//    return;
//}

static WND PrvFindTopWindow(WND panel)
{
	WND w;
	w = panel;

	while ( w )
	{
	     if ( w->parentwin ) 
		  w = w->parentwin;
	     else
		  break;
	}
	return w;
}

/** 
 * @brief	Rexy's Main Event Handler
 * @if HISTORY
 *		26-Jul-2006	Joshua Revised to support capsulated event handling
 * @endif
 */
void Rexy_WinMain(void)
{
    msg		nMsg;
//    bool	bCheckStartTime = TRUE;
	MsgType	NullMessage = { 0, 0, 0};
	MsgType CreateMessage = { AWM_CREATE, 0, 0 };
	WND		hFocusedWindow;
	int		wmsg;
	
	// Initialize message queue
	MgqInitFreeStructures();	
	
	LckInit();
	
	// Initialize application
	AppInit();

	DiagInit();

	// Set default font
	FntSetFont(&FntGlobalContext, 0);

	// Execute application of which state id is 0
	StaGotoState( NULL, 0, NullMessage , CreateMessage );	
	
	while ( MsgGetMessage( hFocusedWindow = WinGetSelection(), &nMsg ) )
    {
		if ( DiagHandler() != DIAG_RET_OK )
		{
			TrcPrint("[FATAL ERROR] DiagHandler\n");
			break;
		}

        TRACE( TR_RexyCore, TR_LEVEL1, ("MsgDispatch in GetMessage Loop with Msg %x\n", nMsg.wmsg) );

		// Dispatch message to current state procedure first
		// in case of that we could have AWM_DESTROY or something
		// big event which can affect all open window
		if ( nMsg.window ) 
		{
			wmsg = AWM_CHILDMSG;
			MsgSetChildMsg( &nMsg );
		}
		else
		{
			wmsg = nMsg.wmsg;
		}
		if ( !StaGetCurrentProc()( (WND)nMsg.window, wmsg, nMsg.wparam, nMsg.lparam ) )
		{ 
			// Since current state proc didn't handle the message,
			// this message will be floating among its window & child windows, Joshua 
			//if ( !MsgRouteMessage( ( ((WND)nMsg.window)->parentwin != NULL ? ((WND)nMsg.window)->parentwin : (WND)nMsg.window ), &nMsg ) )
			if ( !MsgRouteMessage( PrvFindTopWindow((WND)nMsg.window), &nMsg ) )
			{
				// Since current window proc didn't handle the message,
				// we will hand this over to system event handler
				MsgDispatchSystemMessage( &nMsg );
			}
		}
    }

    DiagDeinit();

	TrcPrint("before sending AWM_DESTROY\n");
	StaGetCurrentProc() ((WND)nMsg.window, AWM_DESTROY, 0, 0);
	TrcPrint("after sending AWM_DESTROY\n");
    TRACE(TR_RexyCore, TR_LEVEL1, ("MsgGetMessage End\n"));

}

/** @} */ // End of Logic Main
