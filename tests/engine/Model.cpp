#include "glmock.hpp"

#include "engine/Model.h"

#include <gtest/gtest.h>

using ::testing::_;
using ::testing::AtLeast;


namespace engine {

TEST(Model, Loading) {
  GLMock mock;

  // texture
  EXPECT_CALL(mock, gl_GenTextures(1, _)).Times(1);
  EXPECT_CALL(mock, gl_BindTexture(_, _)).Times(AtLeast(1));
  EXPECT_CALL(mock, gl_TexParameteri(_, _, _)).Times(AtLeast(1));
  EXPECT_CALL(mock, gl_GenerateMipmap(_)).Times(1);
  EXPECT_CALL(mock, gl_TexImage2D(_, _, _, _, _, _, _, _, _));

  // vertices and indices
  EXPECT_CALL(mock, gl_GenBuffers(_, _)).Times(2);
  EXPECT_CALL(mock, gl_EnableVertexAttribArray(_)).Times(AtLeast(1));
  EXPECT_CALL(mock, gl_VertexAttribPointer(_, _, _, _, _, _)).Times(AtLeast(1));
  EXPECT_CALL(mock, gl_BindVertexArray(_)).Times(AtLeast(2));
  EXPECT_CALL(mock, gl_BufferData(_, _, _, _)).Times(AtLeast(2));
  EXPECT_CALL(mock, gl_BindBuffer(_, _)).Times(AtLeast(3));


  engine::Model model_cube("resources/models/cube.obj");

  ASSERT_EQ("resources/models/cube.obj", model_cube.path);
  ASSERT_EQ(model_cube.meshes.size(), 1);
  engine::model::Mesh &mesh = model_cube.meshes.at(0);

  // We mostly expect Assimp to do the right thing (tm)
  ASSERT_EQ(mesh.getIndices().size(), 36);
  ASSERT_EQ(mesh.getVertices().size(), 24);
  ASSERT_EQ(mesh.getTextures().size(), 1);
  ASSERT_EQ(model_cube.loaded_textures.size(), 1);
}


TEST(Model, Draw) {
  using ::testing::NiceMock;
  NiceMock<GLMock> mock;

  engine::Model model_cube("resources/models/cube.obj");



  EXPECT_CALL(mock, gl_BindVertexArray(_)).Times(2);
  EXPECT_CALL(mock, gl_DrawElements(GL_TRIANGLES, _, _, _));

  Shader shader;
  model_cube.draw(shader);
}


TEST(Model, FailLoad) {
  GLMock mock;
  testing::internal::CaptureStderr();


  engine::Model model_cube("non_existent.obj");
  ASSERT_EQ(testing::internal::GetCapturedStderr(), "Import error for non_existent.obj: Unable to open file \"non_existent.obj\".\n");
  ASSERT_EQ(model_cube.meshes.size(), 0);
}

} // end namespace engine