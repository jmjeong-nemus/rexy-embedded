#ifndef __IMGDATA_H__
#define __IMGDATA_H__

#include <stdlib.h>
#include <sys/types.h>

/**
 * Consists of whatever information the user interface might need to
 * know about the image to show.
 *
 * @member width The width of the image in pixels.
 * @member height The height of the image in pixels.
 * @member size The size of the image in bytes.
 * @member data A pointer to the actual image data.
 */

struct img_data {
  int width;
  int height;
  size_t size;
  unsigned char *data;
};


#endif	// __IMGDATA_H__
