#include "ShaderCompiler.h"
#include "FontRender.h"

#include <iostream>
#include <vector>

// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>


#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT 768
bool loadObj(const char* path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec2>& out_uvs,
    std::vector<glm::vec3>& out_normals) {
  std::cout << "Loading file: " << path << "\n";
  return true;
}

// callback function for key events
void cbKeyEvents(GLFWwindow* window, int key, int scancode, int action, int mode) {
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
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

  FontRenderer fontRenderer;
  fontRenderer.load("fonts/OpenSans-Regular.ttf");

  //std::vector<glm::vec3> vertices;
  //std::vector<glm::vec2> uvs;
  //std::vector<glm::vec3> out_normals;

  //loadObj("models/dragon_new.obj", vertices, uvs, out_normals);
  //std::cout << "Hello World, Render :)\n";

  glfwSetKeyCallback(window, cbKeyEvents);

  //GLfloat vertices[] = {
    //-0.5f, -0.5f,  0.0f,  // Top right
    //0.5f,  -0.5f,  0.0f,  // Bottom right
    //-0.5f, -0.5f,  0.0f,  // Bottom left
    //0.0f,   0.5f,  0.0f,  // Top left
  //};

    GLfloat vertices[] = {
         0.5f,  0.5f, 0.0f,  // Top Right
         0.5f, -0.5f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f,  // Bottom Left
        //-0.5f,  0.5f, 0.0f   // Top Left 
    };

  //GLuint indices[] = {
    //0, 1, 3,
    //1, 2, 3,
  //};




  Shader shader("shaders/default_vertex.glsl", "shaders/default_fragment.glsl");



  // generate VBO, VAO and EBO
  GLuint VAO[2], VBO[2];
  //glGenBuffers(1, &EBO); // element buffer object
  glGenBuffers(2, VBO); // vertex buffer object
  glGenVertexArrays(2, VAO); // vertex array object


  // Triangle setup
  glBindVertexArray(VAO[0]); // bind the vertex array object first
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), static_cast<GLvoid*>(0));
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);


  // Text rendering setup
  glBindVertexArray(VAO[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4* sizeof(GLfloat), 0);
  glBindVertexArray(0);



   glfwSwapInterval(0); 

  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Lets tell OpenGL how to use our vertex data
  //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
  //glEnableVertexAttribArray(0);

  //glBindVertexArray(0); // Unbind VAO


  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // Blending needed for text rendering
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  double lastTime = glfwGetTime();
  std::string performance_str;
  std::string fps_str;
  int n_frames = 0;
  do {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // Measure speed
    double currentTime = glfwGetTime();
    n_frames ++;
    if (currentTime - lastTime >= 1.0) {
      double frame_ms = 1000.0/double(n_frames);
      double fps = n_frames;
      std::cout << frame_ms << " ms/frame\n";

      n_frames = 0;
      lastTime += 1.0;
      performance_str = std::to_string(frame_ms) + " ms/frame";
      fps_str = std::to_string(fps) + " FPS";
    }
    fontRenderer.render(performance_str, 20.0f,WINDOW_HEIGHT - 50.0f, glm::vec3(0.5,0.8f, 0.2f), VAO[1], VBO[1]);
    fontRenderer.render(fps_str, 20.0f,WINDOW_HEIGHT - 120.0f, glm::vec3(0.5,0.8f, 0.2f), VAO[1], VBO[1]);

    // Rendering here
    shader.use();
    glBindVertexArray(VAO[0]);
    // draw triangle
    //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0); // unbind VAO


    //fontRenderer.render("hello world!", 20.0f, 20.0f, glm::vec3(0.5,0.8f, 0.2f), VAO[1], VBO[1]);




    glfwSwapBuffers(window);
    glfwPollEvents();
  } while(glfwWindowShouldClose(window) == 0);

  glDeleteVertexArrays(2, VAO);
  //glDeleteBuffers(1, &EBO);
  glDeleteBuffers(2, VBO);
  glfwTerminate();

  return EXIT_SUCCESS;
}
