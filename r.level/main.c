
/****************************************************************************
 *
 * MODULE:       r.level
 * AUTHOR(S):    Thomas Kreuzer <thomas kreuzer at rwth-aachen de>
 * PURPOSE:      Level elevation raster for zero boundary conditions.
 * COPYRIGHT:    (C) 2019 Thomas Kreuzer
 *
 *               This program is free software under the GNU General Public
 *               License (>=v2). Read the COPYING file that comes with GRASS
 *               for details.
 *
 *****************************************************************************/
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "global.h"

typedef struct
{
    unsigned long area;
    mask_t *mask;
    float z;
} Result;

enum
{ OptIn, OptOut, OptType, OptStep, OptPass, OptLast };

void get_extent(int *, int *);
void get_min_max(float *, float *);
void init_module(void);
Result *level(void);

int ncols;
int nrows;
float start;
float stop;
float step;
int sign;
int pass;
struct GModule *mod;
struct Option *opt[OptLast];

void get_extent(int *rows, int *cols)
{
    struct Cell_head wnd;

    G_get_set_window(&wnd);
    *rows = wnd.rows;
    *cols = wnd.cols;
}

void get_min_max(float *min, float *max)
{
    struct FPRange range;
    char name[GNAME_MAX];
    char mapset[GMAPSET_MAX];

    G_unqualified_name(opt[OptIn]->answer, NULL, name, mapset);
    if (Rast_read_fp_range(name, mapset, &range) != 1)
        G_fatal_error(_("Unable to read fp range of raster map <%s>"),
                      opt[OptIn]->answer);
    *min = ceil(range.min);
    *max = floor(range.max);
}

void init_module(void)
{
    mod = G_define_module();
    G_add_keyword(_("raster"));
    G_add_keyword(_("surface"));
    G_add_keyword(_("elevation"));
    mod->description =
        _("Level elevation raster for zero boundary conditions");

    opt[OptIn] = G_define_standard_option(G_OPT_R_ELEV);
    opt[OptIn]->required = YES;

    opt[OptOut] = G_define_standard_option(G_OPT_R_OUTPUT);
    opt[OptOut]->required = YES;

    opt[OptType] = G_define_option();
    opt[OptType]->key = "type";
    opt[OptType]->type = TYPE_STRING;
    opt[OptType]->description = "Morphological type";
    opt[OptType]->required = YES;
    opt[OptType]->options = "ridge,valley";

    opt[OptStep] = G_define_option();
    opt[OptStep]->key = "step";
    opt[OptStep]->answer = "10.0";
    opt[OptStep]->type = TYPE_DOUBLE;
    opt[OptStep]->description = "Increment between contour levels";
    opt[OptStep]->required = YES;

    opt[OptPass] = G_define_option();
    opt[OptPass]->key = "repeat";
    opt[OptPass]->answer = "0";
    opt[OptPass]->type = TYPE_INTEGER;
    opt[OptPass]->description = "Number of times to repeat the filter";
    opt[OptPass]->required = YES;
}

Result *level(void)
{
    static Result r;
    unsigned long area;
    mask_t *buf;
    float x;
    int i, n;

    n = (int)((stop - start) / step) + pass;

    r.mask = Mask_allocate();
    buf = Mask_allocate();
    for (i = 0, x = start; x < stop; i++, x += step) {
        G_percent(i, n, 10);
        Mask_set(buf, sign, x);
        Mask_fill(buf);
        area = Mask_area(buf);
        if (r.area < area) {
            r.area = area;
            r.z = x;
            Mask_copy(r.mask, buf);
        }
    }
    Mask_free(buf);
    return &r;
}

int main(int argc, char *argv[])
{
    Result *res;

    G_gisinit(argv[0]);
    init_module();
    if (G_parser(argc, argv))
        exit(EXIT_FAILURE);
    get_extent(&nrows, &ncols);
    get_min_max(&start, &stop);
    pass = atoi(opt[OptPass]->answer);
    step = strtof(opt[OptStep]->answer, NULL);
    start += step, stop -= step;
    sign = strcmp(opt[OptType]->answer, "ridge") == 0 ? 1 : -1;
    Grid_open_old(opt[OptIn]->answer);

    G_message(_("Percent complete..."));
    res = level();
    Mask_filter(res->mask, pass);
    G_percent(1, 1, 1);

    Grid_copy(opt[OptOut]->answer, res->mask, res->z);
    Mask_free(res->mask);
    exit(EXIT_SUCCESS);
}
