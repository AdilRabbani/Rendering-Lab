#ifndef SCENE_H
#define SCENE_H

#include "RTIOW/camera.h"
#include "RTIOW/color.h"
#include "RTIOW/vec3.h"
#include "Scratchapixel/aabb.h"
#include "constants.h"
#include "hittable.h"
#include "light.h"
#include "mesh.h"
#include "sphere.h"
#include "triangle.h"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include <chrono>
using std::chrono::duration;
using std::chrono::high_resolution_clock;

#include "bvh.h"

// #include <thread>
// #include <mutex>

// std::mutex render_thread_lock;

struct object_reference {
  triangle *reference;
  long long int index = 0;
};

inline void PhongIllumination(color light_color, vec3 light_position,
                              const ray &r, hit_record &hit, color albedo,
                              color &diffuse, color &specular, color &ambient,
                              double &intensity_at_point) {

  vec3 light_dir = light_position - hit.p;

  intensity_at_point = sqrt(light_dir.length());

  light_dir = unit_vector(light_dir);

  // for specular
  vec3 v = unit_vector(r.orig - hit.p);
  vec3 reflection = reflect(-light_dir, unit_vector(hit.normal));
  double spec = pow(std::max(dot(v, reflection), 0.0), SPECULAR_M);

  // specular
  specular = light_color * spec * SPECULAR_K_SPEC;

  // diffuse
  diffuse = std::max(0.0, dot(hit.normal, light_dir)) * 1 / pi * light_color;

  // ambient
  ambient = ((color(albedo.e[0], albedo.e[1], albedo.e[2]))) * AMBIENT_CONSTANT;
}

class Scene {
public:
  Scene() {
    apply_ambient = 1;
    apply_diffuse = 1;
    apply_specular = 1;
    with_aabb = 1;
    number_of_area_lights = 0;
    // ray_mesh_tests = 0;
    ray_triangle_tests = 0;
    ray_sphere_tests = 0;
    texture_set = 0;
    color_set = 0;
    number_of_objects = 0;
  }
  Scene(bool ambient_bool, bool diffuse_bool, bool specular_bool,
        bool with_aabb_bool) {
    apply_ambient = ambient_bool;
    apply_diffuse = diffuse_bool;
    apply_specular = specular_bool;
    with_aabb = with_aabb_bool;
    number_of_area_lights = 0;
    // ray_mesh_tests = 0;
    ray_triangle_tests = 0;
    ray_sphere_tests = 0;
    texture_set = 0;
    color_set = 0;
    number_of_objects = 0;
  }

  Scene(bool ambient_bool, bool diffuse_bool, bool specular_bool,
        bool with_aabb_bool, color background_color_arg) {
    apply_ambient = ambient_bool;
    apply_diffuse = diffuse_bool;
    apply_specular = specular_bool;
    with_aabb = with_aabb_bool;
    number_of_area_lights = 0;
    // ray_mesh_tests = 0;
    ray_triangle_tests = 0;
    ray_sphere_tests = 0;
    background_color = background_color_arg;
    texture_set = 0;
    color_set = 1;
    number_of_objects = 0;
  }
  Scene(bool ambient_bool, bool diffuse_bool, bool specular_bool,
        bool with_aabb_bool, texture background_texture_arg) {
    apply_ambient = ambient_bool;
    apply_diffuse = diffuse_bool;
    apply_specular = specular_bool;
    with_aabb = with_aabb_bool;
    number_of_area_lights = 0;
    // ray_mesh_tests = 0;
    ray_triangle_tests = 0;
    ray_sphere_tests = 0;
    background_texture = background_texture_arg;
    texture_set = 1;
    color_set = 0;
    number_of_objects = 0;
  }
  void add_triangle(triangle t) { scene_triangles.push_back(t); number_of_objects = number_of_objects + 1; }
  void add_sphere(sphere s) { scene_spheres.push_back(s); number_of_objects = number_of_objects + 1; }
  void add_mesh(mesh m) { scene_meshes.push_back(m); number_of_objects = number_of_objects + m.mesh_triangles.size();}
  void add_point_light(PointLight p) { scene_point_lights.push_back(p); }

  void remove_meshes() { scene_meshes.clear(); }

  void remove_triangles() { scene_triangles.clear(); }

  void remove_spheres() { scene_spheres.clear(); }

  void remove_point_lights() { scene_point_lights.clear(); }

  void remove_area_lights() { scene_area_lights.clear(); }

  void clear_scene() {
    remove_meshes();
    remove_triangles();
    remove_spheres();
    remove_point_lights();
    remove_area_lights();
  }

  void add_area_light(AreaLight a, int no_of_samples) {

    for (int l = 0; l <= no_of_samples - 1; l++) {
      for (int m = 0; m <= no_of_samples - 1; m++) {
        scene_area_lights.push_back(
            AreaLight(a.light_color, a.intensity,
                      vec3(a.position.x() + l * AREA_LIGHTS_SPREAD, a.position.y() + l * AREA_LIGHTS_SPREAD,
                           a.position.z() + m * AREA_LIGHTS_SPREAD),
                      a.radius));
      }
    }

    number_of_area_lights = number_of_area_lights + 1;
  }

  bool traverse_uniform_grid(const ray &r, const double t_min, const double t_max, hit_record &hit) {

    vec3 ray_start = r.origin() + r.direction() * t_min;
    vec3 ray_end = r.origin() + r.direction() * t_max;

    int cur_x_index = clamp(std::floor((ray_start.x() - scene_aabb.min_.x()) / cell_dimension_x), 0, grid_resolution_x - 1);
    int end_x_index = clamp(std::floor((ray_end.x() - scene_aabb.min_.x()) / cell_dimension_x), 0, grid_resolution_x - 1);

    int cur_y_index = clamp(std::floor((ray_start.y() - scene_aabb.min_.y()) / cell_dimension_y), 0, grid_resolution_y - 1);
    int end_y_index = clamp(std::floor((ray_end.y() - scene_aabb.min_.y()) / cell_dimension_y), 0, grid_resolution_y - 1);

    int cur_z_index = clamp(std::floor((ray_start.z() - scene_aabb.min_.z()) / cell_dimension_z), 0, grid_resolution_z - 1);
    int end_z_index = clamp(std::floor((ray_end.z() - scene_aabb.min_.z()) / cell_dimension_z), 0, grid_resolution_z - 1);

    int step_x;
    double t_delta_x, t_max_x;

    if (r.direction().x() > 0) {
      step_x = 1;
      t_delta_x = cell_dimension_x / r.direction().x();
      t_max_x = t_min + (scene_aabb.min_.x() + (cur_x_index + 1) * cell_dimension_x - ray_start.x()) / r.direction().x();
    } else if (r.direction().x() < 0) {
      step_x = -1;
      t_delta_x = cell_dimension_x / -r.direction().x();
      t_max_x = t_min + (scene_aabb.min_.x() + cur_x_index * cell_dimension_x - ray_start.x()) / r.direction().x();
    } else {
      step_x = 0;
      t_delta_x = t_max;
      t_max_x = t_max;
    }

    int step_y;
    double t_delta_y, t_max_y;

    if (r.direction().y() > 0) {
      step_y = 1;
      t_delta_y = cell_dimension_y / r.direction().y();
      t_max_y = t_min + (scene_aabb.min_.y() + (cur_y_index + 1) * cell_dimension_y - ray_start.y()) / r.direction().y();
    } else if (r.direction().y() < 0) {
      step_y = -1;
      t_delta_y = cell_dimension_y / -r.direction().y();
      t_max_y = t_min + (scene_aabb.min_.y() + cur_y_index * cell_dimension_y - ray_start.y()) / r.direction().y();
    } else {
      step_y = 0;
      t_delta_y = t_max;
      t_max_y = t_max;
    }

    int step_z;
    double t_delta_z, t_max_z;

    if (r.direction().z() > 0) {
      step_z = 1;
      t_delta_z = cell_dimension_z / r.direction().z();
      t_max_z = t_min + (scene_aabb.min_.z() + (cur_z_index + 1) * cell_dimension_z - ray_start.z()) / r.direction().z();
    } else if (r.direction().z() < 0) {
      step_z = -1;
      t_delta_z = cell_dimension_z / -r.direction().z();
      t_max_z = t_min + (scene_aabb.min_.z() + cur_z_index * cell_dimension_z - ray_start.z()) / r.direction().z();
    } else {
      step_z = 0;
      t_delta_z = t_max;
      t_max_z = t_max;
    }


    hit_record temp_hit = hit;
    bool hit_anything = false;
    double closest_so_far = t_max;

    while (cur_x_index != end_x_index || cur_y_index != end_y_index || cur_z_index != end_z_index) {

      int i = (((grid_resolution_y * cur_z_index) + cur_y_index) * grid_resolution_x) + cur_x_index;

      for (int j = uniform_grids_C_linearized[i]; j < uniform_grids_C_linearized[i + 1]; ++j) {
        triangle *triangle_test = uniform_grids_L[j].reference;
        if (triangle_test->hit(r, t_min, closest_so_far, temp_hit)) {
          ray_triangle_tests = ray_triangle_tests + 1;
          hit_anything = true;
          if (r.shadow_ray) {
            break;
          }
          closest_so_far = temp_hit.t;
          hit = temp_hit;
        }
      }

      if (t_max_x < t_max_y && t_max_x < t_max_z) {
        cur_x_index += step_x;
        t_max_x += t_delta_x;
      } else if (t_max_y < t_max_z) {
        cur_y_index += step_y;
        t_max_y += t_delta_y;
      } else {
        cur_z_index += step_z;
        t_max_z += t_delta_z;
      }

      if (cur_x_index < 0 || cur_y_index < 0 || cur_z_index < 0 || cur_x_index > grid_resolution_x-1 || cur_y_index > grid_resolution_y-1 || cur_z_index > grid_resolution_z-1) {
        break;
      }
    }

    return hit_anything;

  }
  

  color ray_color(const ray &r, double t_min, double t_max, double u,
                  double v, int depth) {

    hit_record hit;

    color hit_color(0, 0, 0);

    // vec3 point_light_debug;
    // double difference = (scene_point_lights[0].position - r.origin()).length();

    // if (r.at(difference).x() == scene_point_lights[0].position.x() && r.at(difference).y() == scene_point_lights[0].position.y() && r.at(difference).z() == scene_point_lights[0].position.z()
    // ) {
    //   return color(0, 0, 0);
    // }

    

      // return traverse_uniform_grid(r, scene_tmin, scene_tmax, hit);

      if (USE_UNIFORM_GRIDS) {
        if (scene_aabb.intersect(r, scene_tmin, scene_tmax)) {
          if (traverse_uniform_grid(r, scene_tmin, scene_tmax, hit)) {
            if (NORMAL_MODE) {
              return 0.5 * (hit.normal + color(1, 1, 1));
            } else {
              color albedo;

              albedo = hit.albedo;

              color diffuse(0, 0, 0);
              color specular(0, 0, 0);
              color ambient(0, 0, 0);
              double intensity_at_point;

              for (std::size_t i = 0; i < scene_point_lights.size(); ++i) {
                calculate_illumination_and_shadows(
                    r, hit, hit_color, albedo, ambient, diffuse, specular,
                    scene_point_lights[i].position, scene_point_lights[i].light_color,
                    scene_point_lights[i].intensity, intensity_at_point, 1, 1);
              }

              for (std::size_t i = 0; i < scene_area_lights.size(); ++i) {
                vec3 random_sample_area_light =
                    scene_area_lights[i].getRandomSample();
                calculate_illumination_and_shadows(
                    r, hit, hit_color, albedo, ambient, diffuse, specular,
                    random_sample_area_light, scene_area_lights[i].light_color,
                    scene_area_lights[i].intensity, intensity_at_point,
                    scene_area_lights.size() / number_of_area_lights, 2);
              }

              return hit_color + (ambient * apply_ambient);
            }
          } 
        }
      } else if (USE_BVH) {
        scene_bvh.hits.clear();
        if (scene_bvh.traverse_bvh(scene_bvh.root, r, scene_tmin, scene_tmax, 0.01, 10000, hit)) {
          hit_record closest_hit;
          for (std::size_t j = 0; j < scene_bvh.hits.size(); ++j) {
            if (j == 0) {
              closest_hit = scene_bvh.hits[j];
            }
            else {
              if (closest_hit.t > scene_bvh.hits[j].t) {
                closest_hit = scene_bvh.hits[j];
              }
            }
          }
          if (NORMAL_MODE) {
            return 0.5 * (closest_hit.normal + color(1, 1, 1));
          } else {
              color albedo;

              albedo = closest_hit.albedo;

              color diffuse(0, 0, 0);
              color specular(0, 0, 0);
              color ambient(0, 0, 0);
              double intensity_at_point;

              for (std::size_t i = 0; i < scene_point_lights.size(); ++i) {
                calculate_illumination_and_shadows(
                    r, closest_hit, hit_color, albedo, ambient, diffuse, specular,
                    scene_point_lights[i].position, scene_point_lights[i].light_color,
                    scene_point_lights[i].intensity, intensity_at_point, 1, 1);
              }

              for (std::size_t i = 0; i < scene_area_lights.size(); ++i) {
                vec3 random_sample_area_light =
                    scene_area_lights[i].getRandomSample();
                calculate_illumination_and_shadows(
                    r, closest_hit, hit_color, albedo, ambient, diffuse, specular,
                    random_sample_area_light, scene_area_lights[i].light_color,
                    scene_area_lights[i].intensity, intensity_at_point,
                    scene_area_lights.size() / number_of_area_lights, 2);
              }

              if (isnan(hit_color.x()) || isnan(hit_color.y()) || isnan(hit_color.z())) {
                std::cout << std::endl << "Warning: this hit color is nan at texture coordinate: (" << u << " , " << v << ")" << std::endl;
                std::cout << "Setting this pixel to black" << std::endl;  
                hit_color = color(0, 0, 0);
              }

              return hit_color + (ambient * apply_ambient);

              // hit_color = hit_color + (ambient * apply_ambient);
              // vec3 R = reflect(unit_vector(r.direction()), closest_hit.normal);
              // return hit_color += 0.8 * ray_color(ray(closest_hit.p + closest_hit.normal * 0.05, R), t_min, t_max, closest_hit.u, closest_hit.v, depth - 1);
          }
        }

      } else {
          if (object_hit(r, t_min, t_max, hit)) {
            if (NORMAL_MODE) {
              return 0.5 * (hit.normal + color(1, 1, 1));
            } else {
              color albedo;

              albedo = hit.albedo;

              color diffuse(0, 0, 0);
              color specular(0, 0, 0);
              color ambient(0, 0, 0);
              double intensity_at_point;

              for (std::size_t i = 0; i < scene_point_lights.size(); ++i) {
                calculate_illumination_and_shadows(
                    r, hit, hit_color, albedo, ambient, diffuse, specular,
                    scene_point_lights[i].position, scene_point_lights[i].light_color,
                    scene_point_lights[i].intensity, intensity_at_point, 1, 1);
              }

              for (std::size_t i = 0; i < scene_area_lights.size(); ++i) {
                vec3 random_sample_area_light =
                    scene_area_lights[i].getRandomSample();
                calculate_illumination_and_shadows(
                    r, hit, hit_color, albedo, ambient, diffuse, specular,
                    random_sample_area_light, scene_area_lights[i].light_color,
                    scene_area_lights[i].intensity, intensity_at_point,
                    scene_area_lights.size() / number_of_area_lights, 2);
              }

              return hit_color + (ambient * apply_ambient);

              // uncomment for reflections (currently not working fully)
              // hit_color = hit_color + (ambient * apply_ambient);
              // vec3 R = reflect(unit_vector(r.direction()), hit.normal);
              // return hit_color += 0.8 * ray_color(ray(hit.p + hit.normal * 0.05, R), t_min, t_max, hit.u, hit.v, depth - 1);
            }
          }
      }

    if (color_set) {
      return background_color;
    } else if (texture_set) {
      return background_texture.get_texel(u, v);
    } else {
      return color(0.01, 0.01, 0.01);
    }
  }

  bool object_hit(const ray &r, double t_min, double t_max, hit_record &hit) {
    hit_record temp_hit;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (std::size_t j = 0; j < scene_meshes.size(); ++j) {
      if (with_aabb) {
        if (scene_meshes[j].mesh_aabb.intersect(r, t_min, t_max)) {
          // ray_mesh_tests = ray_mesh_tests + 1;
          for (std::size_t k = 0; k < scene_meshes[j].mesh_triangles.size();
               ++k) {
            ray_triangle_tests = ray_triangle_tests + 1;
            if (scene_meshes[j].mesh_triangles[k].hit(r, t_min, closest_so_far,
                                                      temp_hit)) {

              hit_anything = true;
              if (r.shadow_ray)
                break;
              closest_so_far = temp_hit.t;
              hit = temp_hit;
            }
          }
        }
      } else {
        // ray_mesh_tests = ray_mesh_tests + 1;
        for (std::size_t k = 0; k < scene_meshes[j].mesh_triangles.size();
             ++k) {
          ray_triangle_tests = ray_triangle_tests + 1;
          if (scene_meshes[j].mesh_triangles[k].hit(r, t_min, closest_so_far,
                                                    temp_hit)) {
            hit_anything = true;
            if (r.shadow_ray)
              break;
            closest_so_far = temp_hit.t;
            hit = temp_hit;
          }
        }
      }
    }

    for (std::size_t j = 0; j < scene_triangles.size(); ++j) {
      if (scene_triangles[j].hit(r, t_min, closest_so_far, temp_hit)) {
        ray_triangle_tests = ray_triangle_tests + 1;
        hit_anything = true;
        if (r.shadow_ray)
          continue;
        closest_so_far = temp_hit.t;
        hit = temp_hit;
      }
    }

    for (std::size_t j = 0; j < scene_spheres.size(); ++j) {
      if (with_aabb) {
        if (scene_spheres[j].sphere_aabb.intersect(r, t_min, t_max)) {
          ray_sphere_tests = ray_sphere_tests + 1;
          if (scene_spheres[j].hit(r, t_min, closest_so_far, temp_hit)) {
            hit_anything = true;
            if (r.shadow_ray)
              continue;
            closest_so_far = temp_hit.t;
            hit = temp_hit;
          }
        }
      } else {
        ray_sphere_tests = ray_sphere_tests + 1;
        if (scene_spheres[j].hit(r, t_min, closest_so_far, temp_hit)) {
          hit_anything = true;
          if (r.shadow_ray)
            continue;
          closest_so_far = temp_hit.t;
          hit = temp_hit;
        }
      }
    }

    return hit_anything;
  }

  void get_object_min_max_cell(triangle const t, vec3 const cell_dimension, int &cell_min_z, int &cell_max_z, 
                               int &cell_min_y, int &cell_max_y, int &cell_min_x, int &cell_max_x, bool is_triangle) {
      vec3 triangle_p0 = t.p0;
      vec3 triangle_p1 = t.p1;
      vec3 triangle_p2 = t.p2;

      vec3 first_min = get_minimum_vector_for_aabb(triangle_p0, triangle_p1);
      vec3 minimum = get_minimum_vector_for_aabb(first_min, triangle_p2);

      vec3 first_max = get_maximum_vector_for_aabb(triangle_p0, triangle_p1);
      vec3 maximum = get_maximum_vector_for_aabb(first_max, triangle_p2);

      double min_cell_x = (minimum.x() - scene_aabb.min_.x()) / cell_dimension.x();
      double min_cell_y = (minimum.y() - scene_aabb.min_.y()) / cell_dimension.y();
      double min_cell_z = (minimum.z() - scene_aabb.min_.z()) / cell_dimension.z();

      double max_cell_x = (maximum.x() - scene_aabb.min_.x()) / cell_dimension.x();
      double max_cell_y = (maximum.y() - scene_aabb.min_.y()) / cell_dimension.y();
      double max_cell_z = (maximum.z() - scene_aabb.min_.z()) / cell_dimension.z();

      aabb cell_aabb(vec3(min_cell_x, min_cell_y, min_cell_z), vec3(max_cell_x, max_cell_y, max_cell_z));

      int extra_voxel = 1;

      if (is_triangle) {
        extra_voxel = 2;
      }

      cell_min_z = clamp(std::floor(cell_aabb.min_.z()), 0, grid_resolution_z - extra_voxel);
      cell_max_z = clamp(std::floor(cell_aabb.max_.z()), 0, grid_resolution_z - extra_voxel);
      cell_min_y = clamp(std::floor(cell_aabb.min_.y()), 0, grid_resolution_y - extra_voxel);
      cell_max_y = clamp(std::floor(cell_aabb.max_.y()), 0, grid_resolution_y - extra_voxel);
      cell_min_x = clamp(std::floor(cell_aabb.min_.x()), 0, grid_resolution_x - extra_voxel);
      cell_max_x = clamp(std::floor(cell_aabb.max_.x()), 0, grid_resolution_x - extra_voxel);
  }

  void create_scene_bounding_box() {

    vec3 triangles_first_min, triangles_first_max, triangles_second_min, triangles_second_max, triangles_third_min, triangles_third_max;
    vec3 spheres_min, spheres_max;
    vec3 meshes_min, meshes_max;

    vec3 min, max;

    for (std::size_t j = 0; j < scene_triangles.size(); ++j) {
      if (j == 0) {
        triangles_first_min = get_minimum_vector_for_aabb(scene_triangles[j].p0, scene_triangles[j].p1);
        triangles_first_max = get_maximum_vector_for_aabb(scene_triangles[j].p0, scene_triangles[j].p1);
        triangles_second_min = get_minimum_vector_for_aabb(triangles_first_min, scene_triangles[j].p2);
        triangles_second_max = get_maximum_vector_for_aabb(triangles_first_max, scene_triangles[j].p2);
      }
      else {
        triangles_first_min = get_minimum_vector_for_aabb(triangles_second_min, scene_triangles[j].p0);
        triangles_second_min = get_minimum_vector_for_aabb(triangles_first_min, scene_triangles[j].p1);
        triangles_third_min = get_minimum_vector_for_aabb(triangles_second_min, scene_triangles[j].p2);

        triangles_first_max = get_maximum_vector_for_aabb(triangles_second_max, scene_triangles[j].p0);
        triangles_second_max = get_maximum_vector_for_aabb(triangles_first_max, scene_triangles[j].p1);
        triangles_third_max = get_maximum_vector_for_aabb(triangles_second_max, scene_triangles[j].p2);
      }
    }

    if (scene_triangles.size() == 1) {
      triangles_third_min = triangles_second_min;
      triangles_third_max = triangles_second_max;
    }

    for (std::size_t j = 0; j < scene_spheres.size(); ++j) {
      if (j == 0) {
        spheres_min = scene_spheres[j].sphere_aabb.min_;
        spheres_max = scene_spheres[j].sphere_aabb.max_;
      }
      else {
        spheres_min = get_minimum_vector_for_aabb(spheres_min, scene_spheres[j].sphere_aabb.min_);
        spheres_max = get_maximum_vector_for_aabb(spheres_max, scene_spheres[j].sphere_aabb.max_);
      }
    }

    for (std::size_t i = 0; i < scene_meshes.size(); ++i) {
      if (i == 0) {
        meshes_min = scene_meshes[i].mesh_aabb.min_;
        meshes_max = scene_meshes[i].mesh_aabb.max_;
      }
      else {
        meshes_min = get_minimum_vector_for_aabb(meshes_min, scene_meshes[i].mesh_aabb.min_);
        meshes_max = get_maximum_vector_for_aabb(meshes_max, scene_meshes[i].mesh_aabb.max_);
      }
    }

    min = get_minimum_vector_for_aabb(triangles_third_min, meshes_min);
    min = get_minimum_vector_for_aabb(min, spheres_min);
    max = get_maximum_vector_for_aabb(triangles_third_max, meshes_max);
    max = get_maximum_vector_for_aabb(max, spheres_max);

    std::cout << std::endl << "Scene bounding box minimum: " << min << std::endl;
    std::cout << "Scene bounding box maximum: " << max << std::endl << std::endl;

    scene_aabb = aabb(vec3(min.x() - 2, min.y() - 2, min.z() - 2), vec3(max.x() + 2, max.y() + 2, max.z() + 2));
  }

  void compute_grid_resolution_and_C() {
    double size_x_of_scene_bbox, size_y_of_scene_bbox, size_z_of_scene_bbox;

    size_x_of_scene_bbox = scene_aabb.max_.x() - scene_aabb.min_.x();
    size_y_of_scene_bbox = scene_aabb.max_.y() - scene_aabb.min_.y();
    size_z_of_scene_bbox = scene_aabb.max_.z() - scene_aabb.min_.z();

    std::cout << "size_x : " << size_x_of_scene_bbox << std::endl;
    std::cout << "size_y : " << size_y_of_scene_bbox << std::endl;
    std::cout << "size_z : " << size_z_of_scene_bbox << std::endl;

    double volume_of_scene_bbox;

    volume_of_scene_bbox = size_x_of_scene_bbox * size_y_of_scene_bbox * size_z_of_scene_bbox;

    std::cout << "volume of scene bounding box : " << volume_of_scene_bbox << " units" << std::endl;

    double grid_density = 4;

    std::cout << "number of objects : " << number_of_objects << std::endl;

    double sigma_by_objects_divide_by_volume =  (grid_density * number_of_objects) / volume_of_scene_bbox;

    std::cout << "sigma * N / volume : " << sigma_by_objects_divide_by_volume << std::endl;

    double cube_root_of_sigma_N_V = std::cbrt(sigma_by_objects_divide_by_volume);

    std::cout << "cube root of above value : " << cube_root_of_sigma_N_V << std::endl << std::endl;

    grid_resolution_x = std::round(size_x_of_scene_bbox * cube_root_of_sigma_N_V);
    grid_resolution_y = std::round(size_y_of_scene_bbox * cube_root_of_sigma_N_V);
    grid_resolution_z = std::round(size_z_of_scene_bbox * cube_root_of_sigma_N_V);

    std::cout << "grid_resolution_x : " << grid_resolution_x << std::endl;
    std::cout << "grid_resolution_y : " << grid_resolution_y << std::endl;
    std::cout << "grid_resolution_z : " << grid_resolution_z << std::endl << std::endl;

    // std::cout << "3D Grid : " << std::endl << std::endl;

    vec3 size_of_scene_bbox = scene_aabb.max_ - scene_aabb.min_;
    cell_dimension_x = size_of_scene_bbox.x() / grid_resolution_x;
    cell_dimension_y = size_of_scene_bbox.y() / grid_resolution_y;
    cell_dimension_z = size_of_scene_bbox.z() / grid_resolution_z;

    vec3 cell_dimension(cell_dimension_x, cell_dimension_y, cell_dimension_z);

    std::cout << "Cell dimension: " << cell_dimension << std::endl;

    for (int z = 0; z < grid_resolution_z; z++) {
      for (int y = 0; y < grid_resolution_y; y++) {
        // std::cout << "           ";
        for (int x = 0; x < grid_resolution_x; x++) {
          // std::cout << uniform_grids_C[z][y][x] << " ";
          uniform_grids_C_linearized.push_back(0);
        }

        for (int x = 0; x < grid_resolution_x; x++) {
          
        }
        // std::cout << std::endl;
      }
      // std::cout << std::endl;
    }

    std::cout << "Size of linearized array: " << uniform_grids_C_linearized.size() << std::endl;

    for (std::size_t j = 0; j < scene_triangles.size(); ++j) {

      int cell_min_z, cell_max_z, cell_min_y, cell_max_y, cell_min_x, cell_max_x;

      get_object_min_max_cell(scene_triangles[j], cell_dimension, cell_min_z, cell_max_z,
                              cell_min_y, cell_max_y, cell_min_x, cell_max_x, true);

      for (int z = cell_min_z; z <= cell_max_z + 1; ++z) {
        for (int y = cell_min_y; y <= cell_max_y + 1; ++y) {
          for (int x = cell_min_x; x <= cell_max_x + 1; ++x) {
            int ith_index = (((grid_resolution_y * z) + y) * grid_resolution_x) + x;
            uniform_grids_C_linearized[ith_index] = uniform_grids_C_linearized[ith_index] + 1;
          }
        }
      }
    }

    for (std::size_t j = 0; j < scene_meshes.size(); ++j) {

      for (std::size_t k = 0; k < scene_meshes[j].mesh_triangles.size(); ++k) {

          int cell_min_z, cell_max_z, cell_min_y, cell_max_y, cell_min_x, cell_max_x;

          get_object_min_max_cell(scene_meshes[j].mesh_triangles[k], cell_dimension, cell_min_z, cell_max_z,
                                  cell_min_y, cell_max_y, cell_min_x, cell_max_x, false);

          for (int z = cell_min_z; z <= cell_max_z; ++z) {
            for (int y = cell_min_y; y <= cell_max_y; ++y) {
              for (int x = cell_min_x; x <= cell_max_x; ++x) {
                int ith_index = (((grid_resolution_y * z) + y) * grid_resolution_x) + x;
                uniform_grids_C_linearized[ith_index] = uniform_grids_C_linearized[ith_index] + 1;
              }
            }
          }
      }
    }

    // std::cout << std::endl;
    // for (int i = 0; i < uniform_grids_C_linearized.size(); ++i) {
    //   std::cout << uniform_grids_C_linearized[i] << " ";
    // }

    for (int i = 0; i <= uniform_grids_C_linearized.size(); ++i) {
      uniform_grids_C_linearized[i] += uniform_grids_C_linearized[i - 1];
    }

    // std::cout << std::endl;
    // for (int i = 0; i < uniform_grids_C_linearized.size(); ++i) {
    //   std::cout << uniform_grids_C_linearized[i] << " ";
    // }
    // std::cout << std::endl;

    std::cout << std::endl << "Joint size of the object list : " << uniform_grids_C_linearized[uniform_grids_C_linearized.size() - 1] << std::endl;

    for (int i = 0; i < uniform_grids_C_linearized[uniform_grids_C_linearized.size() - 1]; ++i) {
      object_reference object;
      object.reference = NULL;
      object.index = 0;
      uniform_grids_L.push_back(object);
    }

    long long int number_of_objects_counter = number_of_objects;

    uniform_grids_C_linearized.push_back(uniform_grids_C_linearized[uniform_grids_C_linearized.size() - 1]);

    for (int j = scene_meshes.size() - 1; j >= 0; --j) {
      for (int k = scene_meshes[j].mesh_triangles.size() - 1; k >= 0; --k) {

        int cell_min_z, cell_max_z, cell_min_y, cell_max_y, cell_min_x, cell_max_x;

        get_object_min_max_cell(scene_meshes[j].mesh_triangles[k], cell_dimension, cell_min_z, cell_max_z,
                                  cell_min_y, cell_max_y, cell_min_x, cell_max_x, false);

        number_of_objects_counter = number_of_objects_counter - 1;

        for (int z = cell_min_z; z <= cell_max_z; ++z) {
          for (int y = cell_min_y; y <= cell_max_y; ++y) {
            for (int x = cell_min_x; x <= cell_max_x; ++x) {
                int each_cell = (((grid_resolution_y * z) + y) * grid_resolution_x) + x;
                object_reference object;
                object.index = number_of_objects_counter;
                object.reference = &scene_meshes[j].mesh_triangles[k];
                uniform_grids_L[--uniform_grids_C_linearized[each_cell]] = object;
            }
          }
        }
      }
    }

    for (int j = scene_triangles.size() - 1; j >= 0; --j) {

      int cell_min_z, cell_max_z, cell_min_y, cell_max_y, cell_min_x, cell_max_x;

      get_object_min_max_cell(scene_triangles[j], cell_dimension, cell_min_z, cell_max_z,
                                  cell_min_y, cell_max_y, cell_min_x, cell_max_x, true);

      number_of_objects_counter = number_of_objects_counter - 1;

      for (int z = cell_min_z; z <= cell_max_z + 1; ++z) {
        for (int y = cell_min_y; y <= cell_max_y + 1; ++y) {
          for (int x = cell_min_x; x <= cell_max_x + 1; ++x) {
            int each_cell = (((grid_resolution_y * z) + y) * grid_resolution_x) + x;
            object_reference object;
            object.index = number_of_objects_counter;
            object.reference = &scene_triangles[j];
            uniform_grids_L[--uniform_grids_C_linearized[each_cell]] = object;
          }
        }
      }
    }
    

    // std::cout <<  std::endl << "C : " << std::endl;

    // for (int i = 0; i < uniform_grids_C_linearized.size(); ++i) {
    //   std::cout << uniform_grids_C_linearized[i] << " ";
    // }
    // std::cout << std::endl;

    C_size = uniform_grids_C_linearized.size();

    // std::cout << std::endl << "L : " << std::endl;
    // for (int i = 0; i < uniform_grids_L.size(); ++i) {
    //   std::cout << uniform_grids_L[i].index << " ";
    // }
    // std::cout << std::endl;

    // std::cout << uniform_grids_C_linearized[47] << std::endl;
    // std::cout << uniform_grids_L[uniform_grids_C_linearized[47]] << std::endl;

  }

  void construct_bvh() {


    for (std::size_t j = 0; j < scene_triangles.size(); ++j) {

      aabb resultant_aabb = scene_triangles[j].construct_aabb();
      scene_aabbs.push_back(resultant_aabb);
      scene_primitives.push_back(std::make_shared<triangle>(scene_triangles[j]));
    }

    for (std::size_t j = 0; j < scene_spheres.size(); ++j) {
      
      scene_aabbs.push_back(scene_spheres[j].sphere_aabb);
      scene_primitives.push_back(std::make_shared<sphere>(scene_spheres[j]));
    }

    for (std::size_t j = 0; j < scene_meshes.size(); ++j) {
      for (std::size_t k = 0; k < scene_meshes[j].mesh_triangles.size(); ++k) {

        aabb resultant_aabb = scene_meshes[j].mesh_triangles[k].construct_aabb();
        scene_aabbs.push_back(resultant_aabb);
        scene_primitives.push_back(std::make_shared<triangle>(scene_meshes[j].mesh_triangles[k]));
      }
    }

    // primitive *prim;
    // triangle t(point3(-3, -1, -6), point3(3, -1, -6),
    //                           point3(0, 1, -6), color(0.2, 0.2, 0.2));
    // prim = &t;
    // std::cout << std::endl << "Center of a triangle by pointer: " << prim->get_center() << std::endl;

    // Now we have all aabbs for all individual triangles, spheres and meshes in the scene

    std::cout << "Scene primitives: " << scene_primitives.size() << std::endl;

    scene_bvh.init(scene_primitives, 0, scene_primitives.size(), scene_aabb);

    if (BVH_RANDOM_SPLIT) {
      scene_bvh.build(scene_bvh.root, 0, scene_primitives.size());
    }

    else if (BVH_CENTROID_SPLIT) {
      scene_bvh.build_centroid_split(scene_bvh.root);
    }

    // std::cout << "Printing AABBS in BVH: " << std::endl;
    // scene_bvh.print_aabbs(scene_bvh.root, 0);

  }

  unsigned long long int get_ray_triangle_tests() { return ray_triangle_tests; }

  unsigned long long int get_ray_sphere_tests() { return ray_sphere_tests; }

  void calculate_illumination_and_shadows(
      const ray &r, hit_record &hit, color &hit_color, color &albedo,
      color &ambient, color &diffuse, color &specular, vec3 light_position,
      color light_color, double light_intensity, double intensity_at_point,
      double scale_intensity, double area_light_correction) {

    PhongIllumination(light_color, light_position, r, hit, albedo, diffuse,
                      specular, ambient, intensity_at_point);

    bool shadow_hit_or_not = true;

    if (USE_SHADOWS) {
      ray shadow_ray = ray(hit.p + hit.normal * SHADOW_BIAS,
                           unit_vector(light_position - hit.p));
      shadow_ray.shadow_ray = true;

      if (USE_UNIFORM_GRIDS) {
        if (traverse_uniform_grid(shadow_ray, 0.01, (light_position - hit.p).length(), hit)) {
          shadow_hit_or_not = false;
        }
      }

      else if (USE_BVH) {
        scene_bvh.shadow_ray_hit = false;
        if (scene_bvh.traverse_bvh(scene_bvh.root, shadow_ray, scene_tmin, scene_tmax, 0.01, (light_position - hit.p).length(), hit)) {
          shadow_hit_or_not = false;
          scene_bvh.shadow_ray_hit = false;
        }
      }

      else if (object_hit(shadow_ray, 0.01, (light_position - hit.p).length(), hit)) {
        shadow_hit_or_not = false;
      }
    }

    double actual_intensity = light_intensity / (4 * pi * intensity_at_point);
    actual_intensity = actual_intensity / scale_intensity;
    if (USE_SHADOWS) {
      hit_color +=
          shadow_hit_or_not *
          (albedo * (diffuse * apply_diffuse) + (specular * apply_specular)) *
          actual_intensity;
    } else {
      hit_color +=
          (albedo * (diffuse * apply_diffuse) + (specular * apply_specular)) *
          actual_intensity;
    }
  }

  void render_scene(int image_width, int image_height, int samples_per_pixel,
                    camera cam, std::ostream &file_to_save_image) {
    if (USE_UNIFORM_GRIDS) {
      auto t1 = high_resolution_clock::now();
      create_scene_bounding_box();
      compute_grid_resolution_and_C();
      auto t2 = high_resolution_clock::now();
      duration<double, std::milli> ms_double = t2 - t1;
      std::cout << "\nTime taken to construct uniform grid: "
              << (ms_double.count() / 1000) << " seconds\n";
    }

    if (USE_BVH) {
      auto t1 = high_resolution_clock::now();
      create_scene_bounding_box();
      construct_bvh();
      auto t2 = high_resolution_clock::now();
      duration<double, std::milli> ms_double = t2 - t1;
      std::cout << "\nTime taken to construct bvh: "
              << (ms_double.count() / 1000) << " seconds\n";
    }

    int depth = 1;
    for (int j = image_height - 1; j >= 0; --j) {
      double percent_complete = (image_height - j);
      percent_complete = percent_complete / image_height;
      std::cerr << "\rRendering complete: " << int(percent_complete * 100)
                << "% " << std::flush;

      for (int i = 0; i < image_width; ++i) {
        color pixel_color(0, 0, 0);

        for (int s = 0; s < samples_per_pixel; ++s) {
          auto u = (i + random_double()) / (image_width - 1);
          auto v = (j + random_double()) / (image_height - 1);
          ray r = cam.get_ray(u, v);
          pixel_color += ray_color(r, 0.001, infinity, (double)i / image_width,
                                   (double)j / image_height, depth);
        }

        write_color(file_to_save_image, pixel_color, samples_per_pixel);
      }
    }
  }

public:
  std::vector<triangle> scene_triangles;
  std::vector<sphere> scene_spheres;
  std::vector<mesh> scene_meshes;
  std::vector<PointLight> scene_point_lights;
  std::vector<AreaLight> scene_area_lights;
  long long int number_of_objects;
  int number_of_area_lights;
  bool apply_ambient;
  bool apply_diffuse;
  bool apply_specular;
  bool with_aabb;

  unsigned long long int ray_triangle_tests;
  unsigned long long int ray_sphere_tests;
  texture background_texture;
  color background_color;
  bool texture_set;
  bool color_set;
  double scene_tmin;
  double scene_tmax;

  std::vector<std::vector<std::vector<int>>> uniform_grids_C;
  std::vector<long long int> uniform_grids_C_linearized;
  int C_size = 0;
  std::vector<object_reference> uniform_grids_L;

  int grid_resolution_x, grid_resolution_y, grid_resolution_z;
  double cell_dimension_x, cell_dimension_y, cell_dimension_z;

  std::vector<color> pixel_color_vector;

  aabb scene_aabb;
  std::vector<aabb> scene_aabbs;
  std::vector<std::shared_ptr<primitive>> scene_primitives;
  BVH scene_bvh;
};

#endif