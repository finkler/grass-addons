/*
 * Queue implementation for matrix addresses
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


#ifndef _ADDR_H
#define _ADDR_H

typedef struct Addr Addr;
struct Addr
{
    int i, j;
    Addr *next;
};

Addr *Addr_pop(Addr **);
void Addr_push(Addr **, int, int);

#endif /* _ADDR_H */
