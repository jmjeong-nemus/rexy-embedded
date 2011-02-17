/**
 * Functions to encode text using base64 code.
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//nclude <unistd.h>

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

static unsigned char base64_codes[] = 
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/**
 * Encodes a piece of data into an base64 encoded string. The encoded
 * string is terminated with a zero.
 * 
 * @param data The data to be encoded.
 * @param length The length of the data in bytes.
 * @param encoded A pointer to an area to hold the encoded string. This
 * @param encoded must be allocated by the caller.
 *
 * @return non-zero value if an error occurred.
 */
int base64_encode(unsigned char *data, int length, unsigned char *encoded)
{
  int i;
  unsigned char d0, d1, d2;

  for(i = 0 ; i < length ; i += 3) {
    d0 = data[i + 0];

    if(i + 1 < length)
      d1 = data[i + 1];
    else
      d1 = 0;

    if(i + 2 < length)
      d2 = data[i + 2];
    else
      d2 = 0;
      
    *encoded++ = base64_codes[d0 >> 2];
    *encoded++ = base64_codes[((d0 & 0x03) << 4) | d1 >> 4];
    if(i + 1 < length)
      *encoded++ = base64_codes[((d1 & 0x0f) << 2) | d2 >> 6];
    else
      *encoded++ = '=';
    if(i + 2 < length)
      *encoded++ = base64_codes[d2 & 0x3f];
    else
      *encoded++ = '=';
  }

  *encoded = '\0';
  
  return 0;
}

