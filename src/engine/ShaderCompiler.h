#ifndef SHADER_COMPILER_H
#define SHADER_COMPILER_H

#include <string>
#include <vector>

#include "opengl.h"

namespace engine {

class Shader {
  public:
    // TODO: Caching, RAII, more ctors, shader-compiler -> shader-instance
    Shader() {}

    Shader(std::string const &shader_name);

    void use() const;
    GLint getUniform(const char* name) {
      return glGetUniformLocation(shader_prog, name);
    }

    GLuint getId() const { return shader_prog; }

  private:
    GLuint shader_prog,
            vertex_shader,
            fragment_shader;
    std::string name;
    static constexpr const char *directory = "resources/shaders/";

    bool compileShader(std::string file, GLuint type, GLuint &shader, std::string &error);

    bool linkShaders(std::vector<GLuint> const &shaders, GLuint &shaderProg, std::string &error);
};

} // namespace engine

#endif
