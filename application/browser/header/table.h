/** 
 * Function prototypes used to layout tables.
 *
 * @author Tomas Berndtsson <tomas@nocrew.org>
 */

#ifndef _LAYOUTER_TABLE_H_
#define _LAYOUTER_TABLE_H_

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

extern int layout_table(struct layout_part *table, int total_width);
extern void layout_table_set_positions(struct layout_part *table,
				       struct layout_part *relative,
				       int force_parts);

#endif /* _LAYOUTER_TABLE_H_ */
