/* 
* $Id: rexy_ctl_list.c 491 2006-01-11 00:26:34Z madholic $
* Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.    
*   
* This file is a part of Rexy Embedded
*   
* This file defines the functions and logics of List Control.
* For function naming rules follow below.
* Exported : "list" + "_" + "(Verb)" {+ "(Object)" : Optional}
* Private : "PrvListCtl" + "(Verb)" + "(Object)"
*/

/** @file */

/**
 * @addtogroup ListCtlGroup Ctrl List Control
 * Ctrl List Control
 * @{
 */

#include "rexy_ctl_list.h"

#define CHECK_BND(e, rng1, rng2, err, r) if(e < rng1 || e > rng2) { err = CTLERR_SETI; return r; }
#define CHECK_BND_VOID(e, rng1, rng2, err)  if(e < rng1 || e > rng2) { err = CTLERR_SETI; return; }

///// 
// //brief        Check margin boundary.
// //param[in]    pCtlListMargin  Pointer to list control margin structure
// //param[out]   pErr            Pointer to error value
// //retval       void
// //remarks      Check if specified margin does not exceed boundary.
// //if HISTORY
//         9-Jan-2006 alscar Initiated
// //endif
///
void PrvListCtlCheckMargin(rect *pCtlListMargin, int *pErr)
{
    int nScrWidth;
    int nScrHeight;

	GdiGetScreenInfo(&nScrWidth,&nScrHeight);

    CHECK_BND_VOID(pCtlListMargin->top, 0, nScrHeight, *pErr);
    CHECK_BND_VOID(pCtlListMargin->bottom, 0, nScrHeight-pCtlListMargin->top, *pErr);

    CHECK_BND_VOID(pCtlListMargin->left, 0, nScrWidth, *pErr);
    CHECK_BND_VOID(pCtlListMargin->right, 0, nScrWidth-pCtlListMargin->left, *pErr);
}

///// 
// //brief        Get data pointer.
// //param[in]    wnd
// //param[out]   pErr            Pointer to error value
// //retval       REXY_CTL_LIST//  List control data pointer.
// //remarks      Control structure has void data pointer member.\n
//               This function returns speicified control's data pointer.
// //if HISTORY
//         9-Jan-2006 alscar Initiated
// //endif
///
REXY_CTL_LIST* PrvListCtlGetDataP(WND wnd, int *pErr)
{
    //Will be checked by control core.
    //CHECK_NULL_PTR(pCtl, NULL, *pErr);

    //CHECK_ERR_IF_NOT_EQUAL(pCtl->nType, CTL_TYPE_LIST, NULL);
    CHECK_ERR_IF_NOT_EQUAL(wnd->window_type, CTL_TYPE_LIST, NULL);
    
    //Will be checked after PrvListCtlGetDataP() function call.
    //CHECK_NULLPTR(pCtl->pData, NULL, *pErr);

    return wnd->pData;
}

/** 
* @brief        Check if the current line is focus line.
* @param[in]    wnd				The window handle
* @param[in]    pCtlList		The list control's data pointer
* @param[in]    line			The current line number
* 
* @retval       bool			If the current line is focus line , return TRUE, otherwise; return FALSE
* @remarks      Check if the current line is focus line.
*
* @if HISTORY
*         18-Sep-2006 madholic Initiated
* @endif
*/
bool CtlListIsFocusLine(WND wnd, REXY_CTL_LIST* pCtlList, int line)
{
	int cur_line = line + pCtlList->nTopIndex;
	int focus_line = pCtlList->nFocus;

	return (focus_line==cur_line);
}

/** 
* @brief        Get column rectangle area.
* 
* @param[in]    wnd			The window handle
* @param[in]    nLine		The line index
* @param[in]    col			The column index
* @param[out]   col_rect	column area
* 
* @retval       void
* @remarks      Get column rectangle area using current line and column index
*
* @if HISTORY
*         18-Sep-2006 madholic Initiated
* @endif
*/
void CtlListGetColumnRect(WND wnd, int nLine, int col, rect* col_rect)
{
	rect rtClient;
	REXY_CTL_LIST* pCtlList = (REXY_CTL_LIST*)CtlGetStruct(wnd);

	GdiGetClientRect(wnd, &rtClient);

    col_rect->left     = 0 + pCtlList->column_pos[col];
	col_rect->top      = nLine * pCtlList->nLineHeight + (nLine * pCtlList->nLineInterval);

	if(col >= pCtlList->column_num-1)
		col_rect->right    = rtClient.right;
	else
		col_rect->right    = pCtlList->column_pos[col+1]-1;

    col_rect->bottom   = col_rect->top + (pCtlList->nLineHeight-1);
}

/** 
* @brief        Get item rectangle area.
* 
* @param[in]    wnd			The window handle
* @param[in]    nLineIndex	The line index
* @param[out]   pRect		item rectangle pointer
* 
* @retval       void
* @remarks      Get item rectangle area using line index
*
* @if HISTORY
*         18-Sep-2006 madholic Initiated
* @endif
*/
void CtlListGetItemRect(WND wnd, int nLineIndex, rect *pRect)
{
	rect rtClient;
	REXY_CTL_LIST* pCtlList = (REXY_CTL_LIST*)CtlGetStruct(wnd);

	GdiGetClientRect(wnd, &rtClient);

    pRect->left     = 0;
	pRect->top      = nLineIndex * pCtlList->nLineHeight + (nLineIndex * pCtlList->nLineInterval);
    pRect->right    = rtClient.right;
    pRect->bottom   = pRect->top + (pCtlList->nLineHeight-1);
}

/** 
* @brief        Get the data pointer of the list control.
* 
* @param[in]    wnd			The window handle
* 
* @retval       REXY_CTL_LIST*	the data pointer of the list control
* @remarks      Get the data pointer of the list control and then return the pointer with REXY_CTL_LIST* type
*
* @if HISTORY
*         18-Sep-2006 madholic Initiated
* @endif
*/
REXY_CTL_LIST* CtlListGetStruct(WND wnd)
{
	return ((REXY_CTL_LIST*)CtlGetStruct(wnd));
}

///// 
// //brief            Set number of lines in list control
// //param[in,out]    pCtlList    Pointer to list control data
// //retval           void
// //remarks          By list control's margin this function calculates list control's number of lines.\n
//                   Calculated results will be stored in specified list control data structure.
// //if HISTORY
//         9-Jan-2006 alscar Initiated
// //endif
///
void PrvListCtlSetLineNum(REXY_CTL_LIST *pCtlList)
{
	int nListCtrlSize;

	nListCtrlSize = pCtlList->rtCtrlWndPos.bottom - pCtlList->rtCtrlWndPos.top;
    pCtlList->nLineNum = nListCtrlSize / (pCtlList->nLineHeight + pCtlList->nLineInterval);
}

void PrvListCtlSetCtrlPos(REXY_CTL_LIST *pCtlList, rect* pos, int *pErr)
{
	pCtlList->rtCtrlWndPos.left = pos->left;
	pCtlList->rtCtrlWndPos.top = pos->top;
	pCtlList->rtCtrlWndPos.right = pos->right;
	pCtlList->rtCtrlWndPos.bottom = pos->bottom;
}

void PrvListCtlSetColumnMargin(REXY_CTL_LIST *pCtlList, int col, rect* pMargin, int *pErr)
{
    pCtlList->column_margin[col].left     = pMargin->left;
    pCtlList->column_margin[col].top      = pMargin->top;
    pCtlList->column_margin[col].right    = pMargin->right;
    pCtlList->column_margin[col].bottom   = pMargin->bottom;
}

void PrvListCtlSetItemFocus(REXY_CTL_LIST *pCtlList, REXY_CTL_LIST_ITEM_FOCUS_STYLE *pCtlListFocus, int *pErr)
{
	pCtlList->item_focus_style.fcolor = pCtlListFocus->fcolor;
	pCtlList->item_focus_style.bcolor = pCtlListFocus->bcolor;
	pCtlList->item_focus_style.text_style = pCtlListFocus->text_style;
	pCtlList->item_focus_style.text_align = pCtlListFocus->text_align;
}

void PrvListCtlSetItemNFocus(REXY_CTL_LIST *pCtlList, REXY_CTL_LIST_ITEM_NFOCUS_STYLE *pCtlListNFocus, int *pErr)
{
	pCtlList->item_nfocus_style.fcolor = pCtlListNFocus->fcolor;
	pCtlList->item_nfocus_style.bcolor = pCtlListNFocus->bcolor;
	pCtlList->item_nfocus_style.text_style = pCtlListNFocus->text_style;
	pCtlList->item_nfocus_style.text_align = pCtlListNFocus->text_align;
}

void PrvListCtlSetWnd(WND wnd, int *pErr)
{
    REXY_CTL_LIST *pCtlList = CtlListGetStruct(wnd);
    CHECK_NULLPTR_VOID(pCtlList, *pErr);

    CtlSetWNDRect(wnd, pCtlList->rtCtrlWndPos.left, pCtlList->rtCtrlWndPos.top, 
			      (pCtlList->rtCtrlWndPos.right - pCtlList->rtCtrlWndPos.left),
				  (pCtlList->rtCtrlWndPos.bottom - pCtlList->rtCtrlWndPos.top), 
				  pErr);
}

void PrvListInit(WND wnd)
{
	int width, height;

	REXY_CTL_LIST* pCtlList = (REXY_CTL_LIST*)CtlGetStruct(wnd);
	if(!pCtlList)
		return;

    //Initialize memory allocated - list control.
    MemSet(pCtlList, 0, sizeof(REXY_CTL_LIST));

	GdiGetScreenInfo(&width, &height);
    //Set default values to list control structure.

	// Set the list control's margin
    pCtlList->rtCtrlWndPos.left    = 0;
    pCtlList->rtCtrlWndPos.top     = 0;
    pCtlList->rtCtrlWndPos.right   = width;
    pCtlList->rtCtrlWndPos.bottom  = height;

	// for scrolling text
	pCtlList->nScrollLeft = 0;

	// Set the control's attribute
	pCtlList->bgcolor_control = COLOR_WHITE;

	pCtlList->nLineInterval = 1;
	pCtlList->font = FntGetFont(&FntGlobalContext);
	
	// Set the attribute of the focus item
	pCtlList->item_focus_style.fcolor = COLOR_BLACK;
	pCtlList->item_focus_style.bcolor = COLOR_WHITE;
	pCtlList->item_focus_style.text_style = BOLDFONT;
	pCtlList->item_focus_style.text_align = ALIGN_LEFT;

	// Set the attribute of the non-focus item
	pCtlList->item_nfocus_style.fcolor = COLOR_BLACK;
	pCtlList->item_nfocus_style.bcolor = COLOR_WHITE;
	pCtlList->item_nfocus_style.text_style = NORMALFONT;
	pCtlList->item_nfocus_style.text_align = ALIGN_LEFT;
}

/** 
* @brief        Create a new list control.
* @param[in]	wnd
* @retval       REXY_CTL*   Pointer to create control structure
* @remarks      This function creates new list control and initiate its value.
* @if HISTORY
*         9-Jan-2006 alscar Initiated
* @endif
*/
void CtlListCreate(WND wnd)
{
    //Allocate memory form list control structure.
    wnd->pData = MemAlloc(sizeof(REXY_CTL_LIST));
    if(wnd->pData == NULL)
    {
		WinDestroyControl(wnd);
		wnd = NULL;
        return ;
    }

	PrvListInit(wnd);
}

/** 
* @brief        Call back function called after creating list control.
* @param[in]    wnd
* @param[out]   pErr            Pointer to error value
* @retval       RxErrNone       Always return RxErrNone
* @remarks      This function is called by control core after CtlListCreate() is called.\n
*               It sets initial list control's window area and foreground/background color.
* @if HISTORY
*         9-Jan-2006 alscar Initiated
* @endif
*/
int CtlListCreatePostCB(WND wnd, int *pErr)
{
    PrvListCtlSetWnd(wnd, pErr);
    CHECK_ERR_IF_NOT_EQUAL(*pErr, CTLERR_NONE, 0);

    CtlSetWNDColor(wnd, COLOR_BLACK, COLOR_WHITE, pErr);
    CHECK_ERR_IF_NOT_EQUAL(*pErr, CTLERR_NONE, 0);

    return RxErrNone;
}

/** 
* @brief        Destroys created list control.
* @param[in]    wnd
* @param[out]   pErr            Pointer to error value
* @retval       RxErrNone       Always return RxErrNone
* @remarks      Destroys created list control.\n
*               Free allocated memory for list control data.
* @if HISTORY
*         9-Jan-2006 alscar Initiated
* @endif
*/
int CtlListDestroy(WND wnd, int *pErr)
{
    REXY_CTL_LIST *pCtlList = CtlListGetStruct(wnd);
    CHECK_NULLPTR(pCtlList, 0, *pErr);

	// kill timer
	UsrKillTimer(wnd, REXY_LIST_TIME_SQUARE_SCROLLING_TIMER );

    MemFree(pCtlList);
    wnd->pData = NULL;
	WinDestroyControl(wnd);
	wnd = NULL;

    return RxErrNone;
}

/** 
* @brief        Call back function called before destroying list control.
* @param[in]    wnd
* @param[out]   pErr            Pointer to error value
* @retval       RxErrNone       On success
* @remarks      Currently does nothing.
* @if HISTORY
*         9-Jan-2006 alscar Initiated
* @endif
*/
int CtlListDestroyPreCB(WND wnd, int *pErr)
{
    return RxErrNone;
}

/** 
* @brief        Try to draw whole list control.
* @param[in]    wnd     Pointer to control window
* @param[out]   pErr    Pointer to error value
* @retval       void
* @remarks      This functions calls application draw callback function for each list control line.\n
*               To prevent screen blinking, this function uses GdiLock feature.
* @if HISTORY
*         9-Jan-2006 alscar Initiated
* @endif
*/
void CtlListDraw(WND wnd, int *pErr)
{
    int nLineIndex;
    rect rtClient, rtItem;
    REXY_CTL_LIST *pCtlList;

    pCtlList = (REXY_CTL_LIST*)CtlGetStruct(wnd);
    CHECK_NULLPTR_VOID(pCtlList, *pErr);

	GdiGetClientRect(wnd, &rtClient);

    GdiLockScreen(wnd);

	// Draw list control's background
	GdiFillRect(wnd, &rtClient, pCtlList->bgcolor_control);

    for(nLineIndex=0; nLineIndex < pCtlList->nLineNum; nLineIndex++)
    {
		CtlListGetItemRect(wnd, nLineIndex, &rtItem);

		// 1. Erase and Draw item area
		GdiFillRect(wnd, &rtItem, GdiGetBkColor(wnd));

		// 2. Draw item
		CtlListDrawItem(wnd, nLineIndex, pCtlList->pData);
    }

    GdiUnlockScreen(wnd);
}

/** 
* @brief        Try to draw single specified line of list control.
* @param[in]    wnd     Pointer to control window
* @param[in]    nLine   Index of line to draw
* @param[out]   pErr    Pointer to error value
* @retval       void
* @remarks      This functions calls application draw callback function for a single line.\n
*               To prevent screen blinking, this function uses GdiLock feature.
* @if HISTORY
*         9-Jan-2006 alscar Initiated
* @endif
*/
void CtlListDrawLine(WND wnd, int nLine, int *pErr)
{
	rect rtItem;
    REXY_CTL_LIST *pCtlList = (REXY_CTL_LIST*)CtlGetStruct(wnd);
    CHECK_NULLPTR_VOID(pCtlList, *pErr);

    //Check drawing line.
    CHECK_BND_VOID(nLine, 0, pCtlList->nLineNum, *pErr);

    GdiLockScreen(wnd);

	CtlListGetItemRect(wnd, nLine, &rtItem);

	// 1. Erase and Draw item area
	GdiFillRect(wnd, &rtItem, GdiGetBkColor(wnd));

	// 2. Draw item
	CtlListDrawItem(wnd, nLine, pCtlList->pData);

    GdiUnlockScreen(wnd);
}

/** 
* @brief        Draws a single specified line of list control.
* @param[in]    wnd			Pointer to control window
* @param[in]    nLine		Index of line to draw
* @param[in]    col			Column number to draw
* @param[in]    pItemText	The column text to draw
* @param[out]   pErr		Pointer to error value
* @retval       void
* @remarks      Draws a single specified line of list control using line index, column index and text
* 
* @if HISTORY
*         9-Jan-2006 alscar Initiated
* @endif
*/
void CtlListDrawItemText(WND wnd, int nLine, int col, char* pItemText, int *pErr)
{
    rect col_rect, item_rect;
	int text_x=0, text_y=0;
	int width, height;
	int item_num=0;
	int prev_clip_left, prev_clip_right;

	COLORVAL old_fcolor, old_bcolor, old_focus_fcolor;
	int old_style, old_align, old_extent;
	int old_font;
	int nScrollLeft=0, col_width=0, text_width=0;

    REXY_CTL_LIST* pCtlList;

	// get attribute
    pCtlList = (REXY_CTL_LIST*)CtlGetStruct(wnd);

	item_num = pCtlList->nItemNum;
    if(nLine > item_num-1)
        return;

	GdiGetScreenInfo(&width, &height);

	CtlListGetItemRect(wnd, nLine, &item_rect);
	CtlListGetColumnRect(wnd, nLine, col, &col_rect);

	// check string width to decide scrolling text
	col_width = col_rect.right - col_rect.left - pCtlList->column_margin[col].left - pCtlList->column_margin[col].right;
	text_width = FntGetTextWidth(&FntGlobalContext, pItemText, StrLen(pItemText));
	if(text_width > col_width)
	{
		if(pCtlList->bUseTextScroll && !pCtlList->bScrollTimer && CtlListIsFocusLine(wnd, pCtlList, nLine))
		{
			UsrSetTimer(wnd, REXY_LIST_TIME_SQUARE_SCROLLING_TIMER, TIME_SQUARE_SCROLLING_INTERVAL);
			pCtlList->bScrollTimer = TRUE;
		}

		nScrollLeft = pCtlList->nScrollLeft;
		// if scroll is ended, move position to the end of the item
		if( -nScrollLeft >= text_width)
			pCtlList->nScrollLeft = col_width;
	}
	else
	{
		nScrollLeft = 0;
		pCtlList->bScrollTimer = FALSE;
	}

	// set coordiantes
    text_x = col_rect.left + pCtlList->column_margin[col].left + nScrollLeft;
    text_y = col_rect.top + pCtlList->column_margin[col].top;

	// set font
	old_font = FntSetFont(&FntGlobalContext, pCtlList->font);

	// set color
	if(CtlListIsFocusLine(wnd, pCtlList, nLine))
	{
		old_fcolor = GdiSetFgColor(wnd, pCtlList->item_focus_style.fcolor);
		old_bcolor = GdiSetBkColor(wnd, pCtlList->item_focus_style.bcolor);
	}
	else
	{
		old_fcolor = GdiSetFgColor(wnd, pCtlList->item_nfocus_style.fcolor);
		old_bcolor = GdiSetBkColor(wnd, pCtlList->item_nfocus_style.bcolor);
	}

	// set text attribute
	if(CtlListIsFocusLine(wnd, pCtlList, nLine))
	{
		old_style = GdiSetTextStyle(pCtlList->item_focus_style.text_style);
		old_align = GdiSetTextAlign(pCtlList->item_focus_style.text_align);
		old_extent = GdiSetTextExtent(col_width);
	}
	else
	{
		old_style = GdiSetTextStyle(pCtlList->item_nfocus_style.text_style);
		old_align = GdiSetTextAlign(pCtlList->item_nfocus_style.text_align);
		old_extent = GdiSetTextExtent(col_width);
	}

	/////////////////////////////////////////////////////////////////////////////
	// DRAWING
	// 

	prev_clip_left = wnd->clip.left;
	prev_clip_right = wnd->clip.right;

	// clipping apply
	wnd->clip.left = pCtlList->rtCtrlWndPos.left + col_rect.left + pCtlList->column_margin[col].left;
	wnd->clip.right = col_rect.right - pCtlList->column_margin[col].right;

	if(CtlListIsFocusLine(wnd, pCtlList, nLine))
	{
        old_focus_fcolor = GdiSetFgColor(wnd, COLOR_RED);
		
		GdiTextOutAttr(wnd, text_x, text_y, pItemText, StrLen(pItemText));

		GdiSetFgColor(wnd, old_focus_fcolor);
	}
	else
	{
		GdiTextOutAttr(wnd, text_x, text_y, pItemText, StrLen(pItemText));
	}

	wnd->clip.left = prev_clip_left;	
	wnd->clip.right = prev_clip_right;

	/////////////////////////////////////////////////////////////////////////////
	// SETTING RESTORE

	FntSetFont(&FntGlobalContext, old_font);
	GdiSetFgColor(wnd, old_fcolor);
	GdiSetBkColor(wnd, old_bcolor);
	GdiSetTextStyle(old_style);
	GdiSetTextAlign(old_align);
	GdiSetTextExtent(old_extent);
	/////////////////////////////////////////////////////////////////////////////
}

/** 
* @brief        Handles timer event by default.
* 
* @param[in]    wnd			The window handle
* 
* @retval       void
* @remarks      Get item rectangle area using line index
*
* @if HISTORY
*         18-Sep-2006 madholic Initiated
* @endif
*/
void CtlListHandleTimer(WND wnd)
{
    REXY_CTL_LIST* pCtlList;
	int cur_focus=0, top_index=0;
	int err;

	pCtlList = (REXY_CTL_LIST*)CtlGetStruct(wnd);
	cur_focus = CtlGeti(wnd, CTL_LIST_CMD_GET_FOCUS, &err);
	top_index = CtlGeti(wnd, CTL_LIST_CMD_GET_TOPINDEX, &err);

	pCtlList->nScrollLeft -= 3;

    CtlDo(wnd, CTL_LIST_CMD_DO_ITEMDRAW, cur_focus-top_index, 0, &err);
}

/** 
* @brief        Draws item focus.
* 
* @param[in]    wnd			The window handle
* @param[in]    line		The line index
* @param[out]   pErr        Pointer to error value
* 
* @retval       void
* @remarks      Draws item focus using the line index passed
*
* @if HISTORY
*         18-Sep-2006 madholic Initiated
* @endif
*/
void CtlListDrawItemFocus(WND wnd, int line, int* pErr)
{
	rect rtItem;
	COLORVAL old_color;
	REXY_CTL_LIST* pCtlList = (REXY_CTL_LIST*)CtlGetStruct(wnd);

	CtlListGetItemRect(wnd, line, &rtItem);

	// Process focus
	if(CtlListIsFocusLine(wnd, pCtlList, line))
	{
		old_color = GdiSetFgColor(wnd, COLOR_RED);
		GdiRectangle(wnd, &rtItem);
		GdiSetFgColor(wnd, old_color);
	}

	// Decorates item interval 
	GdiDrawColorLine(wnd, rtItem.left, rtItem.bottom+1, rtItem.right, rtItem.bottom+1, COLOR_BACKGROUNDGRAY);
}

/** 
* @brief        Do draws item by calling each callback function.
* 
* @param[in]    wnd			The window handle
* @param[in]    line		The line index
* @param[out]   pErr        Pointer to error value
* 
* @retval       void
* @remarks      Do draws item by calling each callback function using the line index
*
* @if HISTORY
*         18-Sep-2006 madholic Initiated
* @endif
*/
void CtlListDrawItem(WND wnd, int line, int* pErr)
{
	int col;
	REXY_CTL_LIST* pCtlList = (REXY_CTL_LIST*)CtlGetStruct(wnd);

	// draw focus callback function
	(*pCtlList->pfnDrawItemFocus)(wnd, line, pCtlList->pData);

	// draw item callback function
//	(*pCtlList->pfnDrawItem)(wnd, line, pCtlList->pData);

	// draw column callback function
	for(col=0; col < pCtlList->column_num; col++)
		(*pCtlList->pfnDrawColumn)(wnd, line, col, pCtlList->pData);
}

/** 
* @brief        Draws item image by default.
* 
* @param[in]    wnd			The window handle
* @param[in]    nLine		The line index
* @param[in]    bitmapN		The bitmap pointer of the normal image
* @param[in]    bitmapS		The bitmap pointer of the selected image
* @param[out]   pErr        Pointer to error value
* 
* @retval       void
* @remarks      Draws item image using line and column index and bitmap image pointer. This is default action
*
* @if HISTORY
*         18-Sep-2006 madholic Initiated
* @endif
*/
void CtlListDrawItemImage(WND wnd, int nLine, int col, RxBITMAP* bitmapN, RxBITMAP* bitmapS, int *pErr)
{
	int img_x, img_y;
	rect col_rect;
	COLORVAL old_bcolor;

	REXY_CTL_LIST* pCtlList = (REXY_CTL_LIST*)CtlGetStruct(wnd);

	CtlListGetColumnRect(wnd, nLine, col, &col_rect);

	img_x = col_rect.left + pCtlList->column_margin[col].left;
	img_y = col_rect.top + pCtlList->column_margin[col].top;

	if(CtlListIsFocusLine(wnd, pCtlList, nLine))
	{
		old_bcolor = GdiSetBkColor(wnd, pCtlList->item_focus_style.bcolor);
		GdiDrawBitmap(wnd, bitmapS, 0, 0, img_x, img_y,
						bitmapS->width, bitmapS->height);
	}
	else
	{
		old_bcolor = GdiSetBkColor(wnd, pCtlList->item_nfocus_style.bcolor);
		GdiDrawBitmap(wnd, bitmapN, 0, 0, img_x, img_y,
						bitmapN->width, bitmapN->height);
	}

    GdiSetBkColor(wnd, old_bcolor);
}

/** 
* @brief        Handles default key down process.
* 
* @param[in]    wnd			The window handle
* @param[in]    nModifier	The changed value by key down action
* @param[out]   pErr        Pointer to error value
* 
* @retval       void
* @remarks      If custom process is needed, use callback function instead of this function
*
* @if HISTORY
*         18-Sep-2006 madholic Initiated
* @endif
*/
void CtlListProcessKeyDown(WND wnd, int nModifier, int *pErr)
{
	int err;
    int nCurrFocus;
    int nNextFocus;
    int nItemNum;
    int nLineNum;
    int nTopIndex;
    bool bDrawAll = FALSE;
	REXY_CTL_LIST* pCtlList = (REXY_CTL_LIST*)CtlGetStruct(wnd);

	pCtlList->nScrollLeft = 0;

	// kill timer
	UsrKillTimer(wnd, REXY_LIST_TIME_SQUARE_SCROLLING_TIMER );
	pCtlList->bScrollTimer = FALSE;

    nCurrFocus  = CtlGeti(wnd, CTL_LIST_CMD_GET_FOCUS, &err);
    nItemNum    = CtlGeti(wnd, CTL_LIST_CMD_GET_ITEMNUM, &err);
    nLineNum    = CtlGeti(wnd, CTL_LIST_CMD_GET_LINENUM, &err);
    nTopIndex   = CtlGeti(wnd, CTL_LIST_CMD_GET_TOPINDEX, &err);

    nNextFocus = (nCurrFocus + nItemNum + nModifier) % nItemNum;
    CtlSeti(wnd, CTL_LIST_CMD_SET_FOCUS, nNextFocus, &err);

    if(nNextFocus < nTopIndex)
    {
        bDrawAll = TRUE;
        nTopIndex = nNextFocus;
        CtlSeti(wnd, CTL_LIST_CMD_SET_TOPINDEX, nTopIndex, &err);
    }
    else if(nNextFocus > nTopIndex + nLineNum - 1)
    {
        bDrawAll = TRUE;
        nTopIndex = nNextFocus - nLineNum +1;
        CtlSeti(wnd, CTL_LIST_CMD_SET_TOPINDEX, nTopIndex, &err);
    }

    if(bDrawAll)
        CtlListDraw(wnd, &err);
    else
    {
        CtlDo(wnd, CTL_LIST_CMD_DO_ITEMDRAW, nCurrFocus-nTopIndex, 0, &err);
        CtlDo(wnd, CTL_LIST_CMD_DO_ITEMDRAW, nNextFocus-nTopIndex, 0, &err);
    }
}

/** 
* @brief        Main message handler of list control.
* @param[in]    wnd         Pointer to control window
* @param[in]    msg        Message to handle
* @param[in]    wparam      wparam of message : control id
* @param[in]    lparam      lparam of message : control style
* @retval       RxErrNone   If nothing to handle or no error occurred
* @retval       WND         If message was AWM_CREATE and control was successfully created
* @remarks      This function handles messages for list control.\n
*               This function is called by control core if some messages is needed to be handled by list control.
* @if HISTORY
*         9-Jan-2006 alscar Initiated
* @endif
*/
int CtlListProc(WND wnd, int msg, int wparam, int lparam)
{
	int err;
    int handled = FALSE;

    switch(msg)
    {
    case AWM_CREATE:
		CtlListCreate(wnd);
        handled = TRUE;
		break;
	case AWM_DESTROY:
		CtlListDestroy(wnd, &err);
        handled = TRUE;
		break;
    case AWM_PAINT:
		CtlListDraw(wnd, &err);
        handled = TRUE;
        break;
	case AWM_SETFOCUS:
		break;
	case AWM_KILLFOCUS:
		break;
    case AWM_KEYDOWN:
        if(wparam == VK_USER_UP)
        {
            CtlListProcessKeyDown(wnd, -1, &err);
            handled = TRUE;
            break;
        }
        else
        if(wparam == VK_USER_DOWN)
        {
            CtlListProcessKeyDown(wnd, 1, &err);
            handled = TRUE;
            break;
        }
       break;
	case AWM_TIMER:
		CtlListHandleTimer(wnd);
        handled = TRUE;
		break;

    default:
        break;
    }

    return handled;
}


/** 
* @brief        Callback message hanlder of list control.
* @param[in]    wnd         Pointer to control window
* @param[in]    nMsg        Message to handle
* @param[in]    wparam      wparam of message
* @param[in]    lparam      lparam of message
* @param[in]    bIsPre      0 if functions is called for pre-callback\n
*                           Besides 0 if function called for post-callback
* @param[out]   pErr        Pointer to error value
* @retval       RxErrNone   If nothing to handle or no error occurred
* @remarks      This callback function called after handling some messages.
* @if HISTORY
*         9-Jan-2006 alscar Initiated
* @endif
*/
int CtlListCBProc(WND wnd, int nMsg, int wparam, int lparam, int bIsPre, int *pErr)
{
    switch(nMsg)
    {
    case AWM_CREATE:
        return CtlListCreatePostCB(wnd, pErr);

    case AWM_DESTROY:
        return CtlListDestroyPreCB(wnd, pErr);
    }

    return RxErrNone;
}

/** 
* @brief        Command handler of list control.
* @param[in]    wnd     Pointer to control window
* @param[in]    nCmd    Command ID
* @param[in]    wParam  wparam of command
* @param[in]    lParam  lparam of command
* @param[out]   pErr    Pointer to error value
* @retval       Varies by command ID
* @remarks      This function commands to list control to do various functions.\n
*               By command ID list control set its inner member value or,
*               returns its inner member value to whom it called.\n
*               Also does some functions such as drawing by specified command ID.
* @if HISTORY
*         9-Jan-2006 alscar Initiated
* @endif
*/
int CtlListCmdProc(WND wnd, int nCmd, int wParam, int lParam, int *pErr)
{
    int nReturn = 0;
    REXY_CTL_LIST *pCtlList;

    pCtlList = (REXY_CTL_LIST*)CtlGetStruct(wnd);
    CHECK_NULLPTR(pCtlList, 0, *pErr);

    switch(nCmd)
    {
	case CTL_LIST_CMD_SET_FONT:
		pCtlList->font = wParam;
		break;
		
	case CTL_LIST_CMD_SET_LINE_HEIGHT:
		pCtlList->nLineHeight = wParam;
		PrvListCtlSetLineNum(pCtlList);
		break;

    case CTL_LIST_CMD_SET_ITEMNUM:
        pCtlList->nItemNum = wParam;
        break;

    case CTL_LIST_CMD_SET_FOCUS:
        pCtlList->nFocus = wParam;
        break;

    case CTL_LIST_CMD_SET_TOPINDEX:
        pCtlList->nTopIndex = wParam;
        break;

	case CTL_LIST_CMD_SET_USE_TEXTSCROLL:
		if(pCtlList->column_num < 3)
			pCtlList->bUseTextScroll = wParam;
		break;

	case CTL_LIST_CMD_SET_COLUMN_NUM:
		pCtlList->column_num = wParam;
		if(pCtlList->column_num > 2)
			pCtlList->bUseTextScroll = FALSE;
		break;

	case CTL_LIST_CMD_SET_TEXT_ALIGN:
		pCtlList->item_focus_style.text_align = wParam;
		pCtlList->item_nfocus_style.text_align = lParam;
		break;

	case CTL_LIST_CMD_SET_CTRL_BGCOLOR:
		pCtlList->bgcolor_control = wParam;
		break;

	case CTL_LIST_CMD_SET_LINE_INTERVAL:
		pCtlList->nLineInterval = wParam;
		break;

	case CTL_LIST_CMD_SET_CTRL_POS:
		PrvListCtlSetCtrlPos(pCtlList, (rect*)lParam, pErr);
        PrvListCtlSetWnd(wnd, pErr);
		break;

	case CTL_LIST_CMD_SET_COLUMN_POS:
		if(wParam >= pCtlList->column_num || wParam < 0)
			break;
		
		pCtlList->column_pos[wParam] = lParam;
		break;

    case CTL_LIST_CMD_SET_COLUMN_MARGIN:
        PrvListCtlSetColumnMargin(pCtlList, wParam, (rect*)lParam, pErr);
        break;
		
	// callback
	case CTL_LIST_CMD_SET_DRAW_ITEM_FOCUS_CB:
		pCtlList->pfnDrawItemFocus = (CtlListDrawItemFocusCB)lParam;
		break;

	case CTL_LIST_CMD_SET_DRAW_ITEM_CB:
		pCtlList->pfnDrawItem = (CtlListDrawItemCB)lParam;
		break;

	case CTL_LIST_CMD_SET_DRAW_COLUMN_CB:
		pCtlList->pfnDrawColumn = (CtlListDrawColumnCB)lParam;
		break;

	case CTL_LIST_CMD_SET_KEY_DOWN_CB:
		pCtlList->pfnProcessKeyDown = (CtlListProcessKeyDownCB)lParam;
		break;

    case CTL_LIST_CMD_SET_DRAWCBDATA:
        pCtlList->pData = (void*)lParam;
        break;

    case CTL_LIST_CMD_GET_ITEMNUM:
        nReturn = pCtlList->nItemNum;
        break;

    case CTL_LIST_CMD_GET_FOCUS:
        nReturn = pCtlList->nFocus;
        break;

    case CTL_LIST_CMD_GET_TOPINDEX:
        nReturn = pCtlList->nTopIndex;
        break;

    case CTL_LIST_CMD_GET_LINENUM:
        nReturn = pCtlList->nLineNum;
        break;

    case CTL_LIST_CMD_DO_DRAW:
		CtlDraw(wnd, pErr);
        break;

    case CTL_LIST_CMD_DO_ITEMDRAW:
        CtlListDrawLine(wnd, wParam, pErr);
        break;
    }

    return nReturn;
}

/** 
* @brief        Boundary check callback function.
* @param[in]    wnd     Pointer to control window
* @param[in]    nCmd    Command ID
* @param[in]    wParam  wparam of command
* @param[in]    lParam  lparam of command
* @param[out]   pErr    Pointer to error value
* @retval       TRUE    If boundary check is successful
* @retval       FALSE   If boundary check fails
* @remarks      This function is called by control core to check boundary 
*               before setting specified value to list conrol data.
* @if HISTORY
*         9-Jan-2006 alscar Initiated
* @endif
*/
int CtlListProcChk(WND wnd, int nCmd, int wParam, int lParam, int *pErr)
{
//    REXY_CTL_LIST *pCtlList;
//
//    pCtlList = PrvListCtlGetDataP(wnd, pErr);
//    CHECK_NULLPTR(pCtlList, FALSE, *pErr);
//
//    switch(nCmd)
//    {
//    case CTL_LIST_CMD_SET_ITEMNUM:
//        if(wParam > 0)
//            return TRUE;
//        break;
//
//    case CTL_LIST_CMD_SET_FOCUS:
//    case CTL_LIST_CMD_SET_TOPINDEX:
//        CHECK_BND(wParam, 0, pCtlList->nItemNum, *pErr, FALSE);
//        CHECK_ERR_IF_EQUAL(*pErr, CTLERR_NONE, TRUE);
//        break;
//
//    case CTL_LIST_CMD_SET_DRAWCB:
//    case CTL_LIST_CMD_SET_DRAWCBDATA:
//        if(lParam)
//            return TRUE;
//        break;
//
//    case CTL_LIST_CMD_SET_RECTMARGIN:
//        PrvListCtlCheckMargin((REXY_CTL_LIST_MARGIN*)lParam, pErr);
//        CHECK_ERR_IF_EQUAL(*pErr, CTLERR_NONE, TRUE);
//        break;
//
//    case CTL_LIST_CMD_GET_RECTMARGIN_TEXT:
//        CHECK_BND(wParam, 0, RalScreenGetHeight(), *pErr, FALSE);
//        CHECK_ERR_IF_EQUAL(*pErr, CTLERR_NONE, TRUE);
//        break;
//    }
//
    return FALSE;
}





/****************************************************************************/
/*  Unit Test Module                                                        */
/****************************************************************************/
#ifdef UNIT_TEST
#define UNIT_TEST_ERR(err, n, str)  if(err != RxErrNone) \
                                    { TrcPrintF("UNIT TEST #%d : %s : FAILED\n", n, str); return; } \
                                    else \
                                    { TrcPrintF("UNIT TEST #%d : %s : OK\n", n, str); n++; }

#define UNIT_TEST_ITEMNUM       8
#define UNIT_TEST_FOCUS         4
#define UNIT_TEST_TOPINDEX      2
#define UNIT_TEST_ITEM_MARGIN   4

void CtlListTestUnitDummyCB(WND hWnd, int nLine, rect rtLineRect, void *pData)
{
    //Do nothing
}

void CtlListTestUnit(WND hWnd)
{
    //REXY_CTL *pCtl;
	WND wnd;
    int err;

    int nTestNum = 1;
    rect sMargin = {0, 0, 0, 0};
    rect sMarginText = {2, 2, 2, 2};

    //Create
    wnd = CtlCreate(hWnd, CTL_TYPE_LIST, 0, NULL, &err);
    UNIT_TEST_ERR(err, nTestNum, "Create");

    //Set : Item Number
    CtlSeti(wnd, CTL_LIST_CMD_SET_ITEMNUM, UNIT_TEST_ITEMNUM, &err);
    UNIT_TEST_ERR(err, nTestNum, "Set : Item Number");

    //Set : Focus
    CtlSeti(wnd, CTL_LIST_CMD_SET_FOCUS, UNIT_TEST_FOCUS, &err);
    UNIT_TEST_ERR(err, nTestNum, "Set : Focus");

    //Set : Focus
    CtlSeti(wnd, CTL_LIST_CMD_SET_TOPINDEX, UNIT_TEST_TOPINDEX, &err);
    UNIT_TEST_ERR(err, nTestNum, "Set : Focus");

    //Set : Draw Callback
    CtlSetPtr(wnd, CTL_LIST_CMD_SET_DRAWCB, (void*)CtlListTestUnitDummyCB, &err);
    UNIT_TEST_ERR(err, nTestNum, "Set : Draw Callback");

    //Set : Draw Callback Data
    CtlSetPtr(wnd, CTL_LIST_CMD_SET_DRAWCBDATA, (void*)&nTestNum, &err);
    UNIT_TEST_ERR(err, nTestNum, "Set : Draw Callback Data");

    //Set : Rectangle Margin
//    CtlSetPtr(wnd, CTL_LIST_CMD_SET_RECTMARGIN, (void*)&sMargin, &err);
//    UNIT_TEST_ERR(err, nTestNum, "Set : Rectangle Margin");

    //Set : Item Margin
//    CtlSeti(wnd, CTL_LIST_CMD_SET_RECTMARGIN_TEXT, (void*)&sMarginText, &err);
//    UNIT_TEST_ERR(err, nTestNum, "Set : Rectangle Item Margin");

    //Get : Item Number
    CtlGeti(wnd, CTL_LIST_CMD_GET_ITEMNUM, &err);
    UNIT_TEST_ERR(err, nTestNum, "Get : Item Margin");

    //Get : Focus
    CtlGeti(wnd, CTL_LIST_CMD_GET_FOCUS, &err);
    UNIT_TEST_ERR(err, nTestNum, "Get : Focus");

    //Get : Top Index
    CtlGeti(wnd, CTL_LIST_CMD_GET_TOPINDEX, &err);
    UNIT_TEST_ERR(err, nTestNum, "Get : Top Index");
    
    //Get : Line Number
    CtlGeti(wnd, CTL_LIST_CMD_GET_LINENUM, &err);
    UNIT_TEST_ERR(err, nTestNum, "Get : Line Number");

    //Get : Item Margin
    CtlGeti(wnd, CTL_LIST_CMD_GET_ITEMMARGIN, &err);
    UNIT_TEST_ERR(err, nTestNum, "Get : Item Margin");

    //Destroy
    CtlDestroy(wnd, &err);
    UNIT_TEST_ERR(err, nTestNum, "Destroy");
}
#endif

/** @} */ // End of List Control
