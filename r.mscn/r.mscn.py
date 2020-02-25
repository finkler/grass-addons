#!/usr/bin/env python
#
############################################################################
#
# MODULE:       r.mscn
# AUTHOR(S):    Thomas Kreuzer
# PURPOSE:      Calculates  mean subtracted contrast normalized coefficents
# COPYRIGHT:    (C) 2014 by the GRASS Development Team
#
#               This program is free software under the GNU General Public
#               License (>=v2). Read the file COPYING that comes with GRASS
#               for details.
#
#############################################################################

#%module
#% description: Calcuates bioclimatic indices.
#% keyword: raster
#% keyword: time series
#% overwrite: yes
#%end
#%option G_OPT_R_INPUTS
#% key: tmin
#% label: Monthly minimum temperatures
#% description: Monthly minimum temperatures for 12 months
#% required: YES
#%end
#%option G_OPT_R_INPUTS
#% key: tmax
#% label: Monthly maximum temperatures
#% description: Monthly maximum temperatures for 12 months
#% required: YES
#%end
#%option G_OPT_R_INPUTS
#% key: tavg
#% label: Monthly average temperatures
#% description: Monthly average temperatures for 12 months
#% required: NO
#%end
#%option G_OPT_R_INPUTS
#% key: precipitation
#% label: Monthly precipitation
#% description: Monthly average precipitation for 12 months
#% required: NO
#%end
#%option
#% key: output
#% type: string
#% description: Prefix for output maps
#% required: YES
#%end
#%option
#% key: tinscale
#% type: integer
#% description: Scale factor for input temperature
#% answer: 1
#%end
#%option
#% key: toutscale
#% type: integer
#% description: Scale factor for output temperature
#% answer: 10
#%end
#%option
#% key: quartals
#% type: integer
#% description: Number of quartals to use
#% options: 4,12
#% answer: 4
#%end
#%Option
#% key: workers
#% type: integer
#% required: no
#% multiple: no
#% options: 1-12
#% answer: 1
#% description: Number of parallel processes to launch
#%End

# https://ieeexplore.ieee.org/document/6272356
# https://towardsdatascience.com/automatic-image-quality-assessment-in-python-391a6be52c11
import numpy as np
import sys

from scipy import signal


def mscn(i, kernel_size=6, sigma=7 / 6):
    def _std(i, mu, w):
        sigma = i ** 2
        sigma = signal.convolve2d(sigma, w, "same")
        return np.sqrt(np.abs(mu ** 2 - sigma))

    def _w(n, sigma):
        Y, X = np.indices((n, n)) - int(n / 2)
        G = (
            1.0
            / (2.0 * np.pi * sigma ** 2)
            * np.exp(-(X ** 2 + Y ** 2) / (2.0 * sigma ** 2))
        )
        return G / np.sum(G)

    C = 1.0 / 255.0
    w = _w(kernel_size, sigma)
    mu = signal.convolve2d(i, w, "same")
    std = _std(i, mu, w)
    return (i - mu) / (std + C)


def main():
    return 0


if __name__ == "__main__":
    sys.exit(main())
