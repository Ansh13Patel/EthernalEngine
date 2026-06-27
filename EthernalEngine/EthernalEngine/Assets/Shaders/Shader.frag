#version 330 core

#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS  16

struct PointLight
{
   vec3 pos;
   float intensity;
   float specularStrength;
   float radius;
   vec4 color;
};

struct DirectionalLight
{
   vec3 direction;
   float intensity;
   float ambientStrength;
   float specularStrength;
   vec4 color;
};

struct SpotLight
{
  vec3 pos;
  vec3 direction;
  float angle;
  float range;
  float specularStrength;
  float intensity;
  vec4 color;
};

in vec2 TexCoords;
in vec4 vertexColor;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D image;
uniform vec3 viewPos;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int pointLightCount;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int spotLightCount;

vec3 CalculateDirectionalLight();
vec3 CalculatePointLights();
vec3 CalculateSpotLights();

void main()
{
    vec3 texColor = texture(image, TexCoords).rgb;

    vec3 lighting = CalculateDirectionalLight() + CalculatePointLights() + CalculateSpotLights();
    vec3 finalColor = lighting * texColor;

    FragColor = vertexColor * vec4(finalColor, 1.0);
}

vec3 CalculateDirectionalLight()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-directionalLight.direction);
    vec3 ambient = directionalLight.ambientStrength * directionalLight.color.rgb;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * directionalLight.color.rgb;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = directionalLight.specularStrength * spec * directionalLight.color.rgb;

    vec3 finalLightColor = (ambient + diffuse + specular) * directionalLight.intensity;
    return finalLightColor;
}

vec3 CalculatePointLights()
{
    vec3 ambientColor = vec3(0.0);
    vec3 diffuseColor = vec3(0.0);
    vec3 specularColor = vec3(0.0);

    for(int i = 0 ;i < min(pointLightCount, MAX_POINT_LIGHTS);i++)
    {
       PointLight pl = pointLights[i];

       vec3 lightDir = normalize(pl.pos - FragPos);
       float distance = length(pl.pos - FragPos);

       if(distance <= pl.radius)
       {
          float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
          
          vec3 norm = normalize(Normal);

          float diff = max(dot(norm, lightDir), 0.0);
          diffuseColor += (diff * attenuation * pl.color.rgb * pl.intensity);

          vec3 viewDir = normalize(viewPos - FragPos);
          vec3 reflectDir = reflect(-lightDir, norm);
          float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
          specularColor += (pl.specularStrength * spec * attenuation * pl.color.rgb * pl.intensity);
       }
    }

    vec3 finalLightColor = (diffuseColor + specularColor);
    return finalLightColor;
}

vec3 CalculateSpotLights()
{
    vec3 ambientColor = vec3(0.0);
    vec3 diffuseColor = vec3(0.0);
    vec3 specularColor = vec3(0.0);

    for(int i = 0 ;i < min(spotLightCount, MAX_SPOT_LIGHTS);i++)
    {
       SpotLight sl = spotLights[i];

       vec3 lightToFrag = normalize(FragPos - sl.pos);
       vec3 lightDir = normalize(sl.pos - FragPos);
       float theta = dot(lightToFrag, normalize(sl.direction));
       float cutoff = cos(radians(sl.angle * 0.5));
       float distance = length(sl.pos - FragPos);

       if(distance <= sl.range && theta >= cutoff)
       {
          float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
          
          vec3 norm = normalize(Normal);

          float diff = max(dot(norm, lightDir), 0.0);
          diffuseColor += (diff * attenuation * sl.color.rgb * sl.intensity);

          vec3 viewDir = normalize(viewPos - FragPos);
          vec3 reflectDir = reflect(-lightDir, norm);
          float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
          specularColor += (sl.specularStrength * spec * attenuation * sl.color.rgb * sl.intensity);
       }
    }

    vec3 finalLightColor = (diffuseColor + specularColor);
    return finalLightColor;
}