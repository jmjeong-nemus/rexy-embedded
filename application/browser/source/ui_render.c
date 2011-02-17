/**
 * Functions to render parts of a page in the oFBis user interface.
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
//#include <ofbis.h>

#include "../header/zen_ui.h"
#include "../header/info.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

#include "../header/rexy_browser.h"

/* The linked list of links. */
struct ofbis_link *links = NULL;

extern WND hBrowserWnd;

/**
 * Adds a link to the linked list of links.
 *
 * @param partp A pointer to a layout part representing the object 
 * @param partp which is shown as a link.
 * @param linkp A pointer to the layout part containing the link information.
 */
static void add_link(struct layout_part *partp, struct layout_part *linkp)
{
  struct ofbis_link *new, *lastp;
  
  new = (struct ofbis_link *)malloc(sizeof(struct ofbis_link));
  if(new == NULL)
    return;

  new->link = partp->geometry;
  new->part = linkp;
  new->visual_part = partp;

  if(links == NULL) {
    new->next = NULL;
    new->previous = NULL;
    links = new;
  } else {
    /* Find the last element in the list of links. */
    lastp = links;
    while(lastp->next)
      lastp = lastp->next;

    /* Put the link last in the list. */
    new->next = NULL;
    new->previous = lastp;
    lastp->next = new;
  }
}

/**
 * Delete all previous links stored on the linked list of links.
 */
static void delete_all_links(void)
{
  struct ofbis_link *linkp, *tmp;

  linkp = links;
  while(linkp) {
    tmp = linkp;
    linkp = linkp->next;
    free(tmp);
  }

  links = NULL;
}

/**
 * Create a list of links, containing all links on the whole page.
 *
 * @param parts A pointer to the first layout part in the list.
 */
void ofbis_create_links(struct layout_part *parts)
{
  struct ofbis_information *info = 
    (struct ofbis_information *)ofbis_ui->ui_specific;
  struct layout_part *partp;
  static struct layout_part *linkp = NULL;

  partp = parts;

  if(partp && partp->type == LAYOUT_PART_PAGE_INFORMATION) {
    delete_all_links();
  }  

  while(partp) {
    switch(partp->type) {
    case LAYOUT_PART_TEXT:
      if(linkp)
	add_link(partp, linkp);
      break;
    
    case LAYOUT_PART_LINK:
      {
	struct layout_part *old_linkp;

	old_linkp = linkp;
	linkp = partp;
	ofbis_create_links(partp->child);
	linkp = old_linkp;
      }
      break;

    case LAYOUT_PART_GRAPHICS:
      if(linkp) {
	add_link(partp, linkp);
	if(partp->data.graphics.border == 0)
	  partp->data.graphics.border = 1;
	partp->data.graphics.border_colour = info->default_link_colour;
      }
      break;

    default:
      break;
    }

    ofbis_create_links(partp->child);
    if(partp->type == LAYOUT_PART_PAGE_INFORMATION)
      break;

    partp = partp->next;
  }
}

/**
 * Render the page by calling the appropriate rendering functions.
 *
 * @param parts A pointer to the first layout part in the list to be rendered.
 * @param geometry The offset and size of the screen area to render on. This is
 * @param geometry relative to the full rendering area, not to the full screen.
 * @param scroll The scroll offset. Only the position values are used.
 *
 * @return non-zero value if an error occurred.
 */
int ofbis_render(struct layout_part *parts, struct layout_rectangle geometry, 
		 struct layout_rectangle scroll)
{
  struct ofbis_information *info = 
    (struct ofbis_information *)ofbis_ui->ui_specific;
  struct layout_part *partp;
#ifdef USE_CONTROL_FIELD
  struct layout_rectangle full_geometry, control_geometry;
#else
  struct layout_rectangle full_geometry;
#endif

  partp = parts;

  // Lock screen
  GdiLockScreen(hBrowserWnd);
	
  while(partp) {

    if((partp->child == NULL ||
	partp->type == LAYOUT_PART_TABLE ||
	partp->type == LAYOUT_PART_TABLE_CELL) &&
       (partp->geometry.x_position - scroll.x_position > 
	geometry.x_position + geometry.width - 1 ||
	partp->geometry.y_position - scroll.y_position > 
	geometry.y_position + geometry.height - 1 ||
	partp->geometry.x_position - scroll.x_position + 
	partp->geometry.width - 1 < geometry.x_position ||
	partp->geometry.y_position - scroll.y_position + 
	partp->geometry.height - 1 < geometry.y_position)) {

      partp = partp->next;
      continue;
    } 

    if(partp->child == NULL &&
       partp->type != LAYOUT_PART_GRAPHICS &&
       (partp->geometry.x_position - scroll.x_position < 
	geometry.x_position ||
	partp->geometry.y_position - scroll.y_position < 
	geometry.y_position ||
	partp->geometry.x_position - scroll.x_position + 
	partp->geometry.width - 1 > 
	geometry.x_position + geometry.width - 1 ||
	partp->geometry.y_position - scroll.y_position + 
	partp->geometry.height - 1 > 
	geometry.y_position + geometry.height - 1)) {
      
      partp = partp->next;
      continue;
    }

    switch(partp->type) {
    case LAYOUT_PART_PAGE_INFORMATION:
      info->default_background_colour = 
	partp->data.page_information.background_colour;
      info->default_text_colour = 
	partp->data.page_information.text_colour;
      info->default_link_colour = 
	partp->data.page_information.link_colour;
      info->default_active_link_colour = 
	partp->data.page_information.active_link_colour;
      info->default_visited_link_colour = 
	partp->data.page_information.visited_link_colour;

	full_geometry.x_position = info->x_position + geometry.x_position;
    full_geometry.y_position = info->y_position + geometry.y_position;
    full_geometry.width = geometry.width;
    full_geometry.height = geometry.height;
    ofbis_fill_area(full_geometry, info->default_background_colour);
    break;

    case LAYOUT_PART_TEXT:
      ofbis_draw_text(partp, geometry, scroll);
      break;

    case LAYOUT_PART_GRAPHICS:
      ofbis_draw_image(partp, geometry, scroll);
      break;

    case LAYOUT_PART_LINE:
      ofbis_draw_line(partp, geometry, scroll);
      break;

    case LAYOUT_PART_TABLE:
      ofbis_draw_table(partp, geometry, scroll);
      break;

	// related for table
	case LAYOUT_PART_TABLE_ROW:
	case LAYOUT_PART_TABLE_CELL:
    // releated for form										
    case LAYOUT_PART_FORM_SUBMIT:
    case LAYOUT_PART_FORM_CHECKBOX:
    case LAYOUT_PART_FORM_RADIO:
    case LAYOUT_PART_FORM_TEXT:
		/* TODO 060428*/
      break;

    default:
      break;
    }

    if(partp->type != LAYOUT_PART_TABLE )
      ofbis_render(partp->child, geometry, scroll);
    if(partp->type == LAYOUT_PART_PAGE_INFORMATION)
      break;

    partp = partp->next;
  }

  // Unlock screen
  GdiUnlockScreen(hBrowserWnd);

  return 0;
}

/**
 * Render the given part in active link colour, and restore the previously
 * active link to its original colour.
 *
 * @param linkp A pointer to the link which is going to become the new 
 * @param linkp active link. If NULL, we only reinitialize the previously
 * @param linkp saved active link as NULL, to start over.
 * @param geometry The geometry of the page, so that the rendering functions
 * @param geometry will know where to draw the part. 
 */
void ofbis_activate_link(struct ofbis_link* linkp,
			 struct layout_rectangle geometry,
			 struct layout_rectangle scroll)
{
  struct ofbis_information *info = 
    (struct ofbis_information *)ofbis_ui->ui_specific;
  static struct layout_part *previously_active = NULL;
  static uint32_t previous_colour = 0;
  struct layout_part *partp;

  if(linkp)
    partp = linkp->visual_part;
  else
    partp = NULL;

  /* If the previously active part is the same as the one we are trying to
   * activate now, we do not need to do anything, and therefore we will save
   * time and money by simply returning from the function.
   */

  /* Restoring the old part. */
  if(previously_active) {
    switch(previously_active->type) {
    case LAYOUT_PART_TEXT:
      previously_active->data.text.style.colour = previous_colour;
      ofbis_render(previously_active, geometry, scroll);      
      break;
      
    case LAYOUT_PART_GRAPHICS:
      previously_active->data.graphics.border_colour = previous_colour;
      ofbis_render(previously_active, geometry, scroll);
      break;
      
    default:
		;
    }
  }

  if(linkp == NULL) {
    previously_active = NULL;
    ofbis_set_status_text(NULL);
    return;
  }

  /* Make the current part a new colour. */
  switch(partp->type) {
  case LAYOUT_PART_TEXT:
    previous_colour = partp->data.text.style.colour;
    partp->data.text.style.colour = info->default_active_link_colour;
    ofbis_render(partp, geometry, scroll);
    break;

  case LAYOUT_PART_GRAPHICS:
    previous_colour = partp->data.graphics.border_colour;
    partp->data.graphics.border_colour = info->default_active_link_colour;
    ofbis_render(partp, geometry, scroll);
    break;

  default:
	  ;
  }

  ofbis_set_status_text(linkp->part->data.link.href);
  
  previously_active = partp;
}
