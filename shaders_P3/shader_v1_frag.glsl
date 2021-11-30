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
vec3 pos;
vec3 N;
vec3 Ka;
vec3 Kd;
vec3 Ks;
float n;
vec3 Ke;

vec3 shade_base() {
	vec3 c = vec3(0);
	vec3 Ia = vec3(0.1);

	//Ambiental
	c += Ia * Ka;

	// Emissive
	c += Ke;

	return c;
}



vec3 shade_point_light() {
	vec3 c = vec3(0);


	vec3 light_point = PointLightP.xyz;
	vec3 Il = PointLightC;

	float d_min = 1.f;
	float d_max = 100.f;
	float d0 = 1.f;

	float d = distance(light_point, pos);

	float f_win = pow(max(pow(1 - (d / d_max), 4), 0), 2);
	float f_dist = pow((d0 / max(d, d_min) ), 2) * f_win;

	//Diffuse
	vec3 L = normalize(light_point - pos);
	c += Kd * max(dot(N, L), 0);

	//Specular
	vec3 V = normalize(-pos);
	vec3 R = reflect(-L, N);

	c += Ks * pow(max(dot(R, V), 0), n);

	c *= Il;
	c *= f_dist;

	return c;
}



void main() {

	Kd = texture(colorTex, tc).rgb;
	Ka = Kd;
	Ks = texture(specularTex, tc).rgb;
	Ke = texture(emiTex, tc).rgb;

	n = 50.0;

	N = normalize(vnormal);
//	bumpMap();

	pos = vpos;

	vec3 c = vec3(0.f);

	/*
	c += shade_directional_light();
	c += shade_focal_light();
	*/

	c += shade_base();
	c += shade_point_light();


	outColor = vec4(c, 1.0);
}