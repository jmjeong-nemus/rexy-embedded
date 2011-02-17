



#include "../header/gifview.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
//#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../../lib/libungif/lib/gif_lib.h"




struct img_data *img_gif_read(int fd, int width, int height)
{
  GifFileType *gifp;
  GifRecordType record;
  GifByteType *extension;
  int i, j, ret, extcode, rowsize, images_read;
  int row, col, image_width, image_height;
  struct img_data *picture;
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
  new_fd = dup(fd);
  if(new_fd == -1) {
    /* If there is an error here, we could theoretically proceed with some
     * special treatment of the streams, but it indicates that something is
     * more wrong that we want it to be, so we just return an error here. 
     */
    return NULL;
  }

  fp = fdopen(new_fd, "r");
  if(fp == NULL) {
    close(new_fd);
    return NULL;
  }
#endif


  picture = (struct img_data *)malloc(sizeof(struct img_data));
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
  picture->size = (picture->width * picture->height * 16) / 8;
		   //user_interface.ui_display.bit_depth) / 8;
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
		unsigned short a;
		colour = &colourmap->Colors[buffer[i][j]];
		a = (colour->Red >> 3) << 11 | (colour->Green >> 2) << 5 | colour->Blue >> 3 ;
		*picturep = a>>8 & 0xFF;
		*(picturep+1) = a & 0xFF;
		picturep += 2;
    }

  }

  for(i = 0 ; i < gifp->SHeight ; i++)
    free(buffer[i]);
  free(buffer);

  DGifCloseFile(gifp);

  return picture;
}
