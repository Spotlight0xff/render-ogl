#ifndef ENGINE_SCENE_H
#define ENGINE_SCENE_H

#include "engine/shader/compiler.h"
#include "engine/handler/Camera.h"
#include "engine/Manager.h"

#include "engine/SceneObject.h"

#include "opengl.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <functional>
#include <engine/handler/Camera.h>
#include <engine/components/ModelObject.h>

namespace engine {

//! @brief Manages the state of a scene including its objects
class Scene {
  public:

    explicit Scene(resource::Manager* m)
    :manager(m){}

    void useCamera(::engine::scene::Camera* camera) {
      camera_ = camera;
    }

     /*!
      * Add an arbitrary object to the scene.
      * @tparam T object type
      * @tparam Args constructor argument types
      * @param args constructor arguments
      * @return object of type `T`
      */
    template<typename T, typename... Args>
    T* addModel(Args... args) {
      T* ptr = manager->loadAsset<T>(std::forward<Args>(args)...);
      objects.emplace_back(static_cast<SceneObject*>(ptr));
      return ptr;
    }

    //void addLight(Light const& light);
    //void addModel(Model model, Shader shader) {
    //}

    //void addObjectRef(::engine::SceneObject* object) {
    //  objects.push_back(object);
    //}

    //void setLight(scene::SceneObject* l) {
    //  objects.push_back(l);
    //}

    //void UseCamera(scene::Camera* cam) { camera_ = cam; }

    //void enableFpsCounter() { enabled_fps = true; }

    //void useFont(const char* path) { font.load(path); }

    ::engine::scene::Camera& getCameraRef() const {
      return *camera_;
    }




    void draw();

    glm::mat4 getProjectionMatrix() {
      int width = 1280;
      int height = 1024;
      return glm::perspective(45.0f, 1.0f*width/height, 0.1f, 1000.0f);
    }

    //Input & getInputRef() { return input; }
    //scene::FontRenderer& getFontRef() { return font; }
    //scene::Camera const& getCameraRef() const { return *camera_; }
    //SceneObject const& getLightRef() const { return *light; }


  private:
    scene::Camera* camera_{nullptr};
    resource::Manager* manager{nullptr};

    std::vector<SceneObject*> objects;
};
} // end namespace engine
#endif
