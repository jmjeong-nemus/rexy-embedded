/**
 * Function to print the title of the page on the oFBis user interface.
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

#include <stdio.h>
#include <string.h>
//#include <ofbis.h>

#include "../header/info.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

#include "../header/rexy_browser.h"

extern WND hBrowserWnd;
extern unsigned int convert_rgb(int color);

/**
 * Display the title in the upper right corner of the screen.
 *
 * @return non-zero value if an error occurred.
 */
int ofbis_set_title(char *title_text)
{
	struct ofbis_information *info = 
		(struct ofbis_information *)ofbis_ui->ui_specific;

	unsigned short bkrgb, fgrgb;
	int x_position;

	if( !title_text )
		return 1;

	bkrgb = convert_rgb(info->control_field_colour);
	fgrgb = convert_rgb(info->control_field_text_colour);
	
	GdiSetBkColor ( hBrowserWnd, bkrgb );
	GdiSetFgColor (hBrowserWnd, fgrgb);

	x_position = ofbis_ui->ui_display.width - 8 * strlen(title_text);
	GdiTextOut( hBrowserWnd, title_text, strlen(title_text), (short)x_position, 0);

  return 0;
}

/**
 * Display the URL of the page in the upper left corner of the screen.
 *
 * @param url The URL of the page.
 * 
 * @return non-zero value if an error occurred.
 */
int ofbis_set_url(char *url)
{
  return 0;
}

/**
 * Display a text in the status area of the control field.
 *
 * @param text The status text to display.
 *
 * @return non-zero value if an error occurred.
 */
int ofbis_set_status_text(char *text)
{
  return 0;
}

