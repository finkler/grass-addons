/*
 * Raster mask and utility functions
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


#ifndef _MASK_H
#define _MASK_H

typedef int mask_t;

mask_t *Mask_allocate(void);
unsigned long Mask_area(mask_t *);
void Mask_clear(mask_t *);
void Mask_copy(mask_t *, mask_t *);
void Mask_fill(mask_t *);
void Mask_filter(mask_t *, int);
void Mask_free(mask_t *);
int Mask_isset(mask_t *, int, int);
void Mask_set(mask_t *, int, float);

#endif /* _MASK_H */
