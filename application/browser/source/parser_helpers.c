/** 
 * Various functions that will aid the parsing of the input stream.
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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32_WCE
	#include <io.h>
#endif
//#include <unistd.h>

/* For special versions of read and write */
#ifdef HAVE_PTHREAD_H
#include "../header/threads.h"
#endif

#include "../header/tags.h"
#include "../header/parser_helpers.h"
#include "../header/ui.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

extern int RalReadFile(int, char*, int);
/**
 * As the name suggests, this function will get one byte from the stream.
 * It does this in a bit more efficient way than just doing a read(), since
 * it reads blocks of data to a buffer first.
 *
 * Comments: Right now, the number of bytes read each time is fixed. If need
 *           be, I will put this as a config file option in the future.
 *
 * @param fd The file descriptor from where to read data. 
 * @param c A pointer to a character where the byte is placed.
 *
 * @return code similar to what a read would return, that is a 1 if the byte
 * @return was read successfully, a 0 if the stream ended before we could read
 * @return the character, or a negative value if an error occurred. 
 */
int parse_get_byte(int fd, char *c)
{
  static char buffer[16384];
  static int index = 0, left = 0;
  static size_t total = 0;
  char *status;

  if(left == 0) {
    left = RalReadFile(fd, buffer, 16384);
    total += left;

    status = (char *)malloc(256);
    if(status == NULL)
      return -1;
    sprintf(status, "Read %d bytes...", (int)total); 
    ui_functions_set_status(status);
    free(status);

    if(left <= 0) {
      total = 0;
      ui_functions_set_status("Done reading page.");
      return left;
    }
    index = 0;
  }

   if ((((unsigned char)buffer[index]) == 0xEF )&&
	  (((unsigned char)buffer[index+1]) == 0xBB )&&
	  (((unsigned char)buffer[index+2]) == 0xBF ))
	{
	  index+=3;
	  left-=3;
	}

  *c = buffer[index++];
  left--;

  return 1;
}

/**
 * Allocates a parse_param struct and initialises its name and value.
 *
 * @param name The name of the parameter.
 * @param value The value of the parameter.
 *
 * @return an allocated parse_param struct or NULL if an error occurred.
 */
struct parse_param *parse_alloc_param(char *name, char *value)
{
  struct parse_param *paramp;
  char *used_value;
  int index;

  paramp = (struct parse_param *)malloc(sizeof(struct parse_param));
  if(paramp == NULL)
    return NULL;

  /* Initialise name */
  paramp->name = (char *)malloc(strlen(name) + 1);
  if(paramp->name == NULL) {
    free(paramp);
    return NULL;
  }

  /* Copy parameter name and convert it to lower case. */
  for(index = 0 ; (unsigned int)index < strlen(name) ; index++)
    paramp->name[index] = tolower(name[index]);
  paramp->name[strlen(name)] = '\0';

  /* If this was a parameter without a value. In the style of XML (I think),
   * we then use the name of the parameter as the value as well.
   */
  if(value == NULL)
    used_value = name;
  else
    used_value = value;

  /* Initiate the value. */
  paramp->value = (char *)malloc(strlen(used_value) + 1);
  if(paramp->value == NULL) {
    free(paramp->name);
    free(paramp);
    return NULL;
  }
  strcpy(paramp->value, used_value);

  paramp->next = NULL;

  return paramp;
}

/**
 * Free the complete memory of a parse_param struct.
 * It only frees the given object, and does not follow the
 * link to the next parameter.
 *
 * @param paramp A pointer to the parse_param object to be freed.
 *
 * @return a non-zero value if an error occurred.
 */
int parse_free_param(struct parse_param *paramp)
{
 if(paramp->name != NULL)
    free(paramp->name);

 if(paramp->value != NULL)
    free(paramp->value);

 free(paramp);

  return 0;
}

/**
 * Look very carefully for a parameter that matches the given parameter
 * name, and come up with the suitable value for that parameter.
 *
 * @param paramp A pointer to the first parse_param object in a linked list.
 * @param name The name of the parameter to look for.
 *
 * @return a pointer to the value for the given parameter name, or NULL
 * @return if the parameter name could not be found.
 */
char *parse_get_param_value(struct parse_param *paramp, char *name)
{
  while(paramp) {
    if(!strcmp(paramp->name, name))
      return paramp->value;

    paramp = paramp->next;
  }

  return NULL;
}

/**
 * Allocates the a parse_tag struct and initialises its name and type. 
 *
 * @param name The name of the parameter.
 * @param type The type of the parameter.
 *
 * @return an allocated parse_tag struct or NULL if an error occurred.
 */
struct parse_tag *parse_alloc_tag(const char *name, enum parse_tag_type type)
{
  struct parse_tag *tagp;
  int index;

  tagp = (struct parse_tag *)malloc(sizeof(struct parse_tag));
  if(tagp == NULL)
    return NULL;

  /* Initialise name */
  tagp->name = (char *)malloc(strlen(name) + 1);
  if(tagp->name == NULL) {
    free(tagp);
    return NULL;
  }

  /* Copy tag name and convert it to lower case. */
  for(index = 0 ; (unsigned int)index < strlen(name) ; index++)
    tagp->name[index] = tolower(name[index]);
  tagp->name[strlen(name)] = '\0';

  /* Initialise type */
  tagp->type = type;

  /* Initialise linked list */
  tagp->first = NULL;

  return tagp;
}

/**
 * Free the complete memory of a parse_tag struct. 
 * Automatically frees the parameter list as well.
 *
 * @param tagp A pointer to the parse_tag object to be freed.
 *
 * @return a non-zero value if an error occurred.
 */
int parse_free_tag(struct parse_tag *tagp)
{
  struct parse_param *paramp;

  if(tagp->name != NULL)
    free(tagp->name);

  /* Free all allocated parameters from the list. */
  paramp = tagp->first;
  while(paramp != NULL) {
    struct parse_param *next_paramp = paramp->next;
    int retval;

    retval = parse_free_param(paramp);
    if(retval != 0)
      return retval;

    paramp = next_paramp;
  }

  free(tagp);

  return 0;
}

/**
 * Read one word from the input stream, up to a delimiter character.
 * Delimiter characters are defined in the string given by the
 * parameter `delimeters'.
 *
 * @param fd File descriptor of input stream.
 * @param word Pointer to a string variable where the word is stored.
 * @param first Put this character in as first character in the word.
 * @param first If it is 0 (zero), do not put any character in.
 * @param delimiters A string of characters which mark the end of the word.
 *
 * @return -1 if an error occurred, zero if the stream ended or
 * @return a positive number containing the delimiter character which 
 * @return ended the word.
 */
int parse_read_word(int fd, char **word, int first, char *delimiters)
{
  char *tmp=NULL;
  unsigned char c;
  int current=0, index=0, done=0;
  int amount_read;

  while(!done) {
    /* Current buffer has come to an end, please allocate a new. */
    if(index == current) {
      char *newtmp;
      current += 1024;
      newtmp = realloc(tmp, current);
      if(newtmp == NULL) {
	if(tmp != NULL)
	  free(tmp);
	return -1;
      }
      tmp = newtmp;
    }
 
    if(first == 0) {
      /* Read one character from the stream. */
      amount_read = parse_get_byte(fd, &c);
    } else {
      /* Use the input character instead, and simulate a read. */
      c = first;
      amount_read = 1;
      first = 0;
    }

    if(amount_read <= 0) {
      /* If amount_read is zero, the stream ended. Interpret this as 
       * an error, just as if read returned -1.
       */
      free(tmp);
      return -1;
    } else if(strchr(delimiters, c) != NULL) {
      tmp[index++] = '\0';
      *word = (char *)realloc(tmp, index);
      if(*word == NULL) {
	free(tmp);
	return -1;
      }
      done = 1;
    } else {
      /* A character which is not a delimiter was read. */
      tmp[index++] = c;
    }	     
  }

  return (int)c;
}

/**
 * Skip characters in the input stream.
 * Which characters to skip are defined in the string given by the
 * parameter `skippers'. Since the last character should not be 
 * skipped, and we read through an unknown stream, we cannot put it
 * back into the stream. This function instead returns the first
 * character which was not skipped.
 *
 * @param fd File descriptor of input stream.
 * @param skippers A string of characters which are to be smoked.
 *
 * @return -1 if an error occurred, zero if the stream ended or
 * @return a positive number containing the first character which 
 * @return was not a skipper.
 */
int parse_skip_leading(int fd, char *skippers)
{
  unsigned char c;
  int done=0;
  size_t amount_read;

  while(!done) {
    /* Read one character from the stream. */
    amount_read = parse_get_byte(fd, &c);

    if(amount_read == -1) {
      /* An error occurred while reading. */
      return -1;
    } else if(amount_read == 0) {
      /* The stream ended in a mysterious way. */
      return 0;
    } else if(strchr(skippers, c) == NULL) {
      /* The character was not a skipper! */
      done = 1;
    }
  }
  return (int)c;
}

/**
 * Skips everything up to a string looking like "-->" on the stream.
 * This marks the end of a comment tag. Feature leap: It now also
 * works for "->" only, due to major fuck ups in Netscape's stupid
 * parser. Argh. *calming down* *deep breath*
 *
 * @param fd File descriptor of input stream.
 */
void parse_skip_comment(int fd)
{
  int nr_of_hyphens;
  unsigned char c;

  nr_of_hyphens = 0;
  while(1) {
    if(parse_get_byte(fd, &c) <= 0)
      break;

    if(nr_of_hyphens < 1) {
      if(c == '-')
	nr_of_hyphens++;
      else
	nr_of_hyphens = 0;
    } else {
      if(c == '>')
	break;
      else if(c != '-')
	nr_of_hyphens = 0;      
    }
  }
}

/**
 * Extract the tag and its parameters from the input stream.
 *
 * @param fd File descriptor of input stream.
 *
 * @return pointer to a parse_tag struct where the result is stored
 * @return or NULL if no tag could be retrieved.
 */
struct parse_tag *parse_get_tag(int fd)
{
  struct parse_tag *tagp;
  char *name, ch;
  int c, ending;
  enum parse_tag_type type;

  c = parse_skip_leading(fd, " \t\n\r");
  if(c <= 0) {
    return NULL;
  }

  if(c == '/') { /* End tag. */
    ending = parse_read_word(fd, &name, 0, "> \t\n\r");
    type = PARSE_TAG_END;
  } else if(c == '!') { /* Commentary tag. */
    if(parse_get_byte(fd, &ch) <= 0)
      return NULL;
    if(ch == '-')
      parse_skip_comment(fd);
    else
      parse_read_word(fd, &name, c, ">");
      
    return NULL;
  } else { /* Start tag. */
    ending = parse_read_word(fd, &name, c, "> \t\n\r");
    type = PARSE_TAG_START;
  }
  /* If we bumped into the end of the stream, we cannot consider this 
   * to be a valid tag
   */
  if(ending <= 0) {
    return NULL;
  }

  tagp = parse_alloc_tag(name, type);
  free(name);
  if(tagp == NULL) {
    return NULL;
  }

  /* We have already reached the end of the tag. No parameters. */
  if(ending == '>')
    return tagp;

  /* Here we know a space character is the last one read. */
  ending = parse_skip_leading(fd, " \t\n\r");
  if(ending <= 0) {
    parse_free_tag(tagp);
    return NULL;
  }

  while(ending != '>') {
    char *param_name, *param_value;
    struct parse_param *tmpparamp;

    tmpparamp = NULL;

    /* Here we have the start of the parameters to the tag. */
    ending = parse_read_word(fd, &param_name, ending, "=> \t\n\r");
    if(ending <= 0) {
      parse_free_tag(tagp);
      return NULL;
    }

    if(ending != '>' && ending != '=') {
      ending = parse_skip_leading(fd, " \t\n\r");
      if(ending <= 0) {
	free(param_name);
	parse_free_tag(tagp);
	return NULL;
      }
    }

    if(ending != '=') {
      /* This parameter did not have a value. */
      tmpparamp = parse_alloc_param(param_name, NULL);
      free(param_name);
      if(tmpparamp == NULL) {
	parse_free_tag(tagp);
	return NULL;
      }

    } else if(ending == '=') {
      /* There is definitely a value to this parameter, or should be. */
      ending = parse_skip_leading(fd, " \t\n\r");
      if(ending <= 0) {
	free(param_name);
	parse_free_tag(tagp);
	return NULL;
      }

      /* If the first character is a quote, the parameter value should
       * only be terminated by a second quote.
       */ 
      if(ending == '"') {
	ending = parse_read_word(fd, &param_value, 0, "\"");
      } else if(ending == '\'') {
	ending = parse_read_word(fd, &param_value, 0, "'");
      } else {
	ending = parse_read_word(fd, &param_value, ending, "> \t\n\r");
      }
      if(ending <= 0) {
	free(param_name);
	parse_free_tag(tagp);
	return NULL;
      }

      if(ending != '>') {
	ending = parse_skip_leading(fd, " \t\n\r");
	if(ending <= 0) {
	  free(param_name);
	  free(param_value);
	  parse_free_tag(tagp);
	  return NULL;
	}
      }
      
      /* Allocate the parameter struct. */
      tmpparamp = parse_alloc_param(param_name, param_value);
      free(param_name);
      free(param_value);
      if(tmpparamp == NULL) {
	parse_free_tag(tagp);
	return NULL;
      }
    }

    /* Put the new parameter, if any, first in the linked list. */
    if(tmpparamp != NULL) {
      tmpparamp->next = tagp->first;
      tagp->first = tmpparamp;
    }
  }

  return tagp;
}


/**
 * Prints all contents of a parse_param struct.
 *
 * @param paramp The parse_param struct to print out.
 */
void debug_dump_param(struct parse_param *paramp)
{
  fprintf(stderr, "Name: %s", paramp->name);

  if(paramp->value != NULL)
    fprintf(stderr, "   Value: %s", paramp->value);

  fprintf(stderr, "\n");
}

/**
 * Prints all contents of a tag struct, including all parameters
 * in the linked list.
 *
 * @param tagp The parse_tag struct to print out.
 */
void debug_dump_tag(struct parse_tag* tagp)
{
  struct parse_param *paramp;

  fprintf(stderr, "-- beginning of tag --\n");

  fprintf(stderr, "Name: %s\n", tagp->name);
  fprintf(stderr, "Type: ");
  if(tagp->type == PARSE_TAG_START)
    fprintf(stderr, "Start\n");
  else if(tagp->type == PARSE_TAG_END)
    fprintf(stderr, "End\n");
  else
    fprintf(stderr, "Unknown (This should not happen!)\n");

  /* Dump all parameters in the linked list. */
  paramp = tagp->first;
  if(paramp != NULL)
    fprintf(stderr, "Parameters: \n");
  while(paramp != NULL) {
    debug_dump_param(paramp);
    paramp = paramp->next;
  }

  fprintf(stderr, "-- end of tag --\n");
}

