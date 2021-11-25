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

struct OglObject {
	unsigned id;
};

struct FragmentShader : public OglObject {
	~FragmentShader() {
		glDeleteShader(id);
	}
};

struct VertexShader : public OglObject {
	~VertexShader() {
		glDeleteShader(id);
	}
};

struct Program : OglObject{

	const VertexShader* vertex;
	const FragmentShader* fragment;

	std::map<std::string, int> uniforms;
	std::map<std::string, int> attributes;

	std::unordered_set<MeshInstance*> asociated_meshes;

	void detach () {
    glDetachShader(id, vertex->id);
    glDetachShader(id, fragment->id);
	}

	~Program () {
		glDeleteProgram(id);
	}

};

struct Texture : public OglObject {
	unsigned n_texture;
};

class OpenGLManager {
private:
  GLuint loadShader(const char *fileName, GLenum type);

public:
	// linked shaders
	std::map<std::string, Program*> programs;
	std::map<std::string, Texture> texture_ids;				  // Texture identifiers

	std::map<std::string, VertexShader*> vertex_shaders;
	std::map<std::string, FragmentShader*> fragment_shaders;

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
											 const VertexShader& vertex_shader,
											 const FragmentShader& fragment_shader,
											 const std::vector<std::string>& uniforms_names,
											 const std::vector<std::string>& attributes_names);

	void set_mesh_per_program (Program& program,
														 MeshInstance* mesh) const;

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
											const Program& program);

	int boundProgramParametersAttributes (Program& program,
																				const std::map<std::string, unsigned>&
																				attribute_name_location);

	~OpenGLManager() {
		destroy ();
	}

	void init_context ();
	void init_OGL ();

	void destroy();
};


#endif