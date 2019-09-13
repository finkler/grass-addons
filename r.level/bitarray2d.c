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


#include <stdint.h>

#include "bitarray2d.h"

static inline uint64_t pair(int, int);

uint64_t pair(uint32_t i, uint32_t j)
{
    return ((i + j) * (i + j + 1) + j) >> 1;
}

void bit2clear(int *a, int i, int j)
{
    uint64_t ij;

    ij = pair(i, j);
    #pragma omp atomic update
    a[ij / WORD] &= ~(1 << (ij % WORD));
}

int bit2isset(int *a, int i, int j)
{
    uint64_t ij;

    ij = pair(i, j);
    return a[ij / WORD] & (1 << (ij % WORD));
}

unsigned long bit2length(int *a, int size)
{
    unsigned long len;
    int i, n;

    for (i = 0; i < size; i++) {
        n = a[i];
        while (n) {
            n &= n - 1;
            len++;
        }
    }
    return len;
}

void bit2set(int *a, int i, int j)
{
    uint64_t ij;

    ij = pair(i, j);
    #pragma omp atomic update
    a[ij / WORD] |= 1 << (ij % WORD);
}

void bit2toggle(int *a, int i, int j)
{
    uint64_t ij;

    ij = pair(i, j);
    #pragma omp atomic update
    a[ij / WORD] ^= 1 << (ij % WORD);
}
