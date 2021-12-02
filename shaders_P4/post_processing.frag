#version 330 core

in vec2 texCoord;
out vec4 color;

uniform sampler2D colorTex;

void main() {
  //color = texture(tex, texCoor);

  //Código del Shader
  //NOTA: Sería más adecuado usar texelFetch.
  //NOTA: No lo hacemos porque simplifica el paso 5

  // No hacemos ningun cálculo de mipmap, accedemos al nivel 0
  color = vec4(textureLod(colorTex, texCoord, 0).xyz, 11);
  //color = vec4(texCoord,vec2(1.0));
}