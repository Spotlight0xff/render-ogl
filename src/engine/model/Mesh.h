#ifndef MESH_H
#define MESH_H

#include "opengl.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include <assimp/scene.h>

#include <vector>

#include "Vertex.h"
#include "engine/Texture2D.h"
#include "engine/ShaderCompiler.h"

namespace engine {
namespace model {

class Mesh {
  public:
    Mesh(std::vector<Vertex> vertices,
         std::vector<GLuint> indices,
         std::vector<Texture2D> textures);

    bool setup();

    // draw with shader (textures mostly)
    void draw(Shader &shader) const;

    // and without
    void draw() const;

    std::vector<GLuint> getIndices() const { return indices; }

    std::vector<Vertex> getVertices() const { return vertices; }

    std::vector<Texture2D> getTextures() const { return textures; }

  private:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture2D> textures;

    GLuint vao = 0;
    GLuint vbo_vertices = 0;
    GLuint ebo_indices = 0;
};

} // end namespace engine::model
} // end namespace engine
#endif
