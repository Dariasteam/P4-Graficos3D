#version 330 core

out vec4 color;

in vec2 texCoord;

uniform sampler2D colorTex;
uniform sampler2D zTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;
uniform sampler2D positionTex;
uniform sampler2D depthTex;

// Lights
uniform vec3 _dirLightC;
uniform vec3 _dirLightD;

uniform vec3 _pointLightC;
uniform vec4 _pointLightP;

uniform vec3 _focalLightC;
uniform vec4 _focalLightP;
uniform float _focalLightA;
uniform vec3 _focalLightD;

uniform vec3 _ambientLightC;

////////////////////////////////////
//	Objeto
vec3 pos;
vec3 N;
vec3 Kd;
vec3 Ks;
float n;
vec3 Ke;

in vec2 vpos;


vec3 shade_point_light() {
	vec3 c = vec3(0);

	vec3 light_point = _pointLightP.xyz;
	vec3 Il = _pointLightC;

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


vec3 shade_focal_light() {
	vec3 c = vec3(0);

	vec3 light_point = _focalLightP.xyz;
	vec3 light_vector = normalize(_focalLightD);

	vec3 Il = _focalLightC;

	float angle = _focalLightA;

	float d_min = 1.f;
	float d_max = 100.f;
	float d0 = 1.f;

	float d = distance(light_point, pos);

	float f_win = pow(max(pow(1 - (d / d_max), 4), 0), 2);
	float f_dist = pow((d0 / max(d, d_min) ), 2) * f_win;

	//Diffuse
	vec3 L = normalize((light_point - pos) / d);
	c += Kd * max(dot(N, L), 0);

	//Specular
	vec3 V = normalize(-pos);
	vec3 R = reflect(-L, N);

	c += Ks * pow(max(dot(R, V), 0), n);

	// FIXME: Precalculate this in the client
	float cos_angle = cos(angle);
	float dot_L_lv = dot(-L, light_vector);

	bool condition = cos_angle < dot_L_lv;
	float attenuation = pow(max((dot_L_lv - cos_angle) / (1 - cos_angle), 0), 1);

	float f_dir = condition ? attenuation : 0.0;

	c *= Il;
	c *= f_dist;
	c *= f_dir;

	return c;
}


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


const float focalDistance = -25.0;
const float maxDistanceFactor = 1.0/5.0;  // A una distancia de 5 unidades borrosidad máxima

void main() {
  Kd = texture(colorTex, texCoord).rgb;
	Ks = texture(specularTex, texCoord).rgb;
	N  = texture(normalTex, texCoord).rgb;
	float z  = texture(zTex, texCoord).x;
	pos = vec3(texture(positionTex, texCoord).rg, z);

	float depth = texture(depthTex, texCoord).x;

	n = 50.0;

	vec3 c = vec3(0);
	Ks = vec3(1, 1, 1);

	c += shade_point_light();
	c += shade_focal_light();
	c += shade_directional_light();

  color = (z < .000001) ? vec4(c, 1) : vec4(0);
}