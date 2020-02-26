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
from scipy import signal


def kern(n, k):
    m = 2 * n + 1
    sigma = np.sqrt(np.square(np.arange(m) - n).mean()) * k
    Y, X = np.indices((m, m)) - n
    G = (
        1.0
        / (2.0 * np.pi * sigma ** 2)
        * np.exp(-(X ** 2 + Y ** 2) / (2.0 * sigma ** 2))
    )
    return G / np.sum(G)


def std(i, mu, w):
    sigma = signal.convolve2d(i ** 2, w, "same")
    return np.sqrt(np.abs(mu ** 2 - sigma))


def write_raster(arr, name, overwrite=True, mtype="FCELL"):
    new = raster.RasterRow(name)
    new.open("w", mtype, overwrite)
    for i in arr:
        buf = raster.buffer.Buffer(i.shape, mtype=mtype)
        np.copyto(buf, i)
        new.put_row(buf)
    new.close()


def main():
    r = raster.RasterRow("dem_1m_norm")
    r.open()
    i = np.full((r._rows, r._cols), np.nan)
    for j, row in enumerate(r):
        i[j, :] = row
    r.close()

    w = kern(3, 3)
    mu = signal.convolve2d(i, w, "same")
    # write_raster(mu, "local_mean_field")

    sigma = std(i, mu, w)
    # write_raster(sigma, "local_variance_field")

    c = i - mu
    # write_raster(c, "zero_log_contrast")

    C = 1.0 / 255.0
    mscn = c / (sigma + C)
    write_raster(mscn, "mscn_field")
    return 0


if __name__ == "__main__":
    options, flags = grass.parser()
    sys.exit(main())
