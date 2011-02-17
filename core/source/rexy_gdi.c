/*
 * $Id: rexy_gdi.c 435 2005-02-16 08:25:12Z longdal $
 * Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
 *
 * This file is a part of Rexy Embedded 
 *
 *	graphic device interface
 *
 */

/** @file */

/**
 * @defgroup gdi Core Graphic Device Interface Service
 * These APIs are associated with Graphic Device Interface
 * @ingroup RexyECoreGroup
 * @{
 */

#include "rexy.h"

/*======================================================================*/
/* Private								*/
/*======================================================================*/
static void PrvFlushPart(WND wnd, rect *boundary);
static void PrvFlushAll(WND wnd);
static void PrvDrawHLine(WND wnd, int x1, int y1, int x2);
static void PrvDrawVLine(WND wnd, int x1, int y1, int y2);
static void PrvDrawLineBresenhamNC(WND wnd, int x0, int y0, int x1, int y1);
static void PrvMergeRow( UNITCL * src_row_p, int width, int mode, int weight );
static void PrvMergeCol( UNITCL * src_p, int mode, int weight );

/*======================================================================*/
/* Module Variables							*/
/*======================================================================*/
#define E_ROK	0	// for ROMFS
#define MIN_X wnd->clip.left
#define MAX_X wnd->clip.right
#define MIN_Y wnd->clip.top
#define MAX_Y wnd->clip.bottom

rect g_update_rect = { 0, 0, 0, 0 };
rect *g_update_area = NULL;

#define RX_ALPHA_BLEND_R(src, dest)		\
	( ( ((((src) & UNITCL_R_MASK) >> 1)+(((dest) & UNITCL_R_MASK) >> 1))) & UNITCL_R_MASK )
#define RX_ALPHA_BLEND_G(src, dest)		\
	( ( (((src) & UNITCL_G_MASK)+((dest) & UNITCL_G_MASK)) >> 1 ) & UNITCL_G_MASK )
#define RX_ALPHA_BLEND_B(src, dest)		\
	( ( (((src) & UNITCL_B_MASK)+((dest) & UNITCL_B_MASK)) >> 1 ) )

#define SRC_COLOR		RGBCOMPONENTTODEV(0x00, 0x00, 0x00)		// black
#define ICON_BULK_ID	4
#define IMAGE_BULK_ID	5

static int g_ScreenLockCount=0;

static int g_TextAlign = ALIGN_LEFT;
static int g_TextStyle = NORMALFONT;
static int g_TextSpace = 0;
static int g_TextExtent = 0;

//static short lcd_semaphore_id = 0;
/*======================================================================*/
/* Implementation							*/
/*======================================================================*/

/**
 * @anchor		GdiGetScreenInfo
 * @brief		Get the width and height of current device in pixel.
 * @param[out]	width  width of current device in pixel
 * @param[out]	height height of current device in pixel
 * @retval		RxErrNone on success
 * @retval		RxErrGdiClass|enInvalidParam if both parameters are invalid.
 * @remarks		You can get the screen width and height of current device 
 *				using this function. If you are interested in only one value, you can
 *				send NULL parameter for that item, but at least one parameter should be valid one.
 * 		
 * @if HISTORY
 *				02-Jan-2006	Joshua	Initiated	
 *				04-Jan-2005	Joshua	one NULL parameter will be allowed
 * @endif
 */
RxErr GdiGetScreenInfo(rint32 * width, rint32 * height)
{
    if ( !width && !height ) return MkGdiErr(enInvalidParam);

	if ( width )
		*width = RalScreenGetWidth();
	if ( height )
		*height = RalScreenGetHeight();

    return RxErrNone;
}
/** 
 * @anchor	GdiGetScreenPtr
 * @brief	Return the pointer to screen buffer
 * @return	the pointer to screen buffer
 * @if HISTORY
 *			11-May-2006	Joshua	created
 * @endif
 */
ruint16 *GdiGetScreenPtr(void)
{
	return RalScreenGetBuffer();
}
/** 
 * @anchor	GdiGetScreenLockCount
 * @brief	Get Screen lock count
 * @return	screen lock count
 * @see		GdiLockScreen
 * @see		GdiUnlockScreen
 * @if HISTORY
 *			9-Jan-2006	Joshua	created
 * @endif
 */
ruint16	GdiGetScreenLockCount(void)
{
	return g_ScreenLockCount;
}
/**	
 * @anchor		GdiLockScreen
 * @brief		Lock current screen drawing operation
 * @param[in]	panel	window handle
 * 
 * @retval		RxErrNone on success
 * @retval		RxErrGdiClass|enOverLockCount	if screen lock count over MAX_SCREEN_LOCK_COUNT
 * @remarks		If you lock screen using this API, whole drawing operation won't be seen
 *				and any change will be reflected to only video buffer. If you reveal your operation,
 *				call GdiUnlockScreen. So, always you have to call GdiLockScreen and GdiUnlockScreen 
 *				as a pair, and the max count of lock can not be over MAX_SCREEN_LOCK_COUNT times.
 * @see			GdiUnlockScreen
 * @if HISTORY
 *				02-Jan-2006	Joshua Initiated
 * @endif
 */
RxErr GdiLockScreen(WND panel)
{
    if ( g_ScreenLockCount >= MAX_SCREEN_LOCK_COUNT )
    {
        return MkGdiErr(enOverLockCount);
    }

    g_ScreenLockCount++;
    
	return RxErrNone;
}

/**
* @brief		GdiUnlockScreen
* @param		panel	window handle
* @retval	error code
* 
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
RxErr GdiUnlockScreen(WND panel)
{
    if ( g_ScreenLockCount <= 0 )
    {
        return MkGdiErr(enUnderLockCount);
    }

    g_ScreenLockCount--;

    if ( g_ScreenLockCount == 0 )
    {
        PrvFlushAll(panel);
    }
    else
    {
        //DOLATER
        //return proper value indicating lock count information
    }
    return RxErrNone;
}

/** 
 * @anchor		GdiFlushPart
 * @brief		Flush Screen buffer's partial content to video memory
 * @param[in]	wnd	Window Handle
 * @param[in]	boundary The area to be update in video memory
 * @return		void
 * @remark		In general, user doesn't have to call this API, since
 *				most of drawing operation will call this API internally if necessary
 * @see			GdiFlushAll
 * @if HISTORY
 *				17-Jan-2006 Joshua exported
 * @endif
 */
void GdiFlushPart(WND wnd, rect *boundary)
{
	g_ScreenLockCount=0;
	PrvFlushPart( wnd, boundary );
}

/** 
 * @anchor		GdiFlushAll
 * @brief		Flush Screen buffer's whole content to video memory
 * @param[in]	wnd	Window Handle
 * @return		void
 * @remark		In general, user doesn't have to call this API, since
 *				most of drawing operation will call this API internally if necessary
 * @see			GdiFlushPart
 *
 * @if HISTORY
 *				17-Jan-2006 Joshua exported
 * @endif
 */
void GdiFlushAll(WND wnd)
{
	g_ScreenLockCount=0;
	PrvFlushAll(wnd);
}


/**
* @brief		Invalidate the passed rectangle area.
* @param[out]	invalid		The rectangle area that will be invalidated
* @return		void
* 
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
void GdiInvalidateRect(rect * invalid)
{
    if ( invalid == NULL )
        return;

    if ( g_update_area == NULL )
    {
        g_update_area = &g_update_rect;
        *g_update_area = *invalid;
    }
    else
    {
        g_update_area->left = ( (g_update_area->left < invalid->left) ?  g_update_area->left : invalid->left );
        g_update_area->right = ( (g_update_area->right > invalid->right) ? g_update_area->right : invalid->right );
        g_update_area->top = ( (g_update_area->top < invalid->top) ? g_update_area->top : invalid->top );
        g_update_area->bottom = ( (g_update_area->bottom > invalid->bottom) ? g_update_area->bottom : invalid->bottom );
    }
}

/**
* @brief		전체 영역 유효화
* @return		void
* @remarks		This function sets all updated rectangle areas to NULL. That is, this function needs to be
*				called after sending frame buffer to LCD, in order to 'validate' updated rectangle areas to NULL.
* 		
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
void GdiValidateAll(void)
{
    if ( g_update_area != NULL )
    {
        g_update_area->left = g_update_area->right = g_update_area->top = g_update_area->bottom = 0;
        g_update_area = NULL;
    }
}

/**
* @brief		Set the specified color as background color of target window.
* @param[in]	wnd		Target window to set background color.
* @param[in]	color	Color to be set.
* @return		Returns the previous background color of target window.
* 			
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
COLORVAL GdiSetBkColor(WND wnd, COLORVAL color)
{

    COLORVAL old_bg;

    old_bg = DEVTORGB(wnd->bg_color);
    wnd->bg_color = RGBVALTODEV(color);

    return old_bg;
}


/**
* @brief		Set the specified color as foreground color of target window.
* @param[in]	wnd		Target window to set foreground color.
* @param[in]	color	Color to be set.
* @return		Returns the previous foreground color of target window.
* @remarks		
*		NOTE : You must restore previous color after this function. Refer to the following example\n
*
*		[EXAMPLE]\n
*		COLORVAL oldColor;	\n
*		\n
*		oldColor = GdiSetFgColor(wnd, COLOR_BLACK);	// save previous font to oldFont variable\n
*		\n
*		// process work...\n
*		\n
*		GdiSetFgColor(wnd, oldColor);	// restore previous font\n
* 		
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
COLORVAL GdiSetFgColor(WND wnd, COLORVAL color)
{
    COLORVAL old_fg;

    old_fg = DEVTORGB(wnd->fg_color);
    wnd->fg_color = RGBVALTODEV(color);

    return old_fg;
}

/**
* @brief		Get the background color of specified window.
* @param[in]	wnd		Window to get background color.
* @return		Returns the background color of target window.
* 		
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
COLORVAL GdiGetBkColor(WND wnd)
{
    return DEVTORGB(wnd->bg_color);
}

/**
* @brief		Get the foreground color of specified window.
* @param[in]	wnd		Window to get foreground color.
* @return		Returns the foreground color of target window.
* 			
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
COLORVAL GdiGetFgColor(WND wnd)
{
    return DEVTORGB(wnd->fg_color);
}

/**
* @brief		Draws a pixel in window.
* @param[in]	wnd		Window to draw a pixel.
* @param[in]	x		x coordinate of pixel to draw.
* @param[in]	y		y coordinate of pixel to draw.
* @param[in]	color	color of the pixel.
* @return		previous pixel color of target coordinate.
*		
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
COLORVAL GdiSetPixel(WND wnd, rint32 x, rint32 y, COLORVAL color)
{
    UNITCL * screenPtr;
    COLORVAL old_color;
    int linelen;
    Point pt;

    if ( (wnd->window_type == WND_PANEL) && (wnd->window_visible == WND_INVISIBLE) )
        return COLOR_INVALID;

    // moving to the screen coordinates
    x += wnd->frame.left;
    y += wnd->frame.top;

    // clipping
    pt.x = x;
    pt.y = y;
    if ( !GdiPtInRect(&wnd->clip, &pt) )
        return  COLOR_INVALID;

    if ( wnd->dev != NULL )
    {
        screenPtr = wnd->dev->mem_addr;
        linelen = wnd->dev->width;
    }
    else
    {
        rect invalid;
        invalid.left = x;
        invalid.right = x + 1;
        invalid.top = y;
        invalid.bottom = y + 1;
        GdiInvalidateRect(&invalid);

        screenPtr = RalScreenGetBuffer();
        linelen = RalScreenGetWidth();
    }

    screenPtr += linelen * y + x;
    old_color = DEVTORGB(*screenPtr);
    *screenPtr = RGBVALTODEV(color);

    //DOLATER
    //It's desirable to flush one pixel than flush whole screen
    //however we don't have enough time
    PrvFlushAll(wnd);

    return old_color;
}

/**
* @brief		Gets the pixel color of target coordnate.
* @param[in]	wnd		Window to get pixel color.
* @param[in]	x		x coordinate of target pixel.
* @param[in]	y		y coordinate of target pixel.
* @return		Pixel color of target coordinate.
* 			
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
COLORVAL GdiGetPixel(WND wnd, rint32 x, rint32 y)
{
    UNITCL * screenPtr;
    int linelen;
    Point pt;

    if ( (wnd->window_type == WND_PANEL) && (wnd->window_visible == WND_INVISIBLE) )
        return COLOR_INVALID;

    // moving to the screen coordinates
    x += wnd->frame.left;
    y += wnd->frame.top;

    // clipping
    pt.x = x;
    pt.y = y;
    if ( !GdiPtInRect(&wnd->clip, &pt) )
        return  COLOR_INVALID;

    if ( wnd->dev != NULL )
    {
        screenPtr = wnd->dev->mem_addr;
        linelen = wnd->dev->width;
    }
    else
    {
        screenPtr = RalScreenGetBuffer();
        linelen = RalScreenGetWidth();
    }

    screenPtr += linelen * y + x;
    return DEVTORGB(*screenPtr);
}

/**
* @brief		Draws a line using specified width
* @param[in]	wnd		Window to draw line.
* @param[in]	x1		X coordinate of line start point.
* @param[in]	y1		Y coordinate of line start point.
* @param[in]	x2		X coordinate of line endpoint.
* @param[in]	y2		Y coordinate of line endpoint.
* @param[in]	width	Width of line.
* @return		void
* @remarks		Draws a line from (x1, y1) to (x2, y2) using specified width and current foreground color. 
				Does not draw the endpoint pixel.
* 			
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
void GdiDrawLineWidth(WND wnd, rint32 x1, rint32 y1, rint32 x2, rint32 y2, int width)
{
    float	delta, beta;
    int		src_x, src_y, dest_x, dest_y;
    int		i;
    rect	rtUpdate;

    if ( (wnd->window_type == WND_PANEL) && (wnd->window_visible == WND_INVISIBLE) )
        return;

    // moving to the screen coordinates
    x1 += wnd->frame.left;
    x2 += wnd->frame.left;
    y1 += wnd->frame.top;
    y2 += wnd->frame.top;

    if ( x1 == x2 )
    {
        rtUpdate.top= ( y1 <= y2 ? y1 : y2 );
        rtUpdate.bottom = ( y1 >= y2 ? y1 : y2 );
        rtUpdate.left=x1;
        rtUpdate.right=x1+width-1;

        for(i=0;i<width;i++)
            PrvDrawVLine(wnd, x1 + i, y1, y2);

        PrvFlushPart( wnd, &rtUpdate );
        return;
    }

    if ( y1 == y2 )
    {
        rtUpdate.top= y1;
        rtUpdate.bottom =y1 + width -1;
        rtUpdate.left= ( x1<= x2 ? x1 : x2 );
        rtUpdate.right=( x1>= x2 ? x1 : x2 );

        for(i=0;i<width;i++)
            PrvDrawHLine(wnd, x1, y1 + i, x2);

        PrvFlushPart( wnd, &rtUpdate );
        return;
    }

    src_x = x1;
    src_y = y1;
    dest_x = x2;
    dest_y = y2;

    delta = (float)(src_y - dest_y) / (float)(src_x - dest_x) ;
    beta = src_y - delta * src_x;

    // x position clipping
    if ( src_x < dest_x )
    {
        if ( (dest_x < MIN_X) || (MAX_X < src_x) )
            return;

        rtUpdate.left = src_x;
        rtUpdate.right = dest_x;

        // clipping X
        if ( src_x < MIN_X )
        {
            src_x = MIN_X;
            src_y = (int)(MIN_X * delta + beta + 0.5); // 0.5 for rounding. y = delta * x + beta
        }

        if ( dest_x > MAX_X )
        {
            dest_x = MAX_X;
            dest_y = (int)(MAX_X * delta + beta + 0.5);
        }
    }
    else	// src_x > dest_x
    {
        if ( (src_x < MIN_X) || ( MAX_X < dest_x) )
            return;

        rtUpdate.left = dest_x;
        rtUpdate.right = src_x;

        if ( dest_x < MIN_X )
        {
            dest_x = MIN_X;
            dest_y = (int)(MIN_X * delta + beta + 0.5);
        }

        if ( MAX_X < src_x )
        {
            src_x = MAX_X;
            src_y = (int)(MAX_X * delta + beta + 0.5);
        }
    }

    // y position clipping
    if ( src_y < dest_y )
    {
        if ( (dest_y < MIN_Y) || (MAX_Y < src_y) )
            return;

        rtUpdate.top = src_y;
        rtUpdate.bottom = dest_y;

        // clipping Y
        if ( src_y < MIN_Y )
        {
            src_y = MIN_Y;
            src_x = (int)((MIN_Y - beta) / delta);
        }

        if ( dest_y > MAX_Y )
        {
            dest_y = MAX_Y;
            dest_x = (int)((MAX_Y - beta) / delta);
        }
    }
    else	// src_y > dest_y
    {
        if ( (src_y < MIN_Y) || (MAX_Y < dest_y) )
            return;

        rtUpdate.top = dest_y;
        rtUpdate.bottom = src_y;

        if ( MAX_Y < src_y )
        {
            src_y = MAX_Y;
            src_x = (int)((MAX_Y - beta) / delta);
        }

        if ( dest_y < MIN_Y )
        {
            dest_y = MIN_Y;
            dest_x = (int)((MIN_Y - beta) / delta);
        }
    }

    for(i=0;i<width;i++)
        PrvDrawLineBresenhamNC(wnd, src_x, src_y+i, dest_x, dest_y+i);

    rtUpdate.bottom += width;

    PrvFlushPart(wnd,&rtUpdate);
    return;
}

/**
* @brief		Draws a line.
* @param[in]	wnd		Window to draw line.
* @param[in]	x1		X coordinate of line start point.
* @param[in]	y1		Y coordinate of line start point.
* @param[in]	x2		X coordinate of line endpoint.
* @param[in]	y2		Y coordinate of line endpoint.
* @return		void
* @remarks		Draws a line from (x1, y1) to (x2, y2) using current foreground color. Does not draw the endpoint pixel.
* 			
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
void GdiDrawLine(WND wnd, rint32 x1, rint32 y1, rint32 x2, rint32 y2)
{
    GdiDrawLineWidth(wnd, x1, y1, x2, y2, 1);
}

/**
* @brief		Draws a line using the specified color.
* @param[in]	wnd		Window to draw line.
* @param[in]	x1		X coordinate of line start point.
* @param[in]	y1		Y coordinate of line start point.
* @param[in]	x2		X coordinate of line endpoint.
* @param[in]	y2		Y coordinate of line endpoint.
* @param[in]	color	Color value.
* @return		void
* @remarks		Draws a line from (x1, y1) to (x2, y2) using current foreground color. Does not draw the endpoint pixel.
* 		
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
void GdiDrawColorLine(WND wnd, rint32 x1, rint32 y1, rint32 x2, rint32 y2, COLORVAL color)
{
    COLORVAL old_color;
    old_color = GdiSetFgColor(wnd, color);
    GdiDrawLine(wnd, x1, y1, x2, y2);
    GdiSetFgColor(wnd, old_color);
    return;
}

/**
* @brief		Draw inverted rectangle.
* @param[in]	wnd Window to draw rectangle.
* @param[in]	src Structure of rectangle to draw. Coordinates are followed by target window.
* @return		void
* @remarks		Draws a rectangle in traget window using invert destination buffer. 
*				Does not draw rightmost and bottommost border of rectangle.
* 			
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
void GdiInvertRect(WND wnd, rect *src)
{
    UNITCL * buf_addr;
    int i, j, x, y, x2, y2, len;
    int width, height;

    //	if ( (wnd->window_type == WND_PANEL) && (wnd->window_visible == WND_INVISIBLE) )
    //		return;

    x = src->left + wnd->frame.left;
    y = src->top + wnd->frame.top;
    x2 = src->right + wnd->frame.left;
    y2 = src->bottom + wnd->frame.top;

    if (x > MAX_X)
    {
        return;
    }
    else
    {
        if (x2 < MIN_X)
        {
            return;
        }
        else
        {
            if (x < MIN_X)
                x = MIN_X;
            if (x2 > MAX_X)
                x2 = MAX_X;
        }
    }

    if (y > MAX_Y)
    {
        return;
    }
    else
    {
        if (y2 < MIN_Y)
        {
            return;
        }
        else
        {
            if (y < MIN_Y)
                y = MIN_Y;
            if (y2 > MAX_Y)
                y2 = MAX_Y;
        }
    }

    width = x2 - x + 1;
    height = y2 - y + 1;
    if ((width <= 0) || (height <= 0))
        return;

    if ( wnd->dev != NULL )
    {
        buf_addr = wnd->dev->mem_addr;
        len = wnd->dev->width;
    }
    else
    {
        rect invalid;
        invalid.left = x;
        invalid.right = x2;
        invalid.top = y;
        invalid.bottom = y2;
        GdiInvalidateRect(&invalid);

        buf_addr = RalScreenGetBuffer();
        len = RalScreenGetWidth();
    }

    buf_addr += len * y + x;

    for ( i = 0; i < height; ++i )
    {
        for ( j = 0; j < width; ++j )
        {
            *buf_addr = (unsigned short)(~*buf_addr);
            ++buf_addr;
        }
        buf_addr += (len - width);
    }

    PrvFlushPart(wnd,src);
    return;

}

/**
* @brief		Draws the rectangle that filled with the specified color.
* @param[in]	wnd			Window to draw rectangle.
* @param[in]	src			Structure of rectangle to draw. Coordinates are followed by target window.
* @param[in]	fill_color	Color to fill rectangle.
* @return		void
* @remarks		Draws a rectangle in traget window using selected fill_color. 
* 			
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
void GdiFillRect(WND wnd, rect * src, COLORVAL fill_color)
{
    UNITCL * buf_addr;
    UNITCL color;
    int i, j, x, y, x2, y2, len;
    int width, height;

    //	if ( (wnd->window_type == WND_PANEL) && (wnd->window_visible == WND_INVISIBLE) )
    //		return;

    x = src->left + wnd->frame.left;
    y = src->top + wnd->frame.top;
    x2 = src->right + wnd->frame.left;
    y2 = src->bottom + wnd->frame.top;

    if (x > MAX_X)
    {
        return;
    }
    else
    {
        if (x2 < MIN_X)
        {
            return;
        }
        else
        {
            if (x < MIN_X)
                x = MIN_X;
            if (x2 > MAX_X)
                x2 = MAX_X;
        }
    }

    if (y > MAX_Y)
    {
        return;
    }
    else
    {
        if (y2 < MIN_Y)
        {
            return;
        }
        else
        {
            if (y < MIN_Y)
                y = MIN_Y;
            if (y2 > MAX_Y)
                y2 = MAX_Y;
        }
    }
    // klenui comment: endpoint
    width = x2 - x + 1;
    height = y2 - y + 1;

    if ((width <= 0) || (height <= 0))
        return;

    if ( wnd->dev != NULL )
    {
        buf_addr = wnd->dev->mem_addr;
        len = wnd->dev->width;
    }
    else
    {
        rect invalid;
        invalid.left = x;
        invalid.right = x2;
        invalid.top = y;
        invalid.bottom = y2;
        GdiInvalidateRect(&invalid);

        buf_addr = RalScreenGetBuffer();
        len = RalScreenGetWidth();
    }

    color = RGBVALTODEV(fill_color);
    buf_addr += len * y + x;

    for ( i = 0; i < height; ++i )
    {
        for ( j = 0; j < width; ++j )
        {
            *buf_addr = color;
            ++buf_addr;
        }
        buf_addr += (len - width);
    }
    PrvFlushPart(wnd,src);
}

/**
* @brief		Draws a rectangle using current foreground color.
* @param[in]	wnd		Window to draw a rectangle.
* @param[in]	src		Structure of rectangle to draw.
* @return		void
* @remarks		Border of rectangle is drawn by current foreground color. Rectangle is filled with background color.
* 			
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
void GdiRectangle(WND wnd, rect * src)
{
    rect inner;
    int x1, y1, x2, y2;

    if ( (wnd->window_type == WND_PANEL) && (wnd->window_visible == WND_INVISIBLE) )
        return;

    inner.left = src->left + 1;
    inner.right = src->right - 1;
    inner.top = src->top + 1;
    inner.bottom = src->bottom - 1;

    GdiFillRect(wnd, &inner, DEVTORGB(wnd->bg_color));

    x1 = src->left + wnd->frame.left;
    x2 = src->right + wnd->frame.left;
    y1 = src->top + wnd->frame.top;
    y2 = src->bottom + wnd->frame.top;

    // Draw Boundary
    PrvDrawHLine(wnd, x1, y1, x2);
    PrvDrawHLine(wnd, x1, y2, x2);

    PrvDrawVLine(wnd, x1, y1, y2);
    PrvDrawVLine(wnd, x2, y1, y2);

    PrvFlushPart(wnd,src);
}

/**
* @brief		Paint a string.
* @param[in]	wnd		Window to paint a string.
* @param[in]	x		x coordinate of the string.
* @param[in]	y		y coordinate of the string.
* @param[in]	str		Points to the character string to be drawn.
* @param[in]	len		Specifies the number of bytes in the string.
* @param[in]	space	space value between characters.
* @return		void
* 		
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
void GdiPaintString(WND wnd, rint32 x, rint32 y, char *str, int len, int space)
{
    rect text_rect, src_clip_rect, dest_clip_rect;
    int height, width;
    UNITCL *dp, *sp, *ep, *buf_addr;
    int linelen_dest, linelen_src;
    int y2, dw;

    x += wnd->frame.left;
    y += wnd->frame.top;

    width = FntGetTextWidthEx(&FntGlobalContext, str, len, space);
    height = FntGetTextHeight(&FntGlobalContext);

    text_rect.left = x;
    text_rect.top = y;
    text_rect.right = x + width -1;
    text_rect.bottom = y + height-1;

    if (GdiIntersectRect(&dest_clip_rect, &text_rect, &wnd->clip) < 0)
        return; // no intersect region...

    src_clip_rect.left = dest_clip_rect.left - x;
    src_clip_rect.top = dest_clip_rect.top - y;
    src_clip_rect.right = dest_clip_rect.right - x;
    // klenui. boundary
    src_clip_rect.bottom = dest_clip_rect.bottom - y;

    // making text image

    // adjust dest. cordination
    x = dest_clip_rect.left;
    width = src_clip_rect.right - src_clip_rect.left +1;
    y = dest_clip_rect.top;
    height = src_clip_rect.bottom - src_clip_rect.top +1;

    sp = FntGetStrImage(&FntGlobalContext, src_clip_rect.left, src_clip_rect.top, width, height, str, len, space);
    linelen_src = src_clip_rect.right - src_clip_rect.left;

    // copy image..
    if ( wnd->dev != NULL )
    {
        buf_addr = wnd->dev->mem_addr;
        linelen_dest = wnd->dev->width;
    }
    else
    {
        rect invalid;
        invalid.left = x;
		if ( width ) invalid.right = x + width - 1;
		else invalid.right = x;

        invalid.top = y;
		if ( height ) invalid.bottom = y + height - 1;
		else invalid.bottom = y;

        GdiInvalidateRect(&invalid);

        buf_addr = RalScreenGetBuffer();
        linelen_dest = RalScreenGetWidth();
    }

    y2 = y + height;
    dw = linelen_dest - width;
    dp = buf_addr + linelen_dest * y + x;

    while (y++ < y2) {
        ep = sp + width;
        while (sp < ep) {
            if (*sp)
                *dp = wnd->fg_color;
            sp++;
            dp++;
        }
        dp += dw; // next line
    }

    PrvFlushPart(wnd,&dest_clip_rect);
}

/** \brief	문자열을 해당 윈도우의 지정된 위치에 출력

	\param[in]	hWnd	문자열을 출력할 윈도우의 핸들
	\param[in]	x	X 좌표
	\param[in]	y	Y 좌표
	\param[in]	width	출력 영역의 길이
	\param[in]	str	문자열
	\param[in]	str_len	문자열의 길이
	\param[in]	hAlign	정렬 방식\n
					ALIGN_CENTER : 가운데 정렬\n
					ALIGN_RIGHT : 오른쪽 정렬\n
					ALIGN_LEFT : 왼쪽 정렬

	\return	출력된 문자열의 길이 또는 error code
*/

int	GdiDrawTextOut(WND hWnd, rint32 x, rint32 y, int width, char *str, int str_len, int hAlign)
{
    int nLen, nTextWidth;

    //TrcPrint("start GdiDrawPictTextOut");
    if (!str)
    {
        TrcPrint("str is NULL");
        return -1;
    }

    if (str_len < 1)
    {
        TrcPrint("str_len is less than 0");
        return -1;
    }

    nLen = str_len;
    nTextWidth = FntGetTextWidth(&FntGlobalContext, str, nLen);
    if (nTextWidth > width)
    {
        nLen = StrGetBreakPos(str, nLen, width);
        nTextWidth = FntGetTextWidth(&FntGlobalContext, str, nLen);
    }

    switch (hAlign)
    {
    case ALIGN_CENTER:
        x = x + (width - nTextWidth) / 2;
        break;
    case ALIGN_RIGHT:
        x = x + (width - nTextWidth);
        break;
    case ALIGN_LEFT:
    default:
        break;
    }

    GdiTextOut(hWnd, str, nLen, x, y);
    // klenui: move to rexygdi.c +1381
    // I moved the following line to make GdiExtTextOut to support static e-moji character.(curitel bug#1448)
    //_DrawGraphicChar_Pict( hWnd, str, nLen, x, y );	// the difference with GdiDrawTextOut is just this line.

    //TrcPrint("end GdiDrawPictTextOut");
    return nLen;
}

/**
 * @ingroup gdi
 * @brief Added word-break in GdiDrawTextOut() function.
 *
 * @param hWnd		Handle of the Window.
 * @param x			X-coordinate.
 * @param y			Y-coordinate.
 * @param width		Width of screen to be drawn.
 * @param str		Pointer to the string to be drawn.
 * @param str_len	Length of the string.
 * @param hAlign	Text-alignment setting.
 *
 * @return			On succees function returns the length of characters. Otherwise returns -1.
*/
int GdiDrawTextOutLF(WND hWnd,rint32 x, rint32 y, int width, char *str, int str_len, int hAlign)
{
	int nLen, nTextWidth;

	if (!str && !*str)
		return -1;

	if (str_len < 1)
		return -1;
    
	nLen = str_len;
	nTextWidth = FntGetTextWidth(&FntGlobalContext, str, nLen);
	if(nTextWidth > width)
	{
		nLen = StrGetBreakPos(str, nLen, width);
		nTextWidth = FntGetTextWidth(&FntGlobalContext, str, nLen);
		
		GdiTextOut(hWnd, str, nLen, x, y);
		GdiDrawTextOutLF(hWnd, x, y+FntGetTextHeight(&FntGlobalContext), width, str+nLen, str_len-nLen, hAlign);
	}
	else
	{
		GdiDrawTextOut(hWnd, x, y, width, str, str_len, hAlign);
	}

	return str_len;
}



/**
* @brief		Draws a character string.
* @param[in]	wnd		Window to draw characters.
* @param[in]	string	Points to the character string to be drawn.
* @param[in]	length	Specifies the number of bytes in the string.
* @param[in]	x		x-coordinate of the starting point of the text.
* @param[in]	y		y-coordinate of the starting point of the text.
* @return		Always return 0.
* @remarks		This function draws at the specified location using current foreground color.
* 			
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
int GdiTextOut(WND wnd, char * string, int length, rint32 x, rint32 y)
{
    int width, height;
    rect bkgnd;
	int space = GdiGetTextSpace();
	int style = GdiGetTextStyle();
	int align = GdiGetTextAlign();
	int extent = GdiGetTextExtent();

    if ( (wnd->window_type == WND_PANEL) && (wnd->window_visible == WND_INVISIBLE) )
        return 0;

    //GdiGetTextWidth(string, length, &width, &height);
    width = FntGetTextWidthEx(&FntGlobalContext, string, length, space);
    height = FntGetTextHeight(&FntGlobalContext);

    bkgnd.left = x;
    bkgnd.top = y;
    bkgnd.right = x + width;
    bkgnd.bottom =y + height;

    if ( width > 0 ) bkgnd.right--;
    if ( height > 0 ) bkgnd.bottom--;
//    if ( boundary )
//    {
//        *boundary = bkgnd;
//    }
 
	if(extent>0)
	{
		switch (align)
		{
		case ALIGN_CENTER:
			x = x + (extent - width) / 2;
			break;
		case ALIGN_RIGHT:
			x = x + (extent - width);
			break;
		case ALIGN_LEFT:
		default:
			break;
		}
	}

    if (wnd->bg_color != COLOR_TRANSPARENT)
    {
        //fill background
        GdiFillRect(wnd, &bkgnd, DEVTORGB(wnd->bg_color));
    }

    GdiPaintString(wnd, x, y, string, length, space);

    if ( style & BOLDFONT ) {
        GdiPaintString(wnd, (x+1), y, string, length, space);
    }

    if ( style & UNDERLINEFONT ) {
        GdiDrawLine(wnd, (x), (y+height-1), (x+width), (y+height-1));
    }

    if ( style & OUTLINEFONT)
    {
        COLORVAL oldColor;
        oldColor = GdiSetFgColor( wnd, COLOR_WHITE );
        GdiPaintString(wnd, (x-1), y, string, length, space);
        GdiPaintString(wnd, (x+1), y, string, length, space);
        GdiPaintString(wnd, x, (y-1), string, length, space);
        GdiPaintString(wnd, x, (y+1), string, length, space);

        // draw original text
        GdiSetFgColor( wnd, COLOR_BLACK );
        GdiPaintString(wnd, x, y, string, length, space);
        GdiSetFgColor( wnd, oldColor );
    }

    PrvFlushPart(wnd,&bkgnd);
    return 0;
}

/**
* @brief		Draw the specified characters to window.
* @param[in]	wnd			Window to draw characters.
* @param[in]	string		Points to the character string to be drawn.
* @param[in]	length		Specifies the number of bytes in the string.
* @param[in]	x			x-coordinate of the starting point of the text.
* @param[in]	y			y-coordinate of the starting point of the text.
* @param[in]	style		Style of characters. Supports bold and underline style.
* @param[out]	boundary	area to be redrawn.
* @param[in]	space		space value between characters.
* @return		Always return 0.
* @remarks		Identical to GdiExtTextOut except boundary parameter.
*			boundary parameter is useful to GdiFlushPart.
* 
* if HISTORY
*				06-Jan-2006 madholic Initiated 
* endif
*/
int GdiExtTextOut(WND wnd, char * string, int length, rint32 x, rint32 y, int style, rect* boundary, int space)
{
    int width, height;
    rect bkgnd;

    if ( (wnd->window_type == WND_PANEL) && (wnd->window_visible == WND_INVISIBLE) )
        return 0;

    //GdiGetTextWidth(string, length, &width, &height);
    width = FntGetTextWidthEx(&FntGlobalContext, string, length, space);
    height = FntGetTextHeight(&FntGlobalContext);

    bkgnd.left = x;
    bkgnd.top = y;
    bkgnd.right = x + width;
    bkgnd.bottom =y + height;

    if ( width > 0 ) bkgnd.right--;
    if ( height > 0 ) bkgnd.bottom--;
    if ( boundary )
    {
        *boundary = bkgnd;
    }

    if (wnd->bg_color != COLOR_TRANSPARENT)
    {
        //fill background
        GdiFillRect(wnd, &bkgnd, DEVTORGB(wnd->bg_color));

    }

    GdiPaintString(wnd, x, y, string, length, space);

    if ( style & BOLDFONT ) {
        GdiPaintString(wnd, (x+1), y, string, length, space);
    }

    if ( style & UNDERLINEFONT ) {
        GdiDrawLine(wnd, (x), (y+height-1), (x+width), (y+height-1));
    }

    if ( style & OUTLINEFONT)
    {
        COLORVAL oldColor;
        oldColor = GdiSetFgColor( wnd, COLOR_WHITE );
        GdiPaintString(wnd, (x-1), y, string, length, space);
        GdiPaintString(wnd, (x+1), y, string, length, space);
        GdiPaintString(wnd, x, (y-1), string, length, space);
        GdiPaintString(wnd, x, (y+1), string, length, space);

        // draw original text
        GdiSetFgColor( wnd, COLOR_BLACK );
        GdiPaintString(wnd, x, y, string, length, space);
        GdiSetFgColor( wnd, oldColor );
    }

    PrvFlushPart(wnd,&bkgnd);
    return 0;
}

/**
* @brief		Draw the specified characters to window.
* @param[in]	wnd			Window to draw characters.
* @param[in]	string		Points to the character string to be drawn.
* @param[in]	length		Specifies the number of bytes in the string.
* @param[in]	x			x-coordinate of the starting point of the text.
* @param[in]	y			y-coordinate of the starting point of the text.
* @param[in]	style		Style of characters. Supports bold and underline style.
* @return		Always return 0.
* @remarks		Identical to GdiTextOut except style parameter.
*
* @if HISTORY
* 			06-Jan-2006 madholic Initiated 
* @endif
*/
int GdiExtTextOutEx(WND wnd, char * string, int length, rint32 x, rint32 y, int style)
{
    int ret;
    ret = GdiExtTextOut(wnd, string, length, x, y, style, NULL, 0 );
    return ret;
}

/**
* @brief		Draws a character string using text attribute.
* @param[in]	wnd		Window to draw characters.
* @param[in]	x		x-coordinate of the starting point of the text.
* @param[in]	y		y-coordinate of the starting point of the text.
* @param[in]	string	Points to the character string to be drawn.
* @param[in]	length	Specifies the number of bytes in the string.
* @return		Always return 0.
* @remarks		This function draws at the specified location using current foreground color.
* @see			
* @if HISTORY
*				22-Aug-2006 madholic Initiated 
* @endif
*/
int GdiTextOutAttr(WND wnd, rint32 x, rint32 y, char * string, int length)
{
    int width, height;
    rect bkgnd;
	int space = GdiGetTextSpace();
	int style = GdiGetTextStyle();
	int align = GdiGetTextAlign();
	int extent = GdiGetTextExtent();

    if ( (wnd->window_type == WND_PANEL) && (wnd->window_visible == WND_INVISIBLE) )
        return 0;

    //GdiGetTextWidth(string, length, &width, &height);
    width = FntGetTextWidthEx(&FntGlobalContext, string, length, space);
    height = FntGetTextHeight(&FntGlobalContext);

    bkgnd.left = x;
    bkgnd.top = y;
    bkgnd.right = x + width;
    bkgnd.bottom =y + height;

    if ( width > 0 ) bkgnd.right--;
    if ( height > 0 ) bkgnd.bottom--;
//    if ( boundary )
//    {
//        *boundary = bkgnd;
//    }

	if(extent>0)
	{
		switch (align)
		{
		case ALIGN_CENTER:
			x = x + (extent - width) / 2;
			break;
		case ALIGN_RIGHT:
			x = x + (extent - width);
			break;
		case ALIGN_LEFT:
		default:
			break;
		}
	}

    if (wnd->bg_color != COLOR_TRANSPARENT)
    {
        //fill background
        GdiFillRect(wnd, &bkgnd, DEVTORGB(wnd->bg_color));
    }

    GdiPaintString(wnd, x, y, string, length, space);

    if ( style & BOLDFONT ) {
        GdiPaintString(wnd, x+1, y, string, length, space);
    }

    if ( style & UNDERLINEFONT ) {
        GdiDrawLine(wnd, x, y+height-1, x+width, y+height-1);
    }

    if ( style & OUTLINEFONT)
    {
        COLORVAL oldColor;
        oldColor = GdiSetFgColor( wnd, COLOR_WHITE );
        GdiPaintString(wnd, x-1, y, string, length, space);
        GdiPaintString(wnd, x+1, y, string, length, space);
        GdiPaintString(wnd, x,	 y-1, string, length, space);
        GdiPaintString(wnd, x,	 y+1, string, length, space);

        // draw original text
        GdiSetFgColor( wnd, COLOR_BLACK );
        GdiPaintString(wnd, x, y, string, length, space);
        GdiSetFgColor( wnd, oldColor );
    }

    //GdiFlushPart(wnd,&bkgnd);
    return width;
}


/**
* @brief		Fill screen with the given color
* @param[in]	wnd		Window to clear.
* @param[in]	color	Color to fill rectangle.
* @return		void
* 		
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
void GdiClrScreen(WND wnd, COLORVAL color)
{
	rect	client;
	int screenwidth, screenheight;

	GdiGetScreenInfo( &screenwidth, &screenheight );
    GdiSetRect( &client, 0, 0, screenwidth-1, screenheight-1 );
    GdiFillRect(wnd, &client, color);

}

/**
* @brief		Do a bitblt opeartion.
* @param[in]	dest_wnd	Destination window to do bitblt operation.
* @param[in]	dest_x		Left coordinate of destination window to do bitblt operation.
* @param[in]	dest_y		Top coordinate of destination window to do bitblt operation.
* @param[in]	src_wnd		Source window to do bitblt operation.
* @param[in]	src_x		Left coordinate of source window to do bitblt operation.
* @param[in]	src_y		Top coordinate of source window to do bitblt operation.
* @param[in]	width		Width of rectangle doing bitblt operation.
* @param[in]	height		Heigth of rectangle doing bitblt operation.
* @param[in]	op			Bitblt operation code. Currently supports SRC_COPY, SRC_AND, SRC_INVERT, and DST_INVERT.
* @param[in]	alpha		Alpha blending value used with SRC_COPY operation code. 
*							On 255, do an exact copy from the source window. 
*							On 0, leave the destination window color as it was.
* @return		void
* 			
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
void GdiBitBlt(WND dest_wnd, int dest_x, int dest_y, WND src_wnd, int src_x, int src_y, int width, int height, int op, int alpha)
{
    rect src_rect, src_clip_rect, dest_clip_rect, clip_rect;
    int src_linelen, dest_linelen;
    int x_inc, y_inc;
    int i,j;
    UNITCL * src_buffer;
    UNITCL * dest_buffer;
    UNITCL * src_p;
    UNITCL * dest_p;

    if ( (dest_wnd->window_type == WND_PANEL) && (dest_wnd->window_visible == WND_INVISIBLE) )
        return;

    if ( (src_wnd->window_type == WND_PANEL) && (src_wnd->window_visible == WND_INVISIBLE) )
        return;

    src_x += src_wnd->frame.left;
    src_y += src_wnd->frame.top;
    dest_x += dest_wnd->frame.left;
    dest_y += dest_wnd->frame.top;

    // get source clipping region
    src_rect.left = src_x;
    src_rect.right = src_rect.left + width;
    src_rect.top = src_y;
    src_rect.bottom = src_rect.top + height;

    if (GdiIntersectRect(&src_clip_rect, &src_rect, &src_wnd->clip) < 0 )
        return; // no intersect region...

    // get destination dc clip rect.
    dest_clip_rect.left = dest_x + src_clip_rect.left - src_x;
    dest_clip_rect.top = dest_y + src_clip_rect.top - src_y;
    dest_clip_rect.right = dest_clip_rect.left + src_clip_rect.right - src_clip_rect.left;
    dest_clip_rect.bottom = dest_clip_rect.top + src_clip_rect.bottom - src_clip_rect.top;

    if (GdiIntersectRect(&clip_rect, &dest_clip_rect, &dest_wnd->clip) < 0)
        return; // no intersect region...

    // get src clipping rect
    dest_x = clip_rect.left;
    dest_y = clip_rect.top;

    width = clip_rect.right - clip_rect.left;
    height = clip_rect.bottom - clip_rect.top;

    src_clip_rect.left += (clip_rect.left - dest_clip_rect.left);
    src_clip_rect.top += (clip_rect.top - dest_clip_rect.top);
    src_clip_rect.right = src_clip_rect.left + width;
    src_clip_rect.bottom = src_clip_rect.top + height;

    if ( src_wnd->dev != NULL )
    {
        src_buffer = src_wnd->dev->mem_addr;
        src_linelen = src_wnd->dev->width;
    }
    else
    {
        src_buffer = RalScreenGetBuffer();
        src_linelen = RalScreenGetWidth();
    }

    if ( dest_wnd->dev != NULL )
    {
        dest_buffer = dest_wnd->dev->mem_addr;
        dest_linelen = dest_wnd->dev->width;
    }
    else
    {
        rect invalid;
        invalid.left = dest_x;
        invalid.right = dest_x + width;
        invalid.top = dest_y;
        invalid.bottom = dest_y + height;
        GdiInvalidateRect(&invalid);

        dest_buffer = RalScreenGetBuffer();
        dest_linelen = RalScreenGetWidth();
    }

    // set destination , source row start point
    if ( src_clip_rect.top < dest_y )
    {
        dest_buffer += dest_linelen * (dest_y + height - 1);
        src_buffer += src_linelen * (src_clip_rect.top + height - 1);

        dest_linelen = -dest_linelen;
        src_linelen = -src_linelen;
        y_inc = -1;
    }
    else
    {
        dest_buffer += dest_linelen * dest_y;
        src_buffer += src_linelen * src_clip_rect.top;
        y_inc = 1;
    }

    if ( src_clip_rect.left < dest_x )
    {
        dest_buffer += (dest_x + width - 1 );
        src_buffer += (src_clip_rect.left + width - 1);
        x_inc = -1;
    }
    else
    {
        dest_buffer += dest_x;
        src_buffer += src_clip_rect.left;
        x_inc = 1;
    }

    switch ( op )
    {
    case SRC_COPY :
        if( alpha ) {
            for ( i = height ; i > 0 ; --i )
            {
                src_p = src_buffer;
                dest_p = dest_buffer;
                for ( j = width ; j > 0 ; --j)
                {
                    // alpha blending
                    register UNITCL t;
                    t=(UNITCL)RX_ALPHA_BLEND_R(*src_p, *dest_p);
                    t+=(UNITCL)RX_ALPHA_BLEND_G(*src_p, *dest_p);
                    t+=(UNITCL)RX_ALPHA_BLEND_B(*src_p, *dest_p);

                    *dest_p=t;
                    src_p += x_inc;
                    dest_p += x_inc;
                }
                src_buffer += src_linelen;
                dest_buffer += dest_linelen;
            }
        }
        else
        {
            for ( i = height ; i > 0 ; --i )
            {
                src_p = src_buffer;
                dest_p = dest_buffer;
                for ( j = width ; j > 0 ; --j)
                {
                    *dest_p=*src_p;
                    src_p += x_inc;
                    dest_p += x_inc;
                }
                src_buffer += src_linelen;
                dest_buffer += dest_linelen;
            }
        }
        break;

    case SRC_AND :
        for ( i = height ; i > 0 ; --i )
        {
            src_p = src_buffer;
            dest_p = dest_buffer;
            for ( j = width ; j > 0 ; --j)
            {
                *dest_p &= *src_p;
                src_p += x_inc;
                dest_p += x_inc;
            }
            src_buffer += src_linelen;
            dest_buffer += dest_linelen;
        }
        break;

    case SRC_INVERT :
        for ( i = height ; i > 0 ; --i )
        {
            src_p = src_buffer;
            dest_p = dest_buffer;
            for ( j = width ; j > 0 ; --j)
            {
                *dest_p ^= *src_p;
                src_p += x_inc;
                dest_p += x_inc;
            }
            src_buffer += src_linelen;
            dest_buffer += dest_linelen;
        }
        break;

    case DST_INVERT :
        for ( i = height ; i > 0 ; --i )
        {
            dest_p = dest_buffer;
            for ( j = width ; j > 0 ; --j)
            {
                *dest_p = (unsigned short)(~*dest_p);
                dest_p += x_inc;
            }
            dest_buffer += dest_linelen;
        }
        break;
    }
}



/**
 * @brief		Draw a bitmap to window.
 * @param[in]	wnd		Window to draw bitmap.
 * @param[in]	bitmap	Bitmap handle to draw.
 * @param[in]	src_x	X coordinate of bitmap to read(left bound).
 * @param[in]	src_y	Y coordinate of bitmap to read(top bound).
 * @param[in]	dst_x	X coordinate of target window to draw(left bound).
 * @param[in]	dst_y	Y coordinate of target window to draw(top bound).
 * @param[in]	width	Width of bitmap to read(draw).
 * @param[in]	height	Height of bitmap to read(draw).
 * @retval		RxErrNone		On success		
 * @retval		enInvisibleWindow		If Window type is panel, or attribute of window is invisible
 * @retval		enNullPointer			If bitmap pointer or bitmap data pointer is null
 * @remarks		This function draws the bitmap image contained in the RxBITMAP to the window. 
 *				You can load the bitmap image file to the RxBITMAP structure by calling GdiLoadDIBitmap.
 * 		
 * @if HISTORY
 *				06-Jan-2006 madholic Initiated 
 * @endif
 */
RxErr GdiDrawBitmap(WND wnd, RxBITMAP * bitmap, int src_x, int src_y, int dst_x, int dst_y, int width, int height)
{
    int i, j, dst_linelen;
    UNITCL * src_p;
    UNITCL * dst_p;
    rect src_rect, src_clip_rect, bitmap_rect;
    rect clip_rect, dst_clip_rect;

    if ( (wnd->window_type == WND_PANEL) && (wnd->window_visible == WND_INVISIBLE) )
        return ( RxErrGdiClass | enInvisibleWindow );

    if ( (bitmap == NULL) || (bitmap->data == NULL) )
        return ( RxErrGdiClass | enNullPointer );

    // get source clipping region
    src_rect.left = src_x;
    src_rect.right = src_rect.left + width - 1;
    src_rect.top = src_y;
    src_rect.bottom = src_rect.top + height - 1;

    bitmap_rect.left = 0;
    bitmap_rect.top = 0;
    bitmap_rect.right = bitmap->width - 1;
    bitmap_rect.bottom = bitmap->height - 1;

    if (GdiIntersectRect(&src_clip_rect, &src_rect, &bitmap_rect) < 0 )
        return (RxErrGdiClass | enNoIntersectRegion); // no intersect region...

    // offset
    dst_x += wnd->frame.left;
    dst_y += wnd->frame.top;

    // get destination dc clip rect.
    dst_clip_rect.left = dst_x + src_clip_rect.left - src_x;
    dst_clip_rect.top = dst_y + src_clip_rect.top - src_y;
    dst_clip_rect.right = dst_clip_rect.left + src_clip_rect.right - src_clip_rect.left;
    dst_clip_rect.bottom = dst_clip_rect.top + src_clip_rect.bottom - src_clip_rect.top;

    if (GdiIntersectRect(&clip_rect, &dst_clip_rect, &wnd->clip) < 0)
        return (RxErr)-1; // no intersect region...

    // get src clipping rect
    dst_x = clip_rect.left;
    dst_y = clip_rect.top;

    width = clip_rect.right - clip_rect.left + 1;
    height = clip_rect.bottom - clip_rect.top + 1;

    src_clip_rect.left += (clip_rect.left - dst_clip_rect.left);
    src_clip_rect.top += (clip_rect.top - dst_clip_rect.top);
    src_clip_rect.right = src_clip_rect.left + width - 1;
    src_clip_rect.bottom = src_clip_rect.top + height - 1;

    // set source bitmap address.
    src_p = bitmap->data;
    src_p += bitmap->width * src_clip_rect.top + src_clip_rect.left;

    // set destination screen address
    if ( wnd->dev != NULL )
    {
        dst_p = wnd->dev->mem_addr;
        dst_linelen = wnd->dev->width;
    }
    else
    {
        rect invalid;
        invalid.left = dst_x;
        invalid.right = dst_x + width - 1;
        invalid.top = dst_y;
        invalid.bottom = dst_y + height - 1;
        GdiInvalidateRect(&invalid);

        dst_p = RalScreenGetBuffer();
        dst_linelen = RalScreenGetWidth();
    }

    dst_p += dst_linelen * dst_y + dst_x;

    if ( bitmap->bTransparent == BITMAP_TRANSPARENT)
    {
        for ( i = 0; i < height; ++i )
        {
            for ( j = 0; j < width; ++j )
            {
				//COLOR_TRANSPARENT(255,0,255) is 0xF81F
//                if ( *src_p != COLOR_TRANSPARENT )
                if ( *src_p != bitmap->transparentColor )
                    *dst_p = *src_p;

                dst_p++;
                src_p++;
            }

            dst_p += dst_linelen - width;
            src_p += bitmap->width - width;
        }
    }
    else
    {
        for ( i = 0; i < height; ++i )
        {
            for ( j = 0; j < width; ++j )
            {
                *dst_p = *src_p;
                dst_p++;
                src_p++;
            }

            dst_p += dst_linelen - width;
            src_p += bitmap->width - width;
        }
    }
    PrvFlushPart( wnd, &dst_clip_rect );
    return 0;
}

/**
 * @brief		Inflate the passed rectangle area using the given x and y value.
 * @param[in]	pRect		The rectangle area that will be inflated
 * @param[in]	dx		The left and right value that will be inflated
 * @param[in]	dy		The top and bottom value that will be inflated
 * @retval		RxErrNone	On success
 * @retval		Error code	Except on success
 * @remarks		for example, if dx is 2, rectangle area is inflated as 2 pixel left and right.
 * 		
 * @if HISTORY
 *				06-Jan-2006 madholic Initiated 
 * @endif
 */
RxErr GdiInflateRect(rect *pRect, int dx, int dy)
{
    if (!pRect)
		return ( RxErrGdiClass | enNullPointer );

    pRect->left -= dx;
    pRect->top -= dy;
    pRect->right += dx;
    pRect->bottom += dy;

    return RxErrNone;
}

/**
 * @brief		Deflate the passed rectangle area using the given x and y value.
 * @param[in]	pRect		The rectangle area that will be deflated
 * @param[in]	dx			The left and right value that will be deflated
 * @param[in]	dy			The top and bottom value that will be deflated
 * @retval		RxErrNone	On success
 * @retval		Error code	Except on success
 * @remarks		for example, if dx is 2, rectangle area is deflated as 2 pixel left and right.
 * 		
 * @if HISTORY
 *				18-Aug-2006 madholic Initiated 
 * @endif
 */
RxErr GdiDeflateRect(rect *pRect, int dx, int dy)
{
    if (!pRect)
		return ( RxErrGdiClass | enNullPointer );

    pRect->left += dx;
    pRect->top += dy;
    pRect->right -= dx;
    pRect->bottom -= dy;

    return RxErrNone;
}

/**
 * @brief		Move the passed rectangle area using the given x and y value.
 * @param[in]	pRect	The rectangle area that will be moved
 * @param[in]	dx		The value that will be moved horizontally, If negative value, rectangle will be moved left; 
 *						In case of reverse, will be moved right
 * @param[in]	dy		The value that will be moved vertically, If negative value, rectangle will be moved left; 
 *						In case of reverse, will be moved right
 * @retval		1		On success
 * @retval		0		Except on success
 * @remarks		for example, if x is 2, rectangle area is moved as 2 pixel right
 *							 if x is -2, rectangle area is moved as 2 pixel left
 * 		
 * @if HISTORY
 *				06-Jan-2006 madholic Initiated 
 * @endif
 */
int GdiMoveRect(rect *pRect, int dx, int dy)
{
    if (!pRect)
        return 0;

    pRect->left += dx;
    pRect->top += dy;
    pRect->right += dx;
    pRect->bottom += dy;

    return 1;
}

/**
* @brief		Set the given coordinates to the passed rectangle.
* @param[in]	pRect	The rectangle area that will be set
* @param[in]	left	The left coordinates
* @param[in]	top		The top coordinates
* @param[in]	right	The right coordinates
* @param[in]	bottom	The bottom coordinates
* @retval		1		On success		
* @retval		0		Except on success		
*		
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
int GdiSetRect(rect *pRect, int left, int top, int right, int bottom)
{
    if (!pRect)
        return 0;

    SET_RECT(pRect, left, top, right, bottom);

    //	pRect->left = left;
    //	pRect->top = top;
    //	pRect->right = right;
    //	pRect->bottom = bottom;

    return 1;
}

/**
* @brief		Do a strecthblt operation.
* @param[in]	dest_wnd	Destination window to do stretchbit operation.
* @param[in]	dx Left		coordinate of destination window to do stretchbit operation.
* @param[in]	dy Top		coordinate of destination window to do stretchbit operation.
* @param[in]	src_wnd		Source window to do stretchbit operation.
* @param[in]	sx Left		coordinate of source window to do stretchbit operation.
* @param[in]	sy Top		coordinate of source window to do stretchbit operation.
* @param[in]	d_width		Width of rectangle doing stretchbit operation in destination window.
* @param[in]	d_height	Heigth of rectangle doing stretchbit operation in destination window.
* @param[in]	s_width		Width of rectangle doing stretchbit operation in source window.
* @param[in]	s_height	Heigth of rectangle doing stretchbit operation in source window.
* @param[in]	op			StretchBlt operation code. Currently supports SRC_COPY, SRC_AND, SRC_INVERT, and DST_INVERT.
* @return		void
*		
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
void GdiStretchBlt(	WND dest_wnd, int dx, int dy,
                    WND src_wnd, int sx, int sy,
                    int d_width, int d_height, int s_width, int s_height, int op)
{
    UNITCL *dest_base_p, *src_base_p, *dest_row_p, *src_row_p, *dest_p, *src_p;
    UNITCL * src_buffer;
    int sRow_inc, sCol_inc, x_scale, y_scale;
    rect src_rect, src_clip_rect, dest_clip_rect, clip_rect;
    int i, j, width, height, temp;
    int linelen_dest, linelen_src, d_pix_line, s_pix_line;
    int sx_reverse, sy_reverse, dx_reverse, dy_reverse;

    if ( (dest_wnd->window_type == WND_PANEL) && (dest_wnd->window_visible == WND_INVISIBLE) )
        return;

    if ( (src_wnd->window_type == WND_PANEL) && (src_wnd->window_visible == WND_INVISIBLE) )
        return;

    // physical offset
    sx += src_wnd->frame.left;
    sy += src_wnd->frame.top;
    dx += dest_wnd->frame.left;
    dy += dest_wnd->frame.top;

    // get source, dest wnd line length
    if ( src_wnd->dev != NULL )
    {
        linelen_src = src_wnd->dev->width * sizeof(UNITCL);
        s_pix_line = src_wnd->dev->width;
    }
    else
    {
        linelen_src = RalScreenGetWidth() * sizeof(UNITCL);
        s_pix_line = RalScreenGetWidth();
    }

    if ( dest_wnd->dev != NULL )
    {
        linelen_dest = dest_wnd->dev->width * sizeof(UNITCL);
        d_pix_line = dest_wnd->dev->width;
    }
    else
    {
        linelen_dest = RalScreenGetWidth() * sizeof(UNITCL);
        d_pix_line = RalScreenGetWidth();
    }

    // get source clipping region
    src_rect.left = sx;
    src_rect.right = src_rect.left + s_width;
    src_rect.top = sy;
    src_rect.bottom = src_rect.top + s_height;

    if ( GdiIntersectRect(&src_clip_rect, &src_rect, &src_wnd->clip) == -1 )
        return; // no intersect region...


    // SWAP left<->right, top<->bottom for base address setting.
    if ( s_width < 0 )
    {
        sx_reverse = -1;
        SWAP(src_clip_rect.left, src_clip_rect.right);
    }
    else
    {
        sx_reverse = 1;
    }

    if ( s_height < 0 )
    {
        sy_reverse = -1;
        SWAP(src_clip_rect.top, src_clip_rect.bottom);
    }
    else
    {
        sy_reverse = 1;
    }


    // calculate scale factor.
    x_scale = ABS((s_width * SCALE)  / d_width);
    y_scale = ABS((s_height * SCALE) / d_height);

    // set the sign of destination width and height.
    if ( d_width < 0 )
        dx_reverse = -1;
    else
        dx_reverse = 1;

    if ( d_height < 0 )
        dy_reverse = -1;
    else
        dy_reverse = 1;

    // get destination dc clip rect.
    dest_clip_rect.left =   dx + ((ABS(src_clip_rect.left - sx) * SCALE) / x_scale) * dx_reverse ;
    dest_clip_rect.right =  dest_clip_rect.left + ((ABS(src_clip_rect.right  - src_clip_rect.left) * SCALE) / x_scale) * dx_reverse;
    dest_clip_rect.top =    dy + ((ABS(src_clip_rect.top -  sy) * SCALE) / y_scale) * dy_reverse;
    dest_clip_rect.bottom = dest_clip_rect.top  + ((ABS(src_clip_rect.bottom - src_clip_rect.top)  * SCALE) / y_scale) * dy_reverse;

    if (GdiIntersectRect(&clip_rect, &dest_clip_rect, &dest_wnd->clip) == -1)
        return; // no intersect region...

    // Swap left<->right, top<->bottom for the base address setting
    if ( d_width < 0 )
    {
        SWAP(clip_rect.left, clip_rect.right);
        SWAP(dest_clip_rect.left, dest_clip_rect.right);
    }

    if ( d_height < 0 )
    {
        SWAP(clip_rect.top, clip_rect.bottom);
        SWAP(dest_clip_rect.top, dest_clip_rect.bottom);
    }


    dx = clip_rect.left;
    dy = clip_rect.top;
    width = ABS(clip_rect.right - clip_rect.left);
    height = ABS(clip_rect.bottom - clip_rect.top);

    if ( dest_wnd->dev == NULL )
    {
        rect invalid;
        invalid.left = dx;
        invalid.right = dx + width;
        invalid.top = dy;
        invalid.bottom = dy + height;
        GdiInvalidateRect(&invalid);
    }

    // round if possible. because x_scale and y_scale are smaller than the real value (they are integer type)
    // we need to round the value in case of multiplication
    src_clip_rect.left += (int)(((float)ABS(clip_rect.left - dest_clip_rect.left) * x_scale) / SCALE ) * sx_reverse;
    src_clip_rect.top +=  (int)(((float)ABS(clip_rect.top  - dest_clip_rect.top)  * y_scale) / SCALE ) * sy_reverse;
    src_clip_rect.right = (int)(src_clip_rect.left + ((((float)width  * x_scale) / SCALE) ) * sx_reverse);
    src_clip_rect.bottom = (int)(src_clip_rect.top + ((((float)height * y_scale) / SCALE) ) * sy_reverse);

    // set the size of source area.
    s_width = ABS(src_clip_rect.right - src_clip_rect.left);
    s_height = ABS(src_clip_rect.bottom - src_clip_rect.top);

    if ( s_width == 0 )
        s_width = 1;

    if ( s_height == 0 )
        s_height = 1;

    // recalculate scale factor. this can be a different value from the ones above
    x_scale = ABS((s_width * SCALE) / width);
    y_scale = ABS((s_height * SCALE) / height);


    //allocate the buffer
    src_buffer = (UNITCL * )MemAlloc((int)(s_width * s_height * sizeof(UNITCL)));
    if ( src_buffer == NULL )
        return;

    //set base address and copy the source to the buffer
    if ( src_wnd->dev != NULL )
        src_base_p = src_wnd->dev->mem_addr + linelen_src * src_clip_rect.top + src_clip_rect.left * sizeof(UNITCL);
    else
        src_base_p = RalScreenGetBuffer() + linelen_src * src_clip_rect.top + src_clip_rect.left * sizeof(UNITCL);

    for ( i = 0; i < s_height ; ++i )
    {
        src_row_p = src_base_p + s_pix_line * i * sy_reverse;
        for ( j = 0; j < s_width ; ++j )
        {
            src_p = src_row_p + j * sx_reverse;
            *(src_buffer + i * s_width + j) = *src_p;
        }
    }

    // calcualte base address
    src_base_p = src_buffer;
    if ( dest_wnd->dev != NULL )
        dest_base_p = dest_wnd->dev->mem_addr + d_pix_line * dy + dx;
    else
        dest_base_p = RalScreenGetBuffer() + d_pix_line * dy + dx;


    sRow_inc = 0;
    src_row_p = src_base_p;

    switch ( op )
    {

    case SRC_COPY :
        for ( i = 0; i < height; ++i )
        {
            int merged_row;

            merged_row = 0;
            dest_row_p = dest_base_p + d_pix_line * i * dy_reverse;	// set destination row position
            while ( SCALE <= sRow_inc )					// set source row position
            {
                if ( s_height > d_height )
                {
                    PrvMergeRow(src_row_p, s_width, COLOR_ON_COLOR, merged_row);
                    ++merged_row;
                }
                src_row_p += s_width;				// consider SetBltMode later.
                sRow_inc -= SCALE;
            }

            sCol_inc = 0;
            src_p = src_row_p;

            for ( j = 0; j < width; ++ j )
            {
                int merged_col;

                merged_col = 0;
                dest_p = dest_row_p + j * dx_reverse;		// set destination col position
                while ( SCALE <= sCol_inc )				// set source column position
                {
                    if ( s_width > d_width )
                    {
                        PrvMergeCol(src_p, COLOR_ON_COLOR, merged_col);
                        ++merged_col;
                    }
                    src_p++;				// consider SetBltMode later.
                    sCol_inc -= SCALE;
                }

                *dest_p = *src_p;
                sCol_inc += x_scale;
            }
            sRow_inc += y_scale;
        }
        break;

    case SRC_AND :
        for ( i = 0; i < height; ++i )
        {
            int merged_row;

            merged_row = 0;
            dest_row_p = dest_base_p + d_pix_line * i * dy_reverse;	// set destination row position
            while ( SCALE <= sRow_inc )					// set source row position
            {
                if ( s_height > d_height )
                {
                    PrvMergeRow(src_row_p, s_width, COLOR_ON_COLOR, merged_row);
                    ++merged_row;
                }
                src_row_p += s_width;				// consider SetBltMode later.
                sRow_inc -= SCALE;
            }

            sCol_inc = 0;
            src_p = src_row_p;

            for ( j = 0; j < width; ++ j )
            {
                int merged_col;

                merged_col = 0;
                dest_p = dest_row_p + j * dx_reverse;		// set destination col position
                while ( SCALE <= sCol_inc )				// set source column position
                {
                    if ( s_width > d_width )
                    {
                        PrvMergeCol(src_p, COLOR_ON_COLOR, merged_col);
                        ++merged_col;
                    }
                    src_p++;				// consider SetBltMode later.
                    sCol_inc -= SCALE;
                }


                *dest_p &= *src_p;
                sCol_inc += x_scale;
            }
            sRow_inc += y_scale;
        }
        break;

    case SRC_INVERT :
        for ( i = 0; i < height; ++i )
        {
            int merged_row;

            merged_row = 0;
            dest_row_p = dest_base_p + d_pix_line * i * dy_reverse;	// set destination row position
            while ( SCALE <= sRow_inc )					// set source row position
            {
                if ( s_height > d_height )
                {
                    PrvMergeRow(src_row_p, s_width, COLOR_ON_COLOR, merged_row);
                    ++merged_row;
                }
                src_row_p += s_width;				// consider SetBltMode later.
                sRow_inc -= SCALE;
            }

            sCol_inc = 0;
            src_p = src_row_p;

            for ( j = 0; j < width; ++ j )
            {
                int merged_col;

                merged_col = 0;
                dest_p = dest_row_p + j * dx_reverse;		// set destination col position
                while ( SCALE <= sCol_inc )				// set source column position
                {
                    if ( s_width > d_width )
                    {
                        PrvMergeCol(src_p, COLOR_ON_COLOR, merged_col);
                        ++merged_col;
                    }
                    src_p++;				// consider SetBltMode later.
                    sCol_inc -= SCALE;
                }


                *dest_p ^= *src_p;
                sCol_inc += x_scale;
            }
            sRow_inc += y_scale;
        }
        break;

    case DST_INVERT :
        for ( i = 0; i < height; ++i )
        {
            int merged_row;

            merged_row = 0;
            dest_row_p = dest_base_p + d_pix_line * i * dy_reverse;	// set destination row position
            while ( SCALE <= sRow_inc )					// set source row position
            {
                if ( s_height > d_height )
                {
                    PrvMergeRow(src_row_p, s_width, COLOR_ON_COLOR, merged_row);
                    ++merged_row;
                }
                src_row_p += s_width;				// consider SetBltMode later.
                sRow_inc -= SCALE;
            }

            sCol_inc = 0;
            src_p = src_row_p;

            for ( j = 0; j < width; ++ j )
            {
                int merged_col;

                merged_col = 0;
                dest_p = dest_row_p + j * dx_reverse;		// set destination col position
                while ( SCALE <= sCol_inc )				// set source column position
                {
                    if ( s_width > d_width )
                    {
                        PrvMergeCol(src_p, COLOR_ON_COLOR, merged_col);
                        ++merged_col;
                    }
                    src_p++;				// consider SetBltMode later.
                    sCol_inc -= SCALE;
                }

                *dest_p = (unsigned short)(~*dest_p);
                sCol_inc += x_scale;
            }
            sRow_inc += y_scale;
        }
        break;
    }

    MemFree(src_buffer);
}

/**
* @brief		Dim target area of window.
* @param[in]	wnd		Target window.
* @param[in]	dest	Rectangle area to give dim effect.
* @return		void
* 			
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
void GdiDimOut(WND wnd, rect *dest)
{
    register int y;
    rect clipped_rect;
    int dest_linelen;
    register int i, j;
    UNITCL * dest_buffer;
    UNITCL * dest_p;
    int width, height;

    if (!wnd)
        return;

    // get clipped dest rect
    clipped_rect = *dest;
    clipped_rect.left += wnd->frame.left;
    clipped_rect.right += wnd->frame.left;
    clipped_rect.top += wnd->frame.top;
    clipped_rect.bottom += wnd->frame.top;

    // if intersect region between dest and clip region, just return
    if (GdiIntersectRect(&clipped_rect, &clipped_rect, &wnd->clip) < 0)
        return;

    width = clipped_rect.right - clipped_rect.left + 1;
    height = clipped_rect.bottom - clipped_rect.top + 1;

    if ( wnd->dev != NULL )
    {
        dest_linelen = wnd->dev->width;
        dest_buffer = wnd->dev->mem_addr + dest_linelen * dest->top;
    }
    else
    {
        GdiInvalidateRect(&clipped_rect);

        dest_linelen = RalScreenGetWidth();
        dest_buffer = RalScreenGetBuffer() + dest_linelen * clipped_rect.top + clipped_rect.left;
    }

    for ( i = height; i > 0; i-- )
    {
        dest_p = dest_buffer;
        dest_buffer += dest_linelen;

        for ( j = width; j > 0; j-- )
        {
            // half alpha blending
            y=( ((SRC_COLOR & UNITCL_R_MASK)+(*dest_p & UNITCL_R_MASK))/2 ) & UNITCL_R_MASK;
            y+=( ((SRC_COLOR & UNITCL_G_MASK)+(*dest_p & UNITCL_G_MASK))/2 ) & UNITCL_G_MASK;
            y+=( ((SRC_COLOR & UNITCL_B_MASK)+(*dest_p & UNITCL_B_MASK))/2 );

            // 75% alpha blending
            // y=( ((SRC_COLOR & UNITCL_R_MASK)+(*dest_p & UNITCL_R_MASK))/4+(*dest_p & UNITCL_R_MASK)/2 ) & UNITCL_R_MASK;
            // y+=( ((SRC_COLOR & UNITCL_G_MASK)+(*dest_p & UNITCL_G_MASK))/4+(*dest_p & UNITCL_G_MASK)/2 ) & UNITCL_G_MASK;
            // y+=( ((SRC_COLOR & UNITCL_B_MASK)+(*dest_p & UNITCL_B_MASK))/4+(*dest_p & UNITCL_B_MASK)/2 );

            *dest_p=(UNITCL)y;
            dest_p++;
        }
    }
}

/** 
 * @brief		Get screen memory size totally
 * @return		return screen memory size
 * @if HISTORY
 *				26-Jul-2006 madholic Initiated 
 * @endif
 */
int GdiScreenGetMemSize(void)
{
	return (RalScreenGetWidth() * RalScreenGetHeight() * sizeof(ruint16));
}

/** 
 * @brief		화면의 특정 영역을 저장하는데 필요한 메모리 요구량은 알려줌
 * @param[in]	pArea	저장할 화면 영역
 * @return		특정 영역을 저장하는데 필요한 메모리 양
 * @if HISTORY
 *				06-Jul-2006 yich Initiated 
 *              26-Jul-2006 madholic modified function name
 * @endif
 */
int	GdiScreenGetMemSizeArea(rect *pArea)
{
	if ( pArea )
	{
		rect	RealRect;

		// Copy clipping area into local variable
		// to check integrity
		RealRect = *pArea;

		if ( RealRect.top < 0 ) // Top
			RealRect.top = 0;
		else if ( RealRect.top > (RalScreenGetHeight() - 1) )	// There's nothing to be clipped
			return 0;

		if ( RealRect.left < 0 ) // Left
			RealRect.left = 0;
		else if ( RealRect.left > (RalScreenGetWidth() - 1) )	// There's nothing to be clipped
			return 0;

		if ( RealRect.right > (RalScreenGetWidth() - 1) ) // Right
			RealRect.right = (RalScreenGetWidth() - 1);
		else if ( RealRect.right < 0 )	// There's nothing to be clipped
			return 0;
		
		if ( RealRect.bottom > (RalScreenGetHeight() - 1) )
			RealRect.bottom = (RalScreenGetHeight() - 1);
		else if ( RealRect.bottom < 0 )	// There's nothing to be clipped
			return 0;

		return sizeof(rect) + 
			(RealRect.right - RealRect.left + 1) * (RealRect.bottom - RealRect.top + 1) * sizeof(UNITCL);

	}
	else
	{
		return sizeof(rect) + RalScreenGetMemSize();
	}
}

/**
* @anchor		GdiScreenSave
* @brief		Save the current screen.
* @param[in]	pArea	clipping area where source image data is to be clipped
* @param[in]	pData	화면 영역이 저장될 공간에 대한 포인터
*
* @return		Success or Fail
* @remarks		You can copy whole screen buffer or part of it to user's buffer
*				so that can use later. To restore these image data unto main screen buffer
*				to be seen, use GdiScreenRestore(). Set pArea as NULL if you copy whole screen.
*				Also it is not necessary that the pArea in GdiScreenSave() call should be same with
*				one of GdiScreenRestore()
* @see			GdiWinRestoreBits
* @if HISTORY
*				06-Jul-2006 yich	 Initiated 
* @endif
*/
bool GdiScreenSave(rect *pArea, void* pData)
{
	UNITCL *SrcPtr;
	UNITCL *DstPtr;
		
	DstPtr = pData;
	SrcPtr = RalScreenGetBuffer();

	if ((DstPtr == NULL) || (SrcPtr == NULL))
		return FALSE;

	MemCopy(DstPtr, pArea, sizeof(rect));
	DstPtr = (UNITCL*)((byte*)DstPtr + sizeof(rect));
	
	if ( pArea )
	{
		rint16	w;
		rint16	y;
		int		inx;
		rect	RealRect;

		// Copy clipping area into local variable
		// to check integrity
		RealRect = *pArea;

		if ( RealRect.top < 0 ) // Top
			RealRect.top = 0;
		else if ( RealRect.top > (RalScreenGetHeight() - 1) )	// There's nothing to be clipped
			return FALSE;

		if ( RealRect.left < 0 ) // Left
			RealRect.left = 0;
		else if ( RealRect.left > (RalScreenGetWidth() - 1) )	// There's nothing to be clipped
			return FALSE;

		if ( RealRect.right > (RalScreenGetWidth() - 1) ) // Right
			RealRect.right = (RalScreenGetWidth() - 1);
		else if ( RealRect.right < 0 )	// There's nothing to be clipped
			return FALSE;
		
		if ( RealRect.bottom > (RalScreenGetHeight() - 1) )
			RealRect.bottom = (RalScreenGetHeight() - 1);
		else if ( RealRect.bottom < 0 )	// There's nothing to be clipped
			return FALSE;

		inx = 0;
		w = RealRect.right - RealRect.left + 1;
		for ( y = RealRect.top ; y <= RealRect.bottom; y++ )
		{
			MemCopy(DstPtr + inx, 
					&SrcPtr[y * RalScreenGetWidth() + RealRect.left], 
					w * sizeof(UNITCL));
			inx += w;
		}
	}
	else
	{
		MemCopy(DstPtr, SrcPtr, RalScreenGetMemSize());
	}

	return TRUE;
}

/**
* @anchor		GdiScreenRestore
* @brief		Restore saved screen.
* @param[in]	pData	a pointer to data and rectangle structure which contains restore area info
*						MUST be a pointer that returns GdiScreenSave().
* @return		성공시 TRUE, 그외 FALSE
* @remarks		You can restore screen buffer image calling this API.
*
* @if HISTORY
*				06-Jul-2006 yich 	Initiated 
* @endif
*/
bool GdiScreenRestore(void* pData)
{
	UNITCL *DstPtr, *SrcPtr;
	rect   *pArea;
	
	DstPtr = RalScreenGetBuffer();
	SrcPtr = pData;

	if ((DstPtr == NULL) || (SrcPtr == NULL))
		return FALSE;

	pArea = (rect*)pData;
	SrcPtr = (UNITCL*)((byte*)SrcPtr + sizeof(rect));

	if ( pArea )
	{
		rint16	w;
		rint16	y;
		int		inx;
		rect	RealRect;

		// Copy clipping area into local variable
		// to check integrity
		RealRect = *pArea;

		if ( RealRect.top < 0 ) // Top
			RealRect.top = 0;
		else if ( RealRect.top > (RalScreenGetHeight() - 1) )	// There's nothing to be clipped
			return FALSE;

		if ( RealRect.left < 0 ) // Left
			RealRect.left = 0;
		else if ( RealRect.left > (RalScreenGetWidth() - 1) )	// There's nothing to be clipped
			return FALSE;

		if ( RealRect.right > (RalScreenGetWidth() - 1) ) // Right
			RealRect.right = (RalScreenGetWidth() - 1);
		else if ( RealRect.right < 0 )	// There's nothing to be clipped
			return FALSE;
		
		if ( RealRect.bottom > (RalScreenGetHeight() - 1) ) // Bottom
			RealRect.bottom = (RalScreenGetHeight() - 1);
		else if ( RealRect.bottom < 0 )	// There's nothing to be clipped
			return FALSE;

		inx = 0;
		w = RealRect.right - RealRect.left + 1;
		for ( y = RealRect.top ; y <= RealRect.bottom; y++ )
		{
			MemCopy(&DstPtr[y * RalScreenGetWidth() + RealRect.left], 
					SrcPtr + inx, 					
					w * sizeof(UNITCL));
			inx += w;
		}

		PrvFlushPart( WinGetSelection(), &RealRect );
	}
	else
	{
		MemCopy(DstPtr, SrcPtr, RalScreenGetMemSize());
		PrvFlushAll(NULL);
	}

	return TRUE;
}

/**
* @brief		Check whether the point coordinate is included in the rectangle.
* @param[in]	lprc	Rectangle data structure.
* @param[in]	pt		Point data structure.
* @retval		1		In case that the point is in the rectangle
* @retval		0		In case that the point isn't in the rectangle
* 		
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
int GdiPtInRect(rect * lprc, Point * pt)
{
    if (lprc->left <= pt->x && pt->x <= lprc->right &&
            lprc->top <= pt->y && pt->y <= lprc->bottom)
        return 1;
    else
        return 0;
}

/**
* @brief		Find the intersect rectangle of two rectangles.
* @param[out]	result	Intersected rectangle coordinates.
* @param[in]	src1	Source rectangle1.
* @param[in]	src2	Source rectangle2.
* @retval		0		If two rectangles are intersected
* @retval		1		If two rectangles are not intersected
* 		
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
int GdiIntersectRect(rect *  result, rect * src1, rect * src2)
{
    long temp;

    if (src1->left > src1->right)
        SWAP(src1->left, src1->right);
    if (src1->top > src1->bottom)
        SWAP(src1->top, src1->bottom);
    if (src2->left > src2->right)
        SWAP(src2->left, src2->right);
    if (src2->top > src2->bottom)
        SWAP(src2->top, src2->bottom);

    result->left = max(src1->left, src2->left);
    result->right = min(src1->right, src2->right);
    if (result->left > result->right)
    {
        result->left = result->right = result->top = result->bottom = 0;
        return -1;
    }

    result->top = max(src1->top, src2->top);
    result->bottom = min(src1->bottom, src2->bottom);
    if (result->top > result->bottom)
    {
        result->left = result->right = result->top = result->bottom = 0;
        return -1;
    }

    return 0;
}

/**
* @brief		Set current text alignment.
* @param[in]	align	align type. These type is declared in the rexy_common.h file as follows.
*				\#define ALIGN_LEFT		0x00\n
*				\#define ALIGN_RIGHT	0x10\n
*				\#define ALIGN_CENTER	0x20\n
*
* @retval		Returns the previous alignment type
* @remarks		If you call this function before use a sort of textout function, you can adjust alignment attribute.
* 			
* @if HISTORY
*				16-May-2006 madholic Initiated 
* @endif
*/
int GdiSetTextAlign(int align)
{
	int prevAlign = g_TextAlign;

	g_TextAlign = align;

	return prevAlign;
}

/**
* @brief		Get current text align.
* @retval		Returns the current align
*
* @if HISTORY
*				16-May-2006 madholic Initiated 
* @endif
*/
int GdiGetTextAlign(void)
{
	return g_TextAlign;
}

/**
* @brief		Set current text space.
* @param[in]	space	space value between characters.
*						
* @retval		Returns the previous space value
* @remarks		If you call this function before use a sort of textout function, you can adjust space value between characters.
*			
* @if HISTORY
*				16-May-2006 madholic Initiated 
* @endif
*/
int GdiSetTextSpace(int space)
{
	int prevSpace = g_TextSpace;

	g_TextSpace = space;

	return prevSpace;
}

/**
* @brief		Get current text space.
* @retval		Returns the current space
*			
* @if HISTORY
*				16-May-2006 madholic Initiated 
* @endif
*/
int GdiGetTextSpace(void)
{
	return g_TextSpace;
}

/**
* @brief		Set current text font style.
* @param[in]	style	style type. These type is declared in the rexy_const.h file as follows.
*						
*				\#define NORMALFONT						0x00\n
*				\#define UNDERLINEFONT					0x01\n
*				\#define BOLDFONT						0x02\n
*				\#define OUTLINEFONT					0x04\n
*
* @retval		Returns the previous style type
* @remarks		If you call this function before use a sort of textout function, you can adjust font style.
*			
* @if HISTORY
*				16-May-2006 madholic Initiated 
* @endif
*/
int GdiSetTextStyle(int style)
{
	int prevStyle = g_TextStyle;	

	g_TextStyle = style;

	return prevStyle;
}

/**
* @brief		Get current text style.
* @retval		Returns the current style
* 		
* @if HISTORY
*				16-May-2006 madholic Initiated 
* @endif
*/
int GdiGetTextStyle(void)
{
	return g_TextStyle;
}

/**
* @brief		Set current text extent.
* @param[in]	extent	Specifies extent of the current line for text alignment. Note this value is pixel unit.
*						
* @retval		Returns the previous extent
* @remarks		If you call this function before use a sort of textout function, you can change text extent.
* 		
* @if HISTORY
*				16-May-2006 madholic Initiated 
* @endif
*/
int GdiSetTextExtent(int extent)
{
	int prevExtent = g_TextExtent;	

	g_TextExtent = extent;

	return prevExtent;
}

/**
* @brief		Get current text extent.
* @retval		Returns the current extent
* 			
* @if HISTORY
*				16-May-2006 madholic Initiated 
* @endif
*/
int GdiGetTextExtent(void)
{
	return g_TextExtent;
}

/**
* @brief		매개변수로 넘어온 윈도우의 client 좌표를 얻음.
* @retval		Returns the error code
* 			
* @param[in]	wnd		좌표를 얻고자 하는 윈도우.
* @param[out]	rt		좌표를 받을 사각형 변수.
*
* @if HISTORY
*				18-Aug-2006 madholic Initiated 
* @endif
*/
RxErr GdiGetClientRect(WND wnd, rect* rt)
{
	rt->left = 0;
	rt->top = 0;
	rt->right = wnd->frame.right - wnd->frame.left;
	rt->bottom = wnd->frame.bottom - wnd->frame.top;

    return RxErrNone;
}

void GdiGetWindowRect(WND wnd, rect* rt)
{
	rt->left = wnd->frame.left;
	rt->top = wnd->frame.top;
	rt->right = wnd->frame.right;
	rt->bottom = wnd->frame.bottom;
}

static void PrvDrawHLine(WND wnd, int x1, int y1, int x2)
{
    int i, delta, temp, linelen;
    UNITCL *screenPtr;

    if ( (wnd->window_type == WND_PANEL) && (wnd->window_visible == WND_INVISIBLE) )
        return;

    if (x1 > x2)
    {
        x1++, x2++;		/* not including the last point */
        SWAP(x1, x2);
    }

    if ((y1 < MIN_Y) || (y1 > MAX_Y))
        return;

    if ((x2 < MIN_X) || (x1 > MAX_X))
        return;

    if (x1 < MIN_X)
        x1 = MIN_X;

    if (x2 > MAX_X)
        x2 = MAX_X;

    if ( wnd->dev != NULL )
    {
        screenPtr = wnd->dev->mem_addr;
        linelen = wnd->dev->width;
    }
    else
    {
        rect invalid;
        invalid.left = x1;
        invalid.right = x2;
        invalid.top = y1;
        invalid.bottom = y1 + 1;
        GdiInvalidateRect(&invalid);

        screenPtr = RalScreenGetBuffer();
        linelen = RalScreenGetWidth();
    }

    delta = x2 - x1 + 1;
    screenPtr += linelen * y1 + x1;
    for (i = 0; i < delta; i++)
        *screenPtr++ = wnd->fg_color;

    return;
}

static void PrvDrawVLine(WND wnd, int x1, int y1, int y2)
{
    int i, delta, temp, linelen;
    UNITCL *screenPtr;

    if ( (wnd->window_type == WND_PANEL) && (wnd->window_visible == WND_INVISIBLE) )
        return;

    if (y1 > y2)
    {
        y1++, y2++;		/* not including the last point */
        SWAP(y1, y2);
    }

    if ((y2 < MIN_Y) || (y1 > MAX_Y))
        return;

    if ((x1 < MIN_X) || (x1 > MAX_X))
        return;

    if (y1 < MIN_Y)
        y1 = MIN_Y;

    if (y2 > MAX_Y)
        y2 = MAX_Y;

    if ( wnd->dev != NULL )
    {
        screenPtr = wnd->dev->mem_addr;
        linelen = wnd->dev->width;
    }
    else
    {
        rect invalid;
        invalid.left = x1;
        invalid.right = x1 + 1;
        invalid.top = y1;
        invalid.bottom = y2;
        GdiInvalidateRect(&invalid);

        screenPtr = RalScreenGetBuffer();
        linelen = RalScreenGetWidth();
    }

    delta = y2 - y1 + 1;
    screenPtr += linelen * y1 + x1;
    for (i = 0; i < delta; i++)
    {
        *screenPtr = wnd->fg_color;
        screenPtr += linelen;
    }

    return;
}

static void PrvDrawLineBresenhamNC(WND wnd, int x0, int y0, int x1, int y1)
{
    UNITCL color;
    UNITCL * screen;
    int dy, dx;
    int stepx, stepy;
    int linelen;

    color = wnd->fg_color;
    dy = y1 - y0;
    dx = x1 - x0;

    if (dy < 0) { dy = -dy;  stepy = -1; } else { stepy = 1; }
    if (dx < 0) { dx = -dx;  stepx = -1; } else { stepx = 1; }
    dy <<= 1;                                                  // dy is now 2*dy
    dx <<= 1;                                                  // dx is now 2*dx

    if ( wnd->dev != NULL )
    {
        screen = wnd->dev->mem_addr;
        linelen = wnd->dev->width;
    }
    else
    {
        rect invalid;
        invalid.left = x0;
        invalid.right = x1;
        invalid.top = y0;
        invalid.bottom = y1;
        GdiInvalidateRect(&invalid);

        screen = RalScreenGetBuffer();
        linelen = RalScreenGetWidth();
    }

    screen += linelen * y0 + x0;

    if (dx > dy) {
        int fraction = dy - (dx >> 1);                         // same as 2*dy - dx
        while (x0 != x1) {
            *screen = color;
            if (fraction >= 0) {
                y0 += stepy;
                screen += stepy * linelen;
                fraction -= dx;                                // same as fraction -= 2*dx
            }
            x0 += stepx;
            screen += stepx;
            fraction += dy;                                    // same as fraction -= 2*dy
        }
    } else {
        int fraction = dx - (dy >> 1);
        while (y0 != y1) {
            *screen = color;
            if (fraction >= 0) {
                x0 += stepx;
                screen += stepx;
                fraction -= dy;
            }
            y0 += stepy;
            screen += stepy * linelen;
            fraction += dx;
        }
    }
}

static void PrvMergeRow( UNITCL * src_row_p, int width, int mode, int weight )
{
    UNITCL * next_row;
    int i;

    if ( weight == 0 )
        return;

    next_row = src_row_p + width;

    switch ( mode )
    {
    case BLACK_ON_WHITE :
        for ( i = 0; i < width; ++i )
            next_row[i] = (UNITCL)(src_row_p[i] & next_row[i]);
        break;

    case WHITE_ON_BLACK :
        for ( i = 0; i < width; ++i )
            next_row[i] = (UNITCL)(src_row_p[i] | next_row[i]);
        break;

    case COLOR_ON_COLOR :
        break;

    case HALF_TONE :
        for ( i = 0; i < width; ++i )
            next_row[i] = (UNITCL)(( src_row_p[i] * weight + next_row[i] ) / (weight + 1));
        break;

    default :
        //printf("###### WRONG MODE IN PrvMergeRow\n");
        break;
    }
}

static void PrvMergeCol( UNITCL * src_p, int mode, int weight )
{
    UNITCL * next_p;

    if ( weight == 0 )
        return;

    next_p = src_p + 1;

    switch ( mode )
    {
    case BLACK_ON_WHITE :
        *next_p = (UNITCL)(*next_p & *src_p);
        break;

    case WHITE_ON_BLACK :
        *next_p = (UNITCL)(*next_p | *src_p);
        break;

    case COLOR_ON_COLOR :
        break;

    case HALF_TONE :
        *next_p = (UNITCL)((*src_p * weight + *next_p) / (weight + 1));
        break;

    default :
        //printf("######## WRONG MODE IN PrvMergeCol\n");
        break;
    }
}
static void PrvFlushAll(WND wnd)
{
    TRACE(TR_RexyCore, TR_LEVEL2, ("GdiFlushAll() called\n"));

    if ( g_ScreenLockCount == 0 )
        RalScreenFlush();
}

/** Redraw specified area.
 *
 * @param wnd		Target window to redraw.
 * @param boundary	Rectangle area to redraw.
 *
 * @return void
 */
static void PrvFlushPart(WND wnd, rect *boundary)
{

    int width, height;
    rect rtFlushPart;

    if (boundary == NULL)
    {
        PrvFlushAll(wnd);
        return;
    }

    width = RalScreenGetWidth();
    height = RalScreenGetHeight();

    rtFlushPart.left = wnd->frame.left + boundary->left;
    rtFlushPart.right = wnd->frame.left + boundary->right;
    rtFlushPart.top = wnd->frame.top + boundary->top;
    rtFlushPart.bottom = wnd->frame.top + boundary->bottom;

    if (rtFlushPart.left < 0)
        rtFlushPart.left = 0;
    if (rtFlushPart.top < 0)
        rtFlushPart.top = 0;
    if (rtFlushPart.right > width -1 )
        rtFlushPart.right = width-1;
    if (rtFlushPart.bottom > height - 1 )
        rtFlushPart.bottom = height-1;

    if ( g_ScreenLockCount == 0 )
        RalScreenFlushPart(&rtFlushPart);
}



/** @} */ // End of GDI
