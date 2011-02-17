/**
 * Function to draw a horizontal line on the oFBis user interface.
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

//#include <ofbis.h>

#include "../header/layout.h"
#include "../header/info.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

#include "../header/rexy_browser.h"

extern WND hBrowserWnd;
extern unsigned int convert_rgb(int color);

/**
 * Draws a simple horizontal line as wide as specified.
 *
 * @param partp A pointer to the layout part representing the line.
 * @param geometry The current geometry of the display, describing what
 * @param geometry part and where to render the line.
 *
 * @return non-zero value if an error occurred.
 */
int ofbis_draw_line(struct layout_part* partp, 
		    struct layout_rectangle geometry,
		    struct layout_rectangle scroll)
{
	unsigned short colour1, colour2;
	struct layout_rectangle rect;
	struct ofbis_information *info = 
    (struct ofbis_information *)ofbis_ui->ui_specific;
	colour1 = convert_rgb(0x666666);
	colour2 = convert_rgb(0xeeeeee);

	rect.x_position = partp->geometry.x_position - scroll.x_position + 8 + info->x_position;
	rect.y_position = partp->geometry.y_position - scroll.y_position + (partp->geometry.height - partp->data.line.size) / 2 + info->y_position;
	rect.width = partp->geometry.width - 16;
	rect.height = partp->data.line.size;

	GdiDrawColorLine (hBrowserWnd, 
		(unsigned short)rect.x_position,
		(unsigned short)rect.y_position,
		(unsigned short)(rect.x_position + rect.width - 2),
		(unsigned short)rect.y_position,
		colour1);

	GdiDrawColorLine (hBrowserWnd, 
		(unsigned short)rect.x_position,
		(unsigned short)rect.y_position,
		(unsigned short)rect.x_position,
		(unsigned short)(rect.y_position + rect.height - 1),
		colour1);

	GdiDrawColorLine (hBrowserWnd, 
		(unsigned short)(rect.x_position + 1),
		(unsigned short)(rect.y_position + rect.height - 1),
		(unsigned short)(rect.x_position + rect.width - 2),
		(unsigned short)(rect.y_position + rect.height - 1),
		colour2);

	GdiDrawColorLine (hBrowserWnd, 
		(unsigned short)(rect.x_position + rect.width - 2),
		(unsigned short)(rect.y_position + 1),
		(unsigned short)(rect.x_position + rect.width - 2),
		(unsigned short)(rect.y_position + rect.height - 1),
		colour2);
#if 0
	struct ofbis_information *info = 
    (struct ofbis_information *)ofbis_ui->ui_specific;
  FB *fb;
  uint32_t colour1, colour2;
  struct layout_rectangle rect;

  fb = info->ofbis_fb;
  colour1 = FBc24_to_cnative(fb, 0x666666);
  colour2 = FBc24_to_cnative(fb, 0xeeeeee);

  /* Calculate the rectangle to represent the line. */
  rect.x_position = partp->geometry.x_position - scroll.x_position + 8 +
    info->x_position;
  rect.y_position = partp->geometry.y_position - scroll.y_position +
    (partp->geometry.height - partp->data.line.size) / 2 + 
    info->y_position;
  rect.width = partp->geometry.width - 16;
  rect.height = partp->data.line.size;

  FBhline(fb, 
	  rect.x_position,
	  rect.x_position + rect.width - 2,
	  rect.y_position,
	  colour1);

  FBline(fb, 
	 rect.x_position,
	 rect.y_position,
	 rect.x_position,
	 rect.y_position + rect.height - 1,
	 colour1);

  FBhline(fb, 
	  rect.x_position + 1,
	  rect.x_position + rect.width - 2,
	  rect.y_position + rect.height - 1,
	  colour2);

  FBline(fb, 
	 rect.x_position + rect.width - 2,
	 rect.y_position + 1,
	 rect.x_position + rect.width - 2,
	 rect.y_position + rect.height - 1,
	 colour2);
#endif
  return 0;
}

