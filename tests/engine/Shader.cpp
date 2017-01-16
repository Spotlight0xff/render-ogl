//
// Created by spotlight on 1/16/17.
//

#include "glmock.hpp"
#include "engine/ShaderCompiler.h"

#include <gtest/gtest.h>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::SetArgPointee;



namespace engine {

TEST(Shader, Loading) {
  GLMock mock;
  ON_CALL(mock, gl_GetShaderiv(_, _, _))
          .WillByDefault(SetArgPointee<2>(1));

  // vertex & fragment shader compilation
  EXPECT_CALL(mock, gl_CreateShader(_)).Times(2);
  EXPECT_CALL(mock, gl_ShaderSource(_, _, _, _)).Times(2);
  EXPECT_CALL(mock, gl_CompileShader(_)).Times(2);
  EXPECT_CALL(mock, gl_GetShaderiv(_, _, _)).Times(2);

  // linking process
  EXPECT_CALL(mock, gl_CreateProgram()).Times(1);
  EXPECT_CALL(mock, gl_LinkProgram(_)).Times(1);
  EXPECT_CALL(mock, gl_AttachShader(_, _)).Times(2);
  EXPECT_CALL(mock, gl_DeleteShader(_)).Times(2);
  EXPECT_CALL(mock, gl_GetProgramiv(_, _, _)).Times(1);

  Shader shader("light_simple");
}


} // end namespace engine