/******************************************************************************
 *
 * Copyright (c) 2005 Nemustech Mobile Dept.
 *
 * File: HwrKeyMgr.h
 *
 * Description:
 *      Key management for S3C2410
 *
 * History:
 *      05/23/2005 Created By tae-hwan jang(jangth@mococo.com)
 *
 *****************************************************************************/

#ifndef __HWRKEYMGR_H__
#define __HWRKEYMGR_H__

#include "includes.h"

#define KEYDOWN		0
#define KEYUP		1
#define KEYLONG		2
#define KEYREPEAT	3
/*
#define NOT_KEY		0
#define EINT0_KEY	1
#define EINT2_KEY	2
#define EINT11_KEY	3	
#define EINT19_KEY	4
#define EINT10_KEY	5
#define EINT13_KEY	6
#define EINT14_KEY	7
#define EINT15_KEY	8
#define UPKEY		9
#define DOWNKEY		10
#define LEFTKEY		11
#define RIGHTKEY	12
*/
enum keyCode
{
    NOT_KEY	= -1,
    EINT0_KEY = 0,
    EINT2_KEY,
    EINT11_KEY,
    EINT19_KEY,
    EINT10_KEY,
    EINT13_KEY,
    EINT14_KEY,
    EINT15_KEY,
    UPKEY,
    DOWNKEY,
    LEFTKEY,
    RIGHTKEY,
    KEYCOUNT
};

// Prototypes
void KeyInit(void);
void longKeyHandler(void);
static void KeyInterruptHandlerEINT0(void);

static void KeyInterruptHandlerEINT2(void);

static void KeyInterruptHandlerEINT8_23(void);

void EnableKeyInterrupt(void);

extern int gKeyState;
extern long gKeydownTime;
extern int gBtnCheck;

#endif // __HWRKEYMGR_H__
