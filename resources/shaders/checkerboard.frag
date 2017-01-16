#version 330 core

out vec4 color;

in vec3 FragPos;

void main()
{
    vec4 color_dark = vec4(0.4, 0.4, 0.4, 1.0);
    vec4 color_bright = vec4(0.1, 0.1, 0.1, 1.0);
  vec2 pos = mod(FragPos.xz, vec2(2));
  if (pos.x > 1.0) {
    if (pos.y > 1.0) {
    color = color_bright;
    } else {
    color = color_dark;
    }
  } else {
    if (pos.y > 1.0) {
    color = color_dark;
    } else {
    color = color_bright;
    }
  }
}
