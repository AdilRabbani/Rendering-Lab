#include "constants.h"
#include "color.h"
#include "obj_loader_helper.h"
#include "camera.h"
#include "sphere.h"
#include "transforms.h"
#include "vec3.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
using std::chrono::high_resolution_clock;
using std::chrono::duration;

color ray_color(const ray& r, const hittable& world, int depth) {
    
    hit_record rec;

    if (depth <= 0) return color(0, 0, 0);

    if (world.hit(r, 0.01, infinity, rec)) {
        // if (rec.u != -1) {
        //     return color(rec.u, rec.v, 1 - rec.u - rec.v);                         // comment out for u,v coordinates
        // }
        // else {
            point3 target = rec.p + rec.normal + random_unit_vector();
            // point3 target = rec.p + random_in_hemisphere(rec.normal);              // comment out for hemispherical reflectance
            return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
            // return 0.5 * (rec.normal + color(1, 1, 1));                // comment out for normals
        // }
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 500;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 200;
    const int max_depth = 2;

    for (int k = 0; k < 90; k++) {

        std::cout << "Image width: " << image_width << std::endl;
        std::cout << "Image height: " << image_height << std::endl;

        // World
        hittable_list world;
        // world.add(make_shared<sphere>(point3(0,-105,-1), 102));

        // point3 vertex_1(-1.0, -0.8, -1);
        // point3 vertex_2(0, 0.8, -1);
        // point3 vertex_3(1, -0.8, -1);


        // world.add(make_shared<triangle>(vertex_1, vertex_2, vertex_3));

        // load_obj("models/teapot.obj", world, 0.3, vec3(-6, -1, 0));
        // load_obj("models/teddy.obj", world, 0.04, vec3(30, -8, 0));
        // load_obj("models/bunny.obj", world, 8, vec3(0, -0.15, 0));
        // load_obj("models/suzanne.obj", world, 0.5, vec3(4, 2, 0));
        // load_obj("models/oloid256_tri.obj", world, 0.45, vec3(-5, -3, 0));

        // load_obj("models/bunny.obj", world, 8, vec3(0, 0, 0));
        // load_obj("models/suzanne.obj", world, 0.75, vec3(0, -0.5, -0.9));

        double angle_x = k * 1;
        double angle_y = 1;
        double angle_z = 1;

        load_obj("models/cube.obj", world, 0.75, vec3(-0.5, -0.5, 0), angle_x, angle_y, angle_z);
        // load_obj("models/suzanne.obj", world, 0.5, vec3(0, 0, 0), angle_x, angle_y, angle_z);
        // load_obj("models/oloid256_tri.obj", world, 0.75, vec3(0, -0.5, -0.9), angle_x, angle_y, angle_z);
        // load_obj("models/teddy.obj", world, 0.04, vec3(0, 0, 0), angle);

        // File

        std::ofstream file_to_save_image;
        file_to_save_image.open("animated/diffuse_pot/ray_obj_hemispherical_lambertian_"+ std::to_string(k)+".ppm");

        std::cout << k << std::endl;

        // Camera

        camera cam;

        // Render

        file_to_save_image << "P3\n" << image_width << " " << image_height << "\n255\n";

        auto t1 = high_resolution_clock::now();
        for (int j = image_height-1; j >= 0; --j) {
            std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0, 0, 0);
                for (int s = 0; s < samples_per_pixel; ++s) {
                    auto u = (i + random_double()) / (image_width-1);
                    auto v = (j + random_double()) / (image_height-1);
                    ray r = cam.get_ray(u, v);
                    pixel_color += ray_color(r, world, max_depth);
                }
                write_color(file_to_save_image, pixel_color, samples_per_pixel);
            }
        }
        auto t2 = high_resolution_clock::now();
        duration<double, std::milli> ms_double = t2 - t1;
        std::cout << "\nTime taken to render the image: " << ms_double.count() << "ms\n";

    }

    std::cerr << "\nDone.\n";
}