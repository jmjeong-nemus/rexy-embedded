/**
 * Functions which are used to build up the layout structs that
 * the layouter will base its work upon.
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

#include "../header/layout.h"
#include "../header/protocol.h"

/* I really do not want this here. The layouter should not know anything
 * about the parser. The alternative would be to move the state handling
 * to the layouter, but it does not really belong there. For now, this
 * is how I will have it. Let us hope I can come up with something better
 * in the future.
 */
#include "../header/states.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

/**
 * Internal linked list for layout parts. 
 */
struct layout_part *layout_internal_parts = NULL;

/**
 * A pointer to the current base for the page being parsed or layouted.
 */
struct layout_part *current_base = NULL;

/** 
 * Create and initialize a layout part of a specified type. This function
 * sets default values, if there are any worth mentioning.
 *
 * @return a pointer to a basically empty layout_part struct or
 * @return NULL an error occurred.
 */
struct layout_part *layout_init_part(enum layout_part_type type)
{
  struct layout_part *new;

  new = (struct layout_part *)malloc(sizeof(struct layout_part));
  if(new == NULL)
    return NULL;

  new->type = type;
  new->geometry.x_position = 0;
  new->geometry.y_position = 0;
  new->geometry.width = 0;
  new->geometry.height = 0;
  /* The aligns are defaulted in parse_state_init(). */
  new->interface_data = NULL;
  new->free_interface_data = NULL;
  new->next = NULL;
  new->previous = NULL;
  new->child = NULL;
  new->parent = NULL;

  switch(type) {
  case LAYOUT_PART_PAGE_INFORMATION:
    new->data.page_information.title = NULL;
    new->data.page_information.url = NULL;
    new->data.page_information.base_url = NULL;
    new->data.page_information.background_colour = 0xffffff;
    new->data.page_information.text_colour = 0x000000;
    new->data.page_information.link_colour = 0x0000ee;
    new->data.page_information.active_link_colour = 0xee0000;
    new->data.page_information.visited_link_colour = 0xddcc22;
    break;

  case LAYOUT_PART_PARAGRAPH:
    new->data.paragraph.paragraph = 0;
    new->data.paragraph.permanent = 0;
    break;

  case LAYOUT_PART_TEXT:
    new->data.text.text = NULL;
    /* The styles are defaulted in parse_state_init(). */
    break;

  case LAYOUT_PART_LINK:
    new->data.link.href = NULL;
    break;

  case LAYOUT_PART_GRAPHICS:
    new->data.graphics.data = NULL;
    new->data.graphics.alt_text = NULL;
    new->data.graphics.src = NULL;
    new->data.graphics.size = 0;
    new->data.graphics.border = 0;
    new->data.graphics.border_colour = 0x80000000;
    new->data.graphics.type = LAYOUT_PART_GRAPHICS_UNKNOWN;
    break;

  case LAYOUT_PART_LINE:
    new->data.line.width = 100;
    new->data.line.absolute = 0;
    new->data.line.size = 2;
    break;

  case LAYOUT_PART_TABLE:
    new->data.table.number_of_rows = 0;
    new->data.table.number_of_columns = 0;
    new->data.table.cellspacing = 2;
    new->data.table.cellpadding = 2;
    new->data.table.border = 0;
    new->data.table.width = 100;
    new->data.table.width_type = LAYOUT_SIZE_PERCENT;
    new->data.table.height = 0;
    new->data.table.height_type = LAYOUT_SIZE_UNSPECIFIED;
    new->data.table.background_colour = 0x80000000;
    break;

  case LAYOUT_PART_TABLE_ROW:
    new->data.table_cell.background_colour = 0x80000000;
    break;

  case LAYOUT_PART_TABLE_CELL:
    new->data.table_cell.colstart = 0;
    new->data.table_cell.rowstart = 0;
    new->data.table_cell.colspan = 1;
    new->data.table_cell.rowspan = 1;
    new->data.table_cell.width = 0;
    new->data.table_cell.width_type = LAYOUT_SIZE_UNSPECIFIED;
    new->data.table_cell.height = 0;
    new->data.table_cell.height_type = LAYOUT_SIZE_UNSPECIFIED;
    new->data.table_cell.background_colour = 0x80000000;
    break;

  case LAYOUT_PART_FORM:
    new->data.form.method = LAYOUT_PART_FORM_GET;
    new->data.form.action = NULL;
    break;

  case LAYOUT_PART_FORM_SUBMIT:
    new->data.form_submit.name = NULL;
    new->data.form_submit.value = NULL;
    break;

  case LAYOUT_PART_FORM_CHECKBOX:
    new->data.form_checkbox.name = NULL;
    new->data.form_checkbox.value = NULL;
    new->data.form_checkbox.checked = 0;
    break;

  case LAYOUT_PART_FORM_RADIO:
    new->data.form_radio.name = NULL;
    new->data.form_radio.value = NULL;
    new->data.form_radio.checked = 0;
    break;

  case LAYOUT_PART_FORM_TEXT:
    new->data.form_text.name = NULL;
    new->data.form_text.value = NULL;
    new->data.form_text.size = 20;
    new->data.form_text.maxlength = 0;
    break;

  case LAYOUT_PART_FORM_HIDDEN:
    new->data.form_hidden.name = NULL;
    new->data.form_hidden.value = NULL;
    break;

  default:
    new->data.generic = NULL;
  }
  
  new->align.horzontal_image = LAYOUT_PART_ALIGN_IMAGE_LEFT;

  return new;
}

/**
 * It is time to free this part, and send its bytes into oblivion.
 * Malloc to free, bits to bits. Of bytes are thy come, and of bytes
 * shall thee again become.
 *
 * @param part A pointer to a layout part object to free.
 *
 * @return non-zero value if an error occurred.
 */
int layout_free_part(struct layout_part *part)
{
  switch(part->type) {
  case LAYOUT_PART_PAGE_INFORMATION:
    if(part->data.page_information.title != NULL)
      free(part->data.page_information.title);
    if(part->data.page_information.url != NULL)
      free(part->data.page_information.url);
    if(part->data.page_information.base_url != NULL)
      free(part->data.page_information.base_url);
    break;

  case LAYOUT_PART_PARAGRAPH:
    break;

  case LAYOUT_PART_TEXT:
    if(part->data.text.text != NULL)
      free(part->data.text.text);
    break;

  case LAYOUT_PART_LINK:
    if(part->data.link.href != NULL)
      free(part->data.link.href);
    break;

  case LAYOUT_PART_GRAPHICS:
    if(part->data.graphics.data != NULL)
      free(part->data.graphics.data);
    if(part->data.graphics.alt_text != NULL)
      free(part->data.graphics.alt_text);
    if(part->data.graphics.src != NULL)
      free(part->data.graphics.src);
    break;

  case LAYOUT_PART_LINE:
    break;

  case LAYOUT_PART_TABLE:
    break;

  case LAYOUT_PART_TABLE_ROW:
    break;

  case LAYOUT_PART_TABLE_CELL:
    break;

  case LAYOUT_PART_FORM:
    if(part->data.form.action != NULL)
      free(part->data.form.action);
    break;

  case LAYOUT_PART_FORM_SUBMIT:
    if(part->data.form_submit.name != NULL)
      free(part->data.form_submit.name);
    if(part->data.form_submit.value != NULL)
      free(part->data.form_submit.value);
    break;

  case LAYOUT_PART_FORM_CHECKBOX:
    if(part->data.form_checkbox.name != NULL)
      free(part->data.form_checkbox.name);
    if(part->data.form_checkbox.value != NULL)
      free(part->data.form_checkbox.value);
    break;

  case LAYOUT_PART_FORM_RADIO:
    if(part->data.form_radio.name != NULL)
      free(part->data.form_radio.name);
    if(part->data.form_radio.value != NULL)
      free(part->data.form_radio.value);
    break;

  case LAYOUT_PART_FORM_TEXT:
    if(part->data.form_text.name != NULL)
      free(part->data.form_text.name);
    if(part->data.form_text.value != NULL)
      free(part->data.form_text.value);
    break;

  case LAYOUT_PART_FORM_HIDDEN:
    if(part->data.form_hidden.name != NULL)
      free(part->data.form_hidden.name);
    if(part->data.form_hidden.value != NULL)
      free(part->data.form_hidden.value);
    break;

  default:
    if(part->data.generic != NULL)
      free(part->data.generic);
  }
  
  /* Safely free possible interface specific data. If the interface
   * has provided its own function for doing this, we oblige to its
   * wishes and let it handle it all by itself.
   */
  if(part->interface_data != NULL) {
    if(part->free_interface_data != NULL)
      part->free_interface_data(part);
    else
      free(part->interface_data);
  }

  free(part);

  return 0;
}

/**
 * Add a new element to the linked list `parts'. The new element is not
 * copied, only the pointer is stored in the list. The new part is placed
 * under the current base part.
 *
 * @param newparts A pointer to the first element in a linked list of layout
 * @param parts parts. If `parts' is NULL, use the internal linked list.
 * @param new A pointer to a layout part that should be added to the
 * @param new linked list provided by `parts'.
 *
 * @return non-zero value if an error occurred.
 */
int layout_add_part(struct layout_part *new)
{
  struct layout_part *partp, *base_part;
  struct layout_aligns align;

  base_part = NULL;
  parse_state_get_current(NULL, NULL, &base_part);

  if(base_part == NULL)
    partp = current_base;
  else
    partp = base_part;

  new->parent = partp;

  /* We start by selecting the child to the current partp. If it does not
   * exist, we put the new part at that position.
   */
  if(partp->child == NULL) {
    partp->child = new;
  } else {
    partp = partp->child;

    /* Find the last element in the list at the current level. */
    while(partp->next) {
      partp = partp->next;
    }
    
    new->previous = partp;
    partp->next = new;
  }

  /* This gets the current alignment state and adds that to the
   * new part which is about to be added to the linked list.
   */
  parse_state_get_current(NULL, &align, NULL);
  new->align = align;
  return 0;
}

/**
 * Delete all parts from the linked list given in the parameter
 * `parts'. Free all memory areas allocated by layout_init_part().
 *
 * @param parts A pointer to the first element in a linked list of layout
 * @param parts parts. If `parts' is NULL, use the internal linked list.
 *
 * @return non-zero value if an error occurred.
 */
int layout_delete_all_parts(struct layout_part *parts)
{
  struct layout_part *partp, *previous_part;

  if(parts == NULL) {
    partp = layout_internal_parts;
  } else {
    partp = parts;
  }

  while(partp) {
    /* Get rid of the child tree first. We do this by recursively call
     * this function again.
     */
    if(partp->child != NULL)
      layout_delete_all_parts(partp->child);

    previous_part = partp;
    partp = partp->next;
    layout_free_part(previous_part);
  }

  if(parts == NULL)
    layout_internal_parts = NULL;

  return 0;
}

/** 
 * Retreive the first part on the linked list given by the parameter,
 * which contains information about the page.
 *
 * @param parts A pointer to the first part of a linked list of layout parts,
 * @param parts or NULL if the internal list is to be used.
 *
 * @return a pointer to the page information part, or NULL if the part was
 * @return not available or if an error occurred.  
 */
struct layout_part *layout_get_page_information(struct layout_part *parts)
{
  if(parts == NULL)
    return current_base;
  else
    return parts;
}

/**
 * Return whatever the internal layout part linked list pointer is pointing
 * to at the moment. 
 *
 * @return the pointer to the internal layout part, or NULL if it has not
 * @return been initialized yet.
 */
struct layout_part *layout_get_internal_parts(void)
{
  return layout_internal_parts;
}

/**
 * Initialize a new page in the linked layout list. This puts a new page
 * information part first as the base of the new page. All parts belonging
 * to this page will be placed under this, starting from the child link of
 * the page information part.
 * This must be called first, and if it is not, the programmer has made a 
 * major mistake. This will set the global current base pointer, which is
 * used by other layout functions as the default base.
 *
 * @param url The URL for the page being initialized.
 * @param referer The URL we came from. This is used to place the new page
 * @param referer in the correct place in the linked list of pages. NULL
 * @param referer means we put the new page last in the list.
 *
 * @return a pointer to a layout part which is the base of the new page.
 */
struct layout_part *layout_init_page(char *url, char *referer)
{
  struct layout_part *previous_part, *partp;

  partp = layout_internal_parts;
  previous_part = partp;
  if(referer != NULL) {
    /* Search for the page corresponding to the referer URL. */
    while(partp && strcmp(previous_part->data.page_information.url, referer)) {
      previous_part = partp;
      partp = partp->next;
    }
    /* Remove all pages after the referer, unless it is the last one. */
    if(previous_part->next) {
      layout_delete_all_parts(previous_part->next);
      previous_part->next = NULL;
    }
  } else {
    /* Search for the last page. */
    while(partp) {
      previous_part = partp;
      partp = partp->next;
    }
  }

  partp = layout_init_part(LAYOUT_PART_PAGE_INFORMATION);
  if(partp == NULL)
    return NULL;
  partp->data.page_information.url = (char *)malloc(strlen(url) + 1);
  if(partp->data.page_information.url == NULL) {
    layout_free_part(partp);
    return NULL;
  }
  strcpy(partp->data.page_information.url, url);

  if(layout_internal_parts == NULL) {
    layout_internal_parts = partp;
  } else {
    partp->previous = previous_part;
    partp->next = previous_part->next;
    if(previous_part->next)
      previous_part->next->previous = partp;
    previous_part->next = partp;
  }

  current_base = partp;

  return partp;
}
