/*
 * Declarations for the whole module
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


#ifndef _GLOBAL_H
#define _GLOBAL_H

#define PACKAGE "raster"

#include <grass/gis.h>
#include <grass/glocale.h>
#include <grass/raster.h>
#include <grass/segment.h>

#include "mask.h"
#include "grid.h"
#include "addr.h"

extern int ncols;
extern int nrows;

#endif /* _GLOBAL_H */
