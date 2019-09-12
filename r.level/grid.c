#include "global.h"

int fd = -1;
FCELL *row = NULL;

void Grid_close(void)
{
    G_free(row);
}

void Grid_copy(char *name, mask_t *mask, float z)
{
    int new;
    int i, j;

    new = Rast_open_fp_new(name);
    if (new < 0)
        G_fatal_error(_("Unable to create raster map <%s>"), name);
    for (i = 0; i < nrows; i++) {
        Grid_get_row(i);
        for (j = 0; j < ncols; j++)
            if (Mask_isset(mask, i, j)) {
                row[j] -= z;
            } else {
                row[j] = 0;
            }
        Rast_put_row(new, row, FCELL_TYPE);
    }
    Rast_close(new);
}

FCELL *Grid_get_row(int i)
{
    if (row == NULL)
        row = Rast_allocate_f_buf();
    Rast_get_f_row(fd, row, i);
    return row;
}

void Grid_open_old(char *name)
{
    char gname[GNAME_MAX];
    char gmapset[GMAPSET_MAX];

    G_unqualified_name(name, NULL, gname, gmapset);
    fd = Rast_open_old(gname, gmapset);
    if (fd < 0)
        G_fatal_error(_("Unable to open raster map <%s>"), name);
}
