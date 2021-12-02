#version 330 core

in vec2 texCoord;
out vec4 color;

uniform sampler2D colorTex;
uniform sampler2D zTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;

const float focalDistance = -25.0;
const float maxDistanceFactor = 1.0/5.0;  // A una distancia de 5 unidades borrosidad máxima

void main() {
  color = texture(colorTex, texCoord);
}