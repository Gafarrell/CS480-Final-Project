#version 460

layout (location = 0) in vec3 v_position; 
layout (location = 1) in vec3 v_color; 
layout (location = 2) in vec2 v_tc;  
layout(location = 3) in vec3 aNormal;
    
out vec3 color; 
out vec2 tc;
out vec3 Normal;
out vec3 crntPos;

uniform mat4 projectionMatrix; 
uniform mat4 viewMatrix; 
uniform mat4 modelMatrix; 
uniform bool hasTC;        
uniform sampler2D sp; 

void main(void) 
{ 
    crntPos = vec3(modelMatrix * vec4(v_position, 1.0f));

    vec4 v = vec4(crntPos, 1.0); 
    gl_Position = (projectionMatrix * viewMatrix) * v; 
    color = v_color; 
    tc = v_tc;
    Normal = aNormal;
} 