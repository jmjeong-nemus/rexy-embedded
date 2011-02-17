/**
 * Functions used to do the actual laying out of all the parts that
 * were collected by the parser and put into the layout linked list.
 *
 * The layouting process is basically split up in four parts:
 * - Prepare parts, takes care of some parts, which needs a special
 *   treatment before the layouting can be done. 
 * - Set sizes, sets the size of those parts which do not yet have
 *   a width and height set.
 * - Layouting, performs the actual layouting of the parts.
 * - Alignment, aligns the parts when they have been layouted.
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
#include <ctype.h>

#include "../header/threads.h"
#include "../header/layout.h"
#include "../header/table.h"
#include "../header/ui.h"
#include "../header/protocol.h"
#include "../header/image.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

extern isUtf8;
extern is2Byte;

/* External declaration of internal linked list for layout parts. 
 * The real variable is placed in build.c.
 */
extern struct layout_part *layout_internal_parts;

/* A pointer to the current page information for the active page. */
static struct layout_part *current_basep = NULL;

/**
 * This function prepares some parts before the actual layouting can 
 * take place. It compresses multiple paragraphs, removed some unwanted
 * text, loads images and layouts tables. 
 *
 * @param partp A pointer to the first element in a linked list of layout
 * @param partp parts. This is never NULL now, since we only call this
 * @param partp from layout_do(). At least so far. 
 *
 * @return non-zero value if an error occurred.
 */
static int layout_prepare_parts(struct layout_part *partp, int total_width)
{
  struct layout_part *tmp;

  while(partp) {
    switch(partp->type) {
      /* If many paragraphs are placed after one another, html work in such 
       * a way that they act as if there was only one. The paragraph tag has
       * higher priority than the break row tag, which means if there is a
       * break row already, and a paragraph is approaching, the stored part
       * is saved as a paragraph.
       * This is not true for plain texts, which should have all paragraphs
       * left intact. Plain texts use permanent paragraphs. However, to get
       * the correct line distances, two paragraphs in a row will always
       * turn the second one into a paragraph, as opposed to a break line.
       * We must also make sure that alignments are always taken from the
       * last paragraph and not from the first. 
       */
    case LAYOUT_PART_PARAGRAPH:
      if(partp->data.paragraph.permanent) {
	if(partp->previous)
	  if(partp->previous->type == LAYOUT_PART_PARAGRAPH)
	    partp->data.paragraph.paragraph = 1;
	
	break;
      }

      if(partp->previous) {
	if(partp->previous->type == LAYOUT_PART_PARAGRAPH) {
	  if(!partp->previous->data.paragraph.paragraph) {
	    partp->previous->data.paragraph.paragraph = 
	      partp->data.paragraph.paragraph;
	  }
	  /* If this is a paragraph, and not a break row, we should make
	   * sure its alignment is copied to the one we are going to keep.
	   */
	  if(partp->data.paragraph.paragraph) {
#ifdef DEBUG
	    fprintf(stderr, "copying alignment... %d => %d\n",
		    partp->previous->align.indent_offset,
		    partp->align.indent_offset);
#endif
	    partp->previous->align = partp->align;
	  }
	  /* Remove the current part from the list, as it is now obsolete. */
	  if(partp->next != NULL)
	    partp->next->previous = partp->previous;
	  partp->previous->next = partp->next;
	  tmp = partp->previous;
	  layout_free_part(partp);
	  partp = tmp;
	} else if(partp->previous->previous && 
		  partp->previous->type == LAYOUT_PART_TEXT && 
		  partp->previous->previous->type == LAYOUT_PART_PARAGRAPH && 
		  (!strcmp(partp->previous->data.text.text, " ") || 
		   strlen(partp->previous->data.text.text) == 0)) {
	  if(!partp->previous->previous->data.paragraph.paragraph) {
	    partp->previous->previous->data.paragraph.paragraph = 
	      partp->data.paragraph.paragraph;
	  }

	  /* Remove both the almost empty text as well as the current 
	   * paragraph. 
	   */
	  if(partp->next != NULL)
	    partp->next->previous = partp->previous->previous;
	  partp->previous->previous->next = partp->next;
	  tmp = partp->previous->previous;
	  layout_free_part(partp->previous);
	  layout_free_part(partp);
	  partp = tmp;
	} 
      }
      break;

      /* This converts a graphics part to a text part if the user
       * interface has reported that it does not support images. 
       * I am not yet sure that this is the best place to do this. 
       * It might be better to let the parser do this directly when 
       * it accounters an img-tag. What is keeping me from doing so
       * is that I did not want to introduce the user interface with
       * the parser. I do not think they are ready to meet yet.
       */
    case LAYOUT_PART_GRAPHICS:
      if(!user_interface.ui_support.image) {
	if(partp->data.graphics.alt_text != NULL) {
	  struct layout_part *new_part;
	  char *new_text;
	  
	  new_part = layout_init_part(LAYOUT_PART_TEXT);
	  if(new_part == NULL) {
	    break;
	  }
	  new_part->align = partp->align;

	  new_text = (char *)malloc(strlen(partp->data.graphics.alt_text) + 1);
	  if(new_part == NULL) {
	    layout_free_part(new_part);
	  }
	  strcpy(new_text, partp->data.graphics.alt_text);
	  new_part->data.text.text = new_text;
	  if(partp->previous != NULL)
	    partp->previous->next = new_part;
	  if(partp->next != NULL)
	    partp->next->previous = new_part;
	  new_part->next = partp->next;
	  new_part->previous = partp->previous;
	  layout_free_part(partp);
	} else {
	  /* In this case the user interface does not support images, and 
	   * there is no alternative text. Then we simply remove the image
	   * from the parts list.
	   * 
	   * We are presented with an interesting problem. If the first part
	   * is an image with no alternative text, we get here, but cannot
	   * assign partp->previous->next a value, since partp->previous is 
	   * NULL. We also cannot remove the part, since we cannot update the 
	   * original from here. Of course it is quite easy to work around, 
	   * if we really want to. 
	   */
	  if(partp->previous != NULL) {
	    if(partp->next != NULL)
	      partp->next->previous = partp->previous;
	    partp->previous->next = partp->next;
	    layout_free_part(partp);
	  }
	}
      } else {
	/* Here, we find that the user interface does support images, and 
	 * therefore, we should read the image now.
	 */
	int img_fd;
	struct image_data *picture;

	/* If the image is already loaded for some reason, do not try to load 
	 * it again, or we will have problems.
	 */
	if(partp->data.graphics.data)
	  break;

	if(current_basep == NULL)
	  img_fd = protocol_open(partp->data.graphics.src, NULL, NULL);
	else

	  img_fd = protocol_open(partp->data.graphics.src, 
				 current_basep->data.page_information.url,
				 current_basep->data.page_information.base_url);

	/* If we are unable to open the image, we might want to convert the
	 * image part into a text part containing the alternative text.
	 */
#if defined _WIN32_WCE || WIN32
  if(img_fd == -1) {
#else
  if(img_fd < 0) {
#endif
	  break;
	}
	
	if(partp->geometry.width == 0)
	  partp->geometry.width = -1;
	if(partp->geometry.height == 0)
	  partp->geometry.height = -1;

	picture = image_open(img_fd, partp->geometry.width, partp->geometry.height);
	if(picture == NULL) {
	  protocol_close(img_fd);
	  if(partp->geometry.width == -1)
	    partp->geometry.width = 0;
	  if(partp->geometry.height == -1)
	    partp->geometry.height = 0;
	  break;
	}

	partp->data.graphics.data = picture->data;
	partp->data.graphics.size = picture->size;
	partp->data.graphics.type = LAYOUT_PART_GRAPHICS_RAW;
	partp->geometry.width = picture->width;
	partp->geometry.height = picture->height;
	free(picture);

	protocol_close(img_fd);
      }
      break;

      /* The table has its own layouting function. */
    case LAYOUT_PART_TABLE:
      layout_table(partp, total_width);
      break;

    default:
      break;
    }

    /* Yield after each layout part. Maybe that's overkill? */
    thread_yield();

    /* Recurse over child parts, if there are any. */
    if(partp->child != NULL && partp->type != LAYOUT_PART_TABLE)
      layout_prepare_parts(partp->child, total_width);

    partp = partp->next;
  }
     
  return 0;
}

/**
 * Set width and height for all parts where it can be set at this point. 
 * Also keep track of the maximum width, because we need to return that
 * to the caller of this eminent function.
 *
 * @param partp A pointer to the first element in a linked list of layout
 * @param partp parts. This is never NULL now, since we only call this
 * @param partp from layout_do(). At least so far. 
 *
 * @return the single maximum width of a part, not counting a text as one
 * @return wide part, since that can be divided later.
 */
static int layout_set_sizes(struct layout_part *partp, int total_width)
{
  int max_width, part_max;

  if(total_width <= 0)
    total_width = user_interface.ui_display.width;

  max_width = 0;
  part_max = 0;
  while(partp) {
    switch(partp->type) {
    case LAYOUT_PART_PARAGRAPH:
      partp->geometry.width = 0;
      if(partp->data.paragraph.paragraph)
	partp->geometry.height = 8;
      else
	partp->geometry.height = 0;
      break;

    case LAYOUT_PART_TEXT:
      partp->geometry.width = 0;
      partp->geometry.height = 0;
      break;

    case LAYOUT_PART_LINE:
      if(!partp->data.line.absolute)
	partp->data.line.width = (partp->data.line.width * total_width) / 100;
      partp->geometry.width = partp->data.line.width;
      partp->geometry.height = 2 * 7 + partp->data.line.size;
      break;

      /* Find out the width and height of this image, somehow. 
       * If it is not known already, we will never find out. So we set
       * it to 16x16.
       */
    case LAYOUT_PART_GRAPHICS:
      if(partp->geometry.width == 0)
	partp->geometry.width = 16;
      if(partp->geometry.height == 0)
	partp->geometry.height = 16;
      break;

      /* The table has already been layouted, and the width and height
       * are set, never to be changed. */
    case LAYOUT_PART_TABLE:
      break;

    case LAYOUT_PART_FORM_SUBMIT:
    case LAYOUT_PART_FORM_CHECKBOX:
    case LAYOUT_PART_FORM_RADIO:
    case LAYOUT_PART_FORM_TEXT:
      user_interface.ui_operations->set_size(partp);
      break;

    default:
      partp->geometry.width = 0;
      partp->geometry.height = 0;
    }

    /* Recurse over the child parts, if there are any. */
    if(partp->child != NULL && partp->type != LAYOUT_PART_TABLE)
      part_max = layout_set_sizes(partp->child, total_width);

    if(part_max == 0) {
      if(partp->geometry.width > max_width)
	max_width = partp->geometry.width;
    } else {
      if(part_max > max_width)
	max_width = part_max;
    }

    partp = partp->next;
  }

  return max_width;
}

/**
 * This puts the correct alignment on previously layouted parts. That
 * means that it travels backwards in the linked list, and possibly moves
 * each part that are on the same row to a more correct position in 
 * the known universe. 
 *
 * @param partp A pointer to the last element in the linked list to be
 * @param partp aligned. If this is NULL, we exit gracefully, but also
 * @param partp returns a fabulous error code.
 * @param max_width The total width of the whole screen as we know it.
 * @param max_part_height The height of the highest part in this row.
 *
 * @return non-zero value if an error occurred.
 */
int layout_align_parts(struct layout_part *partp, int max_width, 
		       int max_part_height)
{
  int current_y_position, found;
  int row_width, row_x_offset;
  int row_hspace = 0;

  /* Find the first valid part to align. */
  found = 0;
  while(!found) {
    while(partp && partp->type == LAYOUT_PART_DELETED)
      partp = partp->previous;
    
    if(partp == NULL)
      return 1;
    
    if(partp->type == LAYOUT_PART_PARAGRAPH)
      return 0;
    
    found = 1;
    while(partp && partp->type == LAYOUT_PART_LINK) {
      found = 0;
      partp = partp->child;
    }
  }

  /* We read the Y position from the pointed part, and store that. This
   * is then used to see that we keep our alignment within the same row
   * of parts. We stop aligning as soon as we hit the previous row, since
   * that is already aligned.
   */
  current_y_position = partp->geometry.y_position;
	
  /* Step backwards through the list and align, until we bump into
   * the start of the list, or until the row comes to a beginning.
   * Also break if we get to a title or a paragraph.
   */
  //current_y_position = partp->geometry.y_position;
  row_width = partp->geometry.x_position + partp->geometry.width;	

    switch(partp->align.horizontal) {
    case LAYOUT_PART_ALIGN_LEFT:
    case LAYOUT_PART_ALIGN_JUSTIFY:
      row_x_offset = 0;
      break;

    case LAYOUT_PART_ALIGN_CENTER:
    case LAYOUT_PART_ALIGN_FORCED_CENTER:
		if ( partp->align.horzontal_image == LAYOUT_PART_ALIGN_IMAGE_CENTER )
			row_x_offset = 0;
		else
			row_x_offset = (max_width - row_width) / 2;
      break;
    
    case LAYOUT_PART_ALIGN_RIGHT:
	  row_x_offset = max_width - (row_width + partp->data.graphics.hspace) ;
      break;

    default:
	  row_x_offset = 0;
/*
		if ( partp->align.horizontal == LAYOUT_PART_ALIGN_CENTER )
			row_x_offset = (max_width - row_width) / 2;

		else if ( partp->align.horizontal == LAYOUT_PART_ALIGN_RIGHT)
			row_x_offset = max_width - row_width;
		else
			row_x_offset = 0;
*/
		break;
	}
  while(partp) {
    if(partp->geometry.y_position < current_y_position ||
       partp->type == LAYOUT_PART_PAGE_INFORMATION ||
       partp->type == LAYOUT_PART_PARAGRAPH)
      break;

	if ( partp->type == LAYOUT_PART_TEXT &&
		partp->align.horzontal_image == LAYOUT_PART_ALIGN_IMAGE_RIGHT )
	{
   		partp->geometry.x_position = partp->previous->align.indent_offset;

	    if (partp->previous->type != LAYOUT_PART_GRAPHICS)
		{
				if ( partp->previous->type == LAYOUT_PART_TEXT )
					partp->previous->geometry.x_position = partp->previous->align.indent_offset;
		}

	}
	else if ( partp->type == LAYOUT_PART_TEXT && 
			  partp->align.horzontal_image == LAYOUT_PART_ALIGN_IMAGE_CENTER )
      partp->geometry.y_position = (partp->previous->geometry.y_position - 8)+ (partp->previous->geometry.height / 2); 

	else
		partp->geometry.x_position += row_x_offset;
	

    /* Vertical alignment does not yet work properly. */
#if 0
    fprintf(stderr, "valign=%d\n", partp->align.vertical);
    /* Align this particular part vertically. */
    switch(partp->align.vertical) {
    case LAYOUT_PART_ALIGN_TOP:
      /* We do not need to do any alignment here. */
      break;

    case LAYOUT_PART_ALIGN_MIDDLE:
      partp->geometry.y_position += (max_part_height - 
				     partp->geometry.height) / 2;
      break;

    case LAYOUT_PART_ALIGN_BOTTOM:
      partp->geometry.y_position += max_part_height - partp->geometry.height;
      break;

    default:
      break;
    }
#endif

    /* After aligning the whole table, we call this to set the cell
     * positions.
     */
    if(partp->type == LAYOUT_PART_TABLE) {
      layout_table_set_positions(partp, NULL, 0);
      break;
    }

    if(partp->previous != NULL || 
       partp->parent->type == LAYOUT_PART_TABLE_CELL)
      partp = partp->previous;
    else
      partp = partp->parent->previous;

    if(partp && partp->type == LAYOUT_PART_TABLE)
      break;

    /* Align the child parts of a link group, starting at the last part. */
    if(partp && partp->type == LAYOUT_PART_LINK && partp->child) {
      partp = partp->child;
      while(partp->next)
	partp = partp->next;
    }
  }

  return 0;
}

/**
 * Perform the layouting of all the wonderful parts that the parser
 * has picked up from the fantastic stream provided by earlier parts
 * of the procedure. This will go through the linked list of layout
 * parts, and decide how they are to be spread over the screen that
 * the interface presents us with. This is done using extensive
 * mathematical calculations, optimised to their maximum.
 *
 * @param parts A pointer to the first element in a linked list of layout
 * @param parts parts. If `parts' is NULL, use the internal linked list.
 * @param keep_position A non-zero value means that the X and Y position
 * @param keep_position we are currently working at, is not changed from 
 * @param keep_position the previous call. This is used for layouting child
 * @param keep_position part trees. A value of zero will reset them to 0;0.
 * @param total_width The total possible width to work with. A value of zero
 * @param total_width will use the width of the user interface display.
 * @param total_width Note that this is not necessarily the actual width of
 * @param total_width the finished parts, but more of a guide line.
 * @param result_height
 *
 * @return the maximum width of the layouted parts, or a negative value
 * @return if an error occurred.
 */
int layout_do(struct layout_part *parts, int keep_position, int total_width,
	      int *result_height)
{
  struct layout_part *partp;
  static int max_width = 0, max_row_height = 0;
  static int x_position = 0, y_position = 0;
  int saved_x_position = 0, saved_y_position = 0;
  int saved_max_width = 0, saved_max_row_height = 0;
  int previous_y_position, tmp;

  static int image_align_left  = 0;
  static int image_align_center  = 0;
  static int image_align_right = 0;


  //unsigned int extra_char_cnts = 0;
  int i;
  unsigned int extra_char_cnts = 0;
  unsigned int special_char_cnts = 0;
  int ncheck1 = 0;
  int ncheck2 = 0;



  if(parts == NULL)
    partp = layout_get_page_information(NULL);
  else
    partp = parts;

  /* If the internal list is completely empty, there is not much good we
   * can do, else than consider it an error and bring these devestating
   * news back to the unfortunate caller of this function.
   */
  if(partp == NULL)
    return -1;

  if(partp->type == LAYOUT_PART_PAGE_INFORMATION) {
    current_basep = partp;
  }

  if(!keep_position) {
    saved_x_position = x_position;
    saved_y_position = y_position;
    saved_max_width = max_width;
    saved_max_row_height = max_row_height;

    x_position = 0;
    y_position = 0;
    max_row_height = 0;
    
    /* Get rid of excess fat. Once. */
    layout_prepare_parts(partp, total_width);
    
    /* Set width and height of all parts. This also find the maximum width 
     * among all parts. 
     */
    max_width = layout_set_sizes(partp, total_width);
    
    /* We want to use max_width as reference when layouting the parts. */
    if(total_width <= 0) {
      if(max_width < user_interface.ui_display.width)
	max_width = user_interface.ui_display.width;
    } else {
      if(max_width < total_width)
	max_width = total_width;
    }
  }


  /* Here is an attempt to explain what this is all about.
   * We go through the linked list from beginning to end, working with one
   * part at a time, until that it completely finished.
   * The variables x_position and y_position are set to the coordinates
   * where the next part will end up. An exception to this rule is when we
   * stumble across a paragraph part, which instead sets those variables
   * to where the next part will be placed. Another exception is if we are
   * at the end of a line. Then we have to put the current part on the next
   * row, and set the position variables accordingly. In the case that the
   * part is a text, we might have to split that text up into several parts
   * if it is too long to fit on one row.
   * The variable max_row_height is used to keep track of the highest height
   * during one row. It is used when we go down to a new row, to know how much
   * to increase y_position with.
   * 
   * Alignments are handled in such a way that a row is first layouted as if
   * there was no alignment, or rather, as if the alignment was always to the
   * left and top, and parts are then moved accordingly to the real alignment.
   *
   * The contents of tables have already been layouted at this point, 
   * and here, only the whole table is moved around in the rest of the page.
   *
   * I hope this is all clear now.
   */

  /* Traverse the tree and lay out to the future. */
  while(partp) {

    previous_y_position = y_position;

    switch(partp->type) {
    case LAYOUT_PART_PARAGRAPH:
      x_position = partp->align.indent_offset;
      y_position += max_row_height;
      max_row_height = 0;
      partp->geometry.x_position = x_position;
      partp->geometry.y_position = y_position;
      y_position += partp->geometry.height;
      break;
 
    case LAYOUT_PART_LINE:
      x_position = partp->align.indent_offset;
      y_position += max_row_height;
      max_row_height = 0;
      partp->geometry.x_position = x_position;
      partp->geometry.y_position = y_position;
      y_position += partp->geometry.height;
      break;

    case LAYOUT_PART_TEXT:
      {
	/* Preformatted text demands special attention. I want to give it
	 * the best treatment it can possible get.
	 */
	if(partp->data.text.style.preformatted) {
	  /* Let the user interface set the width and height for this piece 
	   * of text. This calculates as if the text was placed in one row.
	   */
	  user_interface.ui_operations->set_size(partp);

	  if(max_width < partp->geometry.width)
	    max_width = partp->geometry.width;

	  if(partp->geometry.height > max_row_height)
	    max_row_height = partp->geometry.height;

	  partp->geometry.x_position = x_position;
	  partp->geometry.y_position = y_position;
	  x_position = 0;
	  y_position += max_row_height;
	  max_row_height = 0;
	
	  break;
	}

	/* We do not want any excess whitespace at the beginning of a row. 
	 * We also know that there can be only one whitespace at this point.
	 */
	if(x_position == partp->align.indent_offset && 
	   IS_SPACE_ALPHA((int)partp->data.text.text[0])) {
		  memmove(partp->data.text.text, partp->data.text.text + 1,
			strlen(partp->data.text.text));
	}

	if ((image_align_left || image_align_center || image_align_right) 
		&& IS_SPACE_ALPHA((int)partp->data.text.text[0]))
	{
		  memmove(partp->data.text.text, partp->data.text.text + 1,
			strlen(partp->data.text.text));
	}


	/* Let the user interface set the width and height for this piece 
	 * of text. This calculates as if the text was placed in one row.
	 */

	user_interface.ui_operations->set_size(partp);

	if(partp->geometry.height > max_row_height)
	  max_row_height = partp->geometry.height;
	
	if(x_position + partp->geometry.width <= max_width) {
	  /* If we manage to drag the program counter here, it means that all
	   * text in this part actually fits within the room that is left of
	   * the user interface. Despite the long comment, it is a good thing.
	   */
	  partp->geometry.x_position = x_position;
	  partp->geometry.y_position = y_position;
	  x_position += partp->geometry.width;

	  if ( image_align_right)
		partp->align.horzontal_image = LAYOUT_PART_ALIGN_IMAGE_RIGHT;
	  if ( image_align_center)
		partp->align.horzontal_image = LAYOUT_PART_ALIGN_IMAGE_CENTER;
	  image_align_right = 0;
	  image_align_center = 0;	
	} else 
	{
	  /* If we reach this point, we need to break the text up into two
	   * parts. The first part will of course fit in this row, or we would
	   * not have broken it in that place. The second part is put into a
	   * new layout part, which is discreetly sneaked into the linked list
	   * right after the current, and will therefore become the next part
	   * to be processed by this very same loop. Bejoyed.
	   *
	   * Word wrapping is performed, unless a word takes up the whole line,
	   * then that word is cut off at the end of the line.
	   */
	  int first_text_length, second_text_length, width_left;
	  struct layout_part *second_part;
	  char *first_text, *second_text, *full_text;

	  /* Place this part in the correct position. */
	  partp->geometry.x_position = x_position;
	  partp->geometry.y_position = y_position;
	  
	  /* Put the rest of the string into a new part. */

	  width_left = max_width - x_position;
	  
	  /* Save the full, original text for future reference, since the 
	   * following code will change the text which is placed in the
	   * part. 
	   */
	  full_text = (char *)malloc(strlen(partp->data.text.text) + 1);
 
	  if(full_text == NULL)
	    return -1;
	  strcpy(full_text, partp->data.text.text);

	  /* Word wrap and find the best place to cut the text off. */
	  first_text = partp->data.text.text;
	  first_text_length = strlen(first_text);
 
	  do {
		while(first_text_length > 0)
		{
			if ( IS_SPACE_ALPHA((int)first_text[first_text_length - 1]))
				break;
			else
			{
				if ((((unsigned char)first_text[first_text_length - 1]) <= 0xff ) &&
					(((unsigned char)first_text[first_text_length - 2])== 0xfe ))
				{
					partp->data.text.special_char_cnts--;
					first_text_length--;
					break;
				}
					

							
				if ( isUtf8)
				{
					if (((unsigned char)first_text[first_text_length - 1]) & 0x80)
					{
						extra_char_cnts++;
				
						if ( (extra_char_cnts > 0 ) && (!(extra_char_cnts % 3)))
							partp->data.text.extra_char_cnts--;
					}

				}
			}

			first_text_length--;
		}

	    if(first_text_length > 0)
	      first_text[first_text_length - 1] = '\0';
	    
	    user_interface.ui_operations->set_size(partp);
	  } while(first_text_length > 0 && width_left < partp->geometry.width);

	  if (first_text_length > 0 )
			first_text_length--;

    	  /* If the whole text was wrapped, we mark this part as deleted, and
	   * it the text will be taken care of on the next row. However, if 
	   * the word covers the whole line, it cannot be simply be moved to 
	   * the next row, because then we might end up in an infinite loop.
	   * Instead, we cut the word off.
	   *
	   * Marking the part as deleted is a bad idea, because it will create
	   * a memory leak. This will have to be made in a different way later.
	   */
	  if(first_text_length == 0) {
	    if(x_position == partp->align.indent_offset) {
	      width_left = max_width - x_position;
	      strcpy(first_text, full_text);
	      first_text_length = strlen(first_text);
		  do
		  {
			  first_text_length--;
			  first_text[first_text_length] = '\0';
			  user_interface.ui_operations->set_size(partp);
		  }while(first_text_length > 0 && width_left < partp->geometry.width);
			  

	      /* If we do not even have room for one character, we are in 
	       * trouble. Let us quietly mark the text part as deleted, and 
	       * go on with the next.
	       */

	      if(first_text_length == 0) {
		partp->type = LAYOUT_PART_DELETED;	    
		free(full_text);
		break;
	      }
	    } else {
 	      partp->type = LAYOUT_PART_DELETED;
	    }
	  }
	  
	  extra_char_cnts = 0;
	  special_char_cnts = 0;


	  for ( i = 0; i < first_text_length; i++ )
	  {
		if ( isUtf8 )
		{
		  if ( (unsigned char)first_text[i] & 0x80 )
		  {
			if ( (((unsigned char) first_text[i]) == 0xfe) &&
  				 (((unsigned char) first_text[i+1]) <= 0xff))
			{
					special_char_cnts += 2;
					i++;
			}
			else
			{
				extra_char_cnts++;
			}
		  }
		}
		else
		{
			if ( (unsigned char)first_text[i] & 0x80 )
			{
				if ( (((unsigned char) first_text[i]) == 0xfe) &&
					 (((unsigned char) first_text[i+1]) <= 0xff))
				{
					if (extra_char_cnts == 0 )
					{
						special_char_cnts += 2;
						i++;
					}
					else if ((extra_char_cnts > 0) && !(extra_char_cnts % 2))
					{
						special_char_cnts += 2;
						i++;
					}
					else
						extra_char_cnts++;
				}
				else
				{
					extra_char_cnts++;
				}
			}
		}
	  }

	  if ( isUtf8 )
	  {
		if (extra_char_cnts > 0 && special_char_cnts > 0)
		{

			  ncheck1 = (extra_char_cnts % 3);
			  ncheck2 = (special_char_cnts % 2);

 			  if ( ncheck1 && ncheck2 )
			  {
			    strcpy(first_text, full_text);
				first_text_length -= (3 - ncheck1);
				first_text_length -= (2 - ncheck2);
				first_text[first_text_length] = '\0';
			  }
			  else if ( ncheck1 )
			  {
			    strcpy(first_text, full_text);
				first_text_length -= (3 - ncheck1);
				first_text[first_text_length] = '\0';
			  }
			  else if ( ncheck2 )
			  {
			    strcpy(first_text, full_text);
				first_text_length -= (2 - ncheck2);
				first_text[first_text_length] = '\0';
			  }

		  }
		  else if (extra_char_cnts > 0 )
		  {
			  ncheck1 = (extra_char_cnts % 3);
			  if ( ncheck1 )
			  {
 			    strcpy(first_text, full_text);
				first_text_length -= (3 - ncheck1);
				first_text[first_text_length] = '\0';
			  }
		  }
		  else if (special_char_cnts > 0 )
		  {
			  ncheck1 = (extra_char_cnts % 2);
 			  if ( ncheck1 )
			  {
				strcpy(first_text, full_text);
				first_text_length -= (2 - ncheck1);
				first_text[first_text_length] = '\0';
			  }
		  }
	}
	else
	{
		if (extra_char_cnts > 0 && special_char_cnts > 0)
		{

			  ncheck1 = (extra_char_cnts % 2);
			  ncheck2 = (special_char_cnts % 2);
  			  
			  if ( ncheck1 && ncheck2)
			  {
				strcpy(first_text, full_text);
				first_text_length -= (2 - ncheck1);
				first_text_length -= (2 - ncheck2);
			    first_text[first_text_length] = '\0';
			  }
			  else if ( ncheck1 )
			  {
				strcpy(first_text, full_text);
				first_text_length -= (2 - ncheck1);
				first_text[first_text_length] = '\0';
			  }
			  else if ( ncheck2)
			  {
				strcpy(first_text, full_text);
				first_text_length -= (2 - ncheck2);
				first_text[first_text_length] = '\0';
			  }				  

		  }
		  else if (extra_char_cnts > 0 )
		  {
			  ncheck1 = (extra_char_cnts % 2);
			  if ( ncheck1 )
			  {
 				strcpy(first_text, full_text);
				first_text_length -= (2 - ncheck1);
				first_text[first_text_length] = '\0';
			  }
		  }
		  else if (special_char_cnts > 0 )
		  {
			  ncheck1 = (extra_char_cnts % 2);
			  if ( ncheck1 )
			  {
 				strcpy(first_text, full_text);
				first_text_length -= (2 - ncheck1);
				first_text[first_text_length] = '\0';
			  }
		  }
	}
			  
	  extra_char_cnts = 0;
	  special_char_cnts = 0;
	  
	  /* Set the position variables with new values. */

	  if ( image_align_right )
		partp->align.horzontal_image = LAYOUT_PART_ALIGN_IMAGE_RIGHT; 
	
	  if ( image_align_center )
		partp->align.horzontal_image = LAYOUT_PART_ALIGN_IMAGE_CENTER;  
	  
	  if ( partp->align.horzontal_image != LAYOUT_PART_ALIGN_IMAGE_CENTER )
	  {
		if(partp->geometry.height >= max_row_height)
		{
			x_position = partp->align.indent_offset; 
			y_position += max_row_height;
			max_row_height = partp->geometry.height;
		
			if ( image_align_left )
				image_align_left = 0;
			if ( image_align_right )
				image_align_right = 0;

		}
		else
		{
			x_position = partp->geometry.x_position;
			y_position += partp->geometry.height;
			max_row_height -= partp->geometry.height;

		}
	 }
	 else
	 {
			x_position = partp->align.indent_offset; 
			y_position += max_row_height;
			max_row_height = partp->geometry.height;
			if ( image_align_center )
				image_align_center = 0;
	 }

      second_text_length = strlen(full_text) - first_text_length ;
	  second_text = (char *)malloc(second_text_length + 1);
	  if(second_text == NULL) {
	    /* Flee the field, before it is too late not to. */
	    free(full_text);
	    break;
	  }
      
	  strcpy(second_text, full_text + first_text_length );

	  free(full_text);

	  second_part = layout_init_part(LAYOUT_PART_TEXT);
	  if(second_part == NULL) {
	    /* Oops, that was not supposed to happen. Let us bail out. */
	    free(second_text);
	    break; 
	  }
	  
	  for ( i = 0; i < second_text_length; i++ )
	  {
	    if (((unsigned char)second_text[i]) == 0xfe && ((unsigned char)second_text[i+1]) <= 0xff)
		{
		  special_char_cnts++;
		  i++;
		}
	  }
	
	  if ( isUtf8 )
	  {
		  for ( i = 0; i < second_text_length; i++)
		  {
			  if ( ((unsigned char)second_text[i]) & 0x80 )
			  {
				  if ( ((unsigned char)second_text[i]) == 0xfe && ((unsigned char)second_text[i]) <= 0xff )
					  i++;
				  else
				  {
					  extra_char_cnts++;
					  i += 2;
				  }
			  }
		  }
	  }

	  second_part->data.text.text = second_text;
	  second_part->data.text.style = partp->data.text.style;
	  second_part->data.text.extra_char_cnts = extra_char_cnts;
	  second_part->data.text.special_char_cnts = special_char_cnts;


	  second_part->align.horizontal = partp->align.horizontal;
	  second_part->align.vertical	= partp->align.vertical;
	  second_part->align.indent_offset = partp->align.indent_offset;

	  if ( image_align_center || image_align_right )
		  second_part->align.horzontal_image = partp->align.horzontal_image;
	  

	  //second_part->align = partp->align;	  

	  /* Add new part as the next part. */
	  second_part->parent = partp->parent;
	  second_part->next = partp->next;
	  if(partp->next)
	    partp->next->previous = second_part;
	  
	  second_part->previous = partp;
	  partp->next = second_part;
	}
	break;
  }

   case LAYOUT_PART_GRAPHICS:
     if(x_position + partp->geometry.width > max_width) {
	/* Change row. */
	x_position = partp->align.indent_offset;
	y_position += max_row_height;
	max_row_height = 0;
      }
	  
	  partp->geometry.x_position = x_position =+ partp->data.graphics.hspace;
	  partp->geometry.y_position = y_position;

	  if ( partp->data.graphics.hspace > 0)
	  {
		x_position += partp->geometry.width + partp->data.graphics.hspace;
	  }
	  else
		x_position += partp->geometry.width;

      if(partp->geometry.height > max_row_height)
		max_row_height = partp->geometry.height;

   	  if(partp->align.horzontal_image == LAYOUT_PART_ALIGN_IMAGE_LEFT)
		image_align_left = 1;
	  if ( partp->align.horzontal_image == LAYOUT_PART_ALIGN_IMAGE_CENTER)
		image_align_center = 1;
	  if ( partp->align.horzontal_image == LAYOUT_PART_ALIGN_IMAGE_RIGHT)
		image_align_right = 1;
	  break;

      /* Only layout the whole table, since the parts are already layouted. */
    case LAYOUT_PART_TABLE:
      /* A table always starts at a new line, as if a break row were placed 
       * just before it. It also ends in the same way.
       * This is not completely true. When used together with the <div>-tag,
       * tables can be placed next to each other on one line. This is however
       * not yet supported.
       */
      x_position = partp->align.indent_offset;
      y_position += max_row_height;
      max_row_height = 0;

      partp->geometry.x_position = x_position;
      partp->geometry.y_position = y_position;

      layout_align_parts(partp, max_width, partp->geometry.height);
      y_position += partp->geometry.height;
      previous_y_position = y_position;
      break;

    case LAYOUT_PART_FORM_SUBMIT:
    case LAYOUT_PART_FORM_CHECKBOX:
    case LAYOUT_PART_FORM_RADIO:
    case LAYOUT_PART_FORM_TEXT:
      if(x_position + partp->geometry.width > max_width) {
	/* Change row. */
	x_position = partp->align.indent_offset;
	y_position += max_row_height;
	max_row_height = 0;
      }
      partp->geometry.x_position = x_position;
      partp->geometry.y_position = y_position;

      x_position += partp->geometry.width;
      if(partp->geometry.height > max_row_height)
	max_row_height = partp->geometry.height;
      break;

    default:
      break;
    }

    /* Recurse over the child parts, if there are any. */
    if(partp->child != NULL && partp->type != LAYOUT_PART_TABLE) {
      int total_height;

      layout_do(partp->child, 1, max_width, &total_height);

      if(result_height != NULL)
	*result_height = total_height;

      /* If this part is a page information part, we know we have layouted
       * the complete page at this point.
       */
      if(partp->type == LAYOUT_PART_PAGE_INFORMATION) {
	partp->geometry.width = max_width;
	partp->geometry.height = total_height;

	tmp = max_width;
	
	if(!keep_position) {
	  x_position = saved_x_position;
	  y_position = saved_y_position;
	  max_width = saved_max_width;
	  max_row_height = saved_max_row_height;
	}
	
	return tmp;
      }
    }

    if(partp->type == LAYOUT_PART_LINE) {
      layout_align_parts(partp, max_width, max_row_height);
    }
    if(y_position != previous_y_position) {
      if(partp->previous) {
	if(partp->previous->type != LAYOUT_PART_LINE &&
	   partp->previous->type != LAYOUT_PART_TABLE &&
	   partp->previous->type != LAYOUT_PART_FORM)
	  layout_align_parts(partp->previous, max_width, max_row_height);
      } else {
	if(partp->type != LAYOUT_PART_TABLE &&
	   partp->type != LAYOUT_PART_FORM)
	  layout_align_parts(partp, max_width, max_row_height);
      }
	 }
	// added by wichang
	else if ( partp->previous && partp->previous->align.horzontal_image == LAYOUT_PART_ALIGN_IMAGE_RIGHT)
	  layout_align_parts(partp->previous, max_width, max_row_height);


    /* Do not forget to align the very last parts, please. */
    if(partp->next == NULL && 
       partp->type != LAYOUT_PART_TABLE && 
       partp->type != LAYOUT_PART_FORM && 
       (partp->parent->type == LAYOUT_PART_PAGE_INFORMATION ||
	partp->parent->type == LAYOUT_PART_TABLE_CELL)) {
      layout_align_parts(partp, max_width, max_row_height);
    }

    partp = partp->next;
  }

  if(result_height != NULL)
    *result_height = y_position + max_row_height;

  tmp = max_width;

  if(!keep_position) {
    x_position = saved_x_position;
    y_position = saved_y_position;
    max_width = saved_max_width;
    max_row_height = saved_max_row_height;
  }

  return tmp;
}
