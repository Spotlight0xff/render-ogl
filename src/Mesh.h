#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <assimp/scene.h>

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
  Mesh(aiMesh const* mesh, aiScene const* scene,
    std::string const& path);

  Mesh(std::vector<Vertex> vertices,
       std::vector<GLuint> indices,
       std::vector<Texture> textures);
  bool setup();
  void draw(Shader const& shader) const;

  std::vector<GLuint> getIndices() const{
    return indices;
  }

 private:
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;
  std::vector<Texture> textures;

  GLuint vao = 0;
  GLuint vbo_vertices = 0;
  GLuint ebo_indices = 0;
};
#endif
