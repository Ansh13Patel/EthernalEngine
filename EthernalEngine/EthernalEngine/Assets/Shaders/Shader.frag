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
in vec4 BaseColor;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

out vec4 FragColor;

uniform sampler2D image;
uniform samplerCube skybox;
uniform vec3 viewPos;
uniform sampler2D shadowMap;
uniform float shininess;
uniform float metallic;
uniform float roughness;
uniform float transparency;
uniform float ior;
uniform vec4 sceneAmbientColor;
uniform float sceneintensity;
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int pointLightCount;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int spotLightCount;

vec3 CalculateSceneAmbient();
vec3 CalculateDirectionalLight();
vec3 CalculatePointLights();
vec3 CalculateSpotLights();
vec3 CalculateReflectionColor();
vec3 CalculateRefractionColor();
float CalculateShadow(vec4 fragPosLightSpace);

void main()
{
    vec3 texColor = texture(image, TexCoords).rgb;

    vec3 lighting = CalculateSceneAmbient() + CalculateDirectionalLight() + CalculatePointLights() + CalculateSpotLights();
    vec3 reflection = CalculateReflectionColor();
    vec3 refraction = CalculateRefractionColor();
    vec3 surface = lighting * texColor;

    reflection = mix(reflection, surface, roughness);

    float fresnel = pow(1.0 - max(dot(normalize(Normal), normalize(viewPos - FragPos)), 0.0), 5.0);

    vec3 opaqueColor = mix(surface, reflection, metallic);
    vec3 glass = mix(refraction * texColor, reflection, fresnel);
    glass += surface * 0.1;

    vec3 finalColor = mix(opaqueColor, glass, transparency);

    FragColor = BaseColor * vec4(finalColor, 1.0);
}

vec3 CalculateReflectionColor()
{
    vec3 I = normalize(FragPos - viewPos);
    vec3 R = reflect(I, normalize(Normal));
    
    return texture(skybox, R).rgb;
}

vec3 CalculateRefractionColor()
{
    vec3 I = normalize(FragPos - viewPos);
    vec3 R = refract(I, normalize(Normal), 1.0/ior);

    return texture(skybox, R).rgb;
}

vec3 CalculateSceneAmbient()
{
    return (sceneintensity * sceneAmbientColor.rgb);
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
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = directionalLight.specularStrength * spec * directionalLight.color.rgb;

    float shadow = CalculateShadow(FragPosLightSpace);

    vec3 finalLightColor = (ambient + (1.0 - shadow) * (diffuse + specular)) * directionalLight.intensity;
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
          float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
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
          float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
          specularColor += (sl.specularStrength * spec * attenuation * sl.color.rgb * sl.intensity);
       }
    }

    vec3 finalLightColor = (diffuseColor + specularColor);
    return finalLightColor;
}

float CalculateShadow(vec4 fragPosLightSpace)
{
   vec3 projcoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

   projcoords = projcoords * 0.5 + 0.5;

   if(projcoords.x < 0.0 || projcoords.x > 1.0 ||
      projcoords.y < 0.0 || projcoords.y > 1.0 ||
      projcoords.z > 1.0)
      return 0.0;

   float closestDepth = texture(shadowMap, projcoords.xy).r;
   float currentDepth = projcoords.z;

   vec3 normal = normalize(Normal);
   vec3 lightDir = normalize(-directionalLight.direction);

   float bias = max(0.005 * (1.0 - dot(normal, lightDir)),
                    0.0005);

   return currentDepth - bias > closestDepth ? 1.0 : 0.0;
}