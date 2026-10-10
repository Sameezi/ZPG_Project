#version 330 core


uniform vec3 positionUniform;
uniform vec2 scaleRotation;
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 vertexColor;

void main()
{
   float a = scaleRotation.y; 
    vec3 r = vec3(
        cos(a) * position.x + sin(a) * position.z,
        position.y,
        -sin(a) * position.x + cos(a) * position.z
    );
    vertexColor = color;
    r *= scaleRotation.x;
    gl_Position = vec4(r.x + positionUniform.x,r.y + positionUniform.y, r.z + positionUniform.z, 1.0);

}