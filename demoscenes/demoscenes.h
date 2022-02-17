#ifndef DEMOSCENES_H
#define DEMOSCENES_H

#include <chrono>
using std::chrono::duration;
using std::chrono::high_resolution_clock;

#include "../scene.h"

inline void scene0() {
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 1000;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 5;

  Scene scene(1, 1, 1, 1);

  // scene.add_triangle(triangle(point3(-5, -1, -6), point3(5, -1, -6),
  //                             point3(-5, -1, 2.5), color(0.2, 0.2, 0.2)));
  // scene.add_triangle(triangle(point3(5, -1, 2.5), point3(-5, -1, 2.5),
  //                             point3(5, -1, -6), color(0.2, 0.2, 0.2)));
  
  scene.add_triangle(triangle(point3(-1, -0.5, 0), point3(1, -0.5, -0.5), point3(0, 0.5, -0.5), color(0.2, 0.2, 0.2)));
  // scene.add_triangle(triangle(point3(-2, -0.5, -1), point3(2, -0.5, -1), point3(0, 0.5, -1), color(0.2, 0.3, 0.2)));
  // triangle tri1(point3(-2, -0.5, -1), point3(2, -0.5, -1), point3(0, 0.5, -1), color(0.2, 0.3, 0.2));
  // tri1.transform(vec3(-1, 1, 0), vec3(0.5, 0.5, 0.5), vec3(0, 0, 45));

  // scene.add_triangle(tri1);

  scene.add_point_light(PointLight(color(0, 1, 1), 30, vec3(-2, 2, -1)));
  scene.add_point_light(PointLight(color(1, 1, 0), 30, vec3(2, 2, -1)));
  scene.add_point_light(PointLight(color(1, 1, 1), 10, vec3(0, 2, -1)));

  std::ofstream file_to_save_image;
  file_to_save_image.open("renders/scene0_uniform_grids.ppm");

  camera cam(point3(0, 2, 6), point3(0, 0, -1), vec3(0, 1, 0), 45,
             aspect_ratio);

  file_to_save_image << "P3\n"
                     << image_width << " " << image_height << "\n255\n";

  auto t1 = high_resolution_clock::now();
  scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                     file_to_save_image);
  auto t2 = high_resolution_clock::now();
  duration<double, std::milli> ms_double = t2 - t1;
  std::cout << "\nTime taken to render the image: "
            << (ms_double.count() / 1000) << " seconds\n";
  // std::cout << "Ray-mesh intersection tests: " << scene.get_ray_mesh_tests()
  //           << std::endl;
  std::cout << "Ray-triangle intersection tests: "
            << scene.get_ray_triangle_tests() << std::endl;
  std::cout << "Ray-sphere intersection tests: " << scene.get_ray_sphere_tests()
            << std::endl << std::endl;

  scene.clear_scene();
}

inline void scene1() {

  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 800;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 1;

  Scene scene(1, 1, 1, 1);

  scene.add_triangle(triangle(point3(-6, -1, -6 + 0.0001), point3(6, -1, -6),
                              point3(6, 5, -6), color(0.2, 0.2, 0.2)));

  scene.add_triangle(triangle(point3(-6, -1, -6 + 0.0001), point3(6, 5, -6),
                              point3(-6, 5, -6), color(0.2, 0.2, 0.2)));

  scene.add_triangle(triangle(point3(-6, -1 + 0.0001, -6), point3(6, -1, -6),
                              point3(-6, -1, 2.5), color(0.2, 0.2, 0.2)));

  scene.add_triangle(triangle(point3(6, -1 + 0.0001, 2.5), point3(-6, -1, 2.5),
                              point3(6, -1, -6), color(0.2, 0.2, 0.2)));

  scene.add_point_light(PointLight(color(0, 1, 1), 30, vec3(-2, 2, -1)));
  scene.add_point_light(PointLight(color(1, 1, 0), 30, vec3(2, 2, -1)));
  scene.add_point_light(PointLight(color(1, 1, 1), 50, vec3(0, 2, 5)));

  // scene.add_point_light(PointLight(color(1, 1, 1), 70, vec3(0, 2, 5)));

  mesh cube("models/cube.obj", vec3(1, 1, 1), vec3(0, 0, -1), vec3(0, 45, 25),
            color(0.18, 0.3, 1));
  mesh cube2("models/cube.obj", vec3(1, 1, 1), vec3(2, 0, -1), vec3(0, 45, 25),
            color(0.18, 0.3, 1));
  mesh cube3("models/cube.obj", vec3(1, 1, 1), vec3(-2, 0, -1), vec3(0, 45, 25),
            color(0.18, 0.3, 1));
  scene.add_mesh(cube);
  scene.add_mesh(cube2);
  scene.add_mesh(cube3);

  std::ofstream file_to_save_image;
  file_to_save_image.open("renders/analysis/bvh/sah/scene_1.ppm");

  camera cam(point3(0, 2, 7.5), point3(0, 0, -1), vec3(0, 1, 0), 45,
             aspect_ratio);

  file_to_save_image << "P3\n"
                     << image_width << " " << image_height << "\n255\n";

  auto t1 = high_resolution_clock::now();
  scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                     file_to_save_image);
  auto t2 = high_resolution_clock::now();
  duration<double, std::milli> ms_double = t2 - t1;
  std::cout << "\nTime taken to render the image: "
            << (ms_double.count() / 1000) << " seconds\n";
  std::cout << "Ray-triangle intersection tests: "
            << scene.get_ray_triangle_tests() << std::endl;
  std::cout << "Ray-sphere intersection tests: " << scene.get_ray_sphere_tests()
            << std::endl << std::endl;

  scene.clear_scene();
}

inline void scene2() {
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 800;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 2;

  Scene scene(1, 1, 1, 0);

  scene.add_triangle(triangle(point3(-5, -1, -6), point3(5, -1, -6),
                              point3(-5, -1, 2.5), color(0.2, 0.2, 0.2)));
  scene.add_triangle(triangle(point3(5, -1, 2.5), point3(-5, -1, 2.5),
                              point3(5, -1, -6), color(0.2, 0.2, 0.2)));

  scene.add_triangle(triangle(point3(-5, -1, -6), point3(5, -1, -6),
                              point3(5, 5, -6), color(0.2, 0.2, 0.2)));

  scene.add_triangle(triangle(point3(-5, -1, -6), point3(5, 5, -6),
                              point3(-5, 5, -6), color(0.2, 0.2, 0.2)));

  // scene.add_point_light(PointLight(color(0, 1, 1), 30, vec3(-2, 2, -1)));
  // scene.add_point_light(PointLight(color(1, 1, 0), 30, vec3(2, 2, -1)));
  // scene.add_point_light(PointLight(color(1, 1, 1), 10, vec3(0, 2, -1)));
  // scene.add_point_light(PointLight(color(0.6, 0.5, 0.4), 30, vec3(-2.5, 2, 0.5)));

  scene.add_point_light(PointLight(color(0, 1, 1), 30, vec3(-2, 2, -1)));
  scene.add_point_light(PointLight(color(1, 1, 0), 30, vec3(2, 2, -1)));
  scene.add_point_light(PointLight(color(1, 1, 1), 50, vec3(0, 2, 5)));

  mesh bunny("models/bunny.obj", vec3(15, 15, 15), vec3(-0.75, 0.5, -1),
             vec3(0, 0, 0), color(0.18, 0.3, 1));
  scene.add_mesh(bunny);

  std::ofstream file_to_save_image;
  file_to_save_image.open("renders/analysis/uniform_grids/scene2_without_uniform_grids.ppm");

  camera cam(point3(0, 2, 7.5), point3(0, 0, -1), vec3(0, 1, 0), 45,
             aspect_ratio);

  file_to_save_image << "P3\n"
                     << image_width << " " << image_height << "\n255\n";

  auto t1 = high_resolution_clock::now();
  scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                     file_to_save_image);
  auto t2 = high_resolution_clock::now();
  duration<double, std::milli> ms_double = t2 - t1;
  std::cout << "\nTime taken to render the image: "
            << (ms_double.count() / 1000) << " seconds\n";
  std::cout << "Ray-triangle intersection tests: "
            << scene.get_ray_triangle_tests() << std::endl;
  std::cout << "Ray-sphere intersection tests: " << scene.get_ray_sphere_tests()
            << std::endl << std::endl;

  scene.clear_scene();
}

inline void scene3() {
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 800;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 3;

  Scene scene(1, 1, 1, 0);

  scene.add_triangle(triangle(point3(-5, -1, -6), point3(5, -1, -6),
                              point3(-5, -1, 2.5), color(0.2, 0.2, 0.2)));
  scene.add_triangle(triangle(point3(5, -1, 2.5), point3(-5, -1, 2.5),
                              point3(5, -1, -6), color(0.2, 0.2, 0.2)));

  scene.add_triangle(triangle(point3(-5, -1, -6), point3(5, -1, -6),
                              point3(5, 5, -6), color(0.2, 0.2, 0.2)));

  scene.add_triangle(triangle(point3(-5, -1, -6), point3(5, 5, -6),
                              point3(-5, 5, -6), color(0.2, 0.2, 0.2)));

  // scene.add_point_light(PointLight(color(0, 1, 1), 30, vec3(-2, 2, -1)));
  // scene.add_point_light(PointLight(color(1, 1, 0), 30, vec3(2, 2, -1)));

  // scene.add_point_light(PointLight(color(0.6, 0.5, 0.4), 30, vec3(2.5, 2, 0.5)));
  // scene.add_point_light(PointLight(color(0.6, 0.5, 0.4), 30, vec3(-2.5, 2, 0.5)));

  scene.add_point_light(PointLight(color(0, 1, 1), 30, vec3(-2, 2, -1)));
  scene.add_point_light(PointLight(color(1, 1, 0), 30, vec3(2, 2, -1)));
  scene.add_point_light(PointLight(color(1, 1, 1), 50, vec3(0, 2, 5)));

  mesh teapot("models/teapot1.obj", vec3(0.6, 0.6, 0.6), vec3(-0.75, 0, -1.5),
              vec3(0, 0, 0), color(0.9, 0.2, 0.2));
  scene.add_mesh(teapot);

  std::ofstream file_to_save_image;
  file_to_save_image.open("renders/analysis/uniform_grids/scene3_without_uniform_grids.ppm");

  camera cam(point3(0, 2, 7.5), point3(0, 0, -1), vec3(0, 1, 0), 45,
             aspect_ratio);

  file_to_save_image << "P3\n"
                     << image_width << " " << image_height << "\n255\n";

  auto t1 = high_resolution_clock::now();
  scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                     file_to_save_image);
  auto t2 = high_resolution_clock::now();
  duration<double, std::milli> ms_double = t2 - t1;
  std::cout << "\nTime taken to render the image: "
            << (ms_double.count() / 1000) << " seconds\n";
  // std::cout << "Ray-mesh intersection tests: " << scene.get_ray_mesh_tests()
  //           << std::endl;
  std::cout << "Ray-triangle intersection tests: "
            << scene.get_ray_triangle_tests() << std::endl;
  std::cout << "Ray-sphere intersection tests: " << scene.get_ray_sphere_tests()
            << std::endl << std::endl;

  scene.clear_scene();
}

inline void scene4(bool area_light) {
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 1000;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 15;

  int image_counter = 0;

  for (int k = 0; k < 30; k++) {
    Scene scene(1, 1, 1, 1);

    scene.add_triangle(triangle(point3(-5, -1, -6), point3(5, -1, -6),
                                point3(-5, -1, 2.5), color(0.2, 0.2, 0.2)));
    scene.add_triangle(triangle(point3(5, -1, 2.5), point3(-5, -1, 2.5),
                                point3(5, -1, -6), color(0.2, 0.2, 0.2)));

    if (!area_light) {
      scene.add_point_light(PointLight(color(0, 1, 1), 30, vec3(-2, 2, -1)));
      scene.add_point_light(PointLight(color(1, 1, 0), 30, vec3(2, 2, -1)));
      scene.add_point_light(PointLight(color(1, 1, 1), 10, vec3(0, 2, -1)));
    }
    else {
      scene.add_area_light(AreaLight(color(0, 1, 1), 30, vec3(-2, 2, -1), 1),
                       3);
      scene.add_area_light(AreaLight(color(1, 1, 0), 30, vec3(2, 2, -1), 1),
                       3);
      scene.add_area_light(AreaLight(color(1, 1, 1), 10, vec3(0, 2, -1), 1),
                       2);
    }

    mesh cube("models/cube.obj", vec3(1, 1, 1), vec3(0, 0, -1),
              vec3(k * 3, k * 3, k * 3), color(0.18, 0.3, 1));
    scene.add_mesh(cube);

    std::ofstream file_to_save_image;
    file_to_save_image.open("renders/animated/rotation/area_light/scene4_" +
                            std::to_string(image_counter) + ".ppm");

    camera cam(point3(0, 2, 6), point3(0, 0, -1), vec3(0, 1, 0), 45,
               aspect_ratio);

    file_to_save_image << "P3\n"
                       << image_width << " " << image_height << "\n255\n";

    auto t1 = high_resolution_clock::now();
    scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                       file_to_save_image);
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << "\nTime taken to render the image: "
              << (ms_double.count() / 1000) << " seconds\n";
    // std::cout << "Ray-mesh intersection tests: " << scene.get_ray_mesh_tests()
    //           << std::endl;
    std::cout << "Ray-triangle intersection tests: "
              << scene.get_ray_triangle_tests() << std::endl;
    std::cout << "Ray-sphere intersection tests: "
              << scene.get_ray_sphere_tests() << std::endl << std::endl;
    image_counter = image_counter + 1;
    scene.clear_scene();
  }
}

inline void scene5() {
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 1000;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 15;

  Scene scene(1, 1, 1, 1);

  sphere sphere_1(point3(0, -0.2, -1.0), 0.8, color(0.18, 0.3, 1));
  sphere sphere_2(point3(1.5, -0.4, 0), 0.6, color(0.8, 0.3, 0.8));
  sphere sphere_3(point3(-1.5, -0.4, 0), 0.6, color(0.8, 0.8, 0.3));

  scene.add_sphere(sphere_1);
  scene.add_sphere(sphere_2);
  scene.add_sphere(sphere_3);

  scene.add_triangle(triangle(point3(-5, -1, -6), point3(5, -1, -6),
                              point3(-5, -1, 2.5), color(0.2, 0.2, 0.2)));
  scene.add_triangle(triangle(point3(5, -1, 2.5), point3(-5, -1, 2.5),
                              point3(5, -1, -6), color(0.2, 0.2, 0.2)));

  scene.add_area_light(AreaLight(color(1, 1, 0), 70, vec3(0, -0.2, -0.5), 1),
                       10);

  std::ofstream file_to_save_image;
  file_to_save_image.open("renders/scene5_reflections.ppm");

  camera cam(point3(0, 2, 6), point3(0, 0, -1), vec3(0, 1, 0), 45,
             aspect_ratio);

  file_to_save_image << "P3\n"
                     << image_width << " " << image_height << "\n255\n";

  auto t1 = high_resolution_clock::now();
  scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                     file_to_save_image);
  auto t2 = high_resolution_clock::now();
  duration<double, std::milli> ms_double = t2 - t1;
  std::cout << "\nTime taken to render the image: "
            << (ms_double.count() / 1000) << " seconds\n";
  // std::cout << "Ray-mesh intersection tests: " << scene.get_ray_mesh_tests()
  //           << std::endl;
  std::cout << "Ray-triangle intersection tests: "
            << scene.get_ray_triangle_tests() << std::endl;
  std::cout << "Ray-sphere intersection tests: " << scene.get_ray_sphere_tests()
            << std::endl << std::endl;

  scene.clear_scene();
}

inline void scene6() {
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 1000;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 15;

  Scene scene(1, 1, 1, 1);

  sphere sphere_1(point3(0, -0.2, -1.0), 0.8, color(0.18, 0.3, 1));
  sphere sphere_2(point3(1.5, -0.4, 0), 0.6, color(0.8, 0.3, 0.8));
  sphere sphere_3(point3(-1.5, -0.4, 0), 0.6, color(0.8, 0.8, 0.3));

  scene.add_sphere(sphere_1);
  scene.add_sphere(sphere_2);
  scene.add_sphere(sphere_3);

  scene.add_triangle(triangle(point3(-5, -1, -6), point3(5, -1, -6),
                              point3(-5, -1, 2.5), color(0.2, 0.2, 0.2)));
  scene.add_triangle(triangle(point3(5, -1, 2.5), point3(-5, -1, 2.5),
                              point3(5, -1, -6), color(0.2, 0.2, 0.2)));

  scene.add_area_light(AreaLight(color(0, 1, 1), 30, vec3(-2, 2, -1), 1), 3);
  scene.add_area_light(AreaLight(color(1, 1, 0), 30, vec3(2, 2, -1), 1), 3);
  scene.add_area_light(AreaLight(color(1, 1, 1), 10, vec3(0, 2, -1), 1), 3);

  std::ofstream file_to_save_image;
  file_to_save_image.open("renders/scene6.ppm");

  camera cam(point3(0, 2, 6), point3(0, 0, -1), vec3(0, 1, 0), 45,
             aspect_ratio);

  file_to_save_image << "P3\n"
                     << image_width << " " << image_height << "\n255\n";

  auto t1 = high_resolution_clock::now();
  scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                     file_to_save_image);
  auto t2 = high_resolution_clock::now();
  duration<double, std::milli> ms_double = t2 - t1;
  std::cout << "\nTime taken to render the image: "
            << (ms_double.count() / 1000) << " seconds\n";
  // std::cout << "Ray-mesh intersection tests: " << scene.get_ray_mesh_tests()
  //           << std::endl;
  std::cout << "Ray-triangle intersection tests: "
            << scene.get_ray_triangle_tests() << std::endl;
  std::cout << "Ray-sphere intersection tests: " << scene.get_ray_sphere_tests()
            << std::endl << std::endl;

  scene.clear_scene();
}

inline void scene7(bool area_light) {
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 2000;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 15;

  texture earth("textures/2k_earth_daymap.ppm");
  texture jupiter("textures/jupiter.ppm");
  texture mars("textures/mars.ppm");
  // texture grass("textures/grass.ppm");
  texture background("textures/stars.ppm");
  // texture milkyway("textures/milky_way.ppm");

  Scene scene(1, 1, 1, 1, background);

  sphere sphere_1(point3(0, -0.2, -1.0), 0.8, earth);
  sphere sphere_2(point3(1.5, -0.4, 0), 0.6, jupiter);
  sphere sphere_3(point3(-1.5, -0.4, 0), 0.6, mars);

  scene.add_sphere(sphere_1);
  scene.add_sphere(sphere_2);
  scene.add_sphere(sphere_3);

  scene.add_triangle(triangle(point3(-5, -1, -6), point3(5, -1, -6),
                              point3(-5, -1, 2.5), color(0.2, 0.2, 0.2)));
  scene.add_triangle(triangle(point3(5, -1, 2.5), point3(-5, -1, 2.5),
                              point3(5, -1, -6), color(0.2, 0.2, 0.2)));

  std::ofstream file_to_save_image;
  if (area_light) {
    scene.add_area_light(AreaLight(color(1, 1, 1), 30, vec3(-2, 2, -1), 1), 3);
    scene.add_area_light(AreaLight(color(1, 1, 1), 30, vec3(2, 2, -1), 1), 3);
    scene.add_area_light(AreaLight(color(1, 1, 1), 10, vec3(0, 2, -1), 1), 3);
    file_to_save_image.open("renders/scene7_arealight_uniform_grids.ppm");
  } else {
    scene.add_point_light(PointLight(color(1, 1, 1), 30, vec3(-2, 2, -1)));
    scene.add_point_light(PointLight(color(1, 1, 1), 30, vec3(2, 2, -1)));
    scene.add_point_light(PointLight(color(1, 1, 1), 10, vec3(0, 2, -1)));
    file_to_save_image.open("renders/scene7_pointlight_uniform_grids.ppm");
  }

  camera cam(point3(0, 2, 6), point3(0, 0, -1), vec3(0, 1, 0), 45,
             aspect_ratio);

  file_to_save_image << "P3\n"
                     << image_width << " " << image_height << "\n255\n";

  auto t1 = high_resolution_clock::now();
  scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                     file_to_save_image);
  auto t2 = high_resolution_clock::now();
  duration<double, std::milli> ms_double = t2 - t1;
  std::cout << "\nTime taken to render the image: "
            << (ms_double.count() / 1000) << " seconds\n";
  // std::cout << "Ray-mesh intersection tests: " << scene.get_ray_mesh_tests()
  //           << std::endl;
  std::cout << "Ray-triangle intersection tests: "
            << scene.get_ray_triangle_tests() << std::endl;
  std::cout << "Ray-sphere intersection tests: " << scene.get_ray_sphere_tests()
            << std::endl << std::endl;

  scene.clear_scene();
}

inline void scene8() {
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 1000;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 15;

  int image_counter = 0;

  for (int k = -15; k < 15; k++) {
    Scene scene(1, 1, 1, 1);

    sphere sphere_1(point3(0, -0.2, -1.0), 0.8, color(0.18, 0.3, 1));
    sphere sphere_2(point3(1.5, -0.4, 0), 0.6, color(0.8, 0.3, 0.8));
    sphere sphere_3(point3(-1.5, -0.4, 0), 0.6, color(0.8, 0.8, 0.3));

    scene.add_sphere(sphere_1);
    scene.add_sphere(sphere_2);
    scene.add_sphere(sphere_3);

    scene.add_triangle(triangle(point3(-5, -1, -6), point3(5, -1, -6),
                                point3(-5, -1, 2.5), color(0.2, 0.2, 0.2)));
    scene.add_triangle(triangle(point3(5, -1, 2.5), point3(-5, -1, 2.5),
                                point3(5, -1, -6), color(0.2, 0.2, 0.2)));

    scene.add_point_light(PointLight(color(0, 1, 1), 30, vec3(-2, 2, -1)));
    scene.add_point_light(PointLight(color(1, 1, 0), 30, vec3(2, 2, -1)));
    scene.add_point_light(PointLight(color(1, 1, 1), 10, vec3(0, 2, -1)));

    std::ofstream file_to_save_image;
    file_to_save_image.open("renders/animated/camera/scene8_" +
                            std::to_string(image_counter) + ".ppm");

    camera cam(point3(k * 0.25, 2, 6), point3(0, 0, -1), vec3(0, 1, 0), 45,
               aspect_ratio);

    file_to_save_image << "P3\n"
                       << image_width << " " << image_height << "\n255\n";

    auto t1 = high_resolution_clock::now();
    scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                       file_to_save_image);
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << "\nTime taken to render the image: "
              << (ms_double.count() / 1000) << " seconds\n";
    // std::cout << "Ray-mesh intersection tests: " << scene.get_ray_mesh_tests()
    //           << std::endl;
    std::cout << "Ray-triangle intersection tests: "
              << scene.get_ray_triangle_tests() << std::endl;
    std::cout << "Ray-sphere intersection tests: "
              << scene.get_ray_sphere_tests() << std::endl << std::endl;
    image_counter = image_counter + 1;
    scene.clear_scene();
  }
}

inline void scene9(bool area_light) {
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 1000;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 15;

  int image_counter = 0;

  for (int k = -15; k < 15; k++) {
    Scene scene(1, 1, 1, 1);

    sphere sphere_1(point3(0, -0.2, -1.0), 0.8, color(0.18, 0.3, 1));
    sphere sphere_2(point3(1.5, -0.4, 0), 0.6, color(0.8, 0.3, 0.8));
    sphere sphere_3(point3(-1.5, -0.4, 0), 0.6, color(0.8, 0.8, 0.3));

    scene.add_sphere(sphere_1);
    scene.add_sphere(sphere_2);
    scene.add_sphere(sphere_3);

    scene.add_triangle(triangle(point3(-5, -1, -6), point3(5, -1, -6),
                                point3(-5, -1, 2.5), color(0.2, 0.2, 0.2)));
    scene.add_triangle(triangle(point3(5, -1, 2.5), point3(-5, -1, 2.5),
                                point3(5, -1, -6), color(0.2, 0.2, 0.2)));

    std::ofstream file_to_save_image;

    if (area_light) {
      scene.add_area_light(
          AreaLight(color(1, 1, 1), 40, vec3(k * 0.25, 2, -1), 1), 3);
      file_to_save_image.open("renders/animated/lights/area/scene9_" +
                              std::to_string(image_counter) + ".ppm");
    } else {
      scene.add_point_light(
          PointLight(color(1, 1, 1), 40, vec3(k * 0.25, 2, -1)));
      file_to_save_image.open("renders/animated/lights/point/scene9_" +
                              std::to_string(image_counter) + ".ppm");
    }

    camera cam(point3(0, 2, 6), point3(0, 0, -1), vec3(0, 1, 0), 45,
               aspect_ratio);

    file_to_save_image << "P3\n"
                       << image_width << " " << image_height << "\n255\n";

    auto t1 = high_resolution_clock::now();
    scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                       file_to_save_image);
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << "\nTime taken to render the image: "
              << (ms_double.count() / 1000) << " seconds\n";
    // std::cout << "Ray-mesh intersection tests: " << scene.get_ray_mesh_tests()
    //           << std::endl;
    std::cout << "Ray-triangle intersection tests: "
              << scene.get_ray_triangle_tests() << std::endl;
    std::cout << "Ray-sphere intersection tests: "
              << scene.get_ray_sphere_tests() << std::endl << std::endl;
    image_counter = image_counter + 1;
    scene.clear_scene();
  }
}

inline void scene10() {
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 800;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 3;

  Scene scene(1, 1, 1, 0);

  scene.add_triangle(triangle(point3(-5, -1, -6), point3(5, -1, -6),
                              point3(-5, -1, 2.5), color(0.2, 0.2, 0.2)));
  scene.add_triangle(triangle(point3(5, -1, 2.5), point3(-5, -1, 2.5),
                              point3(5, -1, -6), color(0.2, 0.2, 0.2)));

  scene.add_triangle(triangle(point3(-5, -1, -6), point3(5, -1, -6),
                              point3(5, 5, -6), color(0.2, 0.2, 0.2)));

  scene.add_triangle(triangle(point3(-5, -1, -6), point3(5, 5, -6),
                              point3(-5, 5, -6), color(0.2, 0.2, 0.2)));

  // scene.add_area_light(AreaLight(color(0, 1, 1), 30, vec3(-2, 2, -1), 1), 3);
  // scene.add_area_light(AreaLight(color(1, 1, 0), 30, vec3(2, 2, -1), 1), 3);
  // scene.add_area_light(AreaLight(color(1, 1, 1), 10, vec3(0, 2, -1), 1), 3);
  // scene.add_area_light(AreaLight(color(1, 1, 1), 10, vec3(0, 0, 0.5), 1), 6);

  scene.add_point_light(PointLight(color(0, 1, 1), 30, vec3(-2, 2, -1)));
  scene.add_point_light(PointLight(color(1, 1, 0), 30, vec3(2, 2, -1)));
  scene.add_point_light(PointLight(color(1, 1, 1), 50, vec3(0, 2, 5)));

  mesh bunny("models/suzanne.obj", vec3(1.25, 1.25, 1.25), vec3(-0.75, -0.35, -1),
             vec3(0, 0, 0), color(0.18, 0.3, 1));
  scene.add_mesh(bunny);

  std::ofstream file_to_save_image;
  file_to_save_image.open("renders/analysis/uniform_grids/scene10_without_uniform_grids.ppm");

  camera cam(point3(0, 2, 7.5), point3(0, 0, -1), vec3(0, 1, 0), 45,
             aspect_ratio);

  file_to_save_image << "P3\n"
                     << image_width << " " << image_height << "\n255\n";

  auto t1 = high_resolution_clock::now();
  scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                     file_to_save_image);
  auto t2 = high_resolution_clock::now();
  duration<double, std::milli> ms_double = t2 - t1;
  std::cout << "\nTime taken to render the image: "
            << (ms_double.count() / 1000) << " seconds\n";
  std::cout << "Ray-triangle intersection tests: "
            << scene.get_ray_triangle_tests() << std::endl;
  std::cout << "Ray-sphere intersection tests: " << scene.get_ray_sphere_tests()
            << std::endl << std::endl;

  scene.clear_scene();
}

inline void scene11() {
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 1000;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 2;

  int image_counter = 0;

  for (int i = 0; i < 10; i++) {
    Scene scene(1, 1, 1, 0);

    scene.add_triangle(triangle(point3(-5, -1, -6), point3(5, -1, -6),
                                point3(-5, -1, 2.5), color(0.2, 0.2, 0.2)));
    scene.add_triangle(triangle(point3(5, -1, 2.5), point3(-5, -1, 2.5),
                                point3(5, -1, -6), color(0.2, 0.2, 0.2)));

    scene.add_triangle(triangle(point3(-5, -1, -6), point3(5, -1, -6),
                                point3(5, 5, -6), color(0.2, 0.2, 0.2)));

    scene.add_triangle(triangle(point3(-5, -1, -6), point3(5, 5, -6),
                                point3(-5, 5, -6), color(0.2, 0.2, 0.2)));

    // scene.add_point_light(PointLight(color(0, 1, 1), 30, vec3(-2, 2, -1)));
    // scene.add_point_light(PointLight(color(1, 1, 0), 30, vec3(2, 2, -1)));
    scene.add_point_light(PointLight(color(1, 1, 1), 50, vec3(0, 2, 7)));

    mesh teapot("models/teapot1.obj", vec3(0.6, 0.6, 0.6), vec3(-0.75, 0, -1.5),
                vec3(0, 0, 0), color(0.9, 0.2, 0.2));
    scene.add_mesh(teapot);

    std::ofstream file_to_save_image;
    file_to_save_image.open("renders/analysis/uniform_grids/animated/camera/scene20_" +
                              std::to_string(image_counter) + ".ppm");

    camera cam(point3(-5 + i, 2, 7.5), point3(0, 0, -1), vec3(0, 1, 0), 45,
              aspect_ratio);

    file_to_save_image << "P3\n"
                      << image_width << " " << image_height << "\n255\n";

    auto t1 = high_resolution_clock::now();
    scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                      file_to_save_image);
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << "\nTime taken to render the image: "
              << (ms_double.count() / 1000) << " seconds\n";
    // std::cout << "Ray-mesh intersection tests: " << scene.get_ray_mesh_tests()
    //           << std::endl;
    std::cout << "Ray-triangle intersection tests: "
              << scene.get_ray_triangle_tests() << std::endl;
    std::cout << "Ray-sphere intersection tests: " << scene.get_ray_sphere_tests()
              << std::endl << std::endl;

    scene.clear_scene();
    image_counter = image_counter + 1;
  }
}

inline void scene12() {
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 800;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 3;

  Scene scene(1, 1, 1, 1);

  scene.add_triangle(triangle(point3(-6, -1, -6 + 0.0001), point3(6, -1, -6),
                              point3(6, 5, -6), color(0.2, 0.2, 0.2)));

  scene.add_triangle(triangle(point3(-6, -1, -6 + 0.0001), point3(6, 5, -6),
                              point3(-6, 5, -6), color(0.2, 0.2, 0.2)));

  scene.add_triangle(triangle(point3(-6, -1 + 0.0001, -6), point3(6, -1, -6),
                              point3(-6, -1, 2.5), color(0.2, 0.2, 0.2)));

  scene.add_triangle(triangle(point3(6, -1 + 0.0001, 2.5), point3(-6, -1, 2.5),
                              point3(6, -1, -6), color(0.2, 0.2, 0.2)));

  // scene.add_point_light(PointLight(color(1, 1, 1), 70, vec3(0, 2, 5)));

  scene.add_point_light(PointLight(color(0, 1, 1), 30, vec3(-2, 2, -1)));
  scene.add_point_light(PointLight(color(1, 1, 0), 30, vec3(2, 2, -1)));
  scene.add_point_light(PointLight(color(1, 1, 1), 10, vec3(0, 2, -1)));
  // scene.add_point_light(PointLight(color(0.6, 0.5, 0.4), 30, vec3(-2.5, 2, 0.5)));
  // scene.add_point_light(PointLight(color(0.6, 0.5, 0.4), 30, vec3(2.5, 2, 0.5)));



  // scene.add_area_light(AreaLight(color(0, 1, 1), 30, vec3(-2, 2, -1), 1), 3);
  // scene.add_area_light(AreaLight(color(1, 1, 0), 30, vec3(2, 2, -1), 1), 3);
  // scene.add_area_light(AreaLight(color(1, 1, 1), 10, vec3(0, 2, -1), 1), 3);



  // scene.add_area_light(AreaLight(color(0.6, 0.5, 0.4), 30, vec3(-2.5, 2, 0.5), 1), 3);
  // scene.add_area_light(AreaLight(color(0.6, 0.5, 0.4), 30, vec3(2.5, 2, 0.5), 1), 3);

  // scene.add_area_light(AreaLight(color(1, 1, 1), 10, vec3(0, 2, 5), 1), 3);


  // scene.add_area_light(AreaLight(color(1, 1, 1), 60, vec3(0, 2, 5), 1), 10);

  // mesh teapot("models/teapot.obj", vec3(0.5, 0.5, 0.5), vec3(-0.75, 0, -1.2),
  //             vec3(0, 0, 0), color(0.9, 0.2, 0.2));
  // scene.add_mesh(teapot);

  mesh teapot("models/teapot.obj", vec3(0.02, 0.02, 0.02), vec3(-0.5, 1, -1),
              vec3(0, 0, 0), color(0.9, 0.2, 0.2));
  scene.add_mesh(teapot);

  mesh bunny("models/stanford-bunny.obj", vec3(15, 15, 15), vec3(-4, 0.5, 0),
             vec3(0, -30, 0), color(0.8, 0.8, 0.8));
  scene.add_mesh(bunny);

  mesh suzanne("models/suzanne.obj", vec3(1, 1, 1), vec3(2.5, -0.5, -0.25),
             vec3(0, 30, 0), color(0.18, 0.3, 1));
  scene.add_mesh(suzanne);

  mesh pine_tree_1("models/pine_tree.obj", vec3(0.0035, 0.0035, 0.0035), vec3(2, 0.5, -3),
              vec3(0, 0, 0), color(0, 0.9, 0.45));
  scene.add_mesh(pine_tree_1);

  mesh pine_tree_2("models/pine_tree.obj", vec3(0.0035, 0.0035, 0.0035), vec3(-3.5, 0.5, -3),
              vec3(0, 0, 0), color(0, 0.9, 0.45));
  scene.add_mesh(pine_tree_2);

  // mesh lucy("models/lucy.obj", vec3(0.0025, 0.0025, 0.0025), vec3(-0.4, 0.32, -2.8),
  //             vec3(90, 180, 0), color(0.3, 0.3, 0.3));
  // scene.add_mesh(lucy);

  // mesh xyz_dragon("models/xyzrgb_dragon.obj", vec3(0.02, 0.02, 0.02), vec3(0.85, -0.9, 0.8),
  //             vec3(0, 140, 0), color(0.4, 0.4, 0.8));
  // scene.add_mesh(xyz_dragon);

  std::ofstream file_to_save_image;
  file_to_save_image.open("renders/analysis/bvh/scene12_bvh___.ppm");

  camera cam(point3(0, 3, 7.5), point3(0, 0, -1), vec3(0, 1, 0), 45,
             aspect_ratio);

  file_to_save_image << "P3\n"
                     << image_width << " " << image_height << "\n255\n";

  auto t1 = high_resolution_clock::now();
  scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                     file_to_save_image);
  auto t2 = high_resolution_clock::now();
  duration<double, std::milli> ms_double = t2 - t1;
  std::cout << "\nTime taken to render the image: "
            << (ms_double.count() / 1000) << " seconds\n";
  // std::cout << "Ray-mesh intersection tests: " << scene.get_ray_mesh_tests()
  //           << std::endl;
  std::cout << "Ray-triangle intersection tests: "
            << scene.get_ray_triangle_tests() << std::endl;
  std::cout << "Ray-sphere intersection tests: " << scene.get_ray_sphere_tests()
            << std::endl << std::endl;

  scene.clear_scene();
}

inline void bvh_test_scene() {
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 1000;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 3;

  std::cout << "Image width: " << image_width << std::endl;
  std::cout << "Image height: " << image_height << std::endl;

  Scene scene(1, 1, 1, 1, color(0.678, 0.847, 0.9));

  // texture earth("textures/2k_earth_daymap.ppm");
  // texture jupiter("textures/jupiter.ppm");
  // texture mars("textures/mars.ppm");

  // sphere sphere_1(point3(0, 2, -2), 0.8, earth);
  // sphere sphere_2(point3(2, 2, -2), 0.6, jupiter);
  // sphere sphere_3(point3(-2, 2, -2), 0.6, mars);

  // scene.add_sphere(sphere_1);
  // scene.add_sphere(sphere_2);
  // scene.add_sphere(sphere_3);

  // scene.add_triangle(triangle(point3(-5, -1, -6 + 0.0001), point3(5, -1, -6),
  //                             point3(5, 5, -6), color(0.5, 0.5, 0.5)));

  // scene.add_triangle(triangle(point3(-5, -1, -6 + 0.0001), point3(5, 5, -6),
  //                             point3(-5, 5, -6), color(0.5, 0.5, 0.5)));

  // scene.add_triangle(triangle(point3(-5, -1 + 0.0001, -6), point3(5, -1, -6),
  //                             point3(-5, -1, 2.5), color(0.5, 0.5, 0.5)));

  // scene.add_triangle(triangle(point3(5, -1 + 0.0001, 2.5), point3(-5, -1, 2.5),
  //                             point3(5, -1, -6), color(0.5, 0.5, 0.5)));

  scene.add_triangle(triangle(point3(-10, -1 + 0.0001, -10), point3(10, -1, -10),
                              point3(-10, -1, 10), color(0.5, 0.5, 0.5)));

  scene.add_triangle(triangle(point3(10, -1 + 0.0001, 10), point3(-10, -1, 10),
                              point3(10, -1, -10), color(0.5, 0.5, 0.5)));

  // scene.add_point_light(PointLight(color(0, 1, 1), 30, vec3(-2, 2, 5)));
  // scene.add_point_light(PointLight(color(1, 1, 0), 30, vec3(2, 2, 5)));
  scene.add_point_light(PointLight(color(0.95, 0.8, 0.25), 60, vec3(0, 10, 5)));
  scene.add_point_light(PointLight(color(1, 1, 1), 30, vec3(0, 2, 5)));

  // scene.add_area_light(AreaLight(color(0, 1, 1), 30, vec3(2, 2, 5), 1), 5);
  // scene.add_area_light(AreaLight(color(1, 1, 0), 30, vec3(-2, 2, 5), 1), 5);
  // scene.add_area_light(AreaLight(color(1, 1, 1), 30, vec3(0, 2, 5), 1), 5);
  // scene.add_area_light(AreaLight(color(0.95, 0.8, 0.25), 70, vec3(0, 10, 5), 1), 6);
  // scene.add_area_light(AreaLight(color(1, 1, 1), 30, vec3(0, 2, 5), 1), 6);

  // scene.add_point_light(PointLight(color(1, 1, 1), 70, vec3(0, 2, 5)));

  // mesh suzanne("models/suzanne.obj", vec3(1, 1, 1), vec3(2.5, -0.5, -0.25),
  //            vec3(0, 30, 0), color(0.18, 0.3, 1));
  // scene.add_mesh(suzanne);

  // mesh lucy("models/lucy.obj", vec3(0.0025, 0.0025, 0.0025), vec3(-0.4, 0.32, -2.8),
  //             vec3(90, 180, 0), color(0.3, 0.3, 0.3));
  // scene.add_mesh(lucy);

  mesh dragon("models/dragon.obj", vec3(3, 3, 3), vec3(-0.75, -0.25, 2),
              vec3(0, 260, 0), color(0.9, 0.2, 0.2));
  scene.add_mesh(dragon);

  mesh bunny("models/stanford-bunny.obj", vec3(15, 15, 15), vec3(-4, 0.5, 0),
             vec3(0, -30, 0), color(0.8, 0.8, 0.8));
  scene.add_mesh(bunny);

  // mesh sponza("models/sponza.obj", vec3(0.0005, 0.0005, 0.0005), vec3(0, 0, 0.5),
  //             vec3(0, 90, 0), color(0.6, 0.6, 0.6));
  // scene.add_mesh(sponza);

  // mesh serapis("models/serapis.obj", vec3(0.035, 0.035, 0.035), vec3(0, 0.25, 0),
  //             vec3(90, -60, 0), color(0.2, 0.2, 0.2));
  // scene.add_mesh(serapis);


  // 15, 15 .... 25, 25

  srand(time(0));
  for (int i = -10; i < 10; i++) {
    for (int j = -30; j < 30; j++) {
      int random_scale = (rand() % 5) + 2;
      std::cout << random_scale << std::endl;
      mesh grass("models/low_grass.obj", vec3(random_scale, random_scale, random_scale), vec3(j*0.45, -0.8, i), vec3(0, 0, 0), color(0, 0.9, 0.45));
      scene.add_mesh(grass);
    }
  }
  

  mesh happy_buddha("models/buddha.obj", vec3(3, 3, 3), vec3(2.75, 0, 1), vec3(0, 220, 0), color(0.87, 0.188, 0.004));
  scene.add_mesh(happy_buddha);

  // mesh cube("models/cube.obj", vec3(1, 1, 1), vec3(0, 0, -1), vec3(0, 45, 25),
  //           color(0.18, 0.3, 1));
  // mesh cube2("models/cube.obj", vec3(0.8, 0.8, 0.8), vec3(2, 0, -1), vec3(0, 45, 25),
  //           color(0.9, 0.2, 0.2));
  // mesh cube3("models/cube.obj", vec3(0.8, 0.8, 0.8), vec3(-2, 0, -1), vec3(0, 45, 25),
  //           color(0.8, 0.8, 0.2));
  // scene.add_mesh(cube);
  // scene.add_mesh(cube2);
  // scene.add_mesh(cube3);

  std::ofstream file_to_save_image;
  file_to_save_image.open("renders/analysis/bvh/bvh_test_scene_ug_density_10.ppm");

  // camera cam(point3(0.005, 0.05, -0.75), point3(0, 0, -1), vec3(0, 1, 0), 45,
  //            aspect_ratio);

  // camera cam(point3(0, 2, 10), point3(0, 0, -1), vec3(0, 1, 0), 45,
  //            aspect_ratio);

  camera cam(point3(0, 2, 7), point3(0, 0, -1), vec3(0, 1, 0), 45,
             aspect_ratio);

  file_to_save_image << "P3\n"
                     << image_width << " " << image_height << "\n255\n";

  scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                     file_to_save_image);
  std::cout << "Ray-triangle intersection tests: "
            << scene.get_ray_triangle_tests() << std::endl;
  std::cout << "Ray-sphere intersection tests: " << scene.get_ray_sphere_tests()
            << std::endl << std::endl;

  scene.clear_scene();
}

inline void analysis_scene_2 () {
  const auto aspect_ratio = 1;
  const int image_width = 500;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 1;

  std::cout << "Image width: " << image_width << "\nImage height: " << image_height << std::endl;

  Scene scene(1, 1, 1, 1);

  mesh bunny("models/stanford-bunny.obj", vec3(23, 23, 23), vec3(-1.25, 0.75, 0),
             vec3(0, -30, 0), color(0.97, 0.7, 0.008));
  scene.add_mesh(bunny);

  scene.add_point_light(PointLight(color(1, 1, 1), 30, vec3(0, 2, 5)));

  std::ofstream file_to_save_image;
  file_to_save_image.open("renders/analysis/bvh/analysis_scenes/scene_1_bvh.ppm");
  
  camera cam(point3(0, 2, 7), point3(0, 0, -1), vec3(0, 1, 0), 45,
             aspect_ratio);

  file_to_save_image << "P3\n"
                     << image_width << " " << image_height << "\n255\n";

  scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                     file_to_save_image);
  std::cout << "Ray-triangle intersection tests: "
            << scene.get_ray_triangle_tests() << std::endl;
  std::cout << "Ray-sphere intersection tests: " << scene.get_ray_sphere_tests()
            << std::endl << std::endl;

  scene.clear_scene();

}

inline void analysis_scene_1 () {
  const auto aspect_ratio = 1;
  const int image_width = 500;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 1;

  std::cout << "Image width: " << image_width << "\nImage height: " << image_height << std::endl;

  Scene scene(1, 1, 1, 1);

  mesh teapot("models/teapot.obj", vec3(0.032, 0.032, 0.032), vec3(-0.5, 2, -1),
              vec3(0, 0, 0), color(0.9, 0.2, 0.2));
  scene.add_mesh(teapot);

  scene.add_point_light(PointLight(color(1, 1, 1), 30, vec3(0, 2, 5)));

  std::ofstream file_to_save_image;
  file_to_save_image.open("renders/analysis/bvh/analysis_scenes/scene_2_bvh.ppm");
  
  camera cam(point3(0, 2, 7), point3(0, 0, -1), vec3(0, 1, 0), 45,
             aspect_ratio);

  file_to_save_image << "P3\n"
                     << image_width << " " << image_height << "\n255\n";

  scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                     file_to_save_image);
  std::cout << "Ray-triangle intersection tests: "
            << scene.get_ray_triangle_tests() << std::endl;
  std::cout << "Ray-sphere intersection tests: " << scene.get_ray_sphere_tests()
            << std::endl << std::endl;

  scene.clear_scene();

}

inline void analysis_scene_3 () {
  const auto aspect_ratio = 1;
  const int image_width = 500;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 1;

  std::cout << "Image width: " << image_width << "\nImage height: " << image_height << std::endl;

  Scene scene(1, 1, 1, 1);

  mesh lucy("models/lucy.obj", vec3(0.0042, 0.0042, 0.0042), vec3(-0.4, -1.5, -2.8),
              vec3(90, 180, 0), color(0.6, 0.6, 0.6));
  scene.add_mesh(lucy);

  scene.add_point_light(PointLight(color(1, 1, 1), 30, vec3(0, 2, 5)));

  std::ofstream file_to_save_image;
  file_to_save_image.open("renders/analysis/bvh/analysis_scenes/scene_3_bvh.ppm");
  
  camera cam(point3(0, 2, 7), point3(0, 0, -1), vec3(0, 1, 0), 45,
             aspect_ratio);

  file_to_save_image << "P3\n"
                     << image_width << " " << image_height << "\n255\n";

  scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                     file_to_save_image);
  std::cout << "Ray-triangle intersection tests: "
            << scene.get_ray_triangle_tests() << std::endl;
  std::cout << "Ray-sphere intersection tests: " << scene.get_ray_sphere_tests()
            << std::endl << std::endl;

  scene.clear_scene();

}

inline void analysis_scene_4 () {
  const auto aspect_ratio = 1;
  const int image_width = 500;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 1;

  std::cout << "Image width: " << image_width << "\nImage height: " << image_height << std::endl;

  Scene scene(1, 1, 1, 1);

  mesh xyz_dragon("models/xyzrgb_dragon.obj", vec3(0.022, 0.022, 0.022), vec3(1.35, -0.25, 0.8),
              vec3(0, 140, 0), color(0.4, 0.4, 0.8));
  scene.add_mesh(xyz_dragon);

  scene.add_point_light(PointLight(color(1, 1, 1), 30, vec3(0, 2, 5)));

  std::ofstream file_to_save_image;
  file_to_save_image.open("renders/analysis/bvh/analysis_scenes/scene_4_bvh.ppm");
  
  camera cam(point3(0, 2, 7), point3(0, 0, -1), vec3(0, 1, 0), 45,
             aspect_ratio);

  file_to_save_image << "P3\n"
                     << image_width << " " << image_height << "\n255\n";

  scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                     file_to_save_image);
  std::cout << "Ray-triangle intersection tests: "
            << scene.get_ray_triangle_tests() << std::endl;
  std::cout << "Ray-sphere intersection tests: " << scene.get_ray_sphere_tests()
            << std::endl << std::endl;

  scene.clear_scene();

}

inline void analysis_scene_5 () {
  const auto aspect_ratio = 1;
  const int image_width = 500;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 1;

  std::cout << "Image width: " << image_width << "\nImage height: " << image_height << std::endl;

  Scene scene(1, 1, 1, 1);

  mesh suzanne("models/suzanne.obj", vec3(1.8, 1.8, 1.8), vec3(-1.25, -0.5, -1),
             vec3(0, 0, 0), color(0.18, 0.3, 1));
  scene.add_mesh(suzanne);

  scene.add_point_light(PointLight(color(1, 1, 1), 30, vec3(0, 2, 5)));

  std::ofstream file_to_save_image;
  file_to_save_image.open("renders/analysis/bvh/analysis_scenes/scene_5_bvh.ppm");
  
  camera cam(point3(0, 2, 7), point3(0, 0, -1), vec3(0, 1, 0), 45,
             aspect_ratio);

  file_to_save_image << "P3\n"
                     << image_width << " " << image_height << "\n255\n";

  scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                     file_to_save_image);
  std::cout << "Ray-triangle intersection tests: "
            << scene.get_ray_triangle_tests() << std::endl;
  std::cout << "Ray-sphere intersection tests: " << scene.get_ray_sphere_tests()
            << std::endl << std::endl;

  scene.clear_scene();

}

inline void analysis_scene_6 () {
  const auto aspect_ratio = 1;
  const int image_width = 500;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 1;

  std::cout << "Image width: " << image_width << "\nImage height: " << image_height << std::endl;

  Scene scene(1, 1, 1, 1);

  mesh dragon("models/dragon.obj", vec3(4, 4, 4), vec3(-0.75, 0, 2),
              vec3(0, 260, 0), color(0.9, 0.2, 0.2));
  scene.add_mesh(dragon);

  scene.add_point_light(PointLight(color(1, 1, 1), 30, vec3(0, 2, 5)));

  std::ofstream file_to_save_image;
  file_to_save_image.open("renders/analysis/bvh/analysis_scenes/scene_6_ug.ppm");
  
  camera cam(point3(0, 2, 7), point3(0, 0, -1), vec3(0, 1, 0), 45,
             aspect_ratio);

  file_to_save_image << "P3\n"
                     << image_width << " " << image_height << "\n255\n";

  scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                     file_to_save_image);
  std::cout << "Ray-triangle intersection tests: "
            << scene.get_ray_triangle_tests() << std::endl;
  std::cout << "Ray-sphere intersection tests: " << scene.get_ray_sphere_tests()
            << std::endl << std::endl;

  scene.clear_scene();

}

inline void analysis_scene_7 () {
  const auto aspect_ratio = 1;
  const int image_width = 500;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 1;

  std::cout << "Image width: " << image_width << "\nImage height: " << image_height << std::endl;

  Scene scene(1, 1, 1, 1);

  mesh happy_buddha("models/buddha.obj", vec3(4.5, 4.5, 4.5), vec3(0.25, -0.55, 1), vec3(0, 180, 0), color(0.87, 0.188, 0.004));
  scene.add_mesh(happy_buddha);

  scene.add_point_light(PointLight(color(1, 1, 1), 30, vec3(0, 2, 5)));

  std::ofstream file_to_save_image;
  file_to_save_image.open("renders/analysis/bvh/analysis_scenes/scene_7_ug.ppm");
  
  camera cam(point3(0, 2, 7), point3(0, 0, -1), vec3(0, 1, 0), 45,
             aspect_ratio);

  file_to_save_image << "P3\n"
                     << image_width << " " << image_height << "\n255\n";

  scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                     file_to_save_image);
  std::cout << "Ray-triangle intersection tests: "
            << scene.get_ray_triangle_tests() << std::endl;
  std::cout << "Ray-sphere intersection tests: " << scene.get_ray_sphere_tests()
            << std::endl << std::endl;

  scene.clear_scene();

}

inline void analysis_scene_8 () {
  const auto aspect_ratio = 1;
  const int image_width = 500;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 1;

  std::cout << "Image width: " << image_width << "\nImage height: " << image_height << std::endl;

  Scene scene(1, 1, 1, 1);

  mesh armadillo("models/spot_2.obj", vec3(2.5, 2.5, 2.5), vec3(-0.25, -0.5, 1), vec3(0, 240, 0), color(0, 0.55, 0.47));
  scene.add_mesh(armadillo);

  scene.add_point_light(PointLight(color(1, 1, 1), 30, vec3(0, 2, 5)));

  std::ofstream file_to_save_image;
  file_to_save_image.open("renders/analysis/bvh/analysis_scenes/scene_8_bvh.ppm");
  
  camera cam(point3(0, 2, 7), point3(0, 0, -1), vec3(0, 1, 0), 45,
             aspect_ratio);

  file_to_save_image << "P3\n"
                     << image_width << " " << image_height << "\n255\n";

  scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                     file_to_save_image);
  std::cout << "Ray-triangle intersection tests: "
            << scene.get_ray_triangle_tests() << std::endl;
  std::cout << "Ray-sphere intersection tests: " << scene.get_ray_sphere_tests()
            << std::endl << std::endl;

  scene.clear_scene();

}

inline void benchmark_scene_1() {
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 1000;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 3;

  Scene scene(1, 1, 1, 1);

  scene.add_triangle(triangle(point3(-6, -1, -6 + 0.00001), point3(6, -1, -6),
                              point3(6, 5, -6), color(0.2, 0.2, 0.2)));

  scene.add_triangle(triangle(point3(-6, -1, -6 + 0.00001), point3(6, 5, -6),
                              point3(-6, 5, -6), color(0.2, 0.2, 0.2)));

  scene.add_triangle(triangle(point3(-6, -1 + 0.00001, -6), point3(6, -1, -6),
                              point3(-6, -1, 2.5), color(0.2, 0.2, 0.2)));
  scene.add_triangle(triangle(point3(6, -1 + 0.00001, 2.5), point3(-6, -1, 2.5),
                              point3(6, -1, -6), color(0.2, 0.2, 0.2)));

  // scene.add_point_light(PointLight(color(0, 1, 1), 30, vec3(-2, 2, -1)));
  // scene.add_point_light(PointLight(color(1, 1, 0), 30, vec3(2, 2, -1)));
  // scene.add_point_light(PointLight(color(1, 1, 1), 10, vec3(0, 2, -1)));
  // scene.add_point_light(PointLight(color(0.6, 0.5, 0.4), 30, vec3(-2.5, 2, 0.5)));
  // scene.add_point_light(PointLight(color(0.6, 0.5, 0.4), 30, vec3(2.5, 2, 0.5)));

  scene.add_area_light(AreaLight(color(0, 1, 1), 30, vec3(-2, 2, -1), 1), 3);
  scene.add_area_light(AreaLight(color(1, 1, 0), 30, vec3(2, 2, -1), 1), 3);
  scene.add_area_light(AreaLight(color(1, 1, 1), 10, vec3(0, 2, -1), 1), 3);

  scene.add_area_light(AreaLight(color(0.6, 0.5, 0.4), 30, vec3(-2.5, 2, 0.5), 1), 3);
  scene.add_area_light(AreaLight(color(0.6, 0.5, 0.4), 30, vec3(2.5, 2, 0.5), 1), 3);

  mesh teapot("models/teapot.obj", vec3(0.018, 0.018, 0.018), vec3(-0.5, 1, -1),
              vec3(0, 0, 0), color(1, 0.3, 0.1));
  scene.add_mesh(teapot);

  mesh bunny("models/stanford-bunny.obj", vec3(13, 13, 13), vec3(-4, 0.45, 0),
             vec3(0, -30, 0), color(0.8, 0.8, 0.8));
  scene.add_mesh(bunny);

  mesh spot("models/spot_2.obj", vec3(1.2, 1.2, 1.2), vec3(2.75, -0.45, 1.75), vec3(0, 240, 0), color(0, 0.55, 0.47));
  scene.add_mesh(spot);

  // mesh suzanne("models/suzanne.obj", vec3(1, 1, 1), vec3(2.5, -0.5, -0.25),
  //            vec3(0, 30, 0), color(0.18, 0.3, 1));
  // scene.add_mesh(suzanne);

  mesh dragon("models/dragon.obj", vec3(3.5, 3.5, 3.5), vec3(2.5, -0.25, -0.25),
              vec3(0, 260, 0), color(0.9, 0.2, 0.2));
  scene.add_mesh(dragon);

  mesh pine_tree_1("models/pine_tree.obj", vec3(0.0035, 0.0035, 0.0035), vec3(2, 0.5, -3),
              vec3(0, 0, 0), color(0, 0.9, 0.45));
  scene.add_mesh(pine_tree_1);

  mesh pine_tree_2("models/pine_tree.obj", vec3(0.0035, 0.0035, 0.0035), vec3(-3.5, 0.5, -3),
              vec3(0, 0, 0), color(0, 0.9, 0.45));
  scene.add_mesh(pine_tree_2);

  mesh lucy("models/lucy.obj", vec3(0.0025, 0.0025, 0.0025), vec3(-0.4, 0.32, -2.5),
              vec3(90, 180, 0), color(0.3, 0.3, 0.3));
  scene.add_mesh(lucy);

  mesh serapis("models/serapis.obj", vec3(0.025, 0.025, 0.025), vec3(-2.75, 0, 1.75),
              vec3(90, -100, 0), color(0.2, 0.2, 0.2));
  scene.add_mesh(serapis);

  mesh xyz_dragon("models/xyzrgb_dragon.obj", vec3(0.02, 0.02, 0.02), vec3(0.85, -0.9, 0.8),
              vec3(0, 140, 0), color(0.4, 0.4, 0.8));
  scene.add_mesh(xyz_dragon);

  std::ofstream file_to_save_image;
  file_to_save_image.open("renders/benchmark_scene_1.ppm");

  camera cam(point3(0, 3, 7.5), point3(0, 0, -1), vec3(0, 1, 0), 45,
             aspect_ratio);

  file_to_save_image << "P3\n"
                     << image_width << " " << image_height << "\n255\n";

  auto t1 = high_resolution_clock::now();
  scene.render_scene(image_width, image_height, samples_per_pixel, cam,
                     file_to_save_image);
  auto t2 = high_resolution_clock::now();
  duration<double, std::milli> ms_double = t2 - t1;
  std::cout << "\nTime taken to render the image: "
            << (ms_double.count() / 1000) << " seconds\n";
  std::cout << "Ray-triangle intersection tests: "
            << scene.get_ray_triangle_tests() << std::endl;
  std::cout << "Ray-sphere intersection tests: " << scene.get_ray_sphere_tests()
            << std::endl << std::endl;

  scene.clear_scene();
}
#endif