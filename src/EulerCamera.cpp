#include "EulerCamera.h"
#include "FpsMovement.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

void EulerCamera::setMovement(FpsMovement& movement) {
  do_mouse = [&movement](GLfloat x, GLfloat y) {
    movement.handleMouse(x,y);
  };

  do_keyboard = [&movement](bool keys[]) {
    movement.handleKeyboard(keys);
  };
}

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
}


void EulerCamera::moveForward(GLfloat delta) {
  pos += delta * front;
}

void EulerCamera::moveBackward(GLfloat delta) {
  pos -= delta * front;
}

void EulerCamera::moveLeft(GLfloat delta) {
  glm::vec3 right = glm::normalize(glm::cross(front, up));
  pos -= delta * right;
}

void EulerCamera::moveRight(GLfloat delta) {
  glm::vec3 right = glm::normalize(glm::cross(front, up));
  pos += delta * right;
}

void EulerCamera::moveUp(GLfloat delta) {
  pos += delta * up;
}

void EulerCamera::moveDown(GLfloat delta) {
  pos -= delta * up;
}


inline glm::mat4 EulerCamera::getViewMatrix() const{
  return glm::lookAt(pos, pos + front, up);
}
