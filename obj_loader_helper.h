#ifndef OBJ_LOADER_HELPER_H
#define OBJ_LOADER_HELPER_H

#define TINYOBJLOADER_IMPLEMENTATION
#include "obj_loader.h"

#include "hittable_list.h"
#include "triangle.h"
#include "transforms.h"

void load_obj(std::string input_file, hittable_list& world, double scale_by, vec3 translate_by, double rotate_angle_x, double rotate_angle_y, double rotate_angle_z) {

    std::string inputfile = input_file;
    tinyobj::ObjReaderConfig reader_config;
    reader_config.mtl_search_path = "models/"; // Path to material files

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

                double tri_verts[9];
                int index_vert = 0;

                for (size_t v = 0; v < fv; v++) {
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                    tri_verts[index_vert] = attrib.vertices[3*size_t(idx.vertex_index)];
                    tri_verts[index_vert + 1] = attrib.vertices[3*size_t(idx.vertex_index) + 1];
                    tri_verts[index_vert + 2] = attrib.vertices[3*size_t(idx.vertex_index) + 2];

                    index_vert = index_vert + 3;
                }

                vec3 vertex_1(tri_verts[0], tri_verts[1], tri_verts[2]);
                vec3 vertex_2(tri_verts[3], tri_verts[4], tri_verts[5]);
                vec3 vertex_3(tri_verts[6], tri_verts[7], tri_verts[8]);

                vertex_1 = translate(vertex_1, translate_by);
                vertex_2 = translate(vertex_2, translate_by);
                vertex_3 = translate(vertex_3, translate_by);

                vertex_1 = scale(vertex_1, vec3(scale_by, scale_by, scale_by));
                vertex_2 = scale(vertex_2, vec3(scale_by, scale_by, scale_by));
                vertex_3 = scale(vertex_3, vec3(scale_by, scale_by, scale_by));

                vertex_1 = rotate(vertex_1, rotate_angle_x, point3(0.0, 1.0, 1.0));
                vertex_2 = rotate(vertex_2, rotate_angle_x, point3(0.0, 1.0, 1.0));
                vertex_3 = rotate(vertex_3, rotate_angle_x, point3(0.0, 1.0, 1.0));

                world.add(make_shared<triangle>(vertex_1,
                                                vertex_2,
                                                vertex_3));
                index_offset += fv;

                // per-face material
                shapes[s].mesh.material_ids[f];
            }
        }   
}

#endif