#version 330 core

in vec3 worldPos;
in vec3 worldNorm;
in vec2 uv;
out vec4 fragColor;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;

uniform sampler2D textureUnitID;

float attenuation(float d, float c, float l, float q)
{
    float att = 1.0 / (c + l * d + q * d * d);
    return clamp(att, 0.0, 1.0);
}

void main()
{
    vec3 N = normalize(worldNorm);
    vec3 toLight = lightPos - worldPos;
    float dist = length(toLight);
    vec3 L = toLight / dist;           

    vec3 radiance = lightColor * lightIntensity;

    float diff = max(dot(N, L), 0.0);
    float att = attenuation(dist, 1.0, 0.09, 0.032);

    vec3 albedo = texture(textureUnitID,uv).rgb;

    vec3 ambient = 0.1 * albedo; 

    vec3 color = ambient + att * diff * albedo * radiance; 

    fragColor = vec4(color, 1.0);
}