#ifndef SCENE_H
#define SCENE_H

#include "RTIOW/camera.h"
#include "RTIOW/color.h"
#include "constants.h"
#include "hittable.h"
#include "light.h"
#include "mesh.h"
#include "sphere.h"
#include "triangle.h"
#include <fstream>
#include <string>
#include <vector>

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
    ray_mesh_tests = 0;
    ray_triangle_tests = 0;
    ray_sphere_tests = 0;
    texture_set = 0;
    color_set = 0;
  }
  Scene(bool ambient_bool, bool diffuse_bool, bool specular_bool,
        bool with_aabb_bool) {
    apply_ambient = ambient_bool;
    apply_diffuse = diffuse_bool;
    apply_specular = specular_bool;
    with_aabb = with_aabb_bool;
    number_of_area_lights = 0;
    ray_mesh_tests = 0;
    ray_triangle_tests = 0;
    ray_sphere_tests = 0;
    texture_set = 0;
    color_set = 0;
  }
  Scene(bool ambient_bool, bool diffuse_bool, bool specular_bool,
        bool with_aabb_bool, color background_color_arg) {
    apply_ambient = ambient_bool;
    apply_diffuse = diffuse_bool;
    apply_specular = specular_bool;
    with_aabb = with_aabb_bool;
    number_of_area_lights = 0;
    ray_mesh_tests = 0;
    ray_triangle_tests = 0;
    ray_sphere_tests = 0;
    background_color = background_color_arg;
    texture_set = 0;
    color_set = 1;
  }
  Scene(bool ambient_bool, bool diffuse_bool, bool specular_bool,
        bool with_aabb_bool, texture background_texture_arg) {
    apply_ambient = ambient_bool;
    apply_diffuse = diffuse_bool;
    apply_specular = specular_bool;
    with_aabb = with_aabb_bool;
    number_of_area_lights = 0;
    ray_mesh_tests = 0;
    ray_triangle_tests = 0;
    ray_sphere_tests = 0;
    background_texture = background_texture_arg;
    texture_set = 1;
    color_set = 0;
  }
  void add_triangle(triangle t) { scene_triangles.push_back(t); }
  void add_sphere(sphere s) { scene_spheres.push_back(s); }
  void add_mesh(mesh m) { scene_meshes.push_back(m); }
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
                      vec3(a.position.x() + l * 0.1, a.position.y() + l * 0.1,
                           a.position.z() + m * 0.1),
                      a.radius));
      }
    }

    number_of_area_lights = number_of_area_lights + 1;
  }

  color ray_color(const ray &r, double t_min, double t_max, double u,
                  double v, int depth) {

    hit_record hit;

    color hit_color(0, 0, 0);

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

        // vec3 R = reflect(unit_vector(r.direction()), hit.normal);

        // if (hit.name == "triangle") {
        //   hit_color += 0.8 * ray_color(ray(hit.p + hit.normal * 0.05, R), t_min, t_max, u, v, depth - 1) + (ambient * apply_ambient);
        // }

        return hit_color + (ambient * apply_ambient);
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
        if (scene_meshes[j].mesh_aabb.intersect(r)) {
          ray_mesh_tests = ray_mesh_tests + 1;
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
        ray_mesh_tests = ray_mesh_tests + 1;
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
        hit_anything = true;
        if (r.shadow_ray)
          continue;
        closest_so_far = temp_hit.t;
        hit = temp_hit;
      }
    }

    for (std::size_t j = 0; j < scene_spheres.size(); ++j) {
      if (with_aabb) {
        if (scene_spheres[j].sphere_aabb.intersect(r)) {
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

  unsigned long long int get_ray_mesh_tests() { return ray_mesh_tests; }

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
      if (object_hit(shadow_ray, 0.01, (light_position - hit.p).length(),
                     hit)) {
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
  int number_of_area_lights;
  bool apply_ambient;
  bool apply_diffuse;
  bool apply_specular;
  bool with_aabb;
  unsigned long long int ray_mesh_tests;
  unsigned long long int ray_triangle_tests;
  unsigned long long int ray_sphere_tests;
  texture background_texture;
  color background_color;
  bool texture_set;
  bool color_set;
};

#endif