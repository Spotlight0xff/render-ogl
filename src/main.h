#ifndef MAIN_H
#define MAIN_H

#include <vector>

#include <glm/glm.hpp>

bool loadObj(const char* path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec2>& out_uvs,
    std::vector<glm::vec3>& out_normals);


#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT 768

#endif
