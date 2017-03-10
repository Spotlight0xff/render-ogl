//
// Created by spotlight on 1/19/17.
//

#ifndef GRAPHICS_ENGINE_RANDOMTEXTURE_H
#define GRAPHICS_ENGINE_RANDOMTEXTURE_H


#include "engine/Texture.h"

namespace engine {
namespace particles {

//! @brief Texure containing random data to use in particle simulation
class RandomTexture : public Texture {
  public:
    RandomTexture(size_t size);

  private:
    GLsizei size_;
};

} // end namespace engine::particles
}


#endif //GRAPHICS_ENGINE_RANDOMTEXTURE_H
