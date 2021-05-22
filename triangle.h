#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "vec3.h"

class triangle : public hittable {
    public:
        triangle() {}
        triangle(point3 p0_, point3 p1_, point3 p2_) : p0(p0_), p1(p1_), p2(p2_) {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

    public:
        point3 p0;
        point3 p1;
        point3 p2;
};

bool triangle::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    const vec3 p0p1 = p1 - p0;                      // edge 1
    const vec3 p0p2 = p2 - p0;                      // edge 2

    vec3 rop0 = r.origin() - p0;                        // vector (direction) from one of the points on triangle to ray origin
    vec3 normal = cross(p0p1, p0p2);                    // normal vector to the plane

    vec3 q = cross(rop0, r.direction());                // vector (direction) to the intersection point
    
    double determinant = 1 / dot(r.direction(), normal);     // determinant 1 / (N . r)

    double u_ = determinant * dot( -q, p0p2);           // compute u
    if (u_ < 0.0 || u_ > 1.0) return false;
    double v_ = determinant * dot( q, p0p1);            // compute v
    if (v_ < 0.0 || (u_ + v_) > 1.0) return false;
    double t = determinant * dot( -normal, rop0);            // compute t which is -(N . origin + D) / (N . r) because n is in the opposite direction of ray, we use -n

    // if (u_ < 0.0 || u_ > 1.0 || v_ < 0.0 || (u_ + v_) > 1.0) return false;

    // Find the nearest root that lies in the acceptable range.
    if (t < t_min || t_max < t) {
        return false;
    }

    rec.t = t;
    rec.p = r.at(rec.t);
    rec.u = u_;
    rec.v = v_;

    rec.normal = unit_vector(normal);

    return true;
}

#endif