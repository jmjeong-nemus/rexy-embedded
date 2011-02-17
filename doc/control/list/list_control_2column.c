/* 
* $Id: sample_list.c 491 2006-01-11 00:26:34Z madholic $
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
#include "application.h"

#define LISTCTRL_IMAGE_NUM		20
#define dim(x)      (sizeof(x)/sizeof(x[0]))

static char* ListString[] =
{
    "Item 1",
    "Item 2",
    "Item 3",
    "Item 4",
    "Item 5",
    "Item 6",
    "Item 7",
    "Item 8",
    "Item 9",
    "Item 10",
    "Item 11",
    "Item 12",
    "Item 13"
};

static char* ListDescString[] =
{
    "This is the Item 1",
    "This is the Item 2",
    "This is the Item 3",
    "This is the Item 4",
    "This is the Item 5",
    "This is the Item 6",
    "This is the Item 7",
    "This is the Item 8",
    "This is the Item 9",
    "This is the Item 10",
    "This is the Item 11",
    "This is the Item 12",
    "This is the Item 13"
};

WND		gpListWnd;
WND		gpMainWnd;

bool	g_bScrollTimer=FALSE;
int		g_nScrollLeft=0;

static RxBITMAP g_bmpListNumN[LISTCTRL_IMAGE_NUM];
static RxBITMAP g_bmpListNumA[LISTCTRL_IMAGE_NUM];

static void PrvDrawListItemFocus(WND wnd, int line, void *pData);
static void PrvDrawListColumn(WND wnd, int line, int column, void *pData);
static void PrvProcessKeyDown(WND wnd, int nModifier, void *pData);

static void PrvExampleDrawScrollText(WND wnd, int line, int col, char* string, int* pErr);

void CreateSampleList(WND wnd)
{
	WND w;
    int err,i;
	int img_num = 0;
	int width, height;

	REXY_CTL_LIST *pCtlList;

	rect rtControlPos;
	rect margin_text = {2, 8, 2, 8};
	rect margin_img  = {2, 2, 2, 2};

	GdiGetScreenInfo( &width, &height ); // We are going to use whole screen

    w = CtlCreate(wnd, CTL_TYPE_LIST, 0, NULL, &err);
	gpListWnd = w;
	
	pCtlList = (REXY_CTL_LIST *)CtlGetStruct(w);

	// set control window position
	GdiSetRect(&rtControlPos, 10, 10, width - 10, height - 10);
    CtlSetPtr(w, CTL_LIST_CMD_SET_CTRL_POS, (void*)&rtControlPos, &err);

	// set list control attribute
    CtlSetPtr(w, CTL_LIST_CMD_SET_DRAW_ITEM_FOCUS_CB, (void*)PrvDrawListItemFocus, &err);
    CtlSetPtr(w, CTL_LIST_CMD_SET_DRAW_COLUMN_CB, (void*)PrvDrawListColumn, &err);
    CtlSetPtr(w, CTL_LIST_CMD_SET_KEY_DOWN_CB, (void*)PrvProcessKeyDown, &err);

	CtlSeti(w, CTL_LIST_CMD_SET_STYLE, CTL_LIST_STYLE_SINGLELINE_NORMAL, &err);
	CtlSeti(w, CTL_LIST_CMD_SET_FONT, FntIDAlpha, &err);
	CtlSeti(w, CTL_LIST_CMD_SET_CTRL_BGCOLOR, COLOR_WHITE, &err);
	
	CtlSeti(w, CTL_LIST_CMD_SET_LINE_HEIGHT, 28, &err);

    CtlSeti(w, CTL_LIST_CMD_SET_ITEMNUM, sizeof(ListString)/sizeof(ListString[0]), &err);
    CtlSeti(w, CTL_LIST_CMD_SET_FOCUS, 0, &err);

	// column
	CtlSeti(w, CTL_LIST_CMD_SET_COLUMN_NUM, 2, &err);

	CtlSetii(w, CTL_LIST_CMD_SET_COLUMN_POS, 0, 0, &err);
	CtlSetii(w, CTL_LIST_CMD_SET_COLUMN_POS, 1, 30, &err);

	CtlSetiPtr(w, CTL_LIST_CMD_SET_COLUMN_MARGIN, 0, (void*)&margin_img,&err);
	CtlSetiPtr(w, CTL_LIST_CMD_SET_COLUMN_MARGIN, 1, (void*)&margin_text,&err);

	CtlSetii(w, CTL_LIST_CMD_SET_TEXT_ALIGN, ALIGN_LEFT, ALIGN_LEFT, &err);

	// 
	CtlSeti(w, CTL_LIST_CMD_SET_FOCUS_TYPE, CTL_LIST_FOCUS_TYPE_RECT, &err);
	CtlSeti(w, CTL_LIST_CMD_SET_STRIPE_BGCOLOR, COLOR_MAGENTA, &err);
	CtlSeti(w, CTL_LIST_CMD_SET_LINE_INTERVAL, 1, &err);


	// load list control's number image
	img_num = IMGSUBLISTN000_N_I;
	for(i=0;i<LISTCTRL_IMAGE_NUM;i++)
	{
		RscLoadBitmap(img_num + i, &g_bmpListNumN[i]);
		g_bmpListNumN[i].bTransparent = BITMAP_TRANSPARENT;
		g_bmpListNumN[i].transparentColor = COLOR_BLUE_FOR_PICT;
	}

	img_num = IMGSUBLISTN000_A_I;
	for(i=0;i<LISTCTRL_IMAGE_NUM;i++)
	{
		RscLoadBitmap(img_num + i, &g_bmpListNumA[i]);
		g_bmpListNumA[i].bTransparent = BITMAP_TRANSPARENT;
		g_bmpListNumA[i].transparentColor = COLOR_BLUE_FOR_PICT;
	}

  	GdiClrScreen(w, COLOR_BLACK);
}

void DrawSampleList()
{
    int err;

    CtlDo(gpListWnd, CTL_LIST_CMD_DO_DRAW, 0, 0, &err);
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
		CtlListDrawItemImage(wnd, line, column, &g_bmpListNumN[line + nTopIndex], &g_bmpListNumA[line + nTopIndex], &err);
		break;
	case 1:
		CtlListDrawItemText(wnd, line, column, ListString[line + nTopIndex], &err);
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

int PrvSampleListProc(WND hWnd, int nMsg, int wParam, int lParam)
{
	int bHandled = FALSE;
	int pErr;
	rect rtWin;
	
    switch(nMsg)
    {
    case AWM_WINCREATE:
		gpMainWnd = hWnd;

		// erase current screen
		GdiGetWindowRect(hWnd, &rtWin);
		GdiFillRect(hWnd, &rtWin, COLOR_WHITE);
		
        CreateSampleList(hWnd);
		if(gpListWnd!=NULL)
			DrawSampleList();

        break;
	case AWM_WINDESTROY:
		CtlDestroy(gpListWnd, &pErr);
		break;
    case AWM_PAINT:
        DrawSampleList();
		bHandled = TRUE;
        break;

    case AWM_KEYDOWN:
        switch(wParam)
        {
            //UP
        case VK_USER_UP:
            if(gpListWnd)
            {
                PrvCalcFocus(gpListWnd, -1);
            }
            break;

            //DOWN
        case VK_USER_DOWN:
            if(gpListWnd)
            {
                PrvCalcFocus(gpListWnd, 1);
            }
            break;

		case VK_PAD_9:
			StaGotoState(hWnd, STATE_ID_CTL_TEST, MSG_DESTROY, MSG_CREATE);			
			break;
        }
        break;
    default:
        break;
    }

    return bHandled;
}

int SampleListProc(WND hWnd, int nMsg, int wParam, int lParam)
{
	int ScreenWidth, ScreenHeight;
	int bHandled = FALSE;
    switch(nMsg)
    {
    case AWM_CREATE:
		GdiGetScreenInfo( &ScreenWidth, &ScreenHeight );
		WinCreateWindow(PrvSampleListProc, 0, 0, ScreenWidth, ScreenHeight, WND_PANEL);
		bHandled = TRUE;
        break;
	case AWM_DESTROY:
		WinDestroyWindow(hWnd);
		bHandled = TRUE;
		break;
	}
    return bHandled;
}

