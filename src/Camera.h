#ifndef CAMERA_H
#define CAMERA_H

#include "FpsMovement.h"


#include <GL/glew.h>

#include <glm/glm.hpp>

#include <functional>


class Camera {
 public:
  virtual void look(GLfloat delta_x, GLfloat delta_y) = 0;

  virtual void moveForward(GLfloat delta_frame) = 0;
  virtual void moveLeft(GLfloat delta_frame) = 0;
  virtual void moveRight(GLfloat delta_frame) = 0;
  virtual void moveBackward(GLfloat delta_frame) = 0;
  virtual void moveUp(GLfloat delta_frame) = 0;
  virtual void moveDown(GLfloat delta_frame) = 0;

  virtual glm::mat4 getViewMatrix() const = 0;
  virtual glm::vec3 getPosition() const = 0;

  virtual void setY(GLfloat y) = 0;
  virtual void setPosition(glm::vec3) = 0;
  virtual void setMovement(FpsMovement& movement) = 0;
};

class EulerCamera : public Camera {
 public:
  void look(GLfloat delta_x, GLfloat delta_y) override;

  void moveForward(GLfloat delta_frame) override;
  void moveBackward(GLfloat delta_frame) override;
  void moveLeft(GLfloat delta_frame) override;
  void moveRight(GLfloat delta_frame) override;
  void moveUp(GLfloat delta_frame) override;
  void moveDown(GLfloat delta_frame) override;

  void setY(GLfloat y) override {
    pos.y = y;
  }

  void setPosition(glm::vec3 p) override {
    pos = p;
  }

  void setMovement(FpsMovement& movement) override {
    do_mouse = [&movement](GLfloat x, GLfloat y) {
      movement.handleMouse(x,y);
    };

    do_keyboard = [&movement](bool keys[]) {
      movement.handleKeyboard(keys);
    };
  }




  glm::mat4 getViewMatrix() const;
  glm::vec3 getPosition() const {
    return pos;
  }

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
