#version 330 core

out vec4 outColor;

in vec3 color;
in vec3 vpos;
in vec3 vnormal;
in vec3 vtangent;
in vec2 tc;

in vec3 tangentLightPos;
in vec3 tangentViewPos;
in vec3 tangentFragPos;

uniform sampler2D colorTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;
uniform sampler2D emiTex;

uniform mat4 view;
uniform mat4 proj;


uniform vec3 DirLightC;
uniform vec3 DirLightD;

uniform vec3 PointLightC;
uniform vec4 PointLightP;

uniform vec3 FocalLightC;
uniform vec4 FocalLightP;
uniform float FocalLightA;
uniform vec3 FocalLightD;

////////////////////////////////////
//	Objeto

void main() {
	/*
	Kd = texture(colorTex, tc).rgb;
	Ka = Kd;
	Ks = texture(specularTex, tc).rgb;
	Ke = texture(emiTex, tc).rgb;

	n = 50.0;

	N = normalize(vnormal);
	bumpMap();

	pos = vpos;



	c += shade_base();
	c += shade_directional_light();
	c += shade_point_light();
	c += shade_focal_light();
	*/

	vec3 c = vec3(0.f);


	c +=
    DirLightC;
	+ DirLightD;
	+ PointLightC;
	+ PointLightP.xyz;
 	+ FocalLightC;
	+ FocalLightP.xyz;
	+ vec3(FocalLightA);
	+ FocalLightD;

	outColor = vec4(c, 1.0);
}