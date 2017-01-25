#ifndef GRAPHICS_ENGINE_ENGINE_H
#define GRAPHICS_ENGINE_ENGINE_H

#include "engine/handler/InputHandler.h"

#include "opengl.h"

#include <string>
#include <engine/handler/FpsMovement.h>
#include <engine/handler/EulerCamera.h>
#include <memory>


namespace engine {

class Engine {
  public:
    struct Options {
      bool debug = true;
      bool show_cursor = true;
      bool fullscreen = false;
      unsigned int default_width = 1280;
      unsigned int default_height = 1024;
      unsigned int samples = 4;
      unsigned int opengl_major = 4;
      unsigned int opengl_minor = 3;
      std::string window_name = "Engine";
    };

    enum class Handler {
        FPS_MOVEMENT = 0
    };

    using CursorPosCallbackFunc = std::function<void(float, int, int)>;
    using KeyboardCallbackFunc = std::function<void(float, bool[1024])>;
    using WindowSizeCallbackFunc = std::function<void(int, int)>;

    Engine();
    ~Engine();


    void SetOptions(struct Options& options);

    void SetCursorPosCallback(CursorPosCallbackFunc func) {
        *cursor_pos_func_ = func;
    }

    void SetKeyboardCallback(KeyboardCallbackFunc func) {
        *keyboard_func_ = func;
    }

    void SetWindowSizeCallback(WindowSizeCallbackFunc func) {
        *window_size_func_ = func;
    }

    // static callback funcs
    static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);

    // Initializes GL and all subsystems
    bool Init();

    // Runs the mainloop
    void Run();

    void HandleTime();

    void Render();

  private:
    struct Options options_;
    bool initialized_ = 0;
    GLFWwindow *window_ = nullptr;

    // Input handling
    std::unique_ptr<CursorPosCallbackFunc> cursor_pos_func_{nullptr};
    std::unique_ptr<KeyboardCallbackFunc> keyboard_func_{nullptr};
    std::unique_ptr<WindowSizeCallbackFunc> window_size_func_{nullptr};



    // Camera handling

    float delta_time_ = 0;
    float last_time_ = 0;

    void InitGL();
};

}


#endif //GRAPHICS_ENGINE_ENGINE_H
