#include <SOIL.h>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

#include "Util.h"
#include "Model.h"

namespace engine {

Model::Model(const char *p)
        : path(p) {
  directory = path.substr(0, path.find_last_of('/'));
  directory += '/';
  loadModel();
  size_t count_v = 0;
  size_t count_i = 0;
  size_t count_t = 0;
  for (auto const &m : meshes) {
    count_v += m.getVertices().size();
    count_i += m.getIndices().size();
    count_t += m.getTextures().size();
  }
  std::cout << "Imported " << meshes.size() << " meshes with a total of\n"
            << count_v << " vertices,\n"
            << count_i << " indices and\n"
            << count_t << " textures.\n";
}

void Model::draw(Shader const &shader) const {
  for (auto const &m : meshes) {
    m.draw(shader);
  }
}


void Model::loadModel() {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(path,
                                           aiProcess_Triangulate |
                                           aiProcess_FlipUVs |
                                           aiProcess_GenNormals);
  if (!scene) {
    std::cerr << "Import error for " << path
              << ": " << importer.GetErrorString() << "\n";
  }

  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode const *node, const aiScene *scene) {
  meshes.reserve(meshes.size() + node->mNumMeshes);
  for (size_t i = 0; i < node->mNumMeshes; i++) {
    aiMesh const *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }

  for (size_t i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

model::Mesh Model::processMesh(aiMesh const *mesh, const aiScene *scene) {
  std::vector<model::Vertex> vertices;
  std::vector<GLuint> indices;
  std::vector<model::Texture> textures;

  const glm::vec3 zero_vec(0.0f);
  vertices.reserve(mesh->mNumVertices);
  for (size_t vertex_id = 0; vertex_id < mesh->mNumVertices; vertex_id++) {
    aiVector3D const &v = mesh->mVertices[vertex_id];
    model::Vertex vertex;
    vertex.position = glm::vec3(v.x, v.y, v.z);
    vertex.normal = glm::vec3(0, 0, 0);

    // Set texture coordinates
    if (mesh->HasTextureCoords(0)) {
      aiVector3D const &tex_coord = mesh->mTextureCoords[0][vertex_id];
      vertex.tex_coord = glm::vec2({tex_coord.x, tex_coord.y});
    }

    // Set normal vectors
    if (&mesh->mNormals[vertex_id]) {
      aiVector3D const *n = &mesh->mNormals[vertex_id];
      vertex.normal = glm::vec3(n->x, n->y, n->z);
    }

    vertices.push_back(vertex);
  }
  if (mesh->mMaterialIndex) {
    aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<model::Texture> diffuse_maps = loadTextures(mat,
                                                     aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
    std::vector<model::Texture> specular_maps = loadTextures(mat,
                                                      aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
  }

  for (size_t face_id = 0; face_id < mesh->mNumFaces; face_id++) {
    aiFace &f = mesh->mFaces[face_id];
    for (size_t i = 0; i < f.mNumIndices; i++) {
      indices.push_back(f.mIndices[i]);
    }
  }

  return model::Mesh(vertices, indices, textures);
}

std::vector<model::Texture> Model::loadTextures(
        aiMaterial *mat, aiTextureType ai_type, std::string type_name) {
  std::vector<model::Texture> texs;
  size_t count = mat->GetTextureCount(ai_type);
  texs.reserve(count);
  for (size_t i = 0; i < count; i++) {
    aiString tex_str;
    mat->GetTexture(ai_type, static_cast<unsigned int>(i), &tex_str);
    for (auto const &t : loaded_textures) {
      if (t.path == std::string(tex_str.C_Str())) {
        texs.push_back(t);
      }
    }

    model::TextureType type = model::TextureType::UNKNOWN;
    if (ai_type == aiTextureType_DIFFUSE) {
      type = model::TextureType::DIFFUSE;
    } else if (ai_type == aiTextureType_SPECULAR) {
      type = model::TextureType::SPECULAR;
    }
    model::Texture texture = loadTexture(tex_str.C_Str(), directory.c_str(), type);

    aiColor3D ambient(0.f, 0.f, 0.f);
    mat->Get(AI_MATKEY_COLOR_DIFFUSE, texture.diffuse);
    mat->Get(AI_MATKEY_COLOR_AMBIENT, texture.ambient);
    mat->Get(AI_MATKEY_COLOR_SPECULAR, texture.specular);
    mat->Get(AI_MATKEY_SHININESS, texture.shininess);
    texs.push_back(texture);
  }
  return texs;
}

model::Texture Model::loadTexture(const char *file, const char *directory, model::TextureType type) {
  model::Texture texture;
  texture.type = type;
  int width = 0, height = 0;
  unsigned char *image = nullptr;
  std::string path(directory);
  path += file;
  image = SOIL_load_image(path.c_str(),
                          &width, &height, 0, SOIL_LOAD_RGB);
  if (image == nullptr) {
    std::cerr << "Failed to load texture\n";
    return {};
  }
  texture.path = path;


  // handle textures

  glGenTextures(1, &texture.id);
  glBindTexture(GL_TEXTURE_2D, texture.id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // Set texture wrapping to GL_REPEAT
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // Set texture filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
               0, GL_RGB, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);
  SOIL_free_image_data(image);
  glBindTexture(GL_TEXTURE_2D, 0); // unbind
  return texture;
}

} // end namespace engine
