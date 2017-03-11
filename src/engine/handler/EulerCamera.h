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

//! @brief 3D camera implementation using Euler angles
class EulerCamera : public Camera {
  public:
    /*!
     * Processes mouse movement.
     *
     * @param delta_x change in x-axis
     * @param delta_y change in y-axis
     */
    void look(GLfloat delta_x, GLfloat delta_y) override;


    /*!
     * Moves camera forward in current direction.
     *
     * @param delta_frame time since last call
     */
    void moveForward(GLfloat delta_frame) override;


    /*!
     * Moves camera backwards.
     *
     * @param delta_frame time since last call
     */
    void moveBackward(GLfloat delta_frame) override;


    /*!
     * Moves camera to the left.
     *
     * @param delta_frame time since last call
     */
    void moveLeft(GLfloat delta_frame) override;


    /*!
     * Moves camera to the right.
     *
     * @param delta_frame time since last call
     */
    void moveRight(GLfloat delta_frame) override;


    /*!
     * Moves camera upwards.
     *
     * @param delta_frame time since last call
     */
    void moveUp(GLfloat delta_frame) override;


    /*!
     * Moves camera downwards.
     *
     * @param delta_frame time since last call
     */
    void moveDown(GLfloat delta_frame) override;


    /*!
     * Sets position on y-axis.
     *
     * @param y value to set to
     */
    void setY(GLfloat y) override { pos.y = y; }


    /*!
     * Sets a absolute position.
     *
     * @param p position
     */
    void setPosition(glm::vec3 p) override { pos = p; updateMatrices(); }

    /*!
     * Rotates the camera so that it looks at the given position.
     *
     * @param pos position to look at
     */
    void lookAt(glm::vec3 pos);

    /*!
     * Move a relative position.
     *
     * @param delta relative position
     */
    void move(glm::vec3 delta);


    /*!
     * returns the 4x4 view matrix.
     *
     * @return view matrix
     */
	glm::mat4 getViewMatrix() const override { return view_matrix; }
	
    /*!
     * returns the current positon of the camera.
     *
     * @return position
     */
    glm::vec3 getPosition() const override { return pos; }

    std::function<void(bool keys[])> do_keyboard;
    std::function<void(GLfloat, GLfloat)> do_mouse;

  private:
    GLfloat pitch = 0.0f;
    GLfloat yaw = -90.0f;

    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 view_matrix{ 0 };

    void updateMatrices();

};

} // end engine::model
}

#endif
