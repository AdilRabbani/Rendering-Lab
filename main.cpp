#include "constants.h"

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
    const int image_width = 200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    // const int image_height = 5;

    std::cout << "Image width: " << image_width << std::endl;
    std::cout << "Image height: " << image_height << std::endl;

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,1,0), 0.5));

    load_obj("models/teapot.obj", world, 0.3, vec3(-6, -1, 0));
    load_obj("models/teddy.obj", world, 0.04, vec3(30, -8, 0));
    load_obj("models/bunny.obj", world, 8, vec3(0, -0.15, 0));
    load_obj("models/suzanne.obj", world, 0.5, vec3(4, 2, 0));
    load_obj("models/oloid256_tri.obj", world, 0.45, vec3(-5, -3, 0));

    // File

    std::ofstream file_to_save_image;
    file_to_save_image.open("ray_cube.ppm");

    // Camera

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 2);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    std::cout << "viewport width: " << viewport_width << std::endl;
    std::cout << "viewport height: " << viewport_height << std::endl;
    std::cout << "horizontal: " << horizontal << std::endl;
    std::cout << "vertical: " << vertical << std::endl;
    std::cout << "lower left corner: " << lower_left_corner << std::endl;

    // Render

    file_to_save_image << "P3\n" << image_width << " " << image_height << "\n255\n";

    auto t1 = high_resolution_clock::now();
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color = ray_color(r, world);

            write_color(file_to_save_image, pixel_color);
        }
    }
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << "\nTime taken to render the image: " << ms_double.count() << "ms\n";

    std::cerr << "\nDone.\n";
}