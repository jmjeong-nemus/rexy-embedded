/** 
 * Function prototypes for encoding base64 strings.
 *
 * @author Tomas Berndtsson <tomas@nocrew.org>
 */

#ifndef _COMMON_BASE64_H_
#define _COMMON_BASE64_H_

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

/* Function prototypes. */
extern int base64_encode(unsigned char *data, int length, 
			 unsigned char *encoded);

#endif /* _COMMON_BASE64_H_ */
