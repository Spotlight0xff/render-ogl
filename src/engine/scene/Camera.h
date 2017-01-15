#ifndef CAMERA_H
#define CAMERA_H

#include "FpsMovement.h"

#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <functional>

namespace engine {
namespace scene {

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

    virtual void setMovement(FpsMovement &movement) = 0;
};

} // end namespace engine::scene
}
#endif
