#version 330 core

out vec4 color;

in vec3 FragPos;

void main()
{
  vec2 pos = mod(FragPos.xz, vec2(2));
  if (pos.x > 1.0) {
    if (pos.y > 1.0) {
    color = vec4(1.0, 1.0, 1.0, 1.0);
    } else {
    color = vec4(0.0, 0.0, 0.0, 1.0);
    }
  } else {
    if (pos.y > 1.0) {
    color = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
    color = vec4(1.0, 1.0, 1.0, 1.0);
    }
  }
}
