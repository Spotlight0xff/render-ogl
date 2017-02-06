#ifndef EULERCAMERA_H
#define EULERCAMERA_H

#include "Camera.h"
//#include "FpsMovement.h"

#include "opengl.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <functional>

namespace engine {
namespace scene {

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

    void setPosition(glm::vec3 p) override { pos = p; updateMatrices(); }

    //void setMovement(engine::handler::FpsMovement &movement);

    void lookAt(glm::vec3 pos);

    void move(glm::vec3 delta);


    glm::mat4 getViewMatrix() const override;

    glm::vec3 getPosition() const override { return pos; }

    std::function<void(bool keys[])> do_keyboard;
    std::function<void(GLfloat, GLfloat)> do_mouse;

  private:
    GLfloat pitch = 0.0f;
    GLfloat yaw = -90.0f;

    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 view_matrix;

    void updateMatrices();

};

} // end engine::model
}

#endif
