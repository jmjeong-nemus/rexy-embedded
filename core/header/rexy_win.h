/*
 * $Id: rexy_win.h 5441 2005-07-21 04:31:49Z klenui $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 * Device dependent한 함수 및 상수가 상수가 정의되어 있다.
 *
 */

#ifndef __REXY_WIN_H__
#define __REXY_WIN_H__

#ifdef __cplusplus
extern "C" {
#endif

    /** @file */

    /*======================================================================*/
    /* Public APIs								*/
    /*======================================================================*/
	WND		WinCreateWindow(WINPROC wndproc, int x, int y, int w_width, int w_height, int mem);
    WND		WinCreateWindowEx(WINPROC wndproc, int x, int y, int w_width, int w_height, int mem, int wparam, bool visible);
    void	WinInvalidateWindow(WND panel, int erase);
    void	WinUpdateWindow(WND panel, int status);
    void	WinShowWindow(WND panel, int status);
    void	WinUpdateAllWindow(void);
    int		WinDestroyWindow(WND panel);

    int		WinSetWindowPos(WND panel, int x, int y, int width, int height, int flag);

/**
 * @brief		Get the window that has focus.
 * @return		window handle that has focus
 * @remarks		
 * @see			
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
 WND	WinGetSelection(void);

    WND		WinGetFirstControl(WND panel, int bTabStop);
    WND		WinGetLastControl(WND panel, int bTabStop);
    WND		WinAddControl(WND panel, int type, int id, int style, rect * location, WINPROC wndproc);
    void	WinSetSelection(WND panel);
    WND		WinGetNextControl(WND panel, WND control, int bTabStop);
    WND		WinGetPrevControl(WND panel, WND control, int bTabStop);
    WND		WinGetSelectionControl(WND panel);
    WND		WinGetHiliteControl(WND panel);
    void	WinSetHiliteControl(WND panel, WND control);
    int		WinDestroyControl(WND control);
    int		WinDestroyAllControl(WND panel);
    void	WinUpdateControl(WND control, int erase);
    void	WinUpdateAllControl(WND panel, int erase);
    int		WinSetShowUpControl(WND panel);
    int		WinSetShowDownControl(WND panel);
	int		WinSetCurrentProc(WND panel, WINPROC WndProc);
	WINPROC	WinGetCurrentProc(WND panel);

/** 
 * @brief	Get boundary information of the specified window
 * @return	true on success, false otherwise
 *          Left, right , top, bottom value will be returned in boundary pointer
 * @ifdef HISTORY
 *			23-Jan-2008 Joshua created
 * @endif
 */
bool WinGetBoundary( WND hwnd, rect *boundary );

#ifdef __cplusplus
}
#endif	// __cplusplus

#endif//__REXY_WIN_H__
