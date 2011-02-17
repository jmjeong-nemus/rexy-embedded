/**
 * Parse the input as plain text.
 * This will take the whole file, and put it inside a <pre></pre>
 * container tag, basically. It will make sure not to translate
 * any character entities or such too, and to keep all new lines
 * as they are written. 
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
#include <stdlib.h>
//#include <unistd.h>

#include "../header/states.h"
#include "../header/parser_helpers.h"
#include "../header/layout.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

/**
 * Treat the incoming stream as plain text.
 * 
 * @param fd File descriptor of input stream.
 *
 * @return non-zero value if an error occurred.
 */
int parse_text(int fd)
{
  char c;
  int ret, amount_read;
  struct layout_text_styles style;

  ret = 0;

  /* Initialize the state struct used throughout the whole parsing. */
  parse_state_delete_all();
  parse_state_init();

  parse_state_get_current(&style, NULL, NULL);
  style.monospaced = 1;
  style.preformatted = 1;
  style.directquote = 1;
  parse_state_push("textparser", &style, NULL, NULL);

  while(1) {
    amount_read = parse_get_byte(fd, &c);
    if(amount_read <= 0) { /* Either error or end of stream. */
      ret = amount_read;
      break;
    }
    
    parse_string_store_character(c);
  }

  /* The file is at an end, please place the last read string in the list. */
  parse_string_store_current();

  /* Delete the states when we are done with them. */
  parse_state_delete_all();

  return ret;
}

