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
    glfwSetCursorPosCallback(window, Input::cbMouseEventsDispatch);
  }

  static void cbKeyEventsDispatch(
      GLFWwindow* window, int key, int scancode,
      int action, int mods);

  static void cbMouseEventsDispatch(
      GLFWwindow* window, double x, double y);

  void handleKeys(std::function<void(bool[])> func);

  void keyCallback(
      GLFWwindow* window, int key,
      int scancode, int action, int mods);

  void mouseCallback(
      GLFWwindow* window,
      double x, double y);

  void addMouseCallback(
      std::function<void(double, double)> cb) {
    mouse_callback = cb;
  }


 private:
  std::function<void(double,double)> mouse_callback;
  static Input* instance;
  bool keys[1024] = {0};
};

#endif
