#ifndef INPUT_H
#define INPUT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <functional>

class Input {
 public:
  Input(GLFWwindow* window) {
    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, Input::cbKeyEventsDispatch);
  }

  static void cbKeyEventsDispatch(
      GLFWwindow* window, int key, int scancode,
      int action, int mods);

  void handleKeys(std::function<void(bool[])> func);

  void keyCallback(
      GLFWwindow* window, int key,
      int scancode, int action, int mods);
 private:
  static Input* instance;
  bool keys[1024] = {0};
};

#endif
