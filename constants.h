#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

const double SHADOW_BIAS = 0.001;
const int SPECULAR_M = 10;
const double SPECULAR_K_SPEC = 0.1;
const double AMBIENT_CONSTANT = 0.05;
const bool USE_SHADOWS = true;
const bool NORMAL_MODE = true;
const bool DEBUG_GRID = false;
const bool USE_UNIFORM_GRIDS = false;
const bool USE_BVH = true;
const double AREA_LIGHTS_SPREAD = 0.05; // previously 0.1

// Special thanks to Ray Tracing in One Weekend:
// https://raytracing.github.io/books/RayTracingInOneWeekend.html

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
  return degrees * pi / 180.0;
}

inline double random_double() {
  // Returns a random real in [0,1).
  return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
  // Returns a random real in [min,max).
  return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max) {
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}

// Common Headers

#include "RTIOW/ray.h"

inline color rgb_to_01(float red, float green, float blue) {
  return color(red / 255, green / 255, blue / 255);
}

#endif