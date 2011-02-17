/**
 * Function to draw an image on the oFBis user interface.
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
//#include <ofbis.h>

#include "../header/layout.h"
#include "../header/info.h"

#include "../header/rexy_browser.h"
#include "rexy.h"
#include "rexy_const.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

#ifndef WIN32
	#define min(a,b) ((a)<(b)?(a):(b))
	#define max(a,b) ((a)>(b)?(a):(b))
#endif

extern WND hBrowserWnd;
extern unsigned int convert_rgb(int color);
extern FBline(FB *fb, int x1, int y1, int x2, int y2, int color);
extern FBhline(FB *fb, int x1, int x2, int y1, int color);

/**
 * Draw an image on the display. If the image could not be loaded for some 
 * reason, we only draw an empty box.
 *
 * @param partp A pointer to the layout part representing the image.
 * @param geometry The current geometry of the display, which describes how
 * @param geometry and where to render the image.
 *
 * @return non-zero value if an error occurred.
 */
int ofbis_draw_image(struct layout_part* partp,
		     struct layout_rectangle geometry,
		     struct layout_rectangle scroll)
{
  struct ofbis_information *info = 
    (struct ofbis_information *)ofbis_ui->ui_specific;
  FB *fb;
  uint32_t colour;
  int x_position, y_position;
  struct layout_rectangle cut;

  RxBITMAP bitmap;

  fb = info->ofbis_fb;

  ofbis_cut_rectangle(partp->geometry, geometry, scroll, &cut);

  x_position = partp->geometry.x_position - scroll.x_position +
    cut.x_position;
  y_position = partp->geometry.y_position - scroll.y_position +
    cut.y_position;

  bitmap.width = cut.width;
  bitmap.height = cut.height;
  bitmap.bTransparent = 0;

  bitmap.data = (unsigned short *)partp->data.graphics.data;

  if ( bitmap.data )
  {
	  GdiDrawBitmap(hBrowserWnd, &bitmap, cut.x_position, cut.y_position, \
		x_position + info->x_position, y_position + info->y_position, \
		cut.width, cut.height);
  }

  if(partp->data.graphics.data == NULL || partp->data.graphics.border) {
    if(partp->data.graphics.border_colour & 0x80000000)
      colour = convert_rgb(info->default_text_colour);
    else
      colour = convert_rgb(partp->data.graphics.border_colour);

    if(cut.y_position == 0) {
      FBhline(fb, 
	      x_position + info->x_position, 
	      x_position + info->x_position + cut.width - 1,
	      y_position + info->y_position,
	      colour);
    }

    if(cut.x_position == 0) {    
      FBline(fb, 
	     x_position + info->x_position, 
	     y_position + info->y_position, 
	     x_position + info->x_position,
	     y_position + info->y_position + cut.height - 1,
	     colour);
    }

    if(partp->geometry.height - cut.height == cut.y_position) {
      FBhline(fb, 
	      x_position + info->x_position, 
	      x_position + info->x_position + cut.width - 1,
	      y_position + info->y_position + cut.height - 1,
	      colour);
    }

    if(partp->geometry.width - cut.width == cut.x_position) {
      FBline(fb, 
	     x_position + info->x_position + cut.width - 1, 
	     y_position + info->y_position, 
	     x_position + info->x_position + cut.width - 1,
	     y_position + info->y_position + cut.height - 1,
	     colour);
    }
  }

 return 0;
}
