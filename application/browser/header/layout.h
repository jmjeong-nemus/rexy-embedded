/** 
 * Structs and functions prototypes that are used to define the
 * structure created by the parser and used by the layouter.
 *
 * @author Tomas Berndtsson <tomas@nocrew.org>
 */

#ifndef _LAYOUTER_LAYOUT_H_
#define _LAYOUTER_LAYOUT_H_

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

#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif /* HAVE_INTTYPES_H */

#ifndef _WIN32_WCE
	#include <sys/types.h>
//#else
//#include <types.h>
#endif

/**
 * A generically used rectangle definition for generical use.
 *
 * @member x_position The X position of the upper left corner.
 * @member y_position The Y position of the upper left corner.
 * @member width The width of the rectangle.
 * @member height The height of the rectangle.
 */
struct layout_rectangle {
  int x_position;
  int y_position;
  int width;
  int height;
};

/**
 * The different types of parts there are.
 */
enum layout_part_type {
  LAYOUT_PART_DELETED = -1,
  LAYOUT_PART_PAGE_INFORMATION,
  LAYOUT_PART_PARAGRAPH,
  LAYOUT_PART_TEXT,
  LAYOUT_PART_LINK,
  LAYOUT_PART_GRAPHICS,
  LAYOUT_PART_LINE,
  LAYOUT_PART_TABLE,
  LAYOUT_PART_TABLE_ROW,
  LAYOUT_PART_TABLE_CELL,
  LAYOUT_PART_FORM,
  LAYOUT_PART_FORM_SUBMIT,
  LAYOUT_PART_FORM_CHECKBOX,
  LAYOUT_PART_FORM_RADIO,
  LAYOUT_PART_FORM_TEXT,
  LAYOUT_PART_FORM_TEXTAREA,
  LAYOUT_PART_FORM_HIDDEN
};

/**
 * The different alignments in the horizontal view of the world.
 */
enum layout_part_horizontal_align {
  LAYOUT_PART_ALIGN_LEFT,
  LAYOUT_PART_ALIGN_CENTER,
  LAYOUT_PART_ALIGN_RIGHT,
  LAYOUT_PART_ALIGN_JUSTIFY,
  LAYOUT_PART_ALIGN_FORCED_CENTER
};

/**
 * The different alignments as a vertical being.
 */
enum layout_part_vertical_align {
  LAYOUT_PART_ALIGN_TOP,
  LAYOUT_PART_ALIGN_MIDDLE,
  LAYOUT_PART_ALIGN_BOTTOM
};

// added by wichang
enum layout_part_horzontal_image_align {
  LAYOUT_PART_ALIGN_IMAGE_LEFT,
  LAYOUT_PART_ALIGN_IMAGE_CENTER,
  LAYOUT_PART_ALIGN_IMAGE_RIGHT,
  LAYOUT_PART_ALIGN_IMAGE_JUSTIFY,
  LAYOUT_PART_ALIGN_FORCED_IMAGE_CENTER
};


/**
 * The different types of graphical data that are recognised.
 *
 * Comments: This might disappear as the program evolves. I believe
 *           the RAW type will be the only one actually used, making 
 *           this enumeration obsolete.
 */
enum layout_part_graphics_type {
  LAYOUT_PART_GRAPHICS_UNKNOWN,
  LAYOUT_PART_GRAPHICS_RAW,
  LAYOUT_PART_GRAPHICS_JPEG,
  LAYOUT_PART_GRAPHICS_PNG,
  LAYOUT_PART_GRAPHICS_GIF,
  LAYOUT_PART_GRAPHICS_TIFF
};

/**
 * Contains the title text for the page. There can be only one.
 *
 * @member text A pointer to the title text.
 * @member url The URL for the page represented by this page information.
 * @member base_url The base URL used as reference for all other objects
 * @member base_url which uses a relative URL, or NULL to use `url' as base.
 * @member background_colour The background colour of the page.
 * @member text_colour The text colour of the page.
 * @member link_colour The link colour of the page.
 * @member active_link_colour The active link colour of the page.
 * @member visited_link_colour The visited link colour of the page.
 */
struct layout_part_page_information_data {
  char *title;
  char *url;
  char *base_url;
  uint32_t background_colour;
  uint32_t text_colour;
  uint32_t link_colour;
  uint32_t active_link_colour;
  uint32_t visited_link_colour;
};

/**
 * Contains information used to describe a paragraph. This includes
 * paragraphs, headers, row breaks et cetera.
 *
 * @member paragraph A non-zero value means the break is a paragraph
 * @member paragraph and a new line will be printed on its existance.
 * @member permanent A non-zero value means that this paragraph should
 * @member permanent never be removed, due to several paragraphs in a row.
 */
struct layout_part_paragraph_data {
  int paragraph;
  int permanent;
};

/**
 * Contains information to describe the current style of a text.
 *
 * @member size Size of the font to be used for this text.
 * @member face The name of the font requested to be used, or NULL is the 
 * @member face default font is to be used. This can be used in combination
 * @member face with the 'monospaced' element, to find the right font.
 * @member colour The colour to render the text in.
 * @member monospaced A non-zero value means the text is rendered monospaced.
 * @member italic A non-zero value means the text is rendered italic.
 * @member bold A non-zero value means the text is rendered bold.
 * @member underlined A non-zero value means the text is rendered underlined.
 * @member overlined A non-zero value means the text is rendered overlined.
 * @member subscript A non-zero value means the text is rendered as subscript.
 * @member superscript A non-zero value means the text is rendered as 
 * @member superscript superscript.
 * @member preformatted A non-zero value means the text is rendered as
 * @member preformatted preformatted. This implies using a monospaced font.
 * @member directquote A non-zero value means the text is not checked for
 * @member directquote HTML character entities. This is used for plain text.
 */
struct layout_text_styles {
  int size;
  /*  char *face; */
  uint32_t colour;
  int monospaced;
  int italic;
  int bold;
  int underlined;
  int overlined;
  int subscript;
  int superscript;
  int preformatted;
  int directquote;
};

/**
 * Contains the information needed in order to store a text, which is
 * the text itself accompanied by all its possible attributes.
 *
 * @member text A null terminated char array of the text.
 * @member style The style to describe this particular piece of text.
 */
struct layout_part_text_data {
  char *text;
  unsigned int extra_char_cnts;
  unsigned int special_char_cnts;
  struct layout_text_styles style;
};

/**
 * Contains the information needed for a link.
 *
 * @member href A URL to where the link points, as written in the struct.
 */
struct layout_part_link_data {
  char *href;
};

/**
 * Contains the information needed in order to store a graphical object.
 * This is normally used for the img tag, but may also be used by internal
 * graphical objects.
 *
 * @member data A pointer to the graphical data for the object.
 * @member alt_text A null terminated char array containing the alternative
 * @member alt_text textual description of the graphical object, or NULL if
 * @member alt_text there is none.
 * @member src The URL to where to find the image.
 * @member size The size of the data in bytes.
 * @member border The thickness of the border. Zero means no border.
 * @member border_colour A 24-bit value describing the colour of the border.
 * @member border_colour This is used when the image is a link.
 * @member type The type of the data stored in `data'.
 */
struct layout_part_graphics_data {
  unsigned char *data;
  char *alt_text;
  char *src;
  size_t size;
  int border;
  uint32_t border_colour;
  enum layout_part_graphics_type type;
  // addec by wichang
  int hspace;
  int vspace;
};

/**
 * Contains information about a line, usually a horizontal one.
 *
 * @member width The width of the line.
 * @member absolute A non-zero value means the width is described as absolute 
 * @member absolute pixels, otherwise percentage of the width of the screen.
 * @member size The height of the line.
 */
struct layout_part_line_data {
  int width;
  int absolute;
  int size;
};

/**
 *
 */
enum layout_size_type {
  LAYOUT_SIZE_UNSPECIFIED,
  LAYOUT_SIZE_ABSOLUTE,
  LAYOUT_SIZE_PERCENT,
  LAYOUT_SIZE_PART
};

/**
 * Contains information about a table. This is only the top information
 * about the whole table. The rows and columns are represented by other
 * layout parts.
 *
 * @member number_of_rows The total number of rows that can be used.
 * @member number_of_columns The total number of columns that can be used.
 * @member cellspacing The space between columns and rows in the table.
 * @member cellpadding The space between the table contents and the borders of 
 * @member cellpadding the table.
 * @member border The width of the border in pixels.
 * @member width
 * @member height
 * @member width_type
 * @member height_type
 * @member background_colour
 */
struct layout_part_table_data {
  int number_of_rows;
  int number_of_columns;
  int cellspacing;
  int cellpadding;
  int border;
  int width;
  int height;
  enum layout_size_type width_type;
  enum layout_size_type height_type;
  uint32_t background_colour;
};

/**
 * Contains information about a table row.
 *
 * @member background_colour
 */
struct layout_part_table_row_data {
  uint32_t background_colour;
};

/**
 * Contains information about a table cell.
 *
 * @member rowstart The number of the row to start this cell on.
 * @member colstart The number of the column to start this cell on.
 * @member colspan The number of columns this cell takes up.
 * @member rowspan The number of rows this cell takes up.
 * @member width
 * @member height
 * @member width_type
 * @member height_type
 * @member background_colour
 */
struct layout_part_table_cell_data {
  int rowstart;
  int colstart;
  int colspan;
  int rowspan;
  int width;
  int height;
  enum layout_size_type width_type;
  enum layout_size_type height_type;
  uint32_t background_colour;
};

/**
 * The different types of request methods there are for a form action.
 */
enum layout_part_form_method {
  LAYOUT_PART_FORM_GET,
  LAYOUT_PART_FORM_POST
};

/**
 * Contains information about a form.
 *
 * @member method The type of request method to use for this form.
 * @member action The URL to call when submitting.
 */
struct layout_part_form_data {
  enum layout_part_form_method method;
  char *action;
};

/**
 * Contains information about a submit button in a form.
 *
 * @member name The name of the button.
 * @member value The value of the button.
 */
struct layout_part_form_submit_data {
  char *name;
  char *value;
};

/**
 * Contains information about a checkbox in a form.
 *
 * @member name The name of the checkbox.
 * @member value The value of the checkbox.
 * @member checked Non-zero value if the checkbox is checked. 
 */
struct layout_part_form_checkbox_data {
  char *name;
  char *value;
  int checked;
};

/**
 * Contains information about a radio button in a form.
 *
 * @member name The name of the radio button.
 * @member value The value of the radio button.
 * @member checked Non-zero value if this radio button is checked. 
 */
struct layout_part_form_radio_data {
  char *name;
  char *value;
  int checked;
};

/**
 * Contains information about a text field in a form.
 *
 * @member name The name of the text.
 * @member value The value of the text.
 * @member size The width of the field.
 * @member maxlength The maximum number of characters that can be entered
 * @member maxlength in the text field. 
 */
struct layout_part_form_text_data {
  char *name;
  char *value;
  int size;
  int maxlength;
};

/**
 * Contains information about a hidden input in a form.
 *
 * @member name The name of the hidden input.
 * @member value The value of the hidden input.
 */
struct layout_part_form_hidden_data {
  char *name;
  char *value;
};

/**
 * Contains the data used by the different part types.
 *
 * @member generic Raw data used when no extra information is needed.
 * @member paragraph Used by paragraph breaks.
 * @member text Used by text parts.
 * @member graphics Used by graphical parts.
 */
union layout_part_data {
  void *generic;
  struct layout_part_page_information_data page_information;
  struct layout_part_paragraph_data paragraph;
  struct layout_part_text_data text;
  struct layout_part_link_data link;
  struct layout_part_graphics_data graphics;
  struct layout_part_line_data line;
  struct layout_part_table_data table;
  struct layout_part_table_row_data table_row;
  struct layout_part_table_cell_data table_cell;
  struct layout_part_form_data form;
  struct layout_part_form_submit_data form_submit;
  struct layout_part_form_checkbox_data form_checkbox;
  struct layout_part_form_radio_data form_radio;
  struct layout_part_form_text_data form_text;
  struct layout_part_form_hidden_data form_hidden;
};

/**
 * Contains information about how a part is to be aligned compared to the
 * surrounding parts. 
 *
 * @member horizontal How should the part should be aligned horizontally.
 * @member vertical How should the part should be aligned vertically.
 * @member indent_offset Number of pixels to indent the text at the 
 * @member indent_offset beginning of a row. Used for example in lists.
 */
struct layout_aligns {
  enum layout_part_horizontal_align horizontal;
  enum layout_part_vertical_align vertical;  
  int indent_offset;
  enum layout_part_horzontal_image_align horzontal_image;
};

/**
 * Defines a part of the screen that the layouter will
 * build from all parts.
 *
 * @member type The type of the part.
 * @member geometry The position and size of the part.
 * @member align How should the part be aligned.
 * @member data Specific data for the different part types.
 * @member interface_data Optional data for the user interface.
 * @member free_interface_data Optional function, used to let the
 * @member free_interface_data user interface free its own data. If
 * @member free_interface_data this function pointer is NULL and the
 * @member free_interface_data interface_data is not NULL, it will just
 * @member free_interface_data be freed as it is.
 * @member next Pointer to the next part placed next to this.
 * @member previous Pointer to the previous part placed before this.
 * @member child Pointer to the first part within this.
 * @member parent Pointer to the parent part, if this is a child.
 */
struct layout_part {
  enum layout_part_type type;
  struct layout_rectangle geometry;
  struct layout_aligns align;
  union layout_part_data data;
  void *interface_data;
  void (*free_interface_data)(struct layout_part *partp);
  struct layout_part *next;
  struct layout_part *previous;
  struct layout_part *child;
  struct layout_part *parent;
};

/* Prototypes of layout build functions. */
extern struct layout_part *layout_init_part(enum layout_part_type type);
extern int layout_free_part(struct layout_part *part);
extern int layout_add_part(struct layout_part *new);
extern int layout_delete_all_parts(struct layout_part *parts);
extern struct layout_part *layout_get_page_information(struct 
						       layout_part *parts);
extern struct layout_part *layout_get_internal_parts(void);
extern struct layout_part *layout_init_page(char *url, char *referer);

/* Prototypes of layout layouting functions. */
extern int layout_do(struct layout_part *parts, int keep_position,
		     int total_width, int *result_height);

#endif /* _LAYOUTER_LAYOUT_H_ */
