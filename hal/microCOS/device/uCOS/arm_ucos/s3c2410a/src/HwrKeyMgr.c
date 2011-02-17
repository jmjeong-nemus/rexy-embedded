/******************************************************************************
 *
 * Copyright (c) 2005 Nemustech Mobile Dept.
 *
 * File: HwrKeyMgr.c
 *
 * Description:
 *      Key Manager for S3C2410
 *
 * History:
 *      05/16/2005 Created By Jaewoong Sim
 *
 *****************************************************************************/
#define __HWRKEY_GLOBALS__
#include "includes.h"
#include "frmwrk.h"
#include "HwrDisplay.h"
#include "HwrKeyMgr.h"
#include "HwrTimer.h"
#include "queue.h"
#include "consol.h"

int gKeyState = KEYUP;
long gKeydownTime;
int	gCurrentKey = NOT_KEY;
int gBtnCheck = 1;
int gLastKeyDownTime = 0;

extern int gCurTime;

void KeyInit(void)
{
    HwrS3C2410INTRegPtr INTBaseP = (HwrS3C2410INTRegPtr)HWRS3C2410_INT_BASE;
    HwrS3C2410IOPRegPtr IOPBaseP = (HwrS3C2410IOPRegPtr)HWRS3C2410_IOP_BASE;

    // Initialize Key Interrupt
    IOPBaseP->rGPFCON = (IOPBaseP->rGPFCON & 0xFFCC) | (2<<0) | (2<<4);			// Configure EINT0,EINT2
    //	IOPBaseP->rGPGCON = (IOPBaseP->rGPGCON & 0xFF3FFF3F) | (2<<6) | (2<<22);	// Configure EINT11,EINT19

    // init rGPGCON
    IOPBaseP->rGPGCON &= (0<<6)|(0<<7)|(0<<22)|(0<<23)|(0<<4)|(0<<5)|(0<<10)|(0<<11)|(0<<12)|(0<<13)|(0<<14)|(0<<15);
    IOPBaseP->rGPGCON |= (2<<6);	// Configure EINT11
    IOPBaseP->rGPGCON |= (2<<22);	// Configure EINT19

    IOPBaseP->rGPGCON |= (2<<4);	// Configure EINT10
    IOPBaseP->rGPGCON |= (2<<10);	// Configure EINT13
    IOPBaseP->rGPGCON |= (2<<12);	// Configure EINT14
    IOPBaseP->rGPGCON |= (2<<14);	// Configure EINT15

    IOPBaseP->rGPBCON &= (0<<10)|(0<<11);	// Configure UP
    IOPBaseP->rGPBCON &= (0<<12)|(0<<13);	// Configure DOWN
    IOPBaseP->rGPECON &= (0<<22)|(0<<23);	// Configure LEFT
    IOPBaseP->rGPECON &= (0<<24)|(0<<25);	// Configure RIGHT

    // both Edge trigger setting
    IOPBaseP->rEXTINT0 |= (7<<0);	// EINT0
    IOPBaseP->rEXTINT0 |= (7<<8);	// EINT2
    IOPBaseP->rEXTINT1 |= (7<<12);	// EINT11
    IOPBaseP->rEXTINT2 |= (7<<12);	// EINT19

    IOPBaseP->rEXTINT1 |= (7<<8);	// EINT10
    IOPBaseP->rEXTINT1 |= (7<<20);	// EINT13
    IOPBaseP->rEXTINT1 |= (7<<24);	// EINT14
    IOPBaseP->rEXTINT1 |= (7<<28);	// EINT15

    pISR_EINT0 = (INT32U)KeyInterruptHandlerEINT0;
    pISR_EINT2 = (INT32U)KeyInterruptHandlerEINT2;
    pISR_EINT8_23 = (INT32U)KeyInterruptHandlerEINT8_23;

    // for long and repeat key
    HwrTimerInit(2);

    // Interrupt Enable
    EnableKeyInterrupt();

    // enabled timer interrupt
    EnableTimerSetState();

    CONSOL_Printf("\nKey Init!!\n");
}

void DisableKeyInterrupt()
{
    HwrS3C2410INTRegPtr INTBaseP = (HwrS3C2410INTRegPtr)HWRS3C2410_INT_BASE;
    HwrS3C2410IOPRegPtr IOPBaseP = (HwrS3C2410IOPRegPtr)HWRS3C2410_IOP_BASE;

    // EINT0, EINT2 Interrupt masking
    INTBaseP->rINTMSK |= (0x00000005);

    INTBaseP->rINTMSK |= (1<<5);	// EINT8_23

    IOPBaseP->rEINTMASK |= (1<<11);	// EINT11
    IOPBaseP->rEINTMASK |= (1<<19);	// EINT19

    IOPBaseP->rEINTMASK |= (1<<10);	// EINT10
    IOPBaseP->rEINTMASK |= (1<<13);	// EINT13
    IOPBaseP->rEINTMASK |= (1<<14);	// EINT14
    IOPBaseP->rEINTMASK |= (1<<15);	// EINT15
}


void EnableKeyInterrupt()
{
    HwrS3C2410INTRegPtr INTBaseP = (HwrS3C2410INTRegPtr)HWRS3C2410_INT_BASE;
    HwrS3C2410IOPRegPtr IOPBaseP = (HwrS3C2410IOPRegPtr)HWRS3C2410_IOP_BASE;

    // EINT0, EINT2 Interrupt Unmasking
    INTBaseP->rINTMSK &= (0xFFFFFFFA);	// EINT0, EINT2

    INTBaseP->rINTMSK &= ~(1<<5);		// EINT8_23

    IOPBaseP->rEINTMASK &= ~(1<<11);	// EINT11
    IOPBaseP->rEINTMASK &= ~(1<<19);	// EINT19

    IOPBaseP->rEINTMASK &= ~(1<<10);	// EINT10
    IOPBaseP->rEINTMASK &= ~(1<<13);	// EINT13
    IOPBaseP->rEINTMASK &= ~(1<<14);	// EINT14
    IOPBaseP->rEINTMASK &= ~(1<<15);	// EINT15
}

void DisableKeyTimerInterrupt()
{
    HwrS3C2410INTRegPtr INTBaseP = (HwrS3C2410INTRegPtr)HWRS3C2410_INT_BASE;
    HwrS3C2410IOPRegPtr IOPBaseP = (HwrS3C2410IOPRegPtr)HWRS3C2410_IOP_BASE;

    // EINT0, EINT2 Interrupt masking
    INTBaseP->rINTMSK |= (0x00000005);
    INTBaseP->rINTMSK |= (0x00008000);

    INTBaseP->rINTMSK |= (1<<5);	// EINT8_23

    IOPBaseP->rEINTMASK |= (1<<11);	// EINT11
    IOPBaseP->rEINTMASK |= (1<<19);	// EINT19

    IOPBaseP->rEINTMASK |= (1<<10);	// EINT10
    IOPBaseP->rEINTMASK |= (1<<13);	// EINT13
    IOPBaseP->rEINTMASK |= (1<<14);	// EINT14
    IOPBaseP->rEINTMASK |= (1<<15);	// EINT15
}


void EnableKeyTimerInterrupt()
{
    HwrS3C2410INTRegPtr INTBaseP = (HwrS3C2410INTRegPtr)HWRS3C2410_INT_BASE;
    HwrS3C2410IOPRegPtr IOPBaseP = (HwrS3C2410IOPRegPtr)HWRS3C2410_IOP_BASE;

    // EINT0, EINT2 Interrupt Unmasking
    INTBaseP->rINTMSK &= (0xFFFFFFFA);	// EINT0, EINT2
    INTBaseP->rINTMSK &= (0xFFFF7FFF);

    INTBaseP->rINTMSK &= ~(1<<5);		// EINT8_23

    IOPBaseP->rEINTMASK &= ~(1<<11);	// EINT11
    IOPBaseP->rEINTMASK &= ~(1<<19);	// EINT19

    IOPBaseP->rEINTMASK &= ~(1<<10);	// EINT10
    IOPBaseP->rEINTMASK &= ~(1<<13);	// EINT13
    IOPBaseP->rEINTMASK &= ~(1<<14);	// EINT14
    IOPBaseP->rEINTMASK &= ~(1<<15);	// EINT15
}


int PrvCheckDoubleTab(int keyState)
{
    int nDoubleTabDelayTime = 15;

    if( keyState == KEYDOWN && gLastKeyDownTime )
    {
        //CONSOL_Printf("\ncheck double tab !!!!    %x %x\n", gLastKeyDownTime, gCurTime);
        if( gLastKeyDownTime + nDoubleTabDelayTime > gCurTime )
            return 1;
    }
    return 0;
}

void KeyEventInsert(int keyCode, int keyState)
{
    DisableKeyTimerInterrupt();

    if( keyCode != NOT_KEY )
    {
        if( !PrvCheckDoubleTab(keyState) )
        {
            KeyEventType keyData;

            keyData.keyCode = keyCode;
            keyData.keyStatus = keyState;

            if( keyState == KEYDOWN )
            {
                // save the last key down time
                gLastKeyDownTime = gCurTime;
            }

            /* key re-mapping 필요함 */
            if( !EnqueueKeyEvent(&keyData) )
                CONSOL_Printf("\nevent queue is full!!!!\n");
        }
    }

    EnableKeyTimerInterrupt();
}

static void KeyInterruptHandlerEINT0()
{
    HwrS3C2410IOPRegPtr IOPBaseP = (HwrS3C2410IOPRegPtr)HWRS3C2410_IOP_BASE;

    if ((IOPBaseP->rGPFDAT & (1<<0)) == 0) {	// EINT0
        gKeyState = KEYDOWN;
        gCurrentKey = EINT0_KEY;
    }
    else if ((IOPBaseP->rGPFDAT & (1<<0)) == 1) {
        gKeyState = KEYUP;
    }

    if( gKeyState == KEYUP )
    {
        KeyEventInsert(gCurrentKey, gKeyState);
        gKeydownTime = 0;
        gCurrentKey = NOT_KEY;
    }
    else
    {
        if( gKeyState == KEYDOWN && gKeydownTime == 0 )
        {
            KeyEventInsert(gCurrentKey, gKeyState);
        }
        /*
        else
            CONSOL_Printf("\nOn Yessssssssssssssssssssssssssssssssss !!!!\n");
        */
    }

    ClearPending(BIT_EINT0);
}

static void KeyInterruptHandlerEINT2()
{
    HwrS3C2410IOPRegPtr IOPBaseP = (HwrS3C2410IOPRegPtr)HWRS3C2410_IOP_BASE;

    if ((IOPBaseP->rGPFDAT & (1<<2)) == 0) {	// EINT2
        gKeyState = KEYDOWN;
        gCurrentKey = EINT2_KEY;
    }
    else if ((IOPBaseP->rGPFDAT & (1<<2)) == (1<<2)) {
        gKeyState = KEYUP;
    }

    if( gKeyState == KEYUP )
    {
        KeyEventInsert(gCurrentKey, gKeyState);
        gKeydownTime = 0;
        gCurrentKey = NOT_KEY;
    }
    else
    {
        if( gKeyState == KEYDOWN && gKeydownTime == 0 )
            KeyEventInsert(gCurrentKey, gKeyState);
        /*
        else
            CONSOL_Printf("\nOn Yessssssssssssssssssssssssssssssssss !!!!\n");
        */
    }

    ClearPending(BIT_EINT2);
}

static void KeyInterruptHandlerEINT8_23()
{
    HwrS3C2410IOPRegPtr IOPBaseP = (HwrS3C2410IOPRegPtr)HWRS3C2410_IOP_BASE;


    if(IOPBaseP->rEINTPEND==(1<<11))
    {
        IOPBaseP->rEINTPEND=(1<<11);

        if ((IOPBaseP->rGPGDAT & (1<<3)) == 0) {
            gKeyState = KEYDOWN;
            gCurrentKey = EINT11_KEY;
        }
        else if ((IOPBaseP->rGPGDAT & (1<<3)) == (1<<3)) {
            gKeyState = KEYUP;
        }
    }
    else if(IOPBaseP->rEINTPEND==(1<<19))
    {
        IOPBaseP->rEINTPEND=(1<<19);

        if ((IOPBaseP->rGPGDAT & (1<<11)) == 0) {
            gKeyState = KEYDOWN;
            gCurrentKey = EINT19_KEY;
        }
        else if ((IOPBaseP->rGPGDAT & (1<<11)) == (1<<11)) {
            gKeyState = KEYUP;
        }
    }
    else if(IOPBaseP->rEINTPEND == (1<<10))
    {
        IOPBaseP->rEINTPEND=(1<<10);

        if ((IOPBaseP->rGPGDAT & (1<<2)) == 0) {
            gKeyState = KEYDOWN;
            gCurrentKey = EINT10_KEY;
        }
        else if ((IOPBaseP->rGPGDAT & (1<<2)) == (1<<2)) {
            gKeyState = KEYUP;
        }
    }
    else if(IOPBaseP->rEINTPEND == (1<<13))
    {
        IOPBaseP->rEINTPEND=(1<<13);

        if ((IOPBaseP->rGPGDAT & (1<<5)) == 0) {

            gKeyState = KEYDOWN;
            gCurrentKey = EINT13_KEY;
        }
        else if ((IOPBaseP->rGPGDAT & (1<<5)) == (1<<5)) {

            gKeyState = KEYUP;
        }
    }
    else if(IOPBaseP->rEINTPEND == (1<<14))
    {
        IOPBaseP->rEINTPEND=(1<<14);

        if ((IOPBaseP->rGPGDAT & (1<<6)) == 0) {

            gKeyState = KEYDOWN;
            gCurrentKey = EINT14_KEY;
        }
        else if ((IOPBaseP->rGPGDAT & (1<<6)) == (1<<6)) {

            gKeyState = KEYUP;
        }
    }
    else if(IOPBaseP->rEINTPEND == (1<<15))
    {
        IOPBaseP->rEINTPEND=(1<<15);

        if ((IOPBaseP->rGPGDAT & (1<<7)) == 0) {
            CONSOL_Printf("\nkey down eint15 !!!!\n");
            gKeyState = KEYDOWN;
            gCurrentKey = EINT15_KEY;
        }
        else if ((IOPBaseP->rGPGDAT & (1<<7)) == (1<<7)) {
            CONSOL_Printf("\nkey up eint15 !!!!\n");
            gKeyState = KEYUP;
        }
    }

    if( gKeyState == KEYUP )
    {
        KeyEventInsert(gCurrentKey, gKeyState);
        gKeydownTime = 0;
        gCurrentKey = NOT_KEY;
    }
    else
    {
        if( gKeyState == KEYDOWN && gKeydownTime == 0 )
        {
            KeyEventInsert(gCurrentKey, gKeyState);
        }
        /*
        else
            CONSOL_Printf("\nOn Yessssssssssssssssssssssssssssssssss !!!!\n");
        */
    }

    ClearPending(BIT_EINT8_23);
}

int PrvCurCheckKey()
{
    HwrS3C2410IOPRegPtr IOPBaseP = (HwrS3C2410IOPRegPtr)HWRS3C2410_IOP_BASE;

    DisableKeyInterrupt();

    switch( gCurrentKey )
    {
    case EINT0_KEY:
        if ((IOPBaseP->rGPFDAT & (1<<0)) == 0) // EINT0
            return 1;
        break;
    case EINT2_KEY:
        if ((IOPBaseP->rGPFDAT & (1<<2)) == 0) // EINT2
            return 1;
        break;
    case EINT11_KEY:
        if ((IOPBaseP->rGPGDAT & (1<<3)) == 0) // EINT11
            return 1;
        break;
    case EINT19_KEY:
        if ((IOPBaseP->rGPGDAT & (1<<11)) == 0)	// EINT19
            return 1;
        break;
    case EINT10_KEY:
        if ((IOPBaseP->rGPGDAT & (1<<2)) == 0)	// EINT10
            return 1;
        break;
    case EINT13_KEY:
        if ((IOPBaseP->rGPGDAT & (1<<5)) == 0)	// EINT13
            return 1;
        break;
    case EINT14_KEY:
        if ((IOPBaseP->rGPGDAT & (1<<6)) == 0)	// EINT14
            return 1;
        break;
    case EINT15_KEY:
        if ((IOPBaseP->rGPGDAT & (1<<7)) == 0)	// EINT15
            return 1;
        break;
    case UPKEY:
        if ((IOPBaseP->rGPBDAT & (1<<5)) == 0) // UP key
            return 1;
        break;
    case DOWNKEY:
        if ((IOPBaseP->rGPBDAT & (1<<6)) == 0) // DOWN key
            return 1;
        break;
    case LEFTKEY:
        if ((IOPBaseP->rGPEDAT & (1<<11)) == 0) // LEFT key
            return 1;
        break;
    case RIGHTKEY:
        if ((IOPBaseP->rGPEDAT & (1<<12)) == 0) // RIGHT key
            return 1;
        break;
    }

    gKeyState = KEYUP;
    gCurrentKey = NOT_KEY;
    gKeydownTime = 0;

    EnableKeyInterrupt();

    return 0;
}

void longKeyHandler()
{
    if( gKeydownTime == 50 )
    {
        // check current key
        if( PrvCurCheckKey() )
        {
            // long key..
            gKeyState = KEYLONG;
            KeyEventInsert(gCurrentKey, gKeyState);
        }
    }

    if( gKeydownTime == 100 )
    {
        // check current key
        if( PrvCurCheckKey() )
        {
            // repeat key..
            gKeyState = KEYREPEAT;
            KeyEventInsert(gCurrentKey, gKeyState);
            gKeydownTime = 51;
        }
    }
}


void PrvCheckUpKey()
{
    HwrS3C2410IOPRegPtr IOPBaseP = (HwrS3C2410IOPRegPtr)HWRS3C2410_IOP_BASE;

    // check up key
    if ((IOPBaseP->rGPBDAT & (1<<5)) == 0) // UP key
    {
        // UP key down
        gKeyState = KEYDOWN;
        gCurrentKey = UPKEY;

        // 한번만 key down을 넣기 위함.
        if( gKeydownTime == 0 )
        {
            KeyEventInsert(gCurrentKey, gKeyState);
        }
    }
    else if( gKeyState == KEYDOWN &&
             gCurrentKey == UPKEY &&
             (IOPBaseP->rGPBDAT & (1<<5)) == (1<<5) )
    {
        // UP key up
        gKeyState = KEYUP;
        KeyEventInsert(gCurrentKey, gKeyState);
        gKeydownTime = 0;
        gCurrentKey = NOT_KEY;
    }
}

void PrvCheckDownKey()
{
    HwrS3C2410IOPRegPtr IOPBaseP = (HwrS3C2410IOPRegPtr)HWRS3C2410_IOP_BASE;

    // check DOWN key
    if ((IOPBaseP->rGPBDAT & (1<<6)) == 0) // DOWN key
    {
        // DOWN key down
        gKeyState = KEYDOWN;
        gCurrentKey = DOWNKEY;

        // 한번만 key down을 넣기 위함.
        if( gKeydownTime == 0 )
        {
            KeyEventInsert(gCurrentKey, gKeyState);
        }
        //ClearPending(BIT_EINT8_23);
    }
    else if( gKeyState == KEYDOWN &&
             gCurrentKey == DOWNKEY &&
             (IOPBaseP->rGPBDAT & (1<<6)) == (1<<6) )
    {
        // DOWN key up
        gKeyState = KEYUP;
        KeyEventInsert(gCurrentKey, gKeyState);
        gKeydownTime = 0;
        gCurrentKey = NOT_KEY;
        //ClearPending(BIT_EINT8_23);
    }

}

void PrvCheckLeftKey()
{
    HwrS3C2410IOPRegPtr IOPBaseP = (HwrS3C2410IOPRegPtr)HWRS3C2410_IOP_BASE;

    // check LEFT key
    if ((IOPBaseP->rGPEDAT & (1<<11)) == 0) // LEFT key
    {
        // LEFT key down
        gKeyState = KEYDOWN;
        gCurrentKey = LEFTKEY;

        // 한번만 key down을 넣기 위함.
        if( gKeydownTime == 0 )
        {
            KeyEventInsert(gCurrentKey, gKeyState);
        }
        //ClearPending(BIT_EINT8_23);
    }
    else if( gKeyState == KEYDOWN &&
             gCurrentKey == LEFTKEY &&
             (IOPBaseP->rGPEDAT & (1<<11)) == (1<<11) )
    {
        // LEFT key up
        gKeyState = KEYUP;
        KeyEventInsert(gCurrentKey, gKeyState);
        gKeydownTime = 0;
        gCurrentKey = NOT_KEY;
        //ClearPending(BIT_EINT8_23);
    }

}

void PrvCheckRightKey()
{
    HwrS3C2410IOPRegPtr IOPBaseP = (HwrS3C2410IOPRegPtr)HWRS3C2410_IOP_BASE;

    // check RIGHT key
    if ((IOPBaseP->rGPEDAT & (1<<12)) == 0) // RIGHT key
    {
        // RIGHT key down
        gKeyState = KEYDOWN;
        gCurrentKey = RIGHTKEY;

        // 한번만 key down을 넣기 위함.
        if( gKeydownTime == 0 )
        {
            KeyEventInsert(gCurrentKey, gKeyState);
        }
        //ClearPending(BIT_EINT8_23);
    }
    else if( gKeyState == KEYDOWN &&
             gCurrentKey == RIGHTKEY &&
             (IOPBaseP->rGPEDAT & (1<<12)) == (1<<12) )
    {
        // RIGHT key up
        gKeyState = KEYUP;
        KeyEventInsert(gCurrentKey, gKeyState);
        gKeydownTime = 0;
        gCurrentKey = NOT_KEY;
        //ClearPending(BIT_EINT8_23);
    }

}

void PrvCheckButton()
{
    DisableKeyInterrupt();

    // check up key
    PrvCheckUpKey();

    // check down key
    PrvCheckDownKey();

    // check left key
    PrvCheckLeftKey();

    // check right key
    PrvCheckRightKey();

    EnableKeyInterrupt();
}

