#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

enum class TextureType {
  DIFFUSE,
  SPECULAR
};

struct Texture {
  GLuint id;
  TextureType type;
};
#endif
