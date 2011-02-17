/**
 * Functions to generically deal with images. This exists only to
 * delegate the actual work to the appropriate image handlers.
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

#include "../header/protocol.h"
#include "../header/image.h"
#include "../header/jpeg.h"
#include "../header/pngs.h"
#include "../header/gif.h"
#include "../header/magick.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

/**
 * This will not read from the stream given to it, but will examine the
 * MIME content type string given to it, and pass the file descriptor for
 * the stream on to the correct image handler, which will read and convert
 * the image into the format that the user interface wants.
 *
 * @param fd The file descriptor for the image stream.
 * @param width The requested width of the image, or a negative value if the
 * @param width width is to be taken from the image.
 * @param height The requested height of the image, or a negative value if the
 * @param height height is to be taken from the image.
 *
 * @return an allocated struct containing the converted image and information
 * @return about it, or NULL if an error occurred.
 */
struct image_data *image_open(int fd, int width, int height)
{
  struct image_data *picture;
  struct protocol_http_headers *headers;
  int ret;

  /* Fetch the HTTP headers associated with this stream. */
  headers = protocol_get_headers(fd);
  if(headers == NULL) {
    fprintf(stderr, "Warning! No HTTP headers found!\n");
    return NULL;
  }

  picture = NULL;

  /* Here is a good time to check what kind of image we have, and call
   * the right image handler. Chances are that we do not have image
   * support. Not much we can do then.
   */
  ret = strcmp(headers->content_type_major, "image");

  if(!strcmp(headers->content_type_major, "image")) {

#ifdef HAVE_LIBJPEG
    if(!strcmp(headers->content_type_minor, "jpeg")) {
      picture = image_jpeg_read(fd, width, height);
      return picture;
    }
#endif /* HAVE_LIBJPEG */

#ifdef HAVE_LIBPNG
    if(!strcmp(headers->content_type_minor, "png")) {
      picture = image_png_read(fd, width, height);
      return picture;
    }
#endif /* HAVE_LIBPNG */

#if defined(HAVE_LIBGIF) || defined(HAVE_LIBUNGIF)
    if(!strcmp(headers->content_type_minor, "gif")) {
      picture = image_gif_read(fd, width, height);
      return picture;
    }
#endif /* defined(HAVE_LIBGIF) || defined(HAVE_LIBUNGIF) */

#ifdef HAVE_LIBMAGICK
    picture = image_magick_read(fd, width, height);
    return picture;
#endif /* HAVE_LIBMAGICK */

  } else {

#ifdef HAVE_LIBMAGICK
    picture = image_magick_read(fd, width, height);
#endif /* HAVE_LIBMAGICK */

  }

  return picture;
}

/**
 * Frees an image that has been allocated by one of the image handlers.
 *
 * @param picture A pointer to the image data to be freed.
 *
 * @return a non-zero value if an error occurred.
 */
int image_free(struct image_data *picture)
{
  if(picture) {
    if(picture->data) 
      free(picture->data);
    free(picture);
  }

  return 0;
}
