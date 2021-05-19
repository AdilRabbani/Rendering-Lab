#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>
#include <fstream>

// reference from Inigo Quilez's article on intersectors: https://www.iquilezles.org/www/articles/intersectors/intersectors.htm
double hit_triangle(const point3& p0, const point3& p1, const point3& p2, const ray& r, double &u, double &v) {

    vec3 p0p1 = p1 - p0;                      // edge 1
    vec3 p0p2 = p2 - p0;                      // edge 2

    vec3 rop0 = r.origin() - p0;              // vector (direction) from one of the points on triangle to ray origin
    vec3 n = cross(p0p1, p0p2);               // normal vector to the plane
    
    if (dot(n, r.direction()) < 0.0001) return -1;       // if the ray is parallel to our triangle it would never intersect

    vec3 q = cross(rop0, r.direction());      // vector (direction) to the intersection point
    
    double determinant = 1 / dot(r.direction(), n);     // determinant 1 / (N . r)
    double u_ = determinant * dot( -q, p0p2);           // compute u
    double v_ = determinant * dot( q, p0p1);            // compute v
    double t = determinant * dot( -n, rop0);            // compute t which is -(N . origin + D) / (N . r) because n is in the opposite direction of ray, we use -n

    if (u_ < 0.0 || u_ > 1.0 || v_ < 0.0 || (u_ + v_) > 1.0) return -1;
    
    u = u_;
    v = v_;
    return t;
}

color ray_color(const ray& r) {
    
    double u;
    double v;

    auto t = hit_triangle(point3(-1.0, -0.8, -1), point3(0.0, 0.8, -1), point3(1, -0.8, -1), r, u, v);

    if (t > 0.0) {
        vec3 N = unit_vector(r.at(t));
        return color(v, u, 1 - u - v);
        // return color(N.x() + 1, N.y() + 1, (1 - N.x() - N.y()) + 1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    // const int image_height = 5;

    std::cout << "Image width: " << image_width << std::endl;
    std::cout << "Image height: " << image_height << std::endl;

    // File

    std::ofstream file_to_save_image;
    file_to_save_image.open("ray_triangle_uv.ppm");

    // Camera

    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
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

    for (int j = image_height-1; j >= 0; --j) {
        // std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);
            ray r(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color = ray_color(r);

            write_color(file_to_save_image, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}