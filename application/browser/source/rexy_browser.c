/* 
* $Id: rexy_browser.c 491 2006-04-05 00:26:34Z madholic $
 * Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.    
*   
* This file is a part of Rexy Embedded
*   
*/
#include "../header/rexy_browser.h"
#include "rexy_ctl_popup.h"
#include "application.h"


// ui interface api
#include "../header/ui.h"

#define MAX_LST_NUMBER 15
#define MAX_FILE_NAME 40
#define MAX_CHAR_PER_LINE 27
#define MAX_TXT_BUFFER (MAX_CHAR_PER_LINE*MAX_LST_NUMBER)

#define LIST_TEXT_X 10
#define LIST_TEXT_Y 10
#define LIST_TEXT_SPACE 15

#define LIST_TEXT_FOCUS_X 10
#define LIST_TEXT_FOCUS_Y  23
#define LIST_TEXT_FOCUS_LINE_SPACE 15

#define FOCUS_LINE_LENGTH 100

#define FILEPATH "demodir"


int totalFileNum;


char textList[MAX_LST_NUMBER][MAX_FILE_NAME];

int txvfocusedList;
int listCount;

int currentPage;
int lineNumber;
int totalPage;
bool runningTimer;

int fileLocationPointer;
int preFileLocationPointer;

char * textBuffer;

//extern REXY_CTL *gptextViewerCtl;
extern bool isFileExist;
extern int isFirstAccessToTextViewer;
extern int pageOffSetChar;

//extern int PopTextViewerCB(REXY_CTL *pCtl, int nMsg, int wparam, int lparam, int bIsPre, int *pErr);
extern int BrowserKeyEvent(WND hWnd, int wParam, int lParam);

// temp global variable
static char *argv[100] = {"-i ofbis file://D:/index.htm"};
static int argc = 3;
static char *url = "file://D:/index/index.html";

WND hBrowserWnd;
unsigned char isUtf8  = 0;
int launchParam	= 0;

int InitRexyBrowser(WND hWnd, int wParam)
{
	int ret;
	char url[200];

	// save browser window handle
	hBrowserWnd = hWnd;

	// start browser
	GdiTextOut(hBrowserWnd, "Start Browser", strlen("Start Browser"), 10, 10);

	// init ui
	ret = ui_init(argc, argv);

	if( ret )
		return ret;

	// open ui
	// /* Give over control to the user interface. */
	if( wParam == LAUNCH_FILELIST )
	{
		strcpy(url, (const char *)&textList[txvfocusedList]);
	}
	else	
	{
#if defined _WIN32_WCE && defined ARM && defined _ARM_ && defined ARMV4
		strcpy(url, "file://\\index\\index.html");
#else
#if defined _WIN32_WCE && !defined ARM && !defined _ARM_ && !defined ARMV4
		strcpy(url, "file://\\\\WIJ\\index\\index.html");		
#else
		RalChDir("..\\..\\..\\application\\browser\\index");
		strcpy(url, "index.html");
#endif
#endif
	}

	// 해당 url로 ui open을 한다.
	ret = ui_open(url);

	return ret;
}

int BrowserWinProc (WND hWnd, int nMsg, int wParam, int lParam)
{
    int bHandled = FALSE;

    switch(nMsg)
    {
    case AWM_CREATE:
		// browser window create
		InitRexyBrowser(hWnd, launchParam);
		GdiValidateAll();
		bHandled = TRUE;
		break;

    case AWM_PAINT:
		// draw rexy browser
		//GdiLockScreen(hWnd);
		//GdiClrScreen(hWnd, COLOR_WHITE);
		//GdiUnlockScreen(hWnd);
        break;

    case AWM_KEYDOWN:
		// key operation
		BrowserKeyEvent(hWnd, wParam, lParam);
		bHandled = TRUE;
		break;
	
	case AWM_TIMER:
		break;
    default:
        break;
    }

    return bHandled;
}


int BrowserProc(WND hWnd, int nMsg, int wParam, int lParam)
{
    WND hBrwWnd;

    int	ScreenWidth;
    int ScreenHeight;

    int bHandled = FALSE;

    switch(nMsg)
    {
    case AWM_CREATE:
        GdiGetScreenInfo( &ScreenWidth, &ScreenHeight );
		launchParam = wParam;
   		hBrwWnd	= WinCreateWindow( BrowserWinProc, 0, 0, ScreenWidth, ScreenHeight, WND_PANEL );
        bHandled = TRUE;
        break;

    case AWM_DESTROY:
        WinDestroyWindow(hWnd);
        bHandled = TRUE;
        break;

    default:
        break;
    }

    return bHandled;
}

