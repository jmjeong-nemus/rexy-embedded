/**
 * Parse the input as an image.
 * The functions here will build a structure of a single image part 
 * and pass it on to the layout part of Zen. 
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
//#include <unistd.h>

#include "../header/states.h"
#include "../header/image.h"
#include "../header/layout.h"
#include "../header/ui.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

/**
 * Take the input stream as an image and create a layout part containing
 * an image, as if it was taken from an img-tag in an HTML page.
 * 
 * @param fd File descriptor of input stream.
 *
 * @return non-zero value if an error occurred.
 */
int parse_image(int fd)
{
  struct layout_part *partp;
  struct image_data *picture;

  if(!user_interface.ui_support.image)
    return 1;

  /* Initialize the state struct used throughout the whole parsing. */
  parse_state_delete_all();
  parse_state_init();

  partp = layout_init_part(LAYOUT_PART_GRAPHICS);
  if(partp == NULL) {
    return 1;
  }

  picture = image_open(fd, -1, -1);
  if(picture == NULL)
    return 1;

  /* We should really allocate a new area, copy the image and free the
   * image data. This is too much work for us, so for the time being, we
   * will only free the image data struct, and not the data itself. This
   * will however be freed by the function which destroys the layout parts.
   * So, do not worry about memory leaks, I have it all under control.
   */
  partp->data.graphics.data = picture->data;
  partp->data.graphics.size = picture->size;
  partp->data.graphics.type = LAYOUT_PART_GRAPHICS_RAW;
  partp->data.graphics.alt_text = NULL;
  partp->geometry.width = picture->width;
  partp->geometry.height = picture->height;
  free(picture);
  layout_add_part(partp);

  /* Delete the states when we are done with them. */
  parse_state_delete_all();

  return 0;
}
