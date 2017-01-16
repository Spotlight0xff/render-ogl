#version 330 core

out vec4 color;

struct Material {
  float shininess;
  sampler2D diffuse;
  sampler2D specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform float ambientStrength;


#define NUM_MAX_LIGHTS 4
uniform PointLight pointLights[NUM_MAX_LIGHTS];


// Camera
uniform vec3 viewPos;

uniform int active_lights;

// Texture
uniform sampler2D tex;

/*
 *  Input variables
 */
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos; // fragment position

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < active_lights; i++)
            result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
     color = vec4(result, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Combine results
    vec3 ambient = light.ambient * vec3(texture(tex, TexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}