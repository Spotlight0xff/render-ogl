#ifndef SHADER_COMPILER_H
#define SHADER_COMPILER_H

#include <string>
#include <vector>

#include <GL/glew.h>

class Shader {
 public:
  Shader()  {}
  Shader(std::string const& shader_name);

  void use() const;
  GLuint getId() const { return shader_prog;}
 private:
  GLuint shader_prog,
         vertex_shader,
         fragment_shader;
  std::string name;
  static constexpr const char* directory = "resources/shaders/";

  bool compileShader(std::string file, GLuint type, GLuint& shader, std::string& error);
  bool linkShaders(std::vector<GLuint> const& shaders, GLuint& shaderProg, std::string& error);
};

#endif
