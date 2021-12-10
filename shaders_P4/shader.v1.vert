#version 330 core

layout(location=0) in vec3 inPos;
layout(location=1) in vec3 inColor;
layout(location=2) in vec3 inNormal;
layout(location=3) in vec2 inTexCoord;

uniform mat4 _modelViewProj;
uniform mat4 _modelView;
uniform mat4 _normal;

out vec3 color;
out vec3 vpos;
out vec3 vnormal;
out vec2 tc;

void main()
{
	color = inColor;
	tc = inTexCoord;
	vnormal = (_normal * vec4(inNormal, 0.0)).xyz;
	vpos = (_modelView * vec4(inPos, 1.0)).xyz;

	gl_Position =  _modelViewProj * vec4 (inPos,1.0);
}
