/**
 * Functions which take care of the layouting of tables. Structurally,
 * these belong in layout.c, but to keep the source file sizes down,
 * they were split out into its own file.
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

#include "../header/layout.h"
#include "../header/ui.h"
#include "../header/table.h"

/* This is used when compiling with the libdmalloc debug library. */
#ifdef HAVE_DMALLOC_H
#include <dmalloc.h>
#endif /* HAVE_DMALLOC_H */

#undef DEBUG

/**
 * This is only used internally in this file, to work with the
 * table, and keep track of columns and rows. 
 */
struct layout_table_structure {
  int width;
  int part_width;
  enum layout_size_type width_type;
  int max_width;
  int rowspan;
  int colspan;
};

/**
 * Layout a table, arranging the table cells and rows as either
 * specified in the tags, or as Zen seems fit. It does not always
 * look the same as Netscape, especially when widths are not
 * explicitly specified in the tags.
 *
 * This might be the longest single function in this whole program.
 * Only layout_do() is close.
 *
 * @param table A pointer to the layout part making out the 
 * @param table complete and utter table.
 * @param total_width The width that this function has got to
 * @param total_width fit the table within. Good luck to it.
 *
 * @return non-zero value if an error occurred.
 */
int layout_table(struct layout_part *table, int total_width)
{
  int nr_of_rows, nr_of_cols, rownr, colnr, unset_cols, sum_absolute;
  int max_cols, cell_width, width_left, width_type;
  int x_position, y_position, max_row_height, i;
  struct layout_part *rowp, *cellp;
  struct layout_part **rowspan_cells;
  int *rowspans;
  struct layout_table_structure *virtual_cells;

  if(total_width <= 0)
    total_width = user_interface.ui_display.width;

  /* First calculate the number of rows and columns there is in the
   * whole table.
   */

  nr_of_rows = 0;
  max_cols = 0;
  rowp = table->child;
  while(rowp) {
    if(rowp->type == LAYOUT_PART_TABLE_ROW) {
      nr_of_rows++;
      nr_of_cols = 0;
      cellp = rowp->child;
      while(cellp) {
	if(cellp->type == LAYOUT_PART_TABLE_CELL) {
	  nr_of_cols += cellp->data.table_cell.colspan;
	}

	cellp = cellp->next;
      }

      if(nr_of_cols > max_cols)
	max_cols = nr_of_cols;
    }

    rowp = rowp->next;
  }
  nr_of_cols = max_cols;

  /* Store the total possible number of rows and columns, for future 
   * reference.
   */
  table->data.table.number_of_rows = nr_of_rows;
  table->data.table.number_of_columns = nr_of_cols;

  /* If the number of rows, or number of columns are zero, we treat
   * it as an error.
   */
  if(nr_of_rows == 0 || nr_of_cols == 0)
    return 1;

  /* Reserve a table structure for each virtual cell in one row of the
   * table. This is used to find the correct width of each column in 
   * the finished table.
   */
  virtual_cells = (struct layout_table_structure *)
    malloc(sizeof(struct layout_table_structure) * nr_of_cols);
  if(virtual_cells == NULL)
    return 1;
  for(i = 0 ; i < nr_of_cols ; i++) {
    virtual_cells[i].width = 0;
    virtual_cells[i].part_width = 1;
    virtual_cells[i].width_type = LAYOUT_SIZE_UNSPECIFIED;
    virtual_cells[i].rowspan = 1;
    virtual_cells[i].colspan = 1;
  }

  /* Go through the tables again, and try to decide what widths the
   * cells will have. At the same time, we set the rowstart and colstart
   * information for each table cell. 
   */
  rownr = 0;
  rowp = table->child;
  while(rowp) {
    if(rowp->type == LAYOUT_PART_TABLE_ROW) {
      colnr = 0;
      cellp = rowp->child;
      while(colnr < nr_of_cols) {
	if(cellp == NULL || cellp->type == LAYOUT_PART_TABLE_CELL) {

	  /* Skip if the previous row spans over this cell too. */
	  if(virtual_cells[colnr].rowspan > 1) {

		 virtual_cells[colnr].rowspan--;
	    colnr += virtual_cells[colnr].colspan;
	    continue;
	  }

	  /* Store the beginning row and column of this table cell. */
	  if(cellp != NULL) {
	    cellp->data.table_cell.rowstart = rownr;
	    cellp->data.table_cell.colstart = colnr;
	  }

	  if(cellp == NULL) {
	    fprintf(stderr, "Warning! End of cells and end of rowspans!\n");
	    colnr = nr_of_cols;
	    continue;
	  }

	  virtual_cells[colnr].rowspan = cellp->data.table_cell.rowspan;
	  virtual_cells[colnr].colspan = cellp->data.table_cell.colspan;

	  if(virtual_cells[colnr].width == 0 && 
	     cellp->data.table_cell.width_type != LAYOUT_SIZE_PART &&
	     cellp->data.table_cell.width > 0) {
	    width_left = cellp->data.table_cell.width;
	    width_type = cellp->data.table_cell.width_type;

	    unset_cols = cellp->data.table_cell.colspan;
	    for(i = colnr ; i < colnr + cellp->data.table_cell.colspan ; i++) {
	      if(virtual_cells[i].width > 0) {
		width_left -= virtual_cells[i].width;
		unset_cols--;
	      }
	    }
	    cell_width = width_left / unset_cols;
	  } else if(virtual_cells[colnr].width == 0 &&
		    cellp->data.table_cell.width_type == LAYOUT_SIZE_PART) {
	    width_left = cellp->data.table_cell.width;
	    width_type = cellp->data.table_cell.width_type;

		unset_cols = cellp->data.table_cell.colspan;
	    for(i = colnr ; i < colnr + cellp->data.table_cell.colspan ; i++) {
	      if(virtual_cells[i].width_type == LAYOUT_SIZE_PART) {
		width_left -= virtual_cells[i].part_width;
		unset_cols--;
	      } else if(virtual_cells[i].width > 0) {
		unset_cols--;
	      }
	    }
	    cell_width = width_left / unset_cols;
	  } else {
	    width_type = LAYOUT_SIZE_ABSOLUTE;
	    cell_width = 0;
	  }

	  for(i = colnr ; i < colnr + cellp->data.table_cell.colspan ; i++) {
	    if(width_type == LAYOUT_SIZE_PART) {
	      virtual_cells[i].part_width = cell_width;
	      virtual_cells[i].width = 0;
	      virtual_cells[i].width_type = width_type;
	    } else if(cell_width > 0 && virtual_cells[i].width == 0) {
	      virtual_cells[i].width = cell_width;
	      virtual_cells[i].width_type = width_type;
	    }
	  }

	  colnr += cellp->data.table_cell.colspan;
	}

	cellp = cellp->next;
      }

      rownr++;
    }

    rowp = rowp->next;
  }


  /* Set the size of the whole table, as much as we know. */
  if(table->data.table.width_type == LAYOUT_SIZE_PERCENT) {
    table->geometry.width = (table->data.table.width * total_width) / 100;
  } else {
    table->geometry.width = table->data.table.width;
  }
  table->geometry.height = 0;

  /* Convert relative widths of the virtual table cells into absolute ones. 
   * Also spread out the restoring width of all unset virtual cells.
   */
  unset_cols = 0;
  sum_absolute = 0;
  width_left = table->geometry.width;
  for(i = 0 ; i < nr_of_cols ; i++) {

#ifdef DEBUG
    fprintf(stderr, "virtual_cells[%d].width %d, virtual_cells[%d].part_width %d, virtual_cells[%d].width_type %d\n", 
	    i, virtual_cells[i].width, i, virtual_cells[i].part_width, i, virtual_cells[i].width_type);
#endif

    switch(virtual_cells[i].width_type) {
    case LAYOUT_SIZE_UNSPECIFIED:
      break;

    case LAYOUT_SIZE_PERCENT:
#if 0
      virtual_cells[i].width = (virtual_cells[i].width * 
				table->geometry.width) / 100;
#endif
      break;

    case LAYOUT_SIZE_PART:
      unset_cols += virtual_cells[i].part_width - 1;
      break;

    case LAYOUT_SIZE_ABSOLUTE:
      sum_absolute += virtual_cells[i].width;
      break;

    default:
      fprintf(stderr, "Error! Wrong width type!\n");
    }

    if(virtual_cells[i].width > 0)
      width_left -= virtual_cells[i].width;
    else 
      unset_cols++;
  }

  /* Go through again to fix per cent cells. */
  for(i = 0 ; i < nr_of_cols ; i++) {
    if(virtual_cells[i].width_type == LAYOUT_SIZE_PERCENT)
      virtual_cells[i].width = (virtual_cells[i].width * 
				(table->geometry.width - sum_absolute)) / 100;
    
  }

#ifdef DEBUG
  fprintf(stderr, "unset_cols %d, width_left %d\n", unset_cols, width_left);
#endif

  if(unset_cols > 0) {
    for(i = 0 ; i < nr_of_cols ; i++) {
      if(virtual_cells[i].width == 0) {
	virtual_cells[i].width = (width_left * 
				  virtual_cells[i].part_width) / unset_cols;
      }
    }
  }

  /* If all the columns have a set width, we can set the total table width
   * according to that. 
   * This might not be the best thing to do, since it overrides anything
   * the user has specified as a width in the table tag, but we simply
   * do not care about HTML writers who cannot make up their minds about
   * such a simple matter.
   */
  if(unset_cols == 0) {
    table->geometry.width = 0;
    for(i = 0 ; i < nr_of_cols ; i++)
      table->geometry.width += virtual_cells[i].width;
  }    

#ifdef DEBUG
  for(i = 0 ; i < nr_of_cols ; i++)
    fprintf(stderr, "col %d, width %d\n", i, virtual_cells[i].width);
#endif


  /* Reserve place for the maximum possible cells using rowspans. */
  rowspan_cells = 
    (struct layout_part **)malloc(sizeof(struct layout_part *) * nr_of_cols);
  if(rowspan_cells == NULL)
    return 1;
  rowspans = (int *)malloc(sizeof(int) * nr_of_cols);
  if(rowspans == NULL) {
    free(rowspan_cells);
    return 1;
  }
  for(i = 0 ; i < nr_of_cols ; i++) {
    rowspan_cells[i] = NULL;
    rowspans[i] = 1;
  }

  /* Layout the interior of the table. */
  y_position = 1;
  rowp = table->child;
  while(rowp) {
    /* If there happen to be anything between table cells, we just skip it.
     * It is an error from the HTML writer, do not blame us.
     */
    if(rowp->type != LAYOUT_PART_TABLE_ROW) {
      rowp = rowp->next;
      continue;
    }

    colnr = 0;
    max_row_height = 0;
    x_position = 1;
    cellp = rowp->child;
    while(colnr < nr_of_cols) {
      int tmp_width, tmp_height;

      if(cellp && cellp->type != LAYOUT_PART_TABLE_CELL) {
	cellp = cellp->next;
	continue;
      }

#ifdef DEBUG
      fprintf(stderr, "%s: width_left=%d, colnr=%d, cellp=%p\n", 
	      __FUNCTION__, width_left, colnr, cellp);
#endif

      if(rowspans[colnr] == 1) {
	/* If some cell is missing, we just skip over the area it was 
	 * supposed to be in.
	 */
	if(cellp == NULL) {
	  break;
	}

	rowspans[colnr] = cellp->data.table_cell.rowspan;
	rowspan_cells[colnr] = cellp;

	tmp_width = 0;
	for(i = 0 ; i < cellp->data.table_cell.colspan ; i++)
	  tmp_width += virtual_cells[i + colnr].width;

	tmp_width -= table->data.table.cellpadding * 2 + 
	  table->data.table.cellspacing + 2;
	if(nr_of_cols - colnr == cellp->data.table_cell.colspan)
	  tmp_width -= table->data.table.cellspacing + 1;
	tmp_height = 0;

	if(tmp_width < 0) {
	  fprintf(stderr, "Warning! Table cell width became negative!\n");
	  tmp_width = 1;
	}

	if(cellp->child != NULL)
	  tmp_width = layout_do(cellp->child, 0, tmp_width, &tmp_height);
	
	if(tmp_width >= 0) {
	  tmp_width += table->data.table.cellpadding * 2 + 2;
	  tmp_height += table->data.table.cellpadding * 2 + 3;

	  cellp->geometry.x_position = 
	    x_position + table->data.table.cellspacing;
	  cellp->geometry.y_position = 
	    y_position + table->data.table.cellspacing;
	  cellp->geometry.width = tmp_width;
	  cellp->geometry.height = tmp_height;

	  x_position += tmp_width;

	  /* Do not check height of cells with a rowspan above the ordinary. */
	  if(cellp->data.table_cell.rowspan == 1 && 
	     tmp_height > max_row_height) {
	      max_row_height = tmp_height;
	  }	      

	}
	colnr += cellp->data.table_cell.colspan;
	x_position += table->data.table.cellspacing;

	cellp = cellp->next;
      } else {
	i = colnr;

#ifdef DEBUG
	fprintf(stderr, "%s: skip column %d\n", __FUNCTION__, 
		i);
	fprintf(stderr, "%s: skip column %d, width=%d\n", __FUNCTION__, 
		i, rowspan_cells[i]->geometry.width);
#endif

	/* This column seems to be occupied by a previous cell, so we
	 * just skip it.
	 */
	rowspans[i]--;
	colnr += rowspan_cells[i]->data.table_cell.colspan;
	x_position += rowspan_cells[i]->geometry.width +
	  + table->data.table.cellspacing;

#ifdef DEBUG
	fprintf(stderr, "y=%d, max_row_height=%d, rscell.height=%d\n",
		y_position, max_row_height, rowspan_cells[i]->geometry.height);
#endif

	/* If we are at the bottom of a rowspanned cell, we check to see if 
	 * the total height of that cell is so high that it is actually the
	 * highest of the current row as well. That might happen.
	 */
	if(rowspans[i] == 1 && 
	   rowspan_cells[i]->geometry.height - y_position > 
	   max_row_height) {
	    max_row_height = rowspan_cells[i]->geometry.height - y_position;
	}

#ifdef DEBUG
	fprintf(stderr, "y=%d, max_row_height=%d, rscell.height=%d\n",
		y_position, max_row_height, rowspan_cells[i]->geometry.height);
#endif
      }
    }

#ifdef DEBUG
    fprintf(stderr, "%s: max_row_height=%d\n", 
	    __FUNCTION__, max_row_height);
#endif

    y_position += max_row_height + table->data.table.cellspacing;

    /* Set the maximum height to all cells in this row. */
    i = 0;
    while(i < nr_of_cols) {
      if(rowspan_cells[i] == NULL)
	break;
      if(rowspans[i] == 1) {
	rowspan_cells[i]->geometry.height = y_position - 
	  rowspan_cells[i]->geometry.y_position;
      }
      i += rowspan_cells[i]->data.table_cell.colspan;
    }

    rowp = rowp->next;
  }

  table->geometry.height = y_position + table->data.table.cellspacing + 1;

  free(rowspans);
  free(rowspan_cells);
  free(virtual_cells);

  return 0;
}

/**
 * Set the positions of the parts inside a table cell, relative
 * to the table cell itself.
 *
 * @param partp A pointer to the part to set positions on.
 * @param cellp A pointer to the table cell part.
 */
void layout_table_set_part_positions(struct layout_part *partp,
				     struct layout_part *cellp)
{
  struct layout_part *table = NULL;

  if(cellp->parent)
    table = cellp->parent->parent;

  if(table == NULL || table->type != LAYOUT_PART_TABLE)
    return;

  while(partp) {
    partp->geometry.x_position += cellp->geometry.x_position +
      table->data.table.cellpadding + 1;
    partp->geometry.y_position += cellp->geometry.y_position +
      table->data.table.cellpadding + 1;

    if(partp->type == LAYOUT_PART_LINK ||
       partp->type == LAYOUT_PART_FORM)
      layout_table_set_part_positions(partp->child, cellp);
    else if(partp->type == LAYOUT_PART_TABLE)
      layout_table_set_positions(partp, cellp, 1);

    partp = partp->next;
  }
}

/**
 * Set the positions of the table cells in a table, relative to
 * the whole table, or to a specified part.
 *
 * @param table A pointer to the table part.
 * @param relative A pointer to a part to use as reference for 
 * @param relative setting the positions of the table cells, or
 * @param relative NULL to use the table as reference.
 * @param force_parts A non-zero value if this should always set
 * @param force_parts positions of parts inside a cell. A value of
 * @param force_parts zero will only set these positions if the
 * @param force_parts table is at the top level of the page.
 */
void layout_table_set_positions(struct layout_part *table,
				struct layout_part *relative,
				int force_parts)
{
  struct layout_part *rowp, *cellp, *partp;
  int set_parts;

  rowp = table->child;
  while(rowp) {
    if(rowp->type != LAYOUT_PART_TABLE_ROW) {
      rowp = rowp->next;
      continue;
    }

    cellp = rowp->child;
    while(cellp) {
      if(cellp->type != LAYOUT_PART_TABLE_CELL) {
	cellp = cellp->next;
	continue;
      }

      if(relative) {
	struct layout_part *relative_table = NULL;

	cellp->geometry.x_position += relative->geometry.x_position;
	cellp->geometry.y_position += relative->geometry.y_position;
	
	relative_table = relative->parent;
	while(relative->type != LAYOUT_PART_TABLE_CELL && 
	      relative_table->parent)
	  relative_table = relative_table->parent;
	if(relative_table) {
	  cellp->geometry.x_position += relative_table->data.table.cellpadding;
	  cellp->geometry.y_position += relative_table->data.table.cellpadding;
	}
      } else {
	cellp->geometry.x_position += table->geometry.x_position;
	cellp->geometry.y_position += table->geometry.y_position;
      }      

#ifdef DEBUG
      fprintf(stderr, "cell +%d+%d at %d,%d\n",
	      cellp->data.table_cell.rowstart, cellp->data.table_cell.colstart,
	      cellp->geometry.x_position, cellp->geometry.y_position);
#endif

      /* Decide if the parts inside the table should be positioned.
       * This is done if we are explicitly told to do it, or if this
       * table is not inside another table. I have currently forgotten
       * why this is the case, but there is probably some good reason.
       */
      set_parts = 1;
      partp = table;
      while(partp && set_parts) {
	partp = partp->parent;
	if(partp && partp->type == LAYOUT_PART_TABLE)
	  set_parts = 0;
      }

      /* Adjust all parts inside the table cell. */
      if(force_parts || set_parts)
	layout_table_set_part_positions(cellp->child, cellp);

      cellp = cellp->next;
    }

    rowp = rowp->next;
  }  
}
