//
// Created by spotlight on 1/21/17.
//

#ifndef GRAPHICS_ENGINE_KEYBOARDHANDLER_H
#define GRAPHICS_ENGINE_KEYBOARDHANDLER_H

namespace engine {
namespace handler {

//! @brief interface for classes implementing a keyboard handler
class KeyboardHandler {
  public:
    virtual void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;
    virtual ~KeyboardHandler() {}

};
}
}
#endif //GRAPHICS_ENGINE_KEYBOARDHANDLER_H
