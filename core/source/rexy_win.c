/*
 * $Id: rexy_win.c 650 2005-02-28 11:46:36Z klenui $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 * - implementations of window managing APIs
 *
 */

/** @file */

/**
 *  @defgroup winmgr Core Window Managing Service
 *  Window Management API Set
 *  @ingroup RexyECoreGroup
 *  @{
 */

#include "rexy.h"

/*======================================================================*/
/* Declarations								*/
/*======================================================================*/


/*======================================================================*/
/* Private								*/
/*======================================================================*/
/*======================================================================*/
/* Module Variables							*/
/*======================================================================*/
static WND g_head_wnd = NULL;
static WND g_focused_wnd = NULL;
static WND g_hilite_ctrl = NULL;

/*======================================================================*/
/* Implementation							*/
/*======================================================================*/
/**
 * @brief		Create the panel(parent) window.
 * @param[in]	wndproc	Window procedure. All events related to the window are processed.
 * @param[in]	x			Left-topmost X-coordinate.
 * @param[in]	y			Left-topmost Y-coordinate.
 * @param[in]	w_width	Width of window.
 * @param[in]	w_height	Height of Window.                       
 * @param[in]	mem		Flag to specify the window as "memory window" or "screen window".
 *					- WND_MEM :  Memory window
 *					- WND_SCREEN : Screen window.
 * @retval		the handle to new window		If function succeeds
 * @retval		NULL							If function fails
 * @remarks		Child windows are created by calling the WinAddControl() function.
 * @see			
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
*/
WND WinCreateWindow(WINPROC wndproc, int x, int y, int w_width, int w_height, int mem)
{
    return WinCreateWindowEx( wndproc, x, y, w_width, w_height, mem, 0, TRUE );
}

WND WinCreateWindowEx(WINPROC wndproc, int x, int y, int w_width, int w_height, int mem, int wparam, bool visible)
{
    WND new_wnd;
    rect clipping;
    int width, height;

    new_wnd = malloc_w(sizeof(WINDOW));

    if ( g_head_wnd == NULL )
    {
        g_head_wnd = new_wnd;
        new_wnd->prevwin = NULL;
    }
    else
    {
        new_wnd->prevwin = g_head_wnd;
        g_head_wnd->nextwin = new_wnd;
        g_head_wnd = new_wnd;
    }

    new_wnd->parentwin = NULL;
    new_wnd->nextwin = NULL;
    new_wnd->childwin = NULL;

    new_wnd->WndProc = wndproc;
	if ( visible ) new_wnd->window_visible = WND_VISIBLE;
    else new_wnd->window_visible = WND_INVISIBLE;

    new_wnd->fg_color = RGBVALTODEV(COLOR_BLACK);
    new_wnd->bg_color = RGBVALTODEV(COLOR_WHITE);
    new_wnd->nextfree = NULL;

    //Check Memory Window
    if ( mem == WND_MEM || mem == WND_CUSTOM_MEM)
    {
        new_wnd->window_type = WND_MEM_PANEL;
        new_wnd->dev = RalMemAlloc(sizeof(SCR_DEV));

        new_wnd->frame.left = 0;
        new_wnd->frame.top = 0;

        new_wnd->clip.left = new_wnd->frame.left = 0;
        new_wnd->clip.right = new_wnd->frame.right = w_width - 1;
        new_wnd->clip.top = new_wnd->frame.top = 0;
        new_wnd->clip.bottom = new_wnd->frame.bottom = w_height - 1;

        if ( new_wnd->dev == NULL )
        {
            free_w(new_wnd);
            return NULL;
        }

        new_wnd->dev->width = new_wnd->frame.right + 1;
        new_wnd->dev->height = new_wnd->frame.bottom + 1;

		if ( mem == WND_MEM )
		{
			new_wnd->dev->mem_addr = RalMemAlloc(w_width * w_height * sizeof(UNITCL));
			// jmjeong. 05-08-17. Correct incorrect meme_size
			// new_wnd->dev->mem_size = new_wnd->dev->width * new_wnd->dev->height * sizeof(UNITCL);
			new_wnd->dev->mem_size = w_width * w_height * sizeof(UNITCL);
		}
		else if ( mem == WND_CUSTOM_MEM )	//user will chain this mem_addr with manually allocated memory later, Joshua
		{
			new_wnd->dev->mem_addr = 0;		// should be linked by user
			new_wnd->dev->mem_size = w_width * w_height * sizeof(UNITCL);
		}
    }
    else
    {
        new_wnd->frame.left = x;
        new_wnd->frame.right = x + w_width - 1;
        new_wnd->frame.top = y;
        new_wnd->frame.bottom = y + w_height - 1;

        GdiGetScreenInfo(&width, &height);
        clipping.left = 0;
        clipping.right = width - 1;
        clipping.top = 0;
        clipping.bottom = height - 1;

        GdiIntersectRect(&(new_wnd->clip), &(new_wnd->frame), &clipping);

        new_wnd->window_type = WND_PANEL;
        new_wnd->dev = NULL;	// Using system buffer
    }
    if ( new_wnd->WndProc ) MsgPostMessage( new_wnd, AWM_CREATE, wparam, 0 );

	WinSetSelection(new_wnd);
    return new_wnd;
}

/**
 * @brief		Repaint the window.
 * @param[in]	panel		Invalidate the window. Can invalidate panel and child window.
 * @param[in]	erase		Set this flag to erase background.
 *				- ERASE_BKGND : Send the WM_ERASEBKGND.
 * @return		void
 * @remarks		Sends WM_PAINT message to window. It is identical to InvalidateRect API of WIN32.
 * @see			
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
void	WinInvalidateWindow(WND panel, int erase)
{
    if ( panel == NULL )
        return;

    if ( erase == ERASE_BKGND )
    {
        MsgPostMessage(panel, AWM_ERASEBKGND, 0, 0);
    }
}

/**
 * @brief		Repaint the specified window.
 * @param[in]	panel	Window handle to repaint. 
 * @param[in]	status	Set this flag to erase or leave specified window. System does not redraw window if NO_REDRAW flag is set.
 *				- ERASE_BKGND : Send WM_ERASEBKGND message to the window.
 *				- LEAVE_BKGND : Do not change the window background.
 *				- NO_REDRAW	: Do not redraw window.
 * @return		void
 * @remarks		Basically sends WM_PAINT message to specified window. 
 *				If specified window is panel window, it sends WM_UPDATECONTROL message to child windows to update controls.
 * @see			
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
void	WinUpdateWindow(WND panel, int status)
{
    if ( panel == NULL )
        return;

    if ( panel->window_type == WND_MEM_PANEL )
        return;

    // check window status.
    if ( panel->window_visible == WND_VISIBLE )
    {
        // parent is hidden. don't show control
        if ( (panel->window_type & CTRL) && (panel->parentwin->window_visible != WND_VISIBLE) )
            return;

        if ( status & ERASE_BKGND )
            MsgSendMessage(panel, AWM_ERASEBKGND, 0, 0);

        MsgSendMessage(panel, AWM_PAINT, 0, 0);

        if ( panel->window_type & PANEL )
            MsgSendMessage(panel, AWM_UPDATECONTROL, 0, 0);

        //if( !(status & NO_REDRAW) )
        //	GdiFlushAll(panel);
    }

    return;
}

/**
 * @brief		Show or hide window with the specified flag.
 * @param[in]	panel		Window handle to show or hide.
 * @param[in]	status		Specify the window status.
 *				- SHOW_PANEL : Show the window.
 *				- HIDE_PANEL : Hide the window. Window will lose focus unless HOLD_FOCUS flag is set.
 *				- LEAVE_BKGND : Do not change the background.
 *				- ERASE_BKGND : Send WM_ERASEBKGND message to the specified window.
 *				- HOLD_FOCUS : Use with HIDE_PANEL flag to keep focus even though window gets hidden.
 * @return		void
 * @remarks		
 * @see			
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
void	WinShowWindow(WND panel, int status)
{
    WND focused;
    WND child;

    if ( panel == NULL )
        return;

    if ( panel->window_type == WND_MEM_PANEL )
        return;

    if ( (status & HIDE_PANEL) && (status & SHOW_PANEL) )
        return;

    // check status command
    // hide window
    if ( status & HIDE_PANEL )
    {
        if ( panel->window_visible == WND_INVISIBLE )
            return;

        panel->window_visible = WND_INVISIBLE;

        // if we have focus and don't have to hold focus, then release it.
        if ( !(status & HOLD_FOCUS) )
        {
            focused = WinGetSelection();
            if ( (panel == focused) || (panel == focused->parentwin) )
                WinSetSelection(NULL);
        }

//        // if we have caret, then also release it.
//       if ( (AniCaretGetWnd() != NULL) && ((panel == AniCaretGetWnd()) || (panel == AniCaretGetWnd()->parentwin)) )
//            AniHideCaret();

        // if control, update parent.
        if ( panel->window_type & CTRL )
            WinUpdateWindow(panel->parentwin, ERASE_BKGND);

        // if panel, update all panel. we don't know what will happen.
        if ( panel->window_type & PANEL )
            WinUpdateAllWindow();

        MsgSendMessage(panel, AWM_SHOWWINDOW, 0, 0);

        if ( panel->window_type & PANEL )
        {
            child = panel->childwin;
            while ( child != NULL )
            {
                if ( child->window_visible == WND_VISIBLE )
                    MsgSendMessage(child, AWM_SHOWWINDOW, 0, 0);
                child = child->nextwin;
            }
        }

        return;
    }

    // show window
    if ( status & SHOW_PANEL )
    {
        panel->window_visible = WND_VISIBLE;
    }

    // check window status.
    if ( panel->window_visible == WND_VISIBLE )
    {
        if (WinGetSelection() != panel && !(panel->window_type & CTRL))
            WinSetSelection(panel);

        WinUpdateWindow(panel, status);

        MsgSendMessage(panel, AWM_SHOWWINDOW, 1, 0);

        if ( panel->window_type & PANEL )
        {
            child = panel->childwin;
            while ( child != NULL )
            {
                if ( child->window_visible == WND_VISIBLE )
                    MsgSendMessage(child, AWM_SHOWWINDOW, 1, 0);
                child = child->nextwin;
            }
        }
    }

    return;
}

/**
 * @brief		Update all windows.
 * @return		void
 * @remarks		It call WinUpdateWindow() function with ERASE_BKGND parameter for all windows.
 * @see			
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
void	WinUpdateAllWindow(void)
{
    WND panel;
    WND next;

    panel = g_head_wnd;
    if ( panel == NULL )
        return;

    while ( panel->nextwin != NULL )
        panel = panel->nextwin;

    while(panel)
    {
        next = panel->prevwin;
        WinUpdateWindow(panel, ERASE_BKGND);
        panel = next;
    }

    return;
}

/**
 * @brief		Destroy the window.
 * @param[in]	panel	Window handle to destroy.
 * @return		Always returns 0.
 * @remarks		
 * @see			
 */
int		WinDestroyWindow(WND panel)
{
    if ( panel == NULL )
        return 0;

    // Send AWM_DESTROY to itself
    MsgSendMessage(panel, AWM_DESTROY, 0, 0);

    // destroy child control
    if ( (panel->window_type & PANEL) && (panel->window_type != WND_MEM_PANEL) )
        WinDestroyAllControl(panel);

    // show the window below this panel
    if ( (panel->window_type & PANEL) && (panel->window_type != WND_MEM_PANEL) )
        WinInvalidateWindow(panel->nextwin, ERASE_BKGND);

    // Release allocated memory
    if ( panel->dev )
	{
		if ( panel->window_type == WND_MEM_PANEL )
		{
			if ( panel->dev->mem_addr )
				MemFree( panel->dev->mem_addr );
		}
        MemFree(panel->dev);
	}

    // unlink
    if ( panel->prevwin )
        panel->prevwin->nextwin = panel->nextwin;
    if ( panel->nextwin )
        panel->nextwin->prevwin = panel->prevwin;
    if ( panel == g_head_wnd )
        g_head_wnd = g_head_wnd->nextwin;

    // remove focus. Do we have to send AWM_KILLFOCUS message ?
    if ( g_focused_wnd == panel)
	{
		if (panel->parentwin)
			g_focused_wnd = panel->parentwin;
		else if (panel->prevwin)
			g_focused_wnd = panel->prevwin;
		else if (panel->nextwin)
			g_focused_wnd = panel->nextwin;
		else
			g_focused_wnd = panel->prevwin;

	}

    // delete caret if it belongs to this window
//    if ( AniCaretGetWnd()== panel )
//        AniDestroyCaret();

    if (g_hilite_ctrl == panel)
        g_hilite_ctrl= NULL;

    free_w(panel);

    return 0;
}

/**
 * @brief		Add control window to panel window.
 * @param[in]	panel    Panel window handle to add control.
 * @param[in]	type     Control type. Each basic controls have unique type.
 * @param[in]	id       Control ID.
 * @param[in]	style    Control style.
 * @param[in]	location Location of adding control.
 * @param[in]	wndproc  User define control procedure.
 * @retval		handle to the new window		If function succeeds.		
 * @retval		NULL							If function fails.		
 * @remarks		
 * @see			
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
WND		WinAddControl(WND panel, int type, int id, int style, rect * location, WINPROC wndproc)
{
    WND new_ctrl;
    WND prev_ctrl;

    if ( panel == NULL )
        return NULL;

    new_ctrl = malloc_w(sizeof(WINDOW));
    ASSERT( new_ctrl );

    if ( panel->childwin == NULL )
    {
        panel->childwin = new_ctrl;
        new_ctrl->nextwin = NULL;
    }
    else
    {
        prev_ctrl = panel->childwin;
        while ( prev_ctrl->nextwin != NULL )
            prev_ctrl = prev_ctrl->nextwin;

        prev_ctrl->nextwin = new_ctrl;
        new_ctrl->nextwin = NULL;
        new_ctrl->prevwin = prev_ctrl;
    }

    new_ctrl->parentwin = panel;
    new_ctrl->childwin = NULL;

	if (location)
	{

	    location->left += panel->frame.left;
		location->right += panel->frame.left;
		location->top += panel->frame.top;
		location->bottom += panel->frame.top;	// bug fix, bottom->top, by hapum...

		RalMemCpy(&(new_ctrl->frame), location, sizeof(rect));

	    GdiIntersectRect(&(new_ctrl->clip), &(new_ctrl->frame), &(panel->clip));
	}

    new_ctrl->id = id;
    new_ctrl->style = style;

    if (style & AWS_VISIBLE)
        new_ctrl->window_visible = WND_VISIBLE;
    else
        new_ctrl->window_visible = WND_INVISIBLE;

    new_ctrl->fg_color = RGBVALTODEV(COLOR_BLACK);
    new_ctrl->bg_color = RGBVALTODEV(COLOR_WHITE);
    new_ctrl->nextfree = NULL;

    new_ctrl->window_type = type;

	new_ctrl->WndProc = wndproc;
	/*
    switch( type )
    {
    case WND_USERCONTROL:
        new_ctrl->WndProc = wndproc;
        break;
    default :
        new_ctrl->WndProc = NULL;
        break;
    }
	*/
    new_ctrl->dev = NULL;

    // chemistz, 2005-02-11
    //new_ctrl->pData = (void*)id;
    // chemistz, 2005-02-01
    // passes a pointer of new control to it's message handler to initialize
    //MsgSendMessage(new_ctrl, AWM_CREATE, 0, 0);
    TRACE(TR_RexyCore, TR_LEVEL2, ("WinAddControl AWM_WINCREATE new_ctrl:%08X", new_ctrl));
    MsgPostMessage(new_ctrl, AWM_CREATE, id, 0);

    return new_ctrl;
}

/** 
 * @brief	Get boundary information of the specified window
 * @return	true on success, false otherwise
 *          Left, right , top, bottom value will be returned in boundary pointer
 * @ifdef HISTORY
 *			23-Jan-2008 Joshua created
 * @endif
 */
bool WinGetBoundary( WND hwnd, rect *boundary )
{
	boundary->bottom	= hwnd->frame.bottom;
	boundary->left		= hwnd->frame.left;
	boundary->right		= hwnd->frame.right;
	boundary->top		= hwnd->frame.top;
	return TRUE;
}

/**
 * @brief		Get the window that has focus.
 * @return		window handle that has focus
 * @remarks		
 * @see			
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
WND WinGetSelection(void)
{
    return g_focused_wnd;
}

/**
 * @brief		Set the focus to the specified window.
 * @param[in]	panel	Window handle to set focus. Panel window and child window are both valid.
 * @return		void
 * @remarks		All messages will be dispatched to the focused windows.
 * @see			
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 *				22-Jan-2008 Joshua	 windows that don't have thier event handler won't be selected.
 * @endif
 */
void WinSetSelection(WND panel)
{
    WND static lose_focus = NULL;

    if (panel == NULL)
	{
        return;
	}
	else if ( !panel->WndProc )	// added by Joshua on 22-Jan-2008
	{
		return;
	}

    if ( g_focused_wnd != NULL )
    {
        lose_focus = g_focused_wnd;
        g_focused_wnd = panel;

        MsgPostMessage(lose_focus, AWM_KILLFOCUS, (int)panel, 0);
    }
    else
    {
        g_focused_wnd = panel;
        lose_focus = NULL;
    }


    MsgPostMessage(g_focused_wnd, AWM_SETFOCUS, (int)lose_focus, 0);

    if (panel->window_type & CTRL)
        g_hilite_ctrl = g_focused_wnd;
    else
        WinSetHiliteControl(g_focused_wnd, NULL);

    return;
}

/**
 * @brief		Get the next control window handle of specified handle.
 * @param[in]	panel
 * @param[in]	control	Control window handle.
 * @param[in]	bTabStop	
 * @return		the next control window handle of specified handle.
 *				If specified handle is the last handle, the first handle will be returned.
 *		
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
WND WinGetNextControl(WND panel, WND control, int bTabStop)
{
    WND ptr = NULL;

    if (control)
        ptr = control->nextwin;
    else
        ptr = panel->childwin;

    if (bTabStop)
    {
        while (ptr)
        {
            if (ptr->style & AWS_TABSTOP)
                break;

            ptr = ptr->nextwin;
        }

        if (!ptr)
            ptr = WinGetFirstControl(panel, bTabStop);
    }

    return ptr;
}

/**
 * @brief		Get the previous control window handle of specified handle.
 * @param[in]	panel
 * @param[in]	control	Control window handle.
 * @param[in]	bTabStop	
 * @return		the previous control window handle of specified handle.
 *				If specified handle is the first handle, the last handle will be returned.
 * 		
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
WND WinGetPrevControl(WND panel, WND control, int bTabStop)
{
    WND ptr = NULL;

    if (control)
    {
        ptr = control->prevwin;

        if (bTabStop)
        {
            while (ptr)
            {
                if (ptr->style & AWS_TABSTOP)
                    break;

                ptr = ptr->prevwin;
            }

            if (!ptr)
                ptr = WinGetLastControl(panel, bTabStop);
        }
    }
    else
        ptr = WinGetLastControl(control->parentwin, 1);


    return ptr;
}

/**
 * @brief		Get the first control window handle in panel window.
 * @param[in]	panel	Panel window handle that contains the control.
 * @param		bTabStop
 * @return		Returns the first control window handle in the panel window.
 *		
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
WND WinGetFirstControl(WND panel, int bTabStop)
{
    WND ptr = panel->childwin;

    if (bTabStop)
    {
        while (ptr)
        {
            if (ptr->style & AWS_TABSTOP)
                break;

            ptr = ptr->nextwin;
        }
    }

    return ptr;
}

/**
 * @brief		Get the last control window handle in panel window.
 * @param[in]	panel	Panel window handle that contains the control.
 * @param[in]	bTabStop	
 * @return		the last control window handle in panel window.
 * 		
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
WND WinGetLastControl(WND panel, int bTabStop)
{
    WND ptr = panel->childwin;

    while (ptr)
    {
        if (!ptr->nextwin)
            break;

        ptr = ptr->nextwin;
    }

    if (ptr && bTabStop)
    {
        while (ptr)
        {
            if (ptr->style & AWS_TABSTOP)
                break;

            ptr = ptr->prevwin;
        }
    }

    return ptr;
}

/**
 * @brief		Return the focused control in the specified panel window.
 * @param[in]	panel		Panel window handle that contains the control.
 * @return		Returns the focused control in specified panel window. 
 *				If no control has focus, returns NULL.
 *		
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
WND	WinGetSelectionControl(WND panel)
{
    if ( (g_focused_wnd != NULL) && (g_focused_wnd->parentwin == panel) )
        return g_focused_wnd;

    return NULL;
}

/**
 * @brief		Get highlighted control in the specified panel window.
 * @param[in]	panel		Panel window handle that contains the control.
 * @return		Returns the highlighted control in the specified panel window.
 * 		
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
WND WinGetHiliteControl(WND panel)
{
    if (panel && g_hilite_ctrl && g_hilite_ctrl->parentwin == panel)
        return g_hilite_ctrl;

    return NULL;
}

/**
 * @brief		Set the control window to be highlighted.
 * @param[in]	panel	Panel window handle that contains the control.
 * @param[in]	control	Control window handle to be highlighted.
 * @return		void
 * 		
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
void WinSetHiliteControl(WND panel, WND control)
{
    WND static lose_hilite = NULL;

    lose_hilite = g_hilite_ctrl;

    if (lose_hilite)
    {
        MsgSendMessage(lose_hilite, AWM_KILLHILITE, (int)control, 0);
    }

    g_hilite_ctrl = control;

    if (g_hilite_ctrl)
    {
        MsgSendMessage(g_hilite_ctrl, AWM_SETHILITE, (int)lose_hilite, 0);
    }

    return;
}

/**
 * @brief		Destroy the control window.
 * @param[in]	control		Control window handle to destroy.
 * @return		Always returns 0.
 * 			
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
int		WinDestroyControl(WND control)
{
    int ret_val;

    if ( control == NULL )
        return 0;

    if ( !(control->window_type & CTRL) )
        return 0;


    if ( control->parentwin->childwin == control )
        control->parentwin->childwin = control->nextwin;

    // Post Repaint Message to the parent window

    WinInvalidateWindow(control->parentwin, ERASE_BKGND);

    ret_val = WinDestroyWindow(control);

    return ret_val;
}

/**
 * @brief		Destroy all control windows contained in the specified panel window.
 * @param[in]	panel	Panel window handle that contains the controls to destroy.
 * @return		Always returns 0.
 *		
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
int		WinDestroyAllControl(WND panel)
{
    if ( panel == NULL )
        return 0;

    while ( panel->childwin != NULL )
        WinDestroyControl(panel->childwin);

    return 0;
}

/**
 * @brief		Repaint the control window.
 * @param[in]	control		Control window handle to repaint.
 * @param[in]	erase		Set this flag to erase or leave background.\n
 *					- ERASE_BKGND : Erase background.\n
 *					- LEAVE_BKGND : do not erase background.\n
 * @return		void
 * 		
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
void	WinUpdateControl(WND control, int erase)
{
    WinUpdateWindow(control, erase);
    return;
}

/**
 * @brief		Repaint all control windows contained in the specified panel window.
 * @param[in]	panel		Control window handle to repaint.
 * @param[in]	erase		Set this flag to erase or leave background.\n
 *					- ERASE_BKGND : Erase background.\n
 *					- LEAVE_BKGND : do not erase background.\n
 * @return		void
 *		
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
void	WinUpdateAllControl(WND panel, int erase)
{
    WND child, next;

    if ( panel == NULL )
        return;

    child = panel->childwin;
    while ( child != NULL )
    {
        next = child->nextwin;
        WinUpdateWindow(child, erase);
        child = next;
    }
}

/**
 * @brief		Change the Window position and size.
 * @param[in]	panel		Window handle to change the position or size. It can change the position or size of panel and child window.
 * @param[in]	x			New left-topmost X-coordinate.
 * @param[in]	y			New left-topmost Y-coordinate. 
 * @param[in]	width		New width of window.
 * @param[in]	height		New height of window.
 * @param[in]	flag		Set this flag to show or hide specified window. System does not redraw window if NO_REDRAW flag is set.\n
 *		- SHOW_PANEL : Show the window.\n
 *		- HIDE_PANEL : Hide the window.\n
 *		- NO_REDRAW  : Do not redraw window.
 * @return		Always returns 0. 
 *		
 * @if HISTORY
 *				11-Jan-2006 madholic Initiated 
 * @endif
 */
int		WinSetWindowPos(WND panel, int x, int y, int width, int height, int flag)
{
    rect clipping;
    WND control;
//    WND caret_wnd = NULL;
    int x_offset = 0, y_offset = 0;
//    int caret_width = 0, caret_height = 0;
//    int caret_x = 0, caret_y = 0;
//    int caret_exist = 0, caret_show = 0;
//    COLORVAL caret_bg = 0;

    if ( panel->window_type == WND_MEM_PANEL )
        return 0;

//    // save caret
//    caret_exist = 0;
//    if ( (AniCaretGetWnd()!= NULL) && ((panel == AniCaretGetWnd()) || (panel == AniCaretGetWnd()->parentwin)) )
//     {
//         caret_wnd = AniCaretGetWnd();
//         caret_width = AniCaretGetWidth();
//         caret_height = AniCaretGetHeight();
//         caret_x = AniCaretGetX();
//         caret_y = AniCaretGetY();
//         caret_bg = AniCaretGetBg();
// 
//         caret_exist = 1;
// 
//         if ( AniCaretGetBlinkTime() != 0 )
//             caret_show = 1;
//         else
//             caret_show = 0;
// 
//         AniDestroyCaret();
//     }

    if ( panel->window_type & PANEL )
    {
        x_offset = x - panel->frame.left;
        y_offset = y - panel->frame.top;

        panel->frame.left = x;
        panel->frame.top = y;
        panel->frame.right = x + width;
        panel->frame.bottom = y + height;


        GdiGetScreenInfo(&width, &height);
        clipping.left = 0;
        clipping.right = width;
        clipping.top = 0;
        clipping.bottom = height;

        GdiIntersectRect(&(panel->clip), &(panel->frame), &clipping);

        // update control frame information

        control = panel->childwin;
        while ( control != NULL )
        {
            control->frame.left += x_offset;
            control->frame.right += x_offset;
            control->frame.top += y_offset;
            control->frame.bottom += y_offset;

            GdiIntersectRect(&(control->clip), &(control->frame), &(panel->clip));

            control = control->nextwin;
        }
    }
    else
    {
        panel->frame.left = x + panel->parentwin->frame.left;
        panel->frame.right = panel->frame.left + width;
        panel->frame.top = y + panel->parentwin->frame.top;
        panel->frame.bottom = panel->frame.top + height;
        GdiIntersectRect(&(panel->clip), &(panel->frame), &(panel->parentwin->clip));
    }

    if ( flag & HIDE_PANEL )
        panel->window_visible = WND_INVISIBLE;

    if ( flag & SHOW_PANEL )
        panel->window_visible = WND_VISIBLE;


    if ( !(flag & NO_REDRAW) )
    {
        if ( panel->window_type & PANEL )
            WinUpdateAllWindow();
        else
            WinUpdateWindow(panel->parentwin, ERASE_BKGND);
    }

//     if ( caret_exist )
//     {
//         AniCreateCaret(caret_wnd, caret_width, caret_height, caret_bg);
//         AniSetCaretPos(caret_x, caret_y);
//         if ( caret_show )
//             AniShowCaret();
//     }

    return 0;
}

/**
 * @brief		Change the Window z-order up.
 * @param[in]	panel		Window handle to change the z-order.
 * @return		if successed, returns 0. 
 *			
 * @if HISTORY
 *				25-Jul-2006 venabili Initiated 
 * @endif
 */
int		WinSetShowUpControl(WND panel)
{
	WND w, w2;

	//if (panel->parentwin == NULL) return -1;
	w = panel->nextwin;
	if (w == NULL) return 1;

	w2 = w->nextwin;
	if (w2) w2->prevwin = panel;
	w2 = panel->prevwin;
	if (w2) w2->nextwin = w;
    
	if ( panel->parentwin->childwin == panel )
	    panel->parentwin->childwin = w;
	w->prevwin = panel->prevwin;
	panel->nextwin = w->nextwin;
	w->nextwin = panel;
	panel->prevwin = w;				

	WinUpdateWindow(panel->parentwin, SHOW_PANEL);
	return 0;
}

/**
 * @brief		Change the Window z-order down.
 * @param[in]	panel		Window handle to change the z-order.
 * @return		if successed, returns 0. 
 * 			
 * @if HISTORY
 *				25-Jul-2006 venabili Initiated 
 * @endif
 */
int		WinSetShowDownControl(WND panel)
{
	WND w, w2;

	//if (panel->parentwin == NULL) return -1;
	w = panel->prevwin;
	if (w == NULL) return 1;

	w2 = w->prevwin;
	if (w2) w2->nextwin = panel;
	w2 = panel->nextwin;
	if (w2) w2->prevwin = w;

    if ( panel->parentwin->childwin == w )
	    panel->parentwin->childwin = panel;
	w->nextwin = panel->nextwin;
	panel->prevwin = w->prevwin;
	w->prevwin = panel;
	panel->nextwin = w;				

	WinUpdateWindow(panel->parentwin, SHOW_PANEL);
	return 0;
}

/**
 * @brief		Change the Window z-order top.
 * @param[in]	panel		Window handle to change the z-order.
 * @return		if successed, returns 0. 
 *			
 * @if HISTORY
 *				25-Jul-2006 venabili Initiated 
 * @endif
 */
int		WinSetShowTopControl(WND panel)
{
	while(WinSetShowUpControl(panel) == 1)
		;
	return 0;
}

/**
 * @brief		Change the Window z-order bottom.
 * @param[in]	panel		Window handle to change the z-order.
 * @return		if successed, returns 0. 
 * 		
 * @if HISTORY
 *				25-Jul-2006 venabili Initiated 
 * @endif
 */
int		WinSetShowBottomControl(WND panel)
{
	while(WinSetShowDownControl(panel) == 1)
		;

	return 0;
}

/**
 * @brief		Change the Window message handler.
 * @param[in]	panel		Window handle to change the message handler.
 * @param[in]	WndProc		New message handler
 * @return		if successed, returns 0. 
 * 		
 * @if HISTORY
 *				27-Jul-2006 venabili Initiated 
 * @endif
 */
int		WinSetCurrentProc(WND panel, WINPROC WndProc)
{
	panel->WndProc = WndProc;
	return 0;
}

/**
 * @brief		Get the Window message handler.
 * @param[in]	panel		Window handle to change the message handler.
 * @retval		Window message handler
 *			
 * @if HISTORY
 *				27-Jul-2006 venabili Initiated 
 * @endif
 */
WINPROC	WinGetCurrentProc(WND panel)
{
	return panel->WndProc;
}
// do not use the following functions in user applications.
static WND g_CurrentPanel = NULL;

/**	\brief	The WinSetCurrentWindow function

	\param	panel	window handle

	\return	void
*/
void WinSetCurrentWindow(WND panel)
{
	g_CurrentPanel = panel;
}

/** @} */ // End of Window Managing Service
