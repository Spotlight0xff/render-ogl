#include "Input.h"

void Input::cbKeyEventsDispatch(
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

void Input::cbWindowSizeDispatch(
    GLFWwindow* window, int width, int height) {
  if (glfwGetWindowUserPointer(window)) {
    static_cast<Input*>(glfwGetWindowUserPointer(window))->windowCallback(
        window, width, height);
  }
}


void Input::cbMouseEventsDispatch(
    GLFWwindow* window, double x, double y) {
  if (glfwGetWindowUserPointer(window)) {
    static_cast<Input*>(glfwGetWindowUserPointer(window))->mouseCallback(
        window, x, y);
  }
}

void Input::handleKeys(std::function<void(bool[])> func) {
  func(keys);
}

void Input::keyCallback(
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

void Input::mouseCallback(
    GLFWwindow* window,
    double x, double y) {
  if (mouse_callback) {
    mouse_callback(x,y);
  }
}

void Input::windowCallback(GLFWwindow* window, int w, int h)
{
  width = w;
  height = h;
}
