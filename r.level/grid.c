#include "global.h"

#define NSEG 100

SEGMENT seg;

void Grid_close(void)
{
    Segment_close(&seg);
}

void Grid_copy(char *name, mask_t *mask, float z)
{
    int new;
    int i, j;
    FCELL *row;

    new = Rast_open_fp_new(name);
    if (new < 0)
        G_fatal_error(_("Unable to create raster map <%s>"), name);
    for (i = 0; i < nrows; i++) {
        row = Grid_get_row(i);
        for (j = 0; j < ncols; j++)
            if (Mask_isset(mask, i, j)) {
                row[j] -= z;
            } else {
                row[j] = 0;
            }
        Rast_put_row(new, row, FCELL_TYPE);
        G_free(row);
    }
    Rast_close(new);
}

FCELL *Grid_get_row(int i)
{
    FCELL *row;

    row = Rast_allocate_f_buf();
    Segment_get_row(&seg, row, i);
    return row;
}

void Grid_open_old(char *name)
{
    char gname[GNAME_MAX];
    char gmapset[GMAPSET_MAX];
    int fd, i;
    FCELL *row;

    if (Segment_open(&seg, G_tempfile(), nrows, ncols, 1, ncols, sizeof(FCELL), NSEG) < 0)
        G_fatal_error(_("Unable to create temporary segment file"));

    G_unqualified_name(name, NULL, gname, gmapset);
    fd = Rast_open_old(gname, gmapset);
    if (fd < 0)
        G_fatal_error(_("Unable to open raster map <%s>"), name);
    row = Rast_allocate_f_buf();
    for (i = 0; i < nrows; i++) {
        Rast_get_f_row(fd, row, i);
        Segment_put_row(&seg, row, i);
    }
    Segment_flush(&seg);
}
