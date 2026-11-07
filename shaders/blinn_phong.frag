#version 330 core

in vec3 worldPos;
in vec3 worldNorm;
in vec3 vertexColor;
out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;

void main()
{
    vec3 N = normalize(worldNorm);
    vec3 L = normalize(lightPos.xyz - worldPos); 
    vec3 V = normalize(viewPos - worldPos);
    vec3 H = normalize(L + V);

    vec3 radiance = lightColor * lightIntensity;

    // Ambient
    vec3 ambient = 0.2 * vertexColor * radiance;

    // Diffuse
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * vertexColor * radiance;

    // Specular
    float shininess = 32.0;
    float spec = (diff > 0.0) ? pow(max(dot(N, H), 0.0), shininess) : 0.0;
    float specularStrength = 0.5;
    vec3 specular = specularStrength * spec * radiance;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}