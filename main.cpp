#include "camera.h"
#include "color.h"
#include "constants.h"
#include "obj_loader_helper.h"
#include "sphere.h"
#include "vec3.h"

#include "light.h"

#include <chrono>
#include <cmath>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <math.h>
#include <memory>
#include <string>
#include <vector>
using std::chrono::duration;
using std::chrono::high_resolution_clock;

#include "scene.h"

int main() {

  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 1000;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 25;

  const int depth = 2;

  int from_ = 0;
  int to_ = 36;
  int image_counter = 1;

  // for (int l = 0; l < 2; l++) {

    // for (int k = from_; k < to_; k++) {

        std::cout << "Image width: " << image_width << std::endl;
        std::cout << "Image height: " << image_height << std::endl;
        
        color material_center(0.1, 0.46, 0.74);
        color material_ground(1, 1, 1);
        
        // texture earth("textures/2k_earth_daymap.ppm");
        // texture jupiter("textures/jupiter.ppm");
        // texture mars("textures/mars.ppm");
        // texture neptune("textures/neptune.ppm");
        // texture mercury("textures/mercury.ppm");
        // texture grass("textures/grass2.ppm");

        // sphere sphere_1(point3(0, 0, -1.0), 1.0, color(0.18, 0.3, 1), earth);

        Scene scene(1, 1, 1);
        // scene.add_sphere(sphere_1);
        // scene.add_sphere(sphere(point3(1.5, -0.4, 0), 0.6, color(0.8, 0.3, 0.8)));
        // scene.add_sphere(sphere(point3(-1.5, -0.4, 0), 0.6, color(0.8, 0.8, 0.3)));

        // scene.add_sphere(sphere(point3(0, -0.25, -0.5), 0.6, color(0.18, 0.3, 0.8)));
        // scene.add_sphere(sphere(point3(0, -0.25, -3.5), 0.6, color(0.18, 0.3, 0.8)));

        scene.add_triangle(triangle(point3(-5, -1, -6), point3(5, -1, -6), point3(-5, -1, 2.5), color(0.2, 0.2, 0.2)));
        scene.add_triangle(triangle(point3(5, -1, 2.5), point3(-5, -1, 2.5), point3(5, -1, -6), color(0.2, 0.2, 0.2)));

        // scene.add_triangle(triangle(point3(-5, -1, -6), point3(5, -1, -6), point3(-5, 5.5, -6), material_ground));
        // scene.add_triangle(triangle(point3(5, -1, -6), point3(5, 5.5, -6), point3(-5, 5.5, -6), material_ground));

        // scene.add_triangle(triangle(point3(-5, -1, -6), point3(-5, -1, 2.5), point3(-5, 5.5, -6), color(1, 0.1, 0.1)));
        // scene.add_triangle(triangle(point3(-5, -1, 2.5), point3(-5, 5.5, 2.5), point3(-5, 5.5, -6), color(1, 0.1, 0.1)));

        // scene.add_point_light(PointLight(color(0, 1, 1), 40, vec3(-3, 2.5, -1)));
        // scene.add_point_light(PointLight(color(1, 1, 0), 40, vec3(3, 2.5, -1)));
        // scene.add_point_light(PointLight(color(1, 0, 1), 40, vec3(0, 2.5, -1)));
        // scene.add_point_light(PointLight(color(1, 0.5, 0.2), 30, vec3(0, 2.5, 2)));
        // scene.add_point_light(PointLight(color(1, 1, 1), 30, vec3(0, -0.15, 0)));

        // scene.add_area_light(AreaLight(color(0, 1, 1), 40, vec3(-3, 2.5, -1), 1), 3);
        // scene.add_area_light(AreaLight(color(1, 1, 0), 40, vec3(3, 2.5, -1), 1), 3);
        // scene.add_area_light(AreaLight(color(1, 0, 1), 40, vec3(0, 2.5, -1), 1), 3);
        // scene.add_area_light(AreaLight(color(1, 1, 1), 30, vec3(-3, 2.5, -1), 1), 3);

        // double x_val = k;

        // if (l == 1) {
        //   x_val = x_val * -1;
        // }

        // scene.add_area_light(AreaLight(color(1, 1, 1), 30, vec3(x_val * 0.25, 2.5, 0), 1), 4);
        // scene.add_point_light(PointLight(color(1, 1, 1), 30, vec3(0, 2, -1)));
        // scene.add_point_light(PointLight(color(1, 1, 1), 30, vec3(-2, 2, -1)));
        scene.add_point_light(PointLight(color(1, 1, 1), 30, vec3(-2, 2, -1)));

        // scene.add_area_light(AreaLight(color(1, 1, 1), 40, vec3(-2, 2, -1), 1), 6);


        // scene.add_area_light(AreaLight(color(1, 0, 1), 30, vec3(0, 2.5, -1), 1), 3);
        // scene.add_area_light(AreaLight(color(1, 0.5, 0.2), 30, vec3(0, 2.5, 2), 1), 3);

        // scene.add_area_light(AreaLight(color(1, 1, 1), 1, vec3(0, 2.5, k*-1), 1), 3);
        // scene.add_area_light(AreaLight(color(1, 1, 1), 0.5, vec3(0, 2.5, k), 1), 2);

        // scene.add_point_light(PointLight(color(1, 1, 1), 1, vec3(0, 1, -2)));

        // load_obj("models/suzanne.obj", scene, 0.75, vec3(-0.5, -0.5, -2), (k + 20) * 0.01745, 0, 0, color(0.18, 0.3, 1));

        // load_obj("models/teapot.obj", scene, 0.5, vec3(-0.5, -0.5, -3), (k + 20) * 0.01745, 0, 0, color(0.18, 0.3, 1));

        // load_obj("models/bunny.obj", scene, 15, vec3(-0.5, 0.5, -1), (k + 20) * 0.01745, 0, 0, color(0.18, 0.3, 1));

        load_obj("models/cube.obj", scene, 1, vec3(1, 0, -1), (10) * 0.01745, 0, 0, color(0.18, 0.3, 1));

        // File

        std::ofstream file_to_save_image;
        file_to_save_image.open(
            "animated/phong/pointlight/rotation/" +
            std::to_string(image_counter) + "arealightrotatetranslate.ppm");

        // Camera

        // double rad = (k + 20) * 0.01745;

        // double v1_x = 0 * cos(rad) + 4 * -sin(rad);
        // double v1_z = 0 * sin(rad) + 4 * cos(rad);

        camera cam(point3(0, 2, 6), point3(0,0,-1), vec3(0,1,0), 40, aspect_ratio);

        // camera cam(point3(-3,3,1.5), point3(0,0,-1), vec3(0,1,0), 60, aspect_ratio);


        // using namespace pnm::literals;
        // pnm::image<pnm::rgb_pixel> ppm = pnm::read("textures/2k_earth_daymap.ppm");
        // pnm::image<pnm::rgb_pixel> ground = pnm::read("textures/grass.ppm");

        // std::cout << image.get_height() << std::endl;
        // std::cout << image.get_width() << std::endl;

        // Render

        file_to_save_image << "P3\n"
                          << image_width << " " << image_height << "\n255\n";

        auto t1 = high_resolution_clock::now();
        for (int j = image_height - 1; j >= 0; --j) {
          std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
          
          for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            
            for (int s = 0; s < samples_per_pixel; ++s) {
              auto u = (i + random_double()) / (image_width - 1);
              auto v = (j + random_double()) / (image_height - 1);
              ray r = cam.get_ray(u, v);
              // pixel_color += ray_color(r, world, lights, image);
              pixel_color += scene.ray_color(r, 0.001, infinity, depth, color(0, 0, 0));
            }

            write_color(file_to_save_image, pixel_color, samples_per_pixel);
          }

        }
        auto t2 = high_resolution_clock::now();
        duration<double, std::milli> ms_double = t2 - t1;
        std::cout << "\nTime taken to render the image: " << (ms_double.count() / 1000)
                  << "seconds\n";

        image_counter = image_counter + 1;
    // }
  // }

  std::cerr << "\nDone.\n";
}