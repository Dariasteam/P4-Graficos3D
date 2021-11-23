#ifndef _OPENGL_MANAGER_H_
#define _OPENGL_MANAGER_H_

#include "Spatial.h"
#include "auxiliar.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

#define SOLVE_FGLUT_WARNING
#define GLM_FORCE_RADIANS

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <unordered_set>

struct Program {
	unsigned id;
	std::map<std::string, int> uniforms;
	std::map<std::string, int> attributes;

	std::unordered_set<Mesh*> asociated_meshes;
};


class OpenGLManager {
private:
  GLuint loadShader(const char *fileName, GLenum type);
	template <typename T>
	void insert_at (std::vector<T>& container, T element, int pos = -1);


public:

	// linked shaders
	std::vector<Program> programs;
	std::vector<unsigned> vertex_shaders;
	std::vector<unsigned> fragment_shaders;

	// We use this to preallocate all possible objects of the scene
	unsigned vao;

	unsigned posVBO;
	unsigned colorVBO;
	unsigned normalVBO;
	unsigned texCoordVBO;
	unsigned triangleIndexVBO;

/*
	int inPos;
	int inColor;
	int inNormal;
	int inTexCoord;
*/
	// Identificadores de texturas opengl
	int colorTexId;
	int emiTexId;

	bool load_vertex_shader (const std::string path, int pos = -1);
	bool load_fragment_shader (const std::string path, int pos = -1);

	bool create_program (unsigned V,
											 unsigned F,
											 const std::vector<std::string>& uniforms_names,
											 const std::vector<std::string>& attributes_names,
											 int pos = -1);

	bool set_mesh_per_program (const unsigned programId, Mesh* mesh);

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