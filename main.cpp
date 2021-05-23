#include "constants.h"
#include "color.h"
#include "obj_loader_helper.h"
#include "camera.h"

#include <iostream>
#include <fstream>
#include <chrono>
using std::chrono::high_resolution_clock;
using std::chrono::duration;

color ray_color(const ray& r, const hittable& world) {
    
    hit_record rec;

    if (world.hit(r, 0, infinity, rec)) {
        // if (rec.u != -1) {
        //     return color(rec.u, rec.v, 1 - rec.u - rec.v);
        // }
        // else {
            return 0.5 * (rec.normal + color(1, 1, 1));
        // }
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.2, 0.7, 1.0);
}

int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 50;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 50;
    // const int image_height = 5;

    std::cout << "Image width: " << image_width << std::endl;
    std::cout << "Image height: " << image_height << std::endl;

    // World
    hittable_list world;
    // world.add(make_shared<sphere>(point3(0,0,0), 0.5));

    // load_obj("models/teapot.obj", world, 0.3, vec3(-6, -1, 0));
    // load_obj("models/teddy.obj", world, 0.04, vec3(30, -8, 0));
    // load_obj("models/bunny.obj", world, 8, vec3(0, -0.15, 0));
    // load_obj("models/suzanne.obj", world, 0.5, vec3(4, 2, 0));
    // load_obj("models/oloid256_tri.obj", world, 0.45, vec3(-5, -3, 0));

    // load_obj("models/bunny.obj", world, 8, vec3(0, 0, 0));
    load_obj("models/suzanne.obj", world, 0.75, vec3(0, 0, 0));

    // File

    std::ofstream file_to_save_image;
    file_to_save_image.open("ray_cube_small.ppm");

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
                pixel_color += ray_color(r, world);
            }
            write_color(file_to_save_image, pixel_color, samples_per_pixel);
        }
    }
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << "\nTime taken to render the image: " << ms_double.count() << "ms\n";

    std::cerr << "\nDone.\n";
}