#ifndef FPS_MOVEMENT
#define FPS_MOVEMENT

#define DEFAULT_SENSITIVITY 0.1f
#define DEFAULT_MOVEMENT_SPEED 5.0f

#include <glm/vec3.hpp>
#include <iostream>
#include "opengl.h"
#include "MouseHandler.h"
#include "KeyboardHandler.h"
#include "FrameHandler.h"
//#include "CameraHandler.h"
#include "EulerCamera.h"

namespace engine {
namespace handler {

class FpsMovement : public KeyboardHandler, public MouseHandler, public FrameHandler {// : public MouseHandler, public KeyboardHandler, public CameraHandler {
  public:

    FpsMovement(engine::scene::EulerCamera* camera) {
        this->camera = camera;
    }

    ~FpsMovement() {}

    void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
      if(action == GLFW_PRESS)
        keys_[key] = true;
      else if(action == GLFW_RELEASE)
        keys_[key] = false;
    }

    void FrameCallback(float delta_time) {
      handleKeyboard(keys_);
    }

    void SetWindow(GLFWwindow* window) {
      window_ = window;
    }

    void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)  {
      handleMouse(xpos, ypos);
    }

    void handleKeyboard(bool keys[]);

    void handleMouse(GLfloat x, GLfloat y);

    void handleDelta();

    void setEyelevel(GLfloat el) {
      default_eyelevel = el;
      eyelevel = el;
      camera->setY(eyelevel);
    }

    void setSensitivity(GLfloat sens) { sensitivity = sens; }

    void setSpeed(GLfloat sp) { default_speed = sp; }


  private:
    bool ignore_first = true;
    engine::scene::EulerCamera *camera;
    GLFWwindow* window_ = nullptr;

    // input
    bool keys_[1024] = {};


    // movement-related
    static constexpr GLfloat max_speed = 20.0f;

    // general
    static constexpr GLfloat gravity = 0.891f;
    GLfloat sensitivity = DEFAULT_SENSITIVITY;
    GLfloat default_speed = DEFAULT_MOVEMENT_SPEED;
    GLfloat speed = default_speed;
    GLfloat shift_modifier = 2.0f;

    // movement
    glm::vec3 velocity;


    // jumping
    GLfloat jump_speed = 0.0f;
    bool jumping = false;
    static constexpr GLfloat initial_force = 50.0f;

    // ducking
    GLfloat duck_offset = 3.0f;

    // eyelevel
    GLfloat default_eyelevel = 0.0f;
    GLfloat eyelevel = default_eyelevel;

    // technical auxiliaries
    GLfloat delta_frame = 0.0f;
    GLfloat last_frame = 0.0f;
    GLfloat last_x = 0.0f;
    GLfloat last_y = 0.0f;
};

} // end namespace engine::scene
}

#endif
