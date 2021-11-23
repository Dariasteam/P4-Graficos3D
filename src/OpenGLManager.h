#ifndef _OPENGL_MANAGER_H_
#define _OPENGL_MANAGER_H_

#include "auxiliar.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

#define SOLVE_FGLUT_WARNING
#define GLM_FORCE_RADIANS

#include <iostream>
#include <vector>

class OpenGLManager {
public:
	// We use this to preallocate all possible objects of the scene
	unsigned vao;

	unsigned posVBO;
	unsigned colorVBO;
	unsigned normalVBO;
	unsigned texCoordVBO;
	unsigned triangleIndexVBO;

	int inPos;
	int inColor;
	int inNormal;
	int inTexCoord;

	// Identificadores de texturas opengl
	int colorTexId;
	int emiTexId;

	int load_textures();

	//Crea una textura, la configura, la sube a OpenGL,
	//y devuelve el identificador de la textura
	unsigned int loadTex(const char *fileName);

	int instantiateMesh(const unsigned n_vertices, const unsigned n_faces,
											const unsigned *faceIndices,
											const float *vertexCoord, const float *vertexColors,
											const float *normals, const float *texCoords,
											const float *tangents);

	~OpenGLManager() {
		glDeleteBuffers(1, &posVBO);
		glDeleteBuffers(1, &colorVBO);
		glDeleteBuffers(1, &normalVBO);
		glDeleteBuffers(1, &texCoordVBO);
		glDeleteBuffers(1, &triangleIndexVBO);
		glDeleteVertexArrays(1, &vao);
	}
};


#endif