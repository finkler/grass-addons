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
#% overwrite: yes
#%end
#%option G_OPT_R_INPUT
#% description: Shaded relief map
#% required: YES
#%end
#%option G_OPT_R_BASENAME_OUTPUT
#% required: YES
#%end
#%option
#% key: kernel
#% type: integer
#% description: Kernel size
#% answer: 6
#% required: NO
#%End

# https://ieeexplore.ieee.org/document/6272356
# https://towardsdatascience.com/automatic-image-quality-assessment-in-python-391a6be52c11
import numpy as np
import sys

from grass.pygrass import raster
from scipy import signal


def var(i, mu, w):
    sigma = i ** 2
    sigma = signal.convolve2d(sigma, w, "same")
    return np.sqrt(np.abs(mu ** 2 - sigma))


def kernels(n, sigma):
    m = 2 * n + 1
    Y, X = np.indices((m, m)) - n
    G = (
        1.0
        / (2.0 * np.pi * sigma ** 2)
        * np.exp(-(X ** 2 + Y ** 2) / (2.0 * sigma ** 2))
    )
    return G / np.sum(G)


def main():
    shade = raster.RasterRow(options["input"])
    shade.open()
    i = np.zeros((shade._rows, shade._cols), dtype=np.ubyte)
    for j, row in enumerate(shade):
        i[j, :] = row
    shade.close()
    w = kernels(3, 7/6)
    mu = signal.convolve2d(i, w, "same")
    sigma = var(i, mu, w)
    return 0


if __name__ == "__main__":
    sys.exit(main())
