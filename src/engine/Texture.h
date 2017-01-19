#ifndef TEXTURE_H
#define TEXTURE_H

#include "opengl.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <string>

namespace engine {

class Texture {
  public:
    Texture(GLenum target = GL_TEXTURE_2D)
            : target_(target) {}

    void bind(GLenum unit) const{
      glActiveTexture(unit);
      glBindTexture(target_, id_);
    }

    void setId(GLuint id) { id_ = id; }


  protected:
    GLuint id_;
    GLenum target_;
};

} // end namespace engine
#endif
