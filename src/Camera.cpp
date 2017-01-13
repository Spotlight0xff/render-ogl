#include "Camera.h"


#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream> // DEBUG

void EulerCamera::look(GLfloat delta_x, GLfloat delta_y) {
    delta_x *= mouse_sens;
    delta_y *= mouse_sens;

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


void EulerCamera::moveForward(GLfloat delta_frame) {
  GLfloat speed = movement_speed * delta_frame;
  pos += speed * front;
}

void EulerCamera::moveBackward(GLfloat delta_frame) {
  GLfloat speed = movement_speed * delta_frame;
  pos -= speed * front;
}

void EulerCamera::moveLeft(GLfloat delta_frame) {
  GLfloat speed = movement_speed * delta_frame;
  glm::vec3 right = glm::normalize(glm::cross(front, up));
  pos -= speed * right;
}

void EulerCamera::moveRight(GLfloat delta_frame) {
  GLfloat speed = movement_speed * delta_frame;
  glm::vec3 right = glm::normalize(glm::cross(front, up));
  pos += speed * right;
}

void EulerCamera::moveUp(GLfloat delta_frame) {
  GLfloat speed = movement_speed * delta_frame;
  pos += speed * up;
}

void EulerCamera::moveDown(GLfloat delta_frame) {
  GLfloat speed = movement_speed * delta_frame;
  pos -= speed * up;
}



inline glm::mat4 EulerCamera::getViewMatrix() const{
  return glm::lookAt(pos, pos + front, up);
}
