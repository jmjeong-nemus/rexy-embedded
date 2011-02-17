/** 
 * Structs and function prototypes for dealing with images, loading them
 * and converting them.
 *
 * @author Tomas Berndtsson <tomas@nocrew.org>
 */

#ifndef _IMAGE_IMAGE_H_
#define _IMAGE_IMAGE_H_

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

#ifndef _WIN32_WCE
	#include <sys/types.h>
#endif

/**
 * Consists of whatever information the user interface might need to
 * know about the image to show.
 *
 * @member width The width of the image in pixels.
 * @member height The height of the image in pixels.
 * @member size The size of the image in bytes.
 * @member data A pointer to the actual image data.
 */
struct image_data {
  int width;
  int height;
  size_t size;
  unsigned char *data;
};

/* Function prototype. */
extern struct image_data *image_open(int fd, int width, int height);

/* Helper function. */
extern int image_get_real_colour(unsigned char *datap, unsigned char red, 
				 unsigned char green, unsigned char blue);

#endif /* _IMAGE_IMAGE_H_ */
