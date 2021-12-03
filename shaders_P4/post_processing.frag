#version 330 core

out vec4 color;

in vec2 texCoord;

uniform sampler2D colorTex;
uniform sampler2D zTex;

void main() {
  vec3 c = texture(colorTex, texCoord).rgb;
	float z  = texture(zTex, texCoord).x;
  color = vec4(c, 1);
}