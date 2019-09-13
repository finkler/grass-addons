/*
 * Convenience raster functions
 *
 * Authors:
 *   Thomas Kreuzer <thomas kreuzer at rwth-aachen de>
 *
 * Copyright 2019 by Thomas Kreuzer
 *
 * This program is free software licensed under the GPL (>=v2).
 * Read the COPYING file that comes with GRASS for details.
 *
 */


#ifndef _GRID_H
#define _GRID_H

void Grid_close(void);
void Grid_copy(char *, mask_t *, float);
FCELL *Grid_get_row(int);
void Grid_open_old(char *);

#endif /* _GRID_H */
