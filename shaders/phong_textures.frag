#version 330 core

in vec3 worldPos;
in vec3 worldNorm;
in vec2 uv;
out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform float lightIntensity;

uniform sampler2D textureUnit1;
uniform sampler2D textureUnit2;

float attenuation(float d, float c, float l, float q)
{
    float att = 1.0 / (c + l * d + q * d * d);
    return clamp(att, 0.0, 1.0);
}

void main()
{
    vec3 N = normalize(worldNorm);
    vec3 L = normalize(lightPos.xyz - worldPos);
    vec3 V = normalize(viewPos - worldPos);           

    float d = length(lightPos - worldPos);
    float att = attenuation(d, 1.0, 0.09, 0.032);
    vec3 radiance = lightColor * lightIntensity * att;

    vec4 t1 = texture(textureUnit1, uv);
    vec4 t2 = texture(textureUnit2, uv);
    vec3 albedo = mix(t1.rgb, t2.rgb, t2.a);

    // Ambient
    vec3 ambient = vec3(0.2) * albedo * radiance;

    // Diffuse
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * albedo * radiance;

    // Specular (Phong)
    vec3 R = reflect(-L, N);
    float shininess = 32.0;
    float spec = pow(max(dot(V, R), 0.0), shininess) * step(0.0,diff);
    float specularStrength = 0.5;
    vec3 specular = specularStrength * spec * radiance;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}