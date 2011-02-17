/**
 * Various information about the oFBis user interface.
 *
 * @author Tomas Berndtsson <tomas@nocrew.org>
 */

#ifndef _UI_OFBIS_INFO_H_
#define _UI_OFBIS_INFO_H_

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

#include "../header/zen_ui.h"

/**
 * Contains some generic information used by the oFBis interface.
 *
 * @member ofbis_fb A pointer to the framebuffer struct.
 * @member x_position This specifies where the drawing of the page should
 * @member x_position begin, relative to the left side of the display.
 * @member y_position This specifies where the drawing of the page should
 * @member y_position begin, relative to the top of the display.
 * @member default_background_colour The background colour for the page.
 * @member default_text_colour The text colour for the page.
 * @member default_link_colour The link colour for the page.
 * @member default_active_link_colour The active link colour for the page.
 * @member default_visited_link_colour The visited link colour for the page.
 * @member control_field_text_colour The colour of the text in the control 
 * @member control_field_text_colour field.
 * @member control_field_colour The colour of the background in the control 
 * @member control_field_colour field.
 */
struct ofbis_information {
  FB *ofbis_fb;
  int x_position;
  int y_position;
  uint32_t default_background_colour;
  uint32_t default_text_colour;
  uint32_t default_link_colour;
  uint32_t default_active_link_colour;
  uint32_t default_visited_link_colour;
  uint32_t control_field_text_colour;
  uint32_t control_field_colour;  
};

/**
 * Holds information about the whereabouts of a link.
 *
 * @member link The geometry of the link. 
 * @member part A pointer to the actual layout part which represent the link.
 * @member visual_part A pointer to the layout part which is displayed.
 * @member next A pointer to the next link in the list.
 * @member previous A pointer to the previous link in the list.
 */
struct ofbis_link {
  struct layout_rectangle link;
  struct layout_part *part;
  struct layout_part *visual_part;
  struct ofbis_link *next;
  struct ofbis_link *previous;
};

extern struct zen_ui *ofbis_ui;
extern int ofbis_open(char *url);
extern int ofbis_close(void);
extern int ofbis_render(struct layout_part *partp,
			struct layout_rectangle geometry,
			struct layout_rectangle scroll);
extern int ofbis_set_title(char *title_text);
extern int ofbis_set_url(char *url);
extern int ofbis_set_status_text(char *text);
extern int ofbis_draw_text(struct layout_part *partp,
			   struct layout_rectangle geometry,
			   struct layout_rectangle scroll);
extern int ofbis_draw_image(struct layout_part *partp,
			    struct layout_rectangle geometry,
			    struct layout_rectangle scroll);
extern int ofbis_draw_line(struct layout_part *partp,
			   struct layout_rectangle geometry,
			   struct layout_rectangle scroll);
extern int ofbis_draw_table(struct layout_part *partp,
			    struct layout_rectangle geometry,
			    struct layout_rectangle scroll);
extern int ofbis_set_palette(int *red, int *green, int *blue);
extern int ofbis_set_size(struct layout_part *partp);

extern int ofbis_set_colour(int index, uint32_t colour);
extern int ofbis_fill_area(struct layout_rectangle geometry, 
			   uint32_t colour);
extern int ofbis_cut_rectangle(struct layout_rectangle part, 
			       struct layout_rectangle geometry,
			       struct layout_rectangle scroll,
			       struct layout_rectangle *cut);
extern void ofbis_move_mouse(int x_distance, int y_distance, 
			     int init, int hide);
extern void ofbis_create_links(struct layout_part *parts);
extern void ofbis_activate_link(struct ofbis_link *linkp,
				struct layout_rectangle geometry,
				struct layout_rectangle scroll);

extern int ofbis_main(char *url);

#endif /* _UI_OFBIS_INFO_H_ */
