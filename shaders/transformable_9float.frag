#version 330 core

in vec3 vertexColor;
in vec3 vertexNormal;

out vec4 FragColor;

void main()
{
    const vec3 lightDir = normalize(vec3(-1.0, -1.0, -1.0)); // direction the light travels

    vec3 n = normalize(vertexNormal);
    float diffuse = max(dot(n, -lightDir), 0.0);
    float ambient = 0.2;

    FragColor = vec4(vertexColor * (ambient + diffuse), 1.0);
}