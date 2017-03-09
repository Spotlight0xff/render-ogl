#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SOIL.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <engine/Model.h>

#include "Vertex.h"
#include "engine/Texture.h"
#include "Util.h"

#include "Mesh.h"

namespace engine {
namespace model {
Mesh::Mesh(std::vector<Vertex> v,
           std::vector<GLuint> ind,
           std::vector<Texture2D*> tex)
        : vertices(v),
          indices(ind),
          textures(tex) {
  // Copy stuff into GPU & so on
  setup();
}

Mesh::~Mesh() {
  //if (vbo_vertices) {
  //  glDeleteBuffers(1, &vbo_vertices);
  //}

  //if (ebo_indices) {
  //  glDeleteBuffers(1, &ebo_indices);
  //}

  //if (vao) {
  //  glDeleteVertexArrays(1, &vao);
  //}
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
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

  // Tell OpenGL how to interpret our data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<GLvoid *>(0));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid *>(offsetof(Vertex, normal)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        reinterpret_cast<GLvoid *>(offsetof(Vertex, tex_coord)));
  glEnableVertexAttribArray(2);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // unbind VAO
  glBindVertexArray(0);
  return true;
}

void Mesh::draw(::engine::shader::Compiler &shader) const {
  size_t i = 0;
  for (auto const &t : textures) {
    if (t == nullptr)
      continue; // this should *NOT* happen.

    shader.set("material.shininess", GLfloat(t->shininess_));
    //shader.set("material.ambientStrength", GLfloat(0.5));

    if (t->getType() == ::engine::Texture2D::Type::DIFFUSE) {
      shader.set("material.diffuse", GLint(i));

    } else if (t->getType() == ::engine::Texture2D::Type::SPECULAR) {
      shader.set("material.specular", GLint(i));
    }
    t->bind(GL_TEXTURE0 + i);
    i++;
  }
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Mesh::draw() const {
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

} // end namespace engine::model
}
