#ifndef SHADER_COMPILER_H
#define SHADER_COMPILER_H

#include <string>
#include <vector>

#include <GL/glew.h>

class Shader {
 public:
  Shader()  {}
  Shader(std::string const& path_vertex, std::string const& path_fragment);

void use() const;
bool compileShader(const char* path, GLuint type, GLuint& shader, std::string& error);
bool linkShaders(std::vector<GLuint> const& shaders, GLuint& shaderProg, std::string& error);
GLuint getId() { return shader_prog;}
 private:
GLuint shader_prog,
       vertex_shader,
       fragment_shader;
};

#endif
