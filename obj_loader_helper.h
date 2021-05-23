#ifndef OBJ_LOADER_HELPER_H
#define OBJ_LOADER_HELPER_H

#define TINYOBJLOADER_IMPLEMENTATION
#include "obj_loader.h"

#include "hittable_list.h"
#include "triangle.h"

void load_obj(std::string input_file, hittable_list& world, double scaling, vec3 translate) {

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

#endif