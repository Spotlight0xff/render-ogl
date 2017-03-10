#ifndef SHADER_COMPILER_H
#define SHADER_COMPILER_H

#include "stage.h"
#include "opengl.h"

#include <string>
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


namespace engine {
namespace shader {


class Compiler {
  public:
    // TODO: Caching, RAII, more ctors, shader-compiler -> shader-instance
    //! Default constructor which does nothing.
    Compiler() {}

    /*!
     * Constructs a new shader instance.
     *
     * @param shader_name name of the shader to load
     * @param compile_link `true`: compile and link directly, `false` defer to later
     * @param stages stages of the shader program, @see engine::shader::Stage.
     *               Defaults to vertex and fragment shader.
     */
    Compiler(std::string const &shader_name, bool compile_link = true,
                       std::vector<Stage> const& stages = {Stage::VERTEX_SHADER, Stage::FRAGMENT_SHADER});

    //! Destructor (deletes stages and shader program)
    ~Compiler();

    //! Compiles the stages of a shader program.
    bool setup();

    //! Links the shader program.
    bool finalize();

    //! Use the shader program.
    void use() const;

    /*!
     * Get the uniform location of a variable.
     *
     * @param name uniform variable name
     * @return uniform location
     */
    GLint getUniform(const char *name) {
      return glGetUniformLocation(shader_prog_, name);
    }

    //! Set the value of a uniform variable.
    void set(const char *name, glm::vec3 const &p) {
      use();
      glUniform3fv(getUniform(name), 1, glm::value_ptr(p));
    }

    //! Set the value of a uniform variable.
    void set(const char *name, glm::vec4 const &p) {
      use();
      glUniform4fv(getUniform(name), 1, glm::value_ptr(p));
    }

    //! Set the value of a uniform variable.
    void set(const char *name, glm::mat3 const &p) {
      use();
      glUniformMatrix3fv(getUniform(name), 1, GL_FALSE, glm::value_ptr(p));
    }

    //! Set the value of a uniform variable.
    void set(const char *name, glm::mat4 const &p) {
      use();
      glUniformMatrix4fv(getUniform(name), 1, GL_FALSE, glm::value_ptr(p));
    }

    //! Set the value of a uniform variable.
    void set(const char *name, GLfloat const &p) {
      use();
      glUniform1f(getUniform(name), p);
    }

    //! Set the value of a uniform variable.
    void set(const char *name, GLint const &p) {
      use();
      glUniform1i(getUniform(name), p);
    }

    //! Set the value of a uniform variable.
    void set(std::string name, glm::vec3 const &p) {
      use();
      glUniform3fv(getUniform(name.c_str()), 1, glm::value_ptr(p));
    }

    //! Set the value of a uniform variable.
    void set(std::string name, glm::vec4 const &p) {
      use();
      glUniform4fv(getUniform(name.c_str()), 1, glm::value_ptr(p));
    }

    //! Set the value of a uniform variable.
    void set(std::string name, glm::mat3 const &p) {
      use();
      glUniformMatrix3fv(getUniform(name.c_str()), 1, GL_FALSE, glm::value_ptr(p));
    }

    //! Set the value of a uniform variable.
    void set(std::string name, glm::mat4 const &p) {
      use();
      glUniformMatrix4fv(getUniform(name.c_str()), 1, GL_FALSE, glm::value_ptr(p));
    }

    //! Set the value of a uniform variable.
    void set(std::string name, GLfloat const &p) {
      use();
      glUniform1f(getUniform(name.c_str()), p);
    }

    //! Set the value of a uniform variable.
    void set(std::string name, GLint const &p) {
      use();
      glUniform1i(getUniform(name.c_str()), p);
    }

    //! Returns the id of the shader program.
    //! \return id of shader program
    GLuint getId() const { return shader_prog_; }

    //! Checks for success of operations.
    //! \return `true` if operations were successful, `false` otherwise.
    bool success() const { return success_; }

    //! Returns the error string (if any).
    //! \return error string, or empty string if there was no reported error.
    std::string getErrorStr() const { return error_str_; }

  private:
    GLuint shader_prog_ = 0;
    std::string name_;
    std::vector<Stage> stages_;
    bool success_;
    std::string error_str_;

    static constexpr const char *directory = "resources/shaders/";

    std::vector<GLuint> shaders_;

    bool compileShader(std::string file, GLuint type, GLuint &shader, std::string &error);

    bool linkShaders(std::vector<GLuint> const &shaders, GLuint &shaderProg, std::string &error);
};

} // namespace engine::shader
}

#endif
