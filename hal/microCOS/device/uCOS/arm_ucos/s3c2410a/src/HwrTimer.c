/******************************************************************************
 *
 * Copyright (c) 2005 Nemustech Mobile Dept.
 *
 * File: HwrTimer.c
 *
 * Description:
 *      Timer interrupts for S3C2410
 *
 * History:
 *      05/16/2005 Created By Jaewoong Sim
 *
 *****************************************************************************/

#include "includes.h"
#include "frmwrk.h"
#include "HwrKeyMgr.h"
#include "HwrTimer.h"

//extern int gDoubleTapDelayTime;
int gCurTime = 0;
int gCurTimer = 0;
int gTimerSec = 0;


void (*gFunc)(void) = NULL;

void HwrTimerHandleInterrupt_SetTimer(void);

void HwrTimerInit(INT16U timer)
{
    HwrS3C2410PWMRegPtr PWMBaseP = (HwrS3C2410PWMRegPtr)HWRS3C2410_PWM_BASE;

    // 1:2:4, FCLK=202.8Mhz, HCLK=101.4Mhz, PCLK=50.70Mhz
    //(1/(PCLK/(Prescaler+1)/divider) * count(Max 65535) = Timer clock (Frequency)
    PWMBaseP->rTCFG0 &= ~(0xFF << 8);						/* Prescaler 1's Value */
    PWMBaseP->rTCFG0 |= ((TIMER_PRESCALER1) << 8);

    // Timer 3 is a fine granularity clock used to implement HALDelay
    if (timer == 1) {
        PWMBaseP->rTCFG1 &= ~(0xF << 12);		/* Timer 3's Devider Value */
        PWMBaseP->rTCFG1 |= hwrS3C2410PWMTimer3DivBy2;			/* 1/2 */

        //PWMBaseP->rTCON = 0x0;
        PWMBaseP->rTCNTB3  = 9750/10; // 10으로 나눈 이유는 단위시간을 1ms로 하기 위함 (mio)

        PWMBaseP->rTCON |= hwrS3C2410Timer3Update;			/* update TCVNTB4, stop */

        // Update bit have to be cleared at next write time
        PWMBaseP->rTCON &= ~hwrS3C2410Timer3Update;			/* Clear Update Bit */

        PWMBaseP->rTCON |= (hwrS3C2410Timer3AutoReload |	/* interval mode, start */
                            hwrS3C2410Timer3Start);

        // (1/(50700000/(25+1)/2)) * 9750 = 0.010sec = 10mS
        //		PWMBaseP->rTCNTB4  = 9750;
        //		PWMBaseP->rTCON |= hwrS3C2410Timer4Update;			/* update TCVNTB4, stop */

        // Update bit have to be cleared at next write time
        //		PWMBaseP->rTCON &= ~hwrS3C2410Timer4Update;			/* Clear Update Bit */

        //		PWMBaseP->rTCON |= (hwrS3C2410Timer4AutoReload |	/* interval mode, start */
        //							hwrS3C2410Timer4Start);

        // set interrupt service routine
        pISR_TIMER3 = (U32)HwrTimerHandleInterrupt_SetTimer;

    }
    // Timer 4 is used for the OS tick
    // Initialize Timer 4 for a pulse every 10 milliseconds
    else if (timer == 2) {
        PWMBaseP->rTCFG1 &= ~(0xF << 16);					/* Timer 4's Devider Value */
        PWMBaseP->rTCFG1 |= hwrS3C2410PWMTimer4DivBy2;		/* 1/2 */

        // (1/(50700000/(25+1)/2)) * 9750 = 0.010sec = 10mS
        PWMBaseP->rTCNTB4  = 9750;
        PWMBaseP->rTCON |= hwrS3C2410Timer4Update;			/* update TCVNTB4, stop */

        // Update bit have to be cleared at next write time
        PWMBaseP->rTCON &= ~hwrS3C2410Timer4Update;			/* Clear Update Bit */

        PWMBaseP->rTCON |= (hwrS3C2410Timer4AutoReload |	/* interval mode, start */
                            hwrS3C2410Timer4Start);

        // set interrupt service routine
        pISR_TIMER4 = (U32)HwrTimerHandleInterrupt;
#if 0
#ifndef BUILD_OPTION_SMALL_ROM
        // Enable timer 4
        HALInterruptSetState(INT_TIMER4, IRQEnabled);
#endif // BUILD_OPTION_SMALL_ROM
#endif
    }
}

void EnableTimerSetState()
{
    HwrS3C2410INTRegPtr INTBaseP = (HwrS3C2410INTRegPtr)HWRS3C2410_INT_BASE;

    // TIMER4 Interrupt Unmasking
    INTBaseP->rINTMSK &= (0xFFFFBFFF);

}

void EnableTimerSetState_SetTimer()
{
    HwrS3C2410INTRegPtr INTBaseP = (HwrS3C2410INTRegPtr)HWRS3C2410_INT_BASE;

    // TIMER3 Interrupt Unmasking
    INTBaseP->rINTMSK &= (0xFFFF7FFF);
}

void DisableTimerSetState_SetTimer()
{
    HwrS3C2410INTRegPtr INTBaseP = (HwrS3C2410INTRegPtr)HWRS3C2410_INT_BASE;

    // TIMER3 Interrupt masking
    INTBaseP->rINTMSK |= (0x00008000);
}


int SetTimer(INT32U ticks, void (*func)(void))
{
    gTimerSec = ticks;
    gCurTimer = 0;
    gFunc = func;
    EnableTimerSetState_SetTimer();

    return 0;
}

int KillTimer()
{
    void (*gFunc)(void) = NULL;
    gTimerSec = 0;
    DisableTimerSetState_SetTimer();

    return 0;
}

void HwrTimerHandleInterrupt(void)
{
    gCurTime++;

    // Clear Pending Interrupt
    ClearPending(BIT_TIMER4);

    if( gBtnCheck )
    {
        // check up,down,left,right button
        PrvCheckButton();

        // key scan
        if( gKeyState != KEYUP )
        {
            gKeydownTime++;
            longKeyHandler();
        }
    }


    //	gDoubleTapDelayTime++;
}

INT32U HwrGetTicks()
{
    return gCurTime;
}

void HwrTimerHandleInterrupt_SetTimer(void)
{
    // Clear pending interrupt
    ClearPending(BIT_TIMER3);

    // set timer
    if( gTimerSec )
    {
        if( gTimerSec == gCurTimer )
        {
            if( gFunc )
            {
                // call callback function
                //CONSOL_Printf("Hwr timer\n");
                gFunc();
            }
            gCurTimer = 0;
        }

        gCurTimer++;
    }
}

