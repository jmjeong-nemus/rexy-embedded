/*   
 * $Id: rexy_ctl_list.h 491 2006-01-11 16:32:34Z madholic $
 * Copyright(c) 2004-6, Nemustech Inc, All Rights Reserved.    
 *   
 * This file is a part of Rexy Embedded.    
 *   
 */

#ifndef __REXY_CTL_LIST_H__
#define __REXY_CTL_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @file */
	
#include "rexy.h"
#include "rexy_ctl_core.h"

/**
 * @defgroup ListCtlGroup Ctrl List Control
 * Ctrl List Control
 * @ingroup ControlGroup
 * @{
 */

#define CTL_LIST_MAX_COLUMN						5

#define CTL_LIST_MARGIN_TOP			0	///< List Control의 상단 여백
#define CTL_LIST_MARGIN_LEFT		0	///< List Control의 좌측 여백
#define CTL_LIST_MARGIN_RIGHT		0	///< List Control의 우측 여백
#define CTL_LIST_MARGIN_BOTTOM		0	///< List Control의 하단 여백

// text item's inner margin
#define CTL_LIST_MARGIN_TEXT_LEFT		0
#define CTL_LIST_MARGIN_TEXT_TOP		0
#define CTL_LIST_MARGIN_TEXT_RIGHT		0
#define CTL_LIST_MARGIN_TEXT_BOTTOM		0

// image item's inner margin
#define CTL_LIST_MARGIN_IMG_LEFT		0
#define CTL_LIST_MARGIN_IMG_TOP			0
#define CTL_LIST_MARGIN_IMG_RIGHT		0
#define CTL_LIST_MARGIN_IMG_BOTTOM		0

/**
 * @enum ListControlAttr
 * List Control을 설정하고 제어하기 위한 명령어 집합
 */
enum ListControlAttr{
	//Start offset 0x0100 for no reason.
    CTL_LIST_CMD_SET_ITEMNUM		= 0x0100,	///< Set list item number
    CTL_LIST_CMD_SET_FOCUS,						///< Set focus item
    CTL_LIST_CMD_SET_TOPINDEX,					///< Set top index
	CTL_LIST_CMD_SET_FONT,						///< Set list control's font
	CTL_LIST_CMD_SET_LINE_HEIGHT,				///< Set line height
	CTL_LIST_CMD_SET_CTRL_BGCOLOR,				///< Set the control's background color
	CTL_LIST_CMD_SET_COLUMN_MARGIN,				///< Set the column margin
	CTL_LIST_CMD_SET_LINE_INTERVAL,				///< Set the line interval between items
	
	CTL_LIST_CMD_SET_DRAW_ITEM_FOCUS_CB,		///< Set draw item focus callback function pointer
	CTL_LIST_CMD_SET_DRAW_ITEM_CB,				///< Set draw item callback function pointer
	CTL_LIST_CMD_SET_DRAW_COLUMN_CB,			///< Set draw column callback function pointer
	CTL_LIST_CMD_SET_KEY_DOWN_CB,				///< Set key down process callback function pointer

    CTL_LIST_CMD_SET_DRAWCBDATA,				///< Set data callback function pointer
	CTL_LIST_CMD_SET_CTRL_POS,					///< Set control window position

	CTL_LIST_CMD_SET_USE_TEXTSCROLL,			///< Set use text scrolling. 이 속성은 column 개수가 2보다 큰 경우에만 동작함

	CTL_LIST_CMD_SET_COLUMN_NUM,				///< Set column number
	CTL_LIST_CMD_SET_COLUMN_POS,				///< Set column position

	CTL_LIST_CMD_SET_TEXT_ALIGN,				///< Set the text alignment of the column

    CTL_LIST_CMD_GET_ITEMNUM,					///< Get item number
    CTL_LIST_CMD_GET_FOCUS,						///< Get focus item
    CTL_LIST_CMD_GET_TOPINDEX,					///< Get top index
    CTL_LIST_CMD_GET_LINENUM,					///< Get line number
    CTL_LIST_CMD_GET_RECTMARGIN_TEXT,			///< Get margin for text item
	CTL_LIST_CMD_GET_RECTMARGIN_IMG,			///< Get margin for image item

    CTL_LIST_CMD_DO_DRAW,						///< Execute control drawing totally
    CTL_LIST_CMD_DO_ITEMDRAW,					///< Execute item drawing

    CTL_LIST_CMD_LAST							//Does nothing.
};

/** Drawing Callback function prototype */
typedef int (*CtlListDrawItemFocusCB)(WND hWnd, int nLine, void *pData);
typedef int (*CtlListDrawItemCB)(WND hWnd, int line, void* pData);
typedef int (*CtlListDrawColumnCB)(WND hWnd, int line, int column, void* pData);
typedef int (*CtlListProcessKeyDownCB)(WND hWnd, int nModifier, void *pData);


/** @} */ // end of listctl group

typedef struct _REXY_CTL_LIST_ITEM_FOCUS_STYLE
{
	COLORVAL		fcolor;
	COLORVAL		bcolor;
	int				text_style;
	int				text_align;
	int				text_extent;
} REXY_CTL_LIST_ITEM_FOCUS_STYLE;

typedef struct _REXY_CTL_LIST_ITEM_NFOCUS_STYLE
{
	COLORVAL		fcolor;
	COLORVAL		bcolor;
	int				text_style;
	int				text_align;
	int				text_extent;
} REXY_CTL_LIST_ITEM_NFOCUS_STYLE;

typedef struct _REXY_CTL_LIST
{
	ruint8					style;
    int                     nItemNum;
    int                     nFocus;
    int                     nTopIndex;
    int                     nLineNum;
    int                     nLineHeight;

	int						font;
	COLORVAL				bgcolor_control;			// control's back color

	// column
	int						column_num;
	int						column_pos[CTL_LIST_MAX_COLUMN];
	rect					column_margin[CTL_LIST_MAX_COLUMN];

	// scrolling text
	bool					bUseTextScroll;
	bool					bScrollTimer;
	int						nScrollLeft;

	// item
	int						nLineInterval;		// interval between list control's items

	REXY_CTL_LIST_ITEM_FOCUS_STYLE  item_focus_style;
	REXY_CTL_LIST_ITEM_NFOCUS_STYLE item_nfocus_style;

	// position
	rect rtCtrlWndPos;		// control window position (absolute coordinates)	

    CtlListDrawItemFocusCB		pfnDrawItemFocus;
	CtlListDrawItemCB			pfnDrawItem;
    CtlListDrawColumnCB			pfnDrawColumn;
    CtlListProcessKeyDownCB		pfnProcessKeyDown;

    void*						pData;
} REXY_CTL_LIST;


REXY_CTL_LIST*	CtlListGetStruct(WND wnd);

void	CtlListDrawItem(WND wnd, int line, int* pErr);
void	CtlListDrawItemFocus(WND wnd, int line, int* pErr);
void	CtlListDrawItemText(WND wnd, int nLine, int col, char* pItemText, int *pErr);
void	CtlListDrawItemImage(WND wnd, int nLine, int col, RxBITMAP* bitmapN, RxBITMAP* bitmapS, int *pErr);
void	CtlListProcessKeyDown(WND wnd, int nModifier, int *pErr);

int		CtlListGetItemTextWidth(WND wnd);
void	CtlListSetItemTextWidth(REXY_CTL_LIST* pCtlList, int width);
void	CtlListHandleTimer(WND wnd);
int		CtlListTextOutAttrClip(WND wnd, rint32 x, rint32 y, char * string, int length);
bool	CtlListIsFocusLine(WND wnd, REXY_CTL_LIST* pCtlList, int line);

void	CtlListGetItemRect(WND wnd, int nLineIndex, rect *pRect);
void	CtlListGetColumnRect(WND wnd, int nLine, int col, rect* col_rect);

int CtlListProc(WND wnd, int nMsg, int wparam, int lparam);
int CtlListCBProc(WND wnd, int nMsg, int wparam, int lparam, int bIsPre, int *pErr);
int CtlListCmdProc(WND wnd, int nCmd, int wParam, int lParam, int *pErr);
int CtlListChkProc(WND wnd, int nCmd, int wParam, int lParam, int *pErr);


#ifdef UNIT_TEST
void list_testUnit();
#endif  // UNIT_TEST

#ifdef __cplusplus
}
#endif	// __cplusplus
#endif	// __REXY_CTL_LIST_H__
