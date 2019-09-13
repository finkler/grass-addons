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


#include <math.h>
#include <stdint.h>
#include <string.h>

#include "bitarray2d.h"
#include "global.h"

#define SIZE (nrows*ncols/WORD)
#define WINDOW 8

mask_t *Mask_allocate(void)
{
    return G_calloc(sizeof(mask_t), SIZE);
}

unsigned long Mask_area(mask_t *mask)
{
    return bit2length(mask, SIZE);
}

void Mask_clear(mask_t *mask)
{
    memset(mask, 0, sizeof(mask_t) * SIZE);
}

void Mask_copy(mask_t *dest, mask_t *src)
{
    memcpy(dest, src, sizeof(mask_t) * SIZE);
}

void Mask_fill(mask_t *mask)
{
    Addr *addr, *a;

    addr = NULL;
    Addr_push(&addr, 0, 0);
    #pragma omp parallel for private(a)
    for (a = Addr_pop(&addr); addr; a = Addr_pop(&addr)) {
        if (bit2isset(mask, a->i, a->j)) {
            bit2clear(mask, a->i, a->j);
            Addr_push(&addr, a->i, a->j + 1);
            Addr_push(&addr, a->i, a->j - 1);
            Addr_push(&addr, a->i + 1, a->j);
            Addr_push(&addr, a->i - 1, a->j);
            Addr_push(&addr, a->i + 1, a->j + 1);
            Addr_push(&addr, a->i + 1, a->j - 1);
            Addr_push(&addr, a->i - 1, a->j + 1);
            Addr_push(&addr, a->i - 1, a->j - 1);
        }
        G_free(a);
    }
}

void Mask_filter(mask_t *mask, int pass)
{
    int i, j, k, n;
    int window[WINDOW];
    mask_t *buf;

    buf = Mask_allocate();
    while (pass-- > 0) {
        #pragma omp parallel for private(i, j, k, n, window)
        for (i = 1; i < nrows - 1; i++)
            for (j = 1; j < ncols - 1; j++) {
                window[0] = bit2isset(mask, i, j - 1);
                window[1] = bit2isset(mask, i + 1, j - 1);
                window[2] = bit2isset(mask, i + 1, j);
                window[3] = bit2isset(mask, i + 1, j + 1);
                window[4] = bit2isset(mask, i, j + 1);
                window[5] = bit2isset(mask, i - 1, j + 1);
                window[6] = bit2isset(mask, i - 1, j);
                window[7] = bit2isset(mask, i - 1, j - 1);
                n = 0;
                for (k = 0; k < WINDOW; k++)
                    n += window[k];
                if (n > 4)
                    bit2set(buf, i, j);
            }
        Mask_copy(mask, buf);
        Mask_clear(buf);
    }
    Mask_free(buf);
}

void Mask_free(mask_t *mask)
{
    G_free(mask);
}

int Mask_isset(mask_t *mask, int i, int j)
{
    return bit2isset(mask, i, j);
}

void Mask_set(mask_t *mask, int sign, float z)
{
    int i, j;
    FCELL *row;

    #pragma omp parallel for private(i, j, row)
    for (i = 0; i < nrows; i++) {
        row = Grid_get_row(i);
        for (j = 0; j < ncols; j++) {
            if (i == 0 || i == nrows - 1 || j == 0 || j == ncols - 1) {
                bit2set(mask, i, j);
                continue;
            }
            if (isnan(row[j]))
                continue;
            if (sign * (row[j] - z) > 0)
                bit2set(mask, i, j);
        }
        G_free(row);
    }
}
