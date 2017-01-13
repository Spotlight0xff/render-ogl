#ifndef EULERCAMERA_H
#define EULERCAMERA_H

#include "Camera.h"
#include "FpsMovement.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <functional>

class EulerCamera : public Camera {
 public:
  void look(GLfloat delta_x, GLfloat delta_y) override;

  void moveForward(GLfloat delta_frame) override;
  void moveBackward(GLfloat delta_frame) override;
  void moveLeft(GLfloat delta_frame) override;
  void moveRight(GLfloat delta_frame) override;
  void moveUp(GLfloat delta_frame) override;
  void moveDown(GLfloat delta_frame) override;

  void setY(GLfloat y) override { pos.y = y; }

  void setPosition(glm::vec3 p) override { pos = p; }

  void setMovement(FpsMovement& movement) override;




  glm::mat4 getViewMatrix() const;
  glm::vec3 getPosition() const { return pos; }

  std::function<void(bool keys[])> do_keyboard;
  std::function<void(GLfloat, GLfloat)> do_mouse;

 private:
  GLfloat pitch = 0.0;
  GLfloat yaw = -90.0;

  glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

};

#endif
