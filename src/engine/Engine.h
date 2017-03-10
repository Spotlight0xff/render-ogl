#ifndef GRAPHICS_ENGINE_ENGINE_H
#define GRAPHICS_ENGINE_ENGINE_H

#include "opengl.h"
#include "Scene.h"
#include "Manager.h"

#include <string>
#include <engine/handler/FpsMovement.h>
#include <engine/handler/EulerCamera.h>
#include <memory>


namespace engine {

//! @brief Rendering engine
class Engine {
  public:
    //! %Engine options
    struct Options {
      bool debug = true; //! enables verbose console output
      bool show_cursor = true; //! show cursor in render window
      bool fullscreen = false; // enable or disable fullscreen
      unsigned int default_width = 1280; //! width of requested window in pixels
      unsigned int default_height = 1024; //! height of requested window in pixels
      unsigned int samples = 1; //! Multisampling samples
      unsigned int opengl_major = 3; //! OpenGL major version, if less, startup fails
      unsigned int opengl_minor = 3; //! OpenGL minor version
      std::string window_name = "Engine"; //! name of the render window
    };

    using CursorPosCallbackFunc = std::function<void(float, int, int)>;
    using KeyboardCallbackFunc = std::function<void(float, bool[1024])>;
    using WindowSizeCallbackFunc = std::function<void(int, int)>;

    //! Constructor with default engine options.
    Engine();

    //! Constructor with the given engine options.
    //! @param o engine options to use
    explicit Engine(Options const& o);

    ~Engine();

    // Deleted constructors
    Engine(Engine const &other) = delete; // copy ctor
    Engine &operator=(Engine const &other) = delete; // copy-assignment op


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

    //! Initializes GL including  all engine subsystems.
    bool Init(Options const& options);

    //! Runs the mainloop.
    void Run();

    //! Updates time-related variables..
    void HandleTime();

    //! Renders a single frame.
    void Render() noexcept;

    Scene* CreateScene();

    //! Getter for resource manager.
    //! \return reference to the resource manager
    engine::resource::Manager* getResourceManager();

    void setScene(Scene* scene);

  private:
    struct Options options_;
    bool initialized_ = 0;
    GLFWwindow *window_ = nullptr;

    std::unique_ptr<resource::Manager> manager{nullptr};

    // Input handling
    std::unique_ptr<CursorPosCallbackFunc> cursor_pos_func_{nullptr};
    std::unique_ptr<KeyboardCallbackFunc> keyboard_func_{nullptr};
    std::unique_ptr<WindowSizeCallbackFunc> window_size_func_{nullptr};

    //! keyboard input handler
    engine::handler::KeyboardHandler* keyboard_handler_{nullptr};

    //! mouse input handler
    engine::handler::MouseHandler* mouse_handler_{nullptr};

    //! frame handler (called for each frame)
    engine::handler::FrameHandler* frame_handler_{nullptr};


    //! window handler (for resize, window callback functions)
    //engine::handler::WindowHandler* window_handler_ = nullptr;

    //! List of weak raw pointers to scenes
    std::vector<Scene*> scenes_;

    //! Currently active scene (weak raw pointer)
    Scene* current_scene{nullptr};





    //! elapsed time since last call
    float delta_time_ = 0;

    //! last time called (see `delta_time_`)
    float last_time_ = 0;

    void InitGL();
};

}


#endif //GRAPHICS_ENGINE_ENGINE_H
