# Modern OpenGL Graphics engine [![Build Status](https://travis-ci.org/Spotlight0xff/render-ogl.svg?branch=master)](https://travis-ci.org/Spotlight0xff/render-ogl) [![Coverage Status](https://coveralls.io/repos/github/Spotlight0xff/render-ogl/badge.svg?branch=master)](https://coveralls.io/github/Spotlight0xff/render-ogl?branch=master)

Graphics Engine supporting:
* Object loading (using Assimp)
* Scene with multiple objects and multiple light sources
* Diffuse and specular maps

Work in progress:
* Instanced rendering
* Particle system

TODO:
* Enable ASAN for unit tests, see [this page](https://github.com/google/sanitizers/wiki/MemorySanitizerLibcxxHowTo).

Development:
* Stick to the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
* Don't overuse templates (bloats up compile times)
* Use RAII where possible
* Try to avoid raw pointers
  * use unique_ptr instead
  * only use shared_ptr if required (kills performance due to reference counting)
* Turn on your brain
