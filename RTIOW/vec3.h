#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

// Special thanks to Ray Tracing in One Weekend:
// https://raytracing.github.io/books/RayTracingInOneWeekend.html

using std::sqrt;

class vec3 {
public:
  vec3() : e{0, 0, 0} {}
  vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

  double x() const { return e[0]; }
  double y() const { return e[1]; }
  double z() const { return e[2]; }

  vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
  double operator[](int i) const { return e[i]; }
  double &operator[](int i) { return e[i]; }

  vec3 &operator+=(const vec3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
  }

  vec3 &operator*=(const double t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }

  vec3 &operator/=(const double t) { return *this *= 1 / t; }

  double length() const { return sqrt(length_squared()); }

  double length_squared() const {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
  }

public:
  double e[3];
};

// Type aliases for vec3
using point3 = vec3; // 3D point
using color = vec3;  // RGB color

// vec3 Utility Functions

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v) {
  return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t) { return t * v; }

inline vec3 operator/(vec3 v, double t) { return (1 / t) * v; }

inline double dot(const vec3 &u, const vec3 &v) {
  return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
  return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
              u.e[2] * v.e[0] - u.e[0] * v.e[2],
              u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) { return v / v.length(); }

vec3 reflect(const vec3 &v, const vec3 &n) { return v - 2 * dot(v, n) * n; }

inline vec3 get_minimum_vector_for_aabb(const vec3 &vector_1, const vec3 &vector_2) {
  
  vec3 minimum_vector;
  double min_x, min_y, min_z;

  if (vector_1.x() < vector_2.x()) {
    min_x = vector_1.x();
  }
  else {
    min_x = vector_2.x();
  }

  if (vector_1.y() < vector_2.y()) {
    min_y = vector_1.y();
  }
  else {
    min_y = vector_2.y();
  }

  if (vector_1.z() < vector_2.z()) {
    min_z = vector_1.z();
  }
  else {
    min_z = vector_2.z();
  }

  return vec3(min_x, min_y, min_z);
}

inline vec3 get_maximum_vector_for_aabb(const vec3 &vector_1, const vec3 &vector_2) {
  
  vec3 maximum_vector;
  double max_x, max_y, max_z;

  if (vector_1.x() > vector_2.x()) {
    max_x = vector_1.x();
  }
  else {
    max_x = vector_2.x();
  }

  if (vector_1.y() > vector_2.y()) {
    max_y = vector_1.y();
  }
  else {
    max_y = vector_2.y();
  }

  if (vector_1.z() > vector_2.z()) {
    max_z = vector_1.z();
  }
  else {
    max_z = vector_2.z();
  }

  return vec3(max_x, max_y, max_z);
}
#endif