/**
 * Functions to open a stream from a server using the HTTP protocol.
 * This currently only supports HTTP/1.0 but that is enough for most
 * things.
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
#include <io.h>
//#include <unistd.h>
//#include <sys/socket.h>
#include <winsock2.h>
//#include <netinet/in.h>
//#include <netdb.h>

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif /* HAVE_STRINGS_H */

/* If threads are used, we might need redefined read and write functions. */
#include "../header/threads.h"

#include "../header/base64.h"
#include "../header/settings.h"
#include "../header/ui.h"
#include "../header/http.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

extern int strcasecmp(const char *s1, const char *s2);
extern int RalReadFile(int, char*, int);
/**
 * Open a TCP connection to the other host, on a specific port number. 
 *
 * @param url A pointer to a URL struct containing the hostname and the
 * @param url port number to use for the connection.
 *
 * @return a file descriptor for the opened stream, or a negative value
 * @return representing an error.
 */
static int protocol_http_open_connection(struct protocol_url *url)
{
  /* struct hostent *hent; */
  /* struct protoent *proto; */
  /* struct sockaddr_in saddr; */
  /* int sock; */
  char *status;

  status = (char *)malloc(32 + strlen(url->host) + 6);
  if(status != NULL) {
    if(url->port == 80)
      sprintf(status, "Contacting %s...", url->host);
    else
      sprintf(status, "Contacting %s:%d...", url->host, url->port);
    ui_functions_set_status(status);
    free(status);
  }
  /*
  proto = getprotobyname("tcp");
  if(proto == NULL) {
    return -1;
  }
  */
  /*
  sock = socket(AF_INET, SOCK_STREAM, proto->p_proto);
  if(sock < 0) {
    return -1;
  }
  */
  
  /*  
  hent = gethostbyname(url->host);
  if(hent == NULL) {
    close(sock);
    return -1;
  }
  
  memcpy(&saddr.sin_addr, hent->h_addr_list[0], sizeof(struct in_addr));
  saddr.sin_family = AF_INET;
  */

  /* saddr.sin_port = htons(url->port); */
  
  /*
  if(connect(sock, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
    close(sock);
    return -1;
  }
  */
   /* return sock; */
  return 0;
}

/**
 * Make an HTTP request to a webserver on an open stream. This reads
 * the important headers from the response and fills in the struct
 * available for this purpose.
 *
 * @param fd The file descriptor for the stream to request on.
 * @param url A pointer to a URL struct containing the things needed
 * @param url to make a request.
 * @param use_proxy A non-zero value if a proxy is used.
 * @param referer The referring URL which was used to get to the currently
 * @param referer requested page.
 * @param headers A pointer to an HTTP headers struct, which will be
 * @param headers filled in with appropriate values, taken from the
 * @param headers response from the server.
 *
 * @return non-zero value if an error occurred.
 */
static int protocol_http_make_request(int fd, struct protocol_url *url,
				      int use_proxy, char *referer, 
				      struct protocol_http_headers *headers)
{
  int error, i;
  char *referer_text, *tmp, *request, *auth_text;
  char *header_row, *status, *user_agent;
  char c;
  void *value;

  status = (char *)malloc(32 + strlen(url->host) + 6);
  if(status != NULL) {
    sprintf(status, "Contacting %s...", url->host);
    ui_functions_set_status(status);
    free(status);
  }

  /* Create header text for a referer header, if referer is given. */
  if(referer != NULL) {
    referer_text = (char *)malloc(strlen(referer) + 16);
    if(referer_text == NULL) {
      return -1;
    }
    sprintf(referer_text, "Referer: %s\r\n", referer);
  } else {
    referer_text = "";
  }

  /* Create header text for authorization, if user and pass are given. */
  if(url->user != NULL && url->pass != NULL) {
    auth_text = (char *)malloc((strlen(url->user) + 
				strlen(url->pass)) * 2 + 32);
    if(auth_text == NULL) {
      return -1;
    }
    tmp = (char *)malloc((strlen(url->user) + strlen(url->pass)) * 2 + 32);
    if(tmp == NULL) {
      free(auth_text);
      return -1;
    }
    sprintf(auth_text, "%s:%s", url->user, url->pass);
    base64_encode(auth_text, strlen(auth_text), tmp);
    sprintf(auth_text, "Authorization: Basic %s\r\n", tmp);
    free(tmp);
  } else {
    auth_text = "";
  }

  /* Let us see what User-Agent we should disguise ourselves as today. */
  user_agent = (char *)malloc(1024);
  if(user_agent == NULL)
    return -1;
  settings_get("user_agent_identifier", &value);
  tmp = (char *)value;
  if(tmp == NULL) {
    sprintf(user_agent, "Zen/%s %s/%s", 
	    VERSION, user_interface.ui_name, user_interface.ui_version_text);
  } else {
    strncpy(user_agent, tmp, 1023);
    user_agent[1023] = '\0';
  }

  /* Create the HTTP request to retreive an object from the server. */
  request = (char *)malloc(16384);
  if(request == NULL) {
    free(user_agent);
    return -1;
  }
  if(use_proxy) {
    sprintf(request,
	    "GET %s://%s:%d/%s HTTP/1.0\r\n"
	    "User-Agent: %s\r\n"
	    "Host: %s\r\n"
	    "Accept: */*\r\n"
	    "%s"
	    "%s"
	    "\r\n", 
	    url->type, url->host, url->port, url->file, 
	    user_agent,
	    url->host,
	    referer_text,
	    auth_text);
  } else {
    sprintf(request,
	    "GET /%s HTTP/1.0\r\n"
	    "User-Agent: %s\r\n"
	    "Host: %s\r\n"
	    "Accept: */*\r\n"
	    "%s"
	    "%s"
	    "\r\n", 
	    url->file, 
	    user_agent,
	    url->host,
	    referer_text,
	    auth_text);
  }

#ifdef DEBUG
  fprintf(stderr, "Request:\n%s\n", request);
#endif /* DEBUG */

  free(user_agent);

  write(fd, request, strlen(request));

  /* Take care of the response from the server. 
   * Fill in the header struct with appropriate values.
   * This also have to take into account stupidly implemented HTTP servers
   * which send only LF instead of CRLF as the RFC clearly specifies.
   */
  error = 0;
  header_row = (char *)malloc(1024);
  if(header_row == NULL) {
    if(strlen(referer_text) > 0)
      free(referer_text);
    free(request);
    return -1;
  }

  /* First read the response code. */
  headers->return_code = 404;

  if(RalReadFile(fd, header_row, 9) > 0) {
    header_row[9] = '\0';
    if(!strncmp(header_row, "HTTP/", 5) && RalReadFile(fd, header_row, 4) > 0) {
      headers->return_code = atoi(header_row);
      i = 0;
      while(RalReadFile(fd, &c, 1) > 0 && c != '\r' && c != '\n') {
		if(i < 1023)
		header_row[i++] = c;
      }
      if(c == '\r')
	RalReadFile(fd, &c, 1);
      if(i > 0) {
	header_row[i] = '\0';
	tmp = (char *)malloc(strlen(header_row) + 1);
	if(tmp == NULL)
	  error = 1;
	strcpy(tmp, header_row);
	headers->return_message = tmp;      
      }
    }
  }
  if(strlen(referer_text) > 0)
    free(referer_text);
  free(request);

  headers->location = NULL;
  headers->server = NULL;
  headers->content_type_major = NULL;
  headers->content_type_minor = NULL;
  headers->charset = NULL;
  headers->content_length = 0;

  /* Read all headers and save the valuable ones. 
   * Again, make sure unproper HTTP servers also gets treated.
   */
  i = 0;
  while(!error && read(fd, &c, 1) > 0) {
    if(c == '\r' || c == '\n') {
      char *field, *value;

      /* If we come to a CR, always expect an LF. */
      if(c == '\r')
	read(fd, &c, 1);

      /* If no characters have been read since previous row, it means the
       * actual stream data is coming next, right after another line feed.
       */
      if(i == 0 && c == '\n')
	break;

      header_row[i] = '\0';
      i = 0;
      field = header_row;
      value = strstr(header_row, ": ");
      if(value == NULL)
	continue;
      value[0] = '\0';
      value += 2;

      /* Figure out where to put the data. */
      if(!strcmp/*strcasecmp*/(field, "content-type")) {
	char *divider;

	tmp = (char *)malloc(strlen(value) + 1);
	if(tmp == NULL) {
	  error = 1;
	  continue;
	}
	strcpy(tmp, value);
	headers->content_type_major = tmp;

	divider = strchr(tmp, '/');
	if(divider == NULL)
	  continue;
	divider[0] = '\0';
	tmp = (char *)malloc(strlen(&divider[1]) + 1);
	if(tmp == NULL) {
	  error = 1;
	  continue;
	}
	strcpy(tmp, &divider[1]);
	headers->content_type_minor = tmp;

	/* Look for other options for the content type. This is most
	 * likely the charset. Currently, we do not use the charset
	 * value for anything, but always assume ISO-8859-1 throughout
	 * the whole program. Think what you like about that. 
	 *
	 * Note that I have done this simple, and it is not the charset
	 * which is placed in headers->charset, but the whole option
	 * string. This will change, when there is a need to use the
	 * charset value, or when I have nothing better to do, whichever
	 * comes first.
	 */
	divider = strchr(tmp, ';');
	if(divider == NULL)
	  continue;
	divider[0] = '\0';
	tmp = (char *)malloc(strlen(&divider[1]) + 1);
	if(tmp == NULL) {
	  error = 1;
	  continue;
	}
	strcpy(tmp, &divider[1]);
	headers->charset = tmp;
      } else if(!strcasecmp(field, "server")) {
	tmp = (char *)malloc(strlen(value) + 1);
	if(tmp == NULL) {
	  error = 1;
	  continue;
	}
	strcpy(tmp, value);
	headers->server = tmp;
      } else if(!strcasecmp(field, "location")) {
	tmp = (char *)malloc(strlen(value) + 1);
	if(tmp == NULL) {
	  error = 1;
	  continue;
	}
	strcpy(tmp, value);
	headers->location = tmp;
      } else if(!strcasecmp(field, "content-length")) {
	headers->content_length = atol(value);
      }
    } else {
      if(c != '\n' && i < 1023)
	header_row[i++] = c;
    }
  }

  free(header_row);

  return error;
}

/**
 * Opens an HTTP stream from a web server. 
 *
 * @param url The name of the URL to open.
 * @param referer The URL we were at when entering this new URL.
 * @param headers A pointer to a struct meant to contain information gathered
 * @param headers from the HTTP headers in the response from the server. If
 * @param headers this is set to NULL, do not even try to store any headers.
 *
 * @return the file descriptor for the http stream or a negative value
 * @return if an error occurred.
 */
int protocol_http_open(struct protocol_url *url, char *referer, 
		       struct protocol_http_headers *headers)
{
  int fd;
  int error, using_proxy;
  char *tmp, *absolute_relocation;
  void *value;
  struct protocol_url *proxy_url, *relocation_url;

  /* Check if we should use a proxy for this request. */
  settings_get("http_proxy", &value);
  if(value != NULL) {
    proxy_url = protocol_split_url((char *)value);
    using_proxy = 1;
  } else {
    proxy_url = NULL;
    using_proxy = 0;
  }

  /* Connect and make the request, perhaps through a proxy, perhaps not. */  
  if(using_proxy)
    fd = protocol_http_open_connection(proxy_url);
  else
    fd = protocol_http_open_connection(url);

  if(fd == -1) {
    protocol_free_url(proxy_url);
    return fd;
  }
  protocol_http_make_request(fd, url, using_proxy, referer, headers);

  /* Deal with the return code as we see fit. 
   * Fill in more return codes when we find out what they really stand for.
   */
  switch(headers->return_code) {
    /* Not found. */
  case 404:
    /* If it could not find the page, try adding an ending slash, to get it as
     * a directory. Only do this, if the file did not have an ending slash 
     * already.
     */
    if(url->file && url->file[strlen(url->file) - 1] != '/') {
      /* Close the old stream. */
      close(fd);
      
      /* Open it again. */
      if(using_proxy)
	fd = protocol_http_open_connection(proxy_url);
      else
	fd = protocol_http_open_connection(url);

#if defined _WIN32_WCE || WIN32
  if(fd == (int)INVALID_HANDLE_VALUE)
#else
  if(fd < 0)
#endif  
	break;

      tmp = (char *)malloc(strlen(url->file) + 2);
      if(tmp == NULL) {
	fd = -1;
	break;
      }
      strcpy(tmp, url->file);
      strcat(tmp, "/");
      free(url->file);
      url->file = tmp;
      protocol_http_make_request(fd, url, using_proxy, referer, headers);

      /* There is the possibility here that the new return code is not 404 or
       * 200. However, we ignore that possibility now, and only accept 200 as
       * being ok.
       */
      if(headers->return_code != 200) {
	close(fd);
	fd = -1;
	break;
      }
    } else {
      close(fd);
      fd = -1;
      break;
    }

    /* Last break is deliberately missing, because if we get here, the new
     * return code was 200, and then we should go on as usual.
     */

    /* OK. */
  case 200:
    /* Store the complete URL used for reading. */
    headers->real_url = protocol_unsplit_url(url);
    if(headers->real_url == NULL) {
      error = 1;
    }
    break;

    /* Relocation. */
  case 301:
  case 302:
    /* If we get codes 301 or 302, it means the page has moved permantently
     * or temporarily. Either way, we recursively call protocol_http_open
     * again, with the new location.
     * Later, we will have to check for endless loops of these. We cannot 
     * allow evil system administrators to crash the program.
     */
    /* Close the old stream. */
    close(fd);

    /* Set the status to something informative. */
    tmp = (char *)malloc(strlen(headers->location) + 64);
    if(tmp == NULL)
      break;
    sprintf(tmp, "Relocated to %s", headers->location);
    ui_functions_set_status(tmp);
    free(tmp);

    /* According to the HTTP standard, the Location must be
     * absolute, starting with "http://". But what if it isn't?
     * The following code assumes that if Location is relative,
     * it is relative to the URL that returned 30x.
     */
    tmp = protocol_unsplit_url(url);
    absolute_relocation = protocol_make_absolute(headers->location, tmp);
    relocation_url = protocol_split_url(absolute_relocation);
    free(absolute_relocation);
    free(tmp);
    if(relocation_url)
      /* What should be in the Referer field: the original referer,
       * or the URL that returned 30x? Well, NULL is never wrong...
       */
      fd = protocol_http_open(relocation_url, NULL, headers);    
    else
      fd = -1;
    protocol_free_url(relocation_url);
    break;

    /* Since we do not know what to do at this point, we just say that
     * the page was not found, and let the program act as if it was not.
     */
  default:
    fprintf(stderr, "Unknown return code %d at http://%s/%s port %d.\n",
	    headers->return_code, url->host, url->file, url->port);
    headers->return_code = 404;
    fd = -1;
  }

  return fd;
}

/**
 * Close an HTTP stream.
 *
 * @param fd The file descriptor associated with the stream to close.
 */
int protocol_http_close(int fd)
{
  close(fd);

  return 0;
}
