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
#include "engine/Manager.h"

#include <vector>

namespace engine {

namespace resource {
class Manager;
}
class Model {
  public:
    Model(::engine::resource::Manager* m, std::string const& path);

    ~Model();

    void draw(::engine::shader::Compiler& shader) const;

    void draw() const;

  private:
    FRIEND_TEST(Model, Loading);
    FRIEND_TEST(Model, FailLoad);
    std::string directory;
    std::string path;
    ::engine::resource::Manager* manager;
    std::vector<model::Mesh> meshes;
    std::vector<Texture2D*> loaded_textures;


    //! Loads a 3D model using the Assimp importer.
    void loadModel();

    /*!
     * Processes model nodes recursively using Assimp.
     *
     * @param node Node to be processed
     * @param scene Scene read from model
     */
    void processNode(aiNode const *node, const aiScene *scene);

    /*!
     * Parses assimp meshes for various information.
     *
     * Currently supported:
     * - vertices
     * - indices
     * - textures
     *
     * @param mesh processing mesh
     * @param scene corresponding scene
     * @return Object with information included
     */
    model::Mesh processMesh(aiMesh const *mesh, const aiScene *scene);

    /*!
     * Loads textures from a given material.
     *
     * @param mat material to be parsed
     * @param type texture type
     * @param type_name texture type string representation
     * @return vector of weak pointers to the textures
     */
    std::vector<Texture2D*> loadTextures(
            aiMaterial *mat, aiTextureType type, std::string const& type_name);

    /*!
     * Loads a texture from the filesystem.
     *
     * @param file filename of texture
     * @param directory directory of texture
     * @param type texture type
     * @return weak pointer to the loaded texture on success,
     *         or `nullptr` on error.
     */
    Texture2D* loadTexture(const char *file, const char *directory,
                        Texture2D::Type type);

};

} // end namespace engine
#endif
