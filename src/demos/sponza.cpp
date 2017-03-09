#include <engine/handler/FpsMovement.h>
#include <engine/Scene.h>
#include <engine/components/ModelObject.h>
#include <engine/components/Phong/PhongModel.h>
#include <engine/components/CustomShaderObject.h>
#include "engine/Engine.h"

//TODO(Debug) print matrices
#include <glm/gtx/string_cast.hpp>

int main() {
  using namespace engine;
  Engine engine;
  engine::resource::Manager* manager = engine.getResourceManager();
  Engine::Options engine_options;
  engine::Scene* scene;

  engine_options.fullscreen = false;
  engine_options.default_height = 1080;
  engine_options.default_width = 1920;
  engine_options.show_cursor = true;
  engine_options.samples = 8;

  try {
    engine.Init(engine_options);
  } catch (std::exception const& e) {
    std::cerr << "Engine startup failed.\nError: " << e.what() << "\n";
    return EXIT_FAILURE;
  }

  scene::EulerCamera* camera = manager->loadAsset<scene::EulerCamera>();
  handler::FpsMovement movement(camera);

  movement.setEyelevel(12.0);
  movement.setSensitivity(0.1f);
  movement.setSpeed(20.0f);

  // Set input handlers
  engine.SetKeyboardHandler(&movement);
  engine.SetMouseHandler(&movement);
  engine.SetFrameHandler(&movement);

  scene = engine.CreateScene();
  scene->useCamera(camera);

  // Add nanosuit model and then generate an object from it
  auto nanosuit = manager->loadAsset<Model>(manager, "resources/models/nanosuit2/nanosuit.obj");
  auto nomad = scene->addModel<::engine::components::PhongModel>(nanosuit);
  //nomad->setPosition({0.0, 0.0, 8.0});
  nomad->setPosition({0.0, 0.0, 15.0});

  // Also add the ground as an CustomShaderObject
  auto model_ground = manager->loadAsset<Model>(manager, "resources/models/ground.obj");
  auto checkerboard = scene->addModel<::engine::components::CustomShaderObject>(model_ground, "checkerboard",

                                 [](Scene& s, components::ModelObject& obj, engine::shader::Compiler& shader) {
    shader.set("model", obj.getModelMatrix());
    shader.set("view", s.getCameraRef().getViewMatrix());
    shader.set("projection", s.getProjectionMatrix());
    });
  checkerboard->setScale({50.0, 1.0, 50.0});



  // The engine will now handle all memory management related to the scene.
  engine.setScene(std::move(scene));

  engine.Run();

  return 0;
}
