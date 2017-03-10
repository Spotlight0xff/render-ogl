#ifndef GRAPHICS_ENGINE_MOUSEHANDLER_H
#define GRAPHICS_ENGINE_MOUSEHANDLER_H

#include <GLFW/glfw3.h>

namespace engine {
namespace handler {

//! @brief Interface for classes implementing a mouse handler
class MouseHandler {
  public:
    virtual void CursorPosCallback(GLFWwindow* window, double xpos, double ypos) = 0;
    virtual ~MouseHandler() {}

};

} // end namespace engine::handler
}


#endif //GRAPHICS_ENGINE_MOUSEHANDLER_H
