#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <iostream>
#include "pnm.h"

class texture{
    public:
        texture() {};
        texture(std::string filename) {
            texture_file_name = filename;
            std::cout << "Loading texture " << texture_file_name << " using pnm..." << std::endl;
            texture_image_data = pnm::read(texture_file_name);
            std::cout << "Done!" << std::endl;
        }

    public:
        std::string texture_file_name;
        pnm::image<pnm::rgb_pixel> texture_image_data;
};

#endif