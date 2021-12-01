#version 330 core

in vec3 inPos;
in vec3 inColor;
in vec3 inNormal;
in vec2 inTexCoord;
in vec3 inTangent;

uniform mat4 view;
uniform mat4 modelViewProj;
uniform mat4 modelView;
uniform mat4 model;
uniform mat4 normal;

out vec3 color;
out vec3 vpos;
out vec3 vnormal;

out vec3 tangentLightPos;
out vec3 tangentViewPos;
out vec3 tangentFragPos;

out vec2 tc; //coordenadas de textura

void main()
{
	color = inColor;
	vpos = (modelView * vec4(inPos, 1.0)).xyz;
	vnormal = (normal * vec4(inNormal, 0.0)).xyz;
	tc = inTexCoord;

	gl_Position =  modelViewProj * vec4 (inPos, 1.0);

	// Normal mapping
	vec3 T = normalize(vec3(model * vec4(inTangent,   0.0)));
  vec3 N = normalize(vec3(model * vec4(inNormal,    0.0)));
	//vec3 B = normalize(vec3(model * vec4(cross(N, T), 0.0)));
  vec3 B = normalize(cross(N, T));

	mat3 TBN = mat3(T, B, N);

	vec3 lightPos = (view * vec4 (1.0, 0.0, 0.0, 0.0)).xyz;

	tangentLightPos = TBN * lightPos;
	tangentViewPos  = TBN * vpos;
	tangentFragPos  = TBN * vec3(model * vec4(inPos, 1.0));
}