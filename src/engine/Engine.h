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

    Engine();
    ~Engine();


    void SetOptions(struct Options& options);

    void SetHandler(Handler type) {
      switch(type) {
        case Handler::FPS_MOVEMENT:
          camera_ = std::make_unique(new ::engine::scene::EulerCamera);
          handler_ = std::make_unique(new ::engine::handler::FpsMovement(camera_.get()));
          break;
      }
    }

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
    std::unique_ptr<::engine::handler::Handler> input_handler_{};
    std::unique_ptr<::engine::scene::Camera> camera_{};



    // Camera handling

    float delta_time_ = 0;
    float last_time_ = 0;

    void InitGL();
};

}


#endif //GRAPHICS_ENGINE_ENGINE_H
