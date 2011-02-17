/*
 * $Id: rexy_user.c 435 2005-02-16 08:25:12Z longdal $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 *	Caret, Timer API
 *
 */

/** @file */

/**
 * @defgroup user Core User Service
 * User Service API
 * @ingroup RexyECoreGroup
 * @{
 */

#include "rexy.h"
#define DPRINT(x)
//#define TIMER_DEBUG

/*======================================================================*/
/* Private								*/
/*======================================================================*/
static void	PrvRemoveTimerQueue(rint32 id, rint32 window);
static rint32 PrvFindTimerID(WND window, rint32 uIDEvent, bool bFindEmptySlot);
static void	PrvRemoveTimerQueue(rint32 id, rint32 window);
static rint32 PrvKillTimerSlot(WND window,rint32 uIDEvent);
static rint32 PrvKillTimerAllInternal(WND window);
static ruint32 PrvGetNextTimerElapse(void);
static void PrvSetKernelTimer(ruint32 uElapse);
static void PrvKillKernelTimer(void);

/*======================================================================*/
/* Module Variables							*/
/*======================================================================*/
tm_msg		g_tm_schedule [MAX_TIMER_NUM];	//Timer Message¸¦ ?§ÇÑ ±¸Á¶Ã¼ ¹è¿­
static rint32	g_standby_timer_enabled = 1;

static struct
{
     ruint32 KernelTick;	
     ruint32 KernelElapse;
} s_Timer = { 0,0 };

/*======================================================================*/
/* Implementation							*/
/*======================================================================*/

/**
 * @brief		Change idle timer.
 * @param[in]	new_period	
 * @return		void
 * @remarks		
 * @see			
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
void UsrChangeIdleTimer(rint32 new_period)
{
     rint32 i;

     for ( i = 0; i < MAX_TIMER_NUM; ++i )
     {
	  if ( g_tm_schedule[i].wparam_id == REXY_IDLE_STANDBY_TIMER )
	  {
	       if ( new_period <= 0 )
	       {
		    g_tm_schedule[i].due = g_tm_schedule[i].interval;
	       }
	       else
	       {
		    g_tm_schedule[i].due += new_period;
	       }
	  }
     }
     return;
}

/**
 * @brief		Pause standby timer.
 * @return		void
 * @remarks		
 * @see			
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
void UsrPauseStandbyTimer(void)
{
     g_standby_timer_enabled = 0;
     return;
}

/**
 * @brief		Check as if standby timer is enable.
 * @return		Timer enable : TRUE\n
 Timer disable : FALSE
 * 		
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
rint32 UsrIsStandbyTimerEnabled(void)
{
     return g_standby_timer_enabled;
}

/** 
 * @brief	Get system's current tick
 * @return	current tick value as ruint32
 * @if HISTORY
 *		10-Jan-2006	Joshua	Created
 * @endif
 */
ruint32 UsrGetCurrentTicks()
{
     return RalTimeGetTicks();
}

/**
 * @brief		Resume standby timer.
 * @return		void
 * @remarks		
 * @see			
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
void UsrResumeStandbyTimer(void)
{
     g_standby_timer_enabled = 1;
     return;
}

/**
 * @brief		Insert timer message
 * @param[in]	time_msg
 * @return		void
 * @remarks		
 * @see			
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
void UsrInsertTimerMsg(long time_msg)
{
     rint32 i;
     rint32 retval=FALSE;

     //TrcPrint("UsrInsertTimerMsg--\n");

     for ( i=0; i<MAX_TIMER_NUM; ++i )
     {
	  if ( g_tm_schedule[i].wparam_id > 0 )
	  {
	       g_tm_schedule[i].due -= time_msg;

	       if ( g_tm_schedule[i].due > 0 )
	       {
		    continue;
	       }
	       else
	       {
		    //Reset duration so that can be triggered again
		    g_tm_schedule[i].due = g_tm_schedule[i].interval;
		    retval = TRUE;
	       }

	       //Handle Expired Timers
	       switch (g_tm_schedule[i].wparam_id)
	       {
// 				case CARET_TIMER:
// 					AniCaretPaint();
// 					break;

	       default:
	       {
		    //TrcPrintF("Rexy timer slot fired ! #%d\n",g_tm_schedule[i].wparam_id);
		    MsgPostMessage( g_tm_schedule[i].window, AWM_TIMER, g_tm_schedule[i].wparam_id, 0 );
	       }					
	       break;
	       }
	  }
     }
}

/**
 * @brief		Disable timer
 * @param[in]	nIDEvent
 * @return		void
 * @remarks		
 * @see			
 * @if HISTORY
 *				11-Jan-2006 madholic	Initiated 
 *				02-May-2006 Joshua		Updated to support new PrvFindTimerID
 * @endif
 */
void UsrDisableTimerInternal(rint32 nIDEvent)
{
     rint32 id = PrvFindTimerID(NULL, nIDEvent, FALSE);

     if (id >= 0) {
	  ASSERT(g_tm_schedule[id].wparam_id > 0);
        
	  PrvRemoveTimerQueue((rint32)g_tm_schedule[id].wparam_id, (rint32)g_tm_schedule[id].window);
	  g_tm_schedule[id].wparam_id = g_tm_schedule[id].wparam_id * (-1);
     }
}

/**
 * @brief		Enable timer.
 * @param[in]	nIDEvent
 * @return		void
 * @remarks		
 * @see			
 * @if HISTORY
 *				11-Jan-2006 madholic	Initiated 
 *				02-May-2006	Joshua		Use new PrvFindTimerID()
 * @endif
 */
void UsrEnableTimerInternal(rint32 nIDEvent)
{
     rint32 id = PrvFindTimerID(NULL, nIDEvent,FALSE);

     if (id >= 0) {
	  ASSERT(g_tm_schedule[id].wparam_id< 0);
	  g_tm_schedule[id].wparam_id = g_tm_schedule[id].wparam_id * (-1);
     }
}

/**
 * @brief		Set a timer
 * @param[in]	window		window handle
 * @param[in]	nIDEvent	timer identifier
 * @param[in]	uElapse		the time-out value, in milliseconds.
 * @retval		Timer ID	If timer set succeeds.
 * @retval		error code	other case
 * @remarks		
 * @see			
 * @if HISTORY
 *				11-Jan-2006 madholic	Initiated 
 *				02-May-2006	Joshua		New timer algorithm applied ( elapse based )
 * @endif
 */
rint32 UsrSetTimerInternal(WND window, rint32 nIDEvent, rint32 uElapse)
{
     //struct timeval current_time;
     rint32 RemainTime;
     ruint32 ElapsedTime;
     ruint32 KernelSetTime;
     rint32 id;
     ruint32 CurrentTick;

     if ((nIDEvent <= 0) || (uElapse == 0)) { // Error Check
	  return (RxErrUsrClass | enInvalidParam);
     }

     if (PrvFindTimerID(window, nIDEvent, FALSE) >= 0) {
	  PrvKillTimerSlot(window, nIDEvent);
     }

     id = PrvFindTimerID(window, nIDEvent, TRUE);           // find the empty slot if nIDEvent is normal ones,
     // find the reserved slot if nIDEvent is one of reserved ones
     //Debugging purpose
     ASSERT (id>=0);

     if (id < 0) return -1;                  // if not exist

     CurrentTick = RalTimeGetTicks();
     KernelSetTime = s_Timer.KernelTick;

#ifdef TIMER_DEBUG
     TrcPrintF("UsrSetTimerInternal / elapse : %d\n",uElapse);
#endif

     if ( KernelSetTime )
     {
	  // Update Timer Slot, Joshua on 17-Mar-2006
	  ElapsedTime = CurrentTick - KernelSetTime;
	  RemainTime	= s_Timer.KernelElapse - ElapsedTime;
     }
     else
     {
	  ElapsedTime=0;
	  RemainTime=0;
     }

     g_tm_schedule[id].window = window;
     g_tm_schedule[id].wparam_id = nIDEvent;
     g_tm_schedule[id].interval = uElapse;
     g_tm_schedule[id].due = uElapse+ElapsedTime;
	
     // We have to change the kernel timer's schedule
     if ( uElapse < RemainTime || !RemainTime )
     {
	  g_tm_schedule[id].due = uElapse + ElapsedTime;
	  UsrInsertTimerMsg( ElapsedTime );	// to sync other timer slots
	  PrvSetKernelTimer( uElapse );
     }

     return id;

}

/**
 * @brief		Set the timer in msec.
 * @param[in]	window		Window handle to set the timer.
 * @param[in]	nIDEvent	Timer ID. Timer ID should be larger than REXY_APPLICATION_TIMER.
 * @param[in]	uElapse		Time elapse. Unit is msec.
 * @retval		Timer ID	If timer set succeeds.
 * @retval		error code	other case
 * @remarks		
 * @see			
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
rint32 UsrSetTimer(WND window, rint32 nIDEvent, rint32 uElapse )
{
     return UsrSetTimerInternal(window, nIDEvent, uElapse);
}

/**
 * @brief		Kill timer
 * @param[in]	window		window handle
 * @param[in]	uIDEvent	timer identifier
 * @retval		1			On success
 * @retval		0			other case
 * @remarks		
 * @see			
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
rint32 UsrKillTimerInternal(WND window, rint32 uIDEvent)
{
     rint32 id;
     rint32 processed = 0;
     ruint32 NextElapse;
     ruint32 ElapsedTime;
     ruint32 KernelSetTime;

#ifdef TIMER_DEBUG
     TrcPrintF("UsrKillTimerInternal / uIDEvent : %ld  at %ld\n",uIDEvent,s_Timer.KernelTick);
#endif

     if (uIDEvent == ALL_TIMER) {
	  processed = PrvKillTimerAllInternal(window);
     }
     else {
	  if ((id = PrvFindTimerID(window, uIDEvent, FALSE)) >= 0) 
	  {
	       processed = 1;

	       PrvRemoveTimerQueue((rint32)g_tm_schedule[id].wparam_id, (rint32)g_tm_schedule[id].window);
            
	       g_tm_schedule[id].wparam_id = 0;
	       g_tm_schedule[id].window = NULL;
	       g_tm_schedule[id].interval = 0;
	       g_tm_schedule[id].due = 0;
	  }
     }
     if (!processed) return 0;

     //Joshua modified on 17-Mar-2006

     NextElapse = PrvGetNextTimerElapse();

     if (NextElapse)
     {
	  if ( NextElapse != s_Timer.KernelElapse )
	  {
	       KernelSetTime = s_Timer.KernelTick;

	       // If there was no timer set before, 'processed' must be 0, and line should not reach here
	       ASSERT(KernelSetTime);	

	       if ( KernelSetTime )
	       {
		    // Update Timer Slot, Joshua on 17-Mar-2006
		    ElapsedTime = RalTimeGetTicks() - KernelSetTime;
		    UsrInsertTimerMsg(ElapsedTime);	//to sync other timer slots
		    if ( NextElapse > ElapsedTime )
		    {
			 PrvSetKernelTimer( NextElapse-ElapsedTime );
		    }
	       }
	  }
     }
     else
     {
	  PrvKillKernelTimer();
     }

     return 1;

}

/**
 * @brief		Kill the specified timer.
 * @param[in]	window		Window handle to kill timer.
 * @param[in]	uIDEvent	Timer ID. Timer ID should be larger than REXY_APPLICATION_TIMER.
 * @retval		1			If timer is killed successfully
 * @retval		0			Otherwise,
 * @remarks		
 * @see			
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
rint32 UsrKillTimer(WND window, rint32 uIDEvent)
{
//    if (uIDEvent <= REXY_APPLICATION_TIMER) {
//        // AS_Trace("######## UsrKillTimer - uIDEvent : [%d]\n", uIDEvent);
//        ASSERT(FALSE);
//        return 0;
//    }
     return UsrKillTimerInternal(window, uIDEvent);
}

/**
 * @brief		Get current timer
 * @param[out]	current_time		a structure that will be received current time
 * @retval		RxErrNone			On success
 * @retval		proper error code	Otherwise
 * @remarks		
 * @see			
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
rint32 UsrGetCurrentTime(struct tm * current_time)
{
     bool ret;
     ret = RalTimeGetCurrent(current_time);
     if ( !ret ) return RxErrUsrClass|enSystemError;
     else return RxErrNone;
}
static void PrvSetKernelTimer(ruint32 uElapse)
{

     s_Timer.KernelTick=RalTimeGetTicks();
     s_Timer.KernelElapse=uElapse;

     RalTimeSetTimer( uElapse, HAL_TIMER_ID );

#ifdef TIMER_DEBUG
     TrcPrintF("PrvSetKernelTimer / uElapse : %ld  at %ld\n",uElapse,s_Timer.KernelTick);
#endif
}
static void PrvKillKernelTimer()
{
#ifdef TIMER_DEBUG
     TrcPrintF("PrvKillKernelTimer");
#endif

     s_Timer.KernelTick=0;
     s_Timer.KernelElapse=0;

     RalTimeKillTimer(HAL_TIMER_ID);
}
static rint32 PrvFindTimerID(WND window, rint32 uIDEvent, bool bFindEmptySlot)
{
     rint32 i;
     rint32 TimerID;
     WND	TimerWindow;
     rint32 ReservedSlotNum =-1;

     //Check for the reserved timer slots first, added by Joshua
     //If the uIDEvent is one of reserved timer, bFindEmptySlot will be depreciated

     /*
       switch ( uIDEvent )
       {
       case CARET_TIMER:
       ReservedSlotNum = 0;
       break;				
       case FONT_ANIMATION_TIMER:
       ReservedSlotNum = 1;
       break;
       case PICT_CLOCK_TIMER:
       ReservedSlotNum = 2;
       break;
       case APP_IDLE_TIMER:
       ReservedSlotNum = 3;
       break;
       }
     */

     if ( ReservedSlotNum != -1)
     {
	  if ( bFindEmptySlot || g_tm_schedule[ReservedSlotNum].wparam_id == uIDEvent || 
	       g_tm_schedule[ReservedSlotNum].wparam_id == ( -1 * uIDEvent ))
	       return ReservedSlotNum;
	  else
	       return -1;
     }

     if (bFindEmptySlot) 
     {
	  TimerWindow=NULL;
	  TimerID=0;
     }
     else 
     {
	  TimerWindow=window;
	  TimerID=uIDEvent;
     }

     for ( i= RESERVED_TIMER_NUM; i < MAX_TIMER_NUM; ++i ) 
     {
	  if ((g_tm_schedule[i].wparam_id == TimerID || g_tm_schedule[i].wparam_id == (-1 * TimerID)) &&
	      (!TimerWindow || g_tm_schedule[i].window == TimerWindow) ) {
	       return i;
	  }
     }
     return -1;


}

/** 
 * @brief	Kill timer slot without rescheduling
 * @remark	This function does same thing with KillTimerInternal
 *			but does not reschedule not to burden kernel timer
 * @if HISTORY
 *		29-Mar-2006	Joshua	Created
 * @endif
 **/ 
static rint32 PrvKillTimerSlot(WND window,rint32 uIDEvent)
{
     rint32 id;
     rint32 processed = 0;

     if (uIDEvent == ALL_TIMER) {
	  processed = PrvKillTimerAllInternal(window);
     }
     else {
	  if ((id = PrvFindTimerID(window, uIDEvent, FALSE)) >= 0) 
	  {
	       processed = 1;

	       PrvRemoveTimerQueue((rint32)g_tm_schedule[id].wparam_id, (rint32)g_tm_schedule[id].window);
            
	       g_tm_schedule[id].wparam_id = 0;
	       g_tm_schedule[id].window = NULL;
	       g_tm_schedule[id].interval = 0;
	       g_tm_schedule[id].due = 0;
	  }
     }
     if (!processed) 
	  return 0;
     else
	  return 1;
}

static ruint32 PrvGetNextTimerElapse(void)
{
     long time_msg;
     rint32 j;

     time_msg = 0;

     for (j=0; j < MAX_TIMER_NUM; ++j)
     {
	  if (g_tm_schedule[j].wparam_id > 0)
	  {
	       if (!time_msg) 
		    time_msg = g_tm_schedule[j].due;
	       else
		    if ( g_tm_schedule[j].due < time_msg ) time_msg = g_tm_schedule[j].due;
	  }
     }

     return time_msg;
}
static rint32 PrvKillTimerAllInternal(WND window)
{
     rint32 processed = 0;
     msg_obj *msg, *prev, *target;
     queue_obj *msg_q;
     rint32 i;

     TRACE(TR_RexyCore, TR_LEVEL2, ("PrvKillTimerAllInternal: ALL_TIMER"));
     for(i=0; i < MAX_TIMER_NUM; ++i)
     {
	  // unregister all the timer
	  if (g_tm_schedule[i].wparam_id != 0 && g_tm_schedule[i].window == window)
	  {
	       g_tm_schedule[i].wparam_id = 0;
	       processed = 1;
	  }
     }

     if (processed)
     {
	  LckLock(LOCK_ID_MSGQ);
	  // Removing Queued AWM_TIMER MESSAGE
	  msg_q = &g_process_q.timer;
	  msg = msg_q->m_head;
	  prev = NULL;
	  PrvKillKernelTimer();

	  while(msg != NULL)
	  {
	       if (msg->window == (rint32)g_tm_schedule[i].window) {
		    // do not process window
		    target = msg;
		    msg = msg->m_next;
		    MgqDelMsg(msg_q, target, prev);
	       }
	       else {
		    prev = msg;
		    msg = msg->m_next;
	       }
	  }
	  LckUnlock(LOCK_ID_MSGQ);
     }

     return processed;
}

static void PrvRemoveTimerQueue(rint32 id, rint32 window)
{
     msg_obj * msg, * prev, *target;
     queue_obj * msg_q;

     LckLock(LOCK_ID_MSGQ);
     // Removing Queued AWM_TIMER MESSAGE
     msg_q = &g_process_q.timer;
     msg = msg_q->m_head;
     prev = NULL;

     while (msg != NULL)
     {
	  if (msg->wparam == id && window) {
	       target = msg;
	       msg = msg->m_next;
	       MgqDelMsg(msg_q, target, prev);
	  }
	  else {
	       prev = msg;
	       msg = msg->m_next;
	  }
     }
     LckUnlock(LOCK_ID_MSGQ);
}
void UsrHandleKernelTimerMessage()
{

     rint32	NextElapse;
     rint32	RealElapse;
     rint32	Deviation;
     rint32	CurrentTick;
    
	
     CurrentTick = (rint32)UsrGetCurrentTicks();
     RealElapse = CurrentTick - s_Timer.KernelTick;
     Deviation = RealElapse - s_Timer.KernelElapse;

     UsrInsertTimerMsg(s_Timer.KernelElapse);

     // Fix deviation
#ifdef DEBUG
     printf("CurrentTick:%ld,s_Timer.KernelTick:%ld,RealElapse:%ld,Deviation:%ld\n",CurrentTick,s_Timer.KernelTick,RealElapse,Deviation);
#endif
	 
     if ( Deviation > 0 ) UsrInsertTimerMsg( Deviation );
     else if ( Deviation < 0 && Deviation > -50 ) // re-correct again within 50 ms range
     {
	  while ( (rint32)UsrGetCurrentTicks() - s_Timer.KernelTick  < s_Timer.KernelElapse );
     }
	 

     NextElapse = PrvGetNextTimerElapse();

     // Here, we assume that kernel timer does not repeat
     // Joshua commented on 2008-01-18 Fri?�일 AM  9:

     if (NextElapse>0) 
	  PrvSetKernelTimer(NextElapse);	


}

/** 
 * @brief	Generates a pseudorandom number.
 * @remark	Generates a pseudorandom number.
 * @if HISTORY
 *		09-June-2006	madholic	Created
 * @endif
 **/ 
rint32 UsrRand(void)
{
     return RalRand();
}


/** @} */ // End of User Service

