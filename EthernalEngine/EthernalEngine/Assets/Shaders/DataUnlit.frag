#version 330 core
// Unlit by default so the rendered colour equals the colormap colour exactly.
// Raise SHADING (0..0.4) for a mild view-facing darkening that restores some depth cues
// at the cost of colour accuracy; keep 0.0 for the figure you submit.
const float SHADING = 0.0;

in vec4 BaseColor;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

out vec4 FragColor;

void main()
{
    float facing = max(dot(normalize(Normal), normalize(viewPos - FragPos)), 0.0);
    vec3 c = BaseColor.rgb * (1.0 - SHADING + SHADING * facing);
    FragColor = vec4(c, 1.0);
}
