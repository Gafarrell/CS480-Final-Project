#version 460

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_color;
layout (location = 2) in vec2 v_tc;
layout (location = 3) in mat4 instanceMatrix;

uniform mat4 projectionMatrix; 
uniform mat4 viewMatrix;
uniform mat4 originMatrix;
uniform bool hasTC;
uniform sampler2D sp;
uniform float tf;
uniform int totalCount;
uniform mat3 normMatrix;

out vec4 varyingColor;
out vec3 Normal;
out vec3 varPos;

void main(void)
{

	varPos = (projectionMatrix * originMatrix * vec4(v_position,1)).xyz;

	vec4 v = vec4(v_position, 1.0);
	gl_Position = (projectionMatrix * viewMatrix * originMatrix * instanceMatrix) * v;

	varyingColor = texture(sp, v_tc);
	Normal = normMatrix * v_color;
}
