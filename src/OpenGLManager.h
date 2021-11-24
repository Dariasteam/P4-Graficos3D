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

	unsigned vertex_id;
	unsigned fragment_id;

	std::map<std::string, int> uniforms;
	std::map<std::string, int> attributes;

	std::unordered_set<MeshInstance*> asociated_meshes;
};


class OpenGLManager {
private:
  GLuint loadShader(const char *fileName, GLenum type);

public:
	// linked shaders
	std::map<std::string, Program> programs;

	std::map<std::string, unsigned> texture_unit_handler; // TU ids for shaders
	std::map<std::string, unsigned> texture_ids;				  // Texture identifiers

	std::map<std::string, unsigned> vertex_shaders;
	std::map<std::string, unsigned> fragment_shaders;

	// We use this to preallocate all possible objects of the scene
	unsigned vao;

	unsigned posVBO;
	unsigned colorVBO;
	unsigned normalVBO;
	unsigned texCoordVBO;
	unsigned triangleIndexVBO;

	// Identificadores de texturas opengl
	int colorTexId;
	int emiTexId;

	bool load_vertex_shader (const std::string& path, const std::string& name);
	bool load_fragment_shader (const std::string& path, const std::string& name);

	bool create_program (const std::string& name,
											 const std::string& vertex_s_name,
											 const std::string& fragment_s_name,
											 const std::vector<std::string>& uniforms_names,
											 const std::vector<std::string>& attributes_names,
											 int pos = -1);

	bool set_mesh_per_program (const std::string& program_name, MeshInstance* mesh);

	bool load_texture(const std::string& path,
										const std::string& name);

	//Crea una textura, la configura, la sube a OpenGL,
	//y devuelve el identificador de la textura
	unsigned int loadTex(const char *fileName);

	int instantiateMesh(const unsigned n_vertices,
											const unsigned n_faces,
											const unsigned* faceIndices,
											const float* vertexCoord,
											const float* vertexColors,
											const float* normals,
											const float* texCoords,
											const float* tangents,
											Program& program);

	int boundProgramParameters (Program& program);

	~OpenGLManager() {
		destroy ();
	}

	void init_context ();
	void init_OGL ();

	void destroy();
};


#endif