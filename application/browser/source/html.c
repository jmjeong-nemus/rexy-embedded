/**
 * Parse the input as HTML code.
 * The functions herein will build a structure of different kinds of 
 * entities to pass on to the layout part of Zen. 
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
//#include <unistd.h>

#include "../header/threads.h"
#include "../header/states.h"
#include "../header/tags.h"
#include "../header/parser_helpers.h"
#include "../header/layout.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

extern int strcasecmp(const char *s1, const char *s2);

static int gTagBinding = 0;
static int aTagFlags = 2; // no tag

/**
 * Treat the incoming stream as HTML with equal rights.
 * 
 * @param fd File descriptor of input stream.
 *
 * @return non-zero value if an error occurred.
 */
int parse_html(int fd)
{
	unsigned char c;
	int ret, amount_read;
	struct parse_tag *tmptagp;

	ret = 0;

	/* Initialize the state struct used throughout the whole parsing. */
	parse_state_delete_all();
	parse_state_init();

	while(1) 
	{
		amount_read = parse_get_byte(fd, &c);

		if(amount_read <= 0) 
		{ /* Either error or end of stream. */
			ret = amount_read;
			break;
		}

		if(c == '<') 
		{ /* There is a tag approaching. */
			tmptagp = parse_get_tag(fd);

			if(tmptagp != NULL) 
			{
				aTagFlags = tmptagp->type;
				if( parse_call_tag_binding(tmptagp) == 1 )
					gTagBinding = 0;
				else
				{
					gTagBinding = 1;
					/* Delete the tag when we are done with it. */
					if((!strcasecmp(tmptagp->name, "li")) && (tmptagp->type == 0))
						parse_string_store_character('*');
				}
				/* Delete the tag when we are done with it. */
				parse_free_tag(tmptagp);
				/* Yield after each tag. Maybe that's overkill? */
				thread_yield();
			}
		}
		else
		{ 
			/* Normal, simple, boring character */
			if ( !aTagFlags )
			{
				if( gTagBinding )
					parse_string_store_character(c);
			}
			else
				parse_string_store_character(c);

		}

	}
	/* The file is at an end, please place the last read string in the list. */
	parse_string_store_current();

	/* Delete the states when we are done with them. */
	parse_state_delete_all();

	return ret;
}
