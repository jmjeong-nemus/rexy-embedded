/******************************************************************************
 *
 * Copyright (c) 2005 Nemustech Mobile Dept.
 *
 * File: HwrInterrupts.c
 *
 * Description:
 *      Initialize interrrupts for S3C2410
 *
 * History:
 *      05/16/2005 Created By Jaewoong Sim
 *
 *****************************************************************************/

#include "includes.h"
#include "frmwrk.h"
#include "HwrTimer.h"

// HwrInterruptInit-*
void HwrInterruptInit(void)
{
    HwrS3C2410INTRegPtr INTBaseP = (HwrS3C2410INTRegPtr)HWRS3C2410_INT_BASE;

    // Interrupt Unmasking
    INTBaseP->rINTMSK &= (0xFFFF9FFF);		// Timer Handler

    // Set Timer(Tick) Interrupt
    pISR_TIMER4 = (INT32U)OSTickISR;
}

