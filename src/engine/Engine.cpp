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

void Engine::setOptions(struct Options& options) {
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

  initialized_ = true;
  return true;
}

void Engine::InitGL() {
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW.");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, options_.opengl_major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, options_.opengl_minor);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
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

  if (!options_.show_cursor) {
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }


  glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND); // needed for text rendering
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Engine::Render() {

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

    glfwPollEvents();
  } while(glfwWindowShouldClose(window_) == 0);
}





} // end namespace engine