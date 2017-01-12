#include "ShaderCompiler.h"

#include <GL/glew.h>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

Shader::Shader(std::string const& path_vertex, std::string const& path_fragment) {
  bool success;
  std::string error;
  std::vector<GLuint> shaders;

  success = compileShader(path_vertex.c_str(), GL_VERTEX_SHADER, vertex_shader, error);
  if (!success) {
    return;
  }
  success = compileShader(path_fragment.c_str(), GL_FRAGMENT_SHADER, fragment_shader, error);
  if (!success) {
    std::cerr << "failed: " << error << std::endl;
    return;
  }

  shaders.push_back(vertex_shader);
  shaders.push_back(fragment_shader);
  linkShaders(shaders, shader_prog, error);

  for (auto shader: shaders) {
    glDeleteShader(shader);
  }
}

void Shader::use() const{
  glUseProgram(shader_prog);
}

// TODO, just provide vertex & fragment path --> compile + link together
// TODO: check if context is active --> null exception otherwise
bool Shader::compileShader(const char* path, GLuint type, GLuint& shader, std::string& error) {
  // first load from file
  std::string line, content;
  std::ifstream f(path);
  if (!f.good()) {
    error = "File not found";
    return false;
  }
  while(std::getline(f, line)) {
    content += line + "\n";
  }
  const char* shader_source = content.c_str();

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
    std::cerr << "Failed shader: " << error_log << std::endl;
    error = error_log;
    return false;
  }

  std::cout << "Compiler shader '" << path << "' successfully.\n";
  return true;
}

bool Shader::linkShaders(std::vector<GLuint> const& shaders, GLuint& shaderProg, std::string& error) {
  shaderProg = glCreateProgram();
  for(auto shader : shaders) {
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
