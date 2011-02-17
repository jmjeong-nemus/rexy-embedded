/*
 * $Id: rexy_msgq.c 435 2005-02-16 08:25:12Z longdal $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 *	Message manipulation API
 *
 */

/** @file */

/**
* @defgroup msgq Core Message Queue Service
* Message Queue API
* @ingroup RexyECoreGroup
* @{
*/

#include "rexy.h"
#include "rexy_msgq.h"
#define DPRINT(x)


/*======================================================================*/
/* Declarations								*/
/*======================================================================*/
/*======================================================================*/
/* Public APIs								*/
/*======================================================================*/

bool is_previous_repeat(int code);

/*======================================================================*/
/* Private								*/
/*======================================================================*/
static void PrvInitQObj(queue_obj * queue);
static void PrvInitMObj(msg_obj * msg);
unsigned int PrvMsgCount(queue_obj* queue);

/*======================================================================*/
/* Module Variables							*/
/*======================================================================*/
process_queue	g_process_q;
static msg_obj 	g_msg_pool[MAX_MSG_STORAGE];
static msg_obj	* g_free_msg = NULL;
int		g_mouse_cnt = 0;
static int g_message_count = 0;

/*======================================================================*/
/* Implementation							*/
/*======================================================================*/

/**
* @brief		
* @return		void
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
void MgqInitFreeStructures()
{
    int i;
    //Normal, Paint, Timer
    PrvInitQObj(&g_process_q.norm);
    PrvInitQObj(&g_process_q.paint);
    PrvInitQObj(&g_process_q.timer);

    MemSet((void *)g_msg_pool, 0, sizeof(g_msg_pool));

    for ( i = 0; i < (MAX_MSG_STORAGE - 1); ++i )
        g_msg_pool[i].m_next = &g_msg_pool[i+1];

    g_msg_pool[i].m_next = NULL;
    g_free_msg = g_msg_pool;
    return;
}

/**
* @brief		
* @return		void
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
void MgqDelMsgFreeStructures()
{
    return;
}

/**
* @brief		
* @param[in]	size	
* @return		
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
msg_obj * MgqMalloc( int size )
{
    msg_obj * new_msg;
    LckLock(LOCK_ID_ALLOC_M);

    if ( g_free_msg == NULL )
    {
        ASSERT(FALSE);
        LckUnlock(LOCK_ID_ALLOC_M);
        return NULL;
    }
    else
    {
        new_msg = g_free_msg;
        g_free_msg = g_free_msg->m_next;
    }

    PrvInitMObj(new_msg);
    g_message_count++;

    LckUnlock(LOCK_ID_ALLOC_M);
    return new_msg;
}

/**
* @brief		
* @param	msg .		
* @return		
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
void MgqFree( msg_obj * msg )
{
    LckLock(LOCK_ID_ALLOC_M);
    msg->m_next = g_free_msg;
    g_free_msg = msg;
    g_message_count--;
    LckUnlock(LOCK_ID_ALLOC_M);
    return;
}

/**
* @brief		
* @return		
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
int MgqGetMessageCount(void)
{
    int ret;
    LckLock(LOCK_ID_ALLOC_M);
    ret = g_message_count;
    LckUnlock(LOCK_ID_ALLOC_M);
    return ret;
}

/**
* @brief		
* @param	msg_q .	
* @param	panel .		
* @return		
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
void MgqRmQMsg(queue_obj * msg_q, int panel)
{
    msg_obj * prev;
    msg_obj * msg;
    msg_obj * del_msg;

    //ASSERT(PrvIsMsgqSemaLocked());
    prev = NULL;
    msg = msg_q->m_head;
    while( msg != NULL )
    {
        if ( msg->window == panel )
        {
            del_msg = msg;
            msg = msg->m_next;
            MgqDelMsg(msg_q, del_msg, prev);
        }
        else
        {
            prev = msg;
            msg = msg->m_next;
        }
    }
}

/**
* @brief ..
* @param	msg_queue .	
* @param	msg .
* @param	prev_msg . 
* @return		
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
void MgqDelMsg( queue_obj * msg_queue, msg_obj * msg, msg_obj * prev_msg )
{
    //ASSERT(PrvIsMsgqSemaLocked());

    if ( msg == NULL )
    {
        return;
    }

    if ( prev_msg == NULL )	// msg is equal to m_head
    {
        msg_queue->m_head = msg->m_next;
        MgqFree(msg);
    }
    else
    {
        if ( msg == msg_queue->m_tail )
            msg_queue->m_tail = prev_msg;

        prev_msg->m_next = msg->m_next;
        MgqFree(msg);
    }

    return;
}

/**
* @brief		
* @param	msg_queue
* @param	msg
* @return		
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
int MgqPushMsgAhead(queue_obj * msg_queue, msg_obj * msg)
{
    //ASSERT(PrvIsMsgqSemaLocked());

    if ( msg_queue == NULL )
    {
        return -1;
    }

    msg->m_next = NULL;
    if ( msg_queue->m_head == NULL )
    {
        msg_queue->m_head = msg_queue->m_tail = msg;
    }
    else
    {
        msg->m_next = msg_queue->m_head;
        msg_queue->m_head = msg;
    }

    return 0;
}

/*
bool is_previous_repeat(int code)
{
    msg_obj* msg;
    bool ret;

    msg = g_process_q.norm.m_head;
    ret = FALSE;

    while( msg )
    {
        if ( msg->wmsg == AWM_KEYREPEAT && msg->wparam == code )
        {
            ret = TRUE;
            break;
        }
        msg = msg->m_next;
    }
    return ret;
}*/

/**
* @brief		
* @param	msg_queue . 
* @param	msg .
* @return		
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
int MgqPushMsgDirect(queue_obj * msg_queue, msg_obj * msg)
{
    msg_obj * tracer;

    DPRINT((COLOR_CYAN, "MgqPushMsgDirect"));
    //ASSERT(PrvIsMsgqSemaLocked());

    if ( msg == NULL )
    {
        return -1;
    }

    msg->m_next = NULL;
    if ( msg_queue->m_head == NULL )
    {
        msg_queue->m_head = msg_queue->m_tail = msg;
        return 0;
    }

    ASSERT( msg_queue->m_head && msg_queue->m_tail );

    switch ( msg->wmsg )
    {
    case AWM_UPDATECONTROL:
    case AWM_ERASEBKGND:
    case AWM_PAINT :
        tracer = msg_queue->m_head;
        while ( tracer != NULL )
        {
            if ( (tracer->window == msg->window) && (tracer->wmsg == msg->wmsg) )
            {
                MgqFree(msg);
                break;
            }
            tracer = tracer->m_next;
        }

        if ( tracer == NULL )	// New Paint Message
        {
            msg_queue->m_tail->m_next = msg;
            msg_queue->m_tail = msg;
        }
        return 0;

    case AWM_TIMER :
        tracer = msg_queue->m_head;
        while ( tracer != NULL )
        {
            if ( (tracer->wparam == msg->wparam) && (tracer->window == msg->window) )
            {
                MgqFree(msg);
                break;
            }
            tracer = tracer->m_next;
        }

        if ( tracer == NULL )	// New Timer Message
        {
            msg_queue->m_tail->m_next = msg;
            msg_queue->m_tail = msg;
        }
        return 0;

    case AWM_KEYUP:
        {
            int key_code;
            msg_obj* prev_tracer;
            msg_obj* del_msg;

            key_code = msg->wparam;
            tracer = msg_queue->m_head;
            prev_tracer = NULL;
            while( tracer )
            {
                if ( tracer->wmsg == AWM_KEYREPEAT && tracer->wparam == key_code )
                {
                    del_msg = tracer;
                    tracer = tracer->m_next;
                    MgqDelMsg( msg_queue, del_msg, prev_tracer );
                }
                else
                {
                    prev_tracer = tracer;
                    tracer = tracer->m_next;
                }
            }
            if ( msg_queue->m_head )
            {
                msg_queue->m_tail->m_next = msg;
                msg_queue->m_tail = msg;
            }
            else
            {
                msg_queue->m_tail = msg_queue->m_head = msg;
            }
            return 0;
        }

    default :
        msg_queue->m_tail->m_next = msg;
        msg_queue->m_tail = msg;
        return 0;
    }

    return -1;
}

// queue utility functions
void PrvInitQObj( queue_obj * queue )
{
    MemSet((void *)queue, 0, sizeof(queue_obj));
    return;
}

void PrvInitMObj( msg_obj * msg )
{
    MemSet((void *)msg, 0, sizeof(msg_obj));
    return;
}

unsigned int PrvMsgCount(queue_obj* queue)
{
    msg_obj* msg;
    unsigned int count;

    count = 0;
    msg = queue->m_head;
    while(msg)
    {
        count++;
        msg = msg->m_next;
    }
    return count;
}


/** @} */ // End of Message Queue
