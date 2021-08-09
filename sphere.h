#ifndef SPHERE_H
#define SPHERE_H

#include "Scratchapixel/aabb.h"
#include "hittable.h"
#include "texture.h"
#include <math.h>

// Special thanks to Ray Tracing in One Weekend:
// https://raytracing.github.io/books/RayTracingInOneWeekend.html

class sphere {
public:
  sphere() {}
  sphere(point3 cen, double r, color m) {
    center = cen;
    radius = r;
    albedo = m;
    has_texture = false;
    sphere_aabb = aabb(cen - vec3(r, r, r), cen + vec3(r, r, r));
    // std::cout << sphere_aabb.min_ << " " << sphere_aabb.max_ << std::endl;
  }
  sphere(point3 cen, double r, texture &t) {
    center = cen;
    radius = r;
    tex = t;
    has_texture = true;
    sphere_aabb = aabb(cen - vec3(r, r, r), cen + vec3(r, r, r));
    // std::cout << sphere_aabb.min_ << " " << sphere_aabb.max_ << std::endl;
  }

  bool hit(const ray &r, double t_min, double t_max, hit_record &rec);

  void translate(vec3 translate_by) { center = center + translate_by; }
  void scale(double scale_by) { radius = radius * scale_by; }

public:
  point3 center;
  double radius;
  color albedo;
  bool has_texture;
  texture tex;
  aabb sphere_aabb;
};

bool sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) {

  vec3 oc = r.origin() - center;
  auto a = r.direction().length_squared();
  auto half_b = dot(oc, r.direction());
  auto c = oc.length_squared() - radius * radius;

  auto discriminant = half_b * half_b - a * c;
  if (discriminant < 0)
    return false;
  auto sqrtd = sqrt(discriminant);

  // Find the nearest root that lies in the acceptable range.
  auto root = (-half_b - sqrtd) / a;
  if (root < t_min || t_max < root) {
    root = (-half_b + sqrtd) / a;
    if (root < t_min || t_max < root)
      return false;
  }

  rec.t = root;
  rec.p = r.at(rec.t);

  vec3 outward_normal = (rec.p - center) / radius;
  rec.set_face_normal(r, outward_normal);
  rec.u = atan2(rec.normal.x(), rec.normal.z()) / (2 * pi) + 0.5;
  rec.v = 0.5 - asin(rec.normal.y()) / pi;

  if (has_texture) {
    rec.albedo = tex.get_texel(rec.u, rec.v);
  }

  else {
    rec.albedo = albedo;
  }

  return true;
}

#endif