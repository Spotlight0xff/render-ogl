#include "PhongLight.h"


namespace engine {
namespace components {


void PhongLight::draw(Scene& scene) {
  model.draw();
}

void PhongLight::drawModel() {
  model.draw();
}

glm::mat4 PhongLight::getModelMatrix() {
  return glm::translate(glm::mat4(1.0f), position);
}

}
}
