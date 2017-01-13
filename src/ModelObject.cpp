#include "ModelObject.h"

#include <glm/gtc/matrix_transform.hpp>

void ModelObject::setPosition(glm::vec3 pos) {
  position = pos;
}

void ModelObject::setScale(glm::vec3 s) {
  scale = s;
}

glm::vec3 ModelObject::getPosition() const {
  return position;
}

glm::mat4 ModelObject::getModelMatrix() {
  glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
  return glm::scale(model, scale);
}
