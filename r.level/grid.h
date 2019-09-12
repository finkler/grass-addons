#ifndef _GRID_H
#define _GRID_H

void Grid_close(void);
void Grid_copy(char *, mask_t *, float);
FCELL *Grid_get_row(int);
void Grid_open_old(char *);

#endif /* _GRID_H */
