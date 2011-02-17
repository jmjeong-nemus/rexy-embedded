/**
 * Functions to take care of images with the help of libMagick, the
 * magickle image library. This reads images and converts them into
 * the proper format for the user interface to use.
 * This is made to work with both version 4.x and 5.x of libMagick.
 * Not only that, the API for libMagick is slightly changed in 5.2.x,
 * so we have to support that too now. Will these people never decide
 * on how to do things?
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

/* This whole file is only compiled if we have libMagick. */
#ifdef HAVE_LIBMAGICK

#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

/* Some versions of libMagick are stupid enough to include its
 * config.h file, which will give compilation warnings/errors
 * because of define clashes.
 */
#undef HAVE_CONFIG_H

#ifdef HAVE_MAGICK_API_H
#include <magick/api.h>
#endif /* HAVE_MAGICK_API_H */
#ifdef HAVE_MAGICK_MAGICK_H
#include <magick/magick.h>
#endif /* HAVE_MAGICK_MAGICK_H */
#ifdef HAVE_MAGICK_H
#include <magick.h>
#endif /* HAVE_MAGICK_H */

#include "../header/ui.h"
#include "../header/image.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

/* #define DEBUG_PRINT_TIME */

/**
 * Since libmagick, so far, is silly enough to runlength encode everything
 * we need to decode it here, to get the proper image data. This is said to
 * be changed in future versions of libmagick, starting from the upcoming
 * version 5.0. Then this will change. Or rather, both will be supported.
 * This is now done, and more. 5.0 is different from 5.2. Well, I hope this
 * should now support all available versions, but I can never be sure.
 *
 * @param picture The image data where we are going to store the real image.
 * @param image A pointer to the image struct as provided by libmagick.
 *
 * @return non-zero value if an error occurred.
 */
static int decode_image(struct image_data *picture, Image *image)
{
#if MagickLibVersion < 0x0500
  unsigned char *datap = picture->data;
  RunlengthPacket *rle_data = image->pixels;
  int x, y, i;
  unsigned char red, green, blue;

  i = 0;
  red = rle_data->red;
  green = rle_data->green;
  blue = rle_data->blue;
  for(y = 0 ; y < picture->height ; y++) {
    for(x = 0 ; x < picture->width ; x++) {
      i--;
      if(i < 0) {
	red = rle_data->red;
	green = rle_data->green;
	blue = rle_data->blue;
	i = rle_data->length;
	rle_data++;
      }
      datap += image_get_real_colour(datap, red, green, blue);      
    }
  }
#elif MagickLibVersion < 0x0520
  unsigned char *datap = picture->data;
  PixelPacket *pixel_data;
  int x, y;

  pixel_data = image->pixels;
  if(pixel_data == NULL)
    return 1;

  for(y = 0 ; y < picture->height ; y++) {
    for(x = 0 ; x < picture->width ; x++) {
      datap += image_get_real_colour(datap, 
				     pixel_data->red,
				     pixel_data->green,
				     pixel_data->blue);
      pixel_data++;
    }
  }
#else /* MagickLibVersion >= 0x0520 */
  unsigned char *datap = picture->data;
  PixelPacket *pixel_data;
  int x, y;
  
  pixel_data = GetImagePixels(image, 0, 0, picture->width, picture->height);
  if(pixel_data == NULL)
    return 1;

  for(y = 0 ; y < picture->height ; y++) {
    for(x = 0 ; x < picture->width ; x++) {
      datap += image_get_real_colour(datap, 
				     pixel_data->red,
				     pixel_data->green,
				     pixel_data->blue);
      pixel_data++;
    }
  }

  DestroyImagePixels(image);
#endif /* MagickLibVersion */

  return 0;
}

/**
 * Reads an image from a stream. Earlier functions have decided that 
 * this is the proper function to use for this stream. Therefore, we
 * will not lay much weight on excessive error checking when it comes
 * to the possibility of wrong image types. 
 * The end product is converted into an appropriate format for the
 * user interface. 
 *
 * @param fd The file descriptor of the stream to read the image data from.
 * @param width The requested width of the image, or a negative value if the
 * @param width width is to be taken from the image.
 * @param height The requested height of the image, or a negative value if the
 * @param height height is to be taken from the image.
 *
 * @return an allocated struct with the converted image and information 
 * @return about it, or NULL if an error occurred.
 */
struct image_data *image_magick_read(brw_FILE fd, int width, int height)
{
  FILE *fp;
  int new_fd;
  struct image_data *picture;
  Image *image, *scaled_image;
  ImageInfo image_info;
  char *status;
  char *buffer;
  char tmp_filename[64];
  int bytes, left, written;
  size_t total;
#ifdef DEBUG_PRINT_TIME
  time_t timer;
#endif
#if MagickLibVersion >= 0x0520
  ExceptionInfo exception_info;
#endif /* MagickLibVersion */

  status = (char *)malloc(1024);

  buffer = (char *)malloc(16384);
  if(buffer == NULL) {
    if(status != NULL)
      free(status);
    return NULL;
  }

  /* Unfortunately, libMagick cannot read from a stream directly, so 
   * we have to read the image into a temporary file, and open that
   * with the libMagick functions. 
   * mkstemp() will alter tmp_filename to contain the generated 
   * unique filename, so we can keep on using it throughout the reat
   * of this function. At the end, or on error, it is removed.
   */
  strcpy(tmp_filename, "/tmp/zenXXXXXX");
  new_fd = mkstemp(tmp_filename);
#if defined _WIN32_WCE || WIN32
  if(fd == -1) {
#else
  if(fd < 0) {
#endif
    free(buffer);
    if(status != NULL)
      free(status);
    return NULL;
  }

  total = 0;
  while((bytes = read(fd, buffer, 16384)) > 0) {
    total += bytes;
    if(status != NULL) {
      sprintf(status, "Read %d bytes of image...", total);
      ui_functions_set_status(status);
    }

    left = bytes;
    while(left) {
      written = RalWriteFile(new_fd, buffer, left);
      left -= written;
      if(written < 0)
	left = 0;
    }
  }
  RalCloseFile(new_fd);
  fp = NULL;

  free(buffer);

  if(status != NULL) {
    sprintf(status, "Processing image...");
    ui_functions_set_status(status);
  }

  picture = (struct image_data *)malloc(sizeof(struct image_data));
  if(picture == NULL) {
    fclose(fp);
    unlink(tmp_filename);
    return NULL;
  }

  /* Now it is finally time to let libmagick do its work. */
  GetImageInfo(&image_info);
  strcpy(image_info.filename, tmp_filename);
  image_info.compression = NoCompression; 
  image_info.colorspace = TransparentColorspace;
  /*  image_info.verbose = 1; */

#if MagickLibVersion < 0x0520
  image = ReadImage(&image_info);
#else /* MagickLibVersion >= 0x0520 */
  GetExceptionInfo(&exception_info);
  image = ReadImage(&image_info, &exception_info);
#endif /* MagickLibVersion */

  if(image == NULL) {
    /* ReadImage() closes the stream if an error occurs, so we must not do 
     * that here.
     */
    free(picture);
    unlink(tmp_filename);
    return NULL;
  }

  /* At this place, we look at the width and height parameters to
   * this function, and if they differ from the width and height from the
   * recently read image, then we need to scale that image. Unless the given
   * width and height are negative, of course.
   *
   * If libMagick failed to scale the image, we use the non-scaled image.
   */

  if(width < 0)
    picture->width = image->columns;
  else
    picture->width = width;
  if(height < 0)
    picture->height = image->rows;
  else
    picture->height = height;

  if(picture->width != image->columns || picture->height != image->rows) {
#if MagickLibVersion < 0x0520
    scaled_image = ScaleImage(image, picture->width, picture->height);
#else /* MagickLibVersion >= 0x0520 */
    scaled_image = ScaleImage(image, picture->width, picture->height, 
			      &exception_info);
#endif /* MagickLibVersion */
    if(scaled_image) {
      DestroyImage(image);
      image = scaled_image;
    }
  }

  /* Allocate space for the image data in our struct. */
  picture->size = (picture->width * picture->height *
		   user_interface.ui_display.bit_depth) / 8;
  picture->data = (unsigned char *)malloc(picture->size);
  if(picture->data == NULL) {
    free(picture);
    DestroyImage(image);
    if(status != NULL)
      free(status);
    unlink(tmp_filename);
    return NULL;
  }

#ifdef DEBUG_PRINT_TIME
  timer = time(NULL);
  fprintf(stderr, "%d: %s", __LINE__, ctime(&timer));
#endif

  /* Decode the image, so that we can see what it is. */
  if(decode_image(picture, image) != 0) {
    free(picture->data);
    free(picture);
    picture = NULL;
  }

#ifdef DEBUG_PRINT_TIME
  timer = time(NULL);
  fprintf(stderr, "%d: %s", __LINE__, ctime(&timer));
#endif

  DestroyImage(image);
  if(status != NULL)
    free(status);
  unlink(tmp_filename);

  return picture;
}

#endif /* HAVE_LIBMAGICK */

