/*
 * $Id: rexy_msgq.h 8501 2005-10-07 05:33:26Z klenui $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 *	Message Queue관련 상수및 자료 구조가 정의되어 있다.	
 *
 */

#ifndef __REXY_MSGQ_H__
#define __REXY_MSGQ_H__

#ifdef __cplusplus
extern "C" {
#endif

    /** @file */

#define MAX_MSG_STORAGE	100
#define MOUSE_MAX_CNT	10
#define MSG_QUEUE_NUMBER 3
#define NORM_MSG 0
#define TIMER_MSG 1
#define PAINT_MSG 2

#define ALLOC_MSG(x)    if ( (x = MgqMalloc(sizeof(msg_obj)) ) == NULL) fprintf(stderr, "Can't alloc new message, out of memory?")

    typedef struct _msg_obj {
        int	wmsg;
        int	window;
        int	wparam;
        int	lparam;
        struct _msg_obj * m_next;
    } msg_obj;

    typedef struct _queue_obj {
        msg_obj * m_head;
        msg_obj * m_tail;
    } queue_obj;

    typedef struct _process_queue {
        queue_obj norm;
        queue_obj paint;
        queue_obj timer;
    } process_queue;

    void init_q_obj( queue_obj * queue );
    void init_m_obj( msg_obj * msg );
    msg_obj * MgqMalloc( int size );
    void MgqFree( msg_obj * msg );

    void	MgqRmQMsg(queue_obj * msg_q, int panel);
    
	void	MgqDelMsg( queue_obj * msg_queue, msg_obj * msg, msg_obj * prev_msg );
	int		MgqGetMessageCount(void);
    
	int MgqPushMsgAhead(queue_obj * msg_queue, msg_obj * msg);
    int MgqPushMsgDirect(queue_obj * msg_queue, msg_obj * msg);

    void MgqInitFreeStructures(void);
    void MgqDelMsgFreeStructures(void);

    extern process_queue	g_process_q;


#ifdef __cplusplus
}
#endif

#endif // __REXY_MSGQ_H__
