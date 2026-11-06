#version 330 core

in vec3 vertexColor;
in vec3 normal;
in vec3 worldPos;
in vec3 worldNorm;
out vec4 fragColor;

uniform vec3 viewPos;
uniform vec3 lightPos;

void main()
{
    vec3 N = normalize(worldNorm);
    vec3 L = normalize(lightPos - worldPos);
    float diff = max(dot(N, L), 0.0);

    vec3 ambient = 0.00* vertexColor;
    float shade = 0.3 + 0.7 * abs(N.y);

    vec3 color = ambient + diff * (shade * vertexColor);
    fragColor = vec4(clamp(color, 0.0, 1.0), 1.0);
}
