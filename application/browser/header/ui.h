/** 
 * Structs and functions prototypes that are used by and for
 * the abstract user interface layer. 
 *
 * @author Tomas Berndtsson <tomas@nocrew.org>
 */

#ifndef _UI_UI_H_
#define _UI_UI_H_

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

#include "../header/zen_ui.h"

/* Function prototypes */
extern int ui_init(int argc, char *argv[]);
extern int ui_open(char *url);
extern int ui_exit(void);

extern int ui_set_palette(int *red, int *green, int *blue);
extern int ui_functions_set_status(char *status);

/* The settings of the user interface. This is setup according to 
 * the user interface by the initialization routines of the interface.
 */
extern struct zen_ui user_interface;

#endif /* _UI_UI_H_ */
