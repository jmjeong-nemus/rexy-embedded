/* 
* $Id$
* Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.    
*   
* This file is a part of biorhythm application
*   
*/

#include "rexy.h"
#include "rexy_haldevice.h"

#include "../header/biorhythm.h"
#include "rexy_ctl_popup.h"
#include "application.h"

#ifdef __REGISTRY_H__
#include "registry.h"
#endif

//string
#define BIORHYTHM_STRING	"* BIORHYTHM *"
#define BIRTHDAY_STRING		"< BITRHDAY >"
#define DIVISION_STRING		"/"
#define PLUS_STRING			"+"
#define MINUS_STRING		"-"
#define ERROR_STRING		"Invalid Date"
#define INTELLECTUAL_STRING	"Intellectual"
#define PHYSICAL_STRING		"Physical"
#define EMOTIONAL_STRING	"Emotional"
#define NUM_0				"0"
#define NUM_1				"1"

#define IS_LEAP_YEAR(y) ((y)%4==0 && ((y)%100!=0 || (y)%400==0))

//static function
static rint32 PrvBioGetDaysBetweenYears(rint32 y1, rint32 y2);
static rint32 PrvBioGetDaysBetweenMonths(rint32 y, rint32 m1, rint32 m2);
static rint32 PrvBioGetDaysInMonth(rint32 y, rint32 m);
static rint32 PrvBioDiffDay(rint32 y1, rint32 m1, rint32 d1, rint32 y2, rint32 m2, rint32 d2);
static rint32 PrvBioPopMsgErrorCB(WND wnd, rint32 nMsg, rint32 wparam, rint32 lparam, rint32 bIsPre, rint32 *pErr);
static double PrvBioSinFunc(double x);
static void PrvBioMsgError(WND hWnd);
static void PrvBioCloseMsgErrPop();
static bool PrvBioCheckError();
static void PrvBioPrintDateInForm(WND wnd);
static void PrvBioFormHandler(WND hWnd, rint32 msg, rint32 wParam, rint32 lParam);
static void PrvBioDrawBiorhythmForm(WND wnd);
static void PrvBioInitBiorhythm();

static rint32 PrvBirthdayCreateCaret(WND wnd, char key);
static void PrvBirthdayInputHandler(WND hWnd, rint32 msg, rint32 wParam, rint32 lParam);
static void PrvBirthdayScrollDate(WND wnd);
static void PrvBirthdayFormCreate(WND wnd);

static void PrvGraphCalculateBiorhythm();
static void PrvGraphDrawMonthGraph(WND wnd);
static void PrvGraphDrawDayGraph(WND wnd);
static void PrvGraphMoveMonthGraph(char key);
static void PrvGraphDrawHandler(WND hWnd, rint32 msg, rint32 wParam, rint32 lParam);

static BiorhythmFormHandle g_BioFormHandleList[] =
{
	BIO_BIRTHDAYINPUT_FORM,		PrvBirthdayInputHandler,
	BIO_GRAPHDRAW_FORM,			PrvGraphDrawHandler
};

static WND gpCtlErr = NULL;

struct tm current_time;

//the number of days to each month
static const ruint16 PrvBioMonthLens[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
//the number of accumulated days to each month
static const ruint16 PrvBioAccuMonthLens[]={0,31,59,90,120,151,181,212,243,273,304,334,365};

static BiorhythmGlobalsPtr PrvBioGlobals;

//Get the number of days between y1 and y2 in a year
static rint32 PrvBioGetDaysBetweenYears(rint32 y1, rint32 y2)
{
	rint32 count;
	if (y2 < y1)
		return 0;
	count = (y2 - y1 + 1) * 365;
	y1--;
	count += ((y2 / 4 - y2 / 100 + y2 / 400) - (y1 / 4 - y1 / 100 + y1 / 400));
	return count;
}

//Get the number of days between m1 and m2 days in y years
static rint32 PrvBioGetDaysBetweenMonths(rint32 y, rint32 m1, rint32 m2)
{
	rint32 count;
	if (m2 < m1)
		return 0;
	count = PrvBioAccuMonthLens[m2] - PrvBioAccuMonthLens[m1 - 1];
	if ((m1 < 2 && m2 < 2) || ( m1 > 2 && m2 > 2) || !IS_LEAP_YEAR(y))
		return count;
	return (count+1);
}

//Get the number of days in y years 
static rint32 PrvBioGetDaysInMonth(rint32 y, rint32 m)
{
	if (m != 2 || !IS_LEAP_YEAR(y))
		return PrvBioMonthLens[m];
	return 29;
}

//Get the number of days between y1-m1-d1 and y2-m2-d2
static rint32 PrvBioDiffDay(rint32 y1, rint32 m1, rint32 d1, rint32 y2, rint32 m2, rint32 d2)
{
	rint32 count=0;
	if (y1 != y2)
	{
		count = PrvBioGetDaysBetweenYears(y1 + 1, y2 - 1);
		count += PrvBioGetDaysBetweenMonths(y1, m1 + 1, 12);
		count += PrvBioGetDaysBetweenMonths(y2, 1, m2 - 1);
		count += PrvBioGetDaysInMonth(y1, m1) - d1;
		count += d2;
	}
	else
	{
		if (m1 != m2)
		{
			count = PrvBioGetDaysBetweenMonths(y1, m1 + 1, m2 - 1);
			count += PrvBioGetDaysInMonth(y1, m1) - d1;
			count += d2;
		}
		else
		{
			count = d2 - d1;
		}
	}
	return count;
}

/**
 * @brief		Define sine function of math
 * param[in]	x	sin(x)
 * return	sum		result of sin(x)
 * @remarks 
 * @if HISTORY
 *			4-Apr-2006	Kwanho Shin created
 * @endif
 */
static double PrvBioSinFunc(double x)
{
	rint32 i;
	double sum = x;
    double result = x;
    double fact = 1.0f;
    double x2 = x * x * - 1.0f;

	//finite tayler series 
    for (i = 1; i < 9; ++i)
    {
        fact *= ((2 * i) * (2 * i + 1));
        result *= x2;
        sum += (result / fact);
    }

    return sum;
}

/**
 * @brief		Calculate biorhythm in selected month
 * @remarks 
 * @if HISTORY
 *			4-Apr-2006	Kwanho Shin created
 * @endif
 */
static void PrvGraphCalculateBiorhythm()
{
	rint32 i;
	rint32 nDayInMonth;
	rint32 nDiff;

	nDayInMonth = PrvBioGetDaysInMonth(PrvBioGlobals->nSelectYear, PrvBioGlobals->nSelectMonth);
	nDiff = PrvBioDiffDay(PrvBioGlobals->nInYear, PrvBioGlobals->nInMonth, PrvBioGlobals->nInDay,
		PrvBioGlobals->nSelectYear, PrvBioGlobals->nSelectMonth, 1);
	
	for (i = 0; i < nDayInMonth; i++)
	{
		(PrvBioGlobals->Biorhythm[i]).nIntellectual = (rint32)(PrvBioSinFunc((nDiff+i) % 33 * 2 * 3.14 / 33) * 100);
		(PrvBioGlobals->Biorhythm[i]).nPhysical = (rint32)(PrvBioSinFunc((nDiff+i) % 23 * 2 * 3.14 / 23) * 100);
		(PrvBioGlobals->Biorhythm[i]).nEmotional = (rint32)(PrvBioSinFunc((nDiff+i) % 28 * 2 * 3.14/ 28) * 100);	
	}
}

/**
 * @brief		Print a selected date
 * param[in]	wnd		window handle
 * @remarks 
 * @if HISTORY
 *			4-Apr-2006	Kwanho Shin created
 * @endif
 */
static void PrvBioPrintDateInForm(WND wnd)
{
	char year[5];
	char month[3];
	char day[3];

	StrIToA(PrvBioGlobals->nSelectYear, year);
	StrIToA(PrvBioGlobals->nSelectMonth, month);
	StrIToA(PrvBioGlobals->nSelectDay, day);

	if (StrLen(month) == 1)
	{
		month[1] = month[0];
		month[0] = '0';
		month[2] = '\0';
	}

	if (StrLen(day) == 1)
	{
		day[1] = day[0];
		day[0] = '0';
		day[2] = '\0';
	}

	GdiTextOut(wnd, year, StrLen(year), (int)(PrvBioGlobals->nScreenWidth * 30 / 100), (int)(PrvBioGlobals->nScreenHeight * 2 / 100));
	GdiTextOut(wnd, month, StrLen(month), (int)(PrvBioGlobals->nScreenWidth * 52 / 100), (int)(PrvBioGlobals->nScreenHeight * 2 / 100));
	GdiTextOut(wnd, day, StrLen(day), (int)(PrvBioGlobals->nScreenWidth * 66 / 100), (int)(PrvBioGlobals->nScreenHeight * 2 / 100));
	GdiTextOut(wnd, DIVISION_STRING, StrLen(DIVISION_STRING), (int)(PrvBioGlobals->nScreenWidth * 46 / 100), (int)(PrvBioGlobals->nScreenHeight * 2 / 100));
	GdiTextOut(wnd, DIVISION_STRING, StrLen(DIVISION_STRING), (int)(PrvBioGlobals->nScreenWidth * 60 / 100), (int)(PrvBioGlobals->nScreenHeight * 2 / 100));
}

/**
 * @brief		Scroll date
 * param[in]	wnd		window handle
 * @remarks 
 * @if HISTORY
 *			7-Apr-2006	Kwanho Shin created
 * @endif
 */
static void PrvBirthdayScrollDate(WND wnd)
{
	char year[5];
	char month[3];
	char day[3];

	StrIToA(PrvBioGlobals->nInYear, year);
	StrIToA(PrvBioGlobals->nInMonth, month);
	StrIToA(PrvBioGlobals->nInDay, day);

	if (StrLen(month) == 1)
	{
		month[1] = month[0];
		month[0] = '0';
		month[2] = '\0';
	}

	if (StrLen(day) == 1)
	{
		day[1] = day[0];
		day[0] = '0';
		day[2] = '\0';
	}

	GdiTextOut(wnd, year, StrLen(year), (int)(PrvBioGlobals->nScreenWidth * 28 / 100), (int)(PrvBioGlobals->nScreenHeight * 70 / 100));
	GdiTextOut(wnd, month, StrLen(month), (int)(PrvBioGlobals->nScreenWidth * 52 / 100), (int)(PrvBioGlobals->nScreenHeight * 70 / 100));
	GdiTextOut(wnd, day, StrLen(day), (int)(PrvBioGlobals->nScreenWidth * 66 / 100), (int)(PrvBioGlobals->nScreenHeight * 70 / 100));
}

/**
 * @brief		Draw biorhythm of a month
 * param[in]	wnd		window handle
 * @remarks		This function draws a curve graph concerning biorhythm of the selected month
 * @if HISTORY
 *			4-Apr-2006	Kwanho Shin created
 * @endif
 */
static void PrvGraphDrawMonthGraph(WND wnd)
{
	char day[3];

	rint32 i;
	rint32 nStart;
	rint32 nDayInMonth;
	rint32 nCaret_x;
	double nGap_x;
	double nGap_y;

	PrvGraphCalculateBiorhythm();
	nDayInMonth = PrvBioGetDaysInMonth(PrvBioGlobals->nSelectYear, PrvBioGlobals->nSelectMonth);
	nGap_x = (double)PrvBioGlobals->nScreenWidth * 75 / 100 / 31;
	nGap_y = (double)PrvBioGlobals->nScreenHeight * 20 / 100 / 100;
	
	if ((PrvBioGlobals->nSelectYear == PrvBioGlobals->nInYear) && (PrvBioGlobals->nSelectMonth == PrvBioGlobals->nInMonth))
	{
		nStart = PrvBioGlobals->nInDay - 1;
	}
	else
	{
		nStart = 0;
	}

	//draw biorhythm
	for (i = nStart; i < nDayInMonth - 1; i++)
	{
		GdiDrawColorLine(wnd, (int)((PrvBioGlobals->nScreenWidth * 15 / 100) + (i * nGap_x)),
			(int)((PrvBioGlobals->nScreenHeight * 30 / 100) - (PrvBioGlobals->Biorhythm[i].nIntellectual * nGap_y)),
			(int)((PrvBioGlobals->nScreenWidth * 15 / 100) + ((i + 1) * nGap_x)),
			(int)((PrvBioGlobals->nScreenHeight * 30 / 100) - (PrvBioGlobals->Biorhythm[i + 1].nIntellectual * nGap_y)), COLOR_RED);
		GdiDrawColorLine(wnd, (int)((PrvBioGlobals->nScreenWidth * 15 / 100) + (i * nGap_x)),
			(int)((PrvBioGlobals->nScreenHeight * 30 / 100) - (PrvBioGlobals->Biorhythm[i].nPhysical * nGap_y)),
			(int)((PrvBioGlobals->nScreenWidth * 15 / 100) + ((i + 1) * nGap_x)),
			(int)((PrvBioGlobals->nScreenHeight * 30 / 100) - (PrvBioGlobals->Biorhythm[i + 1].nPhysical * nGap_y)), COLOR_GREEN);
		GdiDrawColorLine(wnd, (int)((PrvBioGlobals->nScreenWidth * 15 / 100) + (i * nGap_x)),
			(int)((PrvBioGlobals->nScreenHeight * 30 / 100) - (PrvBioGlobals->Biorhythm[i].nEmotional * nGap_y)),
			(int)((PrvBioGlobals->nScreenWidth * 15 / 100) + ((i + 1) * nGap_x)),
			(int)((PrvBioGlobals->nScreenHeight * 30 / 100) - (PrvBioGlobals->Biorhythm[i + 1].nEmotional * nGap_y)), COLOR_BLUE);
	}
	
	GdiSetFgColor(wnd, COLOR_RED);
	GdiTextOut(wnd, INTELLECTUAL_STRING, StrLen(INTELLECTUAL_STRING), (int)(PrvBioGlobals->nScreenWidth * 2 / 100), (int)(PrvBioGlobals->nScreenHeight * 55 / 100));
	GdiSetFgColor(wnd, COLOR_GREEN);
	GdiTextOut(wnd, PHYSICAL_STRING, StrLen(PHYSICAL_STRING), (int)(PrvBioGlobals->nScreenWidth * 39 / 100), (int)(PrvBioGlobals->nScreenHeight * 55 / 100));
	GdiSetFgColor(wnd, COLOR_BLUE);
	GdiTextOut(wnd, EMOTIONAL_STRING, StrLen(EMOTIONAL_STRING), (int)(PrvBioGlobals->nScreenWidth * 68 / 100), (int)(PrvBioGlobals->nScreenHeight * 55 / 100));

	GdiSetFgColor(wnd, COLOR_BLACK);
	GdiDrawLine(wnd, (int)(PrvBioGlobals->nScreenWidth * 15 / 100), (int)(PrvBioGlobals->nScreenHeight * 10 / 100),
		(int)(PrvBioGlobals->nScreenWidth * 15 / 100), (int)(PrvBioGlobals->nScreenHeight * 50 / 100));
	GdiDrawLine(wnd, (int)(PrvBioGlobals->nScreenWidth * 15 / 100), (int)(PrvBioGlobals->nScreenHeight * 30 / 100),
		(int)(PrvBioGlobals->nScreenWidth * 90 / 100), (int)(PrvBioGlobals->nScreenHeight * 30 / 100));
	GdiTextOut(wnd, PLUS_STRING, StrLen(PLUS_STRING), (int)(PrvBioGlobals->nScreenWidth * 10 / 100), (int)(PrvBioGlobals->nScreenHeight * 10 / 100));
	GdiTextOut(wnd, MINUS_STRING, StrLen(PLUS_STRING), (int)(PrvBioGlobals->nScreenWidth * 10 / 100), (int)(PrvBioGlobals->nScreenHeight * 47 / 100));
	GdiTextOut(wnd, NUM_0, StrLen(NUM_0), (int)(PrvBioGlobals->nScreenWidth * 10 / 100), (int)(PrvBioGlobals->nScreenHeight * 29 / 100));

	StrIToA(nDayInMonth, day);

	GdiTextOut(wnd, NUM_1, StrLen(NUM_1), (int)(PrvBioGlobals->nScreenWidth * 15 / 100), (int)(PrvBioGlobals->nScreenHeight * 51 / 100));
	GdiTextOut(wnd, day, StrLen(day), (int)(PrvBioGlobals->nScreenWidth * 85 / 100), (int)(PrvBioGlobals->nScreenHeight * 51 / 100));

	if (PrvBioGlobals->nSelectDay > nDayInMonth)
	{
		PrvBioGlobals->nSelectDay = nDayInMonth;
	}
	if ((PrvBioGlobals->nSelectYear == PrvBioGlobals->nInYear) && (PrvBioGlobals->nSelectMonth == PrvBioGlobals->nInMonth) && (PrvBioGlobals->nSelectDay < PrvBioGlobals->nInDay))
	{
		PrvBioGlobals->nSelectDay = PrvBioGlobals->nInDay;
	}

	PrvBioPrintDateInForm(wnd);

	nCaret_x = (PrvBioGlobals->nScreenWidth * 15 / 100) + (int)((PrvBioGlobals->nSelectDay - 1) * nGap_x);

	GdiDrawColorLine(wnd, (int)(nCaret_x), (int)(PrvBioGlobals->nScreenHeight * 10 / 100),	(int)(nCaret_x), (int)(PrvBioGlobals->nScreenHeight * 50 / 100), COLOR_YELLOW);
}

/**
 * @brief		Draw biorhythm of a day
 * param[in]	wnd		window handle
 * @remarks		This function draws a bar graph concerning biorhythm of the selected day
 * @if HISTORY
 *			4-Apr-2006	Kwanho Shin created
 * @endif
 */
static void PrvGraphDrawDayGraph(WND wnd)
{
	rint32 nDayHeight;
	char result[5];
	double nGap;
	rect rec;

	nDayHeight = PrvBioGlobals->nScreenHeight * 30 / 100;
	nGap = (double)PrvBioGlobals->nScreenWidth * 35 / 100 / 100;

	GdiSetFgColor(wnd, COLOR_BLACK);
	rec.top = PrvBioGlobals->nScreenHeight * 60 / 100;
	rec.bottom = PrvBioGlobals->nScreenHeight * 90 / 100;
	rec.left = PrvBioGlobals->nScreenWidth * 10 / 100;
	rec.right = PrvBioGlobals->nScreenWidth * 80 / 100;
	GdiRectangle(wnd, &rec);
	GdiDrawLine(wnd, (int)(PrvBioGlobals->nScreenWidth * 45 / 100), (int)(PrvBioGlobals->nScreenHeight * 60 / 100),
		(int)(PrvBioGlobals->nScreenWidth * 45 / 100), (int)(PrvBioGlobals->nScreenHeight * 90 / 100));
	GdiTextOut(wnd, PLUS_STRING, StrLen(PLUS_STRING), (int)(PrvBioGlobals->nScreenWidth * 76 / 100), (int)(PrvBioGlobals->nScreenHeight * 92 / 100));
	GdiTextOut(wnd, MINUS_STRING, StrLen(PLUS_STRING), (int)(PrvBioGlobals->nScreenWidth * 10 / 100), (int)(PrvBioGlobals->nScreenHeight * 92 / 100));
	GdiTextOut(wnd, NUM_0, StrLen(NUM_0), (int)(PrvBioGlobals->nScreenWidth * 44 / 100), (int)(PrvBioGlobals->nScreenHeight * 92 / 100));

	StrIToA(PrvBioGlobals->Biorhythm[PrvBioGlobals->nSelectDay - 1].nIntellectual, result);
	GdiTextOut(wnd, result, StrLen(result), (int)(PrvBioGlobals->nScreenWidth * 82 / 100), (int)((PrvBioGlobals->nScreenHeight * 60 / 100)+(nDayHeight * 10 / 100)));
	StrIToA(PrvBioGlobals->Biorhythm[PrvBioGlobals->nSelectDay - 1].nPhysical, result);
	GdiTextOut(wnd, result, StrLen(result), (int)(PrvBioGlobals->nScreenWidth * 82 / 100), (int)((PrvBioGlobals->nScreenHeight * 60 / 100)+(nDayHeight * 40 / 100)));
	StrIToA(PrvBioGlobals->Biorhythm[PrvBioGlobals->nSelectDay - 1].nEmotional, result);
	GdiTextOut(wnd, result, StrLen(result), (int)(PrvBioGlobals->nScreenWidth * 82 / 100), (int)((PrvBioGlobals->nScreenHeight * 60 / 100)+(nDayHeight * 70 / 100)));

	//draw biorhythm
	GdiSetBkColor(wnd, COLOR_RED);
	rec.top = (PrvBioGlobals->nScreenHeight * 60 / 100)+(nDayHeight * 10 / 100);
	rec.bottom = (PrvBioGlobals->nScreenHeight * 60 / 100)+(nDayHeight * 30 / 100);
	
	//intellectual
	if (PrvBioGlobals->Biorhythm[PrvBioGlobals->nSelectDay-1].nIntellectual >= 0)
	{
		rec.left = PrvBioGlobals->nScreenWidth * 45 / 100;
		rec.right = (PrvBioGlobals->nScreenWidth * 45 / 100) + (rint16)(PrvBioGlobals->Biorhythm[PrvBioGlobals->nSelectDay - 1].nIntellectual * nGap);
	}
	else
	{
		rec.left = (PrvBioGlobals->nScreenWidth * 45 / 100) + (rint16)(PrvBioGlobals->Biorhythm[PrvBioGlobals->nSelectDay - 1].nIntellectual * nGap);
		rec.right = PrvBioGlobals->nScreenWidth * 45 / 100;
	}
	GdiRectangle(wnd, &rec);

	//physical
	GdiSetBkColor(wnd, COLOR_GREEN);
	rec.top = (PrvBioGlobals->nScreenHeight * 60 / 100)+(nDayHeight * 40 / 100);
	rec.bottom = (PrvBioGlobals->nScreenHeight * 60 / 100)+(nDayHeight * 60 / 100);

	if (PrvBioGlobals->Biorhythm[PrvBioGlobals->nSelectDay - 1].nPhysical >= 0)
	{
		rec.left = PrvBioGlobals->nScreenWidth * 45 / 100;
		rec.right = (PrvBioGlobals->nScreenWidth * 45 / 100) + (rint16)(PrvBioGlobals->Biorhythm[PrvBioGlobals->nSelectDay - 1].nPhysical * nGap);
	}
	else
	{
		rec.left = (PrvBioGlobals->nScreenWidth * 45 / 100) + (rint16)(PrvBioGlobals->Biorhythm[PrvBioGlobals->nSelectDay - 1].nPhysical * nGap);
		rec.right = PrvBioGlobals->nScreenWidth * 45 / 100;
	}
	GdiRectangle(wnd, &rec);

	//emotional
	GdiSetBkColor(wnd, COLOR_BLUE);
	rec.top = (PrvBioGlobals->nScreenHeight * 60 / 100)+(nDayHeight * 70 / 100);
	rec.bottom = (PrvBioGlobals->nScreenHeight * 60 / 100)+(nDayHeight * 90 / 100);

	if (PrvBioGlobals->Biorhythm[PrvBioGlobals->nSelectDay - 1].nEmotional >= 0)
	{
		rec.left = PrvBioGlobals->nScreenWidth * 45 / 100;
		rec.right = (PrvBioGlobals->nScreenWidth * 45 / 100) + (rint16)(PrvBioGlobals->Biorhythm[PrvBioGlobals->nSelectDay - 1].nEmotional * nGap);
	}
	else
	{
		rec.left = (PrvBioGlobals->nScreenWidth * 45 / 100) + (rint16)(PrvBioGlobals->Biorhythm[PrvBioGlobals->nSelectDay - 1].nEmotional * nGap);
		rec.right = PrvBioGlobals->nScreenWidth * 45 / 100;
	}
	GdiRectangle(wnd, &rec);

	GdiSetBkColor(wnd, COLOR_WHITE);
}

/**
 * @brief		Draw biorhythm
 * param[in]	wnd		window handle
 * @remarks		This function draws a curve and a bar graph of biorhythm
 * @if HISTORY
 *			6-Apr-2006	Kwanho Shin created
 * @endif
 */
static void PrvBioDrawBiorhythmForm(WND wnd)
{
	PrvBioGlobals->nCurrFormID = BIO_GRAPHDRAW_FORM;
	GdiLockScreen(wnd);
	GdiClrScreen(wnd, COLOR_WHITE);
	PrvGraphDrawMonthGraph(wnd);
	PrvGraphDrawDayGraph(wnd);
	GdiUnlockScreen(wnd);
}

static rint32 PrvBioPopMsgErrorCB(WND wnd, rint32 nMsg, rint32 wparam, rint32 lparam, rint32 bIsPre, rint32 *pErr)
{
	switch(nMsg)
	{
	case AWM_PAINT:
		if (!bIsPre)
			GdiTextOut(wnd, ERROR_STRING, StrLen(ERROR_STRING), (int)(PrvBioGlobals->nScreenWidth * 35 / 100), (int)(PrvBioGlobals->nScreenHeight * 27 / 100));
		break;
	}
	return 0;
}

/**
 * @brief	Create popup control for the error message
 * @remarks 
 * @if HISTORY
 *			4-Apr-2006	Kwanho Shin created
 * @endif
 */
static void PrvBioMsgError(WND wnd)
{
	rint32 err;
    WND pCtl;
	rect rt;

	PrvBioGlobals->nFocus = 0;

	// Create popup control
    pCtl = CtlCreate(wnd, CTL_TYPE_POPUP, NULL, PrvBioPopMsgErrorCB, &err);

	// Set popup property, Draw popup
	gpCtlErr = pCtl;

	GdiSetRect(&rt, PrvBioGlobals->nScreenWidth * 10 / 100, PrvBioGlobals->nScreenHeight * 10 / 100,
			   PrvBioGlobals->nScreenWidth * 90 / 100, PrvBioGlobals->nScreenHeight * 50 / 100);

	CtlSetPtr(pCtl, CTL_POPUP_CMD_SET_RECT, (void*)&rt, &err);

	//CtlSeti(gpCtlErr, POPUP_COM_TIMEOUT, 1500, &err);
	CtlDraw(gpCtlErr, &err);
}

/**
 * @brief	Destroy popup control for the error message
 * @remarks 
 * @if HISTORY
 *			4-Apr-2006	Kwanho Shin created
 * @endif
 */
static void PrvBioCloseMsgErrPop()
{
	rint32 err;

	if (gpCtlErr != NULL)
	{
		CtlDestroy(gpCtlErr, &err);
		gpCtlErr = NULL;
	}
	PrvBioGlobals->nFocus = 1;
}

/**
 * @brief	Check error for inputed date
 * @retval	TRUE	not error
 * @retval	FALSE	error
 * @remarks 
 * @if HISTORY
 *			4-Apr-2006	Kwanho Shin created
 * @endif
 */
static bool PrvBioCheckError()
{
	rint32 nLastday;

	if ((PrvBioGlobals->nInYear > current_time.tm_year+1900) ||
		((PrvBioGlobals->nInYear == current_time.tm_year+1900) && (PrvBioGlobals->nInMonth > current_time.tm_mon+1)) ||
		((PrvBioGlobals->nInYear == current_time.tm_year+1900) && (PrvBioGlobals->nInMonth == current_time.tm_mon+1) && (PrvBioGlobals->nInDay > current_time.tm_mday)))
		return FALSE;
	
	if (PrvBioGlobals->nInMonth > 12 || PrvBioGlobals->nInMonth < 1)
		return FALSE;

	if (PrvBioGlobals->nInDay > 31 || PrvBioGlobals->nInDay < 1)
		return FALSE;

	nLastday = PrvBioGetDaysInMonth(PrvBioGlobals->nInYear, PrvBioGlobals->nInMonth);

	if (PrvBioGlobals->nInDay > nLastday)
		return FALSE;
	
	return TRUE;
}

/**
 * @brief		Move caret
 * @param[in]	wnd 	window handle
 * @param[in]	key 	inputed key
 * @return	0	Always return 0
 * @remarks 
 * @if HISTORY
 *			4-Apr-2006	Kwanho Shin created
 * @endif
 */
static rint32 PrvBirthdayCreateCaret(WND wnd, char key)
{
	rint32 nCaret_x;

	if (key == 'l')
	{
		if (PrvBioGlobals->nLocation == 0)
			PrvBioGlobals->nLocation = 2;
		else
			PrvBioGlobals->nLocation -= 1;
	}
	if (key == 'r')
	{
		if (PrvBioGlobals->nLocation == 2)
			PrvBioGlobals->nLocation = 0;
		else
			PrvBioGlobals->nLocation += 1;
	}
	if (PrvBioGlobals->nLocation == 0)
	{
		nCaret_x = PrvBioGlobals->nScreenWidth * 25 / 100;
	}
	else if (PrvBioGlobals->nLocation == 1)
	{
		nCaret_x = PrvBioGlobals->nScreenWidth * 50 / 100;
	}
	else if (PrvBioGlobals->nLocation == 2)
	{
		nCaret_x = PrvBioGlobals->nScreenWidth * 64 / 100;
	}

//	AniDestroyCaret();
// 
// 	if (PrvBioGlobals->nLocation == 0)
// 		AniCreateCaret(wnd, (ruint16)(PrvBioGlobals->nScreenWidth * 19 / 100), (ruint16)(PrvBioGlobals->nScreenHeight * 7 / 100), COLOR_WHITE);
// 	else
// 		AniCreateCaret(wnd, (ruint16)(PrvBioGlobals->nScreenWidth * 9 / 100), (ruint16)(PrvBioGlobals->nScreenHeight * 7 / 100), COLOR_WHITE);
// 
// 	AniSetCaretPos((ruint16)nCaret_x, (ruint16)(PrvBioGlobals->nScreenHeight * 68 / 100));
// 	AniShowCaret();

	return 0;
}

/**
 * @brief	Move a month
 * @param[in]	key 	inputed key
 * remarks
 * @if HISTORY
 *			4-Apr-2006	Kwanho Shin created
 * @endif
 */
static void PrvGraphMoveMonthGraph(char key)
{
	if ((key == 'u') || (key == 'r'))
	{
		if (PrvBioGlobals->nSelectMonth == 12)
		{
			PrvBioGlobals->nSelectYear += 1;
			PrvBioGlobals->nSelectMonth = 1;
		}
		else
		{
			PrvBioGlobals->nSelectMonth += 1;
		}

		if (key == 'r')
			PrvBioGlobals->nSelectDay = 1;
	}
	else if ((key == 'd') || (key == 'l'))
	{
		if (PrvBioGlobals->nSelectMonth == 1)
		{
			PrvBioGlobals->nSelectYear -= 1;
			PrvBioGlobals->nSelectMonth = 12;
		}
		else
		{
			PrvBioGlobals->nSelectMonth -= 1;
		}

		if (key == 'l')
			PrvBioGlobals->nSelectDay = PrvBioGetDaysInMonth(PrvBioGlobals->nSelectYear, PrvBioGlobals->nSelectMonth);
	}
}

/**
 * @brief		Initiate the birthday 
 * remarks
 * @if HISTORY
 *			26-Apr-2006	Kwanho Shin created
 * @endif
 */
static void PrvBioInitBiorhythm()
{
	char year[4];
	char month[2];
	char day[2];

	UsrGetCurrentTime(&current_time);

	MemSet(year, 0, sizeof(year));
	MemSet(month, 0, sizeof(month));
	MemSet(day, 0, sizeof(day));

	PrvBioGlobals = MemAlloc(sizeof(BiorhythmGlobalsType));

	PrvBioGlobals->nInYear = current_time.tm_year + 1900;
	PrvBioGlobals->nInMonth = current_time.tm_mon + 1;
	PrvBioGlobals->nInDay = current_time.tm_mday;
	//load registry
#ifdef __REGISTRY_H__
	if (RegInit())
	{
		PrvBioGlobals->nInYear = current_time.tm_year + 1900;
		PrvBioGlobals->nInMonth = current_time.tm_mon + 1;
		PrvBioGlobals->nInDay = current_time.tm_mday;
	}
	else
	{
		RegReadBinary(ID_SET_BR_YEAR, year, sizeof(year));
		RegReadBinary(ID_SET_BR_MONTH, month, sizeof(month));
		RegReadBinary(ID_SET_BR_DAY, day, sizeof(day));

		PrvBioGlobals->nInYear = (year[0] - '0') * 1000 + (year[1] - '0') * 100 + (year[2] - '0') * 10 + (year[3] - '0');
		
		if (month[1] == 0)
		{
			PrvBioGlobals->nInMonth = month[0] - '0';
		}
		else
		{
			PrvBioGlobals->nInMonth = (month[0] - '0') * 10 + (month[1] - '0');
		}

		if (day[1] == 0)
		{
			PrvBioGlobals->nInDay = day[0] - '0';
		}
		else
		{
			PrvBioGlobals->nInDay = (day[0] - '0') * 10 + (day[1] - '0');
		}
	}
#endif
	PrvBioGlobals->nLocation = 0;
	PrvBioGlobals->nFocus = 1;
	GdiGetScreenInfo(&PrvBioGlobals->nScreenWidth, &PrvBioGlobals->nScreenHeight);
}

/**
 * @brief		Create The Birthday Input Interface
 * @param[in]	wnd 	window handle
 * remarks
 * @if HISTORY
 *			6-Apr-2006	Kwanho Shin created
 *			13-Apr-2006	Kwanho Shin modified
 *			26-Apr-2006	Kwanho Shin modified
 * @endif
 */
static void PrvBirthdayFormCreate(WND wnd)
{
	rint32 nBio_x;
	rint32 nBirthday_x;

	rect rec;

	nBio_x = (PrvBioGlobals->nScreenWidth * 50 / 100) - (StrLen(BIORHYTHM_STRING) * 4);
	nBirthday_x = (PrvBioGlobals->nScreenWidth * 50 / 100) - (StrLen(BIRTHDAY_STRING) * 4);

	GdiLockScreen(wnd);

	GdiClrScreen(wnd, COLOR_WHITE);
	GdiTextOut(wnd, BIORHYTHM_STRING, StrLen(BIORHYTHM_STRING), (int)nBio_x, (int)(PrvBioGlobals->nScreenHeight * 10 / 100));
	GdiTextOut(wnd, BIRTHDAY_STRING, StrLen(BIRTHDAY_STRING), (int)nBirthday_x, (int)(PrvBioGlobals->nScreenHeight * 40 / 100));
	GdiTextOut(wnd, DIVISION_STRING, StrLen(DIVISION_STRING), (int)(PrvBioGlobals->nScreenWidth * 46 / 100), (int)(PrvBioGlobals->nScreenHeight * 70 / 100));
	GdiTextOut(wnd, DIVISION_STRING, StrLen(DIVISION_STRING), (int)(PrvBioGlobals->nScreenWidth * 60 / 100), (int)(PrvBioGlobals->nScreenHeight * 70 / 100));

	rec.top = PrvBioGlobals->nScreenHeight * 68 / 100;
	rec.bottom = PrvBioGlobals->nScreenHeight * 75 / 100;
	rec.left = PrvBioGlobals->nScreenWidth * 25 / 100;
	rec.right = PrvBioGlobals->nScreenWidth * 44 / 100;
	GdiRectangle(wnd, &rec);

	rec.left = PrvBioGlobals->nScreenWidth * 50 / 100;
	rec.right = PrvBioGlobals->nScreenWidth * 59 / 100;
	GdiRectangle(wnd, &rec);

	rec.left = PrvBioGlobals->nScreenWidth * 64 / 100;
	rec.right = PrvBioGlobals->nScreenWidth * 73 / 100;
	GdiRectangle(wnd, &rec);

	PrvBirthdayCreateCaret(wnd, 0);
	
	PrvBirthdayScrollDate(wnd);

	GdiUnlockScreen(wnd);

	PrvBioGlobals->nCurrFormID = BIO_BIRTHDAYINPUT_FORM;
}

/** Message handler of Birthday Input Form
 *
 * @param	wnd	Window handle.
 * @param	msg		Message type from message queue.
 * @param	wParam	Contains key code if msg is AWM_KEYDOWN.
 * @param	lParam	Not used.
 *
 * @return	void
 */
static void PrvBirthdayInputHandler(WND wnd, rint32 msg, rint32 wParam, rint32 lParam)
{
	char year[5];
	char month[3];
	char day[3];
	
	switch(msg)
	{
	case AWM_KEYDOWN:
		if (!PrvBioGlobals->nFocus)
		{
			PrvBioCloseMsgErrPop();
		}
		
		switch(wParam)
		{
		case VK_USER_LEFT:	//Pressed "left"direction key
			PrvBirthdayCreateCaret(wnd, 'l');
			break;

		case VK_USER_RIGHT: //Pressed "right"direction key
			PrvBirthdayCreateCaret(wnd, 'r');
			break;

		case VK_USER_OK:	//Pressed "Enter" key
			PrvBioGlobals->nSelectYear = current_time.tm_year + 1900;
			PrvBioGlobals->nSelectMonth = current_time.tm_mon+1;
			PrvBioGlobals->nSelectDay = current_time.tm_mday;

			if (!PrvBioCheckError())
			{
				PrvBioMsgError(wnd);
			}
			else
			{
// 				AniDestroyCaret();
				PrvBioDrawBiorhythmForm(wnd);
				//save registry
				MemSet(year, 0, sizeof(year));
				MemSet(month, 0, sizeof(month));
				MemSet(day, 0, sizeof(day));

				StrIToA(PrvBioGlobals->nInYear, year);
				StrIToA(PrvBioGlobals->nInMonth, month);
				StrIToA(PrvBioGlobals->nInDay, day);
#ifdef __REGISTRY_H__
				RegWriteBinary(ID_SET_BR_YEAR, year, sizeof(year)-1);
				RegWriteBinary(ID_SET_BR_MONTH, month, sizeof(month)-1);
				RegWriteBinary(ID_SET_BR_DAY, day, sizeof(day)-1);
#endif
			}
			break;

		case VK_PAD_9:	//Pressed "Esc" key
// 			AniDestroyCaret();
			StaGotoState(wnd, STATE_ID_ENTRY, MSG_DESTROY, MSG_CREATE );
			break;

		case VK_USER_UP:	//Pressed "up"direction key
			if (PrvBioGlobals->nLocation == 0)
			{
				PrvBioGlobals->nInYear += 1;
			}
			else if (PrvBioGlobals->nLocation == 1)
			{
				if (PrvBioGlobals->nInMonth == 12)
					break;
	
				PrvBioGlobals->nInMonth += 1;
			}
			else if (PrvBioGlobals->nLocation == 2)
			{
				if (PrvBioGlobals->nInDay == 31)
					break;

				PrvBioGlobals->nInDay += 1;
			}
			PrvBirthdayScrollDate(wnd);
			break;

		case VK_USER_DOWN:	//Pressed "down"direction key
			if (PrvBioGlobals->nLocation == 0)
			{
				PrvBioGlobals->nInYear -= 1;
			}
			else if (PrvBioGlobals->nLocation == 1)
			{
				if (PrvBioGlobals->nInMonth == 1)
					break;

				PrvBioGlobals->nInMonth -= 1;
			}
			else if (PrvBioGlobals->nLocation == 2)
			{
				if (PrvBioGlobals->nInDay == 1)
					break;

				PrvBioGlobals->nInDay -= 1;
			}
			PrvBirthdayScrollDate(wnd);
			break;
		}
	case AWM_TIMER:
		if (wParam == REXY_POPUPDLG_TIMER)
		{
			PrvBioCloseMsgErrPop();
		}
	}
}

/** Message handler of Biorhythm Graph Form
 *
 * @param	wnd	Window handle.
 * @param	msg		Message type from message queue.
 * @param	wParam	Contains key code if msg is AWM_KEYDOWN.
 * @param	lParam	Not used.
 *
 * @return	void
 */
static void PrvGraphDrawHandler(WND wnd, rint32 msg, rint32 wParam, rint32 lParam)
{
	rint32 lastday;

	switch(msg)
	{
	case AWM_KEYDOWN:
		if (!PrvBioGlobals->nFocus)
		{
			PrvBioCloseMsgErrPop();
			//break;
		}
		switch(wParam)
		{
		case VK_USER_LEFT:	//Pressed "left"direction key
			if (PrvBioGlobals->nSelectDay > 1)
			{
				if ((PrvBioGlobals->nSelectYear == PrvBioGlobals->nInYear) && (PrvBioGlobals->nSelectMonth == PrvBioGlobals->nInMonth) && (PrvBioGlobals->nSelectDay == PrvBioGlobals->nInDay))
				{
					PrvBioMsgError(wnd);
					break;
				}
				PrvBioGlobals->nSelectDay -= 1;
			}
			else
			{
				PrvGraphMoveMonthGraph('l');
			}
			PrvBioDrawBiorhythmForm(wnd);
			break;

		case VK_USER_RIGHT: //Pressed "right"direction key
			lastday = PrvBioGetDaysInMonth(PrvBioGlobals->nSelectYear,PrvBioGlobals->nSelectMonth);

			if (PrvBioGlobals->nSelectDay < lastday)
			{
				PrvBioGlobals->nSelectDay += 1;
			}
			else
			{
				PrvGraphMoveMonthGraph('r');
			}
			PrvBioDrawBiorhythmForm(wnd);
			break;

		case VK_PAD_9:	//Pressed "Esc" key
			PrvBirthdayFormCreate(wnd);
			break;

		case VK_USER_UP:	//Pressed "up"direction key
			PrvGraphMoveMonthGraph('u');
			PrvBioDrawBiorhythmForm(wnd);
			break;

		case VK_USER_DOWN:	//Pressed "down"direction key
			if ((PrvBioGlobals->nSelectYear == PrvBioGlobals->nInYear) && (PrvBioGlobals->nSelectMonth == PrvBioGlobals->nInMonth))
			{
				PrvBioMsgError(wnd);
				break;
			}
			PrvGraphMoveMonthGraph('d');
			PrvBioDrawBiorhythmForm(wnd);			
			break;
		}
	case AWM_TIMER:
		if (wParam == REXY_POPUPDLG_TIMER)
		{
			PrvBioCloseMsgErrPop();
		}
	}	
}

/** Message dispatcher of the biorhythm application.
 * This function dispatches messages to appropriate message handler current by form ID.
 *
 * @param	hWnd	Window handle from CalendarProc().
 * @param	msg		Message from CalendarProc().
 * @param	wParam	wParam from CalendarProc().
 * @param	lParam	lParam from CalendarProc().
 *
 * @return	void
 */
static void PrvBioFormHandler(WND hWnd, rint32 msg, rint32 wParam, rint32 lParam)
{
	rint32 i;

	for (i = 0; i < BIO_LAST_FORM; i++)
	{
		if(PrvBioGlobals->nCurrFormID == g_BioFormHandleList[i].eFormId)
		{
			g_BioFormHandleList[i].pfnFormHandler(hWnd, msg, wParam, lParam);
			break;
		}
	}
}

/**
 * @brief		biorhythm procedure
 * @param[in]	hWnd	window handle
 * @param[in]	msg		event message
 * @param[in]	wParam	customizable parameter
 * @param[in]	lParam	customizable parameter
 * @return	0	when given event message is not handled by idle procedure
 * @remarks		Currently, this biorhythm Procedure is coded for demonstration.
 * @if HISTORY
 *			4-Apr-2006	Kwanho Shin created
 *			3-May-2006	Kwanho Shin modified
 * @endif
 */
rint32 PrvBiorhythmProc(WND hWnd, rint32 msg, rint32 wParam, rint32 lParam)
{
	if (msg == AWM_CREATE)
	{
		PrvBioInitBiorhythm();
		PrvBirthdayFormCreate(hWnd);
	}
	else if (msg == AWM_DESTROY)
	{
		// Handle quit event of this application here if necessary
	}
	else
	{	
		PrvBioFormHandler(hWnd, msg, wParam, lParam);
	}

	return 0;
}

rint32 BiorhythmProc(WND wnd, rint32 msg, rint32 wParam, rint32 lParam)
{
	WND BioMainWindow;
	int	ScreenWidth, ScreenHeight;
	int bHandled = FALSE;

	switch ( msg )
	{
		case AWM_CREATE:

			// Create Window for this application
			GdiGetScreenInfo( &ScreenWidth, &ScreenHeight ); // We are going to use whole screen
			BioMainWindow= WinCreateWindow( PrvBiorhythmProc, 0, 0, ScreenWidth, ScreenHeight, WND_PANEL );

			// Do something for your application's initialization

			bHandled = TRUE;
			break;
			
		case AWM_DESTROY:
			
			// Do something to close this app
			GdiSetBkColor(wnd,COLOR_WHITE);
			GdiSetFgColor(wnd,COLOR_BLACK);

			// Close all window that are created by this application
			WinDestroyWindow(wnd);
			bHandled = TRUE;	// End this application
			break;
	}

	return bHandled;
}
