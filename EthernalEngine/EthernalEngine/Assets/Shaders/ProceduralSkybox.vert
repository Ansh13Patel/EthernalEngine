#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 vDirection;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    vDirection = aPos;
    mat4 uView = mat4(mat3(view));
    vec4 pos = projection * uView * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}