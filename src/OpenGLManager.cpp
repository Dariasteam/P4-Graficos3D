#include "OpenGLManager.h"

int OpenGLManager::load_textures() {
  colorTexId = loadTex("img/color2.png");
  emiTexId = loadTex("img/emissive.png");
}

unsigned int OpenGLManager::loadTex(const char *fileName) {
  unsigned char *map;
  unsigned int w, h;
  map = loadTexture(fileName, w, h);
  if (!map) {
    std::cout << "Error cargando el fichero: " << fileName << std::endl;
    exit(-1);
  }

  unsigned int texId;
  glGenTextures(1, &texId);
  glBindTexture(GL_TEXTURE_2D, texId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               (GLvoid *)map);

  delete[] map;

  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

  return texId;
}

int OpenGLManager::instantiateMesh(const unsigned n_vertices,
                                   const unsigned n_faces,
                                   const unsigned *faceIndices,
                                   const float *vertexCoord,
                                   const float *vertexColors,
                                   const float *normals, const float *texCoords,
                                   const float *tangents) {

  glGenBuffers(1, &posVBO);
  glGenBuffers(1, &colorVBO);
  glGenBuffers(1, &normalVBO);
  glGenBuffers(1, &texCoordVBO);

  glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
  glBufferData(GL_ARRAY_BUFFER, n_vertices * sizeof(float) * 2, texCoords,
               GL_STATIC_DRAW);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, posVBO);
  glBufferData(GL_ARRAY_BUFFER, n_vertices * sizeof(float) * 3, NULL,
               GL_STATIC_DRAW);

  glBufferSubData(GL_ARRAY_BUFFER, 0, n_vertices * sizeof(float) * 3,
                  vertexCoord);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  if (inPos != -1)
    glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
  glBufferData(GL_ARRAY_BUFFER, n_vertices * sizeof(float) * 3, vertexColors,
               GL_STATIC_DRAW);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

  if (inColor != -1)
    glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
  glBufferData(GL_ARRAY_BUFFER, n_vertices * sizeof(float) * 3, normals,
               GL_STATIC_DRAW);

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

  if (inNormal != -1)
    glEnableVertexAttribArray(inNormal);

  glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
  if (inTexCoord != -1)
    glEnableVertexAttribArray(3);

  glGenBuffers(1, &triangleIndexVBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_faces * sizeof(unsigned int) * 3,
               faceIndices, GL_STATIC_DRAW);
}
