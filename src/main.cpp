#include "main.h"
#include "Util.h"
#include "ShaderCompiler.h"
#include "FontRender.h"
#include "Mesh.h"

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





// callback function for key events
void cbKeyEvents(GLFWwindow* window, int key, int scancode, int action, int mode) {
  (void) scancode,
  (void) mode;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}


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

  GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
  {
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(util::glDebugOutput, nullptr);
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
      // initialize debug output
  }else {
    std::cout << "no debug output :(\n";
  }

  glfwSetKeyCallback(window, cbKeyEvents);

  FontRenderer fontRenderer;
  fontRenderer.load("resources/fonts/OpenSans-Regular.ttf");




  Shader shader("resources/shaders/model_vertex.glsl", "resources/shaders/default_fragment.glsl");
  Mesh mesh("resources/models/cube.obj", shader);



  // Setup model-view-projection
  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
  glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
  glm::mat4 projection = glm::perspective(45.0f, 1.0f*WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 10.0f);

  glm::mat4 mvp = projection * view * model;

  // disable vsync
  glfwSwapInterval(0);


  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnable(GL_BLEND); // needed for text rendering
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  double lastTime = glfwGetTime();
  std::string performance_str;
  std::string fps_str;
  int n_frames = 0;
  do {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Measure speed
    double currentTime = glfwGetTime();
    n_frames ++;

    // Set vertices color
    //GLfloat shift_x = (sin(currentTime) / 2);
    //GLfloat shift_y = (cos(currentTime) / 2);
    //GLint offset_loc = glGetUniformLocation(shader.getId(), "vec_offset");

    if (currentTime - lastTime >= 1.0) {
      double frame_ms = 1000.0/double(n_frames);
      double fps = n_frames;
      std::cout << frame_ms << " ms/frame\n";

      n_frames = 0;
      lastTime += 1.0;
      performance_str = std::to_string(frame_ms) + " ms/frame";
      fps_str = std::to_string(fps) + " FPS";
    }

    fontRenderer.render(performance_str, 20.0f,WINDOW_HEIGHT - 50.0f, glm::vec3(0.5,0.8f, 0.2f));
    fontRenderer.render(fps_str, 20.0f,WINDOW_HEIGHT - 120.0f, glm::vec3(0.5,0.8f, 0.2f));

    // Rendering here
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
    mesh.draw();


    glfwSwapBuffers(window);
    glfwPollEvents();
  } while(glfwWindowShouldClose(window) == 0);

  glfwTerminate();

  return EXIT_SUCCESS;
}
