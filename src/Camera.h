#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>

#include <glm/glm.hpp>

#define DEFAULT_MOUSE_SENS 0.1f
#define DEFAULT_MOVEMENT_SPEED 5.0f

class Camera {
 public:
  virtual void look(GLfloat delta_x, GLfloat delta_y) = 0;

  virtual void moveForward(GLfloat delta_frame) = 0;
  virtual void moveLeft(GLfloat delta_frame) = 0;
  virtual void moveRight(GLfloat delta_frame) = 0;
  virtual void moveBackward(GLfloat delta_frame) = 0;

  virtual glm::mat4 getViewMatrix() const = 0;
  virtual glm::vec3 getPosition() const = 0;

  void setMouseSensitivity(GLfloat sensitivity) {
    mouse_sens = sensitivity;
  }

  GLfloat getMouseSensitivitiy() const {
    return mouse_sens;
  }

  void setMovementSpeed(GLfloat speed) {
    movement_speed = speed;
  }

  GLfloat getMovementSpeed() const {
    return movement_speed;
  }


 protected:
  GLfloat mouse_sens = DEFAULT_MOUSE_SENS;
  GLfloat movement_speed = DEFAULT_MOVEMENT_SPEED;
};

class EulerCamera : public Camera {
 public:
  void look(GLfloat delta_x, GLfloat delta_y);

  void moveForward(GLfloat delta_frame);
  void moveBackward(GLfloat delta_frame);
  void moveLeft(GLfloat delta_frame);
  void moveRight(GLfloat delta_frame);
  void moveUp(GLfloat delta_frame);
  void moveDown(GLfloat delta_frame);

  void setY(GLfloat y) {
    pos.y = y;
  }

  void setPosition(glm::vec3 p) {
    pos = p;
  }


  glm::mat4 getViewMatrix() const;
  glm::vec3 getPosition() const {
    return pos;
  }

 public: // debug
  GLfloat pitch = 0.0;
  GLfloat yaw = -90.0;

  glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
};
#endif
