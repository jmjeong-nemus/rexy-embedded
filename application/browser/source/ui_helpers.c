/**
 * Functions to help the oFBis user interface on its course into perfection.
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
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32_WCE
	#include <sys/types.h>
#endif

//#include <ofbis.h>

#include "rexy.h"
#include "../header/info.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

#ifndef WIN32
	#define min(a,b) ((a)<(b)?(a):(b))
	#define max(a,b) ((a)>(b)?(a):(b))
#endif

extern isUtf8;
extern unsigned int convert_rgb(int color);

/**
 * Sets one colour in the palette of the display, if it is
 * palette based.
 *
 * @param index The index of the colour to set.
 * @param colour The 24-bit value of new colour.
 *
 * @return non-zero value if an error occurred.
 */
int ofbis_set_colour(int index, uint32_t colour)
{
  return 0;  
}

/**
 * Set the palette of the display, if it is palette based.
 *
 * @param red An array of colours to fill the red component.
 * @param green An array of colours to fill the green component.
 * @param blue An array of colours to fill the blue component.
 *
 * @return non-zero value if an error occurred.
 */
int ofbis_set_palette(int *red, int *green, int *blue)
{
  return 0;
}

/**
 * Set the width and height of a part, which only the user interface
 * can know the width and height of. This includes text strings and
 * various widgets in forms.
 *
 * @param partp A pointer to the layout part to set the size of.
 *
 * @return non-zero value if an error occurred.
 */
int ofbis_set_size(struct layout_part *partp)
{
  switch(partp->type) {
  case LAYOUT_PART_TEXT:
	if ( isUtf8 )
		partp->geometry.width = ((partp->data.text.style.size / 2) * 
								 (strlen(partp->data.text.text) - partp->data.text.extra_char_cnts - partp->data.text.special_char_cnts) );
	else
		partp->geometry.width = ((partp->data.text.style.size / 2) * 
								 (strlen(partp->data.text.text)- partp->data.text.special_char_cnts));

	partp->geometry.height = partp->data.text.style.size;

	break;

  case LAYOUT_PART_FORM_SUBMIT:
    /* Just use the default fontsize here. It might be better to look
     * at the nearest text part, but then we'd have to search for it in
     * some good way. This will do for now. 
     */

    partp->geometry.width = 8 * strlen(partp->data.form_submit.value) + 2 * 7;
    partp->geometry.height = ofbis_ui->ui_settings.default_fontsize + 2 * 3;
    break;

  default:
    break;
  }

  return 0;
}

/**
 * Fills an area of the screen with a specific colour.
 *
 * @param geometry A rectangle specifying the area to fill.
 * @param colour The 24-bit value of colour to fill with.
 *
 * @return non-zero value if an error occurred.
 */

extern WND hBrowserWnd;

int ofbis_fill_area(struct layout_rectangle geometry, uint32_t fill_colour)
{
	rect aRect;
	unsigned short rgb;
	
	aRect.left = 0;
	aRect.right = 240;
	aRect.top = 0;
	aRect.bottom = 320;

	rgb = convert_rgb(fill_colour);
	GdiFillRect (hBrowserWnd, &aRect, rgb);

  return 0;  
}

/**
 * Clip a part's rectangle geometry against the display geometry.
 *
 * @param part The geometry of the part.
 * @param geometry The geometry of the display.
 * @param scroll The current scroll offset.
 * @param cut A pointer to a geometry rectangle which will contain the
 * @param cut clipped rectangle, always relative to the original part.
 *
 * @return non-zero value if an error occurred.
 */
int ofbis_cut_rectangle(struct layout_rectangle part, 
			struct layout_rectangle geometry,
			struct layout_rectangle scroll,
			struct layout_rectangle *cut)
{
  int start_x, start_y, end_x, end_y;

  part.x_position -= scroll.x_position;
  part.y_position -= scroll.y_position;

  start_x = part.x_position - geometry.x_position;
  start_y = part.y_position - geometry.y_position;
  cut->x_position = max(0, -start_x);
  cut->y_position = max(0, -start_y);

  start_x = max(part.x_position, geometry.x_position);
  start_y = max(part.y_position, geometry.y_position);

  end_x = min(geometry.x_position + geometry.width - 1, 
	      part.x_position + part.width - 1);
  end_y = min(geometry.y_position + geometry.height - 1, 
	      part.y_position + part.height - 1);

  cut->width = end_x - start_x + 1;
  cut->height = end_y - start_y + 1;

  return 0;
}
