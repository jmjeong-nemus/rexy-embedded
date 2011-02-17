#ifndef __QUEUE_H__
#define __QUEUE_H__


#define MAX 100

typedef struct{
    int	keyCode;
    int	keyStatus;
} KeyEventType;

extern int EnqueueKeyEvent(KeyEventType * dataP);
extern int GetKeyEvent(KeyEventType * dataP);

#endif //__QUEUE_H__
