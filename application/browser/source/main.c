/**
 * This is where it all begins.
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
#include <io.h>

//#include <unistd.h>

#include <string.h>
#include <errno.h>

#include "../header/settings.h"
#include "../header/parse.h"
#include "../header/ui.h"
#include "../header/layout.h"
#include "../header/protocol.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

static char *argv[100] = {"file://D:/index/index.html"};
static int argc = 3;
static char *url = "file://D:/index/index.html";
/**
 * The beginning of the long and difficult journey.
 *
 * @param argc The number of arguments given to the program.
 * @param argv An array of strings with the given arguments.
 *
 * @return The program exit code, 0 if it exited normally.
 */
int main(/*int argc, char *argv[]*/)
{
  int ret, fd;
//  char *url;
  void *value;

  /* Initialize the settings. This means setting defaults, reading
   * configuration files and interpret the command line options.
   */
  settings_read(argc, argv);

  /* Remainings of an ancient time. */
  /* printf("Hello, world!\n"); */

  /* Setup the user interface of Our choice. 
   * We must initialize the user interface before requesting a document, 
   * to be able to send the correct user agent information to the server.
   */
  ret = ui_init(argc, argv);
  if(ret)
    return ret;
  
  /* Now that we have initialized the user inteface, we read the 
   * interface specific settings from the correct files. 
   * Note that these are read after _all_ other configuration files.
   * The consequence of this, is that if you have interface specific
   * settings in the main configuration files, they might not be
   * used correctly. 
   */
//  settings_read_interface();
   
  /* If the user has requested the configuration to be dumped, we do 
   * just that, and then quit.
   */
  settings_get("dump_config", &value);
  if((int)value) {
    debug_dump_settings();
    exit(0);
  }
  
  /* At this point, if the user has given a URL on the command line, 
   * it will have been placed as the setting default_page. If the
   * user has not given a URL on the command line, the default page
   * is used, as specified in the configuration files or environment
   * variable ZEN_DEFAULT_PAGE. Either way, we can simply read the
   * setting here.
   */
  settings_get("default_page", &value);
  //url = (char *)value;

  /* Dump as source, means really dump it. So let us do just that. */
  settings_get("dump_source", &value);
  if((int)value) {
    char *buffer;
    int bytes;

    /* Open a stream to the specified URL. */
    fd = protocol_open(url, NULL, NULL);
    if(fd < 0) {
      fprintf(stderr, "%s: Could not open %s\n", argv[0], url);
      ui_exit();
      return fd;
    }

    buffer = (char *)malloc(16384);
    if(buffer == NULL) {
      fprintf(stderr, "%s: Memory exhausted.\n", argv[0]);
      return 1;
    }

    while((bytes = read(fd, buffer, 16384)) > 0) {
      write(STDOUT_FILENO, buffer, bytes);
    }

    free(buffer);

    return 0;
  }

  /* Give over control to the user interface. */
  // 해당 url로 ui open을 한다.
  ret = ui_open(url);

  /* It looks here as if the program does not do anything, but it does.
   * The user interface has entered its event loop state, and is running
   * the show from now on. The only contact with the outer world, the
   * world into which I am writing this in, is through functions provided
   * for the user interface.
   */

  /* Actually, right now, ui_exit() waits for the thread that the user
   * interface is running in. When it exits, ui_exit() will close down
   * the rest of the show and then the program is finished.
   */

  /* End all user interface activity. */
  ui_exit();

  /* Clean up after ourselves. */
  layout_delete_all_parts(NULL);

  return ret;
}

