//
// Created by spotlight on 1/21/17.
//

#ifndef GRAPHICS_ENGINE_KEYBOARDHANDLER_H
#define GRAPHICS_ENGINE_KEYBOARDHANDLER_H

namespace engine {
namespace handler {

class KeyboardHandler {
  public:
    virtual static void Dispatcher(
            GLFWwindow* window,
            int key,
            int scancode) = 0;
    virtual ~KeyboardHandler();

};
}
}
#endif //GRAPHICS_ENGINE_KEYBOARDHANDLER_H
