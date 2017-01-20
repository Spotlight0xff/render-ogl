//
// Created by spotlight on 1/19/17.
//

#include "RandomTexture.h"

#include <cstdlib>

namespace {

float getRandomFloat()
{
  return static_cast<float>(random() / (float) RAND_MAX);
}
}

namespace engine {
namespace particles {

RandomTexture::RandomTexture(size_t size)
  : size_(size) {
  glm::vec3* random_data = new glm::vec3[size];
  for (unsigned int i = 0 ; i < size ; i++) {
    random_data[i].x = getRandomFloat();
    random_data[i].y = getRandomFloat();
    random_data[i].z = getRandomFloat();
  }

  glGenTextures(1, &id_);
  glBindTexture(GL_TEXTURE_1D, id_);
  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, size, 0.0f, GL_RGB, GL_FLOAT, random_data);
  glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);

  delete [] random_data;
}
}
}
