/** 
 * Structures to keep track of what streams are open, and which protocol
 * they were opened with.
 *
 * @author Tomas Berndtsson <tomas@nocrew.org>
 */

#ifndef _PROTOCOL_STREAMS_H_
#define _PROTOCOL_STREAMS_H_

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

#include "../header/http.h"

/**
 * The different protocols that are handled.
 */
enum protocol_type {
  PROTOCOL_UNKNOWN,
  PROTOCOL_FILE,
  PROTOCOL_HTTP,
  PROTOCOL_FTP
};

/**
 * Linked list containing information about the stream. This is saved
 * in order to be able to close down the stream in the correct manor
 * after the rest of the program is done beating and reading from it.
 *
 * @member type Which protocol the stream is opened with.
 * @member fd The file descriptor associated with the stream.
 * @member headers A pointer to a struct with information taken from the
 * @member headers response headers in an HTTP stream. If the stream is
 * @member headers of a different kind, this is set to NULL.
 * @member next The next stream in the linked list.
 */
struct protocol_stream {
  enum protocol_type protocol;
  int fd;
  struct protocol_http_headers *headers;
  struct protocol_stream *next;
};

#endif /* _PROTOCOL_STREAMS_H_ */
