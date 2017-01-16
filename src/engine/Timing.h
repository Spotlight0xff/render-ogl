//
// Created by spotlight on 1/16/17.
//

#ifndef GRAPHICS_ENGINE_TIMING_H
#define GRAPHICS_ENGINE_TIMING_H

#include "opengl.h"


namespace engine {

class Timing {
  public:

    Timing();
    void Start();
    GLint64 waitStop();

  private:
    GLuint query_timers[2];


};

} // end namespace engine

#endif //GRAPHICS_ENGINE_TIMING_H
