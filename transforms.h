#ifndef TRANSFORMS_H
#define TRANSFORMS_H

#include "vec3.h"
#include <glm/fwd.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

vec3 translate(vec3 vertex, vec3 translate_by) {
    glm::mat4 myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(translate_by.x(), translate_by.y(), translate_by.z()));
    glm::vec4 myVector(vertex.x(), vertex.y(), vertex.z(), 1.0f);
    glm::vec4 transformedVector = myMatrix * myVector;
    vec3 translated_vertex(transformedVector.x, transformedVector.y, transformedVector.z);
    return translated_vertex;
}

vec3 scale(vec3 vertex, vec3 scale_by) {
    glm::mat4 myScalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_by.x(), scale_by.y(), scale_by.z()));
    glm::vec4 myVector(vertex.x(), vertex.y(), vertex.z(), 1.0f);
    glm::vec4 transformedVector = myScalingMatrix * myVector;
    vec3 scaled_vertex(transformedVector.x, transformedVector.y, transformedVector.z);
    return scaled_vertex;
}

vec3 rotate(vec3 vertex, float angle_in_degrees, vec3 axis_of_rotation) {
    glm::mat4 transformation = glm::mat4(1.0f);
    transformation = glm::rotate(transformation, glm::radians(angle_in_degrees), glm::vec3(axis_of_rotation.x(), axis_of_rotation.y(), axis_of_rotation.z()));
    glm::vec3 transformedVector = transformation * glm::vec4(vertex.x(), vertex.y(), vertex.z(), 1.0f);
    vec3 rotated_vertex(transformedVector.x, transformedVector.y, transformedVector.z);
    return rotated_vertex;
}

#endif