#version 330 core

layout (location = 0) out float outZ;
layout (location = 1) out vec4 outColor;
layout (location = 2) out vec3 outNormal;
layout (location = 3) out vec3 outSpecular;

in vec3 color;
in vec3 vpos;
in vec3 vnormal;
in vec3 vtangent;
in vec2 tc;

uniform sampler2D colorTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;
uniform sampler2D emiTex;

void main() {

	vec3 diffuse = texture(colorTex, tc).rgb;
	vec3 emissive = texture(emiTex, tc).rgb;
	vec3 specular = texture(specularTex, tc).rgb;
	vec3 normal = normalize(vnormal);

	outColor = vec4(diffuse + emissive, 1.0);
	outNormal = normal;
	outSpecular = specular;
	outZ = vpos.z;
}