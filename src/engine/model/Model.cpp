#include <SOIL.h>

#define GLM_FORCE_RADIANS

#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>

#include "Util.h"
#include "Model.h"
#include "engine/Texture2D.h"
#include "engine/Manager.h"


namespace engine {
namespace model {

Model::Model(::engine::resource::Manager *m, std::string const &p)
        : path(p),
          manager(m) {
  directory = path.substr(0, path.find_last_of('/'));
  directory += '/';
  loadModel();
  size_t count_v = 0;
  size_t count_i = 0;
  size_t count_t = 0;
  for (auto const &mesh : meshes) {
    count_v += mesh.getVertices().size();
    count_i += mesh.getIndices().size();
    count_t += mesh.getTextures().size();
  }
  std::cout << "Imported " << meshes.size() << " meshes with a total of\n"
            << count_v << " vertices,\n"
            << count_i << " indices and\n"
            << count_t << " textures.\n";
}


Model::~Model() {
  /*for(auto texture : loaded_textures) {
    if (texture != nullptr)
      delete texture;
  }*/

}

/*
 * we shouldn't need this.
 */
void Model::draw(::engine::shader::Compiler &shader) const {
  for (auto const &m : meshes) {
    m.draw(shader);
  }
}


void Model::draw() const {
  for (auto const &m : meshes) {
    m.draw();
  }
}


void Model::loadModel() {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(path,
                                           aiProcess_Triangulate |
                                           aiProcess_FlipUVs |
                                           aiProcess_GenNormals |
                                           aiProcess_JoinIdenticalVertices);
  if (!scene) {
    std::cerr << "Import error for " << path
              << ": " << importer.GetErrorString() << "\n";
    return;
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
  std::vector<Texture2D *> textures;

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
    std::vector<Texture2D *> diffuse_maps = loadTextures(mat,
                                                         aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
    std::vector<Texture2D *> specular_maps = loadTextures(mat,
                                                          aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());

    loaded_textures.insert(loaded_textures.end(), textures.begin(), textures.end());
  }

  for (size_t face_id = 0; face_id < mesh->mNumFaces; face_id++) {
    aiFace &f = mesh->mFaces[face_id];
    for (size_t i = 0; i < f.mNumIndices; i++) {
      indices.push_back(f.mIndices[i]);
    }
  }

  return model::Mesh(vertices, indices, textures);
}

std::vector<Texture2D *> Model::loadTextures(
        aiMaterial *mat, aiTextureType ai_type, std::string const &type_name) {
  std::vector<Texture2D *> texs;
  size_t count = mat->GetTextureCount(ai_type);
  texs.reserve(count);
  for (size_t i = 0; i < count; i++) {
    aiString tex_str;
    mat->GetTexture(ai_type, static_cast<unsigned int>(i), &tex_str);
    for (auto &t : loaded_textures) {
      if (t->getPath() == std::string(tex_str.C_Str())) {
        texs.push_back(t);
      }
    }

    Texture2D::Type type = Texture2D::Type::UNKNOWN;
    if (ai_type == aiTextureType_DIFFUSE) {
      type = Texture2D::Type::DIFFUSE;
    } else if (ai_type == aiTextureType_SPECULAR) {
      type = Texture2D::Type::SPECULAR;
    }
    Texture2D *texture = loadTexture(tex_str.C_Str(), directory.c_str(), type);
    if (texture == nullptr) {
      std::cout << "Failed to load texture " << tex_str.C_Str() << "\n";
      continue;
    }


    aiColor3D ambient(0.5f, 0.5f, 0.5f);
    mat->Get(AI_MATKEY_COLOR_DIFFUSE, texture->diffuse_);
    mat->Get(AI_MATKEY_COLOR_AMBIENT, texture->ambient_);
    mat->Get(AI_MATKEY_COLOR_SPECULAR, texture->specular_);
    mat->Get(AI_MATKEY_SHININESS, texture->shininess_);
    texs.push_back(texture);
  }
  return texs;
}

Texture2D *Model::loadTexture(const char *file, const char *directory, Texture2D::Type type) {
  int width = 0, height = 0;
  int channels = 0;
  unsigned char *image = nullptr;
  std::string path(directory);
  path += file;

  // window -> unix
  util::replaceAllOccurences(path, "\\\\", "/");
  util::replaceAllOccurences(path, "\\", "/");

  if (!util::fileExists(path)) {
    return nullptr;
  }

  image = SOIL_load_image(path.c_str(),
                          &width, &height, &channels, SOIL_LOAD_RGB);
  if (image == nullptr) {
    std::cerr << "Failed to load texture\n";
    return nullptr;
  }

  Texture2D *texture = manager->loadAsset<Texture2D>(type, image, width, height, path);
  SOIL_free_image_data(image);

  return texture;
}

} // end namespace engine::model
}
