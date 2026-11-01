#version 330 core
// Flat "data" shader: colour encodes data, so no lighting, shadows or reflections.
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform vec3 baseColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 BaseColor;
out vec3 Normal;
out vec3 FragPos;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos   = worldPos.xyz;
    Normal    = mat3(transpose(inverse(model))) * aNormal;
    BaseColor = vec4(baseColor, 1.0);
    gl_Position = projection * view * worldPos;
}
