/**
 * Functions to contruct and assist the user interfaces with information.
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
#include <string.h>

#include "../header/ui.h"
#include "../header/zen_ui.h"
#include "../header/retrieve.h"
#include "../header/threads.h"
#include "../header/settings.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

/* Temporary space for status line. This will move and use mutex locks. */
static char ui_status_text[1024] = "";

char gCurUrl[1024];
/**
 * Used as thread function to retrieve a new page.
 *
 * @param argument An array of strings with the URL and referer URL.
 *
 * @return always NULL.
 */
void *threaded_request_page(void *arguments)
{
  char *url, *referer;
  struct layout_part *new_page;

  url = ((char **)arguments)[0];
  referer = ((char **)arguments)[1];

  strcpy(gCurUrl, url);

  referer = NULL;

  new_page = retrieve_page(url, referer);
  thread_give_response(THREAD_RESPONSE_NEW_PAGE_RX, (void *)new_page);

  return NULL;
}

/**
 * User interface helper function, used by the interface to request 
 * a new page.
 *
 * @param url The URL for the new page, relative or absolute.
 * @param referer The full referer URL, or NULL if there is no referer.
 *
 * @return a positive page identifier value, or a negative value to
 * @return indicate an error. 
 */
static int ui_functions_request_page(char *url, char *referer)
{
  static char *arguments[2];
  int ret;

  arguments[0] = url;
  arguments[1] = referer;

  /* First kill off all old page requests. */
  thread_kill(THREAD_TYPE_GET_PAGE_RX);

  ret = thread_start(THREAD_TYPE_GET_PAGE_RX, threaded_request_page, 
		     (void *)arguments);

  if(ret != 0)
    return -1;
  
  return 0;
}

/**
 * User interface helper function, used by the interface to check if
 * a newly requested page is ready for displaying.
 *
 * @param page_id The identifier of the page to poll for. 
 * @param pagep a pointer to a pointer to a page information part, 
 * @param pagep where the pointer to the new page will be placed. 
 * @param pagep If NULL is returned, there is no new page yet.
 *
 * @return a negative value if an error occurred when retrieving the
 * @return page, or zero either if there was no new page yet, in which
 * @return case pagep is set to NULL, or if there was a new page, in
 * @return which case pagep is set to point to the page information.
 */
static int ui_functions_poll_page(int page_id, struct layout_part **pagep)
{
  struct layout_part *new_page = NULL;

  page_id = page_id;
  
  if(thread_check_response(THREAD_RESPONSE_NEW_PAGE_RX, (void **)&new_page)) {
    if(new_page == NULL)
      return -1;

    *pagep = new_page;
  } else {
    *pagep = NULL;
  }

  return 0;
}

/**
 * User interface helper function, used by the interface to get the current
 * status text.
 *
 * @param page_id The identifier of the page to get status for. 
 * @param status A pointer to an area to store the status text in.
 * @param max_length The maximum length of the status text to store.
 *
 * @return the actual number of characters in the status text. 
 */
static int ui_functions_get_status(int page_id, char *status, int max_length)
{
  page_id = page_id;

  strncpy(status, ui_status_text, max_length);
  status[max_length] = '\0';
  ui_status_text[0] = '\0';

  return strlen(status);
}

/**
 * Set the status text, which can be gotten from the user interface later.
 * This is used by various parts of the whole program, to report the 
 * current status about what it is doing, and if something special happens.
 *
 * @param status The new status text to store.
 *
 * @return the length of the new status text.
 */
int ui_functions_set_status(char *status)
{
  strncpy(ui_status_text, status, 1023);
  ui_status_text[1023] = '\0';

  return strlen(ui_status_text);
}

/**
 * User interface helper function, used by the interface to get a 
 * configuration settings value.
 *
 * @param setting The name of the setting to retrieve.
 * @param value The place where the value is stored.
 *
 * @return the type of the setting.
 */
static enum zen_settings_type ui_functions_get_setting(char *setting,
						       void **value)
{
  return 0;//settings_get(setting, value);
}

/**
 * User interface helper function, used by the interface to set a 
 * configuration settings value.
 *
 * @param setting The name of the setting to set.
 * @param value The value to set the setting to.
 * @param type The type of the setting.
 *
 * @return a non-zero value if an error occurred.
 */
static int ui_functions_set_setting(char *setting, void *value,
				    enum zen_settings_type type)
{
  return 0;//settings_set(setting, value, type);
}

/**
 * Initialize the functions that the generic interface can offer to the
 * user interfaces.
 *
 * @return a pointer to the struct containing pointers to the functions.
 */
struct zen_ui_functions *ui_functions_init(void)
{
  static struct zen_ui_functions functions;

  /* These can only be used if the interface is interactive, but
   * we cannot check that here anymore, since the interface needs 
   * to use some functions when initializing.
   */
  functions.request_page = ui_functions_request_page;
  functions.poll_page = ui_functions_poll_page;

  functions.get_page = retrieve_page;
  functions.get_status = ui_functions_get_status;
  functions.get_setting = ui_functions_get_setting;
  functions.set_setting = ui_functions_set_setting;

  return &functions;
}

