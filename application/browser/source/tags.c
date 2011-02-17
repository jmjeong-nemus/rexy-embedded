/**
 * Functions which are called when the parser comes upon a tag.
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

#include "../header/parser_helpers.h"
#include "../header/tags.h"
#include "../header/states.h"
#include "../header/layout.h"
#include "../header/ui.h"
#include "../header/info.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

extern int strcasecmp(const char *s1, const char *s2);

/**
 * Interpret and handle a body tag.
 * These tags are supported by this function:
 * <body> - Body part of page
 *
 * Currently supported parameters:
 * bgcolor - The background colour of the page.
 * text - The colour of the normal text.
 * link - The colour of unvisited links.
 * alink - The colour of active links, whatever that is.
 * vlink - The colour of visited links.
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_body(struct parse_tag *tagp)
{
  struct layout_part *partp;
  struct layout_aligns align;

  parse_string_store_current();

  switch(tagp->type) {
  case PARSE_TAG_START:
    {
      partp = layout_init_part(LAYOUT_PART_PARAGRAPH);
      if(partp == NULL) {
	return 1;
      }
      parse_state_get_current(NULL, &align, NULL);
      align.horizontal = LAYOUT_PART_ALIGN_LEFT;
      parse_state_push(tagp->name, NULL, &align, NULL);
      layout_add_part(partp);

      /* Get the page information part, and fill it up with values. */
      partp = layout_get_page_information(NULL);
      if(partp != NULL) {
	char *param_value;
	
	param_value = parse_get_param_value(tagp->first, "bgcolor");
	if(param_value)
	  partp->data.page_information.background_colour = 
	    parse_convert_colour(param_value);

	param_value = parse_get_param_value(tagp->first, "text");
	if(param_value)
	  partp->data.page_information.text_colour = 
	    parse_convert_colour(param_value);

	param_value = parse_get_param_value(tagp->first, "link");
	if(param_value)
	  partp->data.page_information.link_colour = 
	    parse_convert_colour(param_value);
	  
	param_value = parse_get_param_value(tagp->first, "alink");
	if(param_value)
	  partp->data.page_information.active_link_colour = 
	    parse_convert_colour(param_value);
	  
	param_value = parse_get_param_value(tagp->first, "vlink");
	if(param_value)
	  partp->data.page_information.visited_link_colour = 
	    parse_convert_colour(param_value);
      }

      break;
    }
  
  case PARSE_TAG_END:
    {
      break;
    }

  default:
    return 1;
  }

  return 0;
}


/**
 * Interpret and handle a title tag.
 * These tags are supported by this function:
 * <title> - The title of the page 
 *
 * This tag has no parameters.
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_title(struct parse_tag *tagp)
{
  switch(tagp->type) {
  case PARSE_TAG_START:
    {
      parse_string_store_current();
      break;
    }
  
  case PARSE_TAG_END:
    {
      /* Put the page title first in the internal list. This is done by
       * the function we are about to call. 
       */
      parse_string_store_title();
      break;
    }

  default:
    return 1;
  }

  return 0;
}

/**
 * Interpret and throw away a script tag.
 * These tags are supported by this function:
 * <script> - 
 *
 * Currently supported parameters:
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_script(struct parse_tag *tagp)
{
  switch(tagp->type) {
  case PARSE_TAG_START:
    {
      parse_string_store_current();
      break;
    }
  
  case PARSE_TAG_END:
    {
      /* Just throw away anything that is within a script container. */
      parse_string_discard();
      break;
    }

  default:
    return 1;
  }

  return 0;
}

/**
 * Interpret and handle a base tag.
 * These tags are supported by this function:
 * <base> - Change the base URL for this page
 *
 * Currently supported parameters:
 * href - The hyperlink reference for the new base URL.
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_base(struct parse_tag *tagp)
{
  struct layout_part *partp;

  /* Get the page information part, and fill it up with values. */
  partp = layout_get_page_information(NULL);
  if(partp != NULL) {
    char *param_value, *href;
	
    param_value = parse_get_param_value(tagp->first, "href");
    if(param_value != NULL) {
      if(strlen(param_value) > 0) {
	href = malloc(strlen(param_value) + 1);
	if(href == NULL) {
	  return 1;
	}
	strcpy(href, param_value);
	partp->data.page_information.base_url = href;
      }
    }
  }

  return 0;
}

/**
 * Interpret paragraph tags.
 * These tags are supported by this function:
 * <br>  - Break row
 * <p>  - New paragraph
 * <center> - Centered paragraph
 *
 * Currently supported parameters:
 * align - (p only) Alignment of this paragraph.
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_paragraph(struct parse_tag *tagp)
{
  struct layout_part *partp;
  struct layout_aligns align;

  parse_string_store_current();

  switch(tagp->type) {
  case PARSE_TAG_START:
    {
      partp = layout_init_part(LAYOUT_PART_PARAGRAPH);
      if(partp == NULL) {
	return 1;
      }

      parse_state_get_current(NULL, &align, NULL);
      
      if(!strcmp(tagp->name, "p")) {
	char *align_text;
	int valid_align;

	valid_align = 1;

	align_text = parse_get_param_value(tagp->first, "align");
	if(align_text) {
	  if(!strcasecmp(align_text, "left"))
	    align.horizontal = LAYOUT_PART_ALIGN_LEFT;
	  else if(!strcasecmp(align_text, "center"))
	    align.horizontal = LAYOUT_PART_ALIGN_CENTER;
	  else if(!strcasecmp(align_text, "right"))
	    align.horizontal = LAYOUT_PART_ALIGN_RIGHT;
	  else if(!strcasecmp(align_text, "justify"))
	    align.horizontal = LAYOUT_PART_ALIGN_JUSTIFY;
	  else 
	    valid_align = 0;
	} else if(align.horizontal != LAYOUT_PART_ALIGN_FORCED_CENTER) {
	  align.horizontal = LAYOUT_PART_ALIGN_LEFT;
	} else {
	  valid_align = 0;
	}
	partp->data.paragraph.paragraph = 1;

	/* Do not push a new state, if we are in forced center mode, and the
	 * paragraph tag did not order a new alignment. Also, pop the
	 * current state before this, if it was another <p>-tag.
	 */
	if(valid_align) {
	  if(!strcmp(parse_state_peek(), tagp->name))
	    parse_state_pop(tagp->name, 0);
	  parse_state_push(tagp->name, NULL, &align, NULL);
	}
      } else if(!strcmp(tagp->name, "center")) {
	align.horizontal = LAYOUT_PART_ALIGN_FORCED_CENTER;
	partp->data.paragraph.paragraph = 1;
	parse_state_push(tagp->name, NULL, &align, NULL);
      }

      layout_add_part(partp);
      break;
    }

  case PARSE_TAG_END:
    {
      /* There is no such thing as an ending br-tag, so we just stop
       * right here, if some joker has tried to use one.
       */
      if(!strcmp(tagp->name, "br"))
	 break;

      /* If we are inside a forced center, and this is an ending p-tag, 
       * we should not pop the state here.
       */
      parse_state_get_current(NULL, &align, NULL);
      if(align.horizontal == LAYOUT_PART_ALIGN_FORCED_CENTER &&
	 !strcmp(tagp->name, "center"))
	parse_state_pop(tagp->name, 0);
      else if(align.horizontal != LAYOUT_PART_ALIGN_FORCED_CENTER)
	parse_state_pop(tagp->name, 0);

      partp = layout_init_part(LAYOUT_PART_PARAGRAPH);
      if(partp == NULL) {
	return 1;
      }
      if(!strcmp(tagp->name, "center")) {
	partp->data.paragraph.paragraph = 1;
      }
      layout_add_part(partp);
      break;
    }

  default:
    return 1;
  }

  return 0;
}

/**
 * Interpret division tag.
 * These tags are supported by this function:
 * <div> - Division settings
 *
 * Currently supported parameters:
 * align - Alignment of parts.
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_division(struct parse_tag *tagp)
{
  struct layout_aligns align;

  parse_string_store_current();

  switch(tagp->type) {
  case PARSE_TAG_START:
    {
      char *align_text;

      parse_state_get_current(NULL, &align, NULL);
      
      align_text = parse_get_param_value(tagp->first, "align");
      if(align_text) {
	if(!strcasecmp(align_text, "left"))
	  align.horizontal = LAYOUT_PART_ALIGN_LEFT;
	else if(!strcasecmp(align_text, "center"))
	  align.horizontal = LAYOUT_PART_ALIGN_CENTER;
	else if(!strcasecmp(align_text, "right"))
	  align.horizontal = LAYOUT_PART_ALIGN_RIGHT;
	else if(!strcasecmp(align_text, "justify"))
	  align.horizontal = LAYOUT_PART_ALIGN_JUSTIFY;
      } else if(align.horizontal != LAYOUT_PART_ALIGN_FORCED_CENTER) {
	align.horizontal = LAYOUT_PART_ALIGN_LEFT;
      }
      parse_state_push(tagp->name, NULL, &align, NULL);
      break;
    }

  case PARSE_TAG_END:
    {
      parse_state_pop(tagp->name, 1);
      break;
    }

  default:
    return 1;
  }

  return 0;
}

/**
 * Interpret the table tag.
 * These tags are supported by this function:
 * <table> - Table outline
 *
 * Currently supported parameters:
 * border - The thickness of the border in pixels.
 * cellpadding - The space between the table border and the content.
 * cellspacing - The space between table cells.
 * width - The width of the whole table.
 * height - The height of the whole table (unused).
 * bgcolor - The background colour to use in table cells.
 * align - The alignment of the whole table.
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_table(struct parse_tag *tagp)
{
  parse_string_store_current();

  switch(tagp->type) {
  case PARSE_TAG_START:
    {
      struct layout_part *partp;
      char *param_value;

      /* If the user interface know how to handle tables, we make it,
       * otherwise, we simple make a new paragraph here.
       */
      if(user_interface.ui_support.table) {
	/* Initialize the part to be placed in the main part list. */
	partp = layout_init_part(LAYOUT_PART_TABLE);
	if(partp == NULL) {
	  return 1;
	}

	param_value = parse_get_param_value(tagp->first, "border");
	if(param_value)
	  partp->data.table.border = atoi(param_value);

	param_value = parse_get_param_value(tagp->first, "cellpadding");
	if(param_value)
	  partp->data.table.cellpadding = atoi(param_value);

	param_value = parse_get_param_value(tagp->first, "cellspacing");
	if(param_value)
	  partp->data.table.cellspacing = atoi(param_value);

	param_value = parse_get_param_value(tagp->first, "bgcolor");
	if(param_value)
	  partp->data.table.background_colour =
	    parse_convert_colour(param_value);

	param_value = parse_get_param_value(tagp->first, "width");
	if(param_value != NULL) {
	  partp->data.table.width = atoi(param_value);
	  if(partp->data.table.width <= 0)
	    partp->data.table.width = 1;
	  if(strchr(param_value, '%'))
	    partp->data.table.width_type = LAYOUT_SIZE_PERCENT;
	  else if(strchr(param_value, '*'))
	    partp->data.table.width_type = LAYOUT_SIZE_PART;
	  else
	    partp->data.table.width_type = LAYOUT_SIZE_ABSOLUTE;
	}

	param_value = parse_get_param_value(tagp->first, "height");
	if(param_value != NULL) {
	  partp->data.table.height = atoi(param_value);
	  if(partp->data.table.height <= 0)
	    partp->data.table.height = 1;
	  if(strchr(param_value, '%'))
	    partp->data.table.height_type = LAYOUT_SIZE_PERCENT;
	  else if(strchr(param_value, '*'))
	    partp->data.table.height_type = LAYOUT_SIZE_PART;
	  else
	    partp->data.table.height_type = LAYOUT_SIZE_ABSOLUTE;
	}

	layout_add_part(partp);

	/* The alignment must be set after it has been added, since the
	 * layout_add_part() function will set it first.
	 */
	param_value = parse_get_param_value(tagp->first, "align");
	if(param_value) {
	  if(!strcasecmp(param_value, "left"))
	    partp->align.horizontal = LAYOUT_PART_ALIGN_LEFT;
	  else if(!strcasecmp(param_value, "center"))
	    partp->align.horizontal = LAYOUT_PART_ALIGN_CENTER;
	  else if(!strcasecmp(param_value, "right"))
	    partp->align.horizontal = LAYOUT_PART_ALIGN_RIGHT;
	  else if(!strcasecmp(param_value, "justify"))
	    partp->align.horizontal = LAYOUT_PART_ALIGN_JUSTIFY;
	}   	

	/* The whole table goes into a subsection, so everything within
	 * the table will end up as child parts of this part.
	 */
	parse_state_push(tagp->name, NULL, NULL, &partp);
      } else {
	partp = layout_init_part(LAYOUT_PART_PARAGRAPH);
	if(partp == NULL) {
	  return 1;
	}
	partp->data.paragraph.paragraph = 1;
	layout_add_part(partp);
      }
      break;
    }

  case PARSE_TAG_END:
    {
      struct layout_part *partp;

      if(user_interface.ui_support.table) {
	/* Unset a mark in the state handler, because we should reset 
	 * everything after the table is done.
	 * This will also take care of ending table cells and table rows 
	 * which have not yet been closed.
	 * But all this, only if we are inside a table row, table cell, or
	 * the table itself.
	 */
	parse_state_get_current(NULL, NULL, &partp);
	if(partp && 
	   (partp->type == LAYOUT_PART_TABLE_CELL ||
	    partp->type == LAYOUT_PART_TABLE_ROW ||
	    partp->type == LAYOUT_PART_TABLE))
	  parse_state_pop(tagp->name, 1);
      } else {
	/* Always begin on a new line after a table. */
	partp = layout_init_part(LAYOUT_PART_PARAGRAPH);
	if(partp == NULL) {
	  return 1;
	}
	partp->data.paragraph.paragraph = 1;
	layout_add_part(partp);
      }

      break;
    }

  default:
    return 1;
  }

  return 0;
}

/**
 * Interpret the table row tag. 
 * These tags are supported by this function:
 * <tr> - Table row
 *
 * Currently supported parameters:
 * bgcolor - The background colour to use in table cells.
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_table_row(struct parse_tag *tagp)
{
  parse_string_store_current();

  switch(tagp->type) {
  case PARSE_TAG_START:
    {
      struct layout_part *partp, *table;
      char *param_value;

      /* If the user interface know how to handle tables, we make it,
       * otherwise, we simple make a new row break here.
       */
      if(user_interface.ui_support.table) {
	/* If we are still inside another table row here, it means that
	 * the HTML page does not use the ending table row tags. We
	 * therefore end it here, and go on as usual with the next row.
	 * If we are inside a table cell, we need to leave that first.
	 */
	parse_state_get_current(NULL, NULL, &partp);
	if(partp && partp->type == LAYOUT_PART_TABLE_CELL) {
	  parse_state_pop("td", 1);
	  parse_state_get_current(NULL, NULL, &partp);
	}
	if(partp && partp->type == LAYOUT_PART_TABLE_ROW) {
	  parse_state_pop("tr", 1);
	}

	/* Initialize the part to be placed in the main part list. */
	partp = layout_init_part(LAYOUT_PART_TABLE_ROW);
	if(partp == NULL) {
	  return 1;
	}

	/* If a background colour is specified, use that for future
	 * table cells, otherwise set the background colour of the table.
	 */
	param_value = parse_get_param_value(tagp->first, "bgcolor");
	if(param_value) {
	  partp->data.table_row.background_colour =
	    parse_convert_colour(param_value);
	} else {
	  parse_state_get_current(NULL, NULL, &table);
	  if(table && table->type == LAYOUT_PART_TABLE)
	    partp->data.table_row.background_colour = 
	      table->data.table.background_colour;
	}
	
	layout_add_part(partp);

	/* Everything within this particular table row will be child parts
	 * of this part.
	 */
	parse_state_push(tagp->name, NULL, NULL, &partp);
      } else {
	partp = layout_init_part(LAYOUT_PART_PARAGRAPH);
	if(partp == NULL) {
	  return 1;
	}
	layout_add_part(partp);
      }
      break;
    }

  case PARSE_TAG_END:
    {
      struct layout_part *partp;

      if(user_interface.ui_support.table) {

	/* If the current base is not a table row, we should not end
	 * the current.
	 */
	parse_state_get_current(NULL, NULL, &partp);
	if(partp && partp->type == LAYOUT_PART_TABLE_ROW)
	  parse_state_pop(tagp->name, 1);
      }
      break;
    }

  default:
    return 1;
  }

  return 0;
}

/**
 * Interpret the table cell tags.
 * These tags are supported by this function:
 * <th> - Table header cell
 * <td> - Table data cell
 *
 * Currently supported parameters:
 * colspan - The number of columns this table cell uses in the table.
 * rowspan - The number of rows this table cell uses in the table.
 * width - The width of the table cell.
 * height - The height of the table cell (unused).
 * align - The horizontal alignment of the content of the table cell.
 * valign - The vertical alignment of the content of the table cell.
 * bgcolor - The background colour to use in this table cell.
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_table_cell(struct parse_tag *tagp)
{
  parse_string_store_current();

  switch(tagp->type) {
  case PARSE_TAG_START:
    {
      struct layout_part *partp, *table_row;
      struct layout_aligns align;
      struct layout_text_styles style;
      char *param_value;

      /* If the user interface know how to handle tables, we make it,
       * otherwise, we simple make a new paragraph here.
       */
      if(user_interface.ui_support.table) {
	/* If we are still inside another table cell here, it means that
	 * the HTML page does not use the ending table cell tags. We
	 * therefore end it here, and go on as usual with the next cell.
	 */
	parse_state_get_current(NULL, NULL, &partp);
	if(partp && partp->type == LAYOUT_PART_TABLE_CELL) {
	  parse_state_pop("td", 1);
	}

	/* Initialize the part to be placed in the main part list. */
	partp = layout_init_part(LAYOUT_PART_TABLE_CELL);
	if(partp == NULL) {
	  return 1;
	}

	param_value = parse_get_param_value(tagp->first, "colspan");
	if(param_value)
	  partp->data.table_cell.colspan = atoi(param_value);

	param_value = parse_get_param_value(tagp->first, "rowspan");
	if(param_value)
	  partp->data.table_cell.rowspan = atoi(param_value);

	/* If a background colour is specified, use that for future
	 * table cells, otherwise set the background colour of the table.
	 */
	param_value = parse_get_param_value(tagp->first, "bgcolor");
	if(param_value) {
	  partp->data.table_cell.background_colour =
	    parse_convert_colour(param_value);
	} else {
	  parse_state_get_current(NULL, NULL, &table_row);
	  if(table_row && table_row->type == LAYOUT_PART_TABLE_ROW)
	    partp->data.table_cell.background_colour = 
	      table_row->data.table_row.background_colour;
	}

	param_value = parse_get_param_value(tagp->first, "width");
	if(param_value != NULL) {
	  partp->data.table_cell.width = atoi(param_value);
	  if(partp->data.table_cell.width <= 0)
	    partp->data.table_cell.width = 1;
	  if(strchr(param_value, '%'))
	    partp->data.table_cell.width_type = LAYOUT_SIZE_PERCENT;
	  else if(strchr(param_value, '*'))
	    partp->data.table_cell.width_type = LAYOUT_SIZE_PART;
	  else
	    partp->data.table_cell.width_type = LAYOUT_SIZE_ABSOLUTE;
	}

	param_value = parse_get_param_value(tagp->first, "height");
	if(param_value != NULL) {
	  partp->data.table_cell.height = atoi(param_value);
	  if(partp->data.table_cell.height <= 0)
	    partp->data.table_cell.height = 1;
	  if(strchr(param_value, '%'))
	    partp->data.table_cell.height_type = LAYOUT_SIZE_PERCENT;
	  else if(strchr(param_value, '*'))
	    partp->data.table_cell.height_type = LAYOUT_SIZE_PART;
	  else
	    partp->data.table_cell.height_type = LAYOUT_SIZE_ABSOLUTE;
	}

	/* This is the last piece of the table puzzle. As child part of the
	 * cell will be the actual text, images, or something else.
	 */

	layout_add_part(partp);

      } else {
	/* This is for user interface which do not support tables. */

	partp = layout_init_part(LAYOUT_PART_TEXT);
	if(partp == NULL) {
	  return 1;
	}
	partp->data.text.text = (char *)malloc(2);
	if(partp->data.text.text == NULL) {
	  return 1;
	}
	strcpy(partp->data.text.text, " ");
	parse_state_get_current(&style, NULL, NULL);
	partp->data.text.style = style;

	layout_add_part(partp);
      }

      /* Use default states within the table. */
      parse_state_reset(&style, &align, NULL);
      
      /* Table column headers go in a very centristic and boldly style. */
      if(!strcmp(tagp->name, "th")) {
	style.bold = 1;
	align.horizontal = LAYOUT_PART_ALIGN_CENTER;
      }

      /* Let us see if the HTML writer has anything to say about 
       * the alignment. 
       */
      param_value = parse_get_param_value(tagp->first, "align");
      if(param_value) {
	if(!strcasecmp(param_value, "left"))
	  align.horizontal = LAYOUT_PART_ALIGN_LEFT;
	else if(!strcasecmp(param_value, "center"))
	  align.horizontal = LAYOUT_PART_ALIGN_CENTER;
	else if(!strcasecmp(param_value, "right"))
	  align.horizontal = LAYOUT_PART_ALIGN_RIGHT;
	else if(!strcasecmp(param_value, "justify"))
	  align.horizontal = LAYOUT_PART_ALIGN_JUSTIFY;
      }   	
      param_value = parse_get_param_value(tagp->first, "valign");
      if(param_value) {
	if(!strcasecmp(param_value, "top"))
	  align.vertical = LAYOUT_PART_ALIGN_TOP;
	else if(!strcasecmp(param_value, "middle"))
	  align.vertical = LAYOUT_PART_ALIGN_MIDDLE;
	else if(!strcasecmp(param_value, "bottom"))
	  align.vertical = LAYOUT_PART_ALIGN_BOTTOM;
      }

      /* If the user interface supports tables, the parts within the table
       * should go as child parts to this one.
       * We always give this little push the name "td", so that we can be
       * sure to pop it again in the <tr> tag, if necessary.
       */
      if(user_interface.ui_support.table) {
	parse_state_push("td", &style, &align, &partp);
      } else {
	parse_state_push("td", &style, &align, NULL);
      }

      break;
    }

  case PARSE_TAG_END:
    {
      struct layout_part *partp;

      if(user_interface.ui_support.table) {

	/* If the current base is not a table cell, we should not end
	 * the current.
	 */
	parse_state_get_current(NULL, NULL, &partp);
	if(partp && partp->type == LAYOUT_PART_TABLE_CELL)
	  parse_state_pop("td", 1);
      } else {
	parse_state_pop("td", 1);
      }

      break;
    }

  default:
    return 1;
  }

  return 0;
}

int parse_tag_function_span(struct parse_tag *tagp)
{
	return 0;
}
/**
 * Interpret list tags.
 * These tags are supported by this function:
 * <ol> - Ordered list
 * <ul> - Unordered list
 *
 * Currently supported parameters:
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_list(struct parse_tag *tagp)
{
  struct layout_part *partp;
  struct layout_aligns align;
  int old_indent_offset;

  parse_string_store_current();

  switch(tagp->type) {
  case PARSE_TAG_START:
    {
      /* This is an estimation from my side. It should probably
       * be up to the interface in the end, to decide the indentation
       * offset to be used for lists. For now, set it to 40 pixels. 
       *
       * In any case, we must do this before creating the paragraph
       * layout part, because the layouter expects it to be the so.
       */
      parse_state_get_current(NULL, &align, NULL);
      old_indent_offset = align.indent_offset;
	  // modified by wichang
      align.indent_offset += 10;
	  //
      parse_state_push(tagp->name, NULL, &align, NULL);

      partp = layout_init_part(LAYOUT_PART_PARAGRAPH);
      if(partp == NULL) {
	return 1;
      }
      if(old_indent_offset == 0)
	partp->data.paragraph.paragraph = 1;
      layout_add_part(partp);

      break;
    }

  case PARSE_TAG_END:
    {
      parse_state_pop(tagp->name, 0);
      parse_state_get_current(NULL, &align, NULL);

      partp = layout_init_part(LAYOUT_PART_PARAGRAPH);
      if(partp == NULL) {
	return 1;
      }
      if(align.indent_offset == 0)
	partp->data.paragraph.paragraph = 1;
      layout_add_part(partp);

      break;
    }

  default:
    return 1;
  }

  return 0;
}

/**
 * Interpret list item tag.
 * This tag are supported by this function:
 * <li> - Break row for now
 *
 * Currently supported parameters:
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_list_item(struct parse_tag *tagp)
{
  struct layout_part *partp;

  if(tagp->type == PARSE_TAG_START) {
    parse_string_store_current();
    
    partp = layout_init_part(LAYOUT_PART_PARAGRAPH);
    if(partp == NULL) {
      return 1;
    }
    layout_add_part(partp);
  }  

  return 0;
}

/**
 * Interpret header tags.
 * These tags are supported by this function:
 * <h1> - Header, type 1
 * <h2> - Header, type 2
 * <h3> - Header, type 3
 * <h4> - Header, type 4
 * <h5> - Header, type 5
 * <h6> - Header, type 6
 *
 * Currently supported parameters:
 * align - The horizontal alignment of the header.
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_header(struct parse_tag *tagp)
{
  struct layout_part *partp;

  parse_string_store_current();

  switch(tagp->type) {
  case PARSE_TAG_START:
    {
      struct layout_aligns align;
      struct layout_text_styles style;
      char *align_text;

      parse_state_get_current(&style, &align, NULL);

      partp = layout_init_part(LAYOUT_PART_PARAGRAPH);
      if(partp == NULL) {
	return 1;
      }
      partp->data.paragraph.paragraph = 1;
      layout_add_part(partp);

      align_text = parse_get_param_value(tagp->first, "align");
      if(align_text) {
	if(!strcasecmp(align_text, "left"))
	  align.horizontal = LAYOUT_PART_ALIGN_LEFT;
	else if(!strcasecmp(align_text, "center"))
	  align.horizontal = LAYOUT_PART_ALIGN_CENTER;
	else if(!strcasecmp(align_text, "right"))
	  align.horizontal = LAYOUT_PART_ALIGN_RIGHT;
      } else {
	if(align.horizontal != LAYOUT_PART_ALIGN_FORCED_CENTER)
	  align.horizontal = LAYOUT_PART_ALIGN_LEFT;
      }
    switch(tagp->name[1]) {
      case '1':
	style.size += 5;
	break;
      case '2':
	style.size += 4;
	break;
      case '3':
	style.size += 3;
	break;
      case '4':
	style.size += 2;
	break;
      case '5':
	style.size += 1;
	break;
      case '6':
	style.italic = 1;
	break;
      }
      style.bold = 1;

      parse_state_push(tagp->name, &style, &align, NULL);
      break;
    }

  case PARSE_TAG_END:
	{
      parse_state_pop(tagp->name, 0);
      partp = layout_init_part(LAYOUT_PART_PARAGRAPH);
      if(partp == NULL) {
		return 1;
      }
      break;
    }
  default:
    return 1;
  }

  return 0;
}

/**
 * Interpret an underline tag.
 * These tags are supported by this function:
 * <u> - Underlined text
 *
 * This tag has no parameters.
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_underline(struct parse_tag *tagp)
{
  parse_string_store_current();

  switch(tagp->type) {
  case PARSE_TAG_START:
    {
      struct layout_text_styles style;

      parse_state_get_current(&style, NULL, NULL);
      style.underlined = 1;
      parse_state_push(tagp->name, &style, NULL, NULL);
      break;
    }

  case PARSE_TAG_END:
    {
      parse_state_pop(tagp->name, 0);
      break;
    }

  default:
    return 1;
  }

  return 0;
}

/**
 * Interpret the italicalizating tag.
 * These tags are supported by this function:
 * <i> - Italic text
 * <em> - Emphasised text
 * <address> - Text representing an address
 *
 * This tag has no parameters.
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_italic(struct parse_tag *tagp)
{
  parse_string_store_current();

  switch(tagp->type) {
  case PARSE_TAG_START:
    {
      struct layout_text_styles style;

      parse_state_get_current(&style, NULL, NULL);
      style.italic = 1;
      parse_state_push(tagp->name, &style, NULL, NULL);
      break;
    }

  case PARSE_TAG_END:
    {
      parse_state_pop(tagp->name, 0);
      break;
    }

  default:
    return 1;
  }

  return 0;
}

/**
 * Interpret the bold and the beautiful tag.
 * These tags are supported by this function:
 * <b> - Bold text
 * <strong> - Strong text
 *
 * This tag has no parameters.
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_bold(struct parse_tag *tagp)
{
  parse_string_store_current();

  switch(tagp->type) {
  case PARSE_TAG_START:
    {
      struct layout_text_styles style;

      parse_state_get_current(&style, NULL, NULL);
      style.bold = 1;
      parse_state_push(tagp->name, &style, NULL, NULL);
      break;
    }

  case PARSE_TAG_END:
    {
      parse_state_pop(tagp->name, 0);
      break;
    }

  default:
    return 1;
  }

  return 0;
}

/**
 * Interpret the big tag which makes text a little bigger.
 * These tags are supported by this function:
 * <big> - Big text
 *
 * This tag has no parameters.
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_big(struct parse_tag *tagp)
{
  parse_string_store_current();

  switch(tagp->type) {
  case PARSE_TAG_START:
    {
      struct layout_text_styles style;

      parse_state_get_current(&style, NULL, NULL);
      style.size += 2;
      parse_state_push(tagp->name, &style, NULL, NULL);
      break;
    }

  case PARSE_TAG_END:
    {
      parse_state_pop(tagp->name, 0);
      break;
    }

  default:
    return 1;
  }

  return 0;
}

/**
 * Interpret the small tag which makes text just a bit smaller.
 * These tags are supported by this function:
 * <small> - Small text
 *
 * This tag has no parameters.
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_small(struct parse_tag *tagp)
{
  parse_string_store_current();

  switch(tagp->type) {
  case PARSE_TAG_START:
    {
      struct layout_text_styles style;

      parse_state_get_current(&style, NULL, NULL);
      style.size -= 2;
      parse_state_push(tagp->name, &style, NULL, NULL);
      break;
    }

  case PARSE_TAG_END:
    {
      parse_state_pop(tagp->name, 0);
      break;
    }

  default:
    return 1;
  }

  return 0;
}

/**
 * Interpret the font tag and all what comes with it. Currently, only
 * the size is supported.
 * These tags are supported by this function:
 * <font> - Change font properties
 *
 * Currently supported parameters:
 * size - The size of the font, absolute or relative.
 * color - The american colour of the font.
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_font(struct parse_tag *tagp)
{
  parse_string_store_current();

  switch(tagp->type) {
  case PARSE_TAG_START:
    {
      char *param_value;
      struct layout_text_styles style;

      parse_state_get_current(&style, NULL, NULL);

      /* Take care of the size parameter, if it is given. */
      param_value = parse_get_param_value(tagp->first, "size");
      if(param_value) {
	/* Check if using relative or absolute font size. */
	if(param_value[0] == '+' || param_value[0] == '-') {
	  style.size += atoi(param_value);
	} else {
	  style.size = atoi(param_value);
	}

	if(style.size < user_interface.ui_settings.min_fontsize)
	  style.size = user_interface.ui_settings.min_fontsize;
	else if(style.size > user_interface.ui_settings.max_fontsize)
	  style.size = user_interface.ui_settings.max_fontsize;
      }

      /* The colour is next. */
      param_value = parse_get_param_value(tagp->first, "color");
      if(param_value) {
	style.colour = parse_convert_colour(param_value);
      }

      parse_state_push(tagp->name, &style, NULL, NULL);
      break;
    }

  case PARSE_TAG_END:
    {
      parse_state_pop(tagp->name, 0);
      break;
    }

  default:
    return 1;
  }

  return 0;
}

/**
 * Interpret an preformating tag.
 * These tags are supported by this function:
 * <pre> - Preformatted text
 *
 * This tag has no parameters.
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_preformat(struct parse_tag *tagp)
{
  struct layout_part *partp;

  parse_string_store_current();

  switch(tagp->type) {
  case PARSE_TAG_START:
    {
      struct layout_text_styles style;

      /* A text within a pre tag should begin at a new paragraph. */
      partp = layout_init_part(LAYOUT_PART_PARAGRAPH);
      if(partp == NULL) {
	return 1;
      }
      partp->data.paragraph.paragraph = 1;
      layout_add_part(partp);

      parse_state_get_current(&style, NULL, NULL);
      style.preformatted = 1;
      style.monospaced = 1;
      parse_state_push(tagp->name, &style, NULL, NULL);
      break;
    }

  case PARSE_TAG_END:
    {
      parse_state_pop(tagp->name, 0);

      /* The text right after a pre tag should begin at a new paragraph. */
      partp = layout_init_part(LAYOUT_PART_PARAGRAPH);
      if(partp == NULL) {
	return 1;
      }
      partp->data.paragraph.paragraph = 1;
      layout_add_part(partp);

      break;
    }

  default:
    return 1;
  }

  return 0;
}

/**
 * Interpret an anchor tag.
 * These tags are supported by this function:
 * <a> - Anchor
 *
 * Currently supported parameters:
 * href - The URL to link to.
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_anchor(struct parse_tag *tagp)
{
  parse_string_store_current();

  switch(tagp->type) {
  case PARSE_TAG_START:
    {
      char *param_value, *href;
      struct layout_part *partp;
      struct layout_text_styles style;

      /* If the current base already is a link part, we should end
       * that one right here. Why? Because Netscape says so.
       */
      parse_state_get_current(NULL, NULL, &partp);
      if(partp && partp->type == LAYOUT_PART_LINK)
	parse_state_pop(tagp->name, 0);

      /* Get the href URL for the link. */
      href = NULL;
      param_value = parse_get_param_value(tagp->first, "href");
      if(param_value != NULL) {
	if(strlen(param_value) > 0) {
	  href = malloc(strlen(param_value) + 1);
	  if(href == NULL) {
	    return 1;
	  }
	  strcpy(href, param_value);
	}
      } else {
	return 1;
      }

      /* Initialize the part to be placed in the main part list. */
      partp = layout_init_part(LAYOUT_PART_LINK);
      if(partp == NULL) {
	free(href);
	return 1;
      }

      partp->data.link.href = href;
      layout_add_part(partp);

      /* Set the text style for links. */
      parse_state_get_current(&style, NULL, NULL);
      style.underlined = 1;
      style.colour = 0x400000ff;

      /* Arrange so that every part which is within the anchor tag will
       * end up as a child tree to the original anchor part.
       * Push the new text style at the same time.
       */
      parse_state_push(tagp->name, &style, NULL, &partp);

      break;
    }

  case PARSE_TAG_END:
    {
      struct layout_part *partp;

      /* If the current base is not a link part, we should not end
       * the current.
       */
      parse_state_get_current(NULL, NULL, &partp);
      if(partp && partp->type == LAYOUT_PART_LINK)
	parse_state_pop(tagp->name, 0);

      break;
    }

  default:
    return 1;
  }

  return 0;
}

/**
 * Interpret a tag that is supposed to produce a horizontal line.
 * These tags are supported by this function:
 * <hr> - Horizontal line
 *
 * Currently supported parameters:
 * width - Width of line, absolute or relative.
 * size - The height of the line.
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_hr(struct parse_tag *tagp)
{
  char *param_value;
  struct layout_part *partp;

  parse_string_store_current();

  partp = layout_init_part(LAYOUT_PART_LINE);
  if(partp == NULL) {
    return 1;
  }

  param_value = parse_get_param_value(tagp->first, "width");
  if(param_value != NULL) {
    partp->data.line.width = atoi(param_value);
    if(!strchr(param_value, '%'))
      partp->data.line.absolute = 1;
  }

  param_value = parse_get_param_value(tagp->first, "size");
  if(param_value != NULL) {
    partp->data.line.size = atoi(param_value);
  }

  layout_add_part(partp);

  /* Horizontal lines are always centered, as far as I know. 
   * We need to set this after we have added the part, since layout_add_part()
   * will set the alignment according to the current state.
   */
  partp->align.horizontal = LAYOUT_PART_ALIGN_CENTER;

  return 0;
}

/**
 * Find a long lost image floating around and take care of it as best
 * as we see fit.
 * These tags are supported by this function:
 * <img> - Embedded image
 *
 * Currently supported parameters:
 * width - Wanted width of image.
 * height - Wanted height of image.
 * alt - Alternative text for the image.
 * align - Vertical alignment of the image relative to the surrounding text.
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_image(struct parse_tag *tagp)
{
  char *alt_text, *src_text, *param_value;
  struct layout_part *partp;
  int width, height, border;

  /* If the user interface does not support images, we put the alternative
   * text for it on the internal string.
   */
  if(!user_interface.ui_support.image) {
    int i;

    param_value = parse_get_param_value(tagp->first, "alt");
    if(param_value == NULL || strlen(param_value) == 0) {
      parse_string_store_character('[');
      parse_string_store_character('I');
      parse_string_store_character('M');
      parse_string_store_character('A');
      parse_string_store_character('G');
      parse_string_store_character('E');
      parse_string_store_character(']');
    } else {
      for(i = 0 ; (unsigned char)i < strlen(param_value) ; i++)
	parse_string_store_character(param_value[i]);
    }
    return 0;
  }

  parse_string_store_current();

  /* Get the alternative text from the image tag, if any. */
  alt_text = NULL;
  param_value = parse_get_param_value(tagp->first, "alt");
  if(param_value != NULL) {
    if(strlen(param_value) > 0) {
      alt_text = malloc(strlen(param_value) + 1);
      if(alt_text == NULL)
	return 1;
      strcpy(alt_text, param_value);
    }
  }

  /* Get the source URL for the image. */
  src_text = NULL;
  param_value = parse_get_param_value(tagp->first, "src");
  if(param_value != NULL) {
    if(strlen(param_value) > 0) {
      src_text = malloc(strlen(param_value) + 1);
      if(src_text == NULL) {
	if(alt_text)
	  free(alt_text);
	return 1;
      }
      strcpy(src_text, param_value);
    }
  }

  /* Get the width and height of the image, if they are specified 
   * in the tag. 
   */
  param_value = parse_get_param_value(tagp->first, "width");
  if(param_value != NULL)
    width = atoi(param_value);
  else
    width = 0;
  param_value = parse_get_param_value(tagp->first, "height");
  if(param_value != NULL)
    height = atoi(param_value);
  else
    height = 0;

  /* Find out the border thickness, if there is one. */
  param_value = parse_get_param_value(tagp->first, "border");
  if(param_value != NULL)
    border = atoi(param_value);
  else
    border = 0;

  partp = layout_init_part(LAYOUT_PART_GRAPHICS);
  if(partp == NULL) {
    if(alt_text)
      free(alt_text);
    if(src_text)
      free(src_text);
    return 1;
  }

  /* Do not care about finding an image right now. */
  partp->data.graphics.data = NULL;
  partp->data.graphics.size = 0;
  partp->data.graphics.type = LAYOUT_PART_GRAPHICS_UNKNOWN;
  partp->data.graphics.alt_text = alt_text;
  partp->data.graphics.src = src_text;
  partp->data.graphics.border = border;
  partp->geometry.width = width;
  partp->geometry.height = height;
  layout_add_part(partp);

  /* Set the vertical alignment, if it is specified. */
  param_value = parse_get_param_value(tagp->first, "align");
  if(param_value != NULL) {
    if(!strcasecmp(param_value, "top"))
      partp->align.vertical = LAYOUT_PART_ALIGN_TOP;
    else if(!strcasecmp(param_value, "middle"))
      partp->align.vertical = LAYOUT_PART_ALIGN_MIDDLE;
    else if(!strcasecmp(param_value, "bottom"))
      partp->align.vertical = LAYOUT_PART_ALIGN_BOTTOM;
    else if(!strcasecmp(param_value, "left"))
	{
      partp->align.horizontal = LAYOUT_PART_ALIGN_LEFT;
	  partp->align.horzontal_image = LAYOUT_PART_ALIGN_IMAGE_LEFT;
	}
	else if(!strcasecmp(param_value, "center"))
	{
      partp->align.horizontal = LAYOUT_PART_ALIGN_CENTER;
	  partp->align.horzontal_image = LAYOUT_PART_ALIGN_IMAGE_CENTER;
	}
	else if(!strcasecmp(param_value, "right"))
	{
      partp->align.horizontal = LAYOUT_PART_ALIGN_RIGHT;
	  partp->align.horzontal_image = LAYOUT_PART_ALIGN_IMAGE_RIGHT;
	}
	else
	  partp->align.vertical = LAYOUT_PART_ALIGN_BOTTOM;

  }

  param_value = parse_get_param_value(tagp->first, "hspace");
  if(param_value != NULL)
	partp->data.graphics.hspace = atoi(param_value);
  else
    partp->data.graphics.hspace = 0;

  param_value = parse_get_param_value(tagp->first, "vspace");
  if(param_value != NULL)
	 partp->data.graphics.vspace = atoi(param_value);
  else
     partp->data.graphics.vspace = 0;

  return 0;
}

/**
 * Interpret a form tag.
 * These tags are supported by this function:
 * <form> - Basic form structure
 *
 * Currently supported parameters:
 * method - The request method to use for this form.
 * action - The URL to call on submit.
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_form(struct parse_tag *tagp)
{
  parse_string_store_current();

  switch(tagp->type) {
  case PARSE_TAG_START:
    {
      struct layout_part *partp;
      char *param_value, *action_text;

      action_text = NULL;
      param_value = parse_get_param_value(tagp->first, "action");
      if(param_value != NULL) {
	if(strlen(param_value) > 0) {
	  action_text = malloc(strlen(param_value) + 1);
	  if(action_text == NULL) {
	    return 1;
	  }
	  strcpy(action_text, param_value);
	}
      }

      /* Initialize the part to be placed in the main part list. */
      partp = layout_init_part(LAYOUT_PART_FORM);
      if(partp == NULL) {
	if(action_text)
	  free(action_text);
	return 1;
      }
      
      partp->data.form.action = action_text;
      
      param_value = parse_get_param_value(tagp->first, "method");
      if(param_value != NULL) {
	if(!strcasecmp(param_value, "get"))
	  partp->data.form.method = LAYOUT_PART_FORM_GET;
	else if(!strcasecmp(param_value, "post"))
	  partp->data.form.method = LAYOUT_PART_FORM_POST;
      }      
      
      layout_add_part(partp);
      
      /* Arrange so that every part which is within the form tag will
       * end up as a child tree to this part.
       */
      parse_state_push(tagp->name, NULL, NULL, &partp);

      break;
    }

  case PARSE_TAG_END:
    {
      struct layout_part *partp;

      /* If the current base is not a form part, we should not end
       * the current.
       */
      parse_state_get_current(NULL, NULL, &partp);
      if(partp && partp->type == LAYOUT_PART_FORM)
	parse_state_pop(tagp->name, 0);
      else
	fprintf(stderr, "Warning! Form end tag at wrong place.\n");

      break;
    }

  default:
    return 1;
  }

  return 0;
}

/**
 * Interpret an input tag in a form.
 * These tags are supported by this function:
 * <input> - Form input device
 *
 * Currently supported parameters:
 * type - The type of input widget.
 * name - The name of the input thingie.
 * value - The default value of the input gizmo.
 *
 * @param tagp The current tag and its parameters.
 *
 * @return non-zero value if an error occurred.
 */
int parse_tag_function_form_input(struct parse_tag *tagp)
{
  struct layout_part *partp;
  char *param_value, *input_type, *name, *value;

  parse_string_store_current();

  input_type = parse_get_param_value(tagp->first, "type");
  if(input_type == NULL) {
    return 1;
  }
      
  name = NULL;
  param_value = parse_get_param_value(tagp->first, "name");
  if(param_value != NULL) {
    if(strlen(param_value) > 0) {
      name = malloc(strlen(param_value) + 1);
      if(name == NULL) {
	return 1;
      }
      strcpy(name, param_value);
    }	
  }

  value = NULL;
  param_value = parse_get_param_value(tagp->first, "value");
  if(param_value != NULL) {
    value = malloc(strlen(param_value) + 1);
    if(value == NULL) {
      if(name)
	free(name);
      return 1;
    }
    strcpy(value, param_value);
  }

  partp = NULL;

  /* Check what kind of input gadget we are dealing with here. */

  /* Even though they are not exactly the same thing, we treat simple
   * buttons the same way as submit buttons. Simple buttons are only
   * used with Javascript, I think. 
   */
  if(!strcasecmp(input_type, "submit") ||
     !strcasecmp(input_type, "button")) {
    partp = layout_init_part(LAYOUT_PART_FORM_SUBMIT);
    if(partp == NULL) {
      if(value)
	free(value);
      if(name)
	free(name);
      return 1;
    }

    partp->data.form_submit.name = name;
    partp->data.form_submit.value = value;
  } else if(!strcasecmp(input_type, "checkbox")) {
    partp = layout_init_part(LAYOUT_PART_FORM_CHECKBOX);
    if(partp == NULL) {
      if(value)
	free(value);
      if(name)
	free(name);
      return 1;
    }

    partp->data.form_checkbox.name = name;
    partp->data.form_checkbox.value = value;

    /* Check if the checkbox should be prechecked. */
    param_value = parse_get_param_value(tagp->first, "checked");
    if(param_value != NULL)
      partp->data.form_checkbox.checked = 1;
  } else if(!strcasecmp(input_type, "radio")) {
    partp = layout_init_part(LAYOUT_PART_FORM_RADIO);
    if(partp == NULL) {
      if(value)
	free(value);
      if(name)
	free(name);
      return 1;
    }

    partp->data.form_radio.name = name;
    partp->data.form_radio.value = value;

    /* Check if this radio button should be prechecked. */
    param_value = parse_get_param_value(tagp->first, "checked");
    if(param_value != NULL)
      partp->data.form_radio.checked = 1;
  } else if(!strcasecmp(input_type, "hidden")) {
    partp = layout_init_part(LAYOUT_PART_FORM_HIDDEN);
    if(partp == NULL) {
      if(value)
	free(value);
      if(name)
	free(name);
      return 1;
    }

    partp->data.form_hidden.name = name;
    partp->data.form_hidden.value = value;
  } else {
    /* We do this like Netscape, if the type is unknown to us, 
     * we just set it to be a text field. 
     */
    partp = layout_init_part(LAYOUT_PART_FORM_TEXT);
    if(partp == NULL) {
      if(value)
	free(value);
      if(name)
	free(name);
      return 1;
    }

    partp->data.form_text.name = name;
    partp->data.form_text.value = value;

    param_value = parse_get_param_value(tagp->first, "size");
    if(param_value != NULL)
      partp->data.form_text.size = atoi(param_value);

    param_value = parse_get_param_value(tagp->first, "maxlength");
    if(param_value != NULL)
      partp->data.form_text.maxlength = atoi(param_value);
  }

  if(partp != NULL)
    layout_add_part(partp);

  return 0;
}



/**
 * An array of tag bindings, with all supported tags.
 */
struct parse_tag_binding parse_tag_bindings[] = {
  { "body", parse_tag_function_body },
  { "title", parse_tag_function_title },
  { "script", parse_tag_function_script },
  { "base", parse_tag_function_base },

  { "br", parse_tag_function_paragraph },
  { "p", parse_tag_function_paragraph },
  { "center", parse_tag_function_paragraph },

  { "div", parse_tag_function_division },

  { "table", parse_tag_function_table },
  { "tr", parse_tag_function_table_row },
  { "td", parse_tag_function_table_cell },
  { "th", parse_tag_function_table_cell },

  { "span", parse_tag_function_span},

  { "ol", parse_tag_function_list },
  { "ul", parse_tag_function_list },
  { "li", parse_tag_function_list_item },

  { "h1", parse_tag_function_header },
  { "h2", parse_tag_function_header },
  { "h3", parse_tag_function_header },
  { "h4", parse_tag_function_header },
  { "h5", parse_tag_function_header },
  { "h6", parse_tag_function_header },

  { "i", parse_tag_function_italic },
  { "em", parse_tag_function_italic },
  { "address", parse_tag_function_italic },
  { "u", parse_tag_function_underline },
  { "b", parse_tag_function_bold },
  { "strong", parse_tag_function_bold },

  { "font", parse_tag_function_font },
  { "big", parse_tag_function_big },
  { "small", parse_tag_function_small },

  { "pre", parse_tag_function_preformat },

  { "a", parse_tag_function_anchor },

  { "hr", parse_tag_function_hr },

  { "img", parse_tag_function_image },

  { "form", parse_tag_function_form },
  { "input", parse_tag_function_form_input },

  { NULL, NULL }
};

/**
 * Call the appropriate function bound to the tag.
 *
 * @param tagp The current tag and its parameters.
 *
 * @return the return value of the tag function, if it was found.
 * @return Otherwise a non-zero value.
 */
int parse_call_tag_binding(struct parse_tag *tagp)
{
  int index;

  for(index = 0 ; parse_tag_bindings[index].name ; index++) {
    if(!strcmp(parse_tag_bindings[index].name, tagp->name)) {
      return parse_tag_bindings[index].function(tagp);
    }
  }

  /* No binding was found for this tag. */
  return 1;
}
