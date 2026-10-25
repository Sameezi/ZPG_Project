#version 330 core

in vec3 worldPos;
in vec3 worldNorm;
in vec3 vertexColor;
out vec4 FragColor;

uniform vec3 viewPos;

void main()
{
    vec3 N = normalize(worldNorm);
    vec3 lightDir = normalize(vec3(-1.0, -1.0, -1.0));
    vec3 L = -lightDir;                               
    vec3 V = normalize(viewPos - worldPos);           

    // Ambient
    vec3 ambient = vec3(0.2);

    // Diffuse
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * vertexColor;

    // Specular (Phong)
    vec3 R = reflect(lightDir, N);
    float shininess = 32.0;
    float spec = pow(max(dot(V, R), 0.0), shininess);
    float specularStrength = 1.0;
    vec3 lightColor = vec3(1.0);
    vec3 specular = specularStrength * spec * lightColor;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}