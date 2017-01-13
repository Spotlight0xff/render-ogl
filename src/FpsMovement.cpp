#include "FpsMovement.h"
#include "Camera.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

void FpsMovement::handleKeyboard(bool keys[]) {
  handleDelta();
  if (keys[GLFW_KEY_W]) {
    camera->moveForward(delta_frame * speed);
  }
  if (keys[GLFW_KEY_S]) {
    camera->moveBackward(delta_frame * speed);
  }
  if (keys[GLFW_KEY_A]) {
    camera->moveLeft(delta_frame * speed);
  }
  if (keys[GLFW_KEY_D]) {
    camera->moveRight(delta_frame * speed);
  }
  if (keys[GLFW_KEY_LEFT_SHIFT]) {
    // TODO: just apply the modifier
    speed = shift_modifier * default_speed;
  } else {
    speed = default_speed;
  }
  if (keys[GLFW_KEY_SPACE]) {
    if (jump_force == 0.0f) {
      jump_force = inital_force;
      camera->setY(eyelevel);
    }
  }
  if (keys[GLFW_KEY_H]) {
    camera->moveDown(delta_frame * speed);
  }
  if (camera->getPosition().y >= eyelevel) {
    GLfloat net_force = jump_force - gravity;
    camera->moveUp(net_force * delta_frame);
    jump_force -= gravity;
  }
  if (camera->getPosition().y < eyelevel) {
    jump_force = 0.0f;
    camera->setY(eyelevel);
  }
}

void FpsMovement::handleMouse(GLfloat x, GLfloat y) {
  GLfloat delta_x = GLfloat(x) - last_x;
  GLfloat delta_y = last_y - GLfloat(y);
  last_x = GLfloat(x);
  last_y = GLfloat(y);

  // first delta is high, so ignore the first frame
  if (ignore_first) {
    ignore_first = false;
    return;
  }
  camera->look(delta_x * sensitivity, delta_y * sensitivity);
}

void FpsMovement::handleDelta() {
  GLfloat current_time = glfwGetTime();
  delta_frame = current_time - last_frame;
  last_frame = current_time;
}
