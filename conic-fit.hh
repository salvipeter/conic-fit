#pragma once

#include <geometry.hh>          // https://github.com/salvipeter/libgeom/

struct Conic {
  // Coefficients corresponding to: 1   x   y  x^2  xy y^2
  std::array<double, 6> coeffs; //  c0  c1  c2  c3  c4  c5

  // (Q, P, R) s.t. [x y] Q [x,y] + [x y] P + R = 0
  std::tuple<Geometry::Matrix2x2, Geometry::Vector2D, double> matrixForm() const;

  // Evaluators
  double eval(const Geometry::Point2D &p) const;
  Geometry::Vector2D grad(const Geometry::Point2D &p) const;

  // Approximation of the Euclidean distance (Taubin's second-order formula)
  double distance(const Geometry::Point2D &p) const;

  // Fitter
  using Polyline = std::vector<Geometry::Point2D>;
  enum FitType {
    SIMPLE = 0,                 // if no other option is used
    CLOSED = 1,                 // points wrap around, first coming after last
    ORIGIN = 2,                 // fitted curve should go through the origin
    MIDPOINT = 4,               // approximated integral with 3 points
    SEGMENT = 8                 // exact integral on the segment
  };
  void fit(const Polyline &polyline, FitType type, double tolerance = 1e-8);

  // Classification
  enum Type {
    NO_CURVE = 0, LINE, TWO_LINES, ELLIPSE, PARABOLA, HYPERBOLA
  };
  Type classify(double tolerance = 1e-8) const;
};

constexpr Conic::FitType operator|(Conic::FitType lhs, Conic::FitType rhs) {
  return static_cast<Conic::FitType>(static_cast<int>(lhs) | static_cast<Conic::FitType>(rhs));
}
