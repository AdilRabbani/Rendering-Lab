#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "vec3.h"
#include "texture.h"

class triangle {
public:
  triangle() {}
  triangle(point3 p0_, point3 p1_, point3 p2_, color m) {
    p0 = p0_;
    p1 = p1_;
    p2 = p2_;
    albedo = m;
    has_texture = false;
  }

  triangle(point3 p0_, point3 p1_, point3 p2_, color m, texture t) {
    p0 = p0_;
    p1 = p1_;
    p2 = p2_;
    albedo = m;
    tex = t;
    has_texture = true;
    std::cout << "Hey this has texture!" << std::endl;
  }

  bool hit(const ray &r, double t_min, double t_max,
                   hit_record &rec);

public:
  point3 p0;
  point3 p1;
  point3 p2;
  color albedo;
  bool has_texture;
  texture tex;
};

bool triangle::hit(const ray &r, double t_min, double t_max,
                   hit_record &rec){
  const vec3 p0_to_p1 = p1 - p0; // edge 1
  const vec3 p0_to_p2 = p2 - p0; // edge 2

  vec3 p0_to_ray_origin = r.origin() - p0; // vector (direction) from one of the points on triangle to ray origin
  vec3 normal = cross(p0_to_p1, p0_to_p2); // normal vector to the plane

  vec3 q = cross(p0_to_ray_origin, r.direction()); // vector (direction) to the intersection point

  double determinant = 1 / dot(r.direction(), normal); // determinant 1 / (N . r)

  double u_ = determinant * dot(-q, p0_to_p2); // compute u
  if (u_ < 0.0 || u_ > 1.0)
    return false;
  double v_ = determinant * dot(q, p0_to_p1); // compute v
  if (v_ < 0.0 || (u_ + v_) > 1.0)
    return false;
  double t = determinant * dot(-normal, p0_to_ray_origin); // compute t which is -(N . origin + D) / (N . r) because n is // in the opposite direction of ray, we use -n

  // Find the nearest root that lies in the acceptable range.
  if (t < t_min || t_max < t) {
    return false;
  }

  rec.t = t;
  rec.p = r.at(rec.t);
  rec.u = u_;
  rec.v = v_;

  rec.set_face_normal(r, unit_vector(normal));

  if (has_texture) {
      int height = tex.texture_image_data.y_size();
      int width = tex.texture_image_data.x_size();
      double red = (double)(tex.texture_image_data[(int)round((rec.v) *  height)][(int)round(rec.u * width)].red / 255.0);
      double green = (double)(tex.texture_image_data[(int)round((rec.v) *  height)][(int)round(rec.u * width)].green / 255.0);
      double blue = (double)(tex.texture_image_data[(int)round((rec.v) *  height)][(int)round(rec.u * width)].blue / 255.0);
      rec.albedo = color(red, green, blue);
  }
  else {
      rec.albedo = albedo;
  }

  rec.name = "triangle";

  return true;
}

#endif