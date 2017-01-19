//
// Created by spotlight on 1/18/17.
//

#include <cstring>
#include "System.h"

namespace engine {
namespace particles {

System::System ()
        : shader_vb_("particles/vertex_update"),
          random_tex_(MAX_PARTICLES) {
}

System::~System() {
  glDeleteTransformFeedbacks(2, transform_feedbacks_);
  glDeleteBuffers(2, particle_buffers_);
}

void System::initParticles(glm::vec3 const& pos) {
  Particle particles[MAX_PARTICLES] = {};

  // first particle is the launcher,
  // its velocity determines the particles initial velocity
  particles[0] = {
          .type = ParticleType::LAUNCHER,
          .position = pos,
          .velocity = glm::vec3(0.0, 0.001, 0.0),
          .lifetime_ms = 0.0f,
  };

  glGenTransformFeedbacks(2, transform_feedbacks_);
  glGenBuffers(2, particle_buffers_);

  // create the initial particles
  for (unsigned int i=0; i < 2; i ++) {
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transform_feedbacks_[i]);
    glBindBuffer(GL_ARRAY_BUFFER, particle_buffers_[i]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_DYNAMIC_DRAW);

    // place transform feedback buffer at index 0
    glBindBufferBase(GL_TRANSFORM_FEEDBACK, 0, particle_buffers_[i]);

  }
}

void System::draw(::engine::Scene &scene) {
  GLfloat delta_time = 0.01;
  time_ += delta_time; // TODO: delta time

  updateParticles(delta_time);
  renderParticles(scene);

  // switch buffers
  current_vb_ = current_tfb_;
  current_tfb_ = (current_tfb_ + 1 ) & 0x01;
}


//
void System::updateParticles(GLfloat delta_time) {
  shader_vb_.use();
  shader_vb_.set("time", time_);
  shader_vb_.set("delta_time", delta_time);

  // just bind it to the first texture (valid?)
  random_tex_.bind(GL_TEXTURE0);


}

void System::renderParticles(::engine::Scene &scene) {

}


} // end namespace engine::particles
}
