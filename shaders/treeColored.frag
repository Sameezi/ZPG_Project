#version 330 core

in vec3 vertexColor;
in vec3 normal;
out vec4 fragColor;

void main()
{
    float shade = 0.3 + 0.7 * abs(normal.y);
    fragColor = vec4(shade * vertexColor,1.0);
}