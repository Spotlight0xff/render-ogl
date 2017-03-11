#include "EulerCamera.h"
#include "FpsMovement.h"

#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

namespace engine {
namespace scene {

void EulerCamera::look(GLfloat delta_x, GLfloat delta_y) {
  yaw += delta_x;
  pitch += delta_y;

  if (pitch > 89.0f) {
    pitch = 89.0f;
  } else if (pitch < -89.0f) {
    pitch = -89.0f;
  }
  glm::vec3 temp_front;
  // TODO: replace with glm::rotate
  temp_front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
  temp_front.y = sin(glm::radians(pitch));
  temp_front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
  this->front = glm::normalize(temp_front);

  updateMatrices();
}



void EulerCamera::moveForward(GLfloat delta) {
  pos += delta * front;
  updateMatrices();
}

void EulerCamera::moveBackward(GLfloat delta) {
  pos -= delta * front;
  updateMatrices();
}

void EulerCamera::moveLeft(GLfloat delta) {
  glm::vec3 right = glm::normalize(glm::cross(front, up));
  pos -= delta * right;
  updateMatrices();
}

void EulerCamera::moveRight(GLfloat delta) {
  glm::vec3 right = glm::normalize(glm::cross(front, up));
  pos += delta * right;
  updateMatrices();
}

void EulerCamera::moveUp(GLfloat delta) {
  pos += delta * up;
  updateMatrices();
}

void EulerCamera::moveDown(GLfloat delta) {
  pos -= delta * up;
  updateMatrices();
}

void EulerCamera::move(glm::vec3 delta) {
  // TODO(André): optimize that shit
  glm::vec3 right = glm::normalize(glm::cross(front, up));
  pos += (delta[0] * front);
  pos +=  delta[1] * right;
  updateMatrices();
}

void EulerCamera::lookAt(glm::vec3 p) {
  view_matrix = glm::lookAt(pos, p + front, up);
}


void EulerCamera::updateMatrices() {
  view_matrix = glm::lookAt(pos, pos + front, up);
}

} // end namespace engine::scene
}
