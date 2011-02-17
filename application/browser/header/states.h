/** 
 * Structs and prototypes for handling different states the parts can
 * have. This includes the text styles that can appear in the html
 * page and the alignments, which are not only for text, but for whole
 * parts.
 *
 * @author Tomas Berndtsson <tomas@nocrew.org> */

#ifndef _PARSER_STATES_H_
#define _PARSER_STATES_H_

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

#include "../header/layout.h"

/**
 * Contains information about the style, align and base state of
 * the page. 
 * 
 * @member align The alignment used for the current state.
 * @member style The style to describe this particular state of the parsing.
 * @member base A pointer to the layout part which is to be the base part
 * @member base of new parts added.
 * @member previous A pointer to the previous state in the pushed stack.
*/
struct parse_state {
  char *tagname;
  struct layout_text_styles style;
  struct layout_aligns align;
  struct layout_part *base;
  struct parse_state *previous;
};

/**
 * Contains information about a saved mark in the state stacks.
 *
 * @member mark_style A pointer to the saved style position.
 * @member mark_align A pointer to the saved align position.
 * @member mark_base A pointer to the saved base position.
 * @member previous A pointer to the previous mark in the pushed stack.
 */
struct parse_state_mark {
  struct parse_state_style *mark_style;
  struct parse_state_align *mark_align;
  struct parse_state_base *mark_base;
  struct parse_state_mark *previous;
};

/* State function prototypes. */
extern int parse_state_init(void);
extern int parse_state_delete_all(void);
extern void parse_state_reset(struct layout_text_styles *style,
			      struct layout_aligns *align,
			      struct layout_part **base);
extern int parse_state_get_current(struct layout_text_styles *style,
				   struct layout_aligns *align,
				   struct layout_part **base);
extern int parse_state_push(char *tagname,
			    struct layout_text_styles *style,
			    struct layout_aligns *align,
			    struct layout_part **base);
extern int parse_state_pop(char *tagname, int delete_nested);
extern char *parse_state_peek(void);

#endif /* _PARSER_STATES_H_ */
