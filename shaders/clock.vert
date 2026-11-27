#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;

uniform mat4 modelMatrix = mat4(1.0);;
uniform mat4 viewMatrix = mat4(1.0);;
uniform mat4 projectionMatrix = mat4(1.0);;
uniform float utime;

out vec3 vertexColor;
out vec3 worldNorm;
out vec3 worldPos;


void main()
{
    mat2 R;
    vec2 p;

    if (color == vec3(1.0f,1.0f,0.0f)) {
        vertexColor = vec3(0.0f,1.0f,1.0f);
        p = position.xy;
    }else if (color == vec3(1.0f,0.0f,0.0f)) { // Second hand
        vertexColor = vec3(0.5f,0.0f,0.0f);
        float a = -6.2831853 * utime / 60;    
        R = mat2(cos(a), sin(a),
                 -sin(a), cos(a)); 
                 p = R * position.xy;
    }else if (color == vec3(0.0f, 1.0f, 0.0f)){ // Minute hand
        vertexColor = vec3(0.0f,0.0f,0.0f);
        float a = -6.2831853 * utime / 3600;    
        R = mat2(cos(a), sin(a),
                 -sin(a), cos(a)); 
                 p = R * position.xy;
    }else if (color == vec3(0.0f, 0.0f, 1.0f)){ // Hour hand
        float a = -6.2831853 * utime / 43200;    
        R = mat2(cos(a), sin(a),
                 -sin(a), cos(a)); 
                 p = R * position.xy;
        vertexColor = vec3(0.0f,0.0f,0.0f);
    }else{
        vertexColor = color;
        p = position.xy;
    }

    vec4 world = modelMatrix * vec4(position, 1.0);
    worldPos = world.xyz;
    worldNorm = mat3(transpose(inverse(modelMatrix))) * normal;
    
    gl_Position  =  projectionMatrix * viewMatrix * modelMatrix * vec4(p,position.z, 1.0);

    
}