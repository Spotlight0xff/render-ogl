#include "main.h"
#include "ShaderCompiler.h"
#include "FontRender.h"

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



static size_t split(const std::string& s, char delimiter, std::vector<std::string>& splits) {
  std::string item;
  std::stringstream ss(s);
  while (std::getline(ss, item, delimiter)) {
    splits.push_back(item);
  }
  return splits.size();
}

bool loadObj(const char* path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec3>& out_normals,
    std::vector<GLushort>& out_elements) {
  std::cout << "Loading file: " << path << "\n";
  std::ifstream in(path);
  if (!in.good()) {
    std::cerr << "Failed to open " << path << "\n";
    return false;
  }

  enum class FaceFormat {
    SPACE,
    SLASH,
    DOUBLESLASH
  };
  FaceFormat face_format = FaceFormat::SLASH;

  std::string line;
  in >> std::ws;
  while(std::getline(in, line)) {
    if        (line.substr(0,2) == "v ") { // geometric vertex
      std::istringstream ss(line.substr(2));
      glm::vec3 v;
      ss >> v.x; ss >> v.y; ss >> v.z;
      out_vertices.push_back(v);
    } else if (line.substr(0, 2) == "f ") { // face
      if (face_format == FaceFormat::SPACE) { // "a b c"
        std::istringstream ss(line.substr(2));
        GLushort a, b, c;
        ss >> a; ss >> b; ss >> c;
        a --; b --; c--;
        out_elements.insert(out_elements.end(), {a, b, c});
      } else if (face_format == FaceFormat::SLASH) { // (vertex/texcoord/normal)
        std::istringstream ss(line.substr(2));
        std::string sa, sb, sc;
        ss >> sa; ss >> sb; ss >> sc;
        std::vector<std::string> split_a, split_b, split_c;
        split(sa, '/', split_a);
        split(sb, '/', split_b);
        split(sc, '/', split_c);
        GLushort a, b, c;
        a = static_cast<unsigned short>(std::stoi(split_a[0]));
        b = static_cast<unsigned short>(std::stoi(split_b[0]));
        c = static_cast<unsigned short>(std::stoi(split_c[0]));
        a --; b --; c --;
        out_elements.insert(out_elements.end(), {a, b, c});

      }
    } else if (line.substr(0, 3) == "vn ") { // normal
    } else if (line.substr(0, 2) == "o ") { // object name
    } else if (line.substr(0, 3) == "vt ") { // texture vertex
    } else if (line.substr(0, 2) == "g ") { // group name
    } else if (line.substr(0, 7) == "mtllib ") { // material library
    } else if (line.substr(0, 7) == "usemtl ") { // material name
    } else if (line.substr(0, 2) == "s ") { // smooth shading
    } else if (*line.begin() == '#' || // comment, ignore
               line.empty()) {
    } else {
      std::cout << "Unhandled line: '" << line << "'" << std::endl;
    }
    while (in.peek() == ' ' ||
           in.peek() == '\r') // skip spaces
      in.get();
  }

  out_normals.resize(out_vertices.size(), glm::vec3(0.0, 0.0, 0.0));
  for (size_t i=0; i < out_elements.size(); i += 3) {
    GLushort a = out_elements[i];
    GLushort b = out_elements[i+1];
    GLushort c = out_elements[i+2];
    glm::vec3 normal = glm::normalize(glm::cross(
          glm::vec3(out_vertices[b]) - glm::vec3(out_vertices[a]),
          glm::vec3(out_vertices[c]) - glm::vec3(out_vertices[a])));
    out_normals[a] = out_normals[b] = out_normals[c] = normal;
  }

  return true;
}

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

  //FontRenderer fontRenderer;
  //fontRenderer.load("fonts/OpenSans-Regular.ttf");

  std::vector<glm::vec3> vertices_;
  std::vector<GLushort> elements;
  std::vector<glm::vec3> out_normals;

  loadObj("models/cube.obj", vertices_, out_normals, elements);
  std::cout << "Hello World, Cube :)\n";
  std::cout << "\tVertices: " << vertices_.size() << "\n";
  std::cout << "\tElements: " << elements.size() << "\n";
  std::cout << "\tNormals : " << out_normals.size() << "\n";

  glfwSetKeyCallback(window, cbKeyEvents);

  //GLfloat vertices[] = {
    //-0.5f, -0.5f,  0.0f,  // Top right
    //0.5f,  -0.5f,  0.0f,  // Bottom right
    //-0.5f, -0.5f,  0.0f,  // Bottom left
    //0.0f,   0.5f,  0.0f,  // Top left
  //};

    //GLfloat vertices[] = {
         //0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,// Top Right
         //0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,// Bottom Right
        //-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,// Bottom Left
    //};

  //GLuint indices[] = {
    //0, 1, 3,
    //1, 2, 3,
  //};




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
