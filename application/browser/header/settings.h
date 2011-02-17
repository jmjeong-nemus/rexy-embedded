/** 
 * Structures containing settings that inflicts upon the different parts
 * of Zen. This is a generic settings area, common for all user interfaces
 * or does not affect any user interface.
 *
 * @author Tomas Berndtsson <tomas@nocrew.org>
 */

#ifndef _SETTINGS_H_
#define _SETTINGS_H_

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

/* Names of the global and user configuration directories. */
/*#define GLOBAL_CONFIGDIR SYSCONFDIR "d://zen-0.2.3"*/
#define GLOBAL_CONFIGDIR "d:\\zen-0.2.3"
#define USER_CONFIGDIR "d:\\zen.cfg"

/**
 * Enumeration for the different setting types we can have.
 */
enum zen_settings_type {
  SETTING_UNASSIGNED,
  SETTING_NUMBER,
  SETTING_BOOLEAN,
  SETTING_STRING
};

/**
 * A linked list of all settings for the program. The settings can be
 * asked for through functions provided in settings.c. 
 *
 * @member type The type of the current setting's value.
 * @member setting The name of the setting.
 * @member value The value for the setting.
 * @member next A pointer to the next setting, or NULL if this is the
 * @member next last setting.
 */
struct zen_settings {
  enum zen_settings_type type;
  char *setting;
  void *value;
  struct zen_settings *next;
};

/* Function prototype. */
extern void settings_read(int argc, char *argv[]);
extern void settings_read_interface(void);
extern enum zen_settings_type settings_get(char *setting, void **value);
extern int settings_set(char *setting, void *value, 
			enum zen_settings_type type);
extern void debug_dump_settings(void);

#endif /* _SETTINGS_H_ */
