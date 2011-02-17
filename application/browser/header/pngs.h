/** 
 * Function prototypes for dealing with PNG images using libpng.
 * This file is called pngs.h because it would otherwise conflict 
 * with the png.h that belongs to libpng.
 *
 * @author Tomas Berndtsson <tomas@nocrew.org>
 */

#ifndef _IMAGE_PNGS_H_
#define _IMAGE_PNGS_H_

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

#include "../header/image.h"

/* Function prototype. */
struct image_data *image_png_read(int fd, int width, int height);

// added by wichang
/*
typedef unsigned char png_byte;

typedef struct png_color_struct
{
	png_byte red;
	png_byte green;
	png_byte blue;
}png_color;


typedef png_struct FAR * png_structp;
*/
#endif /* _IMAGE_PNGS_H_ */
