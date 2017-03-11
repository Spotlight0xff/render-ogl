#include "Util.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef _WIN32
#include <Shlwapi.h>
#else
#include <unistd.h>
#endif

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

bool fileExists(std::string const& s) {
#ifdef _WIN32
	return PathFileExists(s.c_str());
#else
  return ( access( s.c_str(), F_OK ) != -1 );
#endif
}

void replaceAllOccurences( std::string& source, std::string const& from, std::string const& to )
{
  std::string new_str;
  // We roughly need the same buffer size (avoid allocations)
  new_str.reserve( source.length() );

  std::string::size_type lastPos = 0;
  std::string::size_type findPos;

  while( std::string::npos != ( findPos = source.find( from, lastPos )))
  {
    new_str.append( source, lastPos, findPos - lastPos );
    new_str += to;
    lastPos = findPos + from.length();
  }

  // Care for the rest after last occurrence
  new_str += source.substr( lastPos );

  source.swap( new_str );
}


// Return some paths for two different paths
std::vector<std::string> getEligiblePaths(const std::string& path1, const std::string& path2) {
  std::vector<std::string> paths;
  std::vector<std::string> split_path1;
  std::vector<std::string> split_path2;

  // TODO: platform independent
  split(path1, '/', split_path1);
  split(path2, '/', split_path2);

  for(size_t i=split_path1.size()-1; i > 0; i--) {
    std::stringstream path;
    for (size_t j=0; j < i; j++) {
      path << split_path1[j] << "/";
    }
    paths.push_back(path.str());
  }

  for(size_t i=split_path2.size()-1; i > 0; i--) {
    std::stringstream path;
    for (size_t j=0; j < i; j++) {
      path << split_path2[j] << "/";
    }
    paths.push_back(path.str());
  }

  paths.push_back("");
  //for(auto const& s: paths) {
    //std::cout << s << std::endl;
  //}
  return paths;
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

bool enableDebugOutput() {
  GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
  {
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(util::glDebugOutput, nullptr);
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
      return true;
  }
  return false;
}


void printVertices(size_t count_elements, GLushort const* flat_elements, size_t count_vertices, GLfloat const* flat_vertices) {
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
