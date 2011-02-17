/**
 * Helper functions when dealing with images. This includes functions
 * that helps converting images into the correct format for the current
 * display. 
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

#include <stdlib.h>

#include "../header/ui.h"
#include "../header/image.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

/**
 * Find the colour index corresponding to the given colours.
 * This does not try to find the closest match, but rather the exact
 * match, because the image is prepared to match the colourmap used.
 *
 * @param red The red component of the colour to find.
 * @param green The green component of the colour to find.
 * @param blue The blue component of the colour to find.
 *
 * @return the colour index matching the given colour, or a negative value
 * @return if the colour could not be found, which should not happen.
 */
static /*inline*/ int find_colour_index(int red, int green, int blue)
{
  int i, nr_of_colours;
  int diff, best_diff, best_match;
  int red_diff, green_diff, blue_diff;

  nr_of_colours = 1 << user_interface.ui_display.bit_depth;

  best_diff = 3 * 255 * 255;
  best_match = nr_of_colours - 1;
  for(i = 0 ; i < nr_of_colours ; i++) {
    red_diff   = red   - user_interface.ui_display.colourmap[3 * i + 0];
    green_diff = green - user_interface.ui_display.colourmap[3 * i + 1];
    blue_diff  = blue  - user_interface.ui_display.colourmap[3 * i + 2];
    /*
    diff = red_diff * red_diff + 
      green_diff * green_diff + 
      blue_diff * blue_diff;
      */
    diff = abs(red_diff) + abs(green_diff) + abs(blue_diff);
    if(diff == 0)
      return i;
    if(diff < best_diff) {
      best_diff = diff;
      best_match = i;
    }
  }

  return best_match;
}

/**
 * Convert one colour into the best matching colour for the current
 * bit depth used.
 *
 * This is declared as an inline function to improve performance.
 *
 * @param datap A pointer to the place where the new colour will be
 * @param datap stored.
 * @param red The red element of the colour.
 * @param green The green element of the colour.
 * @param blue The blue element of the colour.
 *
 * @return the number of bytes to step ahead to get to the next pixel.
 */
int image_get_real_colour(unsigned char *datap, unsigned char red, 
			  unsigned char green, unsigned char blue)
{
  uint32_t colour;
#ifdef WORDS_BIGENDIAN
  int native_big_endian=1;
#else
  int native_big_endian=0;
#endif

  if(user_interface.ui_display.colourmap) {
    /* For palette based modes, we try to find the best matching
     * colour index.
     */
    datap[0] = find_colour_index(red, green, blue);
    return 1;
  } else {
    /* The display appears to be truecoloured. */
    colour = ((((red   >> (8 - user_interface.ui_display.red_length))   & 
		((1 << user_interface.ui_display.red_length)   - 1)) << 
	       user_interface.ui_display.red_offset) |
	      (((green >> (8 - user_interface.ui_display.green_length)) & 
		((1 << user_interface.ui_display.green_length) - 1)) << 
	       user_interface.ui_display.green_offset) |
	      (((blue  >> (8 - user_interface.ui_display.blue_length))  & 
		((1 << user_interface.ui_display.blue_length)  - 1)) << 
	       user_interface.ui_display.blue_offset));
    /* We have to consider both big and little endian. */
    if(user_interface.ui_display.endian == ZEN_UI_DISPLAY_BIG_ENDIAN || 
       (user_interface.ui_display.endian == ZEN_UI_DISPLAY_NATIVE_ENDIAN &&
	native_big_endian)) {
      switch(user_interface.ui_display.bit_depth) {
      case 15:
      case 16:
	datap[0] = (colour >> 8) & 0xff;
	datap[1] = (colour >> 0) & 0xff;
	return 2;
	break;
      case 24:
	datap[0] = (colour >> 16) & 0xff;
	datap[1] = (colour >>  8) & 0xff;
	datap[2] = (colour >>  0) & 0xff;
	return 3;
	break;
      case 32:
	datap[0] = 0;
	datap[1] = (colour >> 16) & 0xff;
	datap[2] = (colour >>  8) & 0xff;
	datap[3] = (colour >>  0) & 0xff;
	return 4;
	break;
      default:
	break;
      }
    } else {
      switch(user_interface.ui_display.bit_depth) {
      case 15:
      case 16:
	datap[0] = (colour >>  0) & 0xff;
	datap[1] = (colour >>  8) & 0xff;
	return 2;
	break;
      case 24:
	datap[0] = (colour >>  0) & 0xff;
	datap[1] = (colour >>  8) & 0xff;
	datap[2] = (colour >> 16) & 0xff;
	return 3;
	break;
      case 32:
	datap[0] = (colour >>  0) & 0xff;
	datap[1] = (colour >>  8) & 0xff;
	datap[2] = (colour >> 16) & 0xff;
	datap[3] = 0;
	return 4;
	break;
      default:
	break;
      }
    }
  }

  return 0;
}

