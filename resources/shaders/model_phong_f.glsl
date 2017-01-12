#version 330 core

out vec4 color;

uniform vec3 lightColor;
uniform float ambientStrength;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D tex;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

void main()
{
  // ambient
  vec3 ambient = ambientStrength * lightColor;

  // diffuse
  vec3 norm = normalize(Normal);
  vec3 light_dir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, light_dir), 0.0); // angle
  vec3 diffuse = diff * lightColor;

  // specular
  float specularStrength = 0.7f;
  vec3 view_dir = normalize(viewPos - FragPos);
  vec3 reflect_dir = reflect(-light_dir, norm);
  float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
  vec3 specular = specularStrength * spec * light_dir;


  // put together
  vec3 result = (ambient + diffuse + specular);

  color = texture(tex, TexCoord) * vec4(result, 1.0);
}
