//
// Created by spotlight on 3/6/17.
//

#ifndef GRAPHICS_ENGINE_SCENEOBJECT_H
#define GRAPHICS_ENGINE_SCENEOBJECT_H


namespace engine {

class Scene;

//! @brief Drawable scene object
class SceneObject {
  public:
    virtual ~SceneObject() {}

    virtual void draw(Scene&) = 0;

  private:
};
}

#endif //GRAPHICS_ENGINE_SCENEOBJECT_H
