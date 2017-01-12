#include "main.h"
#include "Util.h"
#include "ShaderCompiler.h"
#include "FontRender.h"
#include "Model.h"
#include "Input.h"
#include "Camera.h"

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




int main() {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW!\n";
    return EXIT_FAILURE;
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
    std::cerr << "Failed to open GLFW window. Maybe not OpenGL 3.3 compatible?\n";
    return EXIT_FAILURE;
  }
  glfwMakeContextCurrent(window);
  glewExperimental = true; // for core profile
  if (glewInit() != GLEW_OK) {
    std::cerr << "Failed to init GLEW\n";
    return EXIT_FAILURE;
  }

  util::enableDebugOutput();


  FontRenderer fontRenderer;
  fontRenderer.load("resources/fonts/OpenSans-Regular.ttf");

  Input input(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


  //Shader shader("resources/shaders/model_vertex.glsl", "resources/shaders/model_fragment.glsl");
  Shader shader_model("resources/shaders/model_ambient_v.glsl", "resources/shaders/model_ambient_f.glsl");
  Shader shader_light("resources/shaders/light_vertex.glsl", "resources/shaders/light_fragment.glsl");
  Model model("resources/models/nanosuit2/nanosuit.obj");
  Model light("resources/models/cube.obj");

  model.setPosition({0.0, -7.0, -20.0});



  // disable vsync
  glfwSwapInterval(0);


  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnable(GL_BLEND); // needed for text rendering
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  EulerCamera camera;
  camera.setMouseSensitivity(0.1f);
  camera.setMovementSpeed(5.0f);
  camera.look(0.0f, 0.0f);

  GLfloat last_x = input.width;
  GLfloat last_y = input.height;

  input.addMouseCallback([&last_x, &last_y,
                         &camera]
                         (double x, double y) {
    GLfloat delta_x = GLfloat(x) - last_x;
    GLfloat delta_y = last_y - GLfloat(y);
    last_x = GLfloat(x);
    last_y = GLfloat(y);
    camera.look(delta_x, delta_y);
  });
  GLfloat delta_frame = 0.0;
  GLfloat last_frame = 0.0;
  GLfloat current_time = GLfloat(glfwGetTime());
  GLfloat last_time = GLfloat(glfwGetTime());
  std::string performance_str;
  std::string fps_str("1337.0 FPS");
  int n_frames = 0;
  do {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Measure speed
    current_time = GLfloat(glfwGetTime());
    delta_frame = current_time - last_frame;
    last_frame = current_time;
    n_frames ++;

    if (current_time - last_time >= 1.0f) {
      GLfloat frame_ms = 1000.0f/GLfloat(n_frames);
      GLfloat fps = n_frames;
      std::cout << frame_ms << " ms/frame\n";

      n_frames = 0;
      last_time += 1.0;
      performance_str = std::to_string(frame_ms) + " ms/frame";
      fps_str = std::to_string(fps) + " FPS";
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //fontRenderer.render(performance_str, 20.0f, height - 300.0f, glm::vec3(0.5,0.8f, 0.2f));
    fontRenderer.render(fps_str, 20.0f, input.height - 200.0f, glm::vec3(0.5,0.8f, 0.2f));

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // Rendering here
    shader_model.use();
    glm::mat4 mat_model = model.getModelMatrix();
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(45.0f, 1.0f*input.width/GLfloat(input.height), 0.1f, 100.0f);

    GLfloat r = 10.0f;
    GLfloat pos_x = sin(glfwGetTime()) * r;
    GLfloat pos_z = cos(glfwGetTime()) * r;

    light.setPosition(glm::vec3(pos_x, 10.0, pos_z) + model.getPositon());
    glm::mat4 mat_model_light = light.getModelMatrix();
    glm::mat4 view_light = camera.getViewMatrix();
    //glm::mat4 projection_light = glm::perspective(45.0f, 1.0f*input.width/input.height, 0.1f, 100.0f);

    //glm::mat4 mvp = projection * view * mat_model;
    glUniformMatrix4fv(glGetUniformLocation(shader_model.getId(), "model"), 1, GL_FALSE, glm::value_ptr(mat_model));
    glUniformMatrix4fv(glGetUniformLocation(shader_model.getId(), "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader_model.getId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(glGetUniformLocation(shader_model.getId(), "lightColor"), 1.0, 1.0, 1.0);
    glm::vec3 light_pos = glm::vec3(0.0, -7.0, -20.0) + glm::vec3(pos_x, 10.0, pos_z);
    glUniform3fv(glGetUniformLocation(shader_model.getId(), "lightPos"), 1, glm::value_ptr(light_pos));
    glUniform3fv(glGetUniformLocation(shader_model.getId(), "cameraPos"), 1, glm::value_ptr(camera.getPosition()));
    glUniform1f(glGetUniformLocation(shader_model.getId(), "ambientStrength"), 0.1);
    model.draw(shader_model);


    shader_light.use();

    glUniformMatrix4fv(glGetUniformLocation(shader_light.getId(), "model"), 1, GL_FALSE, glm::value_ptr(mat_model_light));
    glUniformMatrix4fv(glGetUniformLocation(shader_light.getId(), "projection"), 1, GL_FALSE, glm::value_ptr(projection_light));
    glUniformMatrix4fv(glGetUniformLocation(shader_light.getId(), "view"), 1, GL_FALSE, glm::value_ptr(view_light));
    light.draw(shader_light);


    glfwSwapBuffers(window);
    glfwPollEvents();
    input.handleKeys([&camera, &delta_frame](bool keys[]) {
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
     if (keys[GLFW_KEY_SPACE]) {
       camera.moveUp(delta_frame);
     }
     if (keys[GLFW_KEY_H]) {
       camera.moveDown(delta_frame);
     }
     //camera_pos.y = 0.0; // keep at ground
   });
  } while(glfwWindowShouldClose(window) == 0);

  glfwTerminate();

  return EXIT_SUCCESS;
}
