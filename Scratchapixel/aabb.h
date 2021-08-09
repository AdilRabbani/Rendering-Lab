#ifndef AABB_H
#define AABB_H

#include "../hittable.h"
#include <algorithm>
#include <array>
#include <math.h>
#include <type_traits>

class aabb {

public:
  aabb() {}
  aabb(const point3 &point_a, const point3 &point_b) {
    min_ = point_a;
    max_ = point_b;
  }

  bool intersect(const ray &r);

  point3 min_;
  point3 max_;
};

// Special thanks to scratchapixel:
// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection

bool aabb::intersect(const ray &r) {

  double tmin = (min_.x() - r.orig.x()) / r.dir.x();
  double tmax = (max_.x() - r.orig.x()) / r.dir.x();

  if (tmin > tmax)
    std::swap(tmin, tmax);

  double tymin = (min_.y() - r.orig.y()) / r.dir.y();
  double tymax = (max_.y() - r.orig.y()) / r.dir.y();

  if (tymin > tymax)
    std::swap(tymin, tymax);

  if ((tmin > tymax) || (tymin > tmax))
    return false;

  if (tymin > tmin)
    tmin = tymin;

  if (tymax < tmax)
    tmax = tymax;

  double tzmin = (min_.z() - r.orig.z()) / r.dir.z();
  double tzmax = (max_.z() - r.orig.z()) / r.dir.z();

  if (tzmin > tzmax)
    std::swap(tzmin, tzmax);

  if ((tmin > tzmax) || (tzmin > tmax))
    return false;

  if (tzmin > tmin)
    tmin = tzmin;

  if (tzmax < tmax)
    tmax = tzmax;

  return true;
}

#endif