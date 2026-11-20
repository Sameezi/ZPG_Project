#version 330 core

in vec3 worldPos;
in vec3 worldNorm;
in vec2 uv;
out vec4 FragColor;

uniform sampler2D textureUnitID;
uniform vec3 viewPos;

// Directional light
struct DirLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0), 32);

    vec3 albedo = texture(textureUnitID,uv).rgb;
    vec3 ambient = light.ambient * albedo;
    vec3 diffuse = light.diffuse * diff * albedo; 
    vec3 specular = light.specular * spec * albedo; 

    return (ambient + diffuse + specular);
}

// Point light
struct PointLight{
    vec3 position;

    float constant; 
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal,lightDir),0.0);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(reflectDir,viewDir),0.0),5.0);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 albedo = texture(textureUnitID,uv).rgb;
    vec3 ambient = light.ambient * albedo;
    vec3 diffuse = light.diffuse * diff * albedo;
    vec3 specular = light.specular * spec * albedo;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

// Spot light
struct SpotLight{
    vec3 position;
    vec3 direction;

    float constant; 
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutOff;
    float outerCutOff;
};

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    vec3 albedo = texture(textureUnitID,uv).rgb;
    vec3 ambient = light.ambient * albedo;

    if(theta > light.cutOff){
        float diff = max(dot(normal,lightDir),0.0);
        vec3 reflectDir = reflect(-lightDir, normal);

        float spec = pow(max(dot(reflectDir,viewDir),0.0),32);
        float distance = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
        
        vec3 diffuse = light.diffuse * diff * albedo;
        vec3 specular = light.specular * spec * albedo;

        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        return (ambient + diffuse + specular);
    }else{
       return (light.ambient);
    }
}

// Uniform Buffer Object
layout(std140) uniform Lights {
    int numDirLights; int numPointLights; int numSpotLights;
    DirLight dirLights[4];
    PointLight pointLights[4];
    SpotLight spotLights[4];
};



void main()
{
    vec3 norm = normalize(worldNorm);
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 result = vec3(0,0,0);
    // phase 1: Directional lighting
    for(int index = 0; index < numDirLights; index++){ // for all light sources
        result += CalcDirLight(dirLights[index], norm, viewDir);
    }

    // phase 2: Point lights
    for(int index = 0; index < numPointLights; index++){ // for all light sources
        result += CalcPointLight(pointLights[index], norm, worldPos, viewDir);
    }

    // phase 3: Spot light
    for(int index = 0; index < numSpotLights; index++){ 
        result += CalcSpotLight(spotLights[index], norm, worldPos, viewDir );
    }
    

    FragColor = vec4(result , 1.0);
}