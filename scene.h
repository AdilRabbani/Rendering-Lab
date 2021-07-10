#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include "constants.h"
#include "hittable.h"
#include "triangle.h"
#include "sphere.h"
#include "light.h"
#include "vec3.h"
#include "aabb.h"

struct mesh {
    aabb mesh_aabb;
    std::vector<triangle> mesh_triangles;
};

inline void PhongIllumination(color light_color,
                       vec3 light_position,
                       const ray& r,
                       hit_record& hit,
                       color albedo,
                       color &diffuse,
                       color &specular,
                       color &ambient,
                       double &intensity_at_point) {

  vec3 light_dir = light_position - hit.p;

  intensity_at_point = sqrt(light_dir.length());

//   if (hit.name == "sphere") {
//       std::cout << intensity_at_point << std::endl;
//   }
  
  light_dir = unit_vector(light_dir);
  
  // for specular
  vec3 v = unit_vector(r.orig - hit.p);
  vec3 reflection = reflect(-light_dir, unit_vector(hit.normal));
  double spec = pow(std::max(dot(v, reflection), 0.0), 10);
  
  // specular
  specular = light_color * spec * 0.1;

  // diffuse
  diffuse = std::max(0.0, dot(hit.normal, light_dir)) * 1 / pi * light_color;

  // ambient
  ambient = ((color(albedo.e[0], albedo.e[1], albedo.e[2]))) * 0.05;
}

class Scene {
    public:
        Scene(bool ambient_bool, bool diffuse_bool, bool specular_bool){
            apply_ambient = ambient_bool;
            apply_diffuse = diffuse_bool;
            apply_specular = specular_bool;
            number_of_area_lights = 0;
            number_of_spheres = 0;
            use_spheres_aabb = false;
        }
        void add_triangle(triangle t) {
            scene_triangles.push_back(t);
        }
        void add_sphere(sphere s) {
            scene_spheres.push_back(s);
            number_of_spheres = number_of_spheres + 1;
            if (number_of_spheres > 3) {

                use_spheres_aabb = true;
            }
        }
        void add_mesh(mesh m) {
            scene_meshes.push_back(m);
        }
        void add_point_light(PointLight p) {
            scene_point_lights.push_back(p);
        }

        void add_area_light(AreaLight a, int no_of_samples) {
            
            for (int l = 0; l <= no_of_samples - 1; l++) {
                for (int m = 0; m <= no_of_samples - 1; m++) {
                    scene_area_lights.push_back(AreaLight(
                        a.light_color, 
                        a.intensity, 
                        vec3(
                            a.position.x() + l*0.05, 
                            a.position.y() + l*0.05, 
                            a.position.z() + m*0.05), 
                        a.radius));
                }
            }

            number_of_area_lights = number_of_area_lights + 1;
        }

        color ray_color(const ray &r, double t_min, double t_max, int depth, color albedo) {
            
            hit_record hit;

            color hit_color(0, 0, 0);

            if (depth <= 0) return color(0, 0, 0);

            if (object_hit(r, t_min, t_max, hit)) {

                color albedo;
                    
                albedo = hit.albedo;

                color diffuse(0, 0, 0);
                color specular(0, 0, 0);
                color ambient(0, 0, 0);
                double intensity_at_point;

                for(std::size_t i = 0; i < scene_point_lights.size(); ++i) {
                    calculate_illumination_and_shadows(
                        r,
                        hit, 
                        hit_color, 
                        albedo, 
                        ambient, 
                        diffuse, 
                        specular, 
                        scene_point_lights[i].position, 
                        scene_point_lights[i].light_color, 
                        scene_point_lights[i].intensity, 
                        intensity_at_point, 
                        1, 
                        1);
                }

                for(std::size_t i = 0; i < scene_area_lights.size(); ++i) {
                    vec3 random_sample_area_light = scene_area_lights[i].getRandomSample();
                    calculate_illumination_and_shadows(
                        r, 
                        hit, 
                        hit_color, 
                        albedo, 
                        ambient, 
                        diffuse, 
                        specular, 
                        random_sample_area_light, 
                        scene_area_lights[i].light_color, 
                        scene_area_lights[i].intensity, 
                        intensity_at_point, 
                        scene_area_lights.size() / number_of_area_lights, 
                        2);
                }

                return hit_color + (ambient * apply_ambient);
            }

            return color(0.01, 0.01, 0.01);
        }

        bool object_hit(const ray &r, double t_min, double t_max, hit_record &hit) {
            hit_record temp_hit;
            bool hit_anything = false;
            double closest_so_far = t_max;

            for(std::size_t j = 0; j < scene_meshes.size(); ++j) {
                if (scene_meshes[j].mesh_aabb.intersect(r)) {
                    for(std::size_t k = 0; k < scene_meshes[j].mesh_triangles.size(); ++k) {
                        if (scene_meshes[j].mesh_triangles[k].hit(r, t_min, closest_so_far, temp_hit)) {
                            hit_anything = true;
                            if (r.shadow_ray) continue;
                            closest_so_far = temp_hit.t;
                            hit = temp_hit;
                        }
                    }
                }
            }

            for(std::size_t j = 0; j < scene_triangles.size(); ++j) {
                if (scene_triangles[j].hit(r, t_min, closest_so_far, temp_hit)) {
                    hit_anything = true;
                    if (r.shadow_ray) continue;
                    closest_so_far = temp_hit.t;
                    hit = temp_hit;
                }
            }

            for(std::size_t j = 0; j < scene_spheres.size(); ++j) {
                if (use_spheres_aabb) {
                    if (scene_spheres[j].sphere_aabb.intersect(r)) {
                        if (scene_spheres[j].hit(r, t_min, closest_so_far, temp_hit)) {
                            hit_anything = true;
                            if (r.shadow_ray) continue;
                            closest_so_far = temp_hit.t;
                            hit = temp_hit;
                        }
                    }
                }
                else {
                    if (scene_spheres[j].hit(r, t_min, closest_so_far, temp_hit)) {
                            hit_anything = true;
                            if (r.shadow_ray) continue;
                            closest_so_far = temp_hit.t;
                            hit = temp_hit;
                    }
                }
            }

            return hit_anything;
        }

        void calculate_illumination_and_shadows(
            const ray &r,
            hit_record &hit,
            color &hit_color, 
            color &albedo, 
            color &ambient, 
            color &diffuse, 
            color &specular, 
            vec3 light_position, 
            color light_color,
            double light_intensity,
            double intensity_at_point,
            double scale_intensity,
            double area_light_correction) {

            PhongIllumination(light_color, light_position, r, hit, albedo, diffuse, specular, ambient, intensity_at_point);

            ray shadow_ray = ray(hit.p + hit.normal * 0.001, unit_vector(light_position - hit.p));
            shadow_ray.shadow_ray = true;
            bool hit_or_not = true;
            if (object_hit(shadow_ray, 0.01, (light_position - hit.p).length(), hit)) {
                hit_or_not = false;
            }

            double actual_intensity = light_intensity / (4 * pi * intensity_at_point);
            actual_intensity = actual_intensity / scale_intensity;
            hit_color += hit_or_not * (albedo * (diffuse * apply_diffuse) + (specular * apply_specular)) * actual_intensity;
        }

    public:
        std::vector<triangle> scene_triangles;
        std::vector<sphere> scene_spheres;
        std::vector<mesh> scene_meshes;
        std::vector<PointLight> scene_point_lights;
        std::vector<AreaLight> scene_area_lights;
        int number_of_area_lights;
        bool use_spheres_aabb;
        int number_of_spheres;
        bool apply_ambient;
        bool apply_diffuse;
        bool apply_specular;
};

#endif