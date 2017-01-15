#ifndef VERTEX_H
#define VERTEX_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace engine {
namespace model {

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 tex_coord;
};
} // end namespace engine::model
}
#endif
