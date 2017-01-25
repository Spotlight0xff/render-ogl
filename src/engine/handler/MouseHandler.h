#ifndef GRAPHICS_ENGINE_MOUSEHANDLER_H
#define GRAPHICS_ENGINE_MOUSEHANDLER_H

#include <GLFW/glfw3.h>

namespace engine {
namespace handler {

class MouseHandler {
  public:
    //virtual static void Dispatcher(
    //        GLFWwindow* window,
    //        int key,
    //        int scancode) = 0;
    virtual ~MouseHandler();

};

} // end namespace engine::handler
}


#endif //GRAPHICS_ENGINE_MOUSEHANDLER_H
