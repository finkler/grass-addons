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


#include "global.h"

Addr *Addr_pop(Addr ** addr)
{
    Addr *a;

    a = *addr;
    #pragma omp atomic write
    *addr = a->next;
    return a;
}

void Addr_push(Addr ** addr, int i, int j)
{
    Addr *a;

    if (i < 0 || i >= nrows || j < 0 || j >= ncols)
        return;

    a = G_malloc(sizeof(Addr));
    a->i = i;
    a->j = j;
    a->next = NULL;
    if (*addr == NULL) {
        #pragma omp atomic write
        *addr = a;
        return;
    }
    a->next = *addr;
    #pragma omp atomic write
    *addr = a;
}
