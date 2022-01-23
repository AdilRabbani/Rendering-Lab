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

  bool intersect(const ray &r, double &tmin_, double &tmax);

  vec3 diagonal () {
    return max_ - min_;
  }

  double surface_area () {
    vec3 d = diagonal();
    return 2 * (d.x() * d.y() + d.x() * d.z() + d.y() * d.z());
  }

  int maximum_extent () {
    vec3 d = diagonal();
    // std::cout << std::endl << "x: " << d.x() << ", y: " << d.y() << " z: " << d.z() << std::endl;
    if (d.x() > d.y() && d.x() > d.z()) {
      return 0;
    }
    else if (d.y() > d.z()) {
      return 1;
    }
    else {return 2;};
  }

  point3 min_;
  point3 max_;
};

// Special thanks to scratchapixel:
// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection

bool aabb::intersect(const ray &r, double &tmin_, double &tmax_) {

  double inv_dir_x = 1.0f / r.dir.x();

  double tmin = (min_.x() - r.orig.x()) * inv_dir_x;
  double tmax = (max_.x() - r.orig.x()) * inv_dir_x;

  // std::cout << "dir_x: " << r.dir.x() << std::endl;

  if (tmin > tmax)
    std::swap(tmin, tmax);

  double inv_dir_y = 1.0f / r.dir.y();

  double tymin = (min_.y() - r.orig.y()) * inv_dir_y;
  double tymax = (max_.y() - r.orig.y()) * inv_dir_y;

  // std::cout << "dir_y: " << r.dir.y() << std::endl;

  if (tymin > tymax)
    std::swap(tymin, tymax);

  if ((tmin > tymax) || (tymin > tmax))
    return false;

  if (tymin > tmin)
    tmin = tymin;

  if (tymax < tmax)
    tmax = tymax;

  double inv_dir_z = 1.0f / r.dir.z();

  double tzmin = (min_.z() - r.orig.z()) * inv_dir_z;
  double tzmax = (max_.z() - r.orig.z()) * inv_dir_z;

  // std::cout << "dir_z: " << r.dir.z() << std::endl;

  if (tzmin > tzmax)
    std::swap(tzmin, tzmax);

  if ((tmin > tzmax) || (tzmin > tmax))
    return false;

  if (tzmin > tmin)
    tmin = tzmin;

  if (tzmax < tmax)
    tmax = tzmax;

  if (!r.shadow_ray) {
    tmin_ = tmin;
    tmax_ = tmax;
  }
  
  return true;

}

#endif