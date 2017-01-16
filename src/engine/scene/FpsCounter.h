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

    void setPosition(int x, int y);

    void draw();

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
