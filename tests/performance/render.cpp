//
// Created by spotlight on 1/16/17.
//

#include <engine/Scene.h>
#include "glmock.hpp"

#include "engine/model/Model.h"
#include "engine/shader/compiler.h"
#include "engine/components/ModelObject.h"

using ::testing::_;
using ::testing::Invoke;


TEST(Performance, Rendering) {
  using ::testing::NiceMock;
  using ::testing::Assign;
  using ::testing::InvokeWithoutArgs;

  NiceMock<GLMock> mock;
  size_t counter_bind_vao = 0;
  size_t counter_draw = 0;
  size_t counter_draw_indices = 0;

  // just to pass the assertion
  GLFWwindow* window = (GLFWwindow*) 1;

  engine::Scene scene(window);




  engine::Model model_cube("resources/models/cube.obj");
/*  engine::components::ModelObject obj_cube(&model_cube);
  scene.addObjectRef(&obj_cube);
  scene.addObjectRef(&obj_cube);


  ON_CALL(mock, gl_BindVertexArray(_))
          .WillByDefault(InvokeWithoutArgs([&]() { counter_bind_vao++; }));


  ON_CALL(mock, gl_DrawElements(_, _, _, _))
          .WillByDefault(DoAll(
                  InvokeWithoutArgs([&]() { counter_draw++; }),
                  Invoke([&](GLenum mode, GLsizei indices, GLenum, const void* p_ind) {counter_draw_indices += indices; })
          ));

  scene.draw();
  std::cout << "counter vao: " << counter_bind_vao << "\n";
  std::cout << "counter draw: " << counter_draw << "\n";
  std::cout << "counter draw indices: " << counter_draw_indices << "\n";*/
}