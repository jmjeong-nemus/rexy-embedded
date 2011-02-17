/******************************************************************************
 *
 * Copyright (c) 2005 Nemustech Mobile Dept.
 *
 * File: queue.c
 *
 * Description:
 *      queue for Key Event
 *
 * History:
 *      05/24/2005 Created By tae-hwan jang (jangth@mococo.com)
 *
 *****************************************************************************/
#include "includes.h"
#include "frmwrk.h"
#include "queue.h"

// global variable
KeyEventType queue[MAX];

int rear=0;
int front=0;

#define AWM_TIMER (0x1000+19)
int EnqueueKeyEvent(KeyEventType * dataP)
{
    if((rear + 1) % MAX == front)
    {
        // queue is overflow!!
		CONSOL_Printf("EnqueueKeyEvent full %d, %d\n", front, rear);
        return 0;
    }
    
    if ( dataP->keyStatus == AWM_TIMER )
    {
    	int t_front;
    	for( t_front = front ; t_front != rear ; t_front = (t_front+1)%MAX )
    	{
    		if ( queue[t_front].keyStatus == AWM_TIMER )
    		return 0;
    	}
    }

    queue[rear].keyCode = dataP->keyCode;
    queue[rear].keyStatus = dataP->keyStatus;
    rear = (rear + 1) % MAX;
    return 1;
}

int GetKeyEvent(KeyEventType * dataP)
{
    DisableKeyTimerInterrupt();

    if( front == rear )
    {
        // queue is empty!!
	    EnableKeyTimerInterrupt();
        return 0;
    }

    dataP->keyCode = queue[front].keyCode;
    dataP->keyStatus = queue[front].keyStatus;

    front = (front + 1) % MAX;
    EnableKeyTimerInterrupt();

    return 1;
}

void DumpKeyEvent(void)
{
	int fr, rr;
	int count;
    DisableKeyTimerInterrupt();
    fr = front;
    count = 0;
    while( fr != rear )
    {
    	CONSOL_Printf("%d ", queue[fr].keyCode);
    	count++;
    	fr = (fr+1)%MAX;
    }
    if ( count )
	    CONSOL_Printf("\n");
	else
		CONSOL_Printf("no key event\n");
    EnableKeyTimerInterrupt();
}
