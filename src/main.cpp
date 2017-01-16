#include "main.h"
#include "Util.h"
#include "engine/ShaderCompiler.h"
#include "engine/scene/FontRender.h"
#include "engine/Model.h"
#include "engine/Input.h"
#include "engine/scene/EulerCamera.h"
#include "engine/Scene.h"
#include "engine/components/PhongLight.h"
#include "engine/components/PhongModel.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
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
  //glEnable(GL_MULTISAMPLE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


  return window;
}
}


int main() {
  using namespace engine;
  GLFWwindow* window = initGL();
  if (window == nullptr) {
    return EXIT_FAILURE;
  }

  Model model_nanosuit("resources/models/nanosuit2/nanosuit.obj");
  Model model_ground("resources/models/ground.obj");
  //Model model_interior("resources/models/audi/audo_r8.obj");


  // Setup camera and movement
  scene::EulerCamera camera;
  camera.look(0.0f, 0.0f);
  scene::FpsMovement movement(&camera);
  movement.setEyelevel(12.0);
  movement.setSensitivity(0.1f);
  movement.setSpeed(20.0f);
  camera.setMovement(movement);

  // Scene initialization
  Scene scene(window);
  scene.useFont("resources/fonts/OpenSans-Regular.ttf");
  scene.enableFpsCounter();
  scene.useCamera(&camera);

  // light source
  components::PhongLight light(&scene);
  //LightObject light2(&scene);


  // Nanosuit object
  components::PhongModel obj_nanosuit(&model_nanosuit, light, &scene);
  obj_nanosuit.setPosition({0.0, 0.0, -20.0});

  // Ground object
  components::ModelObject obj_ground(&model_ground, &scene);
  obj_ground.setPosition({0.0, 0.0, 0.0});
  obj_ground.setScale({50.0, 1.0, 50.0});
  // set custom shader for checkerboard
  obj_ground.setShader("checkerboard",
      [](Scene& s, components::ModelObject& obj, Shader& shader) {
        glm::mat4 model = obj.getModelMatrix();
        glm::mat4 view = s.getCameraRef().getViewMatrix();
        glm::mat4 projection = s.getProjectionMatrix();
        glUniformMatrix4fv(shader.getUniform("model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(shader.getUniform("view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(shader.getUniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));
      });

  // Add our models to the scene
  scene.addObjectRef(&light);
  //scene.addObjectRef(&light2);
  scene.addObjectRef(&obj_nanosuit);
  scene.addObjectRef(&obj_ground);

  //std::vector<ModelObject*> objs;
  //for (int i=-2; i < 5; i ++) {
    //for (int j=-2; j < 5; j ++) {
      //ModelObject* obj= new ModelObject(&model_nanosuit, light, &scene);
      //obj->setPosition({5.0 * i, 0.0, 5.0 * j});
      //scene.addObjectRef(obj);
      //objs.push_back(obj);
    //}
  //}


  scene.getInputRef().addMouseCallback(camera.do_mouse);

  camera.setPosition({0.0, 0.0, -40.0});

  do {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene.draw();

    GLfloat r = 6.0f;
    GLfloat speed = 3.5f;
    GLfloat pos_x = sin(glfwGetTime() * speed) * r;
    GLfloat pos_z = cos(glfwGetTime() * speed) * r;

    light.setPosition(glm::vec3(pos_x, 10.0, pos_z) + obj_nanosuit.getPosition());
    //light2.setPosition(glm::vec3(pos_z, 10.0, pos_x) + obj_nanosuit.getPosition());


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
