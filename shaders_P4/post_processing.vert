#version 330 core

// Identificador 0 tal y como le dimos en el cliente
layout(location = 0) in vec3 inPos;

out vec2 texCoord;

void main() {

  // operacion multiplicación-y-suma. Optimizado
  texCoord = 0.5 * (inPos.xy + vec2(1));
  gl_Position = vec4(inPos.xyz, 1);
}


