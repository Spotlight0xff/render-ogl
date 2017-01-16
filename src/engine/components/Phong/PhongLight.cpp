#include "PhongLight.h"


namespace engine {
namespace components {


void PhongLight::draw(Scene& scene) {
  shader.use();
  engine::scene::Camera const &camera = scene.getCameraRef();

  // Model-View-Projection
  glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE,
                     glm::value_ptr(getModelMatrix()));
  glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "projection"), 1, GL_FALSE,
                     glm::value_ptr(scene.getProjectionMatrix()));
  glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "view"), 1, GL_FALSE,
                     glm::value_ptr(camera.getViewMatrix()));

  model.draw(shader);
}

glm::mat4 PhongLight::getModelMatrix() {
  return glm::translate(glm::mat4(1.0f), position);
}

}
}
