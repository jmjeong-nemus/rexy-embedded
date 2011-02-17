#include "rexy.h"
#include "rexy_haldevice.h"
#include "rexy_ctl_core.h"
#include "rexy_ctl_list.h"

#include "application.h"
#include <math.h>

#define ToRadian( degree )  ( (degree) * (3.14 / 180) )
#define ToDegree( radian )  ( (radian) * (180 / 3.14) )

#define IDLE_KEY_BUFFER_LENGTH 5
#define DEBUG_ENTERING_CODE "13524"

#define NumberOfContributor	20

char	gKeyBuffer[IDLE_KEY_BUFFER_LENGTH];

// Core Test related definitions

#define CT_BACK_COLOR	COLOR_BLUE
#define CT_FRONT_COLOR	COLOR_WHITE
#define MAX_CT_MENU_NUM			5
#define CT_ITEM_NUM_PER_PAGE	8

#define CT_ITEM_LEFT_MARGIN		25
#define CT_ITEM_TOP_MARGIN		80
#define CT_ITEM_HEIGHT_SPACE	20 

#define LISTCTRL_IMAGE_NUM		20

typedef struct {
	int value;
	char TestName[30];
} ResultInfo;

static	int	gCTPageNo=0;
CTMenuItemType gCTMenu[MAX_CT_MENU_NUM];	//Core Test Menu type
static char StrBuffer[1024];


static int g_IdleGlobal=0;
static char	g_strCredit[NumberOfContributor][40];
static rect g_CreditArea;

static char* g_MainListString[] =
{
	"Core Module Test",
	//"Control Test",
	"MGL Sample Apps",
	//"Biorhythm",
	//"Web Browser",
	"Tetris",
	//"Window Demo",
	//"Image Viewer",
	//"DB Test",
	"Exit"
};

WND g_wndMainList;
RxBITMAP g_bmpListNumN[LISTCTRL_IMAGE_NUM];
RxBITMAP g_bmpListNumS[LISTCTRL_IMAGE_NUM];
RxBITMAP g_bmpNemustech;

// Private Function Prototypes
static void PrvPushIdleKeyBuffer( int keycode );
static void PrvCleanIdleKeyBuffer(void);
static bool PrvIsIdleKeyBufferTriggered(void);

// Internal Functions related with Core Test
static void CoreTest(struct _window * hwnd, WINPROC prevProc);

static void PrvDeleteAssertLog(void);
static void PrvWriteAssertLog(const char* log);

static void PrvCTInitMenu(void);
static void PrvCTDrawMenu(struct _window * hwnd, int PageNo );
static int	PrvCTEventLoop(struct _window * hwnd);
static int	PrvCT_GDI(struct _window * hwnd);
static int	PrvCT_FNT(struct _window * hwnd);
static int	PrvCT_STR(struct _window * hwnd);
static int	PrvCT_RSC(struct _window * hwnd);

static int  PrvCTMarkResult(struct _window * hwnd,char *Question,char *Question2);
static int	PrvCTInputKey();

static void PrvCreateMainList(WND wnd);
static void PrvDrawMainList(WND wnd);
static void PrvCalcFocus(WND wnd, int nModifier);

//static void PrvDrawListItemText(WND wnd, int nLine, void *pData);
//static void PrvDrawListItemImage(WND wnd, int nLine, void *pData);
static void PrvDrawListItemFocus(WND wnd, int line, void *pData);
static void PrvDrawListColumn(WND wnd, int line, int column, void *pData);
static void PrvProcessKeyDown(WND wnd, int nModifier, void *pData);

static void PrvDrawIdle(WND wnd);

// Window Handlers
static int  PrvIdleWinProc(WND hwnd, int msg, int wparam, int lparam);

int	IdleEntryProc( WND hwnd, int msg, int wparam, int lparam )
{
	static WND IdleMainWindow;
	int	ScreenWidth, ScreenHeight;
	int i = 0;
	int bHandled = FALSE;
	
	switch ( msg )
	{
		case AWM_CREATE:
			
			// Create Window for this application
			GdiGetScreenInfo( &ScreenWidth, &ScreenHeight ); // We are going to use whole screen
			IdleMainWindow = WinCreateWindow( PrvIdleWinProc, 0, 0, ScreenWidth, ScreenHeight, WND_PANEL );
			

//			// Do something for your application's initialization
//			StrCpy(g_strCredit[i++], "Press 1 : Core Module  Test");
//			StrCpy(g_strCredit[i++], "Press 2 : Control Test");
//			StrCpy(g_strCredit[i++], "Press 3 : MGL Sample Apps");
//			StrCpy(g_strCredit[i++], "Press 4 : Biorhythm");
//			StrCpy(g_strCredit[i++], "Press 5 : Web Browser");
//			StrCpy(g_strCredit[i++], "Press 6 : Tetris");
//			StrCpy(g_strCredit[i++], "Press 7 : Window Demo");
//			StrCpy(g_strCredit[i++], "Press 8 : Image Viewer");
//			StrCpy(g_strCredit[i++], "Press 9 : Exit");
//
//			StrCpy(g_strCredit[i++], "");
//
//			StrCpy(g_strCredit[i++], "Project Manager ...... longdal");
//			StrCpy(g_strCredit[i++], "HAL common interface.. klenui");
//			StrCpy(g_strCredit[i++], "Core design    ....... madholic");
//			StrCpy(g_strCredit[i++], "Core design/Title..... joshua");
//			StrCpy(g_strCredit[i++], "Control design ....... namgung");
//			StrCpy(g_strCredit[i++], "Control design ....... alscar");
//
//			g_CreditArea.left = 10;
//			g_CreditArea.right = 230;
//			g_CreditArea.top = 140;
//			g_CreditArea.bottom = 260;
			bHandled = TRUE;
			break;
		case AWM_DESTROY:
			
			// Do something to close this app
			// GdiSetBkColor(hwnd,COLOR_WHITE);
			// GdiSetFgColor(hwnd,COLOR_BLACK);
			WinDestroyWindow(IdleMainWindow);
			bHandled = TRUE;	// End this application
			break;
	}
	
	return bHandled;
}

/**
 * @anchor  IdleWinProc
 * @breif	Idle window procedure
 * @param[in]	hwnd	window handle
 * @param[in]	msg		event message
 * @param[in]	wparam	customizable parameter
 * @param[in]	lparam	customizable parameter
 * @retval	1	when given event message is handled by idle procedure
 * @retval	0	when given event message is not handled by idle procedure
 * @remark	Currently, this Idle Window Procedure is coded for demonstration.
 * @if HISTORY
 *			11-Jan-2006	Joshua created
 * @endif
 */
static int PrvIdleWinProc(WND hwnd, int msg, int wparam, int lparam)
{
//	int i;		//general purposal index
//	RxBITMAP	bmpNemustech;
	MsgType		CreateMsgForMGL = { AWM_CREATE , 0, 0 };
	int pErr;

	CreateMsgForMGL.wparam = (int)hwnd;
	
    switch( msg )
    {	
    case AWM_CREATE:
        PrvCreateMainList(hwnd);
		PrvDrawIdle(hwnd);
		
		if(g_wndMainList!=NULL)
		{
			PrvDrawMainList(g_wndMainList);
		}

		break;

	case AWM_DESTROY:
		CtlDestroy(g_wndMainList, &pErr);
		break;

	case AWM_PAINT:
		if(g_wndMainList)
		{
			PrvDrawIdle(hwnd);
			PrvDrawMainList(g_wndMainList);
		}
		return 1;

	case AWM_TIMER:
		break;

    case AWM_KEYDOWN:
        switch(wparam)
        {
            //UP
        case VK_USER_UP:
            if(g_wndMainList)
            {
                PrvCalcFocus(g_wndMainList, -1);
            }
            break;

            //DOWN
        case VK_USER_DOWN:
            if(g_wndMainList)
            {
                PrvCalcFocus(g_wndMainList, 1);
            }
            break;

//        // Please don't erase below codes ---------
//        // since these are valid ones. These codes will check
//        // the validity of core API features.
//        // Please ask Joshua@mococo.com about this if you have any question
//        PrvPushIdleKeyBuffer( wparam );
//        if ( PrvIsIdleKeyBufferTriggered() )
//        {
//			UsrKillTimer(hwnd, TEST01_TIMER );
//            // Goto test mode
//            CoreTest( hwnd , PrvIdleWinProc );
//        }
//        // until here -----------------------------
		case VK_USER_OK:
			{
				int err;
				int nFocus = CtlGeti(g_wndMainList, CTL_LIST_CMD_GET_FOCUS, &err);
				MsgPostMessage( hwnd, AWM_KEYDOWN, VK_PAD_1 + nFocus, 0 );
			}
			break;

		case VK_PAD_1:	// Core Test
			CoreTest( hwnd , PrvIdleWinProc );
			break;
		case VK_PAD_2:	// MGL Samples
			StaGotoState(hwnd, STATE_ID_MGL_DEMO_IDLE, MSG_DESTROY , MSG_CREATE);
            break;
		case VK_PAD_3:	// Tetris
			StaGotoState(hwnd, STATE_ID_TETRIS, MSG_DESTROY, MSG_CREATE);
			break;
		/*
		case VK_PAD_2:	// Control Test
			//StaGotoState(hwnd, STATE_ID_CTL_TEST, MSG_DESTROY, MSG_CREATE );
			break;
		case VK_PAD_3:	// Biorhythm
			StaGotoState(hwnd, STATE_ID_BIORHYTHM, MSG_DESTROY, MSG_CREATE);
            break;
		case VK_PAD_5:	// Browser
			{
				// wparam 1 will force browser to open default url , "d:\index" folder
				MsgType MsgForBrowser = { AWM_CREATE, 1, 0 };	
				//StaGotoState(hwnd, STATE_ID_BROWSER, MSG_DESTROY, MsgForBrowser);
			}
            break;

		case VK_PAD_7:	// Window Demo
			//StaGotoState(hwnd, STATE_ID_WINDEMO, MSG_DESTROY, MSG_CREATE);
			break;
		case VK_PAD_8:	// Image viewer
			//StaGotoState(hwnd, STATE_ID_IMGVIEWER, MSG_DESTROY, MSG_CREATE);
			break;
		case VK_PAD_9:	// Image viewer
			//StaGotoState(hwnd, STATE_ID_DB_TEST, MSG_DESTROY, MSG_CREATE);
			break;
		case VK_PAD_0:	// Image viewer
			//StaGotoState(hwnd, STATE_ID_MAINMENU_ANIBMP, MSG_DESTROY, MSG_CREATE);
			break;
		*/
		case VK_PAD_4:
			MsgPostQuitMessage(0);
			break;

		}

        return 1;
	default:
		break;
	}
	return 1;
}


/**
* @brief		
* @param[in]	hwnd		
* @param[in]	prevProc		
* @return		
* @remarks		
* @see			
* @if HISTORY
*				11-Jan-2006 madholic Initiated 
* @endif
*/ 
static void CoreTest(struct _window * hwnd, WINPROC prevProc)
{
	// Drawing Core Test Menu
	PrvCTInitMenu();
	PrvCTEventLoop(hwnd);
	prevProc(hwnd, AWM_PAINT, 0, 0);	// Force back application to update its face
}

/** PrvCleanIdleKeyBuffer
*
* @ Created by Joshua on 27-Dec-2005
* @ this key buffer handling is just for demo
* @ but you can adjust this procedures to apply
*	any buffering operation such as
*	emergency calling
*/
static void PrvCleanIdleKeyBuffer(void)
{
    MemSet(gKeyBuffer,0,IDLE_KEY_BUFFER_LENGTH);
}
static void PrvPushIdleKeyBuffer( int keycode )
{
    int i;
    for ( i = 0; i < IDLE_KEY_BUFFER_LENGTH -1 ; i++ )
    {
        gKeyBuffer[i]=gKeyBuffer[i+1];
    }
    gKeyBuffer[IDLE_KEY_BUFFER_LENGTH-1]=keycode;
}
static bool	PrvIsIdleKeyBufferTriggered()
{
    // You can set any desirable trigger here.
    // We will set a trigger for testing harness.

    if ( StrCmp(gKeyBuffer,DEBUG_ENTERING_CODE) == 0 ) return TRUE;
    else return FALSE;
}


//////////////////////////////////////////////////////////////////////////
static void PrvCTDrawMenu(struct _window * hwnd, int PageNo )
{
	// Erase whole screen
	COLORVAL prevFrontColor,prevBackColor;
	int i;
	int KeyNum = 0;
	char strIndex[3];

	GdiLockScreen(hwnd);
	GdiClrScreen(hwnd,CT_BACK_COLOR);
	
	prevFrontColor = GdiSetFgColor(hwnd, CT_FRONT_COLOR);
	prevBackColor  = GdiSetBkColor(hwnd,CT_BACK_COLOR);

    FntSetFont(&FntGlobalContext, FntIDAlpha);
    GdiTextOut(hwnd, "** CORE TEST MENU **", 20, 40, 30);

	// Drawing items
	for ( i = PageNo * CT_ITEM_NUM_PER_PAGE ; i < ( PageNo + 1 ) * CT_ITEM_NUM_PER_PAGE ; i++ )
	{

		if ( i >= MAX_CT_MENU_NUM ) break;

		strIndex[0]=KeyNum+'0';
		strIndex[1]='.';
		strIndex[2]=0;

		GdiTextOut(hwnd, strIndex, StrLen(strIndex) , (rint16)CT_ITEM_LEFT_MARGIN , (rint16)(CT_ITEM_TOP_MARGIN + ( KeyNum -1 )* CT_ITEM_HEIGHT_SPACE) );
		GdiTextOut(hwnd, gCTMenu[i].CTName, StrLen(gCTMenu[i].CTName), (rint16)(CT_ITEM_LEFT_MARGIN+18), (rint16)(CT_ITEM_TOP_MARGIN + ( KeyNum -1 )* CT_ITEM_HEIGHT_SPACE) );
		
		KeyNum++;
	}
	
	// DOLATER
	// Handle Page Up / Down
	
	GdiSetFgColor(hwnd, CT_FRONT_COLOR);
	GdiSetBkColor(hwnd, CT_BACK_COLOR);
	GdiUnlockScreen(hwnd);
	
}
static int PrvCTEventLoop(struct _window * hwnd)
{
     int PressedKey;
     int Selected;

	do {

		PrvCTDrawMenu(hwnd,gCTPageNo);
		PressedKey = PrvCTInputKey();

		Selected = gCTPageNo * CT_ITEM_NUM_PER_PAGE;
		
		switch ( PressedKey )
		{
		case VK_PAD_0:
		     Selected = Selected + 0;
		     break;
		case VK_PAD_1:
		     Selected = Selected + 1;
		     break;
		case VK_PAD_2:
		     Selected = Selected + 2;
		     break;
		case VK_PAD_3:
		     Selected = Selected + 3;
		     break;
		default:
		     continue;
		}

		if ( Selected < MAX_CT_MENU_NUM ) 
		{
		     gCTMenu[Selected].Proc(hwnd);
		}
			
		
	} while (PressedKey != VK_PAD_4 );	//Exit Key Pressed
	
	return -1;
}
static int	PrvCTInputKey()
{
	msg nMsg;
	while ( MsgGetMessage( NULL, &nMsg)  )
	{
		if ( nMsg.wmsg == AWM_KEYDOWN ) 
			return nMsg.wparam;
	}
	return -1;
}
static int PrvCTMarkResult(struct _window * hwnd,char *Question,char *Question2)
{
	int	ScreenHeight;
	int	PressedKey;
	int x,y;
	int y_increasement;

	GdiGetScreenInfo(NULL,&ScreenHeight);
	x = 10;	
	y = ScreenHeight - 60;
	y_increasement = 15;

	if ( Question2 ) y -= y_increasement;

	GdiTextOut(hwnd,Question,StrLen(Question),2,y );
	y += y_increasement;

	if ( Question2 )
	{
		GdiTextOut(hwnd,Question2,StrLen(Question2),2,y );
		y += y_increasement;
	}

	GdiTextOut(hwnd,"0 for No",8,x,y);
	y += y_increasement;
	GdiTextOut(hwnd,"1 for Yes",9,x,y);
	y += y_increasement;
	GdiTextOut(hwnd,"2 for Quit",10,x,y);

	PressedKey = PrvCTInputKey();

	return PressedKey;
}
static int PrvCT_ANI(struct _window * hwnd)
{
	int screenwidth, screenheight;
	int trcscreenheight;
	COLORVAL prevFrontColor;
	COLORVAL prevBackColor;
	int i;

	ResultInfo	TestResult[10];
	int		TestNo=0;

	prevFrontColor	= GdiSetFgColor(hwnd,COLOR_WHITE);
	prevBackColor	= GdiSetBkColor(hwnd,COLOR_BLACK);

	GdiGetScreenInfo( &screenwidth, &screenheight );
	trcscreenheight=screenheight - 80;

	//goto testfromhere;

	/******************************************************************/
	StrCpy(TestResult[TestNo].TestName,"AniCreateCaret");
	/******************************************************************/
	GdiClrScreen(hwnd,COLOR_BLACK);
//	AniCreateCaret(hwnd,12,16,COLOR_BLACK);
//	AniSetCaretPos(50,52);
	
	GdiTextOut(hwnd,"Caret is blinking over this line",32,7,54);
//	AniShowCaret();

	TestResult[TestNo].value=PrvCTMarkResult(hwnd,"Can you see a blinking carret?",NULL);
//	AniDestroyCaret();
	
	if ( TestResult[TestNo].value == VK_PAD_2 ) return -1;	//Canceled
	else
	{
		TestNo++;
	}

	/******************************************************************/
	StrCpy(TestResult[TestNo].TestName,"AniDestroyCaret");
	/******************************************************************/
	
	TestResult[TestNo].value=PrvCTMarkResult(hwnd,"Did the caret disappear correctly?",NULL);
	if ( TestResult[TestNo].value == VK_PAD_2 ) return -1;	//Canceled
	else
	{
		TestNo++;
	}
	
	{
		char tempStr[40];
		
		GdiClrScreen( hwnd, COLOR_BLACK);
		for ( i=0; i< TestNo; i++ )
		{
			if ( TestResult[i].value == VK_PAD_1 )
				StrPrintF(tempStr,"%s..%s",TestResult[i].TestName,"OK");
			else
				StrPrintF(tempStr,"%s..%s",TestResult[i].TestName,"Fail");

			GdiTextOut(hwnd,tempStr,StrLen(tempStr),2,i*16+10);
		}

		GdiTextOut(hwnd, "Press any key to proceed..",26,5,screenheight-30);
		PrvCTInputKey();
	}

	// GDI Test Ends ------------------------------------------------------------------------------------
	// Restore previous drawing environment
	GdiSetFgColor(hwnd,prevFrontColor);
	GdiSetBkColor(hwnd,prevBackColor);	
	return 0;
}

static int PrvCT_RSC(struct _window * hwnd)
{	
	ResultInfo	TestResult[10];
	int			TestNo=0;
	COLORVAL prevFrontColor;
	COLORVAL prevBackColor;
	int		screenwidth=0, screenheight=0;
	int i=0;
	RxBITMAP	bmp1;
	
	/**************************************************************************************/
	/* TEST01 */
	/**************************************************************************************/
	/* head */
	StrCpy(TestResult[TestNo].TestName,"Loading Bitmap");

	GdiClrScreen(hwnd,COLOR_BLACK);
	prevFrontColor	= GdiSetFgColor(hwnd,COLOR_MAGENTA);
	prevBackColor	= GdiSetBkColor(hwnd,COLOR_BLACK);
	FntSetFont(&FntGlobalContext, FntIDAlpha);

	GdiGetScreenInfo( &screenwidth, &screenheight );

	/* body */
	FntSetFont(&FntGlobalContext, FntIDAlpha);

	GdiClrScreen(hwnd, COLOR_BLACK);
	RscLoadBitmap(MGL_back_I, &bmp1);
	GdiDrawBitmap(hwnd, &bmp1, 0, 0, 0, 0, bmp1.width, bmp1.height);
	
	TestResult[TestNo].value=PrvCTMarkResult(hwnd,"Can you see YURI?",NULL);
	if ( TestResult[TestNo].value == VK_PAD_2 ) return -1;	//Canceled
	else
	{
		TestNo++;
	}

	/* tail */
	FntSetFont(&FntGlobalContext, FntIDAlpha);
	GdiSetFgColor(hwnd,prevFrontColor);
	GdiSetBkColor(hwnd,prevBackColor);

	/**************************************************************************************/
	/* TEST RESULT */
	/**************************************************************************************/
	/* head */
	GdiClrScreen(hwnd,COLOR_BLACK);
	prevFrontColor	= GdiSetFgColor(hwnd,COLOR_CYAN);
	prevBackColor	= GdiSetBkColor(hwnd,COLOR_BLACK);
	FntSetFont(&FntGlobalContext, FntIDAlpha);

	{
		char tempStr[40];
		
		GdiClrScreen( hwnd, COLOR_BLACK);
		for ( i=0; i< TestNo; i++ )
		{
			if ( TestResult[i].value == VK_PAD_1 )
				StrPrintF(tempStr,"%s..%s",TestResult[i].TestName,"OK");
			else
				StrPrintF(tempStr,"%s..%s",TestResult[i].TestName,"Fail");

			GdiTextOut(hwnd,tempStr,StrLen(tempStr),2,(rint16)(i*16+10));
		}

		GdiTextOut(hwnd, "Press any key to proceed..",26,5,(rint16)(screenheight-30));
		PrvCTInputKey();
	}

	/* tail */
	FntSetFont(&FntGlobalContext, FntIDAlpha);
	GdiSetFgColor(hwnd,prevFrontColor);
	GdiSetBkColor(hwnd,prevBackColor);

	return 0;
}

static int PrvCT_STR(struct _window * hwnd)
{
	return 0;
}

static int PrvCT_FNT(struct _window * hwnd)
{
	static ResultInfo	TestResult[10];
	static char CharHeight[50];

	int	TestNo=0;

	int x=0, y=0;
	int width=0, height=0;
	int i=0, line_len=0, len=0;
	int screenwidth=0, screenheight=0;

	rect client;
	COLORVAL prevFrontColor;
	COLORVAL prevBackColor;
	WORD code;

	/**************************************************************************************/
	/* TEST01 */
	/**************************************************************************************/
	/* head */
	StrCpy(TestResult[TestNo].TestName,"ShowStringbyType");

	GdiClrScreen(hwnd,COLOR_BLACK);
	prevFrontColor	= GdiSetFgColor(hwnd,COLOR_GREEN);
	prevBackColor	= GdiSetBkColor(hwnd,COLOR_BLACK);
	FntSetFont(&FntGlobalContext, FntIDAlpha);

	x = y = 20;
	GdiGetScreenInfo( &screenwidth, &screenheight );

	/* body */
	FntSetFont(&FntGlobalContext, FntIDAlpha);
	StrCpy(StrBuffer, "ABCDEFGHIJKLMN");

	height = FntGetTextHeight(&FntGlobalContext);
	StrPrintF(CharHeight,"current character height : %d",height);

	GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),x,y);
	y = y + height + 5;

	GdiTextOut(hwnd,CharHeight,StrLen(CharHeight),x,y);
	y = y + height + 5;

	/******************************************************************/
	FntSetFont(&FntGlobalContext, FntIDNumber10);
	StrCpy(StrBuffer, "1234567890");
	
	height = FntGetTextHeight(&FntGlobalContext);
	StrPrintF(CharHeight,"current character height : %d",height);

	GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),x,y);
	y = y + height + 5;

	FntSetFont(&FntGlobalContext, FntIDAlpha);
	GdiTextOut(hwnd,CharHeight,StrLen(CharHeight),x,y);
	y = y + height + 5;

	/******************************************************************/
	FntSetFont(&FntGlobalContext, FntIDNumber16);
	StrCpy(StrBuffer, "1234567890");

	height = FntGetTextHeight(&FntGlobalContext);
	StrPrintF(CharHeight,"current character height : %d",height);

	GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),x,y);
	y = y + height + 5;

	FntSetFont(&FntGlobalContext, FntIDAlpha);
	GdiTextOut(hwnd,CharHeight,StrLen(CharHeight),x,y);
	y = y + height + 5;

	/******************************************************************/
	FntSetFont(&FntGlobalContext, FntIDAlpha);
	
	TestResult[TestNo].value=PrvCTMarkResult(hwnd,"Can you see these string correct?",NULL);
	if ( TestResult[TestNo].value == VK_PAD_2 ) return -1;	//Canceled
	else
	{
		TestNo++;
	}

	/* tail */

	FntSetFont(&FntGlobalContext, FntIDAlpha);
	GdiSetFgColor(hwnd,prevFrontColor);
	GdiSetBkColor(hwnd,prevBackColor);


	/**************************************************************************************/
	/* TEST02 */
	/**************************************************************************************/

	/* head */
	StrCpy(TestResult[TestNo].TestName,"GdiExtTextOut-Style");

	GdiClrScreen(hwnd,COLOR_BLACK);
	prevFrontColor	= GdiSetFgColor(hwnd,COLOR_YELLOW);
	prevBackColor	= GdiSetBkColor(hwnd,COLOR_BLACK);
	FntSetFont(&FntGlobalContext, FntIDAlpha);

	GdiGetScreenInfo( &screenwidth, &screenheight );

	/* body */
	FntSetFont(&FntGlobalContext, FntIDAlpha);
	height = FntGetTextHeight(&FntGlobalContext);

	GdiSetRect( &client, 0, 0, screenwidth, screenheight-60);

	StrCpy(StrBuffer, "Seated on the divan beside Langdon, Sophie drank her tea and ate a scone, feeling the welcome effects of caffeine and food. Sir Leigh Teabing was beaming as he awkwardly paced before the open fire, his leg braces clicking on the stone hearth.");

	len = StrLen(StrBuffer);
	line_len = 0;
	y = 0;

	GdiLockScreen(hwnd);

	for(i=0;i<len;i++)
	{
		code = FntGetCharCode(StrBuffer+i);

		if(line_len + FntGetFontWidth(&FntGlobalContext, code) > screenwidth)
		{
			line_len = 0;
			y += height;
		}

		if(i>=14 && i<19)
			GdiExtTextOutEx(hwnd, (char*)&code, 1, line_len, y, UNDERLINEFONT);
		else if(i>35 && i<42)
			GdiExtTextOutEx(hwnd, (char*)&code, 1, line_len, y, BOLDFONT);
		else if(i>85 && i<93)
			GdiExtTextOutEx(hwnd, (char*)&code, 1, line_len, y, OUTLINEFONT);
		else
			GdiExtTextOutEx(hwnd, (char*)&code, 1, line_len, y, NORMALFONT);

		line_len += FntGetFontWidth(&FntGlobalContext, code);
	}

	GdiUnlockScreen(hwnd);

	TestResult[TestNo].value=PrvCTMarkResult(hwnd,"Can you see this string proper?", NULL);
	if ( TestResult[TestNo].value == VK_PAD_2 ) return -1;	//Canceled
	else
	{
		TestNo++;
	}

	/* tail */
	FntSetFont(&FntGlobalContext, FntIDAlpha);
	GdiSetFgColor(hwnd,prevFrontColor);
	GdiSetBkColor(hwnd,prevBackColor);

	/**************************************************************************************/
	/* TEST03 */
	/**************************************************************************************/
	/* head */
	StrCpy(TestResult[TestNo].TestName,"TextOut Vertical");
	
	GdiClrScreen(hwnd,COLOR_BLACK);
	prevFrontColor	= GdiSetFgColor(hwnd,COLOR_GREEN);
	prevBackColor	= GdiSetBkColor(hwnd,COLOR_BLACK);
	FntSetFont(&FntGlobalContext, FntIDAlpha);

	GdiGetScreenInfo( &screenwidth, &screenheight );

	/* body */
	GdiLockScreen(hwnd);

	GdiSetFgColor(hwnd, COLOR_CYAN);
	GdiClrScreen( hwnd, COLOR_BLACK );

	FntSetFont(&FntGlobalContext, FntIDAlpha);
	height = FntGetTextHeight(&FntGlobalContext);

	for(i=0;i<27;i++)
	{
		StrPrintF(StrBuffer, "[%02d] From journeyman to master !", i);
		GdiTextOut(hwnd, StrBuffer, StrLen(StrBuffer), (rint16)(-i), (rint16)(i*height-5));
	}

	GdiUnlockScreen(hwnd);

	TestResult[TestNo].value=PrvCTInputKey();
	if ( TestResult[TestNo].value == VK_PAD_2 ) return -1;	//Canceled
	else
	{
		TestNo++;
	}

	/* tail */
	FntSetFont(&FntGlobalContext, FntIDAlpha);
	GdiSetFgColor(hwnd,prevFrontColor);
	GdiSetBkColor(hwnd,prevBackColor);

	/**************************************************************************************/
	/* TEST04 */
	/**************************************************************************************/
	/* head */
	StrCpy(TestResult[TestNo].TestName,"Character Spacing Test");

	GdiClrScreen(hwnd,COLOR_BLACK);
	prevFrontColor	= GdiSetFgColor(hwnd,COLOR_WHITE);
	prevBackColor	= GdiSetBkColor(hwnd,COLOR_BLACK);
	FntSetFont(&FntGlobalContext, FntIDAlpha);

	GdiGetScreenInfo( &screenwidth, &screenheight );
	y = 10;

	/* body */
	height = FntGetTextHeight(&FntGlobalContext);

	//

	StrCpy(StrBuffer, "character spacing : 0");
	GdiExtTextOutEx(hwnd, StrBuffer, StrLen(StrBuffer), 0, y, NORMALFONT);
	y+=height+10;

	StrCpy(StrBuffer, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	GdiSetRect(&client, -1, y-5, screenwidth, y + height+5);

	GdiSetFgColor(hwnd,COLOR_RED);
	GdiRectangle(hwnd, &client);
	GdiSetFgColor(hwnd,COLOR_WHITE);
	GdiPaintString(hwnd, 5, y, StrBuffer, StrLen(StrBuffer), 0);
	y+=(height*2);

	//
	StrCpy(StrBuffer, "character spacing : 1");
	GdiExtTextOutEx(hwnd, StrBuffer, StrLen(StrBuffer), 0, y, NORMALFONT);
	y+=height+10;

	StrCpy(StrBuffer, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	GdiSetRect(&client, -1, y-5, screenwidth, y + height+5);

	GdiSetFgColor(hwnd,COLOR_RED);
	GdiRectangle(hwnd, &client);
	GdiSetFgColor(hwnd,COLOR_WHITE);
	GdiPaintString(hwnd, 5, y, StrBuffer, StrLen(StrBuffer), 1);
	y+=(height*2);


	//
	StrCpy(StrBuffer, "character spacing : 2");
	GdiExtTextOutEx(hwnd, StrBuffer, StrLen(StrBuffer), 0, y, NORMALFONT);
	y+=height+10;

	StrCpy(StrBuffer, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	GdiSetRect(&client, -1, y-5, screenwidth, y + height+5);

	GdiSetFgColor(hwnd,COLOR_RED);
	GdiRectangle(hwnd, &client);
	GdiSetFgColor(hwnd,COLOR_WHITE);
	GdiPaintString(hwnd, 5, y, StrBuffer, StrLen(StrBuffer), 2);
	y+=(height*2);


	//
	StrCpy(StrBuffer, "character spacing : 3");
	GdiExtTextOutEx(hwnd, StrBuffer, StrLen(StrBuffer), 0, y, NORMALFONT);
	y+=height+10;

	StrCpy(StrBuffer, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	GdiSetRect(&client, -1, y-5, screenwidth, y + height+5);

	GdiSetFgColor(hwnd,COLOR_RED);
	GdiRectangle(hwnd, &client);
	GdiSetFgColor(hwnd,COLOR_WHITE);
	GdiPaintString(hwnd, 5, y, StrBuffer, StrLen(StrBuffer), 3);
	y+=(height*2);

	//
	StrCpy(StrBuffer, "character spacing : 4");
	GdiExtTextOutEx(hwnd, StrBuffer, StrLen(StrBuffer), 0, y, NORMALFONT);
	y+=height+10;

	StrCpy(StrBuffer, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	GdiSetRect(&client, -1, y-5, screenwidth, y + height+5);

	GdiSetFgColor(hwnd,COLOR_RED);
	GdiRectangle(hwnd, &client);
	GdiSetFgColor(hwnd,COLOR_WHITE);
	GdiPaintString(hwnd, 5, y, StrBuffer, StrLen(StrBuffer), 4);
	y+=(height*2);


	TestResult[TestNo].value=PrvCTMarkResult(hwnd,"Is character space correct?",NULL);
	if ( TestResult[TestNo].value == VK_PAD_2 ) return -1;	//Canceled
	else
	{
		TestNo++;
	}

	/* tail */
	FntSetFont(&FntGlobalContext, FntIDAlpha);
	GdiSetFgColor(hwnd,prevFrontColor);
	GdiSetBkColor(hwnd,prevBackColor);

	/**************************************************************************************/
	/* TEST RESULT */
	/**************************************************************************************/
	/* head */
	GdiClrScreen(hwnd,COLOR_BLACK);
	prevFrontColor	= GdiSetFgColor(hwnd,COLOR_CYAN);
	prevBackColor	= GdiSetBkColor(hwnd,COLOR_BLACK);
	FntSetFont(&FntGlobalContext, FntIDAlpha);

	GdiGetScreenInfo( &screenwidth, &screenheight );

	{
		char tempStr[40];
		
		GdiClrScreen( hwnd, COLOR_BLACK);
		for ( i=0; i< TestNo; i++ )
		{
			if ( TestResult[i].value == VK_PAD_1 )
				StrPrintF(tempStr,"%s..%s",TestResult[i].TestName,"OK");
			else
				StrPrintF(tempStr,"%s..%s",TestResult[i].TestName,"Fail");

			GdiTextOut(hwnd,tempStr,StrLen(tempStr),2,(rint16)(i*16+10));
		}

		GdiTextOut(hwnd, "Press any key to proceed..",26,5,(rint16)(screenheight-30));
		PrvCTInputKey();
	}

	/* tail */
	FntSetFont(&FntGlobalContext, FntIDAlpha);
	GdiSetFgColor(hwnd,prevFrontColor);
	GdiSetBkColor(hwnd,prevBackColor);

	return 0;
}
static int PrvCT_TMR(struct _window * hwnd)
{
	int	AllowedTimerErrorPercent = 10;	// Timer should be puctual with 90 % trust at least
	int screenwidth, screenheight;
	int trcscreenheight;
	COLORVAL prevFrontColor;
	COLORVAL prevBackColor;
	int i;
	unsigned long	CurrentTicks;
	unsigned long	PrevTicks;
	ResultInfo	TestResult[10];
	int		TestNo=0;

	prevFrontColor	= GdiSetFgColor(hwnd,COLOR_WHITE);
	prevBackColor	= GdiSetBkColor(hwnd,COLOR_BLACK);

	GdiGetScreenInfo( &screenwidth, &screenheight );
	trcscreenheight=screenheight - 80;


	/******************************************************************/
	StrCpy(TestResult[TestNo].TestName,"UsrGetCurrentTicks");
	/******************************************************************/
	GdiClrScreen(hwnd,COLOR_BLACK);
	{
		ruint16 second=0;
		CurrentTicks = 0;
		PrevTicks = 0;
		do {
			CurrentTicks = UsrGetCurrentTicks();
			
			GdiLockScreen(hwnd);

			StrPrintF(StrBuffer,"Currnet Ticks : %ld",CurrentTicks);
			GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),20,20);

			GdiTextOut(hwnd,"Calculated elapsed seconds",26,20,70);
			StrPrintF(StrBuffer,"from ticks : %d",second);
			GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),20,90);
			
			GdiUnlockScreen(hwnd);

			if ( PrevTicks )
			{
				if ( CurrentTicks - PrevTicks >= 1000 )
				{
					PrevTicks = CurrentTicks;
					second++;
				}
			}
			else
				PrevTicks = CurrentTicks;

		} while( second < 6);
	}
	TestResult[TestNo].value=PrvCTMarkResult(hwnd,"Can you get seconds correctly?",NULL);
	if ( TestResult[TestNo].value == VK_PAD_2 ) return -1;	//Canceled
	else
	{
		TestNo++;
	}

testfromhere:

	/******************************************************************/
	StrCpy(TestResult[TestNo].TestName,"UseSetTimer-Alone");
	/******************************************************************/
	{
		msg nMsg;
		rect rtRect; 
		ruint32 testInterval = 1000;	//should be less than 5000

		GdiClrScreen( hwnd, COLOR_BLACK );

		StrPrintF(StrBuffer,"Press any key to set");
		GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,30);
		StrPrintF(StrBuffer,"%d second interval timer..",testInterval / 1000);
		GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,50);
		StrPrintF(StrBuffer,"[Alone]");
		GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,10);

		PrvCTInputKey();

		PrevTicks = UsrGetCurrentTicks();
		UsrSetTimer(hwnd,REXY_APPLICATION_TIMER+1,testInterval);

		StrPrintF(StrBuffer,"Timer was set at %d ticks",PrevTicks);
		GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,70);

		rtRect.left = 10;
		rtRect.right = screenwidth - 40;
		rtRect.top = 100;
		rtRect.bottom = 110;

		GdiRectangle(hwnd,&rtRect);

		rtRect.top+=2;
		rtRect.bottom-=2;
		rtRect.left+=2;

		while ( MsgGetMessage( NULL, &nMsg)  )
		{

			CurrentTicks = UsrGetCurrentTicks();
			if ( CurrentTicks > PrevTicks + 5000 )
			{
			     UsrKillTimer(hwnd,REXY_APPLICATION_TIMER+1);
			     StrPrintF(StrBuffer,"couldn't receive within 5 sec");
			     GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,130);
			     TestResult[TestNo].value=VK_PAD_0;
			     break;
			}

			rtRect.right = ((CurrentTicks - PrevTicks)* ( screenwidth - 54 ) / testInterval)  + rtRect.left;
			GdiFillRect(hwnd,&rtRect,COLOR_YELLOW);
			
			if ( nMsg.wmsg == AWM_TIMER && nMsg.wparam == REXY_APPLICATION_TIMER+1 )
			{
				int accuracy;

				UsrKillTimer(hwnd,REXY_APPLICATION_TIMER+1);

				StrPrintF(StrBuffer,"Received Timer event at %d",CurrentTicks);
				GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,130);

				accuracy = ( CurrentTicks - PrevTicks ) * 100 / testInterval;

				if ( accuracy > 100 + AllowedTimerErrorPercent || accuracy < 100 - AllowedTimerErrorPercent )
				{
					TestResult[TestNo].value = VK_PAD_0;
				}
				else
				{
					TestResult[TestNo].value = VK_PAD_1;
				}

				StrPrintF(StrBuffer,"Accuracy %d %% .",accuracy);

				GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,150);
				break;
			}
		}

		{
			StrPrintF(StrBuffer,"Press any key to continue..");
			GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,180);
			PrvCTInputKey();
			TestNo++;
		}
	}	
	/******************************************************************/
	StrCpy(TestResult[TestNo].TestName,"UseSetTimer-with100ms");
	/******************************************************************/
	{
		msg nMsg;
		rect rtRect; 
		ruint32 testInterval = 3000;	//should be less than 5000
		rect rtRed;
		bool rtRedOn=FALSE;

		GdiClrScreen( hwnd, COLOR_BLACK );

		StrPrintF(StrBuffer,"Press any key to set");
		GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,30);
		StrPrintF(StrBuffer,"3 second interval timer..");
		GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,50);
		StrPrintF(StrBuffer,"[with another 100ms timer]");
		GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,10);

		PrvCTInputKey();

		PrevTicks = UsrGetCurrentTicks();
		UsrSetTimer(hwnd,REXY_APPLICATION_TIMER+1,testInterval);
		UsrSetTimer(hwnd,REXY_APPLICATION_TIMER+2,100);	//mischievous timer


		StrPrintF(StrBuffer,"Timer was set at %d ticks",PrevTicks);
		GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,70);

		rtRect.left = 10;
		rtRect.right = screenwidth - 40;
		rtRect.top = 100;
		rtRect.bottom = 110;

		rtRed.left=screenwidth-10;
		rtRed.right=screenwidth-1;
		rtRed.top=0;
		rtRed.bottom=9;

		GdiRectangle(hwnd,&rtRect);

		rtRect.top+=2;
		rtRect.bottom-=2;
		rtRect.left+=2;

		while ( MsgGetMessage( NULL, &nMsg)  )
		{

			CurrentTicks = UsrGetCurrentTicks();
			if ( PrevTicks > PrevTicks + 5000 )
			{
				StrPrintF(StrBuffer,"couldn't receive within 5 sec");
				GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,130);
				TestResult[TestNo].value=VK_PAD_0;
				break;
			}

			rtRect.right = ((CurrentTicks - PrevTicks)* ( screenwidth -54 ) / testInterval)  + rtRect.left;
			GdiFillRect(hwnd,&rtRect,COLOR_YELLOW);
			
			if ( nMsg.wmsg == AWM_TIMER && nMsg.wparam == REXY_APPLICATION_TIMER+1 )
			{
				int accuracy;

				UsrKillTimer(hwnd,REXY_APPLICATION_TIMER+1);
				UsrKillTimer(hwnd,REXY_APPLICATION_TIMER+2);

				StrPrintF(StrBuffer,"Received Timer event at %d",CurrentTicks);
				GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,130);

				accuracy = ( CurrentTicks - PrevTicks ) * 100 / testInterval;

				if ( accuracy > 100 + AllowedTimerErrorPercent || accuracy < 100 - AllowedTimerErrorPercent )
				{
					TestResult[TestNo].value = VK_PAD_0;
				}
				else
				{
					TestResult[TestNo].value = VK_PAD_1;
				}
				
				StrPrintF(StrBuffer,"Accuracy %d %% .",accuracy);

				GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,150);
				break;
			}
			if ( nMsg.wmsg == AWM_TIMER && nMsg.wparam == REXY_APPLICATION_TIMER+2 )
			{
				if ( rtRedOn )
				{
					rtRedOn = FALSE;
					GdiFillRect(hwnd,&rtRed,COLOR_BLACK);
				}
				else
				{
					rtRedOn = TRUE;
					GdiFillRect(hwnd,&rtRed,COLOR_RED);
				}
			}
		}
		{
			StrPrintF(StrBuffer,"Press any key to continue..");
			GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,180);
			PrvCTInputKey();
			TestNo++;
		}
	}	
	/******************************************************************/
	StrCpy(TestResult[TestNo].TestName,"UseSetTimer-with50ms");
	/******************************************************************/
	{
		msg nMsg;
		rect rtRect; 
		ruint32 testInterval = 3000;	//should be less than 5000
		rect rtRed;
		bool rtRedOn=FALSE;

		GdiClrScreen( hwnd, COLOR_BLACK );

		StrPrintF(StrBuffer,"Press any key to set");
		GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,30);
		StrPrintF(StrBuffer,"3 second interval timer..");
		GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,50);
		StrPrintF(StrBuffer,"[with another 50ms timer]");
		GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,10);

		PrvCTInputKey();

		PrevTicks = UsrGetCurrentTicks();
		UsrSetTimer(hwnd,REXY_APPLICATION_TIMER+1,testInterval);
		UsrSetTimer(hwnd,REXY_APPLICATION_TIMER+2,50);	//mischievous timer

		StrPrintF(StrBuffer,"Timer was set at %d ticks",PrevTicks);
		GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,70);

		rtRect.left = 10;
		rtRect.right = screenwidth - 40;
		rtRect.top = 100;
		rtRect.bottom = 110;

		rtRed.left=screenwidth-10;
		rtRed.right=screenwidth-1;
		rtRed.top=0;
		rtRed.bottom=9;

		GdiRectangle(hwnd,&rtRect);

		rtRect.top+=2;
		rtRect.bottom-=2;
		rtRect.left+=2;

		while ( MsgGetMessage( NULL, &nMsg)  )
		{

			CurrentTicks = UsrGetCurrentTicks();
			if ( PrevTicks > PrevTicks + 5000 )
			{
				StrPrintF(StrBuffer,"couldn't receive within 5 sec");
				GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,130);
				TestResult[TestNo].value=VK_PAD_0;
				break;
			}

			rtRect.right = ((CurrentTicks - PrevTicks)* ( screenwidth -54 ) / testInterval)  + rtRect.left;
			GdiFillRect(hwnd,&rtRect,COLOR_YELLOW);
			
			if ( nMsg.wmsg == AWM_TIMER && nMsg.wparam == REXY_APPLICATION_TIMER+1 )
			{
				int accuracy;

				UsrKillTimer(hwnd,REXY_APPLICATION_TIMER+1);
				UsrKillTimer(hwnd,REXY_APPLICATION_TIMER+2);
				
				StrPrintF(StrBuffer,"Received Timer event at %d",CurrentTicks);
				GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,130);

				accuracy = ( CurrentTicks - PrevTicks ) * 100 / testInterval;

				if ( accuracy > 100 + AllowedTimerErrorPercent || accuracy < 100 - AllowedTimerErrorPercent )
				{
					TestResult[TestNo].value = VK_PAD_0;
				}
				else
				{
					TestResult[TestNo].value = VK_PAD_1;
				}

				
				StrPrintF(StrBuffer,"Accuracy %d %% .",accuracy);

				GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,150);
				break;
			}
			if ( nMsg.wmsg == AWM_TIMER && nMsg.wparam == REXY_APPLICATION_TIMER+2 )
			{
				if ( rtRedOn )
				{
					rtRedOn = FALSE;
					GdiFillRect(hwnd,&rtRed,COLOR_BLACK);
				}
				else
				{
					rtRedOn = TRUE;
					GdiFillRect(hwnd,&rtRed,COLOR_RED);
				}
			}
		}
		{
			StrPrintF(StrBuffer,"Press any key to continue..");
			GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),10,180);
			PrvCTInputKey();
			TestNo++;
		}
	}

	UsrKillTimer(hwnd,REXY_APPLICATION_TIMER+1);
	UsrKillTimer(hwnd,REXY_APPLICATION_TIMER+2);

	{
		char tempStr[40];
		
		GdiClrScreen( hwnd, COLOR_BLACK);
		for ( i=0; i< TestNo; i++ )
		{
			if ( TestResult[i].value == VK_PAD_1 )
				StrPrintF(tempStr,"%s..%s",TestResult[i].TestName,"OK");
			else
				StrPrintF(tempStr,"%s..%s",TestResult[i].TestName,"Fail");

			GdiTextOut(hwnd,tempStr,StrLen(tempStr),2,(rint16)(i*16+10));
		}

		GdiTextOut(hwnd, "Press any key to proceed..",26,5,(rint16)(screenheight-30));
		PrvCTInputKey();
	}

	// GDI Test Ends ------------------------------------------------------------------------------------
	// Restore previous drawing environment
	GdiSetFgColor(hwnd,prevFrontColor);
	GdiSetBkColor(hwnd,prevBackColor);	
	return 0;
}
static int PrvCT_GDI(struct _window * hwnd)
{
	rect	client;
	int screenwidth, screenheight;
	int trcscreenheight;
	COLORVAL prevFrontColor;
	COLORVAL prevBackColor;
	COLORVAL tempColor;
	ruint16 x1,y1,x2,y2;
	int i;
	UNITCL	*pScreenBuffer;

	ResultInfo	TestResult[15];
	int		TestNo=0;

	prevFrontColor	= GdiSetFgColor(hwnd,COLOR_WHITE);
	prevBackColor	= GdiSetBkColor(hwnd,COLOR_BLACK);
	FntSetFont(&FntGlobalContext, FntIDAlpha);
	
	GdiGetScreenInfo( &screenwidth, &screenheight );
	trcscreenheight=screenheight - 80;

	//goto testfromhere;

	/******************************************************************/
	StrCpy(TestResult[TestNo].TestName,"GdiGetScreenInfo");
	/******************************************************************/
	GdiClrScreen(hwnd,COLOR_BLACK);
	StrPrintF(StrBuffer,"Screen Width : %d",screenwidth);
	GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),20,20);
	StrPrintF(StrBuffer,"Screen Height: %d",screenheight);
	GdiTextOut(hwnd,StrBuffer,StrLen(StrBuffer),20,40);

	TestResult[TestNo].value=PrvCTMarkResult(hwnd,"Is screen size correct?",NULL);
	if ( TestResult[TestNo].value == VK_PAD_2 ) return -1;	//Canceled
	else
	{
		TestNo++;
	}

	/******************************************************************/
	StrCpy(TestResult[TestNo].TestName,"GdiFillRect & Position");
	/******************************************************************/
	GdiClrScreen( hwnd, COLOR_BLACK );
	GdiSetRect( &client, 0, 0, screenwidth-1, screenheight-1 );
    GdiFillRect(hwnd, &client, COLOR_YELLOW);
	GdiSetRect( &client, 1, 1, screenwidth-2, screenheight-2 );
    GdiFillRect(hwnd, &client, COLOR_BLACK);
	TestResult[TestNo].value=PrvCTMarkResult(hwnd,"Can you see a seamless","yellow 1 pixel width frame?");
	if ( TestResult[TestNo].value == VK_PAD_2 ) return -1;	//Canceled
	else
	{
		TestNo++;
	}
	
	/******************************************************************/
	StrCpy(TestResult[TestNo].TestName,"GdiDrawLine Horizontal");
	/******************************************************************/
	GdiClrScreen( hwnd, COLOR_BLACK );
	for ( i = 0; i < screenwidth ; i+=2 )
	{
		GdiDrawLine(hwnd,0,(ruint16)i,(ruint16)(screenwidth-1),(ruint16)i);
	}
	TestResult[TestNo].value=PrvCTMarkResult(hwnd,"Can you see a horizontal","white Lines?");
	if ( TestResult[TestNo].value == VK_PAD_2 ) return -1;	//Canceled
	else
	{
		TestNo++;
	}


	/******************************************************************/
	StrCpy(TestResult[TestNo].TestName,"GdiDrawLine Vertical");
	/******************************************************************/
	GdiClrScreen( hwnd, COLOR_BLACK );
	for ( i = 0; i < screenwidth ; i+=2 )
	{
		GdiDrawLine(hwnd,(ruint16)i,0,(ruint16)i,(ruint16)(trcscreenheight-1));
	}
	TestResult[TestNo].value=PrvCTMarkResult(hwnd,"Can you see a vertical","white Lines?");
	if ( TestResult[TestNo].value == VK_PAD_2 ) return -1;	//Canceled
	else
	{
		TestNo++;
	}

	/******************************************************************/
	StrCpy(TestResult[TestNo].TestName,"GdiDrawLine Diagonal");
	/******************************************************************/
	GdiClrScreen( hwnd, COLOR_BLACK );
	for ( i = 0; i < screenwidth ; i+=4 )
	{
		GdiDrawLine(hwnd,(ruint16)i,0,0,(ruint16)i);
		GdiDrawLine(hwnd,(ruint16)(screenwidth -1),(ruint16)(i+1),(ruint16)(i+1),(ruint16)(screenwidth-1));
	}
	TestResult[TestNo].value=PrvCTMarkResult(hwnd,"Can you see a diagonal","white Lines?");
	if ( TestResult[TestNo].value == VK_PAD_2 ) return -1;	//Canceled
	else
	{
		TestNo++;
	}

	/******************************************************************/
	StrCpy(TestResult[TestNo].TestName,"GdiDrawLine Rotational");
	/******************************************************************/
	GdiClrScreen( hwnd, COLOR_BLACK );
	for ( i = 0; i < 180 ; i+=2 )
	{
		x1 = (ruint16) (cos(ToRadian(i))*(screenwidth/2) + (screenwidth/2));
		x2 = screenwidth - x1; 
		y1 = (ruint16)(( trcscreenheight/2 ) - sin(ToRadian(i))*(trcscreenheight/2));
		y2 = trcscreenheight - y1;
		GdiDrawLine(hwnd,x1,y1,x2,y2);
	}
	TestResult[TestNo].value=PrvCTMarkResult(hwnd,"Could you see a rotating","white line?");
	if ( TestResult[TestNo].value == VK_PAD_2 ) return -1;	//Canceled
	else
	{
		TestNo++;
	}

	/******************************************************************/
	StrCpy(TestResult[TestNo].TestName,"GdiDrawLineWidth_Hor");
	/******************************************************************/
	GdiClrScreen( hwnd, COLOR_BLACK );
	for ( i = 1; i <= 10 ; i++ )
	{
		GdiDrawLineWidth(hwnd,0,(ruint16)(i*20),(ruint16)(screenwidth-1),(ruint16)(i*20),i);
	}
	TestResult[TestNo].value=PrvCTMarkResult(hwnd,"Could you see a width increasing ","10 different lines?");
	if ( TestResult[TestNo].value == VK_PAD_2 ) return -1;	//Canceled
	else
	{
		TestNo++;
	}

	/******************************************************************/
	StrCpy(TestResult[TestNo].TestName,"GdiDrawLineWidth_Ver");
	/******************************************************************/
	GdiClrScreen( hwnd, COLOR_BLACK );
	for ( i = 1; i <= 10 ; i++ )
	{
		GdiDrawLineWidth(hwnd,(ruint16)(i*20),0,(ruint16)(i*20),(ruint16)(trcscreenheight-1),i);
	}
	TestResult[TestNo].value=PrvCTMarkResult(hwnd,"Could you see a width increasing ","10 different lines?");
	if ( TestResult[TestNo].value == VK_PAD_2 ) return -1;	//Canceled
	else
	{
		TestNo++;
	}


	/******************************************************************/
	StrCpy(TestResult[TestNo].TestName,"GdiDrawLineWidth_Dia");
	/******************************************************************/
	GdiClrScreen( hwnd, COLOR_BLACK );
	for ( i = 1; i <= 10 ; i++ )
	{
		GdiDrawLineWidth(hwnd,(ruint16)(i*20),0,0,(ruint16)(i*20),i);
	}
	TestResult[TestNo].value=PrvCTMarkResult(hwnd,"Could you see a width increasing ","10 different lines?");
	if ( TestResult[TestNo].value == VK_PAD_2 ) return -1;	//Canceled
	else
	{
		TestNo++;
	}


	/******************************************************************/
	StrCpy(TestResult[TestNo].TestName,"GdiDrawColorLine");
	/******************************************************************/
	GdiClrScreen( hwnd, COLOR_BLACK );
	
	for ( i = 0; i < screenwidth ; i++ )
	{
		if ( i <= screenwidth / 3 )
		{
			tempColor = MAKERGB((i*255)/(screenwidth/3),0,0);
		}
		else if ( i <= screenwidth * 2 / 3)
		{
			tempColor = MAKERGB(255,(i*255)/(screenwidth/3),0);
		}
		else
			tempColor = MAKERGB(255,255,(i*255)/(screenwidth/3));

		GdiDrawColorLine(hwnd,(ruint16)i,0,(ruint16)i,(ruint16)trcscreenheight,tempColor);
	}
	TestResult[TestNo].value=PrvCTMarkResult(hwnd,"Could you see a black-red-yellow ","-white spectrum?");
	if ( TestResult[TestNo].value == VK_PAD_2 ) return -1;	//Canceled
	else
	{
		TestNo++;
	}

	/******************************************************************/
	StrCpy(TestResult[TestNo].TestName,"GdiInvertRect");
	/******************************************************************/
	{
		rect	rtTemp;
		int		sizeTemp;

		rtTemp.top = trcscreenheight;
		rtTemp.bottom = screenheight-1;
		rtTemp.left=0;
		rtTemp.right=screenwidth-1;

		GdiFillRect(hwnd,&rtTemp,COLOR_BLACK);	//erase comment area

		rtTemp.left = screenwidth / 3;
		rtTemp.right = ( screenwidth * 2 / 3);
		rtTemp.top = trcscreenheight / 3;
		rtTemp.bottom = ( trcscreenheight * 2 / 3);

		client = rtTemp;

		sizeTemp = GdiScreenGetMemSizeArea(&client);
		pScreenBuffer = MemAlloc(sizeTemp);
		GdiScreenSave(&client, pScreenBuffer);	//save the current screen
		GdiInvertRect( hwnd, &rtTemp );
	}
	TestResult[TestNo].value=PrvCTMarkResult(hwnd,"Could you see a inverted blue","rectangle in the middle of screen?");

	GdiScreenRestore(pScreenBuffer);
	MemFree( pScreenBuffer );

	if ( TestResult[TestNo].value == VK_PAD_2 ) return -1;	//Canceled
	else
	{
		TestNo++;
	}


	/******************************************************************/
	StrCpy(TestResult[TestNo].TestName,"GdiGetMemSize/Save/LoadScreenArea");
	/******************************************************************/
	{
		rect	rtTemp;
		
		rtTemp.top = trcscreenheight;
		rtTemp.bottom = screenheight-1;
		rtTemp.left=0;
		rtTemp.right=screenwidth-1;

		GdiFillRect(hwnd,&rtTemp,COLOR_BLACK);	//erase comment area

	}
	TestResult[TestNo].value=PrvCTMarkResult(hwnd,"Did the previous inverted","rectangle disappear correctly?");
	if ( TestResult[TestNo].value == VK_PAD_2 ) return -1;	//Canceled
	else
	{
		char tempStr[40];
		
		GdiClrScreen( hwnd, COLOR_BLACK);
		for ( i=0; i< TestNo; i++ )
		{
			if ( TestResult[i].value == VK_PAD_1 )
				StrPrintF(tempStr,"%s..%s",TestResult[i].TestName,"OK");
			else
				StrPrintF(tempStr,"%s..%s",TestResult[i].TestName,"Fail");

			GdiTextOut(hwnd,tempStr,StrLen(tempStr),2,(rint16)(i*16+10));
		}

		GdiTextOut(hwnd, "Press any key to proceed..",26,5,(rint16)(screenheight-30));
		PrvCTInputKey();
	}

	// GDI Test Ends ------------------------------------------------------------------------------------
	// Restore previous drawing environment
	GdiSetFgColor(hwnd,prevFrontColor);
	GdiSetBkColor(hwnd,prevBackColor);	
	return 0;
}
static void PrvCTInitMenu()
{
	int i=0;
	StrCpy(gCTMenu[i].CTName, "GDI Test");
	gCTMenu[i].Proc = PrvCT_GDI;

	i++;
	StrCpy(gCTMenu[i].CTName, "Font Test");
	gCTMenu[i].Proc = PrvCT_FNT;

//	i++;
//	StrCpy(gCTMenu[i].CTName, "Animation Object Test");
//	gCTMenu[i].Proc = PrvCT_ANI;

	i++;
	StrCpy(gCTMenu[i].CTName, "Timer Test");
	gCTMenu[i].Proc = PrvCT_TMR;
	i++;
	StrCpy(gCTMenu[i].CTName, "Resource Test");
	gCTMenu[i].Proc = PrvCT_RSC;

	i++;
	StrCpy(gCTMenu[i].CTName, "Exit");
	gCTMenu[i].Proc = PrvCT_GDI;
}

static void PrvCreateMainList(WND wnd)
{
	WND w;
    int err,i;
	int img_num = 0;
	int width, height;

	REXY_CTL_LIST *pCtlList;

	rect rtControlPos;
	rect margin_text = {8, 8, 8, 8};
	rect margin_img  = {2, 2, 2, 2};

	GdiGetScreenInfo( &width, &height ); // We are going to use whole screen

    w = CtlCreate(wnd, CTL_TYPE_LIST, 0, NULL, &err);
	g_wndMainList = w;
	
	pCtlList = (REXY_CTL_LIST *)CtlGetStruct(w);

	// set control window position
	GdiSetRect(&rtControlPos, 10, 140, width - 10, height - 64);
    CtlSetPtr(w, CTL_LIST_CMD_SET_CTRL_POS, (void*)&rtControlPos, &err);

	// set list control attribute
    CtlSetPtr(w, CTL_LIST_CMD_SET_DRAW_ITEM_FOCUS_CB, (void*)PrvDrawListItemFocus, &err);
//	CtlSetPtr(w, CTL_LIST_CMD_SET_DRAW_ITEM_CB, (void*)PrvDrawListItem, &err);
    CtlSetPtr(w, CTL_LIST_CMD_SET_DRAW_COLUMN_CB, (void*)PrvDrawListColumn, &err);
    CtlSetPtr(w, CTL_LIST_CMD_SET_KEY_DOWN_CB, (void*)PrvProcessKeyDown, &err);

	CtlSeti(w, CTL_LIST_CMD_SET_FONT, FntIDAlpha, &err);
	CtlSeti(w, CTL_LIST_CMD_SET_CTRL_BGCOLOR, COLOR_WHITE, &err);
	
	CtlSeti(w, CTL_LIST_CMD_SET_LINE_HEIGHT, 28, &err);

    CtlSeti(w, CTL_LIST_CMD_SET_ITEMNUM, sizeof(g_MainListString)/sizeof(g_MainListString[0]), &err);
    CtlSeti(w, CTL_LIST_CMD_SET_FOCUS, 0, &err);

	CtlSeti(w, CTL_LIST_CMD_SET_USE_TEXTSCROLL, TRUE, &err);

	// column
	CtlSeti(w, CTL_LIST_CMD_SET_COLUMN_NUM, 2, &err);
//	CtlSeti(w, CTL_LIST_CMD_SET_COLUMN_NUM, 3, &err);
//	CtlSeti(w, CTL_LIST_CMD_SET_COLUMN_NUM, 4, &err);

	CtlSetiPos(w, CTL_LIST_CMD_SET_COLUMN_POS, 0, 0, &err);
	CtlSetiPos(w, CTL_LIST_CMD_SET_COLUMN_POS, 1, 30, &err);
//	CtlSetiPos(w, CTL_LIST_CMD_SET_COLUMN_POS, 2, 100, &err);
//	CtlSetiPos(w, CTL_LIST_CMD_SET_COLUMN_POS, 3, 130, &err);

	CtlSetiPtr(w, CTL_LIST_CMD_SET_COLUMN_MARGIN, 0, (void*)&margin_img,&err);
	CtlSetiPtr(w, CTL_LIST_CMD_SET_COLUMN_MARGIN, 1, (void*)&margin_text,&err);
//	CtlSetiPtr(w, CTL_LIST_CMD_SET_COLUMN_MARGIN, 2, (void*)&margin_text,&err);
//	CtlSetiPtr(w, CTL_LIST_CMD_SET_COLUMN_MARGIN, 3, (void*)&margin_text,&err);

	CtlSetiPos(w, CTL_LIST_CMD_SET_TEXT_ALIGN, ALIGN_CENTER, ALIGN_CENTER, &err);

	// 
	CtlSeti(w, CTL_LIST_CMD_SET_LINE_INTERVAL, 1, &err);


	// load list control's number image
	img_num = IMGSUBLISTN001_N_I;
	for(i=0;i<LISTCTRL_IMAGE_NUM;i++)
	{
		RscLoadBitmap(img_num + i, &g_bmpListNumN[i]);
		g_bmpListNumN[i].bTransparent = BITMAP_TRANSPARENT;
		g_bmpListNumN[i].transparentColor = COLOR_BLUE_FOR_PICT;
	}

	img_num = IMGSUBLISTN001_A_I;
	for(i=0;i<LISTCTRL_IMAGE_NUM;i++)
	{
		RscLoadBitmap(img_num + i, &g_bmpListNumS[i]);
		g_bmpListNumS[i].bTransparent = BITMAP_TRANSPARENT;
		g_bmpListNumS[i].transparentColor = COLOR_BLUE_FOR_PICT;
	}

	// load main logo image
	RscLoadBitmap(MGL_RexyE_Face_I, &g_bmpNemustech);
	g_bmpNemustech.bTransparent = BITMAP_OPAQUE;

  	GdiClrScreen(w, COLOR_BLACK);
}

static void PrvDrawMainList(WND wnd)
{
    int err;

    CtlDo(g_wndMainList, CTL_LIST_CMD_DO_DRAW, 0, 0, &err);
}

static void PrvDrawListItemFocus(WND wnd, int line, void *pData)
{
	int err;

	CtlListDrawItemFocus(wnd, line, &err);
}

static void PrvDrawListColumn(WND wnd, int line, int column, void *pData)
{
	int err;
	int nTopIndex = CtlGeti(wnd, CTL_LIST_CMD_GET_TOPINDEX, &err);
	
	switch(column) 
	{
	case 0:
		CtlListDrawItemImage(wnd, line, column, &g_bmpListNumN[line + nTopIndex], &g_bmpListNumS[line + nTopIndex], &err);
		break;
	case 1:
		CtlListDrawItemText(wnd, line, column, g_MainListString[line + nTopIndex], &err);
		break;
	}
}

static void PrvProcessKeyDown(WND wnd, int nModifier, void *pData)
{
	int err;
	CtlListProcessKeyDown(wnd, nModifier, &err);
}

static void PrvCalcFocus(WND wnd, int nModifier)
{
    int err;
	CtlListProcessKeyDown(wnd, nModifier, &err);
}

static void PrvDrawIdle(WND wnd)
{
	int		x,y;
	rect	boundary;

	// Clear screen
	GdiClrScreen( wnd, COLOR_WHITE );

	// Get coordination of bitmap
	WinGetBoundary( wnd, &boundary );
	x = (( boundary.right - boundary.left + 1 ) - g_bmpNemustech.width ) / 2 + boundary.left;
	y = 0;
	
	GdiDrawBitmap(wnd, &g_bmpNemustech, 0, 0, x, y, g_bmpNemustech.width, g_bmpNemustech.height);
}
