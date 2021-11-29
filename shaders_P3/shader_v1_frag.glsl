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

uniform vec3 areaLightColor;
uniform vec4 areaLightPos;

////////////////////////////////////
//	Objeto
vec3 pos;
vec3 N;
vec3 Ka;
vec3 Kd;
vec3 Ks;
float n;
vec3 Ke;

vec3 shade_focal_light() {
	vec3 c = vec3(0);

	vec3 light_point = vec3(0, 0, 0);
	vec3 light_vector = normalize(vec3(0, .2, -1)); // D

	vec3 Il = vec3(0, 2, 0);    // light intensity (color)
	float angle = 0.3f;

	float d_min = 1.f;
	float d_max = 100.f;
	float d0 = 1.f;

	float d = distance(light_point, pos);

	float f_win = pow(max(pow(1 - (d / d_max), 4), 0), 2);
	float f_dist = pow((d0 / max(d, d_min) ), 2) * f_win;

	//Diffuse
	vec3 L = normalize((light_point - pos) / d);
	N = normalize(N);
	c += Kd * max(dot(N, L), 0);

	//Specular
	vec3 V = normalize(-pos);
	vec3 R = reflect(-L, N);

	c += Ks * pow(max(dot(R, V), 0), n);

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

vec3 shade_point_light() {
	vec3 c = vec3(0.0);

	//vec3 global_pos = (vec4(pos.xyz, 0) * view * proj).xyz;

	vec3 light_point = areaLightPos.xyz;
	//vec3 light_point = (view * proj * vec4(1, 0, 0, 0)).xyz;

	vec3 Il = areaLightColor;

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

vec3 shade_directional_light() {
	vec3 c = vec3(0);

	vec3 light_dir = normalize(tangentLightPos - tangentFragPos);

  vec3 L = - light_dir; //l_tang;        // Unitary light vector

	vec3 normal = texture(normalTex, tc).rgb;
	vec3 N2 = normalize(normal * 2.0 - 1.0);

  vec3 Il = vec3(1.0);                  // light intensity (color)
  vec3 P = pos;                         // Positions of the fragment

	//Diffuse
	c += Il * Kd * max(dot(N2, L), 0);

	//Specular
	vec3 V = normalize(-P);

	//vec3 R = reflect(-L, N);
  vec3 R = 2 * (dot(L, N2)) * N2 - L;

	c += Il * Ks * pow(max(dot(R, V), 0), n);
	return c;
}

vec3 shade_base() {
	vec3 c = vec3(0);
	vec3 Ia = vec3(0.1);

	//Ambiental
	c += Ia * Ka;

	// Emissive
	c += Ke;

	return c;
}

vec3 fog(vec3 color_s) {

	vec3 fog_color = vec3(.2f, 0, 0);

	float e = 2.7182818;
	float fog_density = 0.1;
	float f = pow(e, -pow(fog_density * pos.z, 2));

	return f * color_s + (1 - f) * fog_color;
}

void bumpMap () {

}

void main() {
	Kd = texture(colorTex, tc).rgb;
	Ka = Kd;
	Ks = texture(specularTex, tc).rgb;
	Ke = texture(emiTex, tc).rgb;

	n = 50.0;

	N = normalize(vnormal);
	bumpMap();

	pos = vpos;

	vec3 c = vec3(0.f);

	c += shade_base();
	c += shade_directional_light();
	c += shade_point_light();
	//c += shade_focal_light();

	c = fog(c);

	outColor = vec4(c, 1.0);
}