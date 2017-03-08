#ifndef ENGINE_SCENE_H
#define ENGINE_SCENE_H

#include "engine/shader/compiler.h"
#include "engine/handler/Camera.h"
#include "engine/Input.h"
#include "engine/Manager.h"

#include "engine/SceneObject.h"

//#include "ModelObject.h"
//#include "LightObject.h"

#include "opengl.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <functional>
#include <engine/handler/Camera.h>
#include <engine/components/ModelObject.h>

namespace engine {

class Model;


class Scene {
  public:

    Scene(resource::Manager* m)
    :manager(m){}

    /*!
     * Adds the model as an object to the scene.
     *
     *
     * @param model Model to be added
     * @return
     */
    SceneObject* addModel(::engine::Model* model);

    // TODO: addAsset (which calls the resource manager directly)
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



    void draw();

    void drawPos();

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
