#version 330 core

in vec3 worldPos;
in vec3 worldNorm;
in vec3 vertexColor;
out vec4 FragColor;


void main()
{
    vec3 lightPosition = vec3(10.0,10.0,10.0);
    const vec3 lightDir = normalize(vec3(-1.0, -1.0, -1.0));

    float diff = max(dot(normalize(worldNorm), -lightDir), 0.0);
    vec3 ambient = 0.1 * vertexColor;
    FragColor = vec4(ambient + diff * vertexColor,1.0); 
}
