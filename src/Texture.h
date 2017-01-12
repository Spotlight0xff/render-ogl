#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

#include <string>

enum class TextureType {
  DIFFUSE,
  SPECULAR
};

struct Texture {
  GLuint id;
  TextureType type;
  std::string path;
};
#endif
