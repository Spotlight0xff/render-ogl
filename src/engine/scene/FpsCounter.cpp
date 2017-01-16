//
// Created by spotlight on 1/16/17.
//

#include "FpsCounter.h"

namespace engine {
namespace scene {

void FpsCounter::setPosition(int x, int y) {
  pos_x = x;
  pos_y = y;
}


void FpsCounter::draw() {
  n_frames++;
  GLfloat current_time = GLfloat(glfwGetTime());

  if (current_time - last_time >= 1.0f) {
    GLfloat frame_ms = 1000.0f / GLfloat(n_frames);
    GLfloat fps = n_frames;
    std::cout << frame_ms << " ms/frame\n";

    n_frames = 0;
    last_time += 1.0;
    //performance_str = std::to_string(frame_ms) + " ms/frame";
    fps_string = std::to_string(fps) + " FPS";
    font.updateWindow(input.width, input.height);
  }

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //fontRenderer.render(performance_str, 20.0f, height - 300.0f, glm::vec3(0.5,0.8f, 0.2f));
  font.render(fps_string, 20.0f, input.height - 80.0f, glm::vec3(0.5, 0.8f, 0.2f));
}

} // end namespace engine::scene
}