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

namespace engine {
namespace resource {


class Base {
  public:
    virtual ~Base() {}
  private:

};

template<typename T>
class Resource : public Base {
  public:
    Resource(T* obj)
    : obj_(obj){
    }

    Resource()
    : obj_(new T){
    }

    Resource(const std::unique_ptr<T> &obj_) : obj_(obj_) {}

    T* get() {
      return obj_.get();
    }

  private:
    std::unique_ptr<T> obj_;
};


class Manager {
  public:
    Manager();

    Manager(Manager const &other) = delete; // copy ctor
    Manager &operator=(Manager const &other) = delete; // copy-assignment op


    //! loads the specified object from the given path
    template<typename T>
    T loadAsset(std::string const &path) {
    }


    std::string getUniqueName() {
      return "test_abc123";
    }

    template<typename T>
    T* make() {
      std::unique_ptr<Resource<T>> ptr = std::make_unique<Resource<T>>();
      T* weak_ptr = ptr.get()->get();
      resources_.emplace("abc", std::move(ptr));
      return weak_ptr;
    }

    /*!
     * returns an unique stub pointer to the requested resource type
     * @tparam T requested type
     * @return unique stub pointer
     */
    /*template<typename T>
    Resource<T>* getUnique() {
      // returns a new
      std::string unique_name = getUniqueName();
      std::unique_ptr<Base> ptr(new Base);
      resources_.emplace(unique_name, std::move(ptr));
      return (Resource <T> *) ptr.get();
    }*/

  private:
    std::map <std::string, std::unique_ptr<Base>> resources_;
};

} // end namespace engine::resource
}


#endif //GRAPHICS_ENGINE_RESOURCE_MANAGER_H
