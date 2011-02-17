/** 
 * Structs and functions used to create a user interface for
 * Zen, the web browser of Your choice. 
 *
 * @author Tomas Berndtsson <tomas@nocrew.org>
 */

#ifndef _ZEN_UI_H_
#define _ZEN_UI_H_

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

//#include <sys/types.h>

#include "../header/layout.h"
#include "../header/settings.h"

extern int ofbis_open(char *url);
extern int ofbis_close(void);
extern int ofbis_set_palette(int *r, int *g, int *b);
extern int ofbis_set_size(struct layout_part *partp);
/**
 * The different types of user interfaces. I can only think of two.
 */
enum zen_ui_types {
  ZEN_UI_UNSPECIFIED,
  ZEN_UI_TEXTUAL,
  ZEN_UI_GRAPHICAL
};

/**
 * Contains the operations used by the generic user interface to setup
 * the specific user interfaces. Each interface will set the functions
 * in this struct in its init routine.
 *
 * Comments: The parameters to these operations will most likely
 *           be changed during the course of the evolution of Zen. 
 *           More operations will also be added, as support for such 
 *           is added to the whold program. 
 *
 * @member open Operation to open the interface. This is the user interface
 * @member open operation that will take control of the program, with an 
 * @member open event loop to handle the interaction with the user. This
 * @member open operation is required.
 * @member close Operation to close down and clean up after itself. Called
 * @member close from ui_exit(). This operation is required.
 * @member set_size Operation to set the width and height of a layout part.
 * @member set_size This is used when only the interface can know the actual
 * @member set_size size of the part. This operation is required.
 * @member set_palette Operation to set the palette of the interface. This 
 * @member set_palette is an optional operation. It might disappear.
 */
struct zen_ui_operations {
  int (*open)(char *url);
  int (*close)(void);
  int (*set_size)(struct layout_part *partp);
  int (*set_palette)(int *red, int *green, int *blue);
};

/**
 * Contains pointers to functions in the main program that the user interfaces
 * can use in order to communicate with the main program. 
 *
 * @member get_page Function that will get the page pointed to by the url and
 * @member get_page return a complete layouted linked list of parts.
 * @member get_page Note that this blocks the interface, and should only be
 * @member get_page used by non-interactive interfaces.
 * @member request_page Function that will tell the main program to start
 * @member request_page getting a page in a separate thread. The main program
 * @member request_page will notify the interface when it is ready, and the
 * @member request_page interface will know by polling the thread commands.
 * @member request_page This is NULL for non-interactive interfaces.
 * @member request_page The return value is an identifier for the request which
 * @member request_page is used in poll_page to poll for this specific page,
 * @member request_page or a negative value to indicate an error. 
 * @member poll_page This is used by the interface to poll the main program
 * @member poll_page for a new page, after calling request_page.
 * @member poll_page This is NULL for non-interactive interfaces.
 * @member get_status Get a text string containing information about the
 * @member get_status current status of the main program. This is used for
 * @member get_status progress meters and other information.
 * @member get_setting Get a configuration setting.
 * @member set_setting Set a configuration setting.
 */
struct zen_ui_functions {
  struct layout_part *(*get_page)(char *url, char *referer);
  int (*request_page)(char *url, char *referer);
  int (*poll_page)(int page_id, struct layout_part **pagep);
  int (*get_status)(int page_id, char *status, int max_length);
  enum zen_settings_type (*get_setting)(char *setting, void **value);
  int (*set_setting)(char *setting, void *value, enum zen_settings_type type);
};

/**
 * This is part of the zen_ui struct, and describes what the user
 * interface supports. These values are all set to zero by the generic
 * user interface layer, and set specifically by the init routine of
 * the different user interfaces. Note that they are not all used. 
 * Some of them are used to create different behaviour in the parts
 * of Zen which is not the actual interface. 
 *
 * @member table Non-zero value means the interface supports tables.
 * @member frame Non-zero value means the interface supports frames.
 * @member form Non-zero value means the interface supports forms.
 * @member image Non-zero value means the interface supports images.
 * @member font Non-zero value means the interface supports different 
 * @member font kinds of fonts. This includes the ability to change
 * @member font its size. If not supported, assume 8 pixel font.
 * @member interaction Non-zero value means the interface supports 
 * @member interaction interaction with the user.
 * @member freemove Non-zero value means the interface supports free 
 * @member freemove moves of the cursor. Typically not supported by
 * @member freemove the dump user interface.
 * @member scrollable_x non-zero value means the interface is able to
 * @member scrollable_x scroll in the X direction.
 * @member scrollable_y non-zero value means the interface is able to
 * @member scrollable_y scroll in the Y direction.
 */
struct zen_ui_support {
  int table;
  int frame;
  int form;
  int image;
  int font;
  int interaction;
  int freemove;
  int scrollable_x;
  int scrollable_y;
};

/**
 * Contains various information about the user interface. This is where
 * user settings will end up eventually.
 *
 * @member min_fontsize The minimum possible size of the font.
 * @member max_fontsize The maximum possible size of the font.
 * @member default_fontsize The default size of the font.
 */
struct zen_ui_settings {
  int min_fontsize;
  int max_fontsize;
  int default_fontsize;
};

/**
 * The different types of endian the display can be. 
 */
enum zen_ui_display_endian {
  ZEN_UI_DISPLAY_BIG_ENDIAN,
  ZEN_UI_DISPLAY_LITTLE_ENDIAN,
  ZEN_UI_DISPLAY_NATIVE_ENDIAN
};

/**
 * Contains information about the display of the interface. Most of
 * these are only used for graphical interfaces. Some are used by all
 * kinds. Which is which is left as an exercise for the reader. 
 *
 * @member width The width of the interface, measured in pixels.
 * @member width A negative value means the resolution is unlimited.
 * @member height The height of the interface, measured in pixels.
 * @member height A negative value means the resolution is unlimited.
 * @member bit_depth The number of bits per pixels on the display.
 * @member endian The endian type of the display.
 * @member red_length The number of bits used for the red component.
 * @member green_length The number of bits used for the green component.
 * @member blue_length The number of bits used for the blue component.
 * @member red_offset The offset in bits for the red component.
 * @member green_offset The offset in bits for the green component.
 * @member blue_offset The offset in bits for the blue component.
 * @member colourmap A pointer to an array of colours used to quantize
 * @member colourmap images on palette based displays. They are arranged
 * @member colourmap in the order Red, Green and Blue, with 8 bits for
 * @member colourmap each of them. If the pointer is NULL, if means the
 * @member colourmap display is either non-graphical or truecoloured.
 */
struct zen_ui_display {
  int width;
  int height;
  int bit_depth;
  enum zen_ui_display_endian endian;
  int red_length;
  int green_length;
  int blue_length;
  int red_offset;
  int green_offset;
  int blue_offset;
  unsigned char *colourmap;
};

/**
 * Main structure to describe the user interface. Some values are set up
 * by the ui_init() function, and the rest are set up by the init routine
 * of the user interface that is to be used.
 *
 * @member ui_type The type of the user interface.
 * @member ui_operations A pointer to a struct of zen_ui_operations specific
 * @member ui_operations to the current user interface. 
 * @member ui_name A string containing the name of the user interface. 
 * @member ui_version_text A string containing the version of the user 
 * @member ui_version_text interface.
 * @member ui_version_number A number representation of the version of the
 * @member ui_version_number user interface. This is constructed using the
 * @member ui_version_number following formula:
 * @member ui_version_number major * 262144 + minor * 512 + micro
 * @member ui_support Contains information about what the interface supports.
 * @member ui_settings Contains information about the user interface.
 * @member ui_display Contains information about the display of the interface.
 * @member ui_specific A generic pointer to an interface specific area.
 * @member ui_specific The user interface may use this as it pleases.
 */
struct zen_ui {
  enum zen_ui_types ui_type;
  struct zen_ui_operations *ui_operations;
  struct zen_ui_functions *ui_functions;
  char *ui_name;
  char *ui_version_text;
  uint32_t ui_version_number;
  struct zen_ui_support ui_support;
  struct zen_ui_settings ui_settings;
  struct zen_ui_display ui_display;

  void *ui_specific;
};

/* The type of the initialization routine existant in all user interfaces. */
typedef int zen_ui_init_t(int argc, char *argv[], struct zen_ui *ui);

#endif /* _ZEN_UI_H_ */
