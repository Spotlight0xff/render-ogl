#ifndef SHADER_COMPILER_H
#define SHADER_COMPILER_H

#include <string>
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    void set(const char* name, glm::vec3 const& p) { use(); glUniform3fv(getUniform(name), 1, glm::value_ptr(p)); }
    void set(const char* name, glm::vec4 const& p) { use(); glUniform4fv(getUniform(name), 1, glm::value_ptr(p)); }
    void set(const char* name, glm::mat3 const& p) { use(); glUniformMatrix3fv(getUniform(name), 1, GL_FALSE, glm::value_ptr(p)); }
    void set(const char* name, glm::mat4 const& p) { use(); glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, glm::value_ptr(p)); }
    void set(const char* name, GLfloat const& p)   { use(); glUniform1f(getUniform(name), p); }
    void set(const char* name, GLint const& p)     { use(); glUniform1i(getUniform(name), p); }

    void set(std::string name, glm::vec3 const& p) { use(); glUniform3fv(getUniform(name.c_str()), 1, glm::value_ptr(p)); }
    void set(std::string name, glm::vec4 const& p) { use(); glUniform4fv(getUniform(name.c_str()), 1, glm::value_ptr(p)); }
    void set(std::string name, glm::mat3 const& p) { use(); glUniformMatrix3fv(getUniform(name.c_str()), 1, GL_FALSE, glm::value_ptr(p)); }
    void set(std::string name, glm::mat4 const& p) { use(); glUniformMatrix4fv(getUniform(name.c_str()), 1, GL_FALSE, glm::value_ptr(p)); }
    void set(std::string name, GLfloat const& p)   { use(); glUniform1f(getUniform(name.c_str()), p); }
    void set(std::string name, GLint const& p)     { use(); glUniform1i(getUniform(name.c_str()), p); }
    GLuint getId() const { return shader_prog; }

  private:
    GLuint shader_prog;
    std::string name;
    static constexpr const char *directory = "resources/shaders/";

    bool compileShader(std::string file, GLuint type, GLuint &shader, std::string &error);

    bool linkShaders(std::vector<GLuint> const &shaders, GLuint &shaderProg, std::string &error);
};

} // namespace engine

#endif
