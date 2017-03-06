#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define FRIEND_TEST(test_case_name, test_name)\
friend class test_case_name##_##test_name##_Test

#include "engine/model/Mesh.h"
#include "Texture2D.h"
#include "engine/shader/compiler.h"

#include <vector>

namespace engine {

class Model {
  public:
    Model(const char *path);
    ~Model();

    void draw(::engine::shader::Compiler& shader) const;

    void draw() const;

  private:
    FRIEND_TEST(Model, Loading);
    FRIEND_TEST(Model, FailLoad);
    std::string directory;
    std::string path;
    std::vector<model::Mesh> meshes;
    std::vector<Texture2D*> loaded_textures;


    void loadModel();

    void processNode(aiNode const *node, const aiScene *scene);

    model::Mesh processMesh(aiMesh const *mesh, const aiScene *scene);

    std::vector<Texture2D*> loadTextures(
            aiMaterial *mat, aiTextureType type, std::string type_name);

    Texture2D* loadTexture(const char *file, const char *directory,
                        Texture2D::Type type);

};

} // end namespace engine
#endif
