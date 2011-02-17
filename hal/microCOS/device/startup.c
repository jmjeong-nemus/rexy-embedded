/**************************************************************************
*                                                                         *
*   PROJECT     : ARM port for UCOS-II                                    *
*                                                                         *
*   MODULE      : EX1.c                                                   *
*                                                                         *
*   AUTHOR      : Michael Anburaj                                         *
*                 URL  : http://geocities.com/michaelanburaj/             *
*                 EMAIL: michaelanburaj@hotmail.com                       *
*                                                                         *
*   PROCESSOR   : Any processor                                           *
*                                                                         *
*   IDE         : Any IDE                                                 *
*                                                                         *
*   DESCRIPTION :                                                         *
*   This is a sample code to test UCOS-II.                                *
*                                                                         *
**************************************************************************/

#include "rexy.h"

#include "includes.h"
#include "consol.h"
#include "HwrDisplay.h"
#include "HwrTimer.h"
#include "queue.h"
#include "HwrKeyMgr.h"
#include "HwrRTCTimer.h"

int CheckStack(void);


/* ********************************************************************* */
/* Global definitions */


/* ********************************************************************* */
/* File local definitions */

#define  TASK_STK_SIZE 2048                      /* Size of each task's stacks (# of WORDs) */
#define  NO_TASKS           10                  /* Number of identical tasks */

OS_STK   TaskStk[NO_TASKS][TASK_STK_SIZE];      /* Tasks stacks */
OS_STK   TaskStartStk[TASK_STK_SIZE];
char     TaskData[NO_TASKS];                    /* Parameters to pass to each task */
OS_EVENT *RandomSem;


/* ********************************************************************* */
/* Local functions */
void SetRexyMessage(int wmsg, int window, int wparam, int lparam)
{
    KeyEventType event;
    event.keyCode = AWM_TIMER;
    event.keyStatus = wparam;
    EnqueueKeyEvent( &event );
}

void DAL_GetKeyEvent( int * nKeyMsgTypeP, int * nKeyCodeP, int * nKeyStatusP )
{
/*
    int ret;
    KeyEventType key;

    // Get Key Event From Device Driver
    ret = GetKeyEvent(&key);
    if( ret > 0 )
    {
        if ( key.keyCode == AWM_TIMER )
        {
            *nKeyCodeP = key.keyCode;
            *nKeyStatusP = key.keyStatus;
            return;
        }
        if( key.keyStatus <= KEYREPEAT )
        {
            *nKeyMsgTypeP = key.keyStatus + AWM_KEYDOWN;
        }
        else
        {
            *nKeyCodeP = KEYSWOFF;
            *nKeyStatusP = key.keyStatus;
            return;
        }

        *nKeyCodeP = key.keyCode;
        *nKeyStatusP = key.keyStatus;
    }
    else
    {
        *nKeyCodeP = KEYSWOFF;
    }
    */
}
void DeleteRexyTimerEvent()
{
    KillTimer();
}


void TaskStart (void *data)
{
    U8 i;
    char key;
    INT32U* vAddr = (INT32U*)DISPLAY_BASE_ADDR;
    INT32U point;
    data = data;                            /* Prevent compiler warning */
    
    CONSOL_Printf("uC/OS-II, The Real-Time Kernel ARM Ported version\n");
    CONSOL_Printf("Jean J. Labrosse/ (Ported by) Michael Anburaj\n");
    CONSOL_Printf("EXAMPLE #1\n");

    CONSOL_Printf("Determining  CPU's capacity ...\n");

    // LCD Init
    HwrDisplayInit( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_DEPTH);
    HwrDisplayWake();

    HwrTimerInit(1);

    // Key Device Init
    KeyInit();

    // RTC Clock Init
    HwrRTCInit();
    SetRTCTime(8640000);	// New Time

    OSStatInit();                           /* Initialize uC/OS-II's statistics */

    CONSOL_Printf("\n# Parameter1: No. Tasks\n");
    CONSOL_Printf("# Parameter2: CPU Usage in %%\n");
    CONSOL_Printf("# Parameter3: No. Task switches/sec\n");
    CONSOL_Printf("<-PRESS 'ESC' TO QUIT->\n");

    StartRexy();

	// never visited.
}


/* ********************************************************************* */
/* Global functions */

void APP_vMain (void)
{
    OSInit();                               /* Initialize uC/OS-II                                      */
    RandomSem = OSSemCreate(1);             /* Random number semaphore                              */
    OSTaskCreate(TaskStart, (void *)0, (void *)&TaskStartStk[TASK_STK_SIZE - 1], 0);
    FRMWRK_vStartTicker(OS_TICKS_PER_SEC);  /* os_cfg.h */
    OSStart();                              /* Start multitasking                                       */
}


int CheckStack(void)
{
	unsigned int sp_r;
	__asm
	{
		mov sp_r, sp;
	}
	return (unsigned int)(&TaskStartStk[TASK_STK_SIZE - 1])-sp_r-4;
}

void DumpStack(void)
{
	int i;
	unsigned int* sp_r;
	__asm
	{
		mov sp_r, sp
	}
	for ( i = 0 ; i < 30 ; i++ )
	{
		if ( sp_r+i > &TaskStartStk[TASK_STK_SIZE - 1] ) break;
		CONSOL_Printf("%08X: %08x\n", sp_r+i, *(sp_r+i));
	}
}
/* ********************************************************************* */
