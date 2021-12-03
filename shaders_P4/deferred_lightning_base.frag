#version 330 core

out vec4 color;

in vec2 texCoord;

uniform sampler2D colorTex;
uniform sampler2D zTex;

// Lights
uniform vec3 _ambientLightC;

////////////////////////////////////
//	Objeto

vec3 Ka;

vec3 shade_base() {
	vec3 c = vec3(0);
	vec3 Ia = _ambientLightC;

	//Ambiental
	c += Ia * Ka;

	return c;
}

void main() {
  Ka = texture(colorTex, texCoord).rgb;
	float z  = texture(zTex, texCoord).x;
	vec3 c = shade_base();
  color = (z < .000001) ? vec4(c, 1) : vec4(Ka, 1);
}