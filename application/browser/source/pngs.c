/**
 * Functions that will decompress a PNG image into a nice enough
 * format. This is using libpng instead of libmagick for PNG images,
 * because it is faster, smaller, and hopefully gives a better result
 * than libMagick could ever give us.
 * If you think comments, code structure and more in this file looks
 * familiar, it might be because I have just copied the JPEG decoding
 * code and changed it for PNG. But it is not the same code at all.
 * Look closer.
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

/* This whole file is only compiled if we have libpng. */
#ifdef HAVE_LIBPNG

#include <stdio.h>
#include <stdlib.h>

//#include <unistd.h>
#ifndef _WIN32_WCE
	#include <io.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <fcntl.h>
#endif

#include "../header/pngs.h"
#include "../../../lib/lpng/png.h"
#include <setjmp.h>

#include "../header/ui.h"
#include "../header/image.h"
#include "../header/settings.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

//_fmode = _O_BINARY;

extern int RalCloseFile(int);

static PNG_CONST char *filename = "D:/index/pngtest.png";
/**
 *
 */
static void create_colourmap(png_color *colourmap, int number_of_colours)
{

  int colour;

  for(colour = 0 ; colour < number_of_colours ; colour ++) {
    colourmap[colour].red = 
      user_interface.ui_display.colourmap[3 * colour + 0];
    colourmap[colour].green = 
      user_interface.ui_display.colourmap[3 * colour + 1];
    colourmap[colour].blue = 
      user_interface.ui_display.colourmap[3 * colour + 2];
  }

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
struct image_data *image_png_read(int fd, int width, int height)
{
  png_structp pngp;
  png_infop pnginfop, pngendp;
  png_uint_32 image_width, image_height;
  int bit_depth, colour_type, interlace_type;
  int number_of_passes;
//  char *setting;
  double screen_gamma, gamma;
  int step, i, line_in, row_stride, x, number_of_colours;
  png_color *colourmap;
  png_bytep buffer;
  struct image_data *picture;
  unsigned char *picturep;
  char *status;
  int new_fd;
  FILE *fp;

  /* First duplicate the file descriptor, so that we can close it in the
   * correct way, without interfering with the method libpng has chosen
   * to operate in.
   */
#ifndef _WIN32_WCE
  new_fd = _dup(fd);
#endif

#if defined (_WIN32_WCE) || defined (WIN32)
  if(fd == -1) {
#else
  if(fd < 0) {
#endif
    /* If there is an error here, we could theoretically proceed with some
     * special treatment of the streams, but it indicates that something is
     * more wrong that we want it to be, so we just return an error here. 
     */
    return NULL;
  }
#ifndef _WIN32_WCE
  fp = _fdopen(new_fd, "r");

  if(fp == NULL) {
    RalCloseFile(new_fd);
    return NULL;
  }
#endif

  /* Somewhere here, we might want to check to make sure this really is
   * a PNG image. It does not have to be, just because this function
   * was called, but right now, we assume it is.
   */

  ui_functions_set_status("Reading and processing PNG image...");

  picture = (struct image_data *)malloc(sizeof(struct image_data));
  if(picture == NULL) {
#ifndef _WIN32_WCE  
	fclose(fp);
#else
	RalCloseFile(fd);
#endif
    return NULL;
  }

  picture->data = NULL;

  pngp = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(pngp == NULL) {
    free(picture);
#ifndef _WIN32_WCE  
	fclose(fp);
#else
	RalCloseFile(fd);
#endif
    return NULL;
  }

  pnginfop = png_create_info_struct(pngp);
  if(pnginfop == NULL) {
    png_destroy_read_struct(&pngp, (png_infopp)NULL, (png_infopp)NULL);
    free(picture);
#ifndef _WIN32_WCE  
	fclose(fp);
#else
	RalCloseFile(fd);
#endif
    return NULL;
  }

  pngendp = png_create_info_struct(pngp);
  if(pngendp == NULL) {
    png_destroy_read_struct(&pngp, &pnginfop, (png_infopp)NULL);
    free(picture);
#ifndef _WIN32_WCE  
	fclose(fp);
#else
	RalCloseFile(fd);
#endif
    return NULL;
  }

  /* Not sure what to do if setjmp is not supported in libpng. For now, we
   * simply skip this here.
   */

#ifdef PNG_SETJMP_SUPPORTED
  if(setjmp(png_jmpbuf(pngp))) {
    png_destroy_read_struct(&pngp, &pnginfop, &pngendp);
    free(picture);
#ifndef _WIN32_WCE  
	fclose(fp);
#else
	RalCloseFile(fd);
#endif
    return NULL;
  }
#endif

#ifdef _WIN32_WCE
  png_init_io(pngp, (FILE *)fd);
#else
  png_init_io(pngp, fp);
#endif


  png_read_info(pngp, pnginfop);
  png_get_IHDR(pngp, pnginfop, &image_width, &image_height, &bit_depth, 
	       &colour_type, &interlace_type, NULL, NULL);

  /*
  fprintf(stderr, "PNG: width=%ld, height=%ld, bit_depth=%d, colour_type=%d, "
	  "interlace_type=%d\n", image_width, image_height, bit_depth,
	  colour_type, interlace_type);
  */

  /* This makes sure we get 8-bit results for 16-bit images. */
  if(bit_depth == 16)
    png_set_strip_16(pngp);

  /* This makes sure we get 8-bit results for less than 8-bit images. */
  if(bit_depth < 8)
    png_set_packing(pngp);

  /* To simplify for us, we let libpng deal with converting greyscaled
   * and palette based images into RGB ones. When the target display
   * also is palette based, I would have preferred to be able to get the
   * palette indexes directly, but this does not seem to be possible
   * with libpng.
   */
  if(colour_type == PNG_COLOR_TYPE_GRAY ||
     colour_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(pngp);
  else if(colour_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(pngp);

  /* As of yet, we have no way to handle transparent images, so we simply
   * strip off the alpha bits here. This will change later.
   */
  if(colour_type & PNG_COLOR_MASK_ALPHA)
    png_set_strip_alpha(pngp);

  /* Get the screen gamma from the settings. */
  //settings_get("screen_gamma", (void **)&setting);
  //screen_gamma = atof(setting);

  /* If the PNG file contains a gamma, we should of course use that. 
   * Otherwise, we use the gamma given in the settings.
   */
  //if(!png_get_gAMA(pngp, pnginfop, &gamma)) {
  //  settings_get("png_gamma", (void **)&setting);
  //  gamma = atof(setting);
  //}
  png_set_gamma(pngp, screen_gamma, gamma);

  /* For interlaced images, this will be more than one. 
   * See below how we uses this.
   */
  number_of_passes = png_set_interlace_handling(pngp);

  /* Check if we are to do quantizing of the image. */
  if(user_interface.ui_display.colourmap) {
    number_of_colours = 1 << user_interface.ui_display.bit_depth;
    colourmap = png_malloc(pngp, number_of_colours * 3);
    create_colourmap(colourmap, number_of_colours);

    png_set_dither(pngp, colourmap,
		   number_of_colours, number_of_colours,
		   NULL, 1);
  }

  /* Update the internal information, so we can get the correct rowsize
   * later on.
   */
  png_read_update_info(pngp, pnginfop);

  /* Since scaling is not possible yet, we only use the original
   * geometry of the image.
   */
  picture->width = image_width;
  picture->height = image_height;

  /* Allocate space for the image data in our struct. */
  picture->size = (picture->width * picture->height *
		   user_interface.ui_display.bit_depth) / 8;
  picture->data = (unsigned char *)malloc(picture->size);
  if(picture->data == NULL) {
    png_destroy_read_struct(&pngp, &pnginfop, &pngendp);
    free(picture);
#ifndef _WIN32_WCE  
	fclose(fp);
#else
	RalCloseFile(fd);
#endif
    return NULL;
  }

  /* Allocate memory for one row. */
  row_stride = png_get_rowbytes(pngp, pnginfop);
  buffer = png_malloc(pngp, row_stride);

  /* First, we have to preread parts of the image, if it is interlaced.
   * The last pass will be processed in the main loop below, so this
   * "dummy" loop will be run number_of_passes minus one times. 
   */
  for(i = 0 ; i < number_of_passes - 1 ; i++)
    for(line_in = 0 ; (unsigned int)line_in < image_height ; line_in++)
      png_read_row(pngp, buffer, NULL);
  
  /* Start reading and converting the image, line by line. */
  line_in = 0;
  picturep = picture->data;
  status = (char *)malloc(128);
  while((unsigned int)line_in < image_height) {
    png_read_row(pngp, buffer, NULL);

    if(status != NULL) {
      sprintf(status, "Reading and processing PNG image... %d%%", 
	      (int)((100 * (line_in+1)) / image_height));
      ui_functions_set_status(status);
    }

    if(user_interface.ui_display.colourmap) {
      memcpy(picturep, buffer, row_stride);
      picturep += row_stride;
    } else {
      i = 0;
      for(x = 0 ;(unsigned int)x < image_width ; x++) {
	unsigned char red, green, blue;
	
	red = (unsigned char)buffer[i++];
	green = (unsigned char)buffer[i++];
	blue = (unsigned char)buffer[i++];
	step = image_get_real_colour(picturep, red, green, blue);
	picturep += step;
      }
    }

    line_in++;
  }

  if(status != NULL)
    free(status);
  png_read_end(pngp, pngendp);
  png_destroy_read_struct(&pngp, &pnginfop, &pngendp);
#ifndef _WIN32_WCE  
	fclose(fp);
#else
	RalCloseFile(fd);
#endif
  return picture;
}

#endif /* HAVE_LIBPNG */
