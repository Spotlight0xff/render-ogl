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

template<typename T>
class Resource : public Base {
  public:
    //! Take ownership of the raw pointer
    //! \param obj raw pointer to the object
    Resource(T* obj)
    : obj(obj){
    }

    Resource()
    : obj(new T){
    }

    Resource(std::unique_ptr<T> const& obj) : obj(obj) {}
    Resource(std::unique_ptr<T> const&& obj) : obj(std::move(obj)) {}

    T* get() {
      return obj.get();
    }

  private:
    std::unique_ptr<T> obj;
    Type type;
};


class Manager {
  public:
    Manager();

    Manager(Manager const &other) = delete; // copy ctor
    Manager &operator=(Manager const &other) = delete; // copy-assignment op

    /* TODO:
     * - register new resource types
     * - debug information (memory used, etc.)
     * - optimize vertex data
     */


    /*!
     * loads the specified object from the given path
     * @tparam T type of the resource
     * @param path path of the asset
     * @return weak raw pointer to the asset
     */
    //template<typename T, typename... Args>
    //T* loadAsset(Args... args);

    template<typename T, typename... Args>
    T* loadAsset(Args... args) {
      std::string id = getId<T>(std::forward<Args>(args)...);
      std::cerr << "Loading asset: " << id << "\n";


      // check if already loaded
      auto found = resources_.find(id);
      if (found != resources_.end()) {
        auto base_ptr = (*found).second.get();
        return static_cast<Resource<T>*>(base_ptr)->get();
      }

      std::unique_ptr<T> ptr;

      ptr = std::make_unique<T>(*this, std::forward<Args>(args)...);

      std::unique_ptr<Resource<T>> base_ptr = std::make_unique<Resource<T>>(ptr.get());

      Resource<T>* weak_ptr = base_ptr.get();
      resources_.emplace(id, std::move(base_ptr));
      return weak_ptr->get();
    }


    template<typename T, typename... Args>
    std::string getId(Args... args) {
      std::cerr << "identifier: " << T::id << "\n";
      return std::string(T::id) + "@" + T::getId(std::forward<Args>(args)...);
    };

    /*!
     * returns an unique stub pointer to the requested resource type and creates it
     *
     *
     * @tparam T type of the resource
     * @return weak raw pointer (non-owning) to the created resource
     */
    template<typename T>
    T* make() {
      static size_t counter = 0;
      std::unique_ptr<Resource<T>> ptr = std::make_unique<Resource<T>>();
      T* weak_ptr = ptr.get()->get();
      resources_.emplace("Memory@" + std::to_string(counter), std::move(ptr));
      return weak_ptr;
    }

  private:
    //! Resource map of *all* resources
    std::map <std::string, std::unique_ptr<Base>> resources_;
};

} // end namespace engine::resource
}


#endif //GRAPHICS_ENGINE_RESOURCE_MANAGER_H
