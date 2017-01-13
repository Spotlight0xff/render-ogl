#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>

enum class TextureType {
  UNKNOWN,
  DIFFUSE,
  SPECULAR
};

struct Texture {
  GLuint id;
  TextureType type;
  std::string path;
  float shininess;
  glm::vec3 diffuse = glm::vec3(0.5f);
  glm::vec3 ambient = glm::vec3(0.3f);
  glm::vec3 specular = glm::vec3(1.0f);
};
#endif
