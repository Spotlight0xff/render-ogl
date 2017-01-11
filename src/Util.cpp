#include "Util.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <sstream>
#include <vector>
#include <iostream>


namespace util {
size_t split(const std::string& s, char delimiter, std::vector<std::string>& splits) {
  std::string item;
  std::stringstream ss(s);
  while (std::getline(ss, item, delimiter)) {
    splits.push_back(item);
  }
  return splits.size();
}

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei /*length*/,
                            const GLchar *message,
                            const void * /*userParam*/)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

void printVertices(size_t count_elements, size_t count_vertices, GLfloat const* flat_vertices, GLushort const* flat_elements) {
  size_t count_triangles = count_elements / 3;
  std::cout << count_vertices << " Vertices:\n";
  for (size_t i=0; i < count_vertices; i ++) {
     std::cout << "\tVertices[" << i<< "]=" << flat_vertices[i] << "\n";
  }

  std::cout << count_triangles << " Triangles:\n";
  for (size_t i=0; i < count_elements; i ++) {
     std::cout << "\tElements[" << i<< "]=" << flat_elements[i] << ": "
       << flat_vertices[flat_elements[i]*3] << "|"
       << flat_vertices[flat_elements[i]*3+1] << "|"
       << flat_vertices[flat_elements[i]*3+2]
       << "\n";

    //std::cout << "Triangle " << i << ":\n";
    //for (size_t v=0; v < 9; v += 3) {
      //std::cout << "\tVertex " << 9*i + v<< ": " << flat_vertices[flat_elements[9*i+v]] << "\n";
      //std::cout << "\tVertex " << 9*i + v + 1<< "\n";
      //std::cout << "\tVertex " << 9*i + v + 2<< "\n";
    //}
    //GLfloat vertex_0 = flat_vertices[flat_elements[i]];
    //GLfloat vertex_1 = flat_vertices[flat_elements[i]];
    //GLfloat vertex_2 = flat_vertices[flat_elements[i]];
    //std::cout << "Triangle " << i << ": " << vertex_0 << "|" << vertex_1 << "|" << vertex_2 << "\n";
  }
}
}
