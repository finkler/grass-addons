/*
 * 2 dimensional bit array implementation
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


#ifndef _BITARRAY2D_H
#define _BITARRAY2D_H

#define WORD (sizeof(int)<<3UL)

void bit2clear(int *, int, int);
unsigned long bit2length(int *, int);
int bit2isset(int *, int, int);
void bit2set(int *, int, int);
void bit2toggle(int *, int, int);

#endif /* _BITARRAY2D_H */
