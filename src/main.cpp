#include "main.h"
#include "Util.h"
#include "ShaderCompiler.h"
#include "FontRender.h"
#include "Mesh.h"
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


  Shader shader("resources/shaders/model_vertex.glsl", "resources/shaders/model_fragment.glsl");
  Mesh mesh("resources/models/dragon.obj", shader);



  // Setup model-view-projection
  //glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
  //glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
  //glm::mat4 projection = glm::perspective(45.0f, 1.0f*WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 10.0f);

  //glm::mat4 mvp = projection * view * model;

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

  GLfloat last_x = WINDOW_WIDTH / 2.0;
  GLfloat last_y = WINDOW_HEIGHT / 2.0;

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
    fontRenderer.render(performance_str, 20.0f,WINDOW_HEIGHT - 50.0f, glm::vec3(0.5,0.8f, 0.2f));
    fontRenderer.render(fps_str, 20.0f,WINDOW_HEIGHT - 120.0f, glm::vec3(0.5,0.8f, 0.2f));

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // Rendering here
    mesh.draw();


    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -20.0));
    glm::mat4 view = camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(45.0f, 1.0f*WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 100.0f);

    glm::mat4 mvp = projection * view * model;
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));


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
