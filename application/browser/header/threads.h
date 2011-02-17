/** 
 * Prototypes for the functions handling the threading in Zen.
 *
 * @author Tomas Berndtsson <tomas@nocrew.org>
 */

#ifndef _THREADS_H_
#define _THREADS_H_

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
#ifdef HAVE_PTHREAD_H
#include <pthread.h>
#endif /* HAVE_PTHREAD_H */

#ifdef HAVE_GNU_PTH
#include <pth.h>

/* These versions block the thread when appropriate. */
#define read pth_read
#define write pth_write
#define connect pth_connect
#endif /* HAVE_GNU_PTH */

typedef void *thread_function(void *);

enum thread_command {
  THREAD_COMMAND_UNKNOWN_RX,
  THREAD_COMMAND_GET_PAGE_RX
};

enum thread_response {
  THREAD_RESPONSE_UNKNOWN_RX,
  THREAD_RESPONSE_NEW_PAGE_RX,
};

enum thread_type {
  THREAD_TYPE_UNKNOWN_RX,
  THREAD_TYPE_INTERFACE_RX,
  THREAD_TYPE_GET_PAGE_RX
};

/**
 * Contains information about a thread used in Zen. This forms
 * a linked list of all opened threads.
 *
 * @member thread The POSIX thread identifier.
 * @member type The type of the thread.
 * @member identifier An external identifier for this thread. 
 * @member next A pointer to the next thread in the list, or
 * @member next NULL, if this thread is the last one in the list.
 */

/*
struct thread_info {
  pthread_t thread;
  enum thread_type type;
  int identifier;
  struct thread_info *next;
};
*/
extern int thread_start(enum thread_type type, thread_function *start_function, 
			void *arg);
extern int thread_kill_all(void);
extern int thread_kill(enum thread_type type);
extern int thread_wait(enum thread_type type);
extern enum thread_command thread_wait_command(void **argument);
extern int thread_give_command(enum thread_command command, void *argument);
extern enum thread_response thread_wait_response(void **argument);
extern int thread_check_response(enum thread_response expected_response, 
				 void **argument);
extern int thread_give_response(enum thread_response response, void *argument);

extern void *threaded_request_page(void *arguments);
/**
 * This function is called to yield execution to other threads. That is
 * important with GNU pth, as it is non-preemptive. Other pthread libraries
 * should not need explicit yielding.
 */
static void thread_yield(void) {

#ifdef HAVE_GNU_PTH
pth_yield(NULL);
#endif /* HAVE_GNU_PTH */

}


#endif /* _THREADS_H_ */
