/**
 * Contains functions to initialize the oFBis user interface.
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

#include "../header/zen_ui.h"
#include "../header/version.h"
#include "../header/info.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

/* A pointer to the user interface struct. This is global because
 * the other parts of the oFBis user interface will want to get some
 * information from this.
 */
struct zen_ui *ofbis_ui;

/**
 * Initialize the framebuffer oFBis user interface. This is the function
 * that the main program will look for after loading the ofbis shared
 * library. It sets up the zen_ui struct with appropriate values and
 * sets the zen_ui_operations struct to point to the dump functions. 
 *
 * @param argc The number of arguments given on the command line.
 * @param argv An array of arguments given on the command line. 
 * @param ui A zen_ui struct that contains information about the
 * @param ui oFBis user interface. 
 *
 * @return non-zero value if an error occurred.
 */
int init(int argc, char *argv[], struct zen_ui *ui)
{
  static struct zen_ui_operations ofbis_operations = {
    ofbis_open,
    ofbis_close,
    ofbis_set_size,
    ofbis_set_palette
  };
  static struct ofbis_information ofbis_specific;

  ui->ui_type = ZEN_UI_GRAPHICAL;
  ui->ui_operations = &ofbis_operations;
  ui->ui_name = UI_NAME;
  ui->ui_version_text = UI_VERSION_TEXT;
  ui->ui_version_number = UI_VERSION_NUMBER;

  ui->ui_support.table = 1;
  ui->ui_support.form = 1;
  ui->ui_support.frame = 1;
  ui->ui_support.image = 1;
  ui->ui_support.font = 1;
  ui->ui_support.interaction = 1;
  ui->ui_support.freemove = 1;
  ui->ui_support.scrollable_x = 1;
  ui->ui_support.scrollable_y = 1;

  ui->ui_settings.min_fontsize = 8;
  ui->ui_settings.max_fontsize = 16;
  ui->ui_settings.default_fontsize = 12;

  /* The values in the display struct cannot be set until we have opened the
   * frame buffer in oFBis. This happens in ofbis_open(). This is alright, 
   * because they are not supposed to be used before we have opened the display
   * anyway.
   */

  /* Initialize the oFBis specific area. */
  ofbis_specific.ofbis_fb = NULL;
  ofbis_specific.x_position = 0;
#ifdef USE_CONTROL_FIELD
  ofbis_specific.y_position = 64;
#else
  ofbis_specific.y_position = 0;
#endif
  ofbis_specific.default_background_colour = 0xc0c0c0;
  ofbis_specific.default_text_colour = 0x000000;
  ofbis_specific.default_link_colour = 0x0000dd;
  ofbis_specific.default_active_link_colour = 0xdd0000;
  ofbis_specific.default_visited_link_colour = 0xdd00dd;
  ofbis_specific.control_field_text_colour = 0x000000;
  ofbis_specific.control_field_colour = 0xaaaaaa;

  /* Set the pointer to the generic interface area. */
  ui->ui_specific = &ofbis_specific;

  ofbis_ui = ui;

  return 0;
}
