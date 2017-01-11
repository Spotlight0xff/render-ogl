#include "main.h"
#include "Util.h"
#include "ShaderCompiler.h"
#include "FontRender.h"
#include "Loader.h"

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

  FontRenderer fontRenderer;
  fontRenderer.load("fonts/OpenSans-Regular.ttf");

  std::vector<glm::vec3> vertices_;
  std::vector<GLushort> elements;
  std::vector<glm::vec3> out_normals;

  loader::parseObj("models/cube.obj", vertices_, out_normals, elements, loader::FaceFormat::SLASH);
  GLfloat* flat_vertices = &vertices_[0].x;
  GLushort* flat_elements = &elements[0];
  std::cout << "Hello World, Cube :)\n";
  std::cout << "\tVertices: " << vertices_.size() << "\n";
  std::cout << "\tElements: " << elements.size() << "\n";
  std::cout << "\tNormals : " << out_normals.size() << "\n";

  glfwSetKeyCallback(window, cbKeyEvents);


  Shader shader("shaders/default_vertex.glsl", "shaders/default_fragment.glsl");




  // generate VBO and VAO
  GLuint vao_obj = 0;
  size_t count_elements = elements.size();
  size_t count_coords = vertices_.size() *3;
  size_t count_faces = count_elements/3;
  std::cout << count_elements << " Elements\n"
            << count_coords << " Coordinates\n"
            << count_faces << " Faces\n";

  GLuint vbo_vertices = 0;
  GLuint ebo_elements = 0;
  glGenVertexArrays(1, &vao_obj);
  glGenBuffers(1, &vbo_vertices);
  glGenBuffers(1, &ebo_elements);

  // bind vertex array object first
  glBindVertexArray(vao_obj);

  // handle vertices
  glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
  glBufferData(GL_ARRAY_BUFFER, count_coords * sizeof(GLfloat), flat_vertices, GL_STATIC_DRAW);
  // handle elements data
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_elements);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count_elements  * sizeof(GLushort), flat_elements, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(0));
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // unbind VAO
  glBindVertexArray(0);


  // Text rendering setup
  GLuint vao_font = 0;
  GLuint vbo_font = 0;
  glGenVertexArrays(1, &vao_font);
  glGenBuffers(1, &vbo_font);

  glBindVertexArray(vao_font);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_font);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4* sizeof(GLfloat), 0);
  glBindVertexArray(0);

  // Setup model-view-projection
  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
  glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
  glm::mat4 projection = glm::perspective(45.0f, 1.0f*WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 10.0f);

  glm::mat4 mvp = projection * view * model;
  GLint uniform_mvp = glGetUniformLocation(shader.getId(), "mvp");
  if (uniform_mvp == -1) {
    std::cerr << "Failed to bind uniform \"mvp\"\n";
  } else {
    std::cout << "Located uniform \"mvp\", sir!\n";
  }


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
    fontRenderer.render(performance_str, 20.0f,WINDOW_HEIGHT - 50.0f, glm::vec3(0.5,0.8f, 0.2f), vao_font, vbo_font);
    fontRenderer.render(fps_str, 20.0f,WINDOW_HEIGHT - 120.0f, glm::vec3(0.5,0.8f, 0.2f), vao_font, vbo_font);

    // Rendering here
    {
      shader.use();
      glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
      glBindVertexArray(vao_obj);
      glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(count_elements), GL_UNSIGNED_SHORT, 0);
      glBindVertexArray(0);
    }


    glfwSwapBuffers(window);
    glfwPollEvents();
  } while(glfwWindowShouldClose(window) == 0);

  glDeleteBuffers(1, &ebo_elements);
  glDeleteBuffers(1, &vbo_vertices);
  glDeleteBuffers(1, &vbo_font);
  glDeleteVertexArrays(1, &vao_obj);
  glDeleteVertexArrays(1, &vao_font);
  glfwTerminate();

  return EXIT_SUCCESS;
}
