#include <fstream>
#include <functional>

#include "conic-fit.hh"

using namespace Geometry;
using Polyline = Conic::Polyline;

auto bbox(const Polyline &pl) {
  Point2D min(pl[0]), max(pl[0]);
  for (const auto &p : pl)
    for (size_t i = 0; i < 2; ++i)
      if (p[i] < min[i])
        min[i] = p[i];
      else if (p[i] > max[i])
        max[i] = p[i];
  return std::make_pair(min, max);
}

std::string names[] = {
  "no curve", "line", "two lines", "ellipse", "parabola", "hyperbola"
};

std::array<double, 6> canonical_coeffs[] = {
  //   1   x   y   x2  xy  y2
  { {  0,  0,  0,  0,  0,  0  } }, //  <placeholder>
  { {  0,  0,  1,  0,  0,  0  } }, //  1. single line
  { {  0,  0,  0,  0,  0,  1  } }, //  2. double line
  { { -1,  0,  0,  1,  0,  0  } }, //  3. parallel lines
  { {  0,  0,  0,  0,  1,  0  } }, //  4. intersecting lines
  { {  0,  0, -1,  1,  0,  0  } }, //  5. parabola
  { { -1,  0,  0,  1,  0,  2  } }, //  6. ellipse
  { {  1,  0,  0,  1,  0, -1  } }, //  7. hyperbola
};

Polyline readTXT(std::string filename) {
  std::ifstream f(filename);
  f.exceptions(std::ios::failbit | std::ios::badbit);
  Polyline result;
  while (!f.eof()) {
    double x, y;
    f >> x >> y;
    result.emplace_back(x, y);
    f >> std::ws;
  }
  return result;
}

void writeDistances(const std::function<double(const Point2D &)> &f,
                    const Point2D &center, double radius, size_t resolution,
                    std::string filename) {
  TriMesh mesh;
  mesh.resizePoints(resolution * resolution);
  for (size_t i = 0; i < resolution; ++i) {
    double x = center[0] + ((double)i / (resolution - 1) * 2 - 1) * radius;
    for (size_t j = 0; j < resolution; ++j) {
      double y = center[1] + ((double)j / (resolution - 1) * 2 - 1) * radius;
      double z = f({x, y});
      mesh[i * resolution + j] = Point3D(x, y, z);
    }
  }
  for (size_t i = 1; i < resolution; ++i)
    for (size_t j = 1; j < resolution; ++j) {
      mesh.addTriangle((i - 1) * resolution + j - 1,
                       (i - 1) * resolution + j,
                       i * resolution + j);
      mesh.addTriangle((i - 1) * resolution + j - 1,
                       i * resolution + j,
                       i * resolution + j - 1);
    }
  mesh.writeOBJ(filename);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: " << std::endl;
    std::cerr << "  " << argv[0] << " <input.txt>" << std::endl;
    std::cerr << "Or:" << std::endl;
    std::cerr << "  " << argv[0] << " <default curve # (1-7)>" << std::endl;
    return 1;
  }

  Point2D center(0, 0);
  double radius = 4;

  Conic cf;
  int canonical = std::atoi(argv[1]);
  if (canonical <= 0 || canonical > 7) {
    auto polyline = readTXT(argv[1]);
    cf.fit(polyline);
    auto [min, max] = bbox(polyline);
    center = (min + max) / 2;
    radius = (max - min).norm() / 2 * 1.2;
  } else
    cf.coeffs = canonical_coeffs[canonical];

  std::cout << "Conic:";
  for (auto si : cf.coeffs)
    std::cout << ' ' << si;
  std::cout << std::endl;
  std::cout << "Its type seems to be: " << names[cf.classify()] << std::endl;

  size_t res = 100;
  writeDistances([&](const Point2D &p) { return cf.eval(p); },
                 center, radius, res, "/tmp/algebraic.obj");
  writeDistances([&](const Point2D &p) { return cf.distance(p); },
                 center, radius, res, "/tmp/distance.obj");
  std::cout << "Surface written to /tmp/algebraic.obj." << std::endl;
  std::cout << "Approximate distances written to /tmp/distance.obj." << std::endl;
}
