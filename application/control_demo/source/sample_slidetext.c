/*
* $Id: sample_slidetext.c 491 2006-07-13 00:26:34Z wijang $
* Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.    
*   
* This file is a part of Rexy Embedded
*   
*/

#include "rexy.h"
#include "rexy_haldevice.h"

#include "../header/sample_list.h"
#include "rexy_ctl_core.h"
#include "rexy_ctl_list.h"

#include "rexy_ctl_slidetext.h"

#include "application.h"

#define dim(x)      (sizeof(x)/sizeof(x[0]))

static char* SlideStrMenu[]=
    {
        "1 : Front-Left-Front Sliding",
        "2 : Front-Left-Round Sliding",
        "3 : End-Left-Front Sliding",
        "4 : End-Left-Round Sliding",
        "5 : 2way sliding at same time ",
        "9 : Previous Menu"
    };



static char* SlideStr[] =
    {
        "Slide Text Item #1",
        "Slide Text Item #2",
        "Slide Text Item #3",
        "Slide Text Item #4",
        "Slide Text Item #5",
        "Slide Text Item #6",
        "Slide Text Item #7",
        "Slide Text Item #8",
        "Slide Text Item #9",
        "Slide Text Item #10",
    };


#define UNIT_TEST_X			0
#define UNIT_TEST_Y			0
#define UNIT_TEST_FGCOLOR	COLOR_BLACK

#define UNIT_TEST_TOP		0
#define UNIT_TEST_LEFT		15
#define UNIT_TEST_RIGHT		100
#define UNIT_TEST_BOTTOM	15
#define UNIT_TEST_BGCOLOR	COLOR_YELLOW

//#define UNIT_TEST_TYPE_FRONT	0
//#define CTL_SLIDETEXT_CMD_SET_TYPE_LROUND	1
#define UNIT_TEST_TIMEOUT	100
#define UNIT_TEST_MAXWIDTH	50

//#define UNIT_TEST_START	2
//#define UNIT_TEST_PAUSE	1
//#define UNIT_TEST_FRONT	0
//#define UNIT_TEST_END	1

static WND gpListWND;
static WND gpSlideWND1;
static WND gpSlideWND2;

static int gMultiSlide;

static void PrvDrawSampleSlideTextMenu(WND wnd)
{
    int i = 0;
    ruint16 nY = 0;
    GdiLockScreen(wnd);
    GdiClrScreen(wnd, COLOR_WHITE);

    for ( i = 0; i <= 6; i++)
    {
        if (nY == 10)
            GdiTextOut(wnd, SlideStrMenu[i], StrLen(SlideStrMenu[i]), 0, nY);
        else
            GdiTextOut(wnd, SlideStrMenu[i], StrLen(SlideStrMenu[i]), 0, nY+=15);
    }
    GdiUnlockScreen(wnd);

}



static void PrvDrawSampleSlideText(WND wnd)
{
    int err;

    CtlDo(wnd, CTL_SLIDETEXT_CMD_DO_DRAW, 0, 0, &err);
}


static int DrawSampleListCB(WND wnd, int nLine, rect rtLineRect, void *pData)
{
    int err;
    int nFocus;
    int nItemNum;
    int nTopIndex;
    COLORVAL oldBG;
    COLORVAL oldFG;
    unsigned short x;
    unsigned short y;
	int mode1;
	int mode2;
	rect* sMarginText;
    int nCurrWidth = 0;

    nFocus      = CtlGeti(gpListWND, CTL_LIST_CMD_GET_FOCUS, &err);
    nItemNum    = CtlGeti(gpListWND, CTL_LIST_CMD_GET_ITEMNUM, &err);
    nTopIndex   = CtlGeti(gpListWND, CTL_LIST_CMD_GET_TOPINDEX, &err);

	sMarginText = CtlGetPtr(gpListWND, CTL_LIST_CMD_GET_RECTMARGIN_TEXT, &err);

    if(nLine > nItemNum-1)
        return 0;

    if(nFocus == nLine+nTopIndex)
    {
        oldFG = GdiSetFgColor(wnd, COLOR_WHITE);
        oldBG = GdiSetBkColor(wnd, COLOR_BLUE);
    }

    GdiFillRect(wnd, &rtLineRect, GdiGetBkColor(wnd));

    FntSetFont(&FntGlobalContext, FntIDAlpha);

    x = rtLineRect.left + sMarginText->left;
    y = rtLineRect.top + sMarginText->top;
    GdiTextOut(wnd, SlideStr[nLine+nTopIndex], StrLen(SlideStr[nLine+nTopIndex]), x, y);

    if(nFocus == nLine+nTopIndex)
    {
        GdiSetFgColor(wnd, oldFG);
        GdiSetBkColor(wnd, oldBG);
    }

    if ( (nFocus == nLine+nTopIndex) )
    {
        nCurrWidth = FntGetTextWidth(&FntGlobalContext, SlideStr[nLine+nTopIndex], strlen( SlideStr[nLine+nTopIndex]));
 
		mode1 = CtlGeti(gpSlideWND1, CTL_SLIDETEXT_CMD_GET_MODE, &err);
        GdiFillRect(wnd, &rtLineRect, GdiGetBkColor(wnd));
        CtlSeti(gpSlideWND1, CTL_SLIDETEXT_CMD_SET_X, 5, &err);
        CtlSeti(gpSlideWND1, CTL_SLIDETEXT_CMD_SET_Y, 1, &err);
        CtlSeti(gpSlideWND1, CTL_SLIDETEXT_CMD_SET_LEFT, UNIT_TEST_LEFT, &err);
        CtlSeti(gpSlideWND1, CTL_SLIDETEXT_CMD_SET_TOP, rtLineRect.top, &err);
        CtlSeti(gpSlideWND1, CTL_SLIDETEXT_CMD_SET_RIGHT, UNIT_TEST_RIGHT, &err);
        CtlSeti(gpSlideWND1, CTL_SLIDETEXT_CMD_SET_BOTTOM, rtLineRect.bottom, &err);
        CtlSetPtr(gpSlideWND1, CTL_SLIDETEXT_CMD_SET_DATA, (void *)SlideStr[nLine+nTopIndex], &err);
		//if ( mode1 == CTL_SLIDETEXT_PAUSE )
		//	CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_STOP, 0, 0, &err);
		CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_PLAY, 0, 0, &err);

        if ( gMultiSlide )
        {
	 		mode2 = CtlGeti(gpSlideWND2, CTL_SLIDETEXT_CMD_GET_MODE, &err);
			//GdiFillRect(wnd, &rtLineRect, GdiGetBkColor(wnd));
            CtlSeti(gpSlideWND2, CTL_SLIDETEXT_CMD_SET_X, 2, &err);
            CtlSeti(gpSlideWND2, CTL_SLIDETEXT_CMD_SET_Y, 1, &err);
            CtlSeti(gpSlideWND2, CTL_SLIDETEXT_CMD_SET_LEFT, UNIT_TEST_LEFT + UNIT_TEST_RIGHT +10, &err);
            CtlSeti(gpSlideWND2, CTL_SLIDETEXT_CMD_SET_TOP, rtLineRect.top, &err);
            CtlSeti(gpSlideWND2, CTL_SLIDETEXT_CMD_SET_RIGHT, UNIT_TEST_RIGHT, &err);
            CtlSeti(gpSlideWND2, CTL_SLIDETEXT_CMD_SET_BOTTOM, rtLineRect.bottom, &err);
            CtlSetPtr(gpSlideWND2, CTL_SLIDETEXT_CMD_SET_DATA, (void *)SlideStr[nLine+nTopIndex], &err);
			
			//if ( mode2 == CTL_SLIDETEXT_PAUSE )
			//	CtlDo(gpSlideWND2, CTL_SLIDETEXT_CMD_DO_STOP, 0, 0, &err);				
			CtlDo(gpSlideWND2, CTL_SLIDETEXT_CMD_DO_PLAY, 0, 0, &err);
       }
    }

    return 0;
}

static int DrawSampleSlideTextCB(WND wnd, COLORVAL nFgColor, COLORVAL nBgColor)
{

    return 0;

}

static int KeyDownSampleSildeTextCB(WND wnd, rint16 wparam)
{
    return 0;
}

static void PrvCreateSampleList(WND wnd)
{
    WND wndList;
    int err;
    REXY_CTL_LIST *pCtlList;
    rect sMargin = {0, 0, 0, 0};
    rect sMarginText = {2, 2, 2, 2};

    wndList = CtlCreate(wnd, CTL_TYPE_LIST, 0, NULL, &err);

    if ( wndList != NULL )
    {
        gpListWND = wndList;

        pCtlList = (REXY_CTL_LIST*)CtlGetStruct(wnd);
//        CtlSetPtr(wndList, CTL_LIST_CMD_SET_RECTMARGIN, (void*)&sMargin, &err);
//        CtlSetPtr(wndList, CTL_LIST_CMD_SET_RECTMARGIN_TEXT, (void*)&sMarginText, &err);

        CtlSeti(wndList, CTL_LIST_CMD_SET_ITEMNUM, dim(SlideStr), &err);
        CtlSeti(wndList, CTL_LIST_CMD_SET_FOCUS, 0, &err);
    }

    return;

}

static void PrvInitSampleSlideText(WND wnd)
{
    int err;

    CtlSeti(wnd, CTL_SLIDETEXT_CMD_SET_X, UNIT_TEST_X, &err);
    CtlSeti(wnd, CTL_SLIDETEXT_CMD_SET_Y, UNIT_TEST_Y, &err);
    CtlSeti(wnd, CTL_SLIDETEXT_CMD_SET_FGCOLOR, UNIT_TEST_FGCOLOR, &err);

    CtlSeti(wnd, CTL_SLIDETEXT_CMD_SET_TOP, UNIT_TEST_TOP, &err);
    CtlSeti(wnd, CTL_SLIDETEXT_CMD_SET_LEFT, UNIT_TEST_LEFT, &err);
    CtlSeti(wnd, CTL_SLIDETEXT_CMD_SET_RIGHT, UNIT_TEST_RIGHT, &err);
    CtlSeti(wnd, CTL_SLIDETEXT_CMD_SET_BOTTOM, UNIT_TEST_BOTTOM, &err);
    CtlSeti(wnd, CTL_SLIDETEXT_CMD_SET_BGCOLOR, UNIT_TEST_BGCOLOR, &err);

    CtlSeti(wnd, CTL_SLIDETEXT_CMD_SET_TIMEOUT,	UNIT_TEST_TIMEOUT, &err);

    CtlSeti(wnd, CTL_SLIDETEXT_CMD_SET_MAXWIDTH, UNIT_TEST_MAXWIDTH, &err);
    CtlSetPtr(wnd, CTL_SLIDETEXT_CMD_SET_DATA,(void *)SlideStr[0], &err);

    CtlSetPtr(wnd, CTL_SLIDETEXT_CMD_SET_CB_DRAW, (void*)DrawSampleSlideTextCB, &err);
	CtlSetPtr(wnd, CTL_SLIDETEXT_CMD_SET_DB_KEYDOWN, (void*)KeyDownSampleSildeTextCB, &err);

    CtlDo(wnd, CTL_SLIDETEXT_CMD_DO_LROUND, 0, 0, &err);
    CtlDo(wnd, CTL_SLIDETEXT_CMD_DO_START_FRONT, 0, 0, &err);

}


static void PrvCreateSampleSlideTextWND1(WND wnd)
{
    int err;
    WND wndSlide;

    wndSlide = CtlCreate(wnd, CTL_TYPE_SLIDETEXT, NULL, NULL, &err);

	gpSlideWND1 = wndSlide;
	PrvInitSampleSlideText(gpSlideWND1);

    return;

}

static void PrvCreateSampleSlideTextWND2(WND wnd)
{
    int err;
    WND wndSlide;

    wndSlide = CtlCreate(wnd, CTL_TYPE_SLIDETEXT, NULL, NULL, &err);
  	gpSlideWND2 = wndSlide;
	PrvInitSampleSlideText(gpSlideWND2);

    return;

}

static void PrvDrawSampleList()
{
    int err;

    CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_DRAW, 0, 0, &err);
}

static void PrvCalcFocus(int nModifier)
{
    int err;
    int nCurrFocus;
    int nNextFocus;
    int nItemNum;
    int nLineNum;
    int nTopIndex;
    bool bDrawAll = FALSE;

    nCurrFocus  = CtlGeti(gpListWND, CTL_LIST_CMD_GET_FOCUS, &err);
    nItemNum    = CtlGeti(gpListWND, CTL_LIST_CMD_GET_ITEMNUM, &err);
    nLineNum    = CtlGeti(gpListWND, CTL_LIST_CMD_GET_LINENUM, &err);
    nTopIndex   = CtlGeti(gpListWND, CTL_LIST_CMD_GET_TOPINDEX, &err);

    nNextFocus = (nCurrFocus + nItemNum + nModifier) % nItemNum;
    CtlSeti(gpListWND, CTL_LIST_CMD_SET_FOCUS, nNextFocus, &err);

    if(nNextFocus < nTopIndex)
    {
        bDrawAll = TRUE;
        nTopIndex = nNextFocus;
        CtlSeti(gpListWND, CTL_LIST_CMD_SET_TOPINDEX, nTopIndex, &err);
    }
    else if(nNextFocus > nTopIndex + nLineNum - 1)
    {
        bDrawAll = TRUE;
        nTopIndex = nNextFocus - nLineNum +1;
        CtlSeti(gpListWND, CTL_LIST_CMD_SET_TOPINDEX, nTopIndex, &err);
    }

    if(bDrawAll)
        PrvDrawSampleList();
    else
    {
        GdiLockScreen(gpListWND);
        CtlDo(gpListWND, CTL_LIST_CMD_DO_ITEMDRAW, nCurrFocus-nTopIndex, 0, &err);
        CtlDo(gpListWND, CTL_LIST_CMD_DO_ITEMDRAW, nNextFocus-nTopIndex, 0, &err);
        GdiUnlockScreen(gpListWND);
    }
}


static int PrvSlideTextSampleDestroy(WND wnd )
{
	int err;
 
	CtlDestroy( wnd, &err);
	wnd = NULL;
 
 
	return 0;
}



int PrvSlideTextMainProc(WND wnd, int nMsg, int wParam, int lParam)
{
    int err;

    int bHandled = FALSE;

    switch(nMsg)
    {
    case AWM_CREATE:
        GdiClrScreen(wnd, COLOR_WHITE);
        PrvDrawSampleSlideTextMenu(wnd);
        PrvCreateSampleList(wnd);
        bHandled = TRUE;

        break;

    case AWM_PAINT:
        break;
    case AWM_KEYDOWN:
        switch(wParam)
        {
        case VK_USER_UP:
            if(gpListWND)
            {
                PrvCalcFocus(-1);
                
                bHandled = TRUE;
            }
            break;

        case VK_USER_DOWN:
            if(gpListWND)
            {
         
				PrvCalcFocus(1);

                bHandled = TRUE;
            }
            break;
 
        case VK_PAD_1:

			if ( gMultiSlide == 1 )
			{
				gMultiSlide = 0;
				CtlDestroy (gpSlideWND2, &err);
				gpSlideWND2 = NULL;
			}
			
			if ( gpSlideWND1 == NULL )
				PrvCreateSampleSlideTextWND1(wnd);
	        
			PrvDrawSampleList();
 			
			CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_LFRONT, 0, 0, &err);
            CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_START_FRONT, 0, 0, &err);
            CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_PLAY, 0, 0, &err);
	        
			bHandled = TRUE;

            break;

        case VK_PAD_2:

			if ( gMultiSlide == 1 )
			{
				gMultiSlide = 0;
				CtlDestroy (gpSlideWND2, &err);
				gpSlideWND2 = NULL;
			}

			if ( gpSlideWND1 == NULL )
				PrvCreateSampleSlideTextWND1(wnd);

        	PrvDrawSampleList();
        
			CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_LROUND, 0, 0, &err);
            CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_START_FRONT, 0, 0, &err);
            CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_PLAY, 0, 0, &err);

            bHandled = TRUE;

            break;

        case VK_PAD_3:
			if ( gMultiSlide == 1 )
			{
				gMultiSlide = 0;
				CtlDestroy (gpSlideWND2, &err);
				gpSlideWND2 = NULL;
			}

			if ( gpSlideWND1 == NULL )
				PrvCreateSampleSlideTextWND1(wnd);

        	PrvDrawSampleList();

            CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_LFRONT, 0, 0, &err);
            CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_START_END, 0, 0, &err);
            CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_PLAY, 0, 0, &err);
            
            bHandled = TRUE;

            break;

        case VK_PAD_4:

 			if ( gMultiSlide == 1 )
			{
				gMultiSlide = 0;
				CtlDestroy (gpSlideWND2, &err);
				gpSlideWND2 = NULL;
			}

			if ( gpSlideWND1 == NULL )
				PrvCreateSampleSlideTextWND1(wnd);
            
			PrvDrawSampleList();

            CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_LROUND, 0, 0, &err);
            CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_START_END, 0, 0, &err);
            CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_PLAY, 0, 0, &err);

            bHandled = TRUE;

            break;
        case VK_PAD_5:

            gMultiSlide = 1;
			
			
			if ( gpSlideWND1 == NULL )
				PrvCreateSampleSlideTextWND1(wnd);

			if ( gpSlideWND2 == NULL )
				PrvCreateSampleSlideTextWND2(wnd);
			
			PrvDrawSampleList();

            CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_LFRONT, 0, 0, &err);
            CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_START_FRONT, 0, 0, &err);
            CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_PLAY, 0, 0, &err);
            
			CtlDo(gpSlideWND2, CTL_SLIDETEXT_CMD_DO_LROUND, 0, 0, &err);
            CtlDo(gpSlideWND2, CTL_SLIDETEXT_CMD_DO_START_END, 0, 0, &err);
            CtlDo(gpSlideWND2, CTL_SLIDETEXT_CMD_DO_PLAY, 0, 0, &err);
             
            bHandled = TRUE;

            break;

		case VK_PAD_6:
			if ( gMultiSlide == 1 )
			{
				CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_STOP, 0, 0, &err);
				CtlDo(gpSlideWND2, CTL_SLIDETEXT_CMD_DO_STOP, 0, 0, &err);

			}
			else
				CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_STOP, 0, 0, &err);
			
			break;

		case VK_PAD_7:
		
			if ( gMultiSlide == 1 )
			{
				CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_PLAY, 0, 0, &err);
				CtlDo(gpSlideWND2, CTL_SLIDETEXT_CMD_DO_PLAY, 0, 0, &err);
			}
			else
				CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_PLAY, 0, 0, &err);
			break;

		case VK_PAD_8:
			if ( gMultiSlide == 1 )
			{
				CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_PAUSE, 0, 0, &err);
				CtlDo(gpSlideWND2, CTL_SLIDETEXT_CMD_DO_PAUSE, 0, 0, &err);
				
			}
			else
				CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_PAUSE, 0, 0, &err);

			break;


        case VK_PAD_9:
            GdiClrScreen(wnd, COLOR_WHITE);
            PrvDrawSampleSlideTextMenu(wnd);
            CtlDo(gpSlideWND1, CTL_SLIDETEXT_CMD_DO_STOP, 0, 0,&err);
			CtlDestroy (gpSlideWND1, &err);
			gpSlideWND1 = NULL;
			if (gpSlideWND2 != NULL)
			{
	            CtlDo(gpSlideWND2, CTL_SLIDETEXT_CMD_DO_STOP, 0, 0,&err);
				CtlDestroy (gpSlideWND2, &err);
			}
            StaGotoState(wnd, STATE_ID_CTL_TEST, MSG_DESTROY, MSG_CREATE);
            bHandled = TRUE;

            break;

		}

        break;
    case AWM_DESTROY:
        break;

    default:
        break;
    }

    return 	bHandled;
}

int SampleSlideTextProc(WND wnd, int nMsg, int wParam, int lParam)
{
    WND slideAppWND;

    int	ScreenWidth, ScreenHeight;

    int bHandled = FALSE;

    switch(nMsg)
    {
    case AWM_CREATE:
        // Create Window for this application
        GdiGetScreenInfo( &ScreenWidth, &ScreenHeight ); // We are going to use whole screen
   		slideAppWND	= WinCreateWindow( PrvSlideTextMainProc, 0, 0, ScreenWidth, ScreenHeight, WND_PANEL );

        bHandled = TRUE;
        break;

    case AWM_DESTROY:
        WinDestroyWindow(wnd);
        bHandled = TRUE;

        break;

    default:
        break;
    }

    return bHandled;
}
