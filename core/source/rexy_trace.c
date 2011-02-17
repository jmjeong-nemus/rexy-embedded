/*
 * $Id: rexy_trace.c 9002 2005-11-03 03:32:48Z madholic $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 */

/** @file */

/**
* @defgroup trace Core Trace Service
* Trace APIs
* @ingroup RexyECoreGroup
* @{
*/

#include "rexy.h"

#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

#ifdef __SOFT_DEBUG__
#define MAX_DEBUG_BREAK_FUNC_NUM	5
static char	g_TrcDebugFunction[MAX_DEBUG_BREAK_FUNC_NUM][50]={0,};
static bool	g_DebugStop;
#endif

AREA_LIST arrAreaList[] =
    {
        {"test",	    TR_Test, 0 },
        {"core",	    TR_RexyCore, 0},
        {"control",     TR_RexyControl, 0},
        {"etc",	        TR_RexyEtc, 0},
        {"system",      TR_SystemEvent, 0},
        {"component",   TR_COMPONENT, 0},

        {"mem",         TR_Memory, 0},

    };

const int AREA_COUNT = sizeof(arrAreaList)/sizeof(AREA_LIST);

unsigned int TRACELEVEL = TR_MARKLEVEL4;
unsigned int TRACESELECT = 0;

static int PrvConvertAreaLevel(int level)
{
	int ret;
	switch ( level )
	{
		case 0xf: ret = 1; break;
		case 0xe: ret = 2; break;
		case 0xc: ret = 3; break;
		case 0x8: ret = 4; break;
		default: ret = 0; break;
	}
	return ret;
}
int TrcStrCmp(const char *string1, const char *string2)
{
	int i,j;
	int	Len[2];
	char *SourceS[2];
	char *ConvertedS[2];
	int	 result;

	Len[0] = RalStrLen(string1);
	Len[1] = RalStrLen(string2);

	SourceS[0] = (char *)string1;
	SourceS[1] = (char *)string2;

	for ( i=0; i< 2; i++ )
	{
		ConvertedS[i] = RalMemAlloc(Len[i]+1);
		
		for ( j = 0; j <= Len[i]; j++ )
		{
			if ( *(SourceS[i]+j)>='a' && *(SourceS[i]+j)<='z' )
				*(ConvertedS[i]+j)=*(SourceS[i]+j) - 'a' + 'A';
			else
				*(ConvertedS[i]+j)=*(SourceS[i]+j);
		}
	}

	result = RalStrCmp(ConvertedS[0],ConvertedS[1]);

	RalMemFree(ConvertedS[0]);
	RalMemFree(ConvertedS[1]);

	return result;
}
int TrcAddBreakFunction(char *FunctionName)
{
#ifdef __SOFT_DEBUG__
	int		i;
	bool	bFoundSlot = FALSE;

	for ( i=0; i< MAX_DEBUG_BREAK_FUNC_NUM ; i++ )
	{
		if ( g_TrcDebugFunction[i][0] == NULL )
		{
			RalStrCpy(g_TrcDebugFunction[i],FunctionName);
			bFoundSlot = TRUE;
			break;
		}
	}

	if ( bFoundSlot ) return 0;
	else return 1;	// slot full

#else
	return -1;
#endif
}
int TrcDelBreakFunction(int SlotNumber)
{
	/*
	int i;

	if ( SlotNumber == -1 ) RalMemSet(g_TrcDebugFunction,0,sizeof(g_TrcDebugFunction));
	else if ( SlotNumber >=0 && SlotNumber < MAX_DEBUG_BREAK_FUNC_NUM ) *g_TrcDebugFunction[SlotNumber] = 0;
	*/
}

void TrcReleaseBreak(void)
{
#ifdef __SOFT_DEBUG__
	g_DebugStop = FALSE;
#endif
}

void TrcProbeReport(char *FunctionName)
{
#ifdef __SOFT_DEBUG__
	int i;
	msg DummyMsgContainer;
	
	for ( i=0; i<MAX_DEBUG_BREAK_FUNC_NUM; i++)
	{
		if ( !RalStrCmp( g_TrcDebugFunction[i],FunctionName ) ) 
		{
			g_DebugStop = TRUE;
			TrcPrintF("Break on '%s' executed.\n",FunctionName);
		}
	}
	while ( g_DebugStop )
	{
		RalEventGet(&DummyMsgContainer, 0);
	};
	return;
#endif
}

void TrcDumpBreakPoints()
{
	/*
	int i;
	TrcPrint("== Break Point List ==\n");
	for ( i=0; i < MAX_DEBUG_BREAK_FUNC_NUM ; i ++ )
	{
		TrcPrintF("%d slot : ",i);
		if ( *g_TrcDebugFunction[i] ) 
		{
			TrcPrintF("%s\n",g_TrcDebugFunction[i]);
		}
		else
			TrcPrintF("[empty]\n");
	}
	*/
}
void TrcDumpAreaStatus(void)
{
    char buffer[100];
    int i;

    TrcPrint("=============================================\n");
    sprintf(buffer, "TRACE_LEVEL : %d\n", PrvConvertAreaLevel(TRACELEVEL));
    TrcPrint( buffer );
    
    for ( i = 0 ;i < AREA_COUNT ; i++ )
    {
		sprintf(buffer, "%10s : ", arrAreaList[i].szAreaName);
		TrcPrint(buffer);
		if ( TRACESELECT & arrAreaList[i].szAreaSelect ) {
			sprintf(buffer, "on  [Lv: %d]\n",
					   ((arrAreaList[i].szAreaLevel == 0) ?
						PrvConvertAreaLevel(TRACELEVEL) : PrvConvertAreaLevel(arrAreaList[i].szAreaLevel)));
			TrcPrint(buffer);
		}
		else {
			TrcPrint("off\n");
		}
    }
    TrcPrint("=============================================\n");
}
// Created by Joshua on 18-Sep-2006
void TrcPrintAreaName(void)
{
	int i;

	TrcPrint("all");
	for ( i=0;i<AREA_COUNT;i++)
	{
		TrcPrintF(" | %s",arrAreaList[i].szAreaName);
	}
}

void TrcControlTraceAll(bool on)
{
	int i;
	for ( i = 0 ;i < AREA_COUNT ; i++ ) {
		arrAreaList[i].szAreaLevel = 0;
	}
	if ( on )
		TRACESELECT = 0xFFFFFFFFL;
	else
		TRACESELECT = 0x00000000L;
}

void TrcSetTraceLevel(int level)
{
	if ( level <= 0 || level > 4 )
	{
		return;
	}
	switch ( level )
	{
	case 1:
		TRACELEVEL = TR_MARKLEVEL1;
		break;
	case 2:
		TRACELEVEL = TR_MARKLEVEL2;
		break;
	case 3:
		TRACELEVEL = TR_MARKLEVEL3;
		break;
	case 4:
		TRACELEVEL = TR_MARKLEVEL4;
		break;
	}
}
/** 
 * @brief	Parse trace area
 * @ifdef HISTORY
 *		25-Aug-2006	Joshua	Updated to return error code
 * @endif
 */
RxErr TrcControlTraceArea(const char* areaName, int level)
{
	int i;
	RxErr error = ( RxErrTrcClass | enInvalidParam );

    for ( i = 0 ; i < AREA_COUNT ; i++ )
    {
        if ( !RalStrCmp(areaName, arrAreaList[i].szAreaName ) )
        {
            if ( level == 0 )
            {
				TRACESELECT |= (arrAreaList[i].szAreaSelect);
            }
            else if ( level < 0 || level > 4 )
            {
				TRACESELECT &= ~(arrAreaList[i].szAreaSelect);
            }
            else if (1<= level && level <= 4) 
			{
				TRACESELECT |= (arrAreaList[i].szAreaSelect);
				switch ( level )
				{
				case 1:
					arrAreaList[i].szAreaLevel = TR_MARKLEVEL1;
					break;
				case 2:
					arrAreaList[i].szAreaLevel = TR_MARKLEVEL2;
					break;
				case 3:
					arrAreaList[i].szAreaLevel = TR_MARKLEVEL3;
					break;
				case 4:
					arrAreaList[i].szAreaLevel = TR_MARKLEVEL4;
					break;
				}				
            }
			error = RxErrNone;
			break;
        }
	}
	return error;
}


/*
  Usage:

  TRACE(TR_Test, TR_LEVEL1, ("This is test main : %d", nint));      // X
  TRACE(TR_Address, TR_LEVEL4, ("Important Message : %s", "time")); // O


*/

/**
* @brief		
* @param[in]	color		
* @param[in]	fmt		
* @param[in]	...		
* @return		void
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
void TrcKLoG(int color, const char* fmt, ...)
{
#ifdef USE_ANSI
    int cr;
#endif
    va_list arg;
    char buf[0x100];

#ifdef USE_ANSI
    switch(color)
    {
    case COLOR_RED: cr = 31; break;
    case COLOR_GREEN: cr = 32; break;
    case COLOR_BLUE: cr = 34; break;
    case COLOR_MAGENTA: cr = 35; break;
    case COLOR_YELLOW: cr = 33; break;
    case COLOR_CYAN: cr = 36; break;
    default: cr = 37; break;
    }
    TrcPrintF("\x1B[1;%dm", cr);
#endif
	
    va_start(arg, fmt);
    vsprintf( buf, fmt, arg );
    va_end(arg);
    RalDebugMessage(buf);

#ifdef USE_ANSI
    RalDebugMessage("\x1B[0;37m");
#endif
    RalDebugMessage("\n");
}

/**
* @brief		
* @param[in]	color		
* @param[in]	fmt		
* @param[in]	...		
* @return		void
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
void TrcKLogN(int color, const char* fmt, ...)
{
#ifdef USE_ANSI
    int cr;
#endif
    va_list arg;
    char buf[0x100];

#ifdef USE_ANSI
    switch(color)
    {
    case COLOR_RED: cr = 31; break;
    case COLOR_GREEN: cr = 32; break;
    case COLOR_BLUE: cr = 34; break;
    case COLOR_MAGENTA: cr = 35; break;
    case COLOR_YELLOW: cr = 33; break;
    case COLOR_CYAN: cr = 36; break;
    default: cr = 37; break;
    }
    TrcPrintF("\x1B[1;%dm", cr);
#endif

    va_start(arg, fmt);
    vsprintf( buf, fmt, arg );
    va_end(arg);
    RalDebugMessage(buf);

#ifdef USE_ANSI
    RalDebugMessage("\x1B[0;37m");
#endif
}

#ifdef _DEBUG
static HANDLER_TYPE cur_handler_type = REXY_FUNC;
static int cur_app_id = 0;
static int cur_state_id = 0;
#endif

/**
* @brief		
* @param[in]	type		
* @param[in]	app		
* @param[in]	state		
* @return		void
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
void TrcSetCurrentHandler(int type, int app, int state)
{
#ifdef _DEBUG
    cur_handler_type = type;
    cur_app_id = app;
    cur_state_id = state;
#endif
}

/**
* @brief		Print debug message.
* @param[in]	str		the string will be printed
* @return		void
* @remarks		This debug message is displayed different by each platform.
*				On windows, the message is displayed on the output window of the visual c++ IDE
*				On linux, the message is displayed on the console
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
void TrcPrint(const char * str )
{
	RalDebugMessage( str );
}

/**
* @brief		Print debug message by formatted data.
* @param[in]	format		Format-control string
* @param[in]	...			Optional arguments
* @return		the number of characters written, not including the terminating null character, or a negative value if an output error occurs.
* @remarks		This debug message is displayed different by each platform.
*				On windows, the message is displayed on the output window of the visual c++ IDE
*				On linux, the message is displayed on the console
*			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/
int TrcPrintF(const char * format, ...)
{
	int ret;
    char buf[0x100];
    va_list arg;
    va_start( arg, format );
    ret = vsprintf( buf, format, arg );
    RalDebugMessage( buf );
    va_end( arg );
    return ret;
	//return TrcPrintF(const char * format, ...);
}
/** 
 *	@brief	Dump memory through TrcPrint API
 *	@if HISTORY
 *			18-Sep-2006	Joshua	created
 *	@endif
 */
void TrcDumpMemory(void *Ptr,int DumpSize,int NumColumn)
{
#ifndef F_REXY_ENV_PHONE
	unsigned char *data;
	int i = 0;
	int offset = 0;
	int	RealColumnNum;
	char	Buffer[2]={0,0};
	/*
    * Heading
    */
	TrcPrint("                    "); /* skip address area */
	for ( i = 0; i < NumColumn ; ++i )
	{
		TrcPrintF("+%lX     ", (long unsigned)i);
	}
	TrcPrint("Text\n");
	/*
    * Body
    */
	do
	{
		data = (char *)((unsigned int)Ptr+offset);
		
		/* Print the base address. */
		TrcPrintF("%08lX: ", (long unsigned)data);

		RealColumnNum = ( offset + NumColumn > DumpSize ) ? DumpSize-offset : NumColumn;

		/* Print the characters' hex values. */
		for ( i = 0; i < RealColumnNum; ++i )
		{
			TrcPrintF("0x%02X ", data[i]);
		}
		/* Pad the hex values area if necessary. */
		for ( ++i; i <= NumColumn; ++i )
		{
			TrcPrint("         ");
		}
		/* Print the characters. */
		for ( i = 0; i < RealColumnNum; i++ )
		{
			if ( isprint(data[i]) )
			{
				Buffer[0] = data[i];
				TrcPrint(Buffer);
			}
			else
				TrcPrint(".");
		}
		TrcPrint("\n");

		offset += NumColumn;
		
	} while ( offset < DumpSize ); /* break on partial buffer */
#endif
}

void TrcDbgBreak(const char* file_name, const int line_num)
{
}

const char* TrcGetShortFileName(const char* szFileName)
{
    const char* prev;
    const char* tmp;

    prev = tmp = szFileName;
    while(1)
    {
        while( *tmp && *tmp != '\\' ) tmp++;
        if ( !(*tmp) ) break;
        tmp++;
        prev = tmp;
    }
    return prev;
}


void TrcPrintHdr(unsigned int area, unsigned int level)
{
    int i;
    TrcPrint("[");

    for ( i = 0 ;i < AREA_COUNT ; i++ ) {
        if (arrAreaList[i].szAreaSelect == area) {
            TrcPrintF("%s:", arrAreaList[i].szAreaName);
            break;
        }
    }

    switch (level) {
    case TR_LEVEL1 : TrcPrint("1]");
        break;
    case TR_LEVEL2 : TrcPrint("2]");
        break;
    case TR_LEVEL3 : TrcPrint("3]");
        break;
    case TR_LEVEL4 : TrcPrint("4]");
        break;
    }
}
const char* TrcMessageName(int msg)
{
	const char* str;
	switch(msg)
	{
	case AWM_NONE: 		str="AWM_NONE";		break;
	case AWM_SYS: 		str="AWM_SYS";		break;
	case AWM_CREATE: 	str="AWM_CREATE";	break;
	case AWM_DESTROY: 	str="AWM_DESTROY";	break;
	case AWM_LOAD: 		str="AWM_LOAD";		break;
	case AWM_SAVE: 		str="AWM_SAVE";		break;
	case AWM_ALARM: 	str="AWM_ALARM";	break;
	case AWM_SHORTCUT: 	str="AWM_SHORTCUT";	break;
	case AWM_ERASEBKGND: 	str="AWM_ERASEBKGND";	break;
	case AWM_SHOWWINDOW: 	str="AWM_SHOWWINDOW";	break;
	case AWM_PAINT: 	str="AWM_PAINT";	break;
	case AWM_SETFOCUS: 	str="AWM_SETFOCUS";	break;
	case AWM_KILLFOCUS: 	str="AWM_KILLFOCUS";	break;
	case AWM_COMMAND: 	str="AWM_COMMAND";	break;
	case AWM_CHAR: 		str="AWM_CHAR";		break;
	case AWM_KEYDOWN: 	str="AWM_KEYDOWN";	break;
	case AWM_KEYUP: 	str="AWM_KEYUP";	break;
	case AWM_KEYLONG: 	str="AWM_KEYLONG";	break;
	case AWM_KEYREPEAT: 	str="AWM_KEYREPEAT";	break;
	case AWM_QUIT: 		str="AWM_QUIT";		break;
	case AWM_TIMER: 	str="AWM_TIMER";	break;
	case AWM_DRAWITEM: 	str="AWM_DRAWITEM";	break;
	case AWM_SHUT_S: 	str="AWM_SHUT_S";	break;
	case AWM_RETURN: 	str="AWM_RETURN";	break;
	case AWM_STATUSBAR: 	str="AWM_STATUSBAR";	break;
	case AWM_UPDATE_PICT: 	str="AWM_UPDATE_PICT";	break;
	case AWM_BATTERY_STAT: 	str="AWM_BATTERY_STAT";	break;
	case AWM_CLOSE_POPUP: 	str="AWM_CLOSE_POPUP";	break;
	case AWM_UPDATECONTROL: str="AWM_UPDATECONTROL";break;
	case AWM_DISPUPDATE: 	str="AWM_DISPUPDATE";	break;
	case AWM_SETHILITE: 	str="AWM_SETHILITE";	break;
	case AWM_KILLHILITE: 	str="AWM_KILLHILITE";	break;
	case AWM_SYS_LAST_CODE: str="AWM_SYS_LAST_CODE";break;
	case AWM_USER_CONTROL: 	str="AWM_USER_CONTROL";	break;
	case AWM_USER_CONTROL_LAST_CODE: str="AWM_USER_CONTROL_LAST_CODE";break;
	case AWM_APP: 		str="AWM_APP";		break;
	case AWM_APP_LAST_CODE: str="AWM_APP_LAST_CODE";break;
	default: str="Unknown"; break;
	}
	return str;
}

void TrcAssert(char *log)
{
	{
		WND hwnd;
		rint32 screen_width;
		rint32 screen_height;

		hwnd = WinGetSelection();
		GdiGetScreenInfo( &screen_width, &screen_height );
		GdiClrScreen( hwnd, COLOR_GREEN );
		GdiDrawTextOut( hwnd, 10,10, screen_width-10, log, StrLen(log), ALIGN_LEFT );
		while(1);
	}
}

/** @} */ // End of Trace
