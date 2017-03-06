#ifndef GRAPHICS_ENGINE_ENGINE_H
#define GRAPHICS_ENGINE_ENGINE_H

#include "engine/handler/InputHandler.h"

#include "opengl.h"
#include "Scene.h"

#include <string>
#include <engine/handler/FpsMovement.h>
#include <engine/handler/EulerCamera.h>
#include <memory>
#include <engine/scene/Phong.h>


namespace engine {

class Engine {
  public:
    struct Options {
      bool debug = true;
      bool show_cursor = true;
      bool fullscreen = false;
      unsigned int default_width = 1280;
      unsigned int default_height = 1024;
      unsigned int samples = 1;
      unsigned int opengl_major = 3;
      unsigned int opengl_minor = 3;
      std::string window_name = "Engine";
    };

    using CursorPosCallbackFunc = std::function<void(float, int, int)>;
    using KeyboardCallbackFunc = std::function<void(float, bool[1024])>;
    using WindowSizeCallbackFunc = std::function<void(int, int)>;

    Engine();
    ~Engine();


    void SetOptions(Options& options);

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
    static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    //static void WindowSizeCallback(GLFWwindow* window, int width, int height);

    void SetKeyboardHandler(engine::handler::KeyboardHandler* handler) {
        keyboard_handler_ = handler;
    }

    void SetMouseHandler(engine::handler::MouseHandler* handler) {
        mouse_handler_ = handler;
    }

    void SetFrameHandler(engine::handler::FrameHandler* handler) {
        frame_handler_ = handler;
        frame_handler_->SetWindow(window_);
    }

    //void SetWindowHandler(engine::handler::WindowHandler handler) {
    //    *window_handler_ = handler;
    //}

    // Initializes GL and all subsystems
    bool Init();

    // Runs the mainloop
    void Run();

    void HandleTime();

    void Render();

    void LoadScene(std::unique_ptr<engine::scene::Phong>& scene) {
      scenes_.push_back(std::move(scene));
    }


    template<typename T>
    void LoadObject(std::string const& path) {


    }

  private:
    struct Options options_;
    bool initialized_ = 0;
    GLFWwindow *window_ = nullptr;

    // Input handling
    std::unique_ptr<CursorPosCallbackFunc> cursor_pos_func_{nullptr};
    std::unique_ptr<KeyboardCallbackFunc> keyboard_func_{nullptr};
    std::unique_ptr<WindowSizeCallbackFunc> window_size_func_{nullptr};

    engine::handler::KeyboardHandler* keyboard_handler_ = nullptr;
    engine::handler::MouseHandler* mouse_handler_ = nullptr;
    engine::handler::FrameHandler* frame_handler_ = nullptr;

    std::vector<std::unique_ptr<scene::Phong>> scenes_;
    //engine::handler::WindowHandler* window_handler_ = nullptr;



    // Camera handling

    float delta_time_ = 0;
    float last_time_ = 0;

    void InitGL();
};

}


#endif //GRAPHICS_ENGINE_ENGINE_H
