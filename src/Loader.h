#ifndef LOADER_H
#define LOADER_H

#include <glm/glm.hpp>

#include <vector>

namespace loader {

enum class FaceFormat {
  SPACE,
  SLASH,
  DOUBLESLASH
};

bool parseObj(const char* path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec3>& out_normals,
    std::vector<unsigned int>& out_elements,
    FaceFormat face_format);
}
#endif
