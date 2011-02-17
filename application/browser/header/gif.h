/** 
 * Function prototypes for dealing with GIF images using giflib 
 * or libungif.
 *
 * @author Tomas Berndtsson <tomas@nocrew.org>
 */

#ifndef _IMAGE_GIF_H_
#define _IMAGE_GIF_H_

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
struct image_data *image_gif_read(int fd, int width, int height);

#endif /* _IMAGE_GIF_H_ */
