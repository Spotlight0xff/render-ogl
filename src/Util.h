#ifndef UTIL_H
#define UTIL_H
#include <string>
#include <vector>


#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace util {
size_t split(const std::string& s, char delimiter, std::vector<std::string>& splits);

void printVertices(size_t count_elements, size_t count_vertices, GLfloat const* flat_vertices, GLushort const* flat_elements);

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei /*length*/,
                            const GLchar *message,
                            const void * /*userParam*/);

bool enableDebugOutput();

}  // end namespace util


#endif
