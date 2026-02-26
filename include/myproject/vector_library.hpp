#ifndef SAMPLE_LIBRARY_HPP
#define SAMPLE_LIBRARY_HPP

#include <cmath>

struct Vector3d
{
  double x;
  double y;
  double z;
  Vector3d(double x, double y, double z)
    : x(x), y(y), z(z)
  {}
  Vector3d() : x(0.F), y(0.F), z(0.F) {}


  Vector3d operator+(const Vector3d &other) const { return Vector3d(x + other.x, y + other.y, z + other.z); }
  Vector3d operator-(const Vector3d &other) const { return Vector3d(x - other.x, y - other.y, z - other.z); }

  double length() const { return std::sqrt(x * x + y * y + z * z); }

  Vector3d norm() const { return *this / length(); }

  Vector3d operator*(const Vector3d &other) const
  {
    return Vector3d(
        y * other.z - z * other.y,
        x * other.z - z * other.x,
        x * other.y - y * other.x
    );
  }

  Vector3d operator*(const double factor) const
  {
    return Vector3d(x * factor, y * factor, z * factor);
  }

  Vector3d operator-() const { return Vector3d(-x, -y, -z); }

  Vector3d operator/(const double factor) const { return *this * (1.0 / factor); }

  double dot(const Vector3d &other) const { return x * other.x + y * other.y + z * other.z; }

  bool operator==(const Vector3d &other) const {
    const double EPS = 1e-9;
    return std::fabs(x - other.x) < EPS && std::fabs(y - other.y) < EPS && std::fabs(z - other.z) < EPS;
  }

};

#endif
