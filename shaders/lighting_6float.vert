#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 modelMatrix = mat4(1.0);;
uniform mat4 viewMatrix = mat4(1.0);;
uniform mat4 projectionMatrix = mat4(1.0);;

out vec3 worldPos;
out vec3 worldNorm;
out vec3 vertexColor;

void main()
{
    vertexColor = vec3 (0.4 , 1.0 , 0.4);
    vec4 world = modelMatrix * vec4(position, 1.0);
    worldPos = world.xyz;
    mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
    worldNorm = normalize(normalMatrix * normal);
    gl_Position  =  projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}