#include <engine/shader/compiler.h>
#include "ShaderUpdater.h"


namespace engine {
namespace particles {

ShaderUpdater::ShaderUpdater()
  : shader_update_() {

}

bool ShaderUpdater::init() {
  shader_update_ = ::engine::shader::Compiler("particle_update");

  const GLchar* varyings[4];
  varyings[0] = "Type1";
  varyings[1] = "Position1";
  varyings[2] = "Velocity1";
  varyings[3] = "Age1";

  glTransformFeedbackVaryings(shader_update_.getId(), 4, varyings, GL_INTERLEAVED_ATTRIBS);



  loc_time_delta_ = shader_update_.getUniform("delta_time");
  loc_tex_loc_ = shader_update_.getUniform("random_texture");
  loc_time_ = shader_update_.getUniform("time");
  loc_launcher_lifetime_ = shader_update_.getUniform("launcher_lifetime");
  loc_shell_lifetime_ = shader_update_.getUniform("shell_lifetime");


  // TODO: check for errors of locations
}


void ShaderUpdater::setDeltatime(float deltatime)
{
  glUniform1f(loc_time_delta_, deltatime);
}


void ShaderUpdater::setTime(GLfloat time)
{
  glUniform1f(loc_time_, time);
}


void ShaderUpdater::setRandomTexUnit(unsigned int unit)
{
  glUniform1i(loc_tex_loc_, unit);
}


void ShaderUpdater::setLauncherLifetime(GLfloat lifetime)
{
  glUniform1f(loc_launcher_lifetime_, lifetime);
}


void ShaderUpdater::setShellLifetime(GLfloat lifetime)
{
  glUniform1f(loc_shell_lifetime_, lifetime);
}



} // end namespace engine::particles
}
