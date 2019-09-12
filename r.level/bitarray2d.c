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
    __sync_fetch_and_and(&a[ij / WORD], ~(1 << (ij % WORD)));
}

int bit2isset(int *a, int i, int j)
{
    uint64_t ij;

    ij = pair(i, j);
    return a[ij / WORD] & (1 << (ij % WORD));
}

void bit2set(int *a, int i, int j)
{
    uint64_t ij;

    ij = pair(i, j);
    __sync_fetch_and_or(&a[ij / WORD], 1 << (ij % WORD));
}

void bit2toggle(int *a, int i, int j)
{
    uint64_t ij;

    ij = pair(i, j);
    __sync_fetch_and_xor(&a[ij / WORD], 1 << (ij % WORD));
}
