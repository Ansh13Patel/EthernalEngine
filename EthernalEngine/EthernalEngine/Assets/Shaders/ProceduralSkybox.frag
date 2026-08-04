#version 330 core

in vec3 vDirection;

out vec4 FragColor;

uniform vec3 uSkyColor;
uniform vec3 uHorizonColor;
uniform vec3 uGroundColor;   

void main()
{
    vec3 dir = normalize(vDirection);
    float t = dir.y * 0.5 + 0.5;

    vec3 color;
    if(dir.y > 0.0)
    {
       color = mix(uHorizonColor, uSkyColor, pow(t,0.6));
    }
    else
    {
       color = mix(uGroundColor, uHorizonColor, pow(t,0.6));
    }

    FragColor = vec4(color,1.0);
}