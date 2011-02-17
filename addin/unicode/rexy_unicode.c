
/** @file */

/**
 * @defgroup addin Addin Unicode
 * These APIs are associated with Unicode
 * @ingroup api
 * @{
 */

#include "rexy.h"
#include "rexy_unicode.h"

static int GdiPaintStringW(WND wnd, ruint16 x, ruint16 y, wchar_t *str, int len, int space);

static UNITCL g_fontImgW[240*30];

/**
* @brief		Draws a character string for unicode.
* @param[in]	wnd		Window to draw characters.
* @param[in]	string	Points to the character string to be drawn.
* @param[in]	length	Specifies the number of bytes in the string.
* @param[in]	x		x-coordinate of the starting point of the text.
* @param[in]	y		y-coordinate of the starting point of the text.
* @return		Always return 0.
* @remarks		This function draws at the specified location using current foreground color.
* @see			
* @if HISTORY
*				16-May-2006 madholic Initiated 
* @endif
*/
int GdiTextOutW(WND wnd, ruint16 x, ruint16 y, wchar_t * string, int length)
{
    ruint16 width, height;
    rect bkgnd;
	int space = GdiGetTextSpace();
	int style = GdiGetTextStyle();
	int align = GdiGetTextAlign();
	int extent = GdiGetTextExtent();

    if ( (wnd->window_type == WND_PANEL) && (wnd->window_visible == WND_INVISIBLE) )
        return 0;

    //AS_GetTextWidth(string, length, &width, &height);
    width = FntGetTextWidthExW(&FntGlobalContext, string, length, space);
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

    GdiPaintStringW(wnd, x, y, string, length, space);

    if ( style & BOLDFONT ) {
        GdiPaintStringW(wnd, (ruint16)(x+1), y, string, length, space);
    }

    if ( style & UNDERLINEFONT ) {
        GdiDrawLine(wnd, (ruint16)x, (ruint16)(y+height-1), (ruint16)(x+width), (ruint16)(y+height-1));
    }

    if ( style & OUTLINEFONT)
    {
        COLORVAL oldColor;
        oldColor = GdiSetFgColor( wnd, COLOR_WHITE );
        GdiPaintStringW(wnd, (ruint16)(x-1), (ruint16)(y), string, length, space);
        GdiPaintStringW(wnd, (ruint16)(x+1), (ruint16)(y), string, length, space);
        GdiPaintStringW(wnd, (ruint16)(x),	 (ruint16)(y-1), string, length, space);
        GdiPaintStringW(wnd, (ruint16)(x),	 (ruint16)(y+1), string, length, space);

        // draw original text
        GdiSetFgColor( wnd, COLOR_BLACK );
        GdiPaintStringW(wnd, x, y, string, length, space);
        GdiSetFgColor( wnd, oldColor );
    }

    //GdiFlushPart(wnd,&bkgnd);
    return width;
}

/**
* @brief		Paint a string(unicode).
* @param[in]	wnd		Window to paint a string.
* @param[in]	x		x coordinate of the string.
* @param[in]	y		y coordinate of the string.
* @param[in]	str		Points to the character string to be drawn.
* @param[in]	len		Specifies the number of bytes in the string.
* @param[in]	space	
* @return		void
* @remarks		
* @see			
* @if HISTORY
*				06-Jan-2006 madholic Initiated 
* @endif
*/
static int GdiPaintStringW(WND wnd, ruint16 x, ruint16 y, wchar_t *str, int len, int space)
{
    rect text_rect, src_clip_rect, dest_clip_rect;
    int height, width;
    UNITCL *dp, *sp, *ep, *buf_addr;
    int linelen_dest, linelen_src;
    int y2, dw;

    if ( (wnd->window_type == WND_PANEL) && (wnd->window_visible == WND_INVISIBLE) )
        return 0;

    x += wnd->frame.left;
    y += wnd->frame.top;

    width = FntGetTextWidthExW(&FntGlobalContext, str, len, space);
    height = FntGetTextHeight(&FntGlobalContext);

    text_rect.left = x;
    text_rect.top = y;
    text_rect.right = x + width -1;
    text_rect.bottom = y + height-1;

    if (GdiIntersectRect(&dest_clip_rect, &text_rect, &wnd->clip) < 0)
        return 0; // no intersect region...

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

    sp = FntGetStrImageW(&FntGlobalContext, src_clip_rect.left, src_clip_rect.top, width, height, str, len, space);
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

    //GdiFlushPart(wnd,&dest_clip_rect);
    return width;
}

/**	
 * @anchor		FntGetTextWidthExW
 * @brief		Get the width of specified character string with character-spaces.
 * @param[in]   cxt		Global font context.
 * @param[in]	pStr	Pointer to character string to get width.
 * @param[in]	len		Length of character string to get width.
 * @param[in]	space
 * @return		Returns the width of pStr when drawn in the screen.
 * @remarks		Get the width of specified character string with character-spaces.
 * @see			FntGetFontWidth, FntGetTextHeight, FntGetTextWidth
 * @if HISTORY
 *				04-Jan-2006	klenui Initiated
 * @endif
 */
int FntGetTextWidthExW(FntContextType* cxt, wchar_t * pStr, int len, int space)
{
    int width;
    wchar_t * pEnd;

    pEnd = pStr + len;
    width = 0;

    while (pStr < pEnd)
    {
        width += FntGetFontWidth(cxt, (wchar_t)*pStr++);
        width += space;
    }

    if ( width ) width -= space;

    return width;
}

/**	
 * @anchor		FntGetStrImage
 * @brief		Retrieves a character image of a string.
 * @param[in]	cxt		the pointer of font context
 * @param[in]	x		the start x position from which the string image will be drawn.
 * @param[in]	y		the start y position from which the string image will be drawn.
 * @param[in]	width	the width by which the string image will be drawn.
 * @param[in]	height	the height by which the string image will be drawn.
 * @param[in]	str		Pointer to the string that will be drawn.
 * @param[in]	len		the length of the str.
 * @param[in]	space	the inter-chracter space.
 * @return		the pointer of string image buffer
 * @remarks		Retrieves a character image of a string.
 * @see			
 * @if HISTORY
 *				04-Jan-2006	klenui Initiated
 * @endif
 */
UNITCL* FntGetStrImageW(FntContextType* cxt, int x, int y, int width, int height, wchar_t * str, int len, int space)
{
    int i, j, sx, ex, fw, sbit, ebit, len_bit, line_w;
    WORD code;
    int w_len, img_pad;
    wchar_t * strEnd = str + len;
    unsigned char * pImg, mask, smask, img;
    UNITCL * dstCrBuf = g_fontImgW, *dstCr;

    sx = 0;			// start x cord.
    ex = x + width;	// end x cord.

    while (str < strEnd && sx < ex)
    {
        code = (wchar_t)*str++;

        // fw is the width of the image of the 'code'
        fw = FntGetFontWidth(cxt, code);

        // part of the string is before the start of clipping rect. skip this.
        if (sx + fw < x) { // clipping...
            sx += fw;
            sx += space;
            continue;
        }

        // 'code' image starting point is clipped.
        if (sx < x) // half clip...
            sbit = (x - sx);
        else  // no clip
            sbit = 0;

        // 'code' image ending point is clipped.
        if (sx + fw > ex)
            ebit = ex - sx;	// clipping...
        else
            ebit = fw;

        // the actual width of the 'code' image
        len_bit = ebit - sbit;
        // for destination address calculation.
        line_w = width - len_bit - space;

        pImg = FntGetFontImage(cxt, code);
        if ( !pImg )
        {
            sx += fw;
            sx += space;
            dstCrBuf += len_bit;
            dstCrBuf += space;
            continue;
        }

        // padding offset. add img_pad to the source address to go to the next line.
        w_len = FntGetByteWidth(cxt);
        img_pad = (w_len - (ebit - 1) / 8 - 1);

        if (sbit > 7)
            smask = (unsigned char)( 0x80 >> (sbit % 8));
        else
            smask = (unsigned char)( 0x80 >> sbit);

        dstCr = dstCrBuf;
        pImg += w_len * y;
        for (i = 0; i < height; i++) {
            pImg += sbit / 8;
            mask = smask;
            img = *pImg++;
            for (j = sbit; j < ebit; j++, mask >>= 1) {
                if (mask == 0) {
                    mask = 0x80;
                    img = *pImg++;
                }
                if (img & mask)
                    *dstCr++ = COLOR_WHITE;
                else
                    *dstCr++ = COLOR_BLACK;
            }

            if ( sx + fw < ex )
            {	// we need spaces
                for ( j = 0 ; j < space ; j++ )
                {
                    *dstCr++ = COLOR_BLACK;
                }
            }
            else
            {	// no more spaces
                dstCr += space;
            }
            pImg += img_pad; // may be 0 or 1 in 12 pixel...
            dstCr += line_w;
        }
        sx += fw; // proceed
        sx += space;
        dstCrBuf += len_bit;
        dstCrBuf += space;
    }

    return g_fontImgW;
}

/** @} */ // End of GDI
