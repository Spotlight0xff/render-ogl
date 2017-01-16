//
// Created by spotlight on 1/16/17.
//

#include "Timing.h"

namespace engine {

Timing::Timing() {
  glGenQueries(2, query_timers);
}

void Timing::Start() {
  glQueryCounter(query_timers[0], GL_TIMESTAMP);
}

GLint64 Timing::waitStop() {
  GLint timer_available = 0;
  GLuint64 start_time, stop_time;
  glQueryCounter(query_timers[1], GL_TIMESTAMP);

  while (!timer_available) {
    glGetQueryObjectiv(query_timers[1],
                       GL_QUERY_RESULT_AVAILABLE,
                       &timer_available);
  }

  glGetQueryObjectui64v(query_timers[0], GL_QUERY_RESULT, &start_time);
  glGetQueryObjectui64v(query_timers[1], GL_QUERY_RESULT, &stop_time);
  return (stop_time - start_time);
}

}