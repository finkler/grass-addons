#include <math.h>
#include <stdint.h>
#include <string.h>

#include "global.h"

#define WINDOW 8

mask_t *Mask_allocate(void)
{
    int i;
    mask_t *mask;

    mask = G_malloc(sizeof(mask_t) * nrows);
    for (i = 0; i < nrows; i++)
        mask[i] = G_calloc(1, ncols);
    return mask;
}

unsigned long Mask_area(mask_t *mask)
{
    unsigned long len;
    int i, j;

    len = 0;
    for (i = 0; i < nrows; i++)
        for (j = 0; j < ncols; j++)
            len += mask[i][j];
    return len;
}

void Mask_clear(mask_t *mask)
{
    memset(mask, 0, nrows * ncols);
}

void Mask_copy(mask_t *dest, mask_t *src)
{
    memcpy(dest, src, nrows * ncols);
}

void Mask_fill(mask_t *mask)
{
    Addr *addr, *a;

    addr = NULL;
    Addr_push(&addr, 0, 0);
    while (addr) {
        a = Addr_pop(&addr);
        if (Mask_isset(mask, a->i, a->j)) {
            mask[a->i][a->j] = 0;
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
        for (i = 1; i < nrows - 1; i++)
            for (j = 1; j < ncols - 1; j++) {
                window[0] = Mask_isset(mask, i, j - 1);
                window[1] = Mask_isset(mask, i + 1, j - 1);
                window[2] = Mask_isset(mask, i + 1, j);
                window[3] = Mask_isset(mask, i + 1, j + 1);
                window[4] = Mask_isset(mask, i, j + 1);
                window[5] = Mask_isset(mask, i - 1, j + 1);
                window[6] = Mask_isset(mask, i - 1, j);
                window[7] = Mask_isset(mask, i - 1, j - 1);
                n = 0;
                for (k = 0; k < WINDOW; k++)
                    n += window[k];
                if (n > 4)
                    buf[i][j] = 1;
            }
        Mask_copy(mask, buf);
        Mask_clear(buf);
    }
    Mask_free(buf);
}

void Mask_free(mask_t *mask)
{
    int i;

    for (i = 0; i < nrows; i++)
        G_free(mask[i]);
    G_free(mask);
}

int Mask_isset(mask_t *mask, int i, int j)
{
    return mask[i][j];
}

void Mask_set(mask_t *mask, int sign, float z)
{
    int i, j;
    FCELL *row;

    for (i = 0; i < nrows; i++) {
        row = Grid_get_row(i);
        #pragma omp parallel for
        for (j = 0; j < ncols; j++) {
            if (i == 0 || i == nrows - 1 || j == 0 || j == ncols - 1) {
                mask[i][j] = 1;
                continue;
            }
            if (isnan(row[j]))
                continue;
            if (sign * (row[j] - z) > 0)
                mask[i][j] = 1;
        }
    }
}
