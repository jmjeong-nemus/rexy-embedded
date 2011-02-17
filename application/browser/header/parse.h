/** 
 * Structs and defines that is used by the parser.
 *
 * @author Tomas Berndtsson <tomas@nocrew.org>
 */

#ifndef _PARSER_PARSE_H_
#define _PARSER_PARSE_H_

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

/* Prototypes of parsing functions. */
extern int parse_html(int fd);
extern int parse_text(int fd);
extern int parse_image(int fd);

#endif /* _PARSER_PARSE_H_ */
