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
