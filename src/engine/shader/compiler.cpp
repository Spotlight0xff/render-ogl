#include "compiler.h"
#include "stages.h"
#include "Util.h"

#include <GL/glew.h>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

namespace engine {
namespace shader {


Compiler::Compiler(std::string const &shader_name, bool compile_link, Stages stages)
        : name_(shader_name),
          stages_(stages) {
  if (compile_link) {
    bool success = setup();
    if (success) {
      finalize();
    }
  }
}

Compiler::~Compiler() {
  if (shader_prog_) {
    // TODO check if deleted with glGetProgamiv
    glDeleteProgram(shader_prog_);
  }
}

bool Compiler::setup() {
  bool success;
  std::string error;

  if (shaders_.size()) {
    return true; // already setup
  }


  if (static_cast<int>(stages_ & Stages::GEOMETRY_SHADER) &&
      util::fileExists(directory + name_ + ".geom")) {
    GLuint geometry_shader;
    success = compileShader(name_ + ".geom", GL_GEOMETRY_SHADER, geometry_shader, error);
    if (!success) {
      std::cout << "[Shader::" << name_ << "] Compilation of vertex shader failed:\n"
                << error << std::endl;
      return false;
    }
    shaders_.push_back(geometry_shader);
  }

  if (static_cast<int>(stages_ & Stages::VERTEX_SHADER) &&
      util::fileExists(directory + name_ + ".vert")) {
    GLuint vertex_shader;
    success = compileShader(name_ + ".vert", GL_VERTEX_SHADER, vertex_shader, error);
    if (!success) {
      std::cout << "[Shader::" << name_ << "] Compilation of vertex shader failed:\n"
                << error << std::endl;
      return false;
    }
    shaders_.push_back(vertex_shader);
  }


  if (static_cast<int>(stages_ & Stages::FRAGMENT_SHADER) &&
      util::fileExists(directory + name_ + ".vert")) {
    GLuint fragment_shader;
    success = compileShader(name_ + ".frag", GL_FRAGMENT_SHADER, fragment_shader, error);
    if (!success) {
      std::cout << "[Shader::" << name_ << "] Compilation of fragment shader failed:\n"
                << error << std::endl;
      return false;
    }
    shaders_.push_back(fragment_shader);
  }

  return true;
}

bool Compiler::finalize() {
  std::string error;
  bool success = linkShaders(shaders_, shader_prog_, error);
  if (!success) {
    return false;
  }


  for (auto shader: shaders_) {
    glDeleteShader(shader);
  }

  std::cout << "[Shader::" << name_ << "] Compilation and linking process complete\n";
  return true;
}

void Compiler::use() const {
  GLint current;
  glGetIntegerv(GL_CURRENT_PROGRAM, &current);

  if (current != shader_prog_) {
    glUseProgram(shader_prog_);
  }
}

// TODO, just provide vertex & fragment path --> compile + link together
// TODO: check if context is active --> null exception otherwise
bool Compiler::compileShader(std::string file, GLuint type, GLuint &shader, std::string &error) {
  // first load from file
  std::string line, content;
  std::ifstream f(directory + file);
  if (!f.good()) {
    error = "File not found";
    return false;
  }
  while (std::getline(f, line)) {
    content += line + "\n";
  }
  const char *shader_source = content.c_str();

  // then create & compile shader
  shader = glCreateShader(type);
  glShaderSource(shader, 1, &shader_source, nullptr);
  glCompileShader(shader);

  // and report any errors
  GLint success;
  GLchar error_log[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, nullptr, error_log);
    error = error_log;
    return false;
  }

  return true;
}

bool Compiler::linkShaders(std::vector<GLuint> const &shaders, GLuint &shaderProg, std::string &error) {
  shaderProg = glCreateProgram();
  for (auto shader : shaders) {
    glAttachShader(shaderProg, shader);
  }
  glLinkProgram(shaderProg);

  // and report any errors
  GLint success;
  GLchar error_log[512];
  glGetProgramiv(shaderProg, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProg, 512, nullptr, error_log);
    error = "Linking: " + std::string(error_log);
    return false;
  }

  glValidateProgram(shader_prog_);

  glGetProgramiv(shader_prog_, GL_VALIDATE_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shader_prog_, 512, nullptr, error_log);
    error = "Validating: " + std::string(error_log);
    return false;
  }

  std::cout << "Linked " << shaders.size() << " shaders successfully.\n";
  return true;
}

} // end namespace engine::shader
}
