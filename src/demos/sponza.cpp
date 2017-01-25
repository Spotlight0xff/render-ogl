#include <engine/handler/FpsMovement.h>
#include <engine/Scene.h>
#include "engine/Engine.h"

int main() {
  using namespace engine;
  Engine engine;
  Engine::Options engine_options;
  engine.Init();

  handler::FpsMovement fps_handler;
  fps_handler.setEyelevel(12.0);
  fps_handler.setSensitivity(0.1f);
  fps_handler.setSpeed(20.0);

  // Scene initialization
  Scene scene;
  scene.useCamera(camera);

  std::unique_ptr<EulerCamera> camera{};
  std::unique_ptr<FpsMovement> fps_movement(camera.get());
  fps_movement.SetCamera(camera);
  builder.SetCamera(std::move(camera));
  builder.SetHandler(std::move(fps_movement));

  auto scene = builder.createScene();

  engine.LoadScene(std::move(scene));

  engine.Run();

  return 0;
}
