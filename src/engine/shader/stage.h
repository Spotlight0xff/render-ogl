#ifndef GRAPHICS_ENGINE_SHADER_STAGE_H
#define GRAPHICS_ENGINE_SHADER_STAGE_H

#include <type_traits>

namespace engine {
namespace shader {


/*!
 * Stages of a shader program.
 *
 * This enumeration class helps us to specify the needed shaders to load
 * Note: Tessellation and Compute shader are currently not supported
 */
enum class Stage : int {
    GEOMETRY_SHADER = 1 << 0,
    VERTEX_SHADER   = 1 << 1,
    FRAGMENT_SHADER = 1 << 2,
};

using T = std::underlying_type_t <Stage>;

inline Stage operator | (Stage lhs, Stage rhs)
{
  return static_cast<Stage>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

inline Stage& operator |= (Stage& lhs, Stage rhs)
{
  lhs = static_cast<Stage>(static_cast<T>(lhs) | static_cast<T>(rhs));
  return lhs;
}

inline Stage operator & (Stage lhs, Stage rhs)
{
  return static_cast<Stage>(static_cast<T>(lhs) & static_cast<T>(rhs));
}


}
}
#endif //GRAPHICS_ENGINE_SHADER_STAGE_H
