#include "Scene.h"

void Scene::draw() {
  if (enabled_fps) {
    fps_counter.draw();
  }

  for (auto o : objects) {
    o->draw();
  }
}
