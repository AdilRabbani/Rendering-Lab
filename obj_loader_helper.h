#ifndef OBJ_LOADER_HELPER_H
#define OBJ_LOADER_HELPER_H

#include "vec3.h"
#include <cmath>
#include <vector>
#define TINYOBJLOADER_IMPLEMENTATION
#include "obj_loader.h"

#include "scene.h"

void load_obj(std::string input_file, Scene &scene, double scale_by,
              vec3 translate_by, double rotate_angle_x, double rotate_angle_y,
              double rotate_angle_z, color triangle_material) {

  std::string inputfile = input_file;
  tinyobj::ObjReaderConfig reader_config;
  reader_config.mtl_search_path = "models/"; // Path to material files

  tinyobj::ObjReader reader;
  mesh m;

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

      if (f == 0) {
        min_x = vertex_1.x();
        min_y = vertex_1.y();
        min_z = vertex_1.z();
      }
      else {
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
      }
      else {
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


      index_offset += fv;

      // per-face material
      shapes[s].mesh.material_ids[f];
    }
  }

  std::cout << min_x << " " << min_y << " " << min_z << std::endl;
  std::cout << max_x << " " << max_y << " " << max_z << std::endl;

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

      vec3 vertex_1(tri_verts[0] - center_x, tri_verts[1] - center_y, tri_verts[2] - center_z);
      vec3 vertex_2(tri_verts[3] - center_x, tri_verts[4] - center_y, tri_verts[5] - center_z);
      vec3 vertex_3(tri_verts[6] - center_x, tri_verts[7] - center_y, tri_verts[8] - center_z);

      // rotation z

      double v1_x = vertex_1.x() * cos(rotate_angle_x) + vertex_1.y() * sin(rotate_angle_x);
      double v1_y = vertex_1.x() * (-sin(rotate_angle_x)) + vertex_1.y() * cos(rotate_angle_x);

      double v2_x = vertex_2.x() * cos(rotate_angle_x) + vertex_2.y() * sin(rotate_angle_x);
      double v2_y = vertex_2.x() * (-sin(rotate_angle_x)) + vertex_2.y() * cos(rotate_angle_x);

      double v3_x = vertex_3.x() * cos(rotate_angle_x) + vertex_3.y() * sin(rotate_angle_x);
      double v3_y = vertex_3.x() * (-sin(rotate_angle_x)) + vertex_3.y() * cos(rotate_angle_x);

      // rotation x

      v1_y = v1_y * cos(rotate_angle_x) + vertex_1.z() * sin(rotate_angle_x);
      double v1_z = v1_y * (-sin(rotate_angle_x)) + vertex_1.z() * cos(rotate_angle_x);

      v2_y = v2_y * cos(rotate_angle_x) + vertex_2.z() * sin(rotate_angle_x);
      double v2_z = v2_y * (-sin(rotate_angle_x)) + vertex_2.z() * cos(rotate_angle_x);

      v3_y = v3_y * cos(rotate_angle_x) + vertex_3.z() * sin(rotate_angle_x);
      double v3_z = v3_y * (-sin(rotate_angle_x)) + vertex_3.z() * cos(rotate_angle_x);

      // rotation y

      v1_x = v1_x * cos(rotate_angle_x) + v1_z * -sin(rotate_angle_x);
      v1_z = v1_x * (sin(rotate_angle_x)) + v1_z * cos(rotate_angle_x);

      v2_x = v2_x * cos(rotate_angle_x) + v2_z * -sin(rotate_angle_x);
      v2_z = v2_x * (sin(rotate_angle_x)) + v2_z * cos(rotate_angle_x);
      
      v3_x = v3_x * cos(rotate_angle_x) + v3_z * -sin(rotate_angle_x);
      v3_z = v3_x * (sin(rotate_angle_x)) + v3_z * cos(rotate_angle_x);

      vertex_1 = vec3(v1_x, v1_y, v1_z);
      vertex_2 = vec3(v2_x, v2_y, v2_z);
      vertex_3 = vec3(v3_x, v3_y, v3_z);

      vertex_1 = vertex_1 * scale_by;
      vertex_2 = vertex_2 * scale_by;
      vertex_3 = vertex_3 * scale_by;

      vertex_1 = vertex_1 + translate_by;
      vertex_2 = vertex_2 + translate_by;
      vertex_3 = vertex_3 + translate_by;
      

      m.mesh_triangles.push_back(triangle(vertex_1, vertex_2, vertex_3, triangle_material));

    //   scene.add_triangle(triangle(vertex_1, vertex_2, vertex_3, triangle_material));

      if (f == 0) {
        min_x = vertex_1.x();
        min_y = vertex_1.y();
        min_z = vertex_1.z();
      }
      else {
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
      }
      else {
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


      index_offset += fv;

      // per-face material
      shapes[s].mesh.material_ids[f];
    }
  }

  m.mesh_aabb = aabb(point3(min_x, min_y, min_z), point3(max_x, max_y, max_z));
  scene.add_mesh(m);
}

#endif