/**
 * This file contains functions that takes care of reading and
 * processing the different configuration files, both local and
 * global. The results are entered into the configuration structs. 
 * This also contains functions to set and request settings, that 
 * the rest of the program can use. Another thing is reading and
 * interpreting of environment variables and command line options. 
 * It is all taken care of in here. 
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
//#include <file.h>
#include <io.h>
//#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <pwd.h>

#ifdef HAVE_GETOPT_H
#include <getopt.h>
#endif /* HAVE_GETOPT_H */

#include "../header/settings.h"

/**
 * The global structure that contains all possible settings, read
 * from the configuration files and the command line options.
 */
static struct zen_settings *settings = NULL;

/* The name of the program we are currently running as. */
static char *program_name;

/* Prototypes for functions in this file. */
static void print_usage(void);
static void print_version(void);
static char *find_homedir(void);
static void set_defaults(void);
static int interpret_options(int argc, char *argv[]);
static void interpret_environment(void);
static void read_configuration(char *filename, int warn_missing);
static void settings_free(struct zen_settings *setting);
static enum zen_settings_type find_type(char *setting);

/* Enumeration for states in the configuration parsing. */
enum configuration_state {
  NEWLINE = 0x00,
  COMMENT = 0x01,
  SETTING = 0x10,
  VALUE = 0x12,
  SINGLE_QUOTE = 0x20,
  DOUBLE_QUOTE = 0x22
};

/**
 * This is the function called from the main program. It makes sure 
 * settings from all the different directions are entered and centered
 * into the main settings in the appropriate manner and order.
 * 
 * Settings are set in this order:
 *  1. Default settings, hardcoded into the program.
 *  2. Global configuration file, /etc/zen/main.conf
 *  3. User configuration file, ~/.zen/main.conf
 *  4. Extra configuration files, given on command line.
 *  5. Environment variables.
 *  6. Command line options. 
 *
 * @param argc The number of arguments given to the program.
 * @param argv An array of strings with the given arguments.
 */
void settings_read(int argc, char *argv[])
{
  char *filename;
  /* Set the program name globally, to simplify for me, and me alone. */
  program_name = argv[0];

  /* 1. Default settings, hardcoded into the program. */
  //set_defaults();

  /* 2. Global configuration file, /etc/zen/main.conf */
  //read_configuration(GLOBAL_CONFIGDIR "\\main.conf", 0);

  /* 3. User configuration file, ~/.zen/main.conf */
  //homedir = find_homedir();
  //filename = (char *)malloc(strlen(homedir) + 32);
  //if(filename != NULL) {
  //  sprintf(filename, "%s/.zen/main.conf", homedir);
  //  read_configuration(filename, 0);
  //  free(filename);
  //}
  //added by wichang

  filename = "index.html";
  //strcpy ( filename, "index.html");
  /* 4. Extra configuration files, given on command line.
   * 5. Environment variables.
   * 6. Command line options.
   */
  interpret_options(argc, argv);
}

/**
 * This function figures out what interface is used, and reads the
 * two different configuration files, the global and the user one.
 */
void settings_read_interface(void)
{
  char *interface, *global_filename, *user_filename;
  char *homedir;
  void *value;

  /* Get the name of the interface. */
  settings_get("interface", &value);
  interface = (char *)value;

  /* Global configuration file, /etc/zen/<interface>.conf */
  global_filename = (char *)malloc(strlen(GLOBAL_CONFIGDIR) +
				   strlen(interface) + 42);
  if(global_filename == NULL)
    return;
  sprintf(global_filename, "%s/%s.conf", GLOBAL_CONFIGDIR, interface);
  read_configuration(global_filename, 0);
  free(global_filename);

  /* User configuration file, ~/.zen/<interface>.conf */
  homedir = find_homedir();
  user_filename = (char *)malloc(strlen(homedir) + 
				 strlen(interface) + 42);
  if(user_filename == NULL)
    return;
  sprintf(user_filename, "%s/.zen/%s.conf", homedir, interface);
  read_configuration(user_filename, 0);
  free(user_filename);
}

/**
 * Find the directory that would correspond best to the home directory
 * of the current user. This does not allocate a new area for the string,
 * so it should not be altered by the caller.
 *
 * @return a string containing the home directory.
 */
static char *find_homedir(void)
{
/*
  static char *homedir;
  static struct passwd *pwent;
*/
  /* First check with the HOME environment variable. */
/*  
  homedir = getenv("HOME");
  if(homedir != NULL)
    return homedir;
*/
  /* Second look in the passwd entry. */
/*  
  pwent = getpwuid(geteuid());
  if(pwent && pwent->pw_dir) {
    return pwent->pw_dir;
  }
*/
  /* In worst case, use "/tmp". */
  return "/tmp";
}

/**
 * Sets the default settings for this particular program, called Zen.
 * These are the hard coded default values. They are likely to be 
 * overridden by configuration files, environment variables and/or
 * command line options. 
 */
static void set_defaults(void)
{
  settings_set("dump_source", (void *)0, SETTING_BOOLEAN); //2
  settings_set("dump_config", (void *)0, SETTING_BOOLEAN); //2
  settings_set("interface", (void *)"dump", SETTING_STRING);//3
  settings_set("default_page", (void *)"http://www.nocrew.org/software/zen/",
	       SETTING_STRING);//3
  settings_set("screen_gamma", (void *)"2.42", SETTING_STRING);//3
  settings_set("png_gamma", (void *)"0.45455", SETTING_STRING);//3
}

/**
 * Interpret the options given to the program on the command line.
 *
 * @param argc The number of arguments given to the program.
 * @param argv An array of strings with the given arguments.
 *
 * @return the index of the first argument which did not contain an option.
 */
static int interpret_options(int argc, char *argv[])
{

  int setting_dump_source = 0;
  int setting_dump_config = 0;
  char *setting_interface = "";
  int set_dump_source=0, set_dump_config=0, set_interface=0;

#ifdef HAVE_GETOPT_LONG
  struct option long_options[] = {
    { "interface", required_argument, NULL, 'i' },
    { "source", no_argument, NULL, 's' },
    { "config", required_argument, NULL, 'c' },
    { "dump-config", no_argument, NULL, 'd' },
    { "help", no_argument, NULL, 'h' },
    { "version", no_argument, NULL, 'V' },
    { NULL, 0, NULL, 0 } };
#endif /* HAVE_GETOPT_LONG */

  char *short_options = "i:sc:dhV";

  /* First check if we have started Zen as another name than "zen". This
   * can happen, since we create symbolic links for some interfaces when
   * installing Zen. These symbolic links are:
   *
   * gzen - GTK+ user interface
   * fbzen - oFBis user interface
   * pszen - PostScript dump interface
   *
   * The symbolic name can still be overridden by the command line option
   * --interface (-i), but not by the environment variable ZEN_INTERFACE.
   */
 /* 
  real_program_name = strrchr(program_name, '/');
  if(real_program_name == NULL)
    real_program_name = program_name;
  else
    real_program_name++;
  
  if(!strcmp(real_program_name, "gzen")) {
    setting_interface = "gtk";
    set_interface = 1;
  } else if(!strcmp(real_program_name, "fbzen")) {
    setting_interface = "ofbis";
    set_interface = 1;
  } else if(!strcmp(real_program_name, "pszen")) {
    setting_interface = "psdump";
    set_interface = 1;
  }
*/
 // arg = 0;
 // while(arg != EOF) {

#ifdef HAVE_GETOPT_LONG
    arg = getopt_long(argc, argv, short_options, long_options, NULL);
#else /* !HAVE_GETOPT_LONG */
    //arg = getopt(argc, argv, short_options);
#endif /* !HAVE_GETOPT_LONG */
/*
    switch(arg) {
    case EOF: //* End of arguments //
      break;

    case 'i': ///* --interface //
      setting_interface = optarg;
      set_interface = 1;
      break;

    case 's': ///* --source //
      setting_dump_source = 1;
      set_dump_source = 1;
      break;

    case 'c': ///* --config /
      read_configuration(optarg, 1);
      break;

    case 'd': ///* --dump-config //
      setting_dump_config = 1;
      set_dump_config = 1;
      break;

    case 'h': ///* --help //
      print_usage();
      exit(0);

    case 'V': ///* --version //
      print_version();
      exit(0);

    default:
      fprintf(stderr, "-%c: Unknown option skipped\n", arg);
    }
  }
*/
	setting_interface = "ofbis";
	set_interface = 1;

  /* Read and interpret environment variables. This has to be done here,
   * to be able to get it to read the configurations in the order we have
   * specified above and in the manpage.
   */
  interpret_environment();

  /* At this point, we have treated all command line options, and put the
   * settings for them into the local struct command_line_settings. We do
   * that, because we always want the command line options to override the
   * settings from any configuration file, even those read through the 
   * --config (-c) option. 
   * So, what we do here, is copy the command line settings into the main
   * settings struct.
   */
  if(set_dump_source)
    settings_set("dump_source", (void *)setting_dump_source, SETTING_BOOLEAN);
  if(set_dump_config)
    settings_set("dump_config", (void *)setting_dump_config, SETTING_BOOLEAN);
  if(set_interface)
    settings_set("interface", (void *)setting_interface, SETTING_STRING);

  /* If the user put something that could be interpreted as a URL on
   * the command line, then we put that into the default_page setting.
   */
/*
  if(optind < argc) {
    settings_set("default_page", (void *)argv[optind], SETTING_STRING);
  }
 
  return optind;
*/
  return 0;
}

/**
 * Reads settings from environment variables. Right now, these are not
 * generic variables, but only a few specific variables are read. These are:
 * 
 * http_proxy       - proxy URL
 * ZEN_INTERFACE    - interface name
 * ZEN_DEFAULT_PAGE - default URL
 */
static void interpret_environment(void)
{
  char *value;

  value = getenv("http_proxy");
  if(value != NULL)
    settings_set("http_proxy", (void *)value, SETTING_STRING);

  value = getenv("ZEN_INTERFACE");
  if(value != NULL)
    settings_set("interface", (void *)value, SETTING_STRING);

  value = getenv("ZEN_DEFAULT_PAGE");
  if(value != NULL)
    settings_set("default_page", (void *)value, SETTING_STRING);
}

/**
 * Print the usage help text when the option --help is given.
 *
 * @param program_name The name of the program.
 */
static void print_usage(void)
{
  fprintf(stdout, 
	  "Usage: %s [OPTION(S)] [URL]\n"
	  "\n"
	  "Valid options are:\n"
#ifdef HAVE_GETOPT_LONG
	  "-i name        Select which user interface to use for this session\n"
	  "    --interface=name\n"
	  "-s  --source   Dump the object pointed to by URL on stdout\n"
	  "-c file        Extra configuration file read after all other\n"
	  "    --config=file\n"
	  "-d             Do not load the page, but dump the current \n"
	  " --dump-config configuration on stdout\n"
	  "-h  --help     Print this text and exit\n"
	  "-V  --version  Print version and exit\n"
#else /* !HAVE_GETOPT_LONG */
	  "-i name        Select which user interface to use for this session\n"
	  "-s             Dump the object pointed to by URL on stdout\n"
	  "-c file        Extra configuration file read after all other\n"
	  "-d             Do not load the page, but dump the current \n"
	  "               configuration on stdout\n"
	  "-h             Print this text and exit\n"
	  "-V             Print version and exit\n"
#endif /* !HAVE_GETOPT_LONG */
	  "\n"
	  "Available interfaces:\n"
	  "    %s\n"
	  , program_name, AVAILABLE_UI);
}

/**
 * Print version information when the option --version is given.
 */
static void print_version(void)
{
  fprintf(stdout, "%s version %s\n"
	  "\n"
	  "Available interfaces:\n"
	  "    %s\n"
	  , PACKAGE, VERSION, AVAILABLE_UI);
}

/**
 * Read a configuration file into the global settings struct.
 *
 * @param filename The filename of the configuration file to read.
 * @param warn_missing A non-zero value makes this function warn the
 * @param warn_missing user, if the specified file cannot be found.
 */
static void read_configuration(char *filename, int warn_missing)
{
  int fd, len, ret;
  char c, *setting, *value;
  enum configuration_state state;

  setting = (char *)malloc(1024);
  if(setting == NULL) {
    return;
  }
  value = (char *)malloc(1024);
  if(value == NULL) {
    free(setting);
    return;
  }


  fd = open(filename, O_RDONLY);
/*
  if(fd < 0) {
    if(warn_missing) {
      fprintf(stderr, "%s: Warning! Opening configuration file '%s': ", 
	      program_name, filename);
      perror("");
    }
*/

  if(fd < 0) {
    if(warn_missing) {
      fprintf(stderr, "%s: Warning! Opening configuration file '%s': ", 
	      program_name, filename);
      perror("");
    } 
 
    free(setting);
    free(value);
    return;
  }

  /* Below, we read the configuration file, and through this state machine, 
   * we perform parsing and putting the settings into place. Very advanced
   * stuff. You would not comprehend it, if I tried to explain.
   */
  
  state = NEWLINE;
  while((ret = read(fd, &c, 1)) >= 0) {
    /* At end of file, we fake an end of line. */
    if(ret == 0)
      c = '\n';

    if(state == COMMENT) {
      if(c == '\n' || c == '\r') {
	state = NEWLINE;
      }
    } else if(state == NEWLINE) {
      if(c == '\n' || c == '\r') {
	state = NEWLINE;
      } else if(c == '#') {
	state = COMMENT;
      } else if(!IS_SPACE_ALPHA((int)c)) {
	setting[0] = c;
	setting[1] = '\0';
	state = SETTING;
      }
    } else if(state == SETTING) {
      if(c == '\n' || c == '\r') {
	state = NEWLINE;
      } else if(c == '=') {
	value[0] = '\0';
	state = VALUE;
      } else if(!IS_SPACE_ALPHA((int)c)) {
	len = strlen(setting);
	if(len < 1023) {
	  setting[len] = c;
	  setting[len + 1] = '\0';
	}
      }
    } else if(state == VALUE) {
      if(c == '\n' || c == '\r') {
	char *tmp_value;

	state = NEWLINE;

	/* Remove whitespaces at the beginning and end, 
	 * and set the setting. 
	 */
	tmp_value = value;
	while(IS_SPACE_ALPHA((int)*tmp_value))
	  tmp_value++;
	while(strlen(tmp_value) > 0 && 
	      IS_SPACE_ALPHA((int)tmp_value[strlen(tmp_value) - 1]))
	  value[strlen(tmp_value) - 1] = '\0';

	if(strlen(tmp_value) <= 0)
	  continue;

	switch(find_type(setting)) {
	case SETTING_NUMBER:
	  settings_set(setting, (void *)atoi(tmp_value), SETTING_NUMBER);
	  break;

	case SETTING_BOOLEAN:
	  if(!strcmp(value, "true") || 
	     !strcmp(tmp_value, "t") ||
	     !strcmp(tmp_value, "#t") ||
	     !strcmp(tmp_value, "y") ||
	     !strcmp(tmp_value, "yes") ||
	     !strcmp(tmp_value, "1"))
	    settings_set(setting, (void *)1, SETTING_BOOLEAN);
	  else
	    settings_set(setting, (void *)0, SETTING_BOOLEAN);
	  break;

	case SETTING_STRING:
	default:
	  settings_set(setting, (void *)tmp_value, SETTING_STRING);
	  break;
	}
      } else {
	len = strlen(value);
	if(len < 1023) {
	  value[len] = c;
	  value[len + 1] = '\0';
	}
      }
    }
    
    /* At end of file, we want to end the reading here. */
    if(ret == 0)
      break;
  }

  close(fd);
  free(setting);
  free(value);
}

/** 
 * This finds an old setting with a given name, or, if it does not exist,
 * it is created and initialized. 
 *
 * @param setting The name of the setting to find, or NULL to just
 * @param setting initialize a new one.
 * 
 * @return a pointer to the setting, found or new, or NULL if an error
 * @return occurred.
 */
struct zen_settings *find_or_init_setting(char *setting)
{
  struct zen_settings *setp;
  if(settings == NULL) {
    settings = (struct zen_settings *)malloc(sizeof(struct zen_settings));
	if(settings == NULL)
      return NULL;
    setp = settings;    
    setp->next = NULL;
    setp->setting = NULL;
    setp->type = SETTING_UNASSIGNED;
  } else {
    setp = settings;
    while(strcmp(setp->setting, setting) && setp->next)
      setp = setp->next;    

    if(strcmp(setp->setting, setting)) {
      setp->next = (struct zen_settings *)malloc(sizeof(struct zen_settings));
      if(settings == NULL)
	return NULL;
      setp = setp->next;
      setp->next = NULL;
      setp->setting = NULL;
      setp->type = SETTING_UNASSIGNED;
    }
  }

  return setp;
}

/**
 * Look for a setting with a given name, and return the type of the
 * setting, or SETTING_UNASSIGNED, if the setting was not found.
 *
 * @param setting The name of the setting to find.
 *
 * @return the setting type, or SETTING_UNASSIGNED, if it is not found.
 */
static enum zen_settings_type find_type(char *setting)
{
  struct zen_settings *setp;

  setp = settings;
  while(setp) {
    if(!strcmp(setp->setting, setting))
      return setp->type;
    setp = setp->next;
  }

  return SETTING_UNASSIGNED;
}

/**
 * Generic function to set a settings value. 
 *
 * @param setting The name of the setting.
 * @param value A void pointer containing the value of the setting. This
 * @param value must have been casted properly by the caller, but we trust
 * @param value the caller, do we not? 
 * @param type The type of the setting to set. If the setting has already
 * @param type been set at least once, it must be the same type as it
 * @param type already is. Otherwise this function returns with an error.
 *
 * @return a non-zero value if an error occurred.
 */
int settings_set(char *setting, void *value, enum zen_settings_type type)
{
  struct zen_settings *setp;

  setp = find_or_init_setting(setting);

  /* If the type of the currently set value is not the same as the 
   * type the caller wishes to set this time, we should return an 
   * error here.
   */
  if(setp->type != SETTING_UNASSIGNED && setp->type != type) {
    return 1;
  }

  /* If there already was a setting with the same name, we just delete
   * the contents of that here, before we put in the new content. 
   */
  settings_free(setp);

  setp->setting = (char *)malloc(strlen(setting) + 1);
  if(setp->setting == NULL) {
    return 1;
  }
  strcpy(setp->setting, setting);

  setp->type = type;
  switch(type) {
  case SETTING_NUMBER:
  case SETTING_BOOLEAN:
    setp->value = value;
    break;
      
  case SETTING_STRING:
    setp->value = malloc(strlen((char *)value) + 1);
    if(setp->value == NULL) {
      return 1;
    }
    strcpy((char *)setp->value, (char *)value);
    break;

  default:
    return 1;
  }

  return 0;
}

/**
 * Get the value for the given setting. This is a generic settings
 * and works with all setting types. 
 *
 * @param setting The name of the setting to get.
 * @param value A pointer to a void pointer which will be filled with
 * @param value value of the setting. If the setting could not be
 * @param value found, NULL is placed in the void pointer. However, if
 * @param value all programmers have done the correct thing, this will
 * @param value never happen. 
 *
 * @return the type of the setting, or the special value 
 * @return SETTING_UNASSIGNED, if the setting could not be found.
 */
enum zen_settings_type settings_get(char *setting, void **value)
{
  struct zen_settings *setp;

  setp = settings;
  while(setp && strcmp(setp->setting, setting))
    setp = setp->next;

  if(setp) {
    *value = setp->value;
    return setp->type;
  } else {
    *value = NULL;
    return SETTING_UNASSIGNED;
  }
}

/**
 * Free the memory allocated for one setting. This does not remove the
 * setting from the linked list, it merely frees the memory allocated for 
 * the setting name and value, if they have been allocated.
 *
 * @param setting A pointer to the setting to free memory for.
 */
void settings_free(struct zen_settings *setting)
{
  switch(setting->type) {
  case SETTING_STRING:
    if(setting->value)
      free(setting->value);

    /* Break intentionally left out. */

  case SETTING_UNASSIGNED:
  case SETTING_NUMBER:
  case SETTING_BOOLEAN:
  default:
    if(setting->setting)
      free(setting->setting);
  }
}

/**
 * Free the memory allocated for all the settings.
 */
void settings_free_all(void)
{
  struct zen_settings *setp, *tmp_setp;
  
  setp = settings;
  while(setp) {
    tmp_setp = setp;
    setp = tmp_setp->next;
    settings_free(tmp_setp);
    free(tmp_setp);
  }

  settings = NULL;
}

/**
 * Used for debugging purposes, but might be useful for users too.
 * Dumps all settings in the format: "setting=value". Incidentely,
 * this is the same format that Zen uses for its configuration files.
 */
void debug_dump_settings(void)
{
  struct zen_settings *setp;

  printf("# Zen %s configuration dump.\n\n", VERSION);

  setp = settings;
  while(setp) {
    switch(setp->type) {
    case SETTING_NUMBER:
      printf("%s = %d\n", setp->setting, (int)setp->value);
      break;

    case SETTING_BOOLEAN:
      if((int)setp->value)
	printf("%s = true\n", setp->setting);
      else
	printf("%s = false\n", setp->setting);
      break;

    case SETTING_STRING:
      printf("%s = %s\n", setp->setting, (char *)setp->value);
      break;

    default:
      break;
    }

    setp = setp->next;
  }

  printf("\n# End of dump.\n");
}

