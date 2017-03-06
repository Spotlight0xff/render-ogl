#include "Engine.h"
#include "Util.h"


#include <stdexcept>
#include <iostream>

namespace engine {

// perform some initial allocations
// Most initialization happens at init()
Engine::Engine() {

}

Engine::~Engine() {

}

/*
 * Simple setter and getter functions for EngineOptions
 */

void Engine::SetOptions(Engine::Options& options) {
  options_ = options;

  // TODO re-initialize the engine
}






// Window and subsystem initialization
/*
 * Engine initialization:
 *  - window creation
 *  - subsystem initialization
 */
bool Engine::Init() {
  try {
    InitGL();
  } catch (std::runtime_error const& e) {
    std::cerr << "Engine::initGL() failed: " << e.what() << "\n";
    return false;
  }

  glfwSetWindowUserPointer(window_, this);
  glfwSetKeyCallback(window_, Engine::KeyboardCallback);
  glfwSetCursorPosCallback(window_, Engine::CursorPosCallback);
  //glfwSetWindowSizeCallback(window_, Engine::WindowSizeCallback);

  initialized_ = true;
  return true;
}
void error_cb(int error, const char* err_msg) {
  std::cerr << "GLFW Error: " << error << ", " << err_msg << "\n";
}

void Engine::InitGL() {
  glfwSetErrorCallback(error_cb);
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW.");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, options_.opengl_major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, options_.opengl_minor);
  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  if (options_.debug) {
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  }


  GLFWmonitor* monitor = nullptr;
  if (options_.fullscreen) {
    monitor = glfwGetPrimaryMonitor();
  }

  window_ = glfwCreateWindow(options_.default_width,
                             options_.default_height,
                             options_.window_name.c_str(),
                             monitor,
                             nullptr);

  if (window_ == nullptr) {
    throw std::runtime_error("Failed to create GLFW window. Maybe OpenGL compability issues?");
  }

  glfwMakeContextCurrent(window_);

  // magic variable to enable loading extension entry points
  // even if it is not present in the driver extension string
  glewExperimental = true; // for core profile
  if (glewInit() != GLEW_OK) {
    throw std::runtime_error("Failed to initialize GLEW.");
  }

  if (options_.debug) {
    util::enableDebugOutput();
  }

  // Set input options
  if (!options_.show_cursor) {
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }
  glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE);

  //glfwSetCursorPosCallback(window_, )


  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND); // needed for text rendering
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Engine::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
  auto user_ptr = (Engine*)glfwGetWindowUserPointer(window);
  if (user_ptr) {
    if(user_ptr->mouse_handler_) {
      user_ptr->mouse_handler_->CursorPosCallback(window, xpos, ypos);
    }
  }
}

void Engine::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  auto user_ptr = (Engine*)glfwGetWindowUserPointer(window);
  if (user_ptr) {
    if(user_ptr->keyboard_handler_) {
      user_ptr->keyboard_handler_->KeyboardCallback(window, key, scancode, action, mods);
    }
  }
}

/*void Engine::WindowSizeCallback(GLFWwindow* window, int width, int height) {
  auto user_ptr = (Engine*)glfwGetWindowUserPointer(window);
  if (user_ptr) {
    if(user_ptr->window_size_handler_) {
      user_ptr->window_size_handler_->CursorPosCallback(xpos, ypos);
    }
  }
}*/

void Engine::Render() {
  //scene::SceneObject* current = scenes_.back().get();
  //current->draw();
}

/*
 * Handles time management.
 * Mostly just computes the delta time.
 */
void Engine::HandleTime() {
  float current_time = static_cast<float>(glfwGetTime());
  delta_time_ = glfwGetTime() - last_time_ ;
  last_time_ = current_time;
}

void Engine::Run() {
  do {
    HandleTime();

    if (frame_handler_) {
      frame_handler_->FrameCallback(delta_time_);
    }

    Render();

    glfwPollEvents();
  } while(glfwWindowShouldClose(window_) == 0);
}


} // end namespace engine