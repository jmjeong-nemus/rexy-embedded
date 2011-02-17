/******************************************************************************
 *
 * Copyright (c) 2005 Nemustech Mobile Dept.
 *
 * File: HwrRTCTimer.c
 *
 * Description:
 *      RTC Timer interrupts for S3C2410
 *
 * History:
 *      05/24/2005 Created By tae-hwan jang(jangth@mococo.com)
 *
 *****************************************************************************/

#include "includes.h"
#include "frmwrk.h"
#include "HwrRTCTimer.h"



static char	IS_Y2K = 0;
short t_mdays[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
unsigned long nTask = 0;

typedef struct DATE_ {
    INT16S	nYear;
    INT16S	nMon;
    INT16S  nDay;
    INT16S	nHour;
    INT16S	nMin;
    INT16S	nSec;
} DATE;

// by micro
INT32U 			PrvDateToSeconds(DATE *date);
int 			PrvSecondsToDate(double newTime, DATE *date);
void EnableRTCInterrupt(void);

/*********************************************************************************
 *  FUNCTION: HwrRTCInit
 *
 *  DESCRIPTION: Initializes the Time Manager.
 *
 *		Allocate and initialize the Time Manager globals;
 *		Unmask the RTC interrupt;  Initialize system timer;
 *
 *  PARAMETERS: void
 *
 *  RETURNS: 0 if successful; otherwise: timErrMemory
 *
 *  CREATED: 4/21/95
 *
 *  BY: Vitaly Kruglikov
 *
 *  Porting uC/OS-II
 *
 *  BY: tae-hwan jang(jangth@mococo.com)
 *
 *********************************************************************************/
int HwrRTCInit(void)
{
    int					err = 0;
    HwrS3C2410RTCRegPtr	RTC_baseP = (HwrS3C2410RTCRegPtr)HWRS3C2410_RTC_BASE;

    IS_Y2K = 1;

    // Unmask the RTC interrupt and allow it to wakeup the CPU.
    EnableRTCInterrupt();

    return err;
}



/************************************************************
 *  API
 *  Samsung S3C2410(ARM) hardware-dependent
 *-------------------------------------------------------------------------------
 *
 *  FUNCTION: GetRTCTime
 *
 *  DESCRIPTION: Returns seconds since 1/1/04
 *
 *  PARAMETERS: void
 *
 *  RETURNS: INT32U seconds
 *
 *  CREATED: 3/15/95
 *  
 *  BY: Ron Marianetti
 *
 *  REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			ron	3/15/95	Initial Revision
 *			ADH	6/28/99	Return a dummy value if called before TimInit creates
 *								the globals.
 *
 *  PORTING: 2002/08/12
 *
 *  BY: Yang Sung-hwa(micro@mococo.com)
 *
 *  uC/OS Porting : 2005/5/25
 *
 *  BY: tae-hwan jang(jangth@mococo.com)
 *
 *************************************************************/
INT32U GetRTCTime()
{
    HwrS3C2410RTCRegPtr	RTC_baseP = (HwrS3C2410RTCRegPtr)HWRS3C2410_RTC_BASE;
    INT32U				seconds;
    DATE				date;

    // Get the time according to the Samsung S3C2410 RTC
    date.nSec  = FROM_BCD(RTC_baseP->rBCDSEC);
    date.nMin  = FROM_BCD(RTC_baseP->rBCDMIN);
    date.nHour = FROM_BCD(RTC_baseP->rBCDHOUR);
    date.nDay  = FROM_BCD(RTC_baseP->rBCDDAY);
    date.nMon  = FROM_BCD(RTC_baseP->rBCDMON);
    date.nYear = FROM_BCD(RTC_baseP->rBCDYEAR);

    seconds = PrvDateToSeconds(&date);
    /*
    	// RTC Interrupts off while we do this
    	oldState = HALInterruptSetState(INT_RTC, IRQDisabled);

    	// Restore the RTC interrupt (if it was enabled)
    	if (oldState == IRQEnabled) {
    		HALInterruptSetState(INT_RTC, IRQEnabled); 
    	}
    */
    return seconds;
}

/************************************************************
 *  API
 *  Samsung S3C2410(ARM) hardware-dependent
 *-------------------------------------------------------------------------------
 *
 *  FUNCTION: HALTimeSetSeconds
 *
 *  DESCRIPTION: Sets seconds since 1/1/04
 *
 *			We are going to resynchronize the current alarm setting with
 *			the new RTC seconds.
 *
 *  PARAMETERS: UInt32 newTime
 *
 *  RETURNS: 0 (no error)
 *
 *  CREATED: 3/15/95
 *
 *  BY: Ron Marianetti
 *
 *  REVISION HISTORY:
 *		4/24/95		vmk		Added code to resynchronize the alarm.
 *		5/1/95		vmk		Modified to cancel the current alarm without
 *									restoring it.  Removed masking/unmasking of
 *									RTC interrupt.
 *		11/1/95		vmk		Removed code which cancelled alarms and added
 *									code to resynchronize the Alarm Manager
 *  PORTING: 2002/08/12
 *
 *  BY: Yang Sung-hwa(micro@mococo.com)
 *
 *  uC/OS Porting : 2005/5/25
 *
 *  BY: tae-hwan jang(jangth@mococo.com)
 *
 *************************************************************/
int SetRTCTime(INT32U newTime)
{
    HwrS3C2410RTCRegPtr	RTC_baseP = (HwrS3C2410RTCRegPtr)HWRS3C2410_RTC_BASE;
    INT32S				delta;
    DATE				date;

    // Before we set the time, remember the amount that we're changing it by
    //	delta = (INT32S)(newTime - HALTimeGetSeconds());
    /*
    	// RTC Interrupts off while we do this
    	oldState = HALInterruptSetState(INT_RTC, IRQDisabled);
    */	
    PrvSecondsToDate((double)newTime, &date);

    /* RTC Control Enable & Reset	*/
    RTC_baseP->rRTCCON |= (1<<0);

    RTC_baseP->rBCDYEAR = (unsigned char)TO_BCD(date.nYear % 100);
    RTC_baseP->rBCDMON  = (unsigned char)TO_BCD(date.nMon);
    RTC_baseP->rBCDDAY  = (unsigned char)TO_BCD(date.nDay);
    RTC_baseP->rBCDHOUR = (unsigned char)TO_BCD(date.nHour);
    RTC_baseP->rBCDMIN  = (unsigned char)TO_BCD(date.nMin);
    RTC_baseP->rBCDSEC  = (unsigned char)TO_BCD(date.nSec);

    /* RTC Control Disable			*/
    RTC_baseP->rRTCCON  &= ~((1<<3)|(1<<0));

    return 0;
}

/*********************************************************************************
 *  PRIVATE,
 *-----------------------------------------------------------
 *
 *  FUNCTION: PrvDateToSeconds
 *
 *  DESCRIPTION: translate date to seconds from 1904/1/1
 *
 *  NOTE: 
 *
 *  PARAMETERS: none
 *
 *  RETURNS: seconds from 1904/01/01 00:00:00
 *
 *  CREATED: 2002/08/13
 *
 *  BY: Yang Sung-hwa(micro@mococo.com)
 *
 *********************************************************************************/

INT32U PrvDateToSeconds(DATE *date)
{
    INT32U				seconds;
    INT16U				days, cnt_leap_year, i;

    if (date->nYear==0) IS_Y2K=1;	// by chc

    cnt_leap_year = (INT16U) date->nYear/4 + ((IS_Y2K)? 1 : 0) - ((date->nYear%4 == 0)? 1 : 0);

    days = (date->nYear*365)+cnt_leap_year;
    for (i=1; i<date->nMon; i++) {
        days += t_mdays[i];
        days += ( (i == 2) && (date->nYear%4 == 0) ) ? 1 : 0;
    }
    days += date->nDay-1;

    seconds = ((days*24 + date->nHour)*60 + date->nMin)*60 + date->nSec;
    seconds += (IS_Y2K)? (INT32U)SEC_1904_2K : -(INT32U)SEC_1900_1904;

    return seconds;
}


/*********************************************************************************
 *  PRIVATE,
 *-----------------------------------------------------------
 *
 *  FUNCTION: PrvSecondsToDate
 *
 *  DESCRIPTION: translate seconds to date
 *
 *  NOTE: 
 *
 *  PARAMETERS: newTime - seconds from 1904/01/01 00:00:00
 *              date	- DATE sturct
 *
 *  RETURNS: error value
 *
 *  CREATED: 2002/08/13
 *
 *  BY: Yang Sung-hwa(micro@mococo.com)
 *
 *********************************************************************************/
int PrvSecondsToDate(double newTime, DATE *date)
{
    double				tmpSec;

    if (newTime >= (double)SEC_1904_2K) {
        newTime -= (double)SEC_1904_2K;
        IS_Y2K = 1;
    } else
        IS_Y2K = 0;

    // year
    for (date->nYear = IS_Y2K ? 2000 : 1904, tmpSec = (double)LEAP_YEAR_SEC; newTime >= tmpSec; ) {
        newTime -= tmpSec;
        date->nYear++;

        tmpSec = (double)(date->nYear%4 == 0) && (date->nYear%100 != 0) || (date->nYear%400 == 0) ? LEAP_YEAR_SEC : YEAR_SEC;
    }

    // month
    for (date->nMon = 1, tmpSec = (double)t_mdays[date->nMon]*86400; newTime >= tmpSec; ) {
        newTime -= tmpSec;
        date->nMon++;
        if (date->nMon==2 && ((date->nYear%4 == 0) && (date->nYear%100 != 0) || (date->nYear%400 == 0)))
            tmpSec = (double)2505600; //29*24*60*60
        else
            tmpSec = (double)t_mdays[date->nMon]*86400; /*24*60*60*/
    }

    // day
    for (date->nDay = 1; newTime >= (double)86400; date->nDay++, newTime -= (double)86400);

    // hour
    for (date->nHour = 0; newTime >= (double)3600; date->nHour++, newTime -= (double)3600);

    // minute
    for (date->nMin = 0; newTime >= (double)60; date->nMin++, newTime -= (double)60);

    date->nSec = newTime; // by chc

    return 0;
}


void EnableRTCInterrupt()
{
    HwrS3C2410INTRegPtr INTBaseP = (HwrS3C2410INTRegPtr)HWRS3C2410_INT_BASE;

    // TIMER4 Interrupt Unmasking
    INTBaseP->rINTMSK &= (0xBFFFFFFF);
}

