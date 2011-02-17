

#include "../header/pngview.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
//#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../../lib/lpng/png.h"



/*
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
struct img_data *img_png_read(int fd, int width, int height)
{
  png_structp pngp;
  png_infop pnginfop, pngendp;
  png_uint_32 image_width, image_height;
  int bit_depth, colour_type, interlace_type;
  int number_of_passes;
//  char *setting;
  double screen_gamma, gamma;
  int i, line_in, row_stride, x;
//  png_color *colourmap;
  png_bytep buffer;
  struct img_data *picture;
  unsigned char *picturep;
  int new_fd;
  FILE *fp;

  /* First duplicate the file descriptor, so that we can close it in the
   * correct way, without interfering with the method libpng has chosen
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

 //fp = fopen ( new_fd, "rb");
 if(fp == NULL) {
    close(new_fd);
    return NULL;
  }

  /* Somewhere here, we might want to check to make sure this really is
   * a PNG image. It does not have to be, just because this function
   * was called, but right now, we assume it is.
   */


  picture = (struct img_data *)malloc(sizeof(struct img_data));
  if(picture == NULL) {
    fclose(fp);
    return NULL;
  }
  picture->data = NULL;

  pngp = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(pngp == NULL) {
    free(picture);
    fclose(fp);
    return NULL;
  }

  pnginfop = png_create_info_struct(pngp);
  if(pnginfop == NULL) {
    png_destroy_read_struct(&pngp, (png_infopp)NULL, (png_infopp)NULL);
    free(picture);
    fclose(fp);
    return NULL;
  }

  pngendp = png_create_info_struct(pngp);
  if(pngendp == NULL) {
    png_destroy_read_struct(&pngp, &pnginfop, (png_infopp)NULL);
    free(picture);
    fclose(fp);
    return NULL;
  }

  /* Not sure what to do if setjmp is not supported in libpng. For now, we
   * simply skip this here.
   */

#ifdef PNG_SETJMP_SUPPORTED
  if(setjmp(png_jmpbuf(pngp))) {
    png_destroy_read_struct(&pngp, &pnginfop, &pngendp);
    free(picture);
    fclose(fp);
    return NULL;
  }
#endif

  png_init_io(pngp, fp);

  png_read_info(pngp, pnginfop);
  png_get_IHDR(pngp, pnginfop, &image_width, &image_height, &bit_depth, 
	       &colour_type, &interlace_type, NULL, NULL);


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
  picture->size = (picture->width * picture->height * 16) / 8;
		   //user_interface.ui_display.bit_depth) / 8;
  picture->data = (unsigned char *)malloc(picture->size);
  if(picture->data == NULL) {
    png_destroy_read_struct(&pngp, &pnginfop, &pngendp);
    free(picture);
    fclose(fp);
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
  while((unsigned int)line_in < image_height) {
    png_read_row(pngp, buffer, NULL);


    {
      i = 0;
      for(x = 0 ;(unsigned int)x < image_width ; x++) {
		unsigned char red, green, blue;
		unsigned short a;
	
		red = (unsigned char)buffer[i++];
		green = (unsigned char)buffer[i++];
		blue = (unsigned char)buffer[i++];
		a = (red >> 3) << 11 | (green >> 2) << 5 | blue >> 3 ;
		*picturep = a>>8 & 0xFF;
		*(picturep+1) = a & 0xFF;
		picturep += 2;
      }
    }

    line_in++;
  }

  png_read_end(pngp, pngendp);
  png_destroy_read_struct(&pngp, &pnginfop, &pngendp);
  fclose(fp);

  return picture;
}
