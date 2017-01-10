#ifndef MAIN_H
#define MAIN_H

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

bool loadObj(const char* path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec3>& out_normals,
    std::vector<GLushort>& out_elements);

void cbKeyEvents(GLFWwindow* window, int key, int scancode, int action, int mode);


#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT 768

#endif
