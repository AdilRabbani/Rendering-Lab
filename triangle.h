#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "primitive.h"
#include "hittable.h"
#include "texture.h"

inline void triangle_rotate_in_x(point3 &p1, point3 &p2, point3 &p3,
                          double rotate_angle_x) {

  double v1_x, v2_x, v3_x;
  double v1_y, v2_y, v3_y;
  double v1_z, v2_z, v3_z;

  v1_x = p1.x();
  v1_y = p1.y() * cos(rotate_angle_x) + p1.z() * sin(rotate_angle_x);
  v1_z = p1.y() * (-sin(rotate_angle_x)) + p1.z() * cos(rotate_angle_x);

  v2_x = p2.x();
  v2_y = p2.y() * cos(rotate_angle_x) + p2.z() * sin(rotate_angle_x);
  v2_z = p2.y() * (-sin(rotate_angle_x)) + p2.z() * cos(rotate_angle_x);

  v3_x = p3.x();
  v3_y = p3.y() * cos(rotate_angle_x) + p3.z() * sin(rotate_angle_x);
  v3_z = p3.y() * (-sin(rotate_angle_x)) + p3.z() * cos(rotate_angle_x);

  p1 = vec3(v1_x, v1_y, v1_z);
  p2 = vec3(v2_x, v2_y, v2_z);
  p3 = vec3(v3_x, v3_y, v3_z);
}

inline void triangle_rotate_in_y(point3 &p1, point3 &p2, point3 &p3,
                          double rotate_angle_y) {

  double v1_x, v2_x, v3_x;
  double v1_y, v2_y, v3_y;
  double v1_z, v2_z, v3_z;

  v1_x = p1.x() * cos(rotate_angle_y) + p1.z() * -sin(rotate_angle_y);
  v1_y = p1.y();
  v1_z = p1.x() * (sin(rotate_angle_y)) + p1.z() * cos(rotate_angle_y);

  v2_x = p2.x() * cos(rotate_angle_y) + p2.z() * -sin(rotate_angle_y);
  v2_y = p2.y();
  v2_z = p2.x() * (sin(rotate_angle_y)) + p2.z() * cos(rotate_angle_y);

  v3_x = p3.x() * cos(rotate_angle_y) + p3.z() * -sin(rotate_angle_y);
  v3_y = p3.y();
  v3_z = p3.x() * (sin(rotate_angle_y)) + p3.z() * cos(rotate_angle_y);

  p1 = vec3(v1_x, v1_y, v1_z);
  p2 = vec3(v2_x, v2_y, v2_z);
  p3 = vec3(v3_x, v3_y, v3_z);
}

inline void triangle_rotate_in_z(point3 &p1, point3 &p2, point3 &p3,
                          double rotate_angle_z) {

  double v1_x, v2_x, v3_x;
  double v1_y, v2_y, v3_y;
  double v1_z, v2_z, v3_z;

  v1_x = p1.x() * cos(rotate_angle_z) + p1.y() * sin(rotate_angle_z);
  v1_y = p1.x() * (-sin(rotate_angle_z)) + p1.y() * cos(rotate_angle_z);
  v1_z = p1.z();

  v2_x = p2.x() * cos(rotate_angle_z) + p2.y() * sin(rotate_angle_z);
  v2_y = p2.x() * (-sin(rotate_angle_z)) + p2.y() * cos(rotate_angle_z);
  v2_z = p2.z();

  v3_x = p3.x() * cos(rotate_angle_z) + p3.y() * sin(rotate_angle_z);
  v3_y = p3.x() * (-sin(rotate_angle_z)) + p3.y() * cos(rotate_angle_z);
  v3_z = p3.z();

  p1 = vec3(v1_x, v1_y, v1_z);
  p2 = vec3(v2_x, v2_y, v2_z);
  p3 = vec3(v3_x, v3_y, v3_z);
}

class triangle : public primitive {
public:
  triangle() {}
  triangle(point3 p0_, point3 p1_, point3 p2_, color m) {
    p0 = p0_;
    p1 = p1_;
    p2 = p2_;
    albedo = m;
    has_texture = false;
    center = point3((p0.x() + p1.x() + p2.x()) / 3, (p0.y() + p1.y() + p2.y()) / 3, (p0.z() + p1.z() + p2.z()) / 3);
  }

  triangle(point3 p0_, point3 p1_, point3 p2_, texture t) {
    p0 = p0_;
    p1 = p1_;
    p2 = p2_;
    tex = t;
    has_texture = true;
    center = point3((p0.x() + p1.x() + p2.x()) / 3, (p0.y() + p1.y() + p2.y()) / 3, (p0.z() + p1.z() + p2.z()) / 3);
  }

  void transform(vec3 translate_by, vec3 scale_by, vec3 rotate_by) {

    double rotate_x = rotate_by.x() * 0.01745;
    double rotate_y = rotate_by.y() * 0.01745;
    double rotate_z = rotate_by.z() * 0.01745;

    triangle_rotate_in_z(p0, p1, p2, rotate_z);
    triangle_rotate_in_x(p0, p1, p2, rotate_x);
    triangle_rotate_in_y(p0, p1, p2, rotate_y);

    p0 = vec3(p0.x() * scale_by.x(), p0.y() * scale_by.y(),
              p0.z() * scale_by.z());
    p1 = vec3(p1.x() * scale_by.x(), p1.y() * scale_by.y(),
              p1.z() * scale_by.z());
    p2 = vec3(p2.x() * scale_by.x(), p2.y() * scale_by.y(),
              p2.z() * scale_by.z());

    p0 = p0 + translate_by;
    p1 = p1 + translate_by;
    p2 = p2 + translate_by;
    center = point3((p0.x() + p1.x() + p2.x()) / 3, (p0.y() + p1.y() + p2.y()) / 3, (p0.z() + p1.z() + p2.z()) / 3);
  }

  virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

  virtual aabb construct_aabb() override {
    vec3 max_of_two = get_maximum_vector_for_aabb(p0, p1);
    vec3 aabb_max = get_maximum_vector_for_aabb(p2, max_of_two);
    vec3 min_of_two = get_minimum_vector_for_aabb(p0, p1);
    vec3 aabb_min = get_minimum_vector_for_aabb(p2, min_of_two);
    return aabb(aabb_min, aabb_max);
  }

  virtual vec3 get_center() override {
    return center;
  }

public:
  point3 p0;
  point3 p1;
  point3 p2;
  point3 center;
  color albedo;
  bool has_texture;
  texture tex;
};

// Special thanks to author Inigo Quilez:
// https://www.iquilezles.org/www/articles/intersectors/intersectors.htm

bool triangle::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {

  const vec3 p0_to_p1 = p1 - p0;
  const vec3 p0_to_p2 = p2 - p0;

  vec3 p0_to_ray_origin = r.origin() - p0;
  vec3 normal = cross(p0_to_p1, p0_to_p2);

  vec3 q = cross(p0_to_ray_origin, r.direction());

  double determinant = 1 / dot(r.direction(), normal);

  double u_ = determinant * dot(-q, p0_to_p2);
  if (u_ < 0.0 || u_ > 1.0)
    return false;
  double v_ = determinant * dot(q, p0_to_p1);
  if (v_ < 0.0 || (u_ + v_) > 1.0)
    return false;
  double t = determinant * dot(-normal, p0_to_ray_origin);

  if (t < t_min || t_max < t) {
    return false;
  }

  rec.t = t;
  rec.p = r.at(rec.t);
  rec.u = u_;
  rec.v = v_;

  rec.set_face_normal(r, unit_vector(normal));

  if (has_texture) {
    rec.albedo = tex.get_texel(rec.u, rec.v);
  } else {
    rec.albedo = albedo;
  }

  rec.name = "triangle";

  return true;
}

#endif