/******************************************************************************
 *
 * Copyright (c) 2005 Nemustech Mobile Dept.
 *
 * File: HwrTimer.h
 *
 * Description:
 *      Timer interrupts for S3C2410
 *
 * History:
 *      05/16/2005 Created By Jaewoong Sim
 *
 *****************************************************************************/

#ifndef __HWRTIMER_H__
#define __HWRTIMER_H__

#include "includes.h"

// Prototypes
void HwrTimerInit(INT16U timer);
void EnableTimerSetState(void);

void HwrTimerHandleInterrupt(void);


#endif // !__HWRTIMER_H__
