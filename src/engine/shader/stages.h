//
// Created by spotlight on 1/19/17.
//

#ifndef GRAPHICS_ENGINE_STAGES_H
#define GRAPHICS_ENGINE_STAGES_H

#include <type_traits>

namespace engine {
namespace shader {


// Tesselation and compute shader are current not supported.
enum class Stages : int {
    GEOMETRY_SHADER = 1 << 0,
    VERTEX_SHADER   = 1 << 1,
    FRAGMENT_SHADER = 1 << 2,
};

using T = std::underlying_type_t <Stages>;

inline Stages operator | (Stages lhs, Stages rhs)
{
  return static_cast<Stages>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

inline Stages& operator |= (Stages& lhs, Stages rhs)
{
  lhs = static_cast<Stages>(static_cast<T>(lhs) | static_cast<T>(rhs));
  return lhs;
}

inline Stages operator & (Stages lhs, Stages rhs)
{
  return static_cast<Stages>(static_cast<T>(lhs) & static_cast<T>(rhs));
}

}
}
#endif //GRAPHICS_ENGINE_STAGES_H
