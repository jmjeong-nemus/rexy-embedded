/**
 * Functions to provide a generic method of getting a stream from 
 * any supported protocol, according to the URL.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <direct.h>
//#include <unistd.h>

#ifndef _WIN32_WCE
	#include <errno.h>
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif /* HAVE_STRINGS_H */

#include "../header/protocol.h"
#include "../header/streams.h"
#include "../header/file.h"
#include "../header/http.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

/**
 * A pointer to the first element in the linked list containing the 
 * information about the streams. 
 */
#define MAX_DIRECTORY_LEN 300
static struct protocol_stream *first_stream = NULL;
unsigned char fullFileName[MAX_DIRECTORY_LEN];

#ifndef _WIN32_WCE
extern char *getcwd(char *, int);
#endif

/* Used to store the current base URL. */
char *base_host = NULL;
char *base_path = NULL;

/**
 * Store a URL as the base for following relative references. This will
 * store the URL in two parts, the host, including protocol type, and
 * the path, not including the initial slash.
 *
 * @param url The full URL to make the new base URL.
 *
 * @return non-zero value if an error occurred.
 */
static int protocol_store_base(char *url)
{
  char *host_text, *path_text, *tmp;

  host_text = (char *)malloc(strlen(url) + 16);
  if(host_text == NULL)
    return -1;
  strcpy(host_text, url);

  if(!strncmp(host_text, "http://", 7)) {
    /* If the URL contains GET method information after a question mark, 
     * we will need to remove that first. We search for the first occurrance
     * of a question mark and remove everything efter that. 
     */
    tmp = strchr(host_text, '?');
    if(tmp != NULL)
      tmp[0] = '\0';

    path_text = strchr(&host_text[7], '/');
    if(path_text != NULL) {
      path_text[0] = '\0';
      path_text++;
    }
    base_host = realloc(base_host, strlen(host_text) + 1);
    strcpy(base_host, host_text);
  } else if(!strncmp(host_text, "file://", 7)) {
    path_text = host_text + 7;
    base_host = realloc(base_host, 16);
    strcpy(base_host, "file://");
  } else {
    base_host = realloc(base_host, 16);
    strcpy(base_host, "file://");
    path_text = host_text;
  }

  /* If we have a path, copy it to base_path, otherwise set the same
   * to an empty string.
   */
   if(path_text != NULL) {

    /* If the last element in the URL is a file, we want to remove that. 
     * At this point, we know that the URL will always end with a slash, 
     * if the last element is a directory. We have ensured that when
     * trying to connect to the URL.
     *
     * Hmm. This might not be true for other protocols than HTTP.
     */
    if(path_text[strlen(path_text) - 1] != '/') {
      tmp = strrchr(path_text, '/');
      if(tmp == NULL)
	tmp = path_text;
      tmp[0] = '\0';
    }

    base_path = realloc(base_path, strlen(path_text) + 1);
    if(base_path == NULL)
      return 1;
    strcpy(base_path, path_text);    
  } else {
    base_path = realloc(base_path, 1);
    if(base_path == NULL)
      return 1;
    base_path[0] = '\0';
  }
  
  free(host_text);

  return 0;
}

/**
 * Take a URL and give it an awful treatment, to squeeze the size to its
 * very limits. The lower limits. More detailed, this will remove double,
 * or more, slashes, slash dot slash and slash dot dot together with the 
 * previous directory. We do not remove slash dot at the end of a URL,
 * because Roxen web server uses this in a feature to list a directory
 * where index.html, or similar, normally would have been loaded.
 * This function expects an absolute URL.
 *
 * @param url The uncompressed URL.
 *
 * @return non-zero value if an error occurred.
 */
static int protocol_compress_url(char *url)
{
  char *dir_start, *tmp, *tmp2;

  if(!strncmp(url, "http://", 7)) {
    dir_start = strchr(&url[7], '/');
    if(dir_start == NULL)
      return 0;
  } else if(!strncmp(url, "file://", 7)) {
    dir_start = &url[7];
  } else {
    return 1;
  }

  /* Start with removing all multiple slashes. */
  while((tmp = strstr(dir_start, "//")) != NULL) {
    memmove(tmp, &tmp[1], strlen(tmp));
  }

  /* Then remove slash dot slash. */
  while((tmp = strstr(dir_start, "/./")) != NULL) {
    memmove(tmp, &tmp[2], strlen(tmp) - 1);
  }

  /* And almost last, but almost not least, remove slash dot dot slash
   * and previous directory.  
   */
  while((tmp = strstr(dir_start, "/../")) != NULL) {
    tmp2 = tmp;
    tmp2--;
    while(tmp2 >= dir_start && *tmp2 != '/') {
      tmp2--;
    }
    if(tmp2 < dir_start)
      memmove(tmp, &tmp[3], strlen(tmp) - 2);
    else
      memmove(tmp2, &tmp[3], strlen(tmp) - 2);
  }  

  /* Last case, which is a special treatment of slash dot dot, at the
   * end of a URL.
   */
  tmp = strstr(dir_start, "/..");
  if(tmp != NULL && tmp[3] == '\0') {
    tmp2 = tmp;
    tmp2--;
    while(tmp2 >= dir_start && *tmp2 != '/') {
      tmp2--;
    }
    if(tmp2 < dir_start)
      memmove(tmp, &tmp[3], strlen(tmp) - 2);
    else
      memmove(tmp2, &tmp[3], strlen(tmp) - 2);    
  }

  return 0;
}

/**
 * This takes a URL, absolute or relative, and tries to create an 
 * absolute URL out of it, and the stored base URL. 
 *
 * @param url The absolute or relative URL to absolutify.
 * @param base_url If not NULL, this is used as the new base URL.
 *
 * @return the new, absolutely absolute URL.
 */
char *protocol_make_absolute(char *url, char *base_url)
{
  char *new_url;
  int alloc_size, cwd_size;
  char *cwd;
  int pathcnt = 0;
  int idx = 0;

  base_path = NULL;
  base_host = NULL;

  /* If we are given a completely non-existing URL (It is possible
   * for example if we get an empty src="" option in an img tag.) 
   * we just exit. 
   */
  if(url == NULL)
    return NULL;

  /* First of all, we check if an unsupported protocol is being used.
   * If it is, we do not proceed. 
   * This is a bad method, since we cannot possible list all protocols
   * we will not support. The problem is that if a known protocol is
   * not specified, we want to try to access it as a file.
   */
  if(!strncmp(url, "ftp://", 6) ||
     !strncmp(url, "mailto:", 7) ||
     !strncmp(url, "telnet://", 9)) {
    fprintf(stderr, "Unsupported protocol used for '%s'.\n", url);
    return NULL;
  }

  /* If a new base URL was provided, we set this as the current base URL. */

  if(base_url)
    protocol_store_base(base_url);

  cwd = NULL;

  if(!strncmp(url, "http://", 7) ||
     !strncmp(url, "file://", 7)) {
    /* New absolute URL. */
    new_url = (char *)malloc(strlen(url) + 1);
    if(new_url == NULL)
      return NULL;
    strcpy(new_url, url);

  } else if(url[0] == '/') {
    /* New path only. */
    if(base_host)
      alloc_size = strlen(base_host) + strlen(url) + 1;
    else
      alloc_size = 7 + strlen(url) + 1;

    new_url = (char *)malloc(alloc_size);
    if(new_url == NULL)
      return NULL;

    if(base_host)
      strcpy(new_url, base_host);
    else
      strcpy(new_url, "file://");
    strcat(new_url, url);

  } else {
    /* Relative path. */
    alloc_size = strlen(url) + 4;
    if(base_host)
      alloc_size += strlen(base_host);
    else
      alloc_size += 7;
    if(base_path) {
      alloc_size += strlen(base_path);
    } else {
      if(base_host == NULL) {
	char *ret;

	cwd_size = 0;
	do {
	  cwd_size += 256;
	  cwd = (char *)realloc(cwd, cwd_size);
	  if(cwd == NULL)
	    return NULL;
#ifndef _WIN32_WCE
	  ret = getcwd(cwd, cwd_size - 1);
	  if(ret == NULL && errno != ERANGE) {
	    free(cwd);
	    return NULL;
	  }
#else
	  while ( fullFileName[idx] != '*'  )
	  {
/*
		  if (fullFileName[idx] == '\\')
		  {
			  if (idx == 0)
			  {
			  	cwd[pathcnt++] = '\\'; // 처음 하나일때
				idx+=2;
			  }

			  if (
			  cwd[pathcnt++] = '\\';

			  idx+=2;

		  }
*/
		  if (fullFileName[0] == '\0')
			  break;
		  if (fullFileName[idx] & 0x80)
		  {
			  cwd[pathcnt++] = fullFileName[idx++];
			  cwd[pathcnt++] = fullFileName[idx++];
		  }
		  else
		  {
			  cwd[pathcnt++] = fullFileName[idx++];
			  idx++;
		  }
	  }
	 cwd[pathcnt] = '\0';
	 ret = cwd;
	  if(ret == NULL ) {
	    free(cwd);
	    return NULL;
	  }
#endif
	} while(ret == NULL);
	alloc_size += strlen(cwd);
      }
    }
  
    new_url = (char *)malloc(alloc_size);
    if(new_url == NULL)
      return NULL;

    if(base_host) {
      strcpy(new_url, base_host);
      //strcat(new_url, "/");
    } else {
      strcpy(new_url, "file://");
    }  

    if(base_path) {
      strcat(new_url, base_path);
    } else {
      if(base_host == NULL) {
	strcat(new_url, cwd);
      }
    }
#ifndef _WIN32_WCE
    strcat(new_url, "\\");
#endif
    strcat(new_url, url);
  }

  protocol_compress_url(new_url);

  if(cwd != NULL)
    free(cwd);

  return new_url;
}

/**
 * Frees everything that has been allocated for the HTTP headers.
 *
 * @param headers A pointer to the headers to free.
 */
void protocol_free_headers(struct protocol_http_headers *headers)
{
  if(headers == NULL)
    return;

  if(headers->return_message)
    free(headers->return_message);
  if(headers->content_type_major)
    free(headers->content_type_major);
  if(headers->content_type_minor)
    free(headers->content_type_minor);
  if(headers->server)
    free(headers->server);
  if(headers->location)
    free(headers->location);
  if(headers->real_url)
    free(headers->real_url);
  
  free(headers);
}

/**
 * Gracefully closes a stream using the specified protocol scheme. 
 * This is used when an error has occurred, and the file descriptor
 * is not yet on the linked list of streams.
 *
 * @param fd The file descriptor of the stream to close.
 * @param protocol The protocol to use to close this stream.
 *
 * @return non-zero value if an error occurred.
 */
static int protocol_close_specific(int fd, enum protocol_type protocol)
{
  switch(protocol) {
  case PROTOCOL_HTTP:
    protocol_http_close(fd);
    break;

  case PROTOCOL_FILE:
    protocol_file_close(fd);
    break;

  default:
    fprintf(stderr, 
	    "%s: Unsupported protocol detected. Sorry, cannot close.\n",
	    __FUNCTION__);
    return 1;
  }

  return 0;
}

/**
 * Takes a good look at the specified URL, looks deeply into its darkest
 * attributes and corners of significancy. After that, it tries its best
 * to decide on which protocol to use to achieve a stream from the URL.
 *
 * @param url The URL to open a stream from.
 * @param referer The URL we got from to get here, or NULL if jumping here.
 * @param base_url The base URL to be used to create an absolute URL from 
 * @param base_url the given URL, or NULL to use the locally stored base.
 *
 * @return a file descriptor for the stream, or a negative value if an
 * @return error occurred.
 */
int protocol_open(char *url, char *referer, char *base_url)
{
  int fd;
  char *new_url;
  enum protocol_type protocol;
  struct protocol_stream *new_stream;
  struct protocol_http_headers *headers;
  struct protocol_url *url_parts;

  headers = (struct protocol_http_headers *)
    malloc(sizeof(struct protocol_http_headers));
  if(headers == NULL) {
    return -1;
  }

  /* Fill in the default values for the HTTP headers struct. */
  headers->return_code = 200;
  headers->return_message = NULL;
  headers->content_type_major = NULL;
  headers->content_type_minor = NULL;
  headers->content_length = -1;
  headers->server = NULL;
  headers->location = NULL;
  headers->real_url = NULL;

  if(referer)
    protocol_store_base(referer);

  /* Get the absolute equivalence to the specified URL. */
  new_url = protocol_make_absolute(url, base_url);

  protocol = PROTOCOL_UNKNOWN;
  if(new_url) {
    url_parts = protocol_split_url(new_url);
    if(url_parts) {
      /* Find the correct protocol to use. */
      if(!strcmp(url_parts->type, "http")) {
	fd = protocol_http_open(url_parts, referer, headers);
	protocol = PROTOCOL_HTTP;
      } else if(!strcmp(url_parts->type, "file")) {
	fd = protocol_file_open(url_parts->file, headers);
	protocol = PROTOCOL_FILE;
      } else {
	fprintf(stderr, "Unsupported protocol used for '%s'\n", new_url);
	fd = -1;
      }
    } else {
      fd = -1;
    }
  } else {
    fd = -1;
  }

  /* Unless there was an unexpected error, we put the new file descriptor
   * and information about it onto the linked list. Its much easier to put
   * it at the top of the list, so we do that.
   */
#if defined _WIN32_WCE || WIN32
  if(fd == -1) {
#else
  if(fd < 0) {
#endif
    if(new_url)
      free(new_url);
    protocol_free_headers(headers);
    return fd;
  }

  /* This is not really right. */
  if(headers->return_code != 200) {
    if(new_url)
      free(new_url);
    protocol_close_specific(fd, protocol);
    protocol_free_headers(headers);
    return -1;
  }

  new_stream = (struct protocol_stream *)
    malloc(sizeof(struct protocol_stream));
  if(new_stream == NULL) {
    if(new_url)
      free(new_url);
    protocol_close_specific(fd, protocol);
    protocol_free_headers(headers);
    return -1;
  }
  new_stream->fd = fd;
  new_stream->protocol = protocol;
  new_stream->headers = headers;
  new_stream->next = first_stream;
  first_stream = new_stream;

  /* If we open a HTML page, we should set the base URL strings to something
   * apropriate.
   */
  if(headers->content_type_major && headers->content_type_minor &&
     !strcmp(headers->content_type_major, "text") &&
     !strcmp(headers->content_type_minor, "html"))
    protocol_store_base(headers->real_url);

  free(new_url);

#if 0
  if(new_stream->headers) {
    fprintf(stderr, "return code: %d\n", 
	    new_stream->headers->return_code);
    fprintf(stderr, "return message: %s\n", 
	    new_stream->headers->return_message);
    fprintf(stderr, "content_type: %s/%s\n", 
	    new_stream->headers->content_type_major, 
	    new_stream->headers->content_type_minor);
    fprintf(stderr, "content_length: %d\n", 
	    new_stream->headers->content_length);
    fprintf(stderr, "server: %s\n", 
	    new_stream->headers->server);
    fprintf(stderr, "real_url: %s\n", 
	    new_stream->headers->real_url);
  }
#endif

  return fd;
}

/**
 * Closes a stream that was opened by protocol_stream_open(). It has 
 * to have been opened by that function, or it will not work. This
 * will free the memory that was allocated for the HTTP headers.
 *
 * @param fd The file descriptor of the stream to close.
 *
 * @return non-zero value if an error occurred.
 */
int protocol_close(int fd)
{
  struct protocol_stream *streamp, *previous_stream;

  /* Find the stream associated with the specified file descriptor. */
  previous_stream = NULL;
  streamp = first_stream;
  while(streamp && streamp->fd != fd) {
    previous_stream = streamp;
    streamp = streamp->next;
  }

  /* Maybe we cannot find the file descriptor in the list. What can we do
   * but to exit and tell the caller that there was an error. A thoughtful
   * alternative would be to use close() to close the unknown file
   * descriptor, but that is not really recommended.
   */
  if(streamp == NULL)
    return 1;

  /* Close the stream, and remove it from the linked list. */
  switch(streamp->protocol) {
  case PROTOCOL_HTTP:
    protocol_http_close(fd);
    break;

  case PROTOCOL_FILE:
    protocol_file_close(fd);
    break;

  default:
    fprintf(stderr, 
	    "%s: Unsupported protocol detected. Sorry, cannot close.\n",
	    __FUNCTION__);
  }

  if(previous_stream)
    previous_stream->next = streamp->next;
  else
    first_stream = streamp->next;
  
  protocol_free_headers(streamp->headers);
  free(streamp);

  return 0;
}

/**
 * Get the HTTP headers associated with a particular stream.
 *
 * @param fd The file descriptor of the stream.
 *
 * @return a pointer to the struct holding the header information, or NULL
 * @return if the stream did not have any HTTP headers, or if the stream
 * @return could not be found for the given file descriptor.
 */
struct protocol_http_headers *protocol_get_headers(int fd)
{
  struct protocol_stream *streamp;

  /* Find the stream associated with the specified file descriptor. */
  streamp = first_stream;
  while(streamp && streamp->fd != fd) {
    streamp = streamp->next;
  }
  if(streamp == NULL)
    return NULL;

  return streamp->headers;
}

/**
 * Split up a URL into its different components. Basically, it tries
 * to use the fields in the URL struct logically, depending on the
 * different known protocol types. 
 *
 * @param orgurl The full URL, just as a piece of text.
 *
 * @return a pointer to an allocated URL struct, filled in with 
 * @return nice values taken from the given URL string. 
 */
struct protocol_url *protocol_split_url(char *orgurl)
{
  char *url, *urlp, *tmp, *tmp2;
  struct protocol_url *surl;

  surl = (struct protocol_url *)malloc(sizeof(struct protocol_url));
  if(surl == NULL) {
    return NULL;
  }
  surl->type = NULL;
  surl->user = NULL;
  surl->pass = NULL;
  surl->host = NULL;
  surl->port = 0;
  surl->file = NULL;

  url = _strdup(orgurl);
  if(url == NULL)
    return NULL;

  urlp = url;
  tmp = strstr(urlp, "://");
  if(tmp != NULL) {
    *tmp = '\0';
    tmp += 3;
    surl->type = _strdup(urlp);
    urlp = tmp;
  } else if(!strncmp(urlp, "mailto:", 7)) {
    surl->type = _strdup("mailto");
    urlp += 7;
  } else {
    surl->type = _strdup("file");
  }
  if(!strcmp(surl->type, "file")) {
    surl->file = _strdup(urlp);    
  } else {
    tmp = strchr(urlp, '@');
    if(tmp != NULL) {
      *tmp++ = '\0';
      tmp2 = tmp;
      tmp = strchr(urlp, ':');
      if(tmp != NULL) {
	*tmp++ = '\0';
	surl->pass = _strdup(tmp);
      }
      surl->user = _strdup(urlp);
      urlp = tmp2;
    }
    tmp = strchr(urlp, '/');
    if(tmp != NULL) {
      *tmp++ = '\0';
      surl->file = _strdup(tmp);
    } else {
      surl->file = _strdup("");
    }
    tmp = strchr(urlp, ':');
    if(tmp != NULL) {
      *tmp++ = '\0';
      surl->port = atoi(tmp);
    }
    surl->host = _strdup(urlp);
  }

  if(surl->port == 0) {
      surl->port = protocol_default_port(surl->type);
  }

  free(url);

  return surl;
}

/**
 * Takes a protocol string, and returns its default port
 * number.
 *
 * @param protocol The protocol name, e.g. "http", "ftp"
 *
 * @return the default port number of the protocol, or
 * @return zero if unknown
 */
int protocol_default_port(char *protocol)
{
/* These should really be taken from getservbyname(3), but it does not
 * use the exact same protocol names as a URL does. Later, dude.
 */
  if(!strcmp(protocol, "http"))
    return 80;
  else if(!strcmp(protocol, "ftp"))
    return 21;
  else if(!strcmp(protocol, "mailto"))
    return 25;
  else if(!strcmp(protocol, "telnet"))
    return 23;
  else if(!strcmp(protocol, "gopher"))
    return 70;
  else
    return 0;
}

/**
 * Takes a disassembled protocol_url struct, and puts it
 * into a URL string. This function is rather HTTP-centric,
 * and might need some care to work correctly with other
 * protocols.
 *
 * @param url The URL struct
 *
 * @return a pointer to an allocated URL string, or NULL if
 * @return malloc fails
 */
char *protocol_unsplit_url(struct protocol_url *url)
{
  int alloc_size;
  char *real_url;

  alloc_size = 7 + strlen(url->host) + strlen(url->file) + 64;
  if(url->user)
    alloc_size += strlen(url->user);
  if(url->pass)
    alloc_size += strlen(url->pass);
  real_url = (char *)malloc(alloc_size);
  if(real_url == NULL) {
    return NULL;
  } else {
    strcpy(real_url, url->type);
    strcat(real_url, "://");
    if(url->user) {
      strcat(real_url, url->user);
      if(url->pass) {
	strcat(real_url, ":");
	strcat(real_url, url->pass);
      }
      strcat(real_url, "@");
    }
    strcat(real_url, url->host);
    if(url->port != (unsigned int)protocol_default_port(url->type))
      sprintf(&real_url[strlen(real_url)], ":%d",
	      url->port);
    strcat(real_url, "/");
    strcat(real_url, url->file);
  }
  return real_url;
}

/**
 * Free a URL struct from all the evil allocated space.
 *
 * @param url A pointer to a URL struct.
 */
void protocol_free_url(struct protocol_url *url)
{
  if(url) {
    if(url->type)
      free(url->type);
    if(url->user)
      free(url->user);
    if(url->pass)
      free(url->pass);
    if(url->host)
      free(url->host);
    if(url->file)
      free(url->file);

    free(url);
  }
}
