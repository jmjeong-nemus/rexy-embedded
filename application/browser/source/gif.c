/**
 * Functions that will decompress a GIF image into a format we can
 * use. This is using either giflib or libungif to handle decompression, 
 * depending on what was available at configuration time.
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

/* This whole file is only compiled if we have giflib or libungif. */
#if defined(HAVE_LIBGIF) || defined(HAVE_LIBUNGIF)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include "../../../lib/libungif/lib/gif_lib.h"

#ifndef _WIN32_WCE
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <fcntl.h>
#endif

#include "../header/ui.h"
#include "../header/image.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

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
struct image_data *image_gif_read(int fd, int width, int height)
{
  GifFileType *gifp;
  GifRecordType record;
  GifByteType *extension;
  int i, j, ret, extcode, rowsize, images_read, step;
  int row, col, image_width, image_height;
  struct image_data *picture;
  unsigned char *picturep;
  GifRowType *buffer;
  ColorMapObject *colourmap;
  GifColorType *colour;
  char *status;
  int interlaceoffset[] = { 0, 4, 2, 1 };
  int interlacejump[] = { 8, 8, 4, 2 };

  gifp = DGifOpenFileHandle(fd);
  if(gifp == NULL) {
    return NULL;
  }

#if 0
  /* First duplicate the file descriptor, so that we can close it in the
   * correct way, without interfering with the method libmagick has chosen
   * to operate in.
   */
#ifndef _WIN32_WCE
  new_fd = dup(fd);
#else
  new_fd = fd;
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
#endif

  ui_functions_set_status("Reading and processing GIF image...");

  picture = (struct image_data *)malloc(sizeof(struct image_data));
  if(picture == NULL) {
    DGifCloseFile(gifp);
    return NULL;
  }
  picture->data = NULL;

#if 0
  fprintf(stderr, "screen width=%d, height=%d\n",
	  gifp->SWidth, gifp->SHeight);
#endif

  buffer = (GifRowType *)malloc(gifp->SHeight * sizeof(GifRowType *));
  if(buffer == NULL) {
    free(picture);
    DGifCloseFile(gifp);
    return NULL;
  }
  rowsize = gifp->SWidth * sizeof(GifPixelType);
  buffer[0] = (GifRowType)malloc(rowsize);
  if(buffer[0] == NULL) {
    free(buffer);
    free(picture);
    DGifCloseFile(gifp);
    return NULL;
  }
  for(i = 0 ; i < gifp->SWidth ; i++)
    buffer[0][i] = gifp->SBackGroundColor;
  for(i = 0 ; i < gifp->SHeight ; i++) {
    buffer[i] = (GifRowType)malloc(rowsize);
    if(buffer[i] == NULL) {
      while(--i >= 0)
	free(buffer[i]);
      free(buffer);
      free(picture);
      DGifCloseFile(gifp);
      return NULL;
    }
    memcpy(buffer[i], buffer[0], rowsize);
  }

  /* We only read the first GIF image right now, since we are not
   * supporting animated GIF images. Perhaps the last image would
   * actually be a better choice. Oh well.
   */
  images_read = 0;
  do {
    DGifGetRecordType(gifp, &record);
    ret = GIF_OK;
    switch(record) {
    case IMAGE_DESC_RECORD_TYPE:
      ret = DGifGetImageDesc(gifp);
      if(ret == GIF_ERROR)
	break;

#if 0
      fprintf(stderr, "image width=%d, height=%d, interlace=%d\n",
	      gifp->Image.Width, gifp->Image.Height, gifp->Image.Interlace);
#endif

      row = gifp->Image.Top;
      col = gifp->Image.Left;
      image_width = gifp->Image.Width;
      image_height = gifp->Image.Height;

      if(gifp->Image.Interlace) {
	for(i = 0 ; i < 4 ; i++) {
	  for(j = row + interlaceoffset[i] ; 
	      j < row + image_height ;
	      j += interlacejump[i]) {
	    DGifGetLine(gifp, &buffer[j][col], image_width);
	  }
	}
      } else {
	for(i = 0 ; i < image_height ; i++) {
	  DGifGetLine(gifp, &buffer[row++][col], image_width);
	}
      }

      images_read++;
      
      break;

      /* Read and skip any extension. */
    case EXTENSION_RECORD_TYPE:
      ret = DGifGetExtension(gifp, &extcode, &extension);
      while(extension != NULL)
	ret = DGifGetExtensionNext(gifp, &extension);
      break;

    default:
      break;
    }
  } while(images_read == 0 && ret == GIF_OK && 
	  record != TERMINATE_RECORD_TYPE);
  if(ret == GIF_ERROR) {
    for(i = 0 ; i < gifp->SHeight ; i++)
      free(buffer[i]);
    free(buffer);
    free(picture);
    DGifCloseFile(gifp);
    return NULL;
  }

  /* Since scaling is not possible yet, we only use the original
   * geometry of the image.
   */
  picture->width = gifp->SWidth;
  picture->height = gifp->SHeight;

  /* Allocate space for the image data in our struct. */
  picture->size = (picture->width * picture->height *
		   user_interface.ui_display.bit_depth) / 8;
  picture->data = (unsigned char *)malloc(picture->size);
  if(picture->data == NULL) {
    free(picture);
    for(i = 0 ; i < gifp->SHeight ; i++)
      free(buffer[i]);
    free(buffer);
    DGifCloseFile(gifp);
    return NULL;
  }

  if(gifp->Image.ColorMap != NULL)
    colourmap = gifp->Image.ColorMap;
  else
    colourmap = gifp->SColorMap;
    
  status = (char *)malloc(128);
  picturep = picture->data;
  for(i = 0 ; i < picture->height ; i++) {
    for(j = 0 ; j < picture->width ; j++) {
      colour = &colourmap->Colors[buffer[i][j]];
      step = image_get_real_colour(picturep, colour->Red, 
				   colour->Green, colour->Blue);
      picturep += step;
    }

    if(status != NULL) {
      sprintf(status, "Reading and processing GIF image... %d%%", 
	      (int)((100 * (i+1)) / picture->height));
      ui_functions_set_status(status);
    }
  }

  for(i = 0 ; i < gifp->SHeight ; i++)
    free(buffer[i]);
  free(buffer);

  DGifCloseFile(gifp);

  return picture;
}

#endif /* defined(HAVE_LIBGIF) || defined(HAVE_LIBUNGIF) */
