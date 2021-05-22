#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cmath>
#include <limits>
#include <memory>

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "triangle.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "obj_loader.h"

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

void load_obj(std::string input_file, hittable_list& world, double scaling, vec3 translate) {

    std::string inputfile = input_file;
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "./"; // Path to material files

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(inputfile, reader_config)) {
    if (!reader.Error().empty()) {
        std::cerr << "TinyObjReader: " << reader.Error();
    }
    exit(1);
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
    
    // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            // access to vertex
            tinyobj::index_t idx_0 = shapes[s].mesh.indices[index_offset + 0];
            double vx_0 = attrib.vertices[3*size_t(idx_0.vertex_index)+0];
            double vy_0 = attrib.vertices[3*size_t(idx_0.vertex_index)+1];
            double vz_0 = attrib.vertices[3*size_t(idx_0.vertex_index)+2];

            tinyobj::index_t idx_1 = shapes[s].mesh.indices[index_offset + 1];
            double vx_1 = attrib.vertices[3*size_t(idx_1.vertex_index)+0];
            double vy_1 = attrib.vertices[3*size_t(idx_1.vertex_index)+1];
            double vz_1 = attrib.vertices[3*size_t(idx_1.vertex_index)+2];

            tinyobj::index_t idx_2 = shapes[s].mesh.indices[index_offset + 2];
            double vx_2 = attrib.vertices[3*size_t(idx_2.vertex_index)+0];
            double vy_2 = attrib.vertices[3*size_t(idx_2.vertex_index)+1];
            double vz_2 = attrib.vertices[3*size_t(idx_2.vertex_index)+2];

            double tri_verts[9];
            vec3 tri_normal;
            int index_vert = 0;
            bool normal_available = false;

            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tri_verts[index_vert] = attrib.vertices[3*size_t(idx.vertex_index)];
                tri_verts[index_vert + 1] = attrib.vertices[3*size_t(idx.vertex_index) + 1];
                tri_verts[index_vert + 2] = attrib.vertices[3*size_t(idx.vertex_index) + 2];

                index_vert = index_vert + 3;
            }

            world.add(make_shared<triangle>(point3((tri_verts[0] + translate.x()) * scaling,
                                                    (tri_verts[1] + translate.y()) * scaling,
                                                    (tri_verts[2]) * scaling),
                                            point3((tri_verts[3] + translate.x()) * scaling,
                                                    (tri_verts[4] + translate.y()) * scaling,
                                                    (tri_verts[5]) * scaling),
                                            point3((tri_verts[6] + translate.x()) * scaling,
                                                    (tri_verts[7] + translate.y()) * scaling,
                                                    (tri_verts[8]) * scaling)));
            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
            }
        }   
}

// Common Headers

#include "ray.h"
#include "vec3.h"

#endif