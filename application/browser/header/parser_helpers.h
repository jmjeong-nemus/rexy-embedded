/** 
 * Declarations of helper functions for the parser.
 *
 * @author Tomas Berndtsson <tomas@nocrew.org>
 */

#ifndef _PARSER_HELPERS_H_
#define _PARSER_HELPERS_H_

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

#include "../header/tags.h"
#include "../header/layout.h"

/* Parse helpers */
extern int parse_get_byte(int fd, char *c);
extern struct parse_param *parse_alloc_param(char *name, char *value);
extern int parse_free_param(struct parse_param *paramp);
extern char *parse_get_param_value(struct parse_param *paramp, char *name);
extern struct parse_tag *parse_alloc_tag(const char *name, enum parse_tag_type type);
extern int parse_free_tag(struct parse_tag *tagp);
extern int parse_read_word(int fd, char **word, int first, char *delimiters);
extern int parse_skip_leading(int fd, char *skippers);
extern struct parse_tag *parse_get_tag(int fd);
extern uint32_t parse_convert_colour(char *colour);

/* String helpers */
extern int parse_string_store_character(unsigned char c);
extern void parse_string_set_preformatted(int value);
extern int parse_string_get_stored(char *buf,unsigned int *extra_char_cnts, unsigned int *special_char_cnts, int size);
extern int parse_string_set_stored(char *buf);
extern int parse_string_get_length(void);
extern void parse_string_discard(void);
extern struct layout_part *parse_string_store_current(void);
extern int parse_string_store_title(void);

/* Debug helpers */
extern void debug_dump_string(void);
extern void debug_dump_param(struct parse_param *paramp);
extern void debug_dump_tag(struct parse_tag *tagp);

#endif /* _PARSER_HELPERS_H_ */
