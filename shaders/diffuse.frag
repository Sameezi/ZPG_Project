#version 330 core

in vec3 vertexColor;
in vec3 worldPos;
in vec3 worldNorm;
out vec4 fragColor;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;

float attenuation(float d, float c, float l, float q)
{
    float att = 1.0 / (c + l * d + q * d * d);
    return clamp(att, 0.0, 1.0);
}

void main()
{
    vec3 N = normalize(worldNorm);
    vec3 toLight = normalize(lightPos.xyz - worldPos);
    float dist = length(toLight);
    vec3 L = toLight / dist;           

    vec3 radiance = lightColor * lightIntensity;

    float diff = max(dot(N, L), 0.0);
    float att = attenuation(dist, 1.0, 0.09, 0.032);

    vec3 ambient = 0.1 * vertexColor;     
    vec3 color = ambient + att * diff * vertexColor * radiance; 
    
    fragColor = vec4(color, 1.0);
}