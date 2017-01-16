#include "main.h"
#include "Util.h"
#include "engine/ShaderCompiler.h"
#include "engine/scene/FontRender.h"
#include "engine/Model.h"
#include "engine/Input.h"
#include "engine/scene/EulerCamera.h"
#include "engine/Scene.h"
#include "engine/components/Phong/PhongLight.h"
#include "engine/components/Phong/PhongModel.h"
#include "engine/Timing.h"

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
#include <engine/scene/Phong.h>


//#define INTERACTIVE


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

  scene::Phong phong_scene;

  // Nanosuit object
  components::PhongModel* obj_nanosuit = phong_scene.addModel(model_nanosuit);
  obj_nanosuit->setPosition({0.0, 0.0, 15.0});

  components::PhongLight* light = phong_scene.addLight();
  components::PhongLight* light2 = phong_scene.addLight();

  // Ground object
  components::ModelObject obj_ground(&model_ground);
  obj_ground.setPosition({0.0, 0.0, 0.0});
  obj_ground.setScale({50.0, 1.0, 50.0});
  // set custom shader for checkerboard
  obj_ground.setShader("checkerboard",
      [](Scene& s, components::ModelObject& obj, Shader& shader) {
        shader.set("model", obj.getModelMatrix());
        shader.set("view", s.getCameraRef().getViewMatrix());
        shader.set("projection", s.getProjectionMatrix());
      });


  // Add our models to the scene
  scene.addObjectRef(&obj_ground);
  scene.addObjectRef(&phong_scene);

  std::vector<components::ModelObject*> objs;
  for (int i=-3; i < 4; i ++) {
    for (int j=1; j < 7; j ++) {
      components::PhongModel* obj= phong_scene.addModel(model_nanosuit);
      obj->setPosition(obj_nanosuit->getPosition()
       - glm::vec3({5.0 * i, 0.0, 5.0 * j})
       - glm::vec3({0.0, 0.0, 10.0}));
      objs.push_back(obj);
    }
  }






  engine::Timing timer;
  timer.Start();
#ifdef INTERACTIVE
  scene.getInputRef().addMouseCallback(camera.do_mouse);
  camera.setPosition({0.0, 12.0, 30.0});
  do {
#else
  glm::vec3 look_at = obj_nanosuit->getPosition() + glm::vec3({0.0, 10.0, 0.0});
  camera.lookAt(look_at);
  for (GLfloat camera_r = 0.0f; camera_r < 15.0; camera_r += 0.01) {
#endif

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    scene.draw();
    {
      GLfloat r = 6.0f;
      GLfloat r2 = 25.0f;
      GLfloat speed = 0.5f;
      GLfloat speed2 = 1.5f;
      GLfloat pos_x = sin(glfwGetTime() * speed) * r;
      GLfloat pos_z = cos(glfwGetTime() * speed) * r;
      GLfloat pos_x2 = sin(glfwGetTime() * speed2) * r2;
      GLfloat pos_z2 = cos(glfwGetTime() * speed2) * r2;

      light->setPosition(glm::vec3(pos_x, 5.0, pos_z) + obj_nanosuit->getPosition());
      light2->setPosition(glm::vec3(pos_z2, 10.0, pos_x2) + (obj_nanosuit->getPosition() - glm::vec3(0.0, 0.0, 30.0)));
    }
#ifndef INTERACTIVE
    {
      GLfloat r = 20.0f;
      GLfloat speed = 0.5f;
      GLfloat pos_x = sin(camera_r * speed) * r;
      GLfloat pos_z = cos(camera_r * speed) * r;

      camera.setPosition(glm::vec3(pos_x, 10.0, pos_z) + look_at);
      camera.lookAt(look_at);
    }
#endif


    glfwSwapBuffers(window);
    glfwPollEvents();

#ifdef INTERACTIVE
    scene.getInputRef().handleKeys([&camera]
                      (bool keys[]) {
                      camera.do_keyboard(keys);
                      });
    } while(glfwWindowShouldClose(window) == 0);
#else
  } // end for loop
#endif
  std::cout << "Time elapsed: " << timer.waitStop()/ 1000000.0 << " ms\n";

  glfwTerminate();

  return EXIT_SUCCESS;
}
