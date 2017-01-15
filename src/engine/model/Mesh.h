#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <assimp/scene.h>

#include <vector>

#include "Vertex.h"
#include "Texture.h"
#include "engine/ShaderCompiler.h"

namespace engine {
namespace model {

class Mesh {
  public:
    Mesh(std::vector<Vertex> vertices,
         std::vector<GLuint> indices,
         std::vector<Texture> textures);

    bool setup();

    void draw(Shader const &shader) const;

    std::vector<GLuint> getIndices() const { return indices; }

    std::vector<Vertex> getVertices() const { return vertices; }

    std::vector<Texture> getTextures() const { return textures; }

  private:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    GLuint vao = 0;
    GLuint vbo_vertices = 0;
    GLuint ebo_indices = 0;
};

} // end namespace engine::model
} // end namespace engine
#endif
