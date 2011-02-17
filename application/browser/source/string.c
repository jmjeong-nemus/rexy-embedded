/** 
 * Functions to handle the internal string used by the parser.
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
//#include <unistd.h>

#include "../header/layout.h"
#include "../header/states.h"
#include "../header/parser_helpers.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

extern isUtf8;
extern int strcasecmp(const char *s1, const char *s2);

static unsigned char *internal_string = NULL;
static int internal_allocation = 0;
static int is_preformatted = 0;

/* HTML character entities, starting at ISO-8859-1 index 160. */
static char *entities[] = { 
  "nbsp", "iexcl", "cent", "pound", "curren", "yen", "brvbar", "sect",
  "uml", "copy", "ordf", "laquo", "not", "shy", "reg", "macr",
  "deg", "plusmn", "sup2", "sup3", "acute", "micro", "para", "middot",
  "cedil", "sup1", "ordm", "raquo", "frac14", "frac12", "frac34", "iquest",
  "Agrave", "Aacute", "Acirc", "Atilde", "Auml", "Aring", "AElig", "Ccedil",
  "Egrave", "Eacute", "Ecirc", "Euml", "Igrave", "Iacute", "Icirc", "Iuml",
  "ETH", "Ntilde", "Ograve", "Oacute", "Ocirc", "Otilde", "Ouml", "times",
  "Oslash", "Ugrave", "Uacute", "Ucirc", "Uuml", "Yacute", "THORN", "szlig",
  "agrave", "aacute", "acirc", "atilde", "auml", "aring", "aelig", "ccedil",
  "egrave", "eacute", "ecirc", "euml", "igrave", "iacute", "icirc", "iuml",
  "eth", "ntilde", "ograve", "oacute", "ocirc", "otilde", "ouml", "divide",
  "oslash", "ugrave", "uacute", "ucirc", "uuml", "yacute", "thorn", "yuml"
};

/**
 * Convert a hexadecimal digit into its corresponding decimal value.
 *
 * @param hex The hexadecimal digit.
 *
 * @return the decimal value.
 */
static int parse_convert_hex(char hex)
{
  if(hex >= '0' && hex <= '9')
    return hex - '0';
  if(hex >= 'a' && hex <= 'f')
    return hex - 'a' + 10;

  return 0;
}

/**
 * Converts a string in the form #rrggbb into a 24-bit colour value.
 * Later, this will also handle named colours.
 *
 * @param colour A string containing the colour code.
 *
 * @return a 24-bit colour value.
 */
uint32_t parse_convert_colour(char *colour)
{
  uint32_t real_colour;
  int red, green, blue;

  /* Convert some well known named colours. An idea here is to check
   * for rgb.txt that comes with X, and use that for conversion.
   */
  if(colour[0] != '#' && 
     !(isxdigit((int)colour[0]) &&
       isxdigit((int)colour[1]) &&
       isxdigit((int)colour[2]) &&
       isxdigit((int)colour[3]) &&
       isxdigit((int)colour[4]) &&
       isxdigit((int)colour[5]))) {
    if(!strcasecmp(colour, "black"))
      return 0x000000;
    else if(!strcasecmp(colour, "white"))
      return 0xffffff;
    else if(!strcasecmp(colour, "red"))
      return 0xff0000;
    else if(!strcasecmp(colour, "green"))
      return 0x00ff00;
    else if(!strcasecmp(colour, "blue"))
      return 0x0000ff;
    else if(!strcasecmp(colour, "yellow"))
      return 0xffff00;
    else if(!strcasecmp(colour, "cyan"))
      return 0x00ffff;
    else if(!strcasecmp(colour, "magenta"))
      return 0xff00ff;
    else
      return 0xffffff;
  }

  red = parse_convert_hex((char)tolower((int)colour[1])) * 16 +
    parse_convert_hex((char)tolower((int)colour[2]));
  green = parse_convert_hex((char)tolower((int)colour[3])) * 16 +
    parse_convert_hex((char)tolower((int)colour[4]));
  blue = parse_convert_hex((char)tolower((int)colour[5])) * 16 +
    parse_convert_hex((char)tolower((int)colour[6]));

  real_colour = ((red   << 16) |
		 (green <<  8) |
		 (blue  <<  0));

  return real_colour;
}

/**
 * Store one character at the end of the temporary internal string.
 * More than one whitespace characters in a row are ignored. All
 * whitespace characters are converted into space characters, unless
 * the current string is preformatted.
 *
 * @param c The character to be stored.
 *
 * @return non-zero value if an error occurred.
 */
int parse_string_store_character(unsigned char c)
{
  int size;

  /* Convert all whitespace characters into spaces,
   * unless we are in preformating mode. 
   */
  if(!is_preformatted && IS_SPACE_ALPHA((unsigned int)c))
    c = ' ';

  /* If we are preformating, newline characters should create what is
   * equivalent to a br-tag.
   */
  if(is_preformatted && c == '\n') {
    struct layout_part *partp;

    parse_string_store_current();
    partp = layout_init_part(LAYOUT_PART_PARAGRAPH);
    if(partp == NULL) {
      return 1;
    }
    partp->data.paragraph.permanent = 1;
    layout_add_part(partp);

    /* Exit here, to avoid having to store the evil newline character. */
    return 0;
  }

  if(internal_string == NULL) {
    internal_string = malloc(1024);
    internal_string[0] = '\0';
    internal_allocation = 1024;
  } else if(strlen(internal_string) == (unsigned int)internal_allocation - 1) {
    unsigned char *tmp;

    tmp = realloc(internal_string, internal_allocation + 1024);
    if(tmp == NULL)
      return 1;
    internal_string = tmp;
    internal_allocation += 1024;
  }
  
  size = strlen(internal_string);

  /* Skip several whitespaces, unless preformatted text. */
  if(!is_preformatted && 
     size > 0 && internal_string[size - 1] == ' ' && c == ' ')
    return 0;

  /* Store character, but never store a newline. */
  if(c == '\n' || c == '\r') {
    internal_string[size] = '\0';
  } 
  else{
    internal_string[size] = c;
    internal_string[size + 1] = '\0';
  }

  return 0;
}

/**
 * Sets the static variable is_preformatted, simply because I do
 * not want to make it global. Modularity, my friend.
 *
 * @param value The new value of is_preformatted.
 */
void parse_string_set_preformatted(int value)
{
  is_preformatted = value;
}

/**
 * Copy the stored string into the provided buffer `buf'. It will 
 * copy the string, or a maximum of `size' characters, in which case
 * the string will not be nul terminated.
 * After the string is copied, the internal string is cleared.
 *
 * @param buf A buffer into which the string goes.
 * @param size Maximum number of characters that are copied.
 *
 * @return non-zero value if an error occurred.
 */
int parse_string_get_stored(char *buf, unsigned int *extra_char_cnts, unsigned int *special_char_cnts, int size)
{
  int i;
  int tmpsize = 0;

  if(internal_string == NULL || buf == NULL)
    return 1;
  
  for ( i = 0; internal_string[i] > 0; i++ )
  	tmpsize++;

  // for special char count
  for ( i = 0; i < tmpsize + 1; i++ )
  {
	  if (internal_string[i] == 0xfe && internal_string[i+1] <= 0xff)
	  {
		(*special_char_cnts)++;
		i++;
	  }
  }
	
  // for utf8 char count
  if ( isUtf8 )
  {
	for ( i = 0; i < tmpsize + 1; i++)
	{
		if ( internal_string[i] & 0x80 )
		{
			if ( internal_string[i] == 0xfe && internal_string[i+1] <= 0xff )
				i++;
			else
			{
				(*extra_char_cnts)++;
				i += 2;
			}
		}
	}
  }
  
  strncpy(buf, internal_string, size);

  free(internal_string);
  internal_string = NULL;

  return 0;
}

/**
 * Copy the string in buf into the internal string. To be able to
 * do that safely, first discard the old string, as if it were not
 * worth anything at all. 
 *
 * @param buf A buffer containing the string to store.
 *
 * @return non-zero value if an error occurred.
 */
int parse_string_set_stored(char *buf)
{
  int i;

  if(buf == NULL)
    return 1;

  parse_string_discard();
  
  for(i = 0 ;(unsigned int)i < strlen(buf) ; i++)
    parse_string_store_character(buf[i]);

  return 0;
}

/**
 * Trims the internal string from whitespace characters at the front and 
 * at the back of the string. If there were whitespace characters at the
 * front, we keep one of them and trim the rest off. The same thing goes
 * for the whitespace characters at the back.
 *
 * @param skip_front_space If non-zero, do not keep any leading space.
 *
 * @return non-zero value if an error occurred.
 */
int parse_string_trim(int skip_front_space)
{
  unsigned char *stringp_front, *stringp_back;
  int front_space = 0, back_space = 0;

  if(internal_string == NULL || (!skip_front_space &&
				 strlen(internal_string) == 1))
    return 0;

  if(!skip_front_space && IS_SPACE_ALPHA((int)internal_string[0]))
		front_space = 1;


  if(IS_SPACE_ALPHA((int)internal_string[strlen(internal_string) - 1]))
	back_space = 1;


  /* Find the front. */
  stringp_front = internal_string;

  while(*stringp_front && IS_SPACE_ALPHA((int)*stringp_front))
	stringp_front++;

  if(front_space)
	stringp_front--;
 

  /* Cut the back, but only if there is something left to cut. */
  if(strlen(stringp_front) > 0) {

    stringp_back = internal_string + strlen(internal_string) - 1;
	while(IS_SPACE_ALPHA((int)*stringp_back))
			stringp_back--;

	if(back_space)
		stringp_back++;

    stringp_back[1] = '\0';
    /* Move the trimmed string to the first position in the internal string. */
    memmove(internal_string, stringp_front, strlen(stringp_front) + 1);
  } else {
    internal_string[0] = '\0';
  }

  return 0;
}

/**
 * Return the length of the internal string. 
 *
 * @return the length of the string.
 */
int parse_string_get_length(void)
{
  if(internal_string == NULL)
    return 0;

  return strlen(internal_string);
}

/**
 * Empty, free and nullify the internal string.
 */
void parse_string_discard(void)
{
  if(internal_string != NULL)
    free(internal_string);

  internal_string = NULL;
}

/**
 * Find the corresponding ISO-8859-1 character for the given
 * character entity.
 *
 * Comments: &mdash; should really be a long dash, in Lynx represented
 *           by two '-'. This is however not yet supported in Zen, and
 *           only one '-' is used. Live with it.
 *
 * @param entity A string containing the character entity to find.
 *
 * @return the corresponding ISO-8859-1 character.
 */
unsigned char parse_string_find_character_entity(char *entity)
{
  int i;

  unsigned int int_entity = 0;

  if(entity[0] == '#') {
	if ( entity[1] == 'x')
	{
		if (entity[2] >= 0x61 &&  entity[2] <= 0x7a )
			int_entity = (entity[2] - 0x57) * 16;
		else 
			int_entity = (entity[2] - 0x37) * 16;

		if (entity[3] >= 0x61 &&  entity[2] <= 0x7a )
			int_entity += entity[3] - 0x57;
		else 
			int_entity += entity[3] - 0x37;

		return int_entity;
	}
	else
	   return (unsigned int)atoi(&entity[1]);
  } 
  else {
    /* First check for a number of special codes. */
    if(!strcmp(entity, "quot"))
      return '"';
    else if(!strcmp(entity, "amp"))
      return '&';
    else if(!strcmp(entity, "lt"))
      return '<';
    else if(!strcmp(entity, "gt"))
      return '>';
    else if(!strcmp(entity, "mdash"))
      return '-';
    else {
      for(i = 0 ; i < 96 ; i++) {
	
	if(!strcmp(entity, entities[i]))
	  return i + 160;
      }
    }
  }

  return 0;
}

/**
 * Convert all character entities in the internal string into
 * ISO-8859-1 characters.
 */
void parse_string_convert_character_entities(void)
{
  char *textp, *entityp, *entity;

  unsigned int new;

  int entity_length;

  if(internal_string == NULL)
    return;

  textp = internal_string;

  while(*textp != '\0') {
    while(*textp != '\0' && *textp != '&')
      textp++;
    if(*textp == '\0')
      return;
    
    entityp = textp;

	textp++;
    while(*textp != '\0' && *textp != ';')
      textp++;
    if(*textp == '\0')
      return;
    
    entity_length = textp - entityp - 1;
    entity = (char *)malloc(entity_length + 1);
    if(entity == NULL)
      return;
    strncpy(entity, entityp + 1, entity_length);
    entity[entity_length] = '\0';
    
    textp++;
    
    /* Find the correspnding character and replace it in the string. */
    new = parse_string_find_character_entity(entity);
  
	if(new > 0) {
	  *entityp++ = (char)0xfe;
	  *entityp++ = new;
      memmove(entityp, textp, strlen(textp) + 1);    
      textp = entityp;
    }

    free(entity);
  }
}

/**
 * Put the currently stored string as a textual layout part.
 * This reads the current text style from the style state stack, and
 * uses the right style for the right text.
 *
 * @return a pointer to the layout part that was just added to the list
 * @return or NULL if there was no string stored.
 */
struct layout_part *parse_string_store_current(void)
{
  char *text;
  unsigned int extra_char_cnts = 0;
  unsigned int special_char_cnts = 0;
  int string_length;
  struct layout_part *partp;
  struct layout_text_styles current_style;
  static int last_was_space = 1;
  
  /* Get the current styles of the text to be stored. */
  parse_state_get_current(&current_style, NULL, NULL);

  /* If the page says that this particular piece of text is to be rendered
   * preformatted, then so be it, and we must not modify it by cutting off
   * excess whitespaces.
   */
  if(!current_style.preformatted) {
    parse_string_trim(last_was_space);
  }

  string_length = parse_string_get_length();
  if(string_length > 0) {
    text = malloc(string_length + 1);
    if(text == NULL)
      return NULL;

    /* Convert HTML coded characters into ISO-8859-1. 
     * Only do this, if the text is not marked as direct quote.
     */
   if(!current_style.directquote)
      parse_string_convert_character_entities();

    parse_string_get_stored(text, &extra_char_cnts, &special_char_cnts, string_length + 1);
    partp = layout_init_part(LAYOUT_PART_TEXT);
    if(partp == NULL) {
      free(text);
      return NULL;
    }

    partp->data.text.text = text;
    partp->data.text.style = current_style;
	partp->data.text.extra_char_cnts = extra_char_cnts;
	partp->data.text.special_char_cnts = special_char_cnts;

    layout_add_part(partp);

    if(IS_SPACE_ALPHA((int)text[strlen(text) - 1]))
      last_was_space = 1;
    else
      last_was_space = 0;
  } else {
    /* If the string was trimmed away completely, we still need to discard
     * what might be left of it. 
     */
    parse_string_discard();
    return NULL;
  }

  return partp;
}

/**
 * Put the currently stored string as the title for the page. 
 * What is special about this is that it is always placed first in the
 * linked list or parts. 
 *
 * @return non-zero value if an error occurred.
 */
int parse_string_store_title(void)
{
  char *text;
  unsigned int extra_char_cnts = 0;
  unsigned int special_char_cnts = 0;

  int string_length;
  struct layout_part *partp;
  
  parse_string_trim(0);
  string_length = parse_string_get_length();
  if(string_length > 0) {
    text = malloc(string_length + 1);
    if(text == NULL)
      return 1;
    parse_string_get_stored(text, &extra_char_cnts, &special_char_cnts, string_length + 1);
  } else {
    /* If the string was trimmed away, save NULL in the title text. */
    parse_string_discard();
    text = NULL;
  }

  partp = layout_get_page_information(NULL);

  if(partp == NULL) {
    partp = layout_init_part(LAYOUT_PART_PAGE_INFORMATION);
    if(partp == NULL) {
      if(text)
	free(text);
      return 1;
    }
    layout_add_part(partp);
  }
  
  /* At this point, we will change the real part by changing what is in
   * the pointer partp.
   */
  partp->data.page_information.title = text;

  return 0;
}


/**
 * Prints the currently stored string.
 */
void debug_dump_string(void)
{
  if(internal_string)
    fprintf(stderr, "String: '%s'\n", internal_string);
  else 
    fprintf(stderr, "String: (null)\n");
}

