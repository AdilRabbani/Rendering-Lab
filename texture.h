#ifndef TEXTURE_H
#define TEXTURE_H

#include "RTIOW/vec3.h"
#include "libraries/pnm.h"
#include <iostream>
#include <string>

class texture {
public:
  texture(){};
  texture(std::string filename) {
    texture_file_name = filename;
    std::cout << "Loading texture " << texture_file_name << " using pnm..."
              << std::endl;
    texture_image_data = pnm::read(texture_file_name);
    std::cout << "Done loading texture!" << std::endl;
  }
  color get_texel(double u, double v) {
    int height = texture_image_data.y_size();
    int width = texture_image_data.x_size();
    double red = (double)(texture_image_data[(int)round((v)*height)]
                                            [(int)round(u * width)]
                                                .red /
                          255.0);
    double green = (double)(texture_image_data[(int)round((v)*height)]
                                              [(int)round(u * width)]
                                                  .green /
                            255.0);
    double blue = (double)(texture_image_data[(int)round((v)*height)]
                                             [(int)round(u * width)]
                                                 .blue /
                           255.0);
    return color(red, green, blue);
  }

public:
  std::string texture_file_name;
  pnm::image<pnm::rgb_pixel> texture_image_data;
};

#endif