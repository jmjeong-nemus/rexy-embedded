/**
 * Open function for the oFBis user interface.
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
#include "rexy.h"

#include "../header/info.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

/* The signal information. */
//static struct sigaction oldsigs[16];

/**
 * This is the fatal error signal handler. This will guide you from the
 * light, and into the darkness. It will also close the framebuffer and
 * exit abruptly in a very unclean way. I hope to be able to change this
 * behaviour some time in the future.
 *
 * @param sig The signal number which caused the signal handler to be called.
 */
static void signal_handler(int sig)
{
	return;
}

/**
 * This sets up the interface to catch a number of signals that might occur
 * and disturb the task we are here to achieve. This is only for safety, in
 * order to be able to close the framebuffer before exiting the program, if
 * something would go too wrong.
 */
static void setup_signals(void)
{
	return;
}

/**
 * Restore the signals set up by setup_signals().
 */
static void restore_signals(void)
{
	return;
}

/**
 * Open the framebuffer with the settings of the current virtual console.
 * This might change later, so that the interface will be able to set
 * a different resolution. 
 *
 * Comments: We currently open a new virtual console for the interface,
 *           because it is easier to use while developing. Later, this
 *           might change to using the same console, or perhaps let the
 *           user decide.
 *
 * @param url The URL given on the command line for the program.
 *
 * @return non-zero value if an error occurred.
 */
int ofbis_open(char *url)
{
	int ret;

	struct ofbis_information *info = 
		(struct ofbis_information *)ofbis_ui->ui_specific;

	info->ofbis_fb = (unsigned char*)GdiGetScreenPtr();
	GdiGetScreenInfo(&ofbis_ui->ui_display.width ,&ofbis_ui->ui_display.height);
	ofbis_ui->ui_display.bit_depth = 16;
	ofbis_ui->ui_display.red_length = 5;
	ofbis_ui->ui_display.green_length = 6;
	ofbis_ui->ui_display.blue_length = 5;
	ofbis_ui->ui_display.red_offset = 11;
	ofbis_ui->ui_display.green_offset = 5;
	ofbis_ui->ui_display.blue_offset = 0;
	ofbis_ui->ui_display.endian = ZEN_UI_DISPLAY_NATIVE_ENDIAN;

  
	ret = ofbis_main(url);
	return ret;
}
