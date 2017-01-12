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
      GLFWwindow* window,
      int key,
      int scancode,
      int action,
      int mods) {
    if (glfwGetWindowUserPointer(window)) {
      static_cast<Input*>(glfwGetWindowUserPointer(window))->keyCallback(
        window, key, scancode, action, mods);
    }
  }

  void handle(std::function<void(bool[])> func) {
    func(keys);
  }

    void keyCallback(
        GLFWwindow* window,
        int key,
        int scancode,
        int action,
        int mods) {
      if (action == GLFW_PRESS) {
        keys[key] = true;
      } else if (action == GLFW_RELEASE) {
        keys[key] = false;
      }
      if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
      }
    }
 private:
  static Input* instance;
  bool keys[1024];
};

#endif
