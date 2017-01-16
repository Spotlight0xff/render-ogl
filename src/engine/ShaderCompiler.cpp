#include "ShaderCompiler.h"

#include <GL/glew.h>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

namespace engine {

Shader::Shader(std::string const &shader_name)
        : name(shader_name) {
  bool success;
  std::string error;
  std::vector<GLuint> shaders;

  success = compileShader(shader_name + ".vert", GL_VERTEX_SHADER, vertex_shader, error);
  if (!success) {
    std::cout << "[Shader::" << shader_name << "] Compilation of vertex shader failed:\n"
              << error << std::endl;
    return;
  }
  success = compileShader(shader_name + ".frag", GL_FRAGMENT_SHADER, fragment_shader, error);
  if (!success) {
    std::cout << "[Shader::" << shader_name << "] Compilation of fragment shader failed:\n"
              << error << std::endl;
    return;
  }

  shaders.push_back(vertex_shader);
  shaders.push_back(fragment_shader);
  linkShaders(shaders, shader_prog, error);

  for (auto shader: shaders) {
    glDeleteShader(shader);
  }

  std::cout << "[Shader::" << shader_name << "] Compilation and linking process complete\n";
}

void Shader::use() const {
  GLint current;
  glGetIntegerv (GL_CURRENT_PROGRAM, &current);

  if (current != shader_prog) {
    glUseProgram(shader_prog);
  }
}

// TODO, just provide vertex & fragment path --> compile + link together
// TODO: check if context is active --> null exception otherwise
bool Shader::compileShader(std::string file, GLuint type, GLuint &shader, std::string &error) {
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

bool Shader::linkShaders(std::vector<GLuint> const &shaders, GLuint &shaderProg, std::string &error) {
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
    error = error_log;
    return false;
  }

  std::cout << "Linked " << shaders.size() << " shaders successfully.\n";
  return true;
}

} // end namespace engine
