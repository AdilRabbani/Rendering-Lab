#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "Scratchapixel/aabb.h"

class primitive {
  public:
  primitive() {}
  virtual aabb construct_aabb() = 0;
  virtual vec3 get_center() = 0;
  virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif