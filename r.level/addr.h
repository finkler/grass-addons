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
