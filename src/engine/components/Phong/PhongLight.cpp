#include "PhongLight.h"


namespace engine {
namespace components {


/*void PhongLight::draw(Scene& scene) {
  model.draw();
}*/

void PhongLight::drawModel() {
  if (render_object_) {
    model.draw();
  }
}

void PhongLight::renderObject(bool render_object) {
  render_object_ = render_object;
}

glm::mat4 PhongLight::getModelMatrix() {
  return glm::translate(glm::mat4(1.0f), position);
}

}
}
