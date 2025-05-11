#include <cmath>

#include "conic-fit.hh"

using namespace Geometry;

std::tuple<Matrix2x2, Vector2D, double> Conic::matrixForm() const {
  Matrix2x2 Q{
    coeffs[3],     coeffs[4] / 2,
    coeffs[4] / 2, coeffs[5]
  };
  Vector2D P(coeffs[1], coeffs[2]);
  return std::make_tuple(Q, P, coeffs[0]);
}

double Conic::eval(const Point2D &p) const {
  return
    coeffs[0] +
    coeffs[1] * p[0] + coeffs[2] * p[1] +
    coeffs[3] * p[0] * p[0] + coeffs[4] * p[0] * p[1] + coeffs[5] * p[1] * p[1];
}

Vector2D Conic::grad(const Point2D &p) const {
  return {
    coeffs[1] + coeffs[3] * 2 * p[0] + coeffs[4] * p[1],
    coeffs[2] + coeffs[4] * p[0] + coeffs[5] * 2 * p[1]
  };
}

double Conic::distance(const Point2D &p) const {
  auto a = -Vector3D(coeffs[3], coeffs[4] / std::sqrt(2), coeffs[5]).norm();
  auto b = -grad(p).norm();
  auto c = std::abs(eval(p));

  auto D = b * b - 4 * a * c;
  return (-b - std::sqrt(D)) / (2 * a);
}

Conic::Type Conic::classify(double tolerance) const {
  auto [Q, P, R] = matrixForm();
  auto det = Q.determinant();
  auto beta = det * R +
    (coeffs[1] * coeffs[2] * coeffs[4] -
     coeffs[1] * coeffs[1] * coeffs[5] -
     coeffs[2] * coeffs[2] * coeffs[3]) / 4;

  // Non-degenerate case
  if (std::abs(beta) >= tolerance) {
    if (std::abs(det) < tolerance)
      return PARABOLA;
    return det >= tolerance ? ELLIPSE : HYPERBOLA;
  }

  // Parabolic degenerate
  if (std::abs(det) < tolerance) {
    if (std::abs(coeffs[3]) + std::abs(coeffs[4]) + std::abs(coeffs[5]) >= tolerance)
      return TWO_LINES;
    return P.normSqr() < tolerance ? NO_CURVE : LINE;
  }

  // Elliptic/hyperbolic degenerate
  return det >= tolerance ? NO_CURVE : TWO_LINES;
}
