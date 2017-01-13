#include "main.h"
#include "Util.h"
#include "ShaderCompiler.h"
#include "FontRender.h"
#include "Model.h"
#include "Input.h"
#include "EulerCamera.h"
#include "Scene.h"
#include "LightObject.h"
#include "ModelObject.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace {

GLFWwindow* initGL() {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW!\n";
    return nullptr;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);


  GLFWwindow* window;
  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Rendering", nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to open GLFW window. Maybe not OpenGL 4.3 compatible?\n";
    return nullptr;
  }
  glfwMakeContextCurrent(window);
  glewExperimental = true; // for core profile
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to init GLEW\n";
    return nullptr;
  }

  util::enableDebugOutput();
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // disable vsync
  glfwSwapInterval(0);


  glEnable(GL_BLEND); // needed for text rendering
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


  return window;
}
}


int main() {
  GLFWwindow* window = initGL();
  if (window == nullptr) {
    return EXIT_FAILURE;
  }

  Model model_nanosuit("resources/models/nanosuit2/nanosuit.obj");
  Model model_ground("resources/models/ground.obj");





  // Setup camera and movement
  EulerCamera camera;
  camera.look(0.0f, 0.0f);
  camera.setPosition({0.0, 7.0, 0.0});
  FpsMovement movement(&camera);
  movement.setEyelevel(7.0);
  movement.setSensitivity(0.1f);
  movement.setSpeed(20.0f);
  camera.setMovement(movement);

  // Scene initialization
  Scene scene(window);
  scene.useFont("resources/fonts/OpenSans-Regular.ttf");
  scene.enableFpsCounter();
  scene.useCamera(&camera);

  // light source
  LightObject light(&scene);


  // Nanosuit object
  ModelObject obj_nanosuit(&model_nanosuit, light, &scene);
  obj_nanosuit.setPosition({0.0, 0.0, -20.0});

  // Ground object
  ModelObject obj_ground(&model_ground, light, &scene);
  obj_ground.setPosition({0.0, 0.0, 0.0});
  obj_ground.setScale({50.0, 1.0, 50.0});
  // set custom shader for checkerboard
  obj_ground.setShader("resources/shaders/ground.vs", "resources/shaders/ground.fs",
      [](Scene& s, ModelObject& obj, Shader& shader) {
        glm::mat4 model = obj.getModelMatrix();
        glm::mat4 view = s.getCameraRef().getViewMatrix();
        glm::mat4 projection = s.getProjectionMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
      });

  // Add our models to the scene
  scene.addObjectRef(&light);
  scene.addObjectRef(&obj_nanosuit);
  scene.addObjectRef(&obj_ground);

  scene.getInputRef().addMouseCallback(camera.do_mouse);


  do {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene.draw();

    GLfloat r = 6.0f;
    GLfloat speed = 3.0f;
    GLfloat pos_x = sin(glfwGetTime() * speed) * r;
    GLfloat pos_z = cos(glfwGetTime() * speed) * r;

    light.setPosition(glm::vec3(pos_x, 10.0, pos_z) + obj_nanosuit.getPosition());


    glfwSwapBuffers(window);
    glfwPollEvents();

    scene.getInputRef().handleKeys([&camera]
                      (bool keys[]) {
                      camera.do_keyboard(keys);
                      });
  } while(glfwWindowShouldClose(window) == 0);

  glfwTerminate();

  return EXIT_SUCCESS;
}
