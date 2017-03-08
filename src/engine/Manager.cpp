//
// Created by spotlight on 3/6/17.
//

#include "Manager.h"
#include "Model.h"


namespace engine {

namespace resource {
/*!
 * Generates a identifier for the resource `name` of type `type`.
 * @param type resource type
 * @param name resource name
 * @return identifier as string
 */
/*std::string Manager::getId(Type type, std::string const &name) {
  return std::string(type2str(type)) + '/'+ name;
}*/

/*!
 * Template spezialization to load models from file.
 * @param path path to the asset
 * @return weak-pointer to the loaded resource
 */
/*template<>
::engine::Model* Manager::loadAsset<engine::Model, char const*>(char const* path) {
  using ResModel = Resource<::engine::Model>;

  std::string id = getId(Type::OBJ_MODEL, std::string(path));


  // check if already loaded
  auto found = resources_.find(id);
  if (found != resources_.end()) {
    auto base_ptr = (*found).second.get();
    return static_cast<ResModel*>(base_ptr)->get();
  }

  std::unique_ptr<::engine::Model> model_ptr;

  model_ptr = std::make_unique<::engine::Model>(path, *this);

  std::unique_ptr<ResModel> base_ptr = std::make_unique<ResModel>(model_ptr.get());

  ResModel* weak_ptr = base_ptr.get();
  resources_.emplace(id, std::move(base_ptr));
  return weak_ptr->get();
}*/

/*!
 * Creates a new resource for a given texture.
 *
 * @param type type of texture
 * @param image image data
 * @param width image width
 * @param height image height
 * @param path path to the texture data
 * @return weak-ptr to the asset
 */
/*template<>
::engine::Texture2D* Manager::loadAsset<::engine::Texture2D,
        ::engine::Texture2D::Type, unsigned char*,
                int, int, std::string>(::engine::Texture2D::Type type,
                                       unsigned char* image,
                                       int width,
                                       int height,
                                       std::string /*TODO: const-ref! */ //path) {
  /*std::cerr << getId(Type::TEXTURE2D, path) << "\n";
  return nullptr;

}*/



} // end namespace engine::resource
}
