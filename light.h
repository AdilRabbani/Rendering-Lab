#ifndef LIGHT_H
#define LIGHT_H

#include "constants.h"
#include "hittable.h"

#include <string>
#include <vector>
class Light {
public:
  Light() {}
  virtual ~Light() {}
  color light_color;
  double intensity;
};

class PointLight : public Light {
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
    newSample.e[0] =
        this->position.e[0] + radius * (2.0 * random_double() - 1.0);
    newSample.e[1] =
        this->position.e[1] + radius * (2.0 * random_double() - 1.0);
    newSample.e[2] =
        this->position.e[2] + radius * (2.0 * random_double() - 1.0);
    return newSample;
  }

public:
  float radius;
};

#endif