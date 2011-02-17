/*
* $Id: rexy_font.c 491 2005-02-22 00:26:34Z klenui $
* Copyright(c) 2000-6, Nemustech Inc, All Rights Reserved.
*
* This file is a part of Rexy Embedded 
*
*	Routines to process font data. These codes read data from font files and update font bitmap image.
*
*/

/** @file */

/**
 * @defgroup CoreFont Core Font Service
 * These APIs are associated with Font
 * @ingroup RexyECoreGroup
 * @{
 */


/*======================================================================*/
/* Header																*/
/*======================================================================*/
#include "rexy.h"

/*======================================================================*/
/* Module Variables														*/
/*======================================================================*/
FntContextType FntGlobalContext;
static UNITCL g_fontImg[240*30];
/*======================================================================*/
/* Function Definition													*/
/*======================================================================*/


/*======================================================================*/
/* Implementation														*/
/*======================================================================*/

/**	
 * @anchor		FntGetFontWidth
 * @brief		Return the width of font representing the specified code.
 * @param[in]   cxt		Global font context.
 * @param[in]	code    Code of font to get width. Four types of font width, 
 *		as in alphabet, hangul, symbol, and kanji is returned by code value.
 * 
 * @return		The width of font representing the specified code
 * @remarks		The width is described as pixel unit.
 * @see			FntGetTextHeight
 * @if HISTORY
 *				04-Jan-2006	klenui Initiated
 * @endif
 */
int FntGetFontWidth(FntContextType* cxt, WORD code)
{
    return RscGetFontWidth(cxt->fntID, code);
}


/**	
 * @anchor		FntGetTextHeight
 * @brief		Return the height of current font
 * @param[in]   cxt		Global font context.
 * @return		Returns the height of text.
 * @remarks		The hight is described as pixel unit.
 * @see			FntGetFontWidth
 * @if HISTORY
 *				04-Jan-2006	klenui Initiated
 * @endif
 */
int FntGetTextHeight(FntContextType* cxt)
{
    return RscGetFontHeight(cxt->fntID);
}


/**	
 * @anchor		FntGetByteWidth
 * @brief		Return number of bytes which needed to store a row in font data.
 * @param[in]   cxt		Global font context.
 * @return		byte number which needed to store a row in font data.
 * @remarks		You can find out how many bytes should be used to store row font image data using this API.
 * @see			FntGetFontWidth, FntGetTextHeight
 * @if HISTORY
 *				04-Jan-2006	klenui Initiated
 * @endif
 */
int FntGetByteWidth(FntContextType* cxt)
{
    return RscGetFontByteWidth(cxt->fntID);
}

/**	
 * @anchor		FntGetTextWidth
 * @brief		Get the width of specified character string.
 * @param[in]   cxt		Global font context.
 * @param[in]	pStr	Pointer to character string to get width.
 * @param[in]	len		Length of character string to get width.
 * @return		Returns the width of pStr when drawn in the screen.
 * @remarks		Get the width of specified character string.
 * @see			FntGetFontWidth, FntGetTextHeight
 * @if HISTORY
 *				04-Jan-2006	klenui Initiated
 * @endif
 */
int FntGetTextWidth(FntContextType* cxt, char * pStr, int len)
{
    return FntGetTextWidthEx( cxt, pStr, len, 0 );
}

/**	
 * @anchor		FntGetTextWidthEx
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
int FntGetTextWidthEx(FntContextType* cxt, char * pStr, int len, int space)
{
    int width;
    char * pEnd;

    pEnd = pStr + len;
    width = 0;

    while (pStr < pEnd)
    {
        width += FntGetFontWidth(cxt, (byte)*pStr++);
        width += space;
    }

    if ( width ) width -= space;

    return width;
}

/**	
 * @anchor		FntSetFont
 * @brief		Set the given font as current font.
 * @param[in]   cxt		Global font context.
 * @param[in]	font	Font to set.
 * @return		Returns previous font id.
 * @remarks		You must restore previous font after this function. refer to the following example\n
 *		[EXAMPLE]\n
 *		FntID oldFont;	\n
 *		\n
 *		oldFont = FntSetFont(RXFONT_JAPAN24_font);	// save previous font to oldFont variable\n
 *		\n
 *		// process work...\n
 *		\n
 *		FntSetFont(oldFont);	// restore previous font\n
 *		Get the width of specified character string with character-spaces.
 * @see			FntGetFont
 * @if HISTORY
 *				04-Jan-2006	klenui Initiated
 * @endif
 */
FntID FntSetFont(FntContextType* cxt, FntID font)
{
    FntID oldFnt;
    oldFnt = cxt->fntID;

    //ASSERT( cxt->fntID >= 0 && cxt->fntID < FntIDMaxNum );

    cxt->fntID = font;
    return oldFnt;
}


/**	
 * @anchor		FntGetFont
 * @brief		Return current selected font
 * @param[in]   cxt		Global font context.
 * @return		current selected font as FntID
 * @remarks		Do not access any member of FntContextType structure directly.
 *				It can be changed on later version without notice.
 * @see			FntSetFont
 * @if HISTORY
 *				04-Jan-2006	klenui	Initiated
 *				23-Aug-2006 Joshua	Modified comment
 * @endif
 */
FntID FntGetFont(FntContextType* cxt)
{
    //ASSERT( cxt->fntID >= 0 && cxt->fntID < FntIDMaxNum );
    return cxt->fntID;
}

/**	
 * @anchor		FntGetFontImage
 * @brief		Return raw font image
 * @param[in]   cxt		Global font context pointer
 * @param[in]   code	character code
 * @return		bitwise array
 * @remarks		You can get raw bitmap image data using this API. Note that the code can be multi byte code.
 * @see			FntSetFont, FntGetFont
 * @if HISTORY
 *				04-Jan-2006	klenui	Initiated
 *				23-Aug-2006 Joshua	Modified comment
 * @endif
 */
unsigned char * FntGetFontImage(FntContextType* cxt, WORD code)
{
    static ruint8 buffer[100];
    RscGetFontImage( cxt->fntID, buffer, sizeof(buffer), code);
    return buffer;
}

/**	
 * @anchor		FntGetTextLengthFittingInWidth
 * @brief		Return the length of specified character string fitting in given width.
 * @param[in]   cxt		Global font context.
 * @param[in]	pStr	Pointer to character string to get width.
 * @param[in]	len		Length of character string to get width.
 * @param[in]	width	Width of a region that character string is drawn on.
 * @return		Returns the length of pStr fitting in given width.
 * @remarks		Get font image
 * @see			FntGetTextWidth, FntGetTextWidthEx
 * @if HISTORY
 *				04-Jan-2006	klenui Initiated
 * @endif
 */
int FntGetTextLengthFittingInWidth(FntContextType* cxt, char *pStr, int len, int width)
{
    char*	t;
    char*	pEnd;
    int		codeWidth;

    pEnd = pStr + len;
    t = pStr;

    while (t < pEnd)
    {
        codeWidth = FntGetFontWidth(cxt, FntGetCharCode(t));
        if (codeWidth > width)
            break;

        width -= codeWidth;
        t = FntCharNext(t);
    }

    return (int)(t - pStr);
}


/**	
 * @anchor		FntIsDBCSLeadByte
 * @brief		Return the possiblity that given character can be a lead byte of DBCS.
 * @param[in]	LeadByte	Byte code of character
 * @return		If the given code byte can be a lead byte of DBCS, return TRUE. If else, return FALSE.
 * @remarks		Note that returning TRUE does not mean that it's surely the lead byte of DBCS.
 * @if HISTORY
 *				04-Jan-2006	klenui	Initiated
 *				23-Aug-2006	Johusa	Modified
 * @endif
 */
bool	FntIsDBCSLeadByte(byte LeadByte)
{
    return ( LeadByte > 0x80 );
}

/**	
 * @anchor		FntCharNext
 * @brief		Return the pointer of next character starting from the specified string pointer with care of DBCS.
 * @param[in]	lpsz	the pointer of string
 * @return		the pointer of the next char pointer
 * @see			FntIsDBCSLeadByte
 * @if HISTORY
 *				04-Jan-2006	klenui Initiated
 * @endif
 */
char*	FntCharNext(const char *lpsz)
{
    char*	ret = (char*)lpsz;

    if (ret[0] == '\0')
    {
        //reached at end.
        return ret;
    }

    if (FntIsDBCSLeadByte(ret[0]))
    {
        if (ret[1] == '\0')
        {
            //invalid char. in general, 2nd char in DBCS cannot be '\0'
            //for the compatibility with SBCS String Fuctions.
            return ret + 1;
        }
        else
        {
            //current char is DBCS.
            return ret + 2;
        }
    }
    else
    {
        //current char is SBCS.
        return ret + 1;
    }
}

/**	
 * @anchor		FntCharPrev
 * @brief		Return the pointer of previous character starting from the given string pointer in specified string with care of DBCS
 * @param[in]	lpszStart		Pointer to the beginning of the string. 
 * @param[in]	lpszCurrent		Pointer to a character in a null-terminated string. 
 * @return		the pointer of the previous char pointer.
 * @see			FntIsDBCSLeadByte
 * @if HISTORY
 *				04-Jan-2006	klenui Initiated
 * @endif
 */
char*	FntCharPrev(const char *lpszStart, const char *lpszCurrent)
{
    char*	ret;
    char*	next;

    ret = (char*)lpszStart;
    next = FntCharNext(ret);

    while (ret < next && next < lpszCurrent)
    {
        ret = next;
        next = FntCharNext(next);
    }

    return ret;
}

/**	
 * @anchor		FntGetCharCode
 * @brief		Return character code of the specified string pointer
 * @param[in]	lpsz	the pointer of a string
 * @return		Character code. 
 * @remarks		If the string pointer points double byte code character,
 *				it will be two byte integer.
 * @see			FntIsDBCSLeadByte
 * @if HISTORY
 *				04-Jan-2006	klenui	Initiated
 *				23-Aug-2006	Joshua	Modified comment
 * @endif
 */
WORD	FntGetCharCode(const char *lpsz)
{
    WORD	code;
    WORD	code2;

    code = (WORD)((WORD)lpsz[0] & 0xFF);
    if (FntIsDBCSLeadByte((byte)code))
    {
        code2 = (WORD)((WORD)lpsz[1] & 0x00FF);
        if (code2 == '\0')
            return code;

        code = (WORD)((code & 0x00FF) << 8 | code2);
    }

    return code;
}

/**	
 * @anchor		FntGetStrImage
 * @brief		Return the pointer of character image of a string.
 * @param[in]	cxt		the pointer of font context
 * @param[in]	x		the start x position from which the string image will be drawn.
 * @param[in]	y		the start y position from which the string image will be drawn.
 * @param[in]	width	the width by which the string image will be drawn.
 * @param[in]	height	the height by which the string image will be drawn.
 * @param[in]	str		Pointer to the string that will be drawn.
 * @param[in]	len		the length of the str.
 * @param[in]	space	the inter-chracter space.
 * @return		the pointer of string image buffer
 * @see			FntGetFontImage
 * @if HISTORY
 *				04-Jan-2006	klenui Initiated
 * @endif
 */
UNITCL* FntGetStrImage(FntContextType* cxt, int x, int y, int width, int height, char * str, int len, int space)
{
    int i, j, sx, ex, fw, sbit, ebit, len_bit, line_w;
    WORD code;
    int w_len, img_pad;
    char * strEnd = str + len;
    unsigned char * pImg, mask, smask, img;
    UNITCL * dstCrBuf = g_fontImg, *dstCr;

    sx = 0;			// start x cord.
    ex = x + width;	// end x cord.

    while (str < strEnd && sx < ex)
    {
        code = (unsigned char )*str++;

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

    return g_fontImg;
}

/** @} */ // End of Font

