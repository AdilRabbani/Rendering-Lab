#ifndef MESH_H
#define MESH_H

#include "Scratchapixel/aabb.h"
#include "triangle.h"
#include <algorithm>
#include <cstddef>
#include <vector>
#define TINYOBJLOADER_IMPLEMENTATION
#include "libraries/tinyobjloader.h"

void update_min_max_for_aabb(vec3 vertex_1, vec3 vertex_2, vec3 vertex_3,
                             double &min_x, double &min_y, double &min_z,
                             double &max_x, double &max_y, double &max_z,
                             size_t f) {

  if (f == 0) {
    min_x = vertex_1.x();
    min_y = vertex_1.y();
    min_z = vertex_1.z();
  } else {
    if (min_x > vertex_1.x()) {
      min_x = vertex_1.x();
    }

    if (min_y > vertex_1.y()) {
      min_y = vertex_1.y();
    }

    if (min_z > vertex_1.z()) {
      min_z = vertex_1.z();
    }
  }

  if (min_x > vertex_2.x()) {
    min_x = vertex_2.x();
  }
  if (min_x > vertex_3.x()) {
    min_x = vertex_3.x();
  }

  if (min_y > vertex_2.y()) {
    min_y = vertex_2.y();
  }
  if (min_y > vertex_3.y()) {
    min_y = vertex_3.y();
  }

  if (min_z > vertex_2.z()) {
    min_z = vertex_2.z();
  }
  if (min_z > vertex_3.z()) {
    min_z = vertex_3.z();
  }

  if (f == 0) {
    max_x = vertex_1.x();
    max_y = vertex_1.y();
    max_z = vertex_1.z();
  } else {
    if (max_x < vertex_1.x()) {
      max_x = vertex_1.x();
    }

    if (max_y < vertex_1.y()) {
      max_y = vertex_1.y();
    }

    if (max_z < vertex_1.z()) {
      max_z = vertex_1.z();
    }
  }

  if (max_x < vertex_2.x()) {
    max_x = vertex_2.x();
  }
  if (max_x < vertex_3.x()) {
    max_x = vertex_3.x();
  }

  if (max_y < vertex_2.y()) {
    max_y = vertex_2.y();
  }
  if (max_y < vertex_3.y()) {
    max_y = vertex_3.y();
  }

  if (max_z < vertex_2.z()) {
    max_z = vertex_2.z();
  }
  if (max_z < vertex_3.z()) {
    max_z = vertex_3.z();
  }
}

class mesh {

public:
  mesh() {}
  mesh(std::string input_file, vec3 scale_by, vec3 translate_by, vec3 rotate_by,
       color mesh_color);

public:
  aabb mesh_aabb;
  std::vector<triangle> mesh_triangles;
};

mesh::mesh(std::string input_file, vec3 scale_by, vec3 translate_by,
           vec3 rotate_by, color mesh_color) {

  std::string inputfile = input_file;
  tinyobj::ObjReaderConfig reader_config;
  reader_config.mtl_search_path = "models/"; // Path to models folder

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

  auto &attrib = reader.GetAttrib();
  auto &shapes = reader.GetShapes();
  auto &materials = reader.GetMaterials();

  double min_x, min_y, min_z;
  double max_x, max_y, max_z;

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
        tri_verts[index_vert] = attrib.vertices[3 * size_t(idx.vertex_index)];
        tri_verts[index_vert + 1] =
            attrib.vertices[3 * size_t(idx.vertex_index) + 1];
        tri_verts[index_vert + 2] =
            attrib.vertices[3 * size_t(idx.vertex_index) + 2];

        index_vert = index_vert + 3;
      }

      vec3 vertex_1(tri_verts[0], tri_verts[1], tri_verts[2]);
      vec3 vertex_2(tri_verts[3], tri_verts[4], tri_verts[5]);
      vec3 vertex_3(tri_verts[6], tri_verts[7], tri_verts[8]);

      update_min_max_for_aabb(vertex_1, vertex_2, vertex_3, min_x, min_y, min_z,
                              max_x, max_y, max_z, f);

      index_offset += fv;

      // per-face material
      shapes[s].mesh.material_ids[f];
    }
  }

  double center_x = min_x + max_x / 2;
  double center_y = min_y + max_y / 2;
  double center_z = min_z + max_z / 2;

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
        tri_verts[index_vert] = attrib.vertices[3 * size_t(idx.vertex_index)];
        tri_verts[index_vert + 1] =
            attrib.vertices[3 * size_t(idx.vertex_index) + 1];
        tri_verts[index_vert + 2] =
            attrib.vertices[3 * size_t(idx.vertex_index) + 2];

        index_vert = index_vert + 3;
      }

      vec3 vertex_1(tri_verts[0] - center_x, tri_verts[1] - center_y,
                    tri_verts[2] - center_z);
      vec3 vertex_2(tri_verts[3] - center_x, tri_verts[4] - center_y,
                    tri_verts[5] - center_z);
      vec3 vertex_3(tri_verts[6] - center_x, tri_verts[7] - center_y,
                    tri_verts[8] - center_z);

      triangle t(vertex_1, vertex_2, vertex_3, mesh_color);
      t.transform(translate_by, scale_by,
                  vec3(rotate_by.x(), rotate_by.y(), rotate_by.z()));

      mesh_triangles.push_back(t);

      update_min_max_for_aabb(t.p0, t.p1, t.p2, min_x, min_y, min_z, max_x,
                              max_y, max_z, f);

      index_offset += fv;

      // per-face material
      shapes[s].mesh.material_ids[f];
    }
  }

  mesh_aabb = aabb(point3(min_x, min_y, min_z), point3(max_x, max_y, max_z));
  std::cout << "Number of triangles in the mesh: " << mesh_triangles.size()
            << std::endl;
}

#endif