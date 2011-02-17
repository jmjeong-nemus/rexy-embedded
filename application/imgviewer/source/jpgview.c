

#include "../header/jpgview.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
//#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../../lib/libjpeg/jpeglib.h"
#include <setjmp.h>



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
/*
static void create_colourmap(JSAMPARRAY colourmap, int number_of_colours)
{
  int colour;

  for(colour = 0 ; colour < number_of_colours ; colour ++) {
    colourmap[0][colour] = user_interface.ui_display.colourmap[3 * colour + 0];
    colourmap[1][colour] = user_interface.ui_display.colourmap[3 * colour + 1];
    colourmap[2][colour] = user_interface.ui_display.colourmap[3 * colour + 2];
  }
}
*/
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
struct img_data *img_jpeg_read(int fd, int width, int height)
{
  struct jpeg_decompress_struct cinfo;
  struct error_information error;
  int i, line_in, row_stride, x;
  JSAMPARRAY buffer;
  struct img_data *picture;
  unsigned char *picturep;
  int new_fd;
  FILE *fp;

  /* First duplicate the file descriptor, so that we can close it in the
   * correct way, without interfering with the method libjpeg has chosen
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


  picture = (struct img_data *)malloc(sizeof(struct img_data));
  if(picture == NULL) {
    fclose(fp);
    return NULL;
  }
  picture->data = NULL;

  cinfo.err = jpeg_std_error(&error.pub);
  error.pub.error_exit = error_handler;
  if(setjmp(error.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    free(picture);
    fclose(fp);
    return NULL;
  }
  
  jpeg_create_decompress(&cinfo);

  jpeg_stdio_src(&cinfo, fp);

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
  cinfo.quantize_colors = FALSE;

  jpeg_calc_output_dimensions(&cinfo);

  /* Since scaling is not possible yet, we only use the original
   * geometry of the image.
   */
  picture->width = cinfo.output_width;
  picture->height = cinfo.output_height;

  /* Allocate space for the image data in our struct. */
  picture->size = (picture->width * picture->height * 16 ) / 8;
		   //user_interface.ui_display.bit_depth) / 8;
  picture->data = (unsigned char *)malloc(picture->size);
  if(picture->data == NULL) {
    jpeg_destroy_decompress(&cinfo);
    free(picture);
    fclose(fp);
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
  while((unsigned int)line_in < cinfo.output_height) {
    jpeg_read_scanlines(&cinfo, buffer, 1);


    if(cinfo.output_components == 1) {
      for(x = 0 ; (unsigned int)x < cinfo.output_width ; x++) {
		unsigned short a;
		unsigned char colour;
		colour = (unsigned char)(((unsigned int)buffer[0][x] << 8) >> BITS_IN_JSAMPLE);
		a = ( colour >> 3) << 11 | ( colour >> 2) << 5 | colour >> 3;
		*picturep = a>>8 & 0xFF;
		*(picturep+1) = a & 0xFF;
		picturep += 2;
		//step = image_get_real_colour(picturep, colour, colour, colour);
		//picturep += step;
      }
    } else {
		unsigned short a;
      i = 0;
      for(x = 0 ; (unsigned int)x < cinfo.output_width ; x++) {
		unsigned char red, green, blue;
		red = 
		  (unsigned char)(((unsigned int)buffer[0][i++] << 8) >> BITS_IN_JSAMPLE);
		green = 
		  (unsigned char)(((unsigned int)buffer[0][i++] << 8) >> BITS_IN_JSAMPLE);
		blue = 
		  (unsigned char)(((unsigned int)buffer[0][i++] << 8) >> BITS_IN_JSAMPLE);
		//step = image_get_real_colour(picturep, red, green, blue);
		//picturep += step;
		a = (red >> 3) << 11 | (green >> 2) << 5 | blue >> 3 ;
		*picturep = a>>8 & 0xFF;
		*(picturep+1) = a & 0xFF;
		picturep += 2;
      }
    }

    line_in++;
  }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(fp);

  return picture;
}
