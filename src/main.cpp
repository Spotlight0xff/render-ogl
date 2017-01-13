#include "main.h"
#include "Util.h"
#include "ShaderCompiler.h"
#include "FontRender.h"
#include "Model.h"
#include "Input.h"
#include "Camera.h"
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
#include <glm/gtx/norm.hpp>


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





  EulerCamera camera;
  //FpsMovement movement;
  //movement.setEyelevel(7.0);
  //camera.useMovement(movement);
  camera.setMouseSensitivity(0.1f);
  camera.setMovementSpeed(5.0f);
  camera.look(0.0f, 0.0f);
  camera.setPosition({0.0, 7.0, 0.0});

  // Scene initialization
  Scene scene(window);
  scene.useFont("resources/fonts/OpenSans-Regular.ttf");
  scene.enableFpsCounter();
  scene.useCamera(&camera);

  LightObject light(&scene);
  scene.setLight(light);



  ModelObject obj_nanosuit(&model_nanosuit, scene.getLightRef(), &scene);
  obj_nanosuit.setPosition({0.0, 0.0, -20.0});

  ModelObject obj_ground(&model_ground, scene.getLightRef(), &scene);
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
  scene.addObjectRef(&obj_nanosuit);
  scene.addObjectRef(&obj_ground);

  GLfloat last_x = 0.0;
  GLfloat last_y = 0.0;
  scene.getInputRef().addMouseCallback([&last_x, &last_y,
                         &camera]
                         (double x, double y) {
    GLfloat delta_x = GLfloat(x) - last_x;
    GLfloat delta_y = last_y - GLfloat(y);
    last_x = GLfloat(x);
    last_y = GLfloat(y);
    camera.look(delta_x, delta_y);
  });

  //GLfloat last_x = input.width;
  //GLfloat last_y = input.height;

  GLfloat delta_frame = 0.0;
  GLfloat last_frame = 0.0;
  GLfloat current_time = GLfloat(glfwGetTime());
  GLfloat jump_force = 0.0f;
  GLfloat gravity = 0.000981f;
  do {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene.draw();
    current_time = glfwGetTime();
    delta_frame = current_time - last_frame;
    last_frame = current_time;


    GLfloat r = 6.0f;
    GLfloat speed = 1.0f;
    GLfloat pos_x = sin(glfwGetTime() * speed) * r;
    GLfloat pos_z = cos(glfwGetTime() * speed) * r;

    light.setPosition(glm::vec3(pos_x, 10.0, pos_z) + obj_nanosuit.getPosition());


    glfwSwapBuffers(window);
    glfwPollEvents();

    scene.getInputRef().handleKeys([&camera, &delta_frame,
                      &jump_force, &gravity](bool keys[]) {
     if (keys[GLFW_KEY_W]) {
       camera.moveForward(delta_frame);
     }
     if (keys[GLFW_KEY_S]) {
       camera.moveBackward(delta_frame);
     }
     if (keys[GLFW_KEY_A]) {
       camera.moveLeft(delta_frame);
     }
     if (keys[GLFW_KEY_D]) {
       camera.moveRight(delta_frame);
     }
     if (keys[GLFW_KEY_LEFT_SHIFT]) {
     // TODO: just apply the modifier
      camera.moveForward(2*delta_frame);
     }
     if (keys[GLFW_KEY_SPACE]) {
       if (jump_force == 0.0f) {
           jump_force = 0.05f;
           camera.setY(7.0);
        }
     }
     if (keys[GLFW_KEY_H]) {
       camera.moveDown(delta_frame);
     }
     if (camera.getPosition().y >= 7.0) {
       GLfloat net_force = jump_force - gravity;
       camera.moveUp(net_force);
       jump_force -= gravity;
     }
       if (camera.getPosition().y < 7.0) {
         jump_force = 0.0f;
         camera.setY(7.0);
       }
   });
  } while(glfwWindowShouldClose(window) == 0);

  glfwTerminate();

  return EXIT_SUCCESS;
}
