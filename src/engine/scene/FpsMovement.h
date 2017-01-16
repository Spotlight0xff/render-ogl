#ifndef FPS_MOVEMENT
#define FPS_MOVEMENT

#define DEFAULT_SENSITIVITY 0.1f
#define DEFAULT_MOVEMENT_SPEED 5.0f

#include "opengl.h"

namespace engine {
namespace scene {

class Camera;

class FpsMovement {
  public:
    FpsMovement(Camera *cam)
            : camera(cam) {
    }

    void handleKeyboard(bool keys[]);

    void handleMouse(GLfloat x, GLfloat y);

    void handleDelta();

    void setEyelevel(GLfloat el) {
      default_eyelevel = el;
      eyelevel = el;
    }

    void setSensitivity(GLfloat sens) { sensitivity = sens; }

    void setSpeed(GLfloat sp) { default_speed = sp; }

  private:
    bool ignore_first = true;
    Camera *camera;

    // movement-related

    // general
    static constexpr GLfloat gravity = 0.891f;
    GLfloat sensitivity = DEFAULT_SENSITIVITY;
    GLfloat default_speed = DEFAULT_MOVEMENT_SPEED;
    GLfloat speed = default_speed;
    GLfloat shift_modifier = 2.0f;


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
