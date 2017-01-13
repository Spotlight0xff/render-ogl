#ifndef MODELOBJECTPHONG_H
#define MODELOBJECTPHONG_H

#include "Model.h"
#include "ModelObject.h"
#include "ShaderCompiler.h"
#include "SceneObject.h"
#include "LightObject.h"
#include "Scene.h"

#include <glm/gtc/type_ptr.hpp>

class ModelObjectPhong : public ModelObject {
 public:
  ModelObjectPhong(Model* m, SceneObject const& l, Scene* s);
  
 private:
  SceneObject const& light;
};
#endif
