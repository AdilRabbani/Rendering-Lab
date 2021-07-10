#ifndef LIGHT_H
#define LIGHT_H

#include "constants.h"
#include "hittable.h"
#include "vec3.h"

#include <string>
#include <vector>

// Light Direction = P - Lp where P is the point on the surface and Lp is the spherical light position
// Light amount = light color (r, g, b) * light intensity
// the light color can be kept in the range of [0, 1]
// the intensity however can be kept to any value from 0 to a large value

// cosQ = N.L the amount of light that a surface receives is directly proportional
// to the angle between the surface normal N and the light direction L

// albedo = reflect light / incident light

// diffuse surface color = albedo = rho * incident light energy * N.L

class Light {
    public:
    Light() {}
    virtual ~Light() {}
    color light_color;
    double intensity;
};

class PointLight : public Light{
    public:
    PointLight() {}
    PointLight(color lc, double i, vec3 p) {
        this->position = p;
        this->light_color = lc;
        this->intensity = i;
    }
    vec3 position;
};

class AreaLight : public PointLight {
    public:
        AreaLight() {}
        AreaLight(color lc, double i, vec3 p, float r) {
            this->light_color = lc;
            this->radius = r;
            this->intensity = i;
            this->position = p;
        }
        vec3 getRandomSample() {
            vec3 newSample;
            newSample.e[0] = this->position.e[0] + radius * (2.0 * random_double() - 1.0);
            newSample.e[1] = this->position.e[1] + radius * (2.0 * random_double() - 1.0);
            newSample.e[2] = this->position.e[2] + radius * (2.0 * random_double() - 1.0);
            return newSample;
        }
    public:
        float radius;
};

#endif