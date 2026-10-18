#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in mat4 instanceMatrix; // 2,3,4,5 for vec4

uniform mat4 modelMatrix = mat4(1.0);;
uniform mat4 viewMatrix = mat4(1.0);;
uniform mat4 projectionMatrix = mat4(1.0);;

out vec3 vertexColor;
out vec3 normal;

void main()
{
    normal = color;
    if(gl_VertexID>36443) vertexColor = vec3(0.f,1.f,0.f);
    else vertexColor = vec3(1.f,0.3f,0.f);
    gl_Position  =  projectionMatrix * viewMatrix * instanceMatrix * vec4(position, 1.0);
}