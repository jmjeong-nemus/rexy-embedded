/**
 * Function to render text on the oFBis user interface.
 *
 * @author Tomas Berndtsson <tomas@nocrew.org>
 */

/*
 * Copyright (C) 1999, Tomas Berndtsson <tomas@nocrew.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "../header/config.h"

#include <string.h>
//#include <ofbis.h>

#include "../header/layout.h"
#include "../header/info.h"

#include "../header/rexy_browser.h"
#include "rexy.h"
#include "rexy_const.h"

#include "application.h"
#include "../../addin/hangle/hangle.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

//#include <fm.h>
//#include <wc.h>
extern WND hBrowserWnd;
extern isUtf8;
extern GdiTextOutW(WND, unsigned short, unsigned short, unsigned short*, int);

#if 0
typedef struct fbfont
{
  unsigned char *data;
  int           width;
  int           height;
} FBFONT;

int  fontheight_8x8 = 8;
int  fontwidth_8x8 = 8;

int  fontheight_8x14 = 14;
int  fontwidth_8x14 = 8;

int  fontheight_8x16 = 16;
int  fontwidth_8x16 = 8;


void FBsetfont( FB *f, FBFONT *newfont )
{
	int FontID;

	switch( newfont->width )
	{
		case 8:
			FontID = FntIDNumber10;
			break;
		case 14:
			FontID = FntIDNumber16;
			break;
		case 16:
			FontID = FntIDAlpha;
			break;
	}

	FntSetFont(&FntGlobalContext, FontID);
}

void FBputchar(FB *fb, int x, int y, int fcolor, int bcolor, char ch)
{
	char string[2] = {0,};
	string[0] = ch;

	GdiSetFgColor(hBrowserWnd, fcolor);
	GdiSetBkColor(hBrowserWnd, bcolor);
	
	GdiTextOut(hBrowserWnd, string, 1, x, y);
}
#endif

unsigned int convert_rgb(int color)
{
	unsigned int r,g,b;
	r = ( color >> 19 ) &0x1F;
	g = ( color >> 10 ) & 0x3F;
	b = ( color >> 3 ) & 0x1F;

	return (r << 11 | g << 5 | b);
}

/**
 * Render a piece of text on the display.
 * Supports underlined style. 
 *
 * Comments: Currently, this only supports one kind of monospaced font,
 *           which is the 8 by 8 pixel default font in oFBis. It will
 *           hopefully at least support True Type fonts in the future.
 *
 * @param partp A pointer to the layout part representing the text.
 * @param geometry The current geometry of the display, describing what
 * @param geometry and where to render the text.
 *
 * @return non-zero value if an error occurred.
 */
int ofbis_draw_text(struct layout_part* partp,
		    struct layout_rectangle geometry,
		    struct layout_rectangle scroll)
{
	rect aRect;
	int i;
	unsigned short rgb;
	int x_position, y_position;
	unsigned char putchar[2] = {0,};
	unsigned int tmp1,tmp2;
	static FntID PrevFntID = 0;
	unsigned char utf8[4] = {0,};

	struct ofbis_information *info = 
		(struct ofbis_information *)ofbis_ui->ui_specific;
	
	aRect.left   = 0;
	aRect.right  = 0;
	aRect.top    = 0;
	aRect.bottom = 0;

	if(partp->data.text.text[0] == '\0')
		return 0;

	if(partp->data.text.style.colour & 0x80000000)
	{
		rgb = convert_rgb(info->default_text_colour);
	}
	else if(partp->data.text.style.colour & 0x40000000)
	{
		rgb = convert_rgb(info->default_link_colour);
	}
	else
	{
		rgb = convert_rgb(partp->data.text.style.colour);
	}

    x_position = partp->geometry.x_position - scroll.x_position + 
      info->x_position;
    y_position = partp->geometry.y_position - scroll.y_position +
      info->y_position;

	GdiSetBkColor ( hBrowserWnd, COLOR_TRANSPARENT );
	GdiSetFgColor (hBrowserWnd, rgb);

	PrevFntID = FntGetFont(&FntGlobalContext);

	for(i = 0 ; (unsigned int)i < strlen(partp->data.text.text) ; i++)
	{
		if (((unsigned char)partp->data.text.text[i]) == 0xfe)
		{
			if ( (((unsigned char)partp->data.text.text[i + 1]) & 0x80))
			{
				  if(partp->data.text.style.size < 12)
						FntSetFont(&FntGlobalContext, FntIDUnicode10SL);				  
				  else if(partp->data.text.style.size < 14)
					  FntSetFont(&FntGlobalContext, FntIDUnicode12SL);
				  else if(partp->data.text.style.size < 16)
					  FntSetFont(&FntGlobalContext, FntIDUnicode14SL);
				  else
					  FntSetFont(&FntGlobalContext, FntIDUnicode16SL);

				tmp2 = (unsigned char)partp->data.text.text[i + 1];
				tmp2 = tmp2 - 0x80;

			}
			else if ( (((unsigned char)partp->data.text.text[i + 1]) == '&')  ||
					  (((unsigned char)partp->data.text.text[i + 1]) == '"')  ||		 
					  (((unsigned char)partp->data.text.text[i + 1]) == '<')  ||
					  (((unsigned char)partp->data.text.text[i + 1]) == '>')  ||
					  (((unsigned char)partp->data.text.text[i + 1]) == '-') )
			{
				 if(partp->data.text.style.size < 12)
					FntSetFont(&FntGlobalContext, FntIDUnicode10BL);				  
				 else if(partp->data.text.style.size < 14)
				  FntSetFont(&FntGlobalContext, FntIDUnicode12BL);
				 else if(partp->data.text.style.size < 16)
				  FntSetFont(&FntGlobalContext, FntIDUnicode14BL);
				 else
				  FntSetFont(&FntGlobalContext, FntIDUnicode16BL);

				tmp2 = (unsigned char)partp->data.text.text[i + 1];
			}
			i++;
		}

		else if ( ! isUtf8 )
		{	
			if((unsigned char)partp->data.text.text[i] & 0x80)
			{
				  if(partp->data.text.style.size < 12)
						FntSetFont(&FntGlobalContext, FntIDUnicode10Han);				  
				  else if(partp->data.text.style.size < 14)
					  FntSetFont(&FntGlobalContext, FntIDUnicode12Han);
				  else if(partp->data.text.style.size < 16)
					  FntSetFont(&FntGlobalContext, FntIDUnicode14Han);
				  else
					  FntSetFont(&FntGlobalContext, FntIDUnicode16Han);

				
				tmp1 = (unsigned char)partp->data.text.text[i];
				tmp1 = tmp1<<8;
				i++;
				tmp1 = tmp1 | (unsigned char)partp->data.text.text[i];
				HanCodeConvert(tmp1, &tmp2,EUC,IW);
				tmp2 = tmp2 - 0xac00;
			}
			else
			{
				  if(partp->data.text.style.size < 12)
						FntSetFont(&FntGlobalContext, FntIDUnicode10BL);				  
				  else if(partp->data.text.style.size < 14)
					  FntSetFont(&FntGlobalContext, FntIDUnicode12BL);
				  else if(partp->data.text.style.size < 16)
					  FntSetFont(&FntGlobalContext, FntIDUnicode14BL);
				  else
					  FntSetFont(&FntGlobalContext, FntIDUnicode16BL);

				tmp2= (unsigned char)partp->data.text.text[i];
			}
		}
		else // utf8
		{
			if((unsigned char)partp->data.text.text[i] & 0x80) 
			{
				  if(partp->data.text.style.size < 12)
						FntSetFont(&FntGlobalContext, FntIDUnicode10Han);				  
				  else if(partp->data.text.style.size < 14)
					  FntSetFont(&FntGlobalContext, FntIDUnicode12Han);
				  else if(partp->data.text.style.size < 16)
					  FntSetFont(&FntGlobalContext, FntIDUnicode14Han);
				  else
					  FntSetFont(&FntGlobalContext, FntIDUnicode16Han);

				utf8[0] = (unsigned char)partp->data.text.text[i];
				utf8[1] = (unsigned char)partp->data.text.text[i+1];
				utf8[2] = (unsigned char)partp->data.text.text[i+2];
				UTF8toUCS2(&utf8[0],&tmp2);
				tmp2 = tmp2 - 0xac00;
				i += 2;
			}
			else
			{
				  if(partp->data.text.style.size < 12)
						FntSetFont(&FntGlobalContext, FntIDUnicode10BL);				  
				  else if(partp->data.text.style.size < 14)
					  FntSetFont(&FntGlobalContext, FntIDUnicode12BL);
				  else if(partp->data.text.style.size < 16)
					  FntSetFont(&FntGlobalContext, FntIDUnicode14BL);
				  else
					  FntSetFont(&FntGlobalContext, FntIDUnicode16BL);

				utf8[0] = (unsigned char)partp->data.text.text[i];
				UTF8toUCS2(&utf8[0],&tmp2);
			}
		}

		GdiTextOutW(hBrowserWnd,(unsigned short) x_position, (unsigned short)y_position, (unsigned short*)&tmp2, 1);

		if(partp->data.text.style.bold)
				GdiTextOutW(hBrowserWnd,(unsigned short)(x_position+1), (unsigned short)y_position, (unsigned short*)&tmp2, 1);

		x_position += FntGetFontWidth(&FntGlobalContext, (unsigned short)tmp2 );
	}
	
	if(partp->data.text.style.underlined) 
	{
		if ( !(((unsigned char)partp->data.text.text[0] == 0xfe )&& ((unsigned char)partp->data.text.text[1] == 0xa0)))
		{

			GdiDrawColorLine(hBrowserWnd,
					(unsigned short)(partp->geometry.x_position - scroll.x_position + info->x_position),
					(unsigned short)(partp->geometry.y_position - scroll.y_position + partp->data.text.style.size + info->y_position - 1),
					(unsigned short)(x_position - 1),	
					(unsigned short)(partp->geometry.y_position - scroll.y_position + partp->data.text.style.size + info->y_position - 1), 
					rgb);
		}
	}

	FntSetFont(&FntGlobalContext, PrevFntID);

  return 0;
}

