#ifndef FPS_MOVEMENT
#define FPS_MOVEMENT

#define DEFAULT_SENSITIVITY 0.1f
#define DEFAULT_MOVEMENT_SPEED 5.0f

#include <GL/glew.h>

class Camera;

class FpsMovement {
 public:
  FpsMovement(Camera* cam)
    : camera(cam) {
    }

  void handleKeyboard(bool keys[]);
  void handleMouse(GLfloat x, GLfloat y);
  void handleDelta();

  void setEyelevel(GLfloat el) { default_eyelevel = el; eyelevel = el; }
  void setSensitivity(GLfloat sens) { sensitivity = sens; }
  void setSpeed(GLfloat sp) { default_speed = sp; }

 private:
  bool ignore_first = true;
  Camera* camera;

  // movement-related
  GLfloat jump_force = 0.0f;
  GLfloat default_eyelevel = 0.0f;
  GLfloat eyelevel = default_eyelevel;
  GLfloat duck_offset = 3.0f;
  GLfloat sensitivity = DEFAULT_SENSITIVITY;
  GLfloat default_speed = DEFAULT_MOVEMENT_SPEED;
  GLfloat speed = default_speed;
  GLfloat shift_modifier = 2.0f;
  static constexpr GLfloat gravity = 0.0891f;
  static constexpr GLfloat inital_force = 40.0f;


  // technical auxiliaries
  GLfloat delta_frame = 0.0f;
  GLfloat last_frame = 0.0f;
  GLfloat last_x = 0.0f;
  GLfloat last_y = 0.0f;
};

#endif
