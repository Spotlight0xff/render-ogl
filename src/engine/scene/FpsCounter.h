#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

//#include "Scene.h"
#include "engine/Input.h"
#include "FontRender.h"

#include "opengl.h"

namespace engine {
namespace scene {

class FpsCounter {
  public:
    FpsCounter(FontRenderer &f, Input &inp)
            : fps_string("Initializing..."),
              font(f), input(inp) {
    }

    //void setScene(Scene* s) {
    //scene = s;
    //}

    void setPosition(int x, int y) {
      pos_x = x;
      pos_y = y;
    }

    void draw() {
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
      }

      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      //fontRenderer.render(performance_str, 20.0f, height - 300.0f, glm::vec3(0.5,0.8f, 0.2f));
      font.render(fps_string, 20.0f, input.height, glm::vec3(0.5, 0.8f, 0.2f));
    }

  private:
    int pos_x = -1;
    int pos_y = -1;
    int n_frames = 0;
    GLfloat last_time = 0.0f;
    std::string fps_string;

    FontRenderer &font;

    // Used to query height & width
    Input &input;
};

} // end namespace engine::scene
}

#endif
