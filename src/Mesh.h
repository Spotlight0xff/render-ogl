#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>

#include "Vertex.h"
#include "Texture.h"
#include "ShaderCompiler.h"

enum class FaceFormat {
  SPACE,
  SLASH,
  DOUBLESLASH
};

class Mesh {
 public:
  Mesh(const char* path, Shader const& shader);
  bool setup();
  void draw();

 private:
  Shader shader;
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  std::vector<Texture> textures;
  
  GLuint vao = 0;
  GLuint vbo_vertices = 0;
  GLuint ebo_indices = 0;
};
#endif
