#!/usr/bin/env python3
#
############################################################################
#
# MODULE:       r.mscn
# AUTHOR(S):    Thomas Kreuzer <thomer kreuzer at rwth-acchen de>
# PURPOSE:      Calculates  mean subtracted contrast normalized coefficents
# COPYRIGHT:    (C) 2020 Thomas Kreuzer
#
#               This program is free software under the GNU General Public
#               License (>=v2). Read the file COPYING that comes with GRASS
#               for details.
#
#############################################################################

#%module
#% description: Calcuates MSCN coefficents
#% keyword: raster
#%end

# https://ieeexplore.ieee.org/document/6272356
# https://towardsdatascience.com/automatic-image-quality-assessment-in-python-391a6be52c11
import grass.script as grass
import numpy as np
import sys

from grass.pygrass import raster

NO_DATA = -2147483648


def main():
    r = raster.RasterRow("4454690_5448000")
    r.open()
    a = r.info.min
    b = r.info.max
    new = raster.RasterRow("dem_1m_norm")
    new.open("w", "FCELL", True)
    for row in r:
        arr = np.asarray(row)
        mask = ~np.isnan(arr)
        arr[mask] = (arr[mask] - a) / (b - a)
        new.put_row(row)
    new.close()
    r.close()
    return 0


if __name__ == "__main__":
    options, flags = grass.parser()
    sys.exit(main())
