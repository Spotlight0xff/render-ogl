//
// Created by spotlight on 3/6/17.
//

#ifndef GRAPHICS_ENGINE_RESOURCEMANAGER_H
#define GRAPHICS_ENGINE_RESOURCEMANAGER_H


namespace engine {

template<typename T>
class Resource {};

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
 */
class ResourceManager {
  public:
    ResourceManager();

    ResourceManager(ResourceManager const& other) = delete; // copy ctor
    ResourceManager& operator=(ResourceManager const& other) = delete; // copy-assignment op


    //! loads the specified object from the given path
    template<typename T>
    Resource<T> loadResource(std::string const& path) {

    }


  private:
    std::map<std::string, std::unique_ptr<Resource>> resources_;
};

} // end namespace engine


#endif //GRAPHICS_ENGINE_RESOURCEMANAGER_H
