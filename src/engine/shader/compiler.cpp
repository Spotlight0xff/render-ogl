#include "compiler.h"
#include "stage.h"
#include "Util.h"

#include <GL/glew.h>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

namespace engine {
namespace shader {

Compiler::Compiler(std::string const &shader_name, bool compile_link, std::vector<Stage> stages)
        : name_(shader_name),
          stages_(stages) {
  if (compile_link) {
    success_ = setup();
    if (!success_) {
      return;
    }
    success_ = finalize();
    if (!success_) {
      return;
    }
  }
}

Compiler::~Compiler() {
  glDeleteProgram(shader_prog_);
  for (auto shader: shaders_) {
    glDeleteShader(shader);
  }
}

bool Compiler::setup() {
  std::string error;
  if (shaders_.size()) {
    return true; // already setup
  }


  // Compile all required shaders
  for(auto const& stage : stages_) {
    std::string extension;
    std::string shader_name;
    GLuint shader_type;

    switch(stage) {
      case Stage::GEOMETRY_SHADER:
        extension = ".geom";
        shader_name = "Geometry";
        shader_type = GL_GEOMETRY_SHADER;
        break;

      case Stage::VERTEX_SHADER:
        extension = ".vert";
        shader_name = "Vertex";
        shader_type = GL_VERTEX_SHADER;
        break;

      case Stage::FRAGMENT_SHADER:
        extension = ".frag";
        shader_name = "Fragment";
        shader_type = GL_FRAGMENT_SHADER;
        break;
    }

    std::string path = directory + name_ + extension;
    success_ = util::fileExists(path);
    if (!success_) {
      error_str_ = "[Shader::" + name_ + "] " + shader_name
              + " shader required but not found.";
      return false;
    }

    GLuint shader_id;
    std::string error;
    success_ = compileShader(path, shader_type, shader_id, error);
    if (!success_) {
      error_str_ = "[Shader::" + name_ + "] Compilation of " + shader_name
                   + " shader failed: " + error;
      return false;
    }
    shaders_.push_back(shader_id);
  }
  success_ = true;
  return true;
}

bool Compiler::finalize() {
  std::string error;
  success_ = linkShaders(shaders_, shader_prog_, error);
  if (!success_) {
    error_str_ = "[Shader::" + name_ + "] Linking process failed: " + error;
    return false;
  }


  for (auto shader: shaders_) {
    glDeleteShader(shader);
  }
  shaders_.clear();

  success_ = true;
  return true;
}

void Compiler::use() const {
  GLint current;
  glGetIntegerv(GL_CURRENT_PROGRAM, &current);

  if (current != shader_prog_) {
    glUseProgram(shader_prog_);
  }
}

bool Compiler::compileShader(std::string path, GLuint type, GLuint &shader, std::string &error) {
  // first load from file
  std::string line, content;
  std::ifstream f(path);
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

  return true;
}

} // end namespace engine::shader
}
