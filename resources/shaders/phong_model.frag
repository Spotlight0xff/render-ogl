#version 330 core

out vec4 color;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

uniform Material material;
uniform float ambientStrength;

// Light
uniform vec3 lightColor;
uniform vec3 lightPos;

// Camera
uniform vec3 viewPos;


// Texture
uniform sampler2D tex;

/*
 *  Input variables
 */
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos; // fragment position


void main()
{
  // Ambient
  vec3 ambient = ambientStrength * material.ambient;

  // Diffuse
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  // calc the angle between the incoming light and us
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * material.diffuse;

  // Specular
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
  vec3 specular = spec * material.specular;

  vec3 result = (ambient + diffuse + specular) * lightColor;
  color = texture(tex, TexCoord) * vec4(result, 1.0);
}
