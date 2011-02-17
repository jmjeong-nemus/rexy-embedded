/**
 * Function to draw a table on the oFBis user interface.
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


/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

#include "../header/rexy_browser.h"

#ifndef WIN32
	#define min(a,b) ((a)<(b)?(a):(b))
	#define max(a,b) ((a)>(b)?(a):(b))
#endif

extern WND hBrowserWnd;
extern unsigned int convert_rgb(int color);

FBline(FB *fb, int x1, int y1, int x2, int y2, int color)
{
	GdiDrawColorLine( hBrowserWnd, (unsigned short)x1, (unsigned short)y1, (unsigned short)x2, (unsigned short)y2, (unsigned short)color);
}

FBhline(FB *fb, int x1, int x2, int y1, int color)
{
	GdiDrawColorLine( hBrowserWnd, (unsigned short)x1, (unsigned short)y1, (unsigned short)x2, (unsigned short)y1, (unsigned short)color);
}

int FBc24_to_cnative(FB fb, int color)
{
	// dummy
	return color;
}

/**
 * Draws a table, or a table cell, and recursively render the contents
 * of the table cells. 
 *
 * @param partp A pointer to the layout part representing the line.
 * @param geometry The current geometry of the display, describing what
 * @param geometry part and where to render the line.
 *
 * @return non-zero value if an error occurred.
 */
int ofbis_draw_table(struct layout_part* partp, 
		     struct layout_rectangle geometry,
		     struct layout_rectangle scroll)
{

  struct ofbis_information *info = 
    (struct ofbis_information *)ofbis_ui->ui_specific;
  FB *fb;
  uint32_t colour1, colour2;
  struct layout_rectangle rect, cut;
  struct layout_part *rowp, *cellp;
  int border = 0;

  fb = info->ofbis_fb;

  ofbis_cut_rectangle(partp->geometry, geometry, scroll, &rect);

  cut = rect;

  rect.x_position += partp->geometry.x_position - scroll.x_position +
    + info->x_position;
  rect.y_position += partp->geometry.y_position - scroll.y_position +
    + info->y_position;

  /* If the table has a background colour worth mentioning, we fill the area
   * with that colour here. Also select colour for the border.
   */
  if(partp->type == LAYOUT_PART_TABLE) {
    colour1 = convert_rgb(0xffffff);
    colour2 = convert_rgb(0x000000);
    border = partp->data.table.border;
  } else {
    colour1 = convert_rgb(0x000000);
    colour2 = convert_rgb(0xffffff);
    if(!(partp->data.table_cell.background_colour & 0x80000000))
      ofbis_fill_area(rect, partp->data.table_cell.background_colour);

    if(partp->parent && partp->parent->parent &&
       partp->parent->parent->type == LAYOUT_PART_TABLE)
      border = partp->parent->parent->data.table.border;
  }

  if(border > 0) {
    
    if(cut.y_position == 0) {
      FBhline(fb, 
	      rect.x_position,
	      rect.x_position + rect.width - 2,
	      rect.y_position,
	      colour1);
    }

    if(cut.x_position == 0) {    
      FBline(fb, 
	     rect.x_position,
	     rect.y_position,
	     rect.x_position,
	     rect.y_position + rect.height - 1,
	     colour1);
    }

    if(partp->geometry.height - cut.height == cut.y_position) {
      FBhline(fb, 
	      rect.x_position + 1,
	      rect.x_position + rect.width - 2,
	      rect.y_position + rect.height - 1,
	      colour2);
    }

    if(partp->geometry.width - cut.width == cut.x_position) {
      FBline(fb, 
	     rect.x_position + rect.width - 2,
	     rect.y_position + 1,
	     rect.x_position + rect.width - 2,
	     rect.y_position + rect.height - 1,
	     colour2);
    }
  }

  if(partp->type == LAYOUT_PART_TABLE) {
    rowp = partp->child;
    while(rowp) {
      if(rowp->type != LAYOUT_PART_TABLE_ROW) {
	rowp = rowp->next;
	continue;
      }

      cellp = rowp->child;
      while(cellp) {
	if(cellp->type != LAYOUT_PART_TABLE_CELL) {
	  cellp = cellp->next;
	  continue;
	}

	if(!(cellp->geometry.x_position - scroll.x_position > 
	     geometry.x_position + geometry.width ||
	     cellp->geometry.y_position - scroll.y_position > 
	     geometry.y_position + geometry.height ||
	     cellp->geometry.x_position - scroll.x_position + 
	     cellp->geometry.width < geometry.x_position ||
	     cellp->geometry.y_position - scroll.y_position + 
	     cellp->geometry.height < geometry.y_position)) {
	  struct layout_rectangle cell_geometry;

	  /* Draw the frame around the cell. */
	  ofbis_draw_table(cellp, geometry, scroll);

	  cell_geometry.x_position = cellp->geometry.x_position - 
	    scroll.x_position;
	  cell_geometry.y_position = cellp->geometry.y_position - 
	    scroll.y_position;
	  cell_geometry.width = 
	    min(cellp->geometry.width, 
		geometry.width + geometry.x_position - 
		(cellp->geometry.x_position - scroll.x_position));
	  cell_geometry.height = 
	    min(cellp->geometry.height, 
		geometry.height + geometry.y_position - 
		(cellp->geometry.y_position - scroll.y_position));
	  if(cell_geometry.x_position < 0) {
	    cell_geometry.width -= -cell_geometry.x_position;
	    cell_geometry.x_position = 0;
	  }
	  if(cell_geometry.y_position < 0) {
	    cell_geometry.height -= -cell_geometry.y_position;
	    cell_geometry.y_position = 0;
	  }

	  /* Render the parts placed inside the table cell. */
	  if(cellp->child != NULL) {
	    ofbis_render(cellp, cell_geometry, scroll);
	  }
	}

	cellp = cellp->next;
      }

      rowp = rowp->next;
    }
  }
  return 0;
}

