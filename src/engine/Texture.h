#ifndef TEXTURE_H
#define TEXTURE_H

#include "opengl.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <string>

namespace engine {

//! @brief Generic texture abstraction
class Texture {
  public:
    Texture(GLenum target = GL_TEXTURE_2D)
            : target_(target) {}

    virtual ~Texture() {
      if (id_ != 0) {
        glDeleteTextures(1, &id_);
      }
    }

    //! Binds this particular texture to the texture unit.
    //! \param unit texture unit
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
