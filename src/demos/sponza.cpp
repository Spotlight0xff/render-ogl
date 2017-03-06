#include <engine/handler/FpsMovement.h>
#include <engine/Scene.h>
#include <engine/components/ModelObject.h>
#include <engine/components/Phong/PhongModel.h>
#include <engine/scene/Phong.h>
#include "engine/Engine.h"

int main() {
  using namespace engine;
  Engine engine;
  Engine::Options engine_options;
  try {
    engine.Init();
  } catch (std::exception const& e) {
    std::cerr << "Engine startup failed.\nError: " << e.what() << "\n";
    return EXIT_FAILURE;
  }

  // TODO: camera should be on heap
  std::unique_ptr<scene::EulerCamera> camera{new scene::EulerCamera};
  handler::FpsMovement movement(camera.get());

  movement.setEyelevel(12.0);
  movement.setSensitivity(0.1f);
  movement.setSpeed(20.0f);

  // Set input handlers
  engine.SetKeyboardHandler(&movement);
  engine.SetMouseHandler(&movement);
  engine.SetFrameHandler(&movement);

  // Load the .obj models
  //Model* model = engine.LoadResource<Model>("resources/models/nanosuit2/nanosuit.obj");
  //SimpleLight* light = engine.LoadResource<SimpleLight>();

  //std::unique_ptr<components::PhongModel> obj{new components::PhongModel(model.get())};

  //std::unique_ptr<Scene> scene{new Scene};
  std::unique_ptr<scene::Phong> scene;
  //scene->AddModel(model);
  //scene->AddLight(light);



  // The engine will now handle all memory management about the scene.
  engine.LoadScene(std::move(scene));

  engine.Run();

  return 0;
}
