#include "Scene.h"

namespace engine {

void Scene::draw() {
  //if (enabled_fps) {
  //  fps_counter.draw();
  //}

  //drawPos();

  for (auto const& o : objects) {
    o->draw(*this);
  }
}

//void Scene::drawPos() {
  //glm::vec3 camera_pos = camera->getPosition();
  //std::string str_pos_x = "X: " + std::to_string(camera_pos.x);
  //std::string str_pos_y = "Y: " + std::to_string(camera_pos.y);
  //std::string str_pos_z = "Z: " + std::to_string(camera_pos.z);

  //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //font.render(str_pos_x, 20.0f, input.height - 120.0f, glm::vec3(0.5f, 0.8f, 0.2f));
  //font.render(str_pos_y, 20.0f, input.height - 160.0f, glm::vec3(0.5f, 0.8f, 0.2f));
  //font.render(str_pos_z, 20.0f, input.height - 200.0f, glm::vec3(0.5f, 0.8f, 0.2f));
//}


} // end namespace engine
