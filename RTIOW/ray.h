#ifndef RAY_H
#define RAY_H

#include "vec3.h"

// Special thanks to Ray Tracing in One Weekend:
// https://raytracing.github.io/books/RayTracingInOneWeekend.html

class ray {
    public:
        ray() {}
        ray(const point3& origin, const vec3& direction)
            : orig(origin), dir(direction)
        {}

        point3 origin() const  { return orig; }
        vec3 direction() const { return dir; }

        point3 at(double t) const {
            return orig + t*dir;
        }

    public:
        point3 orig;
        vec3 dir;
        bool shadow_ray = false;
};

#endif