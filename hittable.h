#ifndef HITTABLE_H
#define HITTABLE_H

#include "RTIOW/ray.h"
#include "constants.h"
#include "texture.h"

// Special thanks to Ray Tracing in One Weekend:
// https://raytracing.github.io/books/RayTracingInOneWeekend.html

struct hit_record {
  point3 p;
  vec3 normal;
  color albedo;
  double t;
  bool front_face;
  double u = -1;
  double v = -1;
  std::string name;

  inline void set_face_normal(const ray &r, const vec3 &outward_normal) {
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class primitive {
  public:
  virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};
#endif