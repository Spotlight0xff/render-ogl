#include "Engine.h"
#include "Util.h"


#include <stdexcept>
#include <iostream>

namespace engine {

// perform some initial allocations
// Most initialization happens at init()
Engine::Engine()
: manager(std::make_unique<resource::Manager>()){
}

Engine::Engine(Engine::Options const& o)
        : options_(o) {
  Engine();
}

Engine::~Engine() {
  glfwTerminate();
}

#define glGetDebugMessageLogARB pfnGetDebugMessageLog
PFNGLGETDEBUGMESSAGELOGARBPROC pfnGetDebugMessageLog;


// Flag indicating whether the extension is supported
int has_ARB_debug_output = 0;




// Window and subsystem initialization
/*
 * Engine initialization:
 *  - window creation
 *  - subsystem initialization
 */
bool Engine::Init(Engine::Options const& options) {
  options_ = options;
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

    if (glfwExtensionSupported("GL_ARB_debug_output"))
    {
      pfnGetDebugMessageLog = (PFNGLGETDEBUGMESSAGELOGARBPROC)
              glfwGetProcAddress("glGetDebugMessageLogARB");
      // The extension is supported by the current context
      has_ARB_debug_output = 1;
    }
  }

  // Set input options
  if (!options_.show_cursor) {
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }
  glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE);

  if (options_.samples > 1) {
    glEnable(GL_MULTISAMPLE);
  }

  //glfwSetCursorPosCallback(window_, )


  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_BLEND); // needed for text rendering
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Engine::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
  auto user_ptr = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
  if (user_ptr) {
    if(user_ptr->mouse_handler_) {
      user_ptr->mouse_handler_->CursorPosCallback(window, xpos, ypos);
    }
  }
}

void Engine::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  auto user_ptr = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));
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

void Engine::Render() noexcept {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TODO(performance): rework to remove if-clause
  if(current_scene) {
    current_scene->draw();
  }


  if (has_ARB_debug_output) {
    // Now the extension function can be called as usual
    unsigned int count = 10; // max. num. of messages that will be read from the log
    int bufsize = 2048;

    unsigned int *sources = new unsigned int[count];
    unsigned int *types = new unsigned int[count];
    unsigned int *ids = new unsigned int[count];
    unsigned int *severities = new unsigned int[count];
    int *lengths = new int[count];

    char *messageLog = new char[bufsize];

    unsigned int retVal = glGetDebugMessageLogARB(count, bufsize, sources, types, ids,
                                                  severities, lengths, messageLog);
    if (retVal > 0) {
      unsigned int pos = 0;
      for (unsigned int i = 0; i < retVal; i++) {
        std::cerr << "Error: " << sources[i] << ": " << messageLog[pos] << "\n";
        /*DebugOutputToFile(sources[i], types[i], ids[i], severities[i],
                          &messageLog[pos]);
        */pos += lengths[i];
      }
    }
    delete[] sources;
    delete[] types;
    delete[] ids;
    delete[] severities;
    delete[] lengths;
    delete[] messageLog;
  }
  glfwSwapBuffers(window_);
}

/*!
 * Computes delta time.
 */
void Engine::HandleTime() {
  float current_time = static_cast<float>(glfwGetTime());
  delta_time_ = glfwGetTime() - last_time_ ;
  last_time_ = current_time;
}

/*!
 * Runs the main loop of the engine:
 *  - time handling (delta time)
 *  - rendering
 *  - input polling
 *  - callbacks (if used)
 */
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

void Engine::handleInput() {
	glfwPollEvents();

	if (frame_handler_) {
		frame_handler_->FrameCallback(delta_time_);
	}
}

bool Engine::wantsExit() noexcept {
	return (glfwWindowShouldClose(window_) != 0);
}

engine::resource::Manager* Engine::getResourceManager() {
  return manager.get();
}

void Engine::setScene(Scene *scene) {
  current_scene = scene;
}

Scene *Engine::CreateScene() {
  return manager->loadAsset<Scene>(manager.get());
}

} // end namespace engine