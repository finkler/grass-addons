#ifndef _BITARRAY2D_H
#define _BITARRAY2D_H

#define WORD (sizeof(int)<<3UL)

void bit2clear(int *, int, int);
int bit2isset(int *, int, int);
void bit2set(int *, int, int);
void bit2toggle(int *, int, int);

#endif /* _BITARRAY2D_H */
