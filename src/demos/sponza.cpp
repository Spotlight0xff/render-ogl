#include <engine/handler/FpsMovement.h>
#include <engine/Scene.h>
#include "engine/Engine.h"

int main() {
  using namespace engine;
  Engine engine;
  Engine::Options engine_options;
  if (!engine.Init()) {
    std::cerr << "Engine startup failed.\n";
    return EXIT_FAILURE;
  }

  scene::EulerCamera camera;
  handler::FpsMovement movement(&camera);

  movement.setEyelevel(12.0);
  movement.setSensitivity(0.1f);
  movement.setSpeed(20.0f);

  // Set input handlers
  engine.SetKeyboardHandler(&movement);
  engine.SetMouseHandler(&movement);
  engine.SetFrameHandler(&movement);

  // Indicate the engine where to get the camera information from
  //engine.UseCameraHandler(&movement);

  //Initialize scene
  // Scene scene;

  // Load the .obj models



  // Load models

  // Scene initialization
  //Scene scene;
  //scene.useCamera(camera);

  // The engine will now handle all memory management about the scene.
  //engine.LoadScene(std::move(scene));

  engine.Run();

  return 0;
}
