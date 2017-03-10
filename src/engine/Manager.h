//
// Created by spotlight on 3/6/17.
//

/*
 * Design ideas:
 *
 * Implementing a flexible resource manager, based on the following principles:
 * - as simple as possible
 * - supporting hierarchical objects
 * - exporting objects to binary stream (protobuf/ordinary streams/etc.)
 *
 * Even though there might be multiple instances of the resource manager running,
 * it is generally not advised, because of decreased performance gains.
 *
 * It supports currently two different resource types:
 *
 * assets:
 *  These are assets loaded from file like images, 3D models, fonts, etc.
 *
 *
 * objects:
 *  Instances which need to be memory managed.
 *  These include scenes for example.
 *  TODO: add serialization by adding an interface.
 *
 */

#ifndef GRAPHICS_ENGINE_RESOURCE_MANAGER_H
#define GRAPHICS_ENGINE_RESOURCE_MANAGER_H


#include <memory>
#include <map>
#include <iostream>

namespace engine {
namespace resource {

//! available resource types
enum class Type : uint8_t {
    MEMORY = 0, //! in-memory resource, dynamically allocated
    TEXTURE2D = 1, //! 2-dimensional texture data
    OBJ_MODEL = 2, //! .obj 3D model loaded from file
    SHADER = 3, //! GLSL shader code (TODO!)
};

namespace {

struct Entry {
  Type type;
  char const *name;
};

/*!
 * Mapping from type to string.
 *
 * Has to have the same ordering as the numbered types in `Type`.
 */
constexpr Entry map[]{
        {Type::MEMORY,    "Memory"},
        {Type::TEXTURE2D, "Texture2D"},
        {Type::OBJ_MODEL, "ObjModel"},
        {Type::SHADER,    "Shader"},
};

//! Converts a type to the string representations
// pretty hacky :(
constexpr static char const* type2str(Type type) {
  return map[static_cast<std::underlying_type_t<Type>>(type)].name;
}
} // end anonymous namespace

class Base {
  public:
    virtual ~Base() {}
  private:
};

/*!
 * Resource object holding instances of type `T`.
 *
 * Inherits from Base in order to be able to put into a vector.
 *
 * @tparam T type of the object
 */
template<typename T>
class Resource : public Base {
  public:
    template<typename... Args>
    Resource(Args&&... args)
    : ptr(std::make_unique<T>(std::forward<decltype(args)>(args)...)){
    }

    T* get() {
      return ptr.get();
    }

  private:
    std::unique_ptr<T> ptr;
    Type type;
};


class Manager {
  public:
    Manager() = default;
    Manager(Manager const &other) = delete; // copy ctor
    Manager &operator=(Manager const &other) = delete; // copy-assignment op

    /* TODO:
     * - register new resource types
     * - debug information (memory used, etc.)
     * - optimize vertex data
     */


    /*!
    * loads the specified object from the given path
     * @tparam T type of the requested asset/object
     * @tparam Args constructor argument types
     * @param args constructor arguments
     * @return weak raw pointer to the asset
     */
    template<typename T, typename... Args>
    T* loadAsset(Args&&... args) {
      std::string id = getId<T>(std::forward<Args>(args)...);
      std::cerr << "Loading resource: " << id << "\n";


      // check if already loaded
      auto found = resources_.find(id);
      if (found != resources_.end()) {
        auto base_ptr = (*found).second.get();
        return static_cast<Resource<T>*>(base_ptr)->get();
      }

      // construct the smart pointer which holds Resource<T>
      // Resource<T> is a thin wrapper for unique_ptr allowing for more fine-grained control.
      std::unique_ptr<Resource<T>> base_ptr = std::make_unique<Resource<T>>(std::forward<decltype(args)>(args)...);

      // move the constructed pointer to the vector, but save the raw weak pointer first.
      Resource<T>* weak_ptr = base_ptr.get();
      resources_.emplace(id, std::move(base_ptr));
      return weak_ptr->get();
    }



    /*!
     * Generates a string identifying the class instance.
     *
     * Currently, this takes only the class name and a counter in consideration.
     *
     * Future implementations may use `args` to query the class to get a more precise identifier.
     *
     * @tparam T type of the requested instance (i.e. its class)
     * @tparam Args type of the arguments to the constructor
     * @param args arguments to the constructor
     * @return identifier for the object
     */
    template<typename T, typename... Args>
    std::string getId(Args... args) {
      static size_t counter = 0;
      // TODO: use boost/core/demangle to demand C++ class name
      // TODO: switch to a more robust implementation (e.g. .hash_code())
      // std::type_info.name() returns a string which may be the same between different types,
      // change from one program executation to another, and other nasty stuff.
      // more information: http://en.cppreference.com/w/cpp/types/type_info/name
      //return std::to_string(typeid(T).hash_code()) + "@" + std::to_string(counter++);
      return std::string(typeid(T).name()) + "@" + std::to_string(counter++);
    }

  private:
    //! Resource map of *all* resources
    std::map <std::string, std::unique_ptr<Base>> resources_;
};

} // end namespace engine::resource
}


#endif //GRAPHICS_ENGINE_RESOURCE_MANAGER_H
