/******************************************************************************
 *
 * Copyright (c) 2005 Nemustech Mobile Dept.
 *
 * File: HwrRTCTimer.h
 *
 * Description:
 *      RTC Timer interrupts for S3C2410
 *
 * History:
 *      05/24/2005 Created By tae-hwan jang
 *
 *****************************************************************************/

#ifndef __HWRRTCTIMER_H__
#define __HWRRTCTIMER_H__

#include "includes.h"

// Prototypes
int HwrRTCInit(void);
INT32U GetRTCTime(void);
int SetRTCTime(INT32U newTime);

// by micro
#define SEC_1900_1904	126144000UL		//((1904-1900)*365*24*60*60
#define SEC_1904_2K		3029529600UL	//((2000-1904)*365+24)*24*60*60
#define YEAR_SEC		31536000UL		// 365*24*60*60
#define LEAP_YEAR_SEC	31622400UL		// 366*24*60*60
#define FROM_BCD(n)		((((n) >> 4) * 10) + ((n) & 0xf))
#define TO_BCD(n)		((((n) / 10) << 4) | ((n) % 10))

#endif // !__HWRTIMER_H__
