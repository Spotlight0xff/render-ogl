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

#include "Mesh.h"

Mesh::Mesh(const char* path, Shader const& s)
  : shader(s) {
  Assimp::Importer importer;

  const aiScene* scene = importer.ReadFile(path,
      aiProcess_CalcTangentSpace |
      aiProcess_Triangulate |
      aiProcess_JoinIdenticalVertices |
      aiProcess_SortByPType);
  if (!scene) {
    std::cerr << "Import error for " << path
      << ": " << importer.GetErrorString() << "\n";
  }

  if (!scene->HasMeshes()) {
    std::cerr << "Object file " << path << " contains no meshes.\n";
  }
  std::cout << "This object has " << scene->mNumMeshes << " meshes\n";

  for (size_t mesh_id = 0; mesh_id < scene->mNumMeshes; mesh_id ++) {
    aiMesh* mesh = scene->mMeshes[mesh_id];
    if (!mesh->HasFaces() || !mesh->HasPositions()) {
      std::cerr << "Mesh[" << mesh_id << "] doesn't contain faces or positions.\n";
      continue;
    }

    for (size_t vertex_id = 0; vertex_id < mesh->mNumVertices; vertex_id ++) {
      aiVector3D const& v = mesh->mVertices[vertex_id];
      Vertex vertex;
      vertex.position = glm::vec3(v.x, v.y, v.z);
      vertex.normal = glm::vec3(0, 0, 0);
      vertex.tex_coord = glm::vec3(0, 0, 0);

      vertices.push_back(vertex);
    }

    for (size_t face_id = 0; face_id < mesh->mNumFaces; face_id ++) {
      aiFace& f = mesh->mFaces[face_id];
      for(size_t i = 0; i < f.mNumIndices; i ++) {
        indices.push_back(f.mIndices[i]);
      }
    }
    std::cout << "Object parsing done:\n"
      << vertices.size() << " vertices\n"
      << indices.size() << " indices\n";
  }

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
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, normal)));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, tex_coord)));

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // unbind VAO
  glBindVertexArray(0);
  return true;
}

void Mesh::draw() {
  shader.use();
  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(vertices.size()), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
