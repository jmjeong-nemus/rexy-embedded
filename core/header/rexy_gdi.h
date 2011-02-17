/*
 * $Id: rexy_gdi.h 5441 2005-07-21 04:31:49Z klenui $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 * Device dependent한 함수 및 상수가 상수가 정의되어 있다.
 *
 */

#ifndef __REXY_GDI_H__
#define __REXY_GDI_H__

#ifdef __cplusplus
extern "C" {
#endif

    /** @file */

    /*-------------------------------------------------------------------------------*/
    /* Structures */
    /*-------------------------------------------------------------------------------*/

#define MkGdiErr(x) ( RxErrGdiClass | x )
#define MAX_SCREEN_LOCK_COUNT	5
#define MAX_PATH_LEN	0x40

    /*-------------------------------------------------------------------------------*/
    /* Function Definition */
    /*-------------------------------------------------------------------------------*/
    RxErr		GdiGetScreenInfo(rint32* width, rint32* height);
	ruint16		*GdiGetScreenPtr(void);
    RxErr		GdiLockScreen(WND panel);
    RxErr		GdiUnlockScreen(WND panel);
	ruint16		GdiGetScreenLockCount(void);
	void		GdiFlushAll(WND wnd);
	void		GdiFlushPart(WND wnd, rect *boundary);

	/* align */
	int			GdiSetTextAlign(int align);
	int			GdiGetTextAlign(void);

	/* space */
	int			GdiSetTextSpace(int space);
	int			GdiGetTextSpace(void);
	
	/* style */
	int			GdiSetTextStyle(int style);
	int			GdiGetTextStyle(void);

	/* extent */
	int			GdiSetTextExtent(int extent);
	int			GdiGetTextExtent(void);

    /* color */
    COLORVAL	GdiSetBkColor(WND wnd, COLORVAL color);
    COLORVAL	GdiSetFgColor(WND wnd, COLORVAL color);
    COLORVAL	GdiGetBkColor(WND wnd);
    COLORVAL	GdiGetFgColor(WND wnd);

    /* pixel */
    COLORVAL	GdiSetPixel(WND wnd, rint32 x, rint32 y, COLORVAL color);
    COLORVAL	GdiGetPixel(WND wnd, rint32 x, rint32 y);

    /* line */
    void		GdiDrawLineWidth(WND wnd, rint32 x1, rint32 y1, rint32 x2, rint32 y2, int width);
    void		GdiDrawLine(WND wnd, rint32 x1, rint32 y1, rint32 x2, rint32 y2);
    void		GdiDrawColorLine(WND wnd, rint32 x1, rint32 y1, rint32 x2, rint32 y2, COLORVAL color);

    /* rectangle */
    void		GdiRectangle(WND wnd, rect * src);

    /* text */
    int			GdiTextOut(WND wnd, char * string, int length, rint32 x, rint32 y);
    int			GdiExtTextOut(WND wnd, char * string, int length, rint32 x, rint32 y, int style, rect* boundary, int space);
    int			GdiExtTextOutEx(WND wnd, char * string, int length, rint32 x, rint32 y, int style);
    void		GdiPaintString(WND wnd, rint32 x, rint32 y, char *str, int len, int space);
    int			GdiDrawTextOut(WND hWnd, rint32 x, rint32 y, int width, char *str, int str_len, int hAlign);
    int			GdiDrawTextOutLF(WND hWnd,rint32 x, rint32 y, int width, char *str, int str_len, int hAlign);
	int			GdiTextOutAttr(WND wnd, rint32 x, rint32 y, char * string, int length);

    /* rect */
    int			GdiSetRect(rect *pRect, int left, int top, int right, int bottom);	//right and bottom should be the position
																					//these are NOT 'width' and 'height'
    int			GdiPtInRect(rect *lprc, Point *pt);
    int			GdiMoveRect(rect *pRect, int dx, int dy);
    void		GdiFillRect(WND wnd, rect *src, COLORVAL fill_color);
    void		GdiInvertRect(WND wnd, rect *src);
    int			GdiIntersectRect(rect *result, rect *src1, rect *src2);
    RxErr		GdiInflateRect(rect *pRect, int dx, int dy);
	RxErr		GdiDeflateRect(rect *pRect, int dx, int dy);
    void		GdiInvalidateRect(rect *invalid);
	RxErr		GdiGetClientRect(WND wnd, rect* rt);
	void		GdiGetWindowRect(WND wnd, rect* rt);

    /* bitmap */
    RxErr		GdiDrawBitmap(WND wnd, RxBITMAP * bitmap, int src_x, int src_y, int dst_x, int dst_y, int width, int height);
    void		GdiBitBlt(WND dest_wnd, int dest_x, int dest_y, WND src_wnd, int src_x, int src_y, int width, int height, int op, int alpha);
    void		GdiStretchBlt(	WND dest_wnd, int dx, int dy,
                         WND src_wnd, int sx, int sy,
                         int d_width, int d_height, int s_width, int s_height, int op);


    /* flush */
    void		GdiValidateAll(void);

    /* screen restore */
	int			GdiScreenGetMemSize(void);
	int			GdiScreenGetMemSizeArea(rect *pArea);
	bool		GdiScreenSave(rect *pArea, void* pData);
	bool		GdiScreenRestore(void* pData);

	/* */
    void		GdiDimOut(WND wnd, rect *dest);

	// Added by Joshua for convinience's sake
	void		GdiClrScreen(WND wnd, COLORVAL color);


	
#ifdef __cplusplus
}
#endif	// __cplusplus

#endif	// __REXY_GDI_H__
