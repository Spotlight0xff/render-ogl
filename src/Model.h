#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Texture.h"
#include "ShaderCompiler.h"

#include <vector>

class Model {
 public:
  Model(const char* path);

  void draw(Shader const& shader) const;

  void setPosition(glm::vec3 pos);
  glm::vec3 getPositon();
  glm::mat4 getModelMatrix();

  // TODO: move(delta_vec)

 private:
  std::string directory;
  std::string path;
  std::vector<Mesh> meshes;
  std::vector<Texture> loaded_textures;

  glm::vec3 position;

  void loadModel();
  void processNode(aiNode const* node, const aiScene* scene);
  Mesh processMesh(aiMesh const* mesh, const aiScene* scene);
  std::vector<Texture> loadTextures(
      aiMaterial* mat, aiTextureType type, std::string type_name);
  Texture loadTexture(const char* path, const char* directory);
};
#endif