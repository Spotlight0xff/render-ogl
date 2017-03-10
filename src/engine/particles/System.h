#ifndef GRAPHICS_ENGINE_PARTICLES_SYSTEM_H
#define GRAPHICS_ENGINE_PARTICLES_SYSTEM_H

#include "RandomTexture.h"

#include <opengl.h>
#include <engine/shader/compiler.h>
#include <engine/Scene.h>

namespace engine {
namespace particles {

#define MAX_PARTICLES 5000

//! @brief particle type (launcher, shell, etc.)
enum class ParticleType {
    LAUNCHER = 0,
    SHELL,
    SECONDARY_SHELL,
};

//! @brief Structure to hold information about a single particle
struct Particle {
  ParticleType type;

  glm::vec3 position;
  glm::vec3 velocity;

  GLfloat lifetime_ms;
};

//! @brief Simple particle system
class System {
  public:
    System();
    ~System();

    void initParticles(glm::vec3 const& pos);

    void draw(::engine::Scene& scene);

    void updateParticles(GLfloat delta_time);
    void renderParticles(::engine::Scene& scene);

  private:
    unsigned int current_vb_ = 0;
    unsigned int current_tfb_ = 1;
    GLuint particle_buffers_[2];
    GLuint transform_feedbacks_[2];

    // current time
    GLfloat time_ = 0.0;

    // shader for vertex update
    ::engine::shader::Compiler shader_vb_;

    // random texture
   RandomTexture random_tex_;
};

} // end namespace engine::particles
}

#endif
