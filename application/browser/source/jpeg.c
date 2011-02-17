/**
 * Functions that will decompress a JPEG image into a presentable
 * format. This is using libjpeg instead of libmagick for JPEG images,
 * because it is much faster and gives better quality that libmagick.
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

/* This whole file is only compiled if we have libjpeg. */
#ifdef HAVE_LIBJPEG

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32_WCE
	#include <io.h>
	//#include <unistd.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <fcntl.h>
#endif

#include "../../../lib/libjpeg/jpeglib.h"
#include <setjmp.h>

#include "../header/ui.h"
#include "../header/image.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

extern int RalCloseFile(int);

/**
 *
 */
struct error_information {
  struct jpeg_error_mgr pub;
  jmp_buf setjmp_buffer;
};

/**
 *
 */
static void error_handler(j_common_ptr cinfo)
{
  struct error_information *error;

  error = (struct error_information *)cinfo->err;
  longjmp(error->setjmp_buffer, 1);
}

/**
 *
 */
static void create_colourmap(JSAMPARRAY colourmap, int number_of_colours)
{
  int colour;

  for(colour = 0 ; colour < number_of_colours ; colour ++) {
    colourmap[0][colour] = user_interface.ui_display.colourmap[3 * colour + 0];
    colourmap[1][colour] = user_interface.ui_display.colourmap[3 * colour + 1];
    colourmap[2][colour] = user_interface.ui_display.colourmap[3 * colour + 2];
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
struct image_data *image_jpeg_read(int fd, int width, int height)
{
  struct jpeg_decompress_struct cinfo;
  struct error_information error;
  int step, i, line_in, row_stride, x, number_of_colours;
  JSAMPARRAY buffer, colourmap;
  struct image_data *picture;
  unsigned char *picturep;
  char *status;
  int new_fd;
  FILE *fp;

  /* First duplicate the file descriptor, so that we can close it in the
   * correct way, without interfering with the method libjpeg has chosen
   * to operate in.
   */
#ifndef _WIN32_WCE
  new_fd = _dup(fd);
#endif

#if defined _WIN32_WCE || WIN32
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

  ui_functions_set_status("Reading and processing JPEG image...");

  picture = (struct image_data *)malloc(sizeof(struct image_data));

#ifndef _WIN32_WCE  
  if(picture == NULL) {
    fclose(fp);
    return NULL;
  }
#endif
  picture->data = NULL;

  cinfo.err = jpeg_std_error(&error.pub);
  error.pub.error_exit = error_handler;
  if(setjmp(error.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    free(picture);
#ifndef _WIN32_WCE  
	fclose(fp);
#else
	RalCloseFile(fd);
#endif
    return NULL;
  }
  
  jpeg_create_decompress(&cinfo);

#ifdef _WIN32_WCE
  jpeg_stdio_src(&cinfo, (FILE *) fd);
#else
  jpeg_stdio_src(&cinfo, fp);
#endif

  jpeg_read_header(&cinfo, TRUE);

  /* We cannot do automatic scaling with libjpeg.
   * So, we will have to do this ourselves later. 
   */
  cinfo.scale_num = 1;
  cinfo.scale_denom = 1;
  cinfo.do_block_smoothing = FALSE;
  cinfo.dct_method = JDCT_ISLOW;
  cinfo.out_color_space = JCS_RGB;

  /* Check if we are to do quantizing of the image. */
  if(user_interface.ui_display.colourmap) {
    number_of_colours = 1 << user_interface.ui_display.bit_depth;
    cinfo.quantize_colors = TRUE;
    cinfo.desired_number_of_colors = number_of_colours;
    cinfo.two_pass_quantize = FALSE;
    cinfo.dither_mode = JDITHER_FS;

    colourmap = (*cinfo.mem->alloc_sarray)
      ((j_common_ptr) &cinfo, JPOOL_IMAGE, number_of_colours, 3);
    
    create_colourmap(colourmap, number_of_colours);
    
    cinfo.colormap = colourmap;
    cinfo.actual_number_of_colors = number_of_colours;
  } else {
    cinfo.quantize_colors = FALSE;
  }

  jpeg_calc_output_dimensions(&cinfo);

  /* Since scaling is not possible yet, we only use the original
   * geometry of the image.
   */
  picture->width = cinfo.output_width;
  picture->height = cinfo.output_height;

  /* Allocate space for the image data in our struct. */
  picture->size = (picture->width * picture->height *
		   user_interface.ui_display.bit_depth) / 8;
  picture->data = (unsigned char *)malloc(picture->size);
  if(picture->data == NULL) {
    jpeg_destroy_decompress(&cinfo);
    free(picture);
#ifndef _WIN32_WCE  
	fclose(fp);
#else
	RalCloseFile(fd);
#endif
    return NULL;
  }

  jpeg_start_decompress(&cinfo);

  /* Allocate memory for one row. */
  row_stride = cinfo.output_width * cinfo.output_components;
  buffer = (*cinfo.mem->alloc_sarray)
                ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

  /* Start reading and converting the image, line by line. */
  line_in = 0;
  picturep = picture->data;
  status = (char *)malloc(128);
  while((unsigned int)line_in < cinfo.output_height) {
    jpeg_read_scanlines(&cinfo, buffer, 1);

    if(status != NULL) {
      sprintf(status, "Reading and processing JPEG image... %d%%", 
	      (int)((100 * (line_in+1)) / cinfo.output_height));
      ui_functions_set_status(status);
    }

    if(BITS_IN_JSAMPLE == 8 && user_interface.ui_display.colourmap) {
      memcpy(picturep, buffer[0], row_stride);
      picturep += row_stride;
    } else if(cinfo.output_components == 1) {
      for(x = 0 ; (unsigned int)x < cinfo.output_width ; x++) {
	unsigned char colour;
	colour = 
	  (unsigned char)(((uint32_t)buffer[0][x] << 8) >> BITS_IN_JSAMPLE);
	step = image_get_real_colour(picturep, colour, colour, colour);
	picturep += step;
      }
    } else {
      i = 0;
      for(x = 0 ; (unsigned int)x < cinfo.output_width ; x++) {
	unsigned char red, green, blue;
	red = 
	  (unsigned char)(((uint32_t)buffer[0][i++] << 8) >> BITS_IN_JSAMPLE);
	green = 
	  (unsigned char)(((uint32_t)buffer[0][i++] << 8) >> BITS_IN_JSAMPLE);
	blue = 
	  (unsigned char)(((uint32_t)buffer[0][i++] << 8) >> BITS_IN_JSAMPLE);
	step = image_get_real_colour(picturep, red, green, blue);
	picturep += step;
      }
    }

    line_in++;
  }

  if(status != NULL)
    free(status);
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
#ifndef _WIN32_WCE  
	fclose(fp);
#else
	RalCloseFile(fd);
#endif

  return picture;
}

#endif /* HAVE_LIBJPEG */
