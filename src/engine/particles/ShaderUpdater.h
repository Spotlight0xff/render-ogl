#ifndef GRAPHICS_ENGINE_SHADERUPDATER_H
#define GRAPHICS_ENGINE_SHADERUPDATER_H

namespace engine {
namespace particles {

//! @brief Shader updating the particles information
class ShaderUpdater {
  public:
    ShaderUpdater();

    bool init();

    void setParticleLifetime(GLfloat t);
    void setDeltatime(float deltatime);
    void setTime(GLfloat time);
    void setRandomTexUnit(unsigned int unit);
    void setLauncherLifetime(GLfloat lifetime);
    void setShellLifetime(GLfloat lifetime);


  private:
    ::engine::shader::Compiler shader_update_;

    // uniform locations
    GLuint loc_time_;
    GLuint loc_time_delta_;
    GLuint loc_tex_loc_; // location of the random texture
    GLuint loc_launcher_lifetime_;
    GLuint loc_shell_lifetime_;
    //GLuint loc_secondary_shell_lifetime_;

};

} // end namespace engine::particles
}

#endif //GRAPHICS_ENGINE_SHADERUPDATER_H
