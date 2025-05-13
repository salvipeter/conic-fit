# Conic Fit
C++ library for handling conic sections - evaluation/gradient, approximate Euclidean distance computation, fitting on a polyline, and classification.

This is the 2D version of my [quadric fitter library](https://github.com/salvipeter/quadric-fit).

There is also a test program for fitting and classification.

## Compilation
Uses [my geometry library](https://github.com/salvipeter/libgeom/). Needs [Eigen](https://eigen.tuxfamily.org/) to compile.
Edit the `Makefile` to set the correct paths and build.

## Documentation
Read the header file (`conic-fit.hh`).
Fitting has four options that can be combined:

- `Conic::CLOSED` indicates that the points wrap around -
  this is interesting only when either `Conic::MIDPOINT` or `Conic::SEGMENT` is also set

- `Conic::ORIGIN` tells the fitter that the curve should go through the origin

- With `Conic::SEGMENT`, the deviation from the polyline *segments* is minimized;
  `Conic::MIDPOINT` is a 3-point approximation

- When none of the above flags are needed, you can pass `Conic::SIMPLE` instead
