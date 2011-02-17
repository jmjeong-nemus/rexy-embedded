/** 
 * Structs and defines that is used by the parser to parse tags.
 *
 * @author Tomas Berndtsson <tomas@nocrew.org>
 */

#ifndef _PARSER_TAGS_H_
#define _PARSER_TAGS_H_

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

/**
 * The different types a tag can be.
 * There are only two relevant types, start and end.
 */
enum parse_tag_type {
  PARSE_TAG_START,
  PARSE_TAG_END
};

/**
 * Contains one parameter with its name and value.
 *
 * @member name The name of the parameter.
 * @member value The value of the parameter.
 */
struct parse_param {
  char *name;
  char *value;
  struct parse_param *next;
};

/**
 * Contains the name of the tag and its a linked list of its
 * parameters.
 *
 * @member name The name of the tag.
 * @member type Tells if the tag is a start or an end tag. 
 * @member first A pointer to the first parameter in the list.
 */
struct parse_tag {
  char *name;
  enum parse_tag_type type;
  struct parse_param *first;
};

/**
 * Type definition for tag binding functions.
 */
typedef int parse_tag_function(struct parse_tag *);

/**
 * Contains function bindings to each tag. The functions are
 * called for both start and end tag. It is possible to use
 * the same function for several tags.
 *
 * @member name The name of the tag.
 * @member function Pointer to function tahe should be called
 * @member function for this tag.
 */
struct parse_tag_binding {
  char *name;
  parse_tag_function *function;
};

/* Function prototype */
extern int parse_call_tag_binding(struct parse_tag *tagp);

#endif /* _PARSER_TAGS_H_ */
