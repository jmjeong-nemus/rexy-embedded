/**
 * Functions for the main event loop in the oFBis user interface.
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
#include <stdlib.h>
//#include <ofbis.h>

#include "../header/layout.h"
#include "../header/info.h"

#include "rexy.h"
#include "rexy_haldevice.h"
#include "application.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

#include "../header/rexy_browser.h"

#ifndef WIN32
	#define min(a,b) ((a)<(b)?(a):(b))
	#define max(a,b) ((a)>(b)?(a):(b))
#endif

/* The linked list of links. */
extern struct ofbis_link *links;

/* Current position of the mouse cursor. */
static int current_x_position = -1;
static int current_y_position = -1;

/* Current scrolling offset of the page. */
static struct layout_rectangle scroll;

/* Current geometry of the page. */
static struct layout_rectangle geometry;

/* The current active link being selected. */
static struct ofbis_link *active_link;

extern char gCurUrl[1024];

static int page_id;
struct layout_part *parts;
extern isUtf8;

extern ui_exit();

void scroll_page(int scrollWidth, int scrollHeight)
{
	struct ofbis_information *info = 
		(struct ofbis_information *)ofbis_ui->ui_specific;
	FB *fb;
	/*struct ofbis_link *link;*/
	int button_pressed;
	char status_text[2048];/*, *new_url;*/
	/*int page_id, error;*/
	
	fb = info->ofbis_fb;
	
	geometry.x_position = 0;
	geometry.y_position = 0;
	geometry.width = ofbis_ui->ui_display.width - info->x_position;
	geometry.height = ofbis_ui->ui_display.height - info->y_position;
	
	sprintf(status_text, "Loading: %s", gCurUrl);
	ofbis_set_status_text(status_text);
	
	button_pressed = 0;

	scroll.x_position += scrollWidth;
	if(scroll.x_position > parts->geometry.width - geometry.width)
		scroll.x_position = parts->geometry.width - geometry.width;
	if(scroll.x_position < 0)
		scroll.x_position = 0;

	scroll.y_position += scrollHeight;
	if(scroll.y_position > parts->geometry.height - geometry.height)
		scroll.y_position = parts->geometry.height - geometry.height;
	if(scroll.y_position < 0)
		scroll.y_position = 0;

	ofbis_render(parts, geometry, scroll);
}

int BrowserKeyEvent(WND hWnd, int wParam, int lParam)
{
	struct layout_part *tmp_part;
	int error;
	
	if( wParam == VK_USER_CLR || wParam == VK_PAD_9 )
	{
		/* End all user interface activity. */
		ui_exit();
		/* Clean up after ourselves. */
		layout_delete_all_parts(NULL);
	}
	else
	{
		switch( wParam )
		{
			case VK_PAD_1:
			case VK_USER_OK:
				if(active_link) {
					page_id = ofbis_ui->ui_functions->
						request_page(active_link->part->data.link.href,
						parts->data.page_information.url);
				error = ofbis_ui->ui_functions->poll_page(page_id, &tmp_part);
				if(error == 0 && tmp_part != NULL) {
					parts = tmp_part;
					scroll.x_position = 0;
					scroll.y_position = 0;
					ofbis_create_links(parts);
					ofbis_activate_link(NULL, geometry, scroll);
					ofbis_render(parts, geometry, scroll);
					active_link = NULL;
					ofbis_set_status_text(NULL);
					}
				}
				break;
/*
				GdiTextOut(hWnd, "VK_PAD_1", strlen("VK_PAD_1"), 10, 10);
				break;
*/
/*
			case VK_PAD_2:
				GdiTextOut(hWnd, "VK_PAD_2", strlen("VK_PAD_2"), 10, 30);
				break;
*/
			case VK_USER_DOWN:
				if ( parts != NULL)
				{
					scroll_page(0, 12);
					ofbis_activate_link(active_link, geometry, scroll);
				}
				break;
			case VK_USER_UP:
				if ( parts != NULL)
				{
					scroll_page(0, -12);
					ofbis_activate_link(active_link, geometry, scroll);
				}
				break;
			case VK_USER_LEFT:
				if ( parts != NULL )
				{
					scroll_page(-12, 0);
					ofbis_activate_link(active_link, geometry, scroll);
				}
				break;
			case VK_USER_RIGHT:
				if ( parts != NULL )
				{
					scroll_page(12, 0);
					ofbis_activate_link(active_link, geometry, scroll);
				}
				break;
			case VK_USER_NEXT:
			case VK_PAD_6:
				if (links == NULL)
					break;
				if(active_link == NULL)
					active_link = links;
				else if(active_link->next)
					active_link = active_link->next;

				if ((geometry.height + scroll.y_position) <= active_link->link.y_position)
				{
					scroll_page(0, (active_link->link.y_position + active_link->link.height) - (geometry.height + scroll.y_position));
				}

				if ((geometry.height + scroll.y_position) > active_link->link.y_position)
				{
					if ( (scroll.y_position) >= active_link->link.y_position)
						scroll_page(0, ( active_link->link.y_position + active_link->link.height ) - (geometry.height + scroll.y_position));
				}

				ofbis_activate_link(active_link, geometry, scroll);
				
				break;
			
			case VK_USER_PRV:
			case VK_PAD_7:
				if (links == NULL)
					break;
				if(active_link == NULL)
					active_link = links;
				else if(active_link->previous)
					active_link = active_link->previous;
				
				if ((geometry.height + scroll.y_position) >= active_link->link.y_position)
				{
					if ( (scroll.y_position) >= active_link->link.y_position)

						scroll_page(0, ( active_link->link.y_position + active_link->link.height ) - (geometry.height + scroll.y_position));
				}

				if ((geometry.height + scroll.y_position) < active_link->link.y_position)
				{
					if ( (scroll.y_position) <= active_link->link.y_position)
						scroll_page(0, ( active_link->link.y_position + active_link->link.height ) - (geometry.height + scroll.y_position));
				}

				ofbis_activate_link(active_link, geometry, scroll);
				break;
			case VK_USER_BW:
			case VK_PAD_2:
				if ( parts != NULL )
				{
					if(parts->previous) {
						scroll.x_position = 0;
						scroll.y_position = 0;
						parts = parts->previous;
						ofbis_ui->ui_functions->request_page(parts->data.page_information.url, NULL);
						ofbis_create_links(parts);
						ofbis_activate_link(NULL, geometry, scroll);
						ofbis_render(parts, geometry, scroll);
						active_link = NULL;
					}
				}
				break;

			case VK_USER_FW:
			case VK_PAD_3:
				if ( parts != NULL )
				{
					if(parts->next) {
						scroll.x_position = 0;
						scroll.y_position = 0;
						parts = parts->next;
						ofbis_ui->ui_functions->request_page(parts->data.page_information.url, NULL);
						ofbis_create_links(parts);
						ofbis_activate_link(NULL, geometry, scroll);
						ofbis_render(parts, geometry, scroll);
						active_link = NULL;
					}
				}
				break;

			case VK_USER_EUC:
			case VK_PAD_STAR:
				isUtf8 = 0;
				page_id = ofbis_ui->ui_functions->request_page(gCurUrl, NULL);
				error = ofbis_ui->ui_functions->poll_page(page_id, &tmp_part);
				if(error == 0 && tmp_part != NULL) {
					parts = tmp_part;
					scroll.x_position = 0;
					scroll.y_position = 0;
					ofbis_create_links(parts);
					ofbis_activate_link(NULL, geometry, scroll);
					ofbis_render(parts, geometry, scroll);
					active_link = NULL;
					ofbis_set_status_text(NULL);
					}
				
				break;

			case VK_USER_UTF8:
			case VK_PAD_POUND:
				isUtf8 = 1;
				page_id = ofbis_ui->ui_functions->request_page(gCurUrl, NULL);				
				error = ofbis_ui->ui_functions->poll_page(page_id, &tmp_part);
				if(error == 0 && tmp_part != NULL) {
					parts = tmp_part;
					scroll.x_position = 0;
					scroll.y_position = 0;
					ofbis_create_links(parts);
					ofbis_activate_link(NULL, geometry, scroll);
					ofbis_render(parts, geometry, scroll);
					active_link = NULL;
					ofbis_set_status_text(NULL);
					}
				
				break;
	
		}
	}
	
	return 0;
}

/**
 * Checks the given X and Y position against the parts stored in the
 * list of links, to see if the position is part of the link.
 *		
 * @param x The X position of the point to check. If negative, use the
 * @param x globally stored current position.
 * @param y The Y position of the point to check. If negative, use the
 * @param y globally stored current position.
 *
 * @return a pointer to the link under the point, or NULL if there
 * @return was no link at the point.
 */
static struct ofbis_link *check_link(int x, int y)
{
  struct ofbis_information *info = 
    (struct ofbis_information *)ofbis_ui->ui_specific;
  struct ofbis_link *linkp;
  int x_position, y_position;

  if(x < 0)
    x_position = current_x_position;
  else
    x_position = x;

  if(y < 0)
    y_position = current_y_position;
  else
    y_position = y;

  /* Compensate for scrolling and static part. */
  x_position += scroll.x_position - info->x_position;
  y_position += scroll.y_position - info->y_position;

  linkp = links;
  while(linkp) {
    if(x_position >= linkp->link.x_position &&
       x_position < (linkp->link.x_position + linkp->link.width) &&
       y_position >= linkp->link.y_position &&       
       y_position < (linkp->link.y_position + linkp->link.height))
      return linkp;

    linkp = linkp->next;
  }

  return NULL;
}

/**
 * Move the mouse cursor on the screen. This function keeps track of 
 * previous position of the mouse, and saves the background as well as
 * restoring it. If the mouse pointer stumbles over a link, it will
 * change the appearance of the mouse pointer slightly, and highlight
 * the link in the appropriate colour.
 *
 * @param x_distance The amount of pixels the mouse has been moved
 * @param x_distance horizontally since the last check.
 * @param y_distance The amount of pixels the mouse has been moved
 * @param y_distance vertically since the last check.
 * @param init Non-zero value means force reinitialization.
 * @param hide Non-zero value means hiding the pointer.
 */
void ofbis_move_mouse(int x_distance, int y_distance, int init, int hide)
{
	return;
}

/**
 * Scroll the page in some direction, and redraw the area which needs
 * to be redrawn in the new position.
 *
 * So far only scrolling in y direction has been tested.
 *
 * @param parts A pointer to the page information part of the page.
 * @param x_offset The offset in pixels to scroll in x direction. 
 * @param x_offset A negative value scrolls left, a positive right.
 * @param y_offset The offset in pixels to scroll in y direction. 
 * @param y_offset A negative value scrolls up, a positive down.
 *
 * @return non-zero value if an error occurred.
 */
static int ofbis_scroll_page(struct layout_part *parts, 
			     int x_offset, int y_offset)
{

	struct ofbis_information *info = 
		(struct ofbis_information *)ofbis_ui->ui_specific;
	FB *fb;
	int real_scroll_x, real_scroll_y;
	struct layout_rectangle redraw_geometry, old_scroll, tmp_scroll;

	if(parts == NULL)
		return 1;

	fb = info->ofbis_fb;

	old_scroll = scroll;

	scroll.x_position += x_offset;
	if(scroll.x_position > parts->geometry.width - geometry.width)
		scroll.x_position = parts->geometry.width - geometry.width;
	if(scroll.x_position < 0)
		scroll.x_position = 0;

	scroll.y_position += y_offset;
	if(scroll.y_position > parts->geometry.height - geometry.height)
		scroll.y_position = parts->geometry.height - geometry.height;
	if(scroll.y_position < 0)
		scroll.y_position = 0;


	if(old_scroll.x_position != scroll.x_position || 
	old_scroll.y_position != scroll.y_position) {
		real_scroll_x = scroll.x_position - old_scroll.x_position;
		real_scroll_y = scroll.y_position - old_scroll.y_position;

		tmp_scroll = scroll;
		scroll = old_scroll;
		ofbis_move_mouse(0, 0, 0, 1);
		ofbis_move_mouse(0, 0, 1, 1);
		scroll = tmp_scroll;

		if(abs(real_scroll_y) < geometry.height) {
		}

		redraw_geometry.x_position = 0;
		if(real_scroll_y < 0)
		redraw_geometry.y_position = 0;
		else
		redraw_geometry.y_position = geometry.height - real_scroll_y;
		redraw_geometry.width = geometry.width;
		redraw_geometry.height = abs(real_scroll_y);

		ofbis_render(parts, redraw_geometry, scroll);
	}

	return 0;
}

/**
 * Switch over to the tty and read a new URL.
 *
 * @param url The area where the new URL is stored.
 * @param max_length The maximum length of the string used for the URL.
 *
 * @return the actual number of characters read from the user.
 */
int ofbis_get_url_from_user(char *url, int max_length)
{
  struct ofbis_information *info = 
    (struct ofbis_information *)ofbis_ui->ui_specific;
  FB *fb;

  fb = info->ofbis_fb;

  /* Switch to the original TTY. */

  printf("Enter new URL: ");
  fflush(stdout);
  fgets(url, max_length, stdin);
  url[max_length] = '\0';
  if(strlen(url) > 0)
    if(url[strlen(url) - 1] == '\n')
      url[strlen(url) - 1] = '\0';

  /* Switch back to the framebuffer VT. */

  return strlen(url);
}

/**
 * This is the main event loop for the oFBis user interface. This is where
 * it is all controlled. This is where the program take input from the user
 * and acts upon it. This is it.
 *
 * @param url The URL given at the command line.
 *
 * @return non-zero value if an error occurred.
 */
int ofbis_main(char *url)
{
  struct ofbis_information *info = 
    (struct ofbis_information *)ofbis_ui->ui_specific;
  struct layout_part *tmp_part;
  FB *fb;
  /*struct layout_rectangle full_geometry;*/
  
  /*struct ofbis_link *link;*/
  int button_pressed;

  char status_text[2048];/*, *new_url;*/
  int  error;

  fb = info->ofbis_fb;

  scroll.x_position = 0;
  scroll.y_position = 0;

  geometry.x_position = 0;
  geometry.y_position = 0;
  geometry.width = ofbis_ui->ui_display.width - info->x_position;
  geometry.height = ofbis_ui->ui_display.height - info->y_position;

  page_id = ofbis_ui->ui_functions->request_page(url, NULL);
 
  ofbis_set_status_text(status_text);
  
  button_pressed = 0;
  active_link = NULL;

  if(ofbis_ui->ui_functions->get_status(page_id, status_text, 2047) > 0)
     ofbis_set_status_text(status_text);    

    /* Check if we have a new page ready for us. */
   
  error = ofbis_ui->ui_functions->poll_page(page_id, &tmp_part);
  if(error == 0 && tmp_part != NULL) {
    parts = tmp_part;
    scroll.x_position = 0;
    scroll.y_position = 0;
    ofbis_create_links(parts);
    ofbis_activate_link(NULL, geometry, scroll);
    ofbis_render(parts, geometry, scroll);
    active_link = NULL;
    ofbis_set_status_text(NULL);
  }
  else
    ofbis_fill_area(geometry, 0xffffff); // empty html file


  return 0;
}
