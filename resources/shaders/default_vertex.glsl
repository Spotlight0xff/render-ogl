#version 330 core

layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 color;

//uniform vec3 vec_offset;
uniform mat4 mvp;
//out vec3 vertexColor;

void main()
{
  //vec3 loc = vec3(position.x + vec_offset.x, position.yz);
  //vec3 loc = position + vec_offset;
  gl_Position = mvp * vec4(position, 1.0);
  //vertexColor = color;

  //vertexColor = vec4(0.5, 0.0f, 0.0f, 1.0f);
}
