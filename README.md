# Conic Fit
C++ library for handling conic sections - evaluation/gradient, approximate Euclidean distance computation, fitting on a polyline, and classification.

This is the 2D version of my [quadric fitter library](https://github.com/salvipeter/quadric-fit).

There is also a test program for fitting and classification.

## Compilation
Uses [my geometry library](https://github.com/salvipeter/libgeom/). Needs [Eigen](https://eigen.tuxfamily.org/) to compile.
Edit the `Makefile` to set the correct paths and build.

## Documentation
Read the header file (`conic-fit.hh`).
Note that the integrals during fitting can be exact or approximative;
this is controlled by `USE_EXACT_LINE_INTEGRAL` in `fitter.cc`.
