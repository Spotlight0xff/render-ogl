#ifndef INPUT_H
#define INPUT_H

#include "opengl.h"

#include <iostream>
#include <functional>

#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT 768

namespace engine {

class Input {
  public:
    Input(GLFWwindow *window) {
      glfwSetWindowUserPointer(window, this);

      glfwSetKeyCallback(window, Input::cbKeyEventsDispatch);
      glfwSetCursorPosCallback(window, Input::cbMouseEventsDispatch);
      glfwSetWindowSizeCallback(window, Input::cbWindowSizeDispatch);
      glfwGetWindowSize(window, &width, &height);
    }

    static void cbWindowSizeDispatch(
            GLFWwindow *window, int width, int height);

    static void cbKeyEventsDispatch(
            GLFWwindow *window, int key, int scancode,
            int action, int mods);

    static void cbMouseEventsDispatch(
            GLFWwindow *window, double x, double y);

    void handleKeys(std::function<void(bool[])> func);

    void keyCallback(
            GLFWwindow *window, int key,
            int scancode, int action, int mods);

    void mouseCallback(
            GLFWwindow *window,
            double x, double y);

    void windowCallback(GLFWwindow *window, int w, int h);

    void addMouseCallback(
            std::function<void(double, double)> cb) {
      mouse_callback = cb;
    }

    int height = 0;
    int width = 0;


  private:
    std::function<void(double, double)> mouse_callback;
    static Input *instance;
    bool keys[1024] = {0};
};

} // namespace engine

#endif
