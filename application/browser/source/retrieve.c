/**
 * Functions to retrieve an object.
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
#include <string.h>

#include "../header/protocol.h"
#include "../header/parse.h"
#include "../header/layout.h"
#include "../header/ui.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

/**
 * This function will take a URL from the the user interface, get the
 * page, parse it, layout it and send it back to the user interface.
 * Of course this function will not do all that work by itself, but use
 * other functions for it.
 *
 * @param url The URL to the object to retreive.
 * @param referer The URL we were at when moving to the new URL.
 *
 * @return a pointer to the layouted parts from what is found on URL, or
 * @return NULL if an error occurred.
 */
struct layout_part *retrieve_page(char *url, char *referer)
{
  struct protocol_http_headers *headers;
  struct layout_part *base_part;
  int fd;
  int ret;
  char *status;

  /* Open a stream to the specified URL. */
  fd = protocol_open(url, referer, NULL);
#if defined _WIN32_WCE || WIN32
  if(fd == -1) {
#else
  if(fd < 0) {
#endif
    ui_functions_set_status("Unable to load page.");
    return NULL;
  }
    
  ret = 0;
  headers = protocol_get_headers(fd);
  if(headers == NULL) {
    protocol_close(fd);
    return NULL;
  } else {
    /* Aah, we have headers, my friend. First of all, we initialize a new
     * layout page, based on the HTTP headers we just got from the protocol.
     * The layout part pointer returned is pointing to the base of the linked
     * list containing all parts of the new page.
     */

    base_part = layout_init_page(headers->real_url, referer);

    if(headers->content_type_major == NULL || 
       !strcmp(headers->content_type_major, "text")) {
      if(headers->content_type_minor == NULL ||
	 !strcmp(headers->content_type_minor, "html")) {
	ret = parse_html(fd);
      } else if(!strcmp(headers->content_type_minor, "plain")) {
	ret = parse_text(fd);
      }
    } else if(!strcmp(headers->content_type_major, "image")) {
      ret = parse_image(fd);
    } else {
      status = malloc(strlen(headers->content_type_major) + 
		      strlen(headers->content_type_minor) + 64);
      if(status == NULL) {
	fprintf(stderr, 
		"I do not yet know what to do with streams "
		"of the type %s/%s.\n",
		headers->content_type_major, headers->content_type_minor);
      } else {
	sprintf(status, 
		"Do not know how to handle %s/%s.",
		headers->content_type_major, headers->content_type_minor);
	ui_functions_set_status(status);
	free(status);
      }

      return NULL;
    }
  }

  /* Check if the page does not contain any data. */
  if(base_part->child == NULL) {
    ui_functions_set_status("Page contains no data.");
    ret = 1;
  }

  /* Check if the parsing went smooth. */
  if(ret == 0) {
    /* Do the layouting on the page we got from layout_init_page(). */
    layout_do(base_part, 0, 0, NULL);
  }

  /* Close the stream in the proper way. */
  protocol_close(fd);

  if(ret == 0)
    return base_part;
  else
    return NULL;
}
