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
  std::cout << "Hello World, Cube :)\n";
  std::cout << "\tVertices: " << vertices_.size() << "\n";
  std::cout << "\tElements: " << elements.size() << "\n";
  std::cout << "\tNormals : " << out_normals.size() << "\n";

  glfwSetKeyCallback(window, cbKeyEvents);


  Shader shader("shaders/default_vertex.glsl", "shaders/default_fragment.glsl");


		GLfloat flat_vertices[] = {
				 0.5f,  0.5f, 0.5f,  // Top Right
				 0.5f, -0.5f, 0.0f,  // Bottom Right
				-0.5f, -0.5f, 0.0f,  // Bottom Left
				-0.5f,  0.5f, 0.0f   // Top Left 
		};
		GLushort flat_elements[] = {  // Note that we start from 0!
				0, 1, 3,  // First Triangle
				1, 2, 3   // Second Triangle
		};


  // generate VBO and VAO
  //GLuint [>VAO[2],<] vbo_vertices;
  GLuint vao = 0;
	int count = 6;
  GLuint vbo_vertices = 0;
  GLuint ebo_elements = 0;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo_vertices);
  glGenBuffers(1, &ebo_elements);
  //const GLfloat* flat_vertices = &vertices_[0].x;
	//const GLushort* flat_elements = &elements[0];
	//const GLfloat* flat_vertices = &vertices_[0].x;
//for(auto v : vertices_) {
			//std::cout << v.x << " "<<v.y<<" "<<v.z << std::endl;
//}
  //GLfloat* flat_array = static_cast<GLfloat*>(glm::value_ptr(vertices_.front()));

	//for (size_t i=0; i < elements.size(); i++) {
		//std::cout << "vertex[" << i << "]= " << flat_elements[i] << std::endl;
	//}

  // bind vertex array object first
  glBindVertexArray(vao);

  // handle vertices
  glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
  glBufferData(GL_ARRAY_BUFFER, sizeof(flat_vertices), flat_vertices, GL_STATIC_DRAW);
  
  // handle elements data
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_elements);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(flat_elements), flat_elements, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // unbind VAO
  glBindVertexArray(0);


  // Triangle setup
  //glBindVertexArray(VAO[0]); // bind the vertex array object first
  //glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  
  // vertices:
  /*glVertexAttribPointer(*/
      //0, // attribute index
      //3, // number of elements per vertex
      //GL_FLOAT, // type
      //GL_FALSE, // normalized
      //6*sizeof(GLfloat), // stride
      //static_cast<GLvoid*>(0) // offset to first elem
      //);
  //glEnableVertexAttribArray(0);

  //// colors:
  //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat) [> stride <], reinterpret_cast<GLvoid*>(sizeof(GLfloat)*3));
  //glEnableVertexAttribArray(1);
  //glBindVertexArray(0); // unbind VAO
  //
  //
  
  /*
   * .obj file
   */
  // upload vertices into gpu (VBO)
  
  //glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
  //const GLfloat* flat_vertices = &vertices_[0].x;
  //glBufferData(GL_ARRAY_BUFFER, sizeof(flat_vertices), flat_vertices, GL_STATIC_DRAW);
  //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  //glEnableVertexAttribArray(0);

  // upload indices
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
  //const GLushort* flat_elements = &elements[0];
  //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(flat_elements), flat_elements, GL_STATIC_DRAW);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


  // Text rendering setup
  //glBindVertexArray(VAO[1]);
  //glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  //glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
  //glEnableVertexAttribArray(0);
  //glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4* sizeof(GLfloat), 0);
  //glBindVertexArray(0);

  glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -4.0));
  glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
  glm::mat4 projection = glm::perspective(45.0f, 1.0f*WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 10.0f);

    glm::mat4 mvp = projection * view * model;


  glfwSwapInterval(0);

  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Lets tell OpenGL how to use our vertex data
  //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
  //glEnableVertexAttribArray(0);

  //glBindVertexArray(0); // Unbind VAO

  GLint uniform_mvp = glGetUniformLocation(shader.getId(), "mvp");
  if (uniform_mvp == -1) {
    std::cerr << "Failed to bind uniform \"mvp\"\n";
  } else {
    std::cout << "Located uniform \"mvp\", sir!\n";
  }

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // Blending needed for text rendering
  glEnable(GL_BLEND);
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
    //fontRenderer.render(performance_str, 20.0f,WINDOW_HEIGHT - 50.0f, glm::vec3(0.5,0.8f, 0.2f), VAO[1], VBO[1]);
    //fontRenderer.render(fps_str, 20.0f,WINDOW_HEIGHT - 120.0f, glm::vec3(0.5,0.8f, 0.2f), VAO[1], VBO[1]);

    // Rendering here
    {

  //glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
  //glBufferData(GL_ARRAY_BUFFER, sizeof(flat_vertices), flat_vertices, GL_STATIC_DRAW);
  //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  //glEnableVertexAttribArray(0);

    //shader.use();

  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
  //glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_SHORT, 0);
    }

    {
      shader.use();
      glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));
      glBindVertexArray(vao);
      glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, 0);
      glBindVertexArray(0);
    }

    //glUniform3f(offset_loc, shift_x, shift_y, 0.0f);
    //glBindVertexArray(VAO[0]);
    //// draw triangle
    ////glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    //glBindVertexArray(0); // unbind VAO


    //fontRenderer.render("hello world!", 20.0f, 20.0f, glm::vec3(0.5,0.8f, 0.2f), VAO[1], VBO[1]);




    glfwSwapBuffers(window);
    glfwPollEvents();
  } while(glfwWindowShouldClose(window) == 0);

  //glDeleteVertexArrays(2, VAO);
  //glDeleteBuffers(1, &EBO);
  glDeleteBuffers(1, &vbo_vertices);
  glfwTerminate();

  return EXIT_SUCCESS;
}
