/** 
 * Functions to handle the text styles and part alignments that can appear 
 * in the html page. These takes care of stacking old states, so that they 
 * can easily be retreived again.
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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../header/ui.h"
#include "../header/layout.h"
#include "../header/states.h"
#include "../header/parser_helpers.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

/* A pointer to the current state. */
static struct parse_state *current_state = NULL;

static void parse_state_delete(struct parse_state *state)
{
  struct parse_state *prev, *statep;

  prev = NULL;
  statep = current_state;
  while(statep && statep != state) {
    prev = statep;
    statep = statep->previous;
  }

  /* Return directly if the state could not be found. */
  if(statep == NULL)
    return;

  if(prev == NULL)
    current_state = statep->previous;
  else
    prev->previous = statep->previous;

  free(statep->tagname);
  free(statep);
}

/**
 * Reset the different states to their default values. This is
 * used when initializing the states, and can also be used later
 * to bring the state to its default state. 
 *
 * @param style A pointer to a text style struct to be reset.
 * @param style NULL means the text style is not be reset.
 * @param align A pointer to an alignment struct to be reset.
 * @param align NULL means the alignment is not be reset.
 * @param base A pointer to a pointer of a base part to be reset.
 * @param base NULL means the base part is not be reset.
 */
void parse_state_reset(struct layout_text_styles *style,
		       struct layout_aligns *align,
		       struct layout_part **base)
{
  if(style != NULL) {
    style->size = user_interface.ui_settings.default_fontsize;
    /*     style->face = NULL; */
    style->colour = 0x80ffffff;
    style->monospaced = 0;
    style->italic = 0;
    style->bold = 0;
    style->underlined = 0;
    style->overlined = 0;
    style->subscript = 0;
    style->superscript = 0;
    style->preformatted = 0;
    style->directquote = 0;
  }

  if(align != NULL) {
    align->horizontal = LAYOUT_PART_ALIGN_LEFT;
    align->vertical = LAYOUT_PART_ALIGN_BOTTOM;
    align->indent_offset = 0;
	align->horzontal_image = LAYOUT_PART_ALIGN_IMAGE_LEFT;
  }

  if(base != NULL) {
    *base = NULL;
  }
}

/**
 * Initialize the state stack with the default values.
 *
 * @return non-zero value if an error occurred.
 */
int parse_state_init(void)
{
  if(current_state == NULL) {
    current_state = (struct parse_state *)malloc(sizeof(struct parse_state));
    if(current_state == NULL)
      return 1;
    current_state->tagname = (char *)malloc(16);
    if(current_state->tagname == NULL) {
      free(current_state);
      return 1;
    }
    strcpy(current_state->tagname, "initiated");
    current_state->previous = NULL;
  }

  parse_state_reset(&current_state->style, NULL, NULL);
  parse_state_reset(NULL, &current_state->align, NULL);
  parse_state_reset(NULL, NULL, &current_state->base);

  return 0;
}

/**
 * Delete all previous states.
 *
 * @return non-zero value if an error occurred.
 */
int parse_state_delete_all(void)
{
  while(current_state) {
    parse_state_delete(current_state);
  }

  return 0;
}

/**
 * Copy the state values from the current state to the given structs.
 *
 * @param style A pointer to where to store text style information. A NULL
 * @param style value means the style will not be retreived.
 * @param align A pointer to where to store part align information. A NULL
 * @param align value means the alignment will not be retreived.
 * @param base A pointer to a layout part which is the base of parts
 * @param base used by the current set of tags. This copies the 
 * @param base pointer, as opposed to the style and align which copy
 * @param base the actual structs.
 *
 * @return non-zero value if an error occurred.
 */
int parse_state_get_current(struct layout_text_styles *style,
			    struct layout_aligns *align,
			    struct layout_part **base)
{
  if(style)
    *style = current_state->style;
  
  if(align)
    *align = current_state->align;

  if(base)
    *base = current_state->base;

  return 0;
}

/**
 * Copy the style and align values into a new state and place that first 
 * on the stack. Also sets the new mother of layout parts, if given.
 *
 * @param tagname A string containing the name of the tag that sets this
 * @param tagname tag. Used when popping the state back.
 * @param style A pointer to the style to store in the state. A NULL value
 * @param style means the style will be the same as the previous state.
 * @param align A pointer to the align to store in the state. A NULL value
 * @param align means the align will be the same as the previous state.
 * @param base A pointer to a layout part which is to be the base part
 * @param base of new parts added to the linked list.
 *
 * @return non-zero value if an error occurred.
 */
int parse_state_push(char *tagname,
		     struct layout_text_styles *style,
		     struct layout_aligns *align,
		     struct layout_part **base)
{
  struct parse_state *new_state = NULL;

  new_state = (struct parse_state *)malloc(sizeof(struct parse_state));
  if(new_state == NULL)
    return 1;

  new_state->tagname = (char *)malloc(strlen(tagname) + 1);
  if(new_state->tagname == NULL) {
    free(new_state);
    return 1;
  }

  strcpy(new_state->tagname, tagname);

  if(style)
    new_state->style = *style;
  else
    parse_state_get_current(&new_state->style, NULL, NULL);

  if(align)
    new_state->align = *align;
  else
    parse_state_get_current(NULL, &new_state->align, NULL);

  if(base)
    new_state->base = *base;
  else
    parse_state_get_current(NULL, NULL, &new_state->base);

  new_state->previous = current_state;
  current_state = new_state;

  /* We need special treatment of the preformatted style, because this
   * inflicts on the parsing.
   */
  parse_string_set_preformatted(current_state->style.preformatted);

  return 0;
}

/**
 * Remove the first state from the stack, no matter what there is inside it.
 * Only do this if we do not pop over a set state mark.
 *
 * @member pop_style Non-zero value if popping the current style.
 * @member pop_align Non-zero value if popping the current align.
 * @member pop_base Non-zero value if popping the current base.
 *
 * @return non-zero value if an error occurred.
 */
int parse_state_pop(char *tagname, int delete_nested)
{
  struct parse_state *statep, *old_statep;

  if(current_state == NULL) {
    fprintf(stderr, "%s(%d): Trying to pop from empty stack.\n",
	    __FUNCTION__, __LINE__);
    return 1;
  }

  /* Search backwards for the given tagname. If it cannot be found, 
   * There is definitely something wrong with the page. If another
   * tagname is found before the one we search for, there is a nested
   * error in the page. In either case, we print some messages to let
   * the nice user know this. 
   */
  if(!strcmp(current_state->tagname, tagname)) {
    parse_state_delete(current_state);
  } else {
    statep = current_state;
    while(statep && strcmp(statep->tagname, tagname) != 0) {
      fprintf(stderr, "Warning! Nested tags! Popping '%s' right past '%s'.\n",
	      tagname, statep->tagname);
      old_statep = statep;
      statep = statep->previous;
      if(delete_nested) {
	fprintf(stderr, "And deleting it.\n");      
	parse_state_delete(old_statep);
      }
    }
    if(statep == NULL) {
      fprintf(stderr, "Warning! Cannot find tagname '%s' to pop.\n", tagname);
    } else {
      parse_state_delete(statep);
    }
  }

  /* If we happen to pop more than we push, we will come to a position
   * where current_state is NULL again. This cannot be allowed, and to
   * easily recover from this problem, we simply reinitialize it.
   */
  if(current_state == NULL) {
    fprintf(stderr, "Warning! Initialized the state more than once!\n");
    parse_state_init();
  }

  /* We need special treatment of the preformatted style, because this
   * inflicts on the parsing.
   */
  parse_string_set_preformatted(current_state->style.preformatted);

  return 0;
}

/**
 * Return the name of the top state.
 *
 * @return the name of the top state. This string is not copied, so
 * @return it should never ever be written to.
 */
char *parse_state_peek(void)
{
  return current_state->tagname;
}




