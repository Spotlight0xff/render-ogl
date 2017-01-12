#include <GL/glew.h>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SOIL.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "Vertex.h"
#include "Texture.h"
#include "Util.h"

#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> v,
       std::vector<GLuint> ind,
       std::vector<Texture> tex)
  : vertices(v),
    indices(ind),
    textures(tex)
{
  // Copy stuff into GPU & so on
  setup();
}

bool Mesh::setup() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo_vertices);
  glGenBuffers(1, &ebo_indices);

  // bind vertex array object first
  glBindVertexArray(vao);



  // handle vertices
  glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  // handle indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_indices);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()  * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

  // Tell OpenGL how to interpret our data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<GLvoid*>(0));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, normal)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, tex_coord)));
  glEnableVertexAttribArray(2);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // unbind VAO
  glBindVertexArray(0);
  return true;
}

void Mesh::draw(Shader const& shader) const{
  for(auto const& t : textures) {
    glBindTexture(GL_TEXTURE_2D, t.id);
  }
  shader.use();
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertices.size()), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
