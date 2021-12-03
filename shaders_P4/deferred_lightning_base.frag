#version 330 core

out vec4 color;

in vec2 texCoord;

uniform sampler2D colorTex;
uniform sampler2D zTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;
uniform sampler2D positionTex;

// Lights
uniform vec3 _ambientLightC;

uniform vec3 _dirLightC;
uniform vec3 _dirLightD;

////////////////////////////////////
//	Objeto
vec3 pos;
vec3 Ka;
vec3 Ks;
vec3 Kd;
vec3 N;
float n;

in vec2 vpos;

vec3 shade_directional_light() {
	vec3 c = vec3(0);

  vec3 L = -_dirLightD;
  vec3 Il = _dirLightC;                  // light intensity (color)
  vec3 P = pos;                          // Positions of the fragment

	//Diffuse
	c += Kd * max(dot(N, L), 0);

	//Specular
	vec3 V = normalize(-P);
	vec3 R = reflect(-L, N);
	c += Ks * pow(max(dot(R, V), 0), n);

	c *= Il;
	return c;
}

vec3 shade_base() {
	vec3 c = vec3(0);
	vec3 Ia = _ambientLightC;

	//Ambiental
	c += Ia * Ka;

	return c;
}

void main() {
  Ka = texture(colorTex, texCoord).rgb;
	Ks = texture(specularTex, texCoord).rrr;
	N  = texture(normalTex, texCoord).rgb;
	float z  = texture(zTex, texCoord).x;
	pos = vec3(texture(positionTex, texCoord).rg, z);

	Kd = Ka;
	n = 50.0;
	vec3 c = vec3(0);

	c += shade_base();
	c += shade_directional_light();

  color = (z < .000001) ? vec4(c, 1) : vec4(Ka, 1);
}