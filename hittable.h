#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "constants.h"
#include "texture.h"

struct hit_record {
    point3 p;
    vec3 normal;
    color albedo;
    double t;
    bool front_face;
    double u = -1;
    double v = -1;
    std::string name;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal :-outward_normal;
    }
};
#endif