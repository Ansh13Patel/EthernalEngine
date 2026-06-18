#version 330 core

in vec2 TexCoords;
in vec4 vertexColor;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D image;
uniform float ambientStrength;
uniform float specularStrength;
uniform float intensity;
uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
    vec3 texColor = texture(image, TexCoords).rgb;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-lightDirection);
    vec3 ambient = ambientStrength * lightColor;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 lighting = (ambient + diffuse + specular) * intensity;
    vec3 finalColor = lighting * texColor;

    FragColor = vertexColor * vec4(finalColor, 1.0);
}