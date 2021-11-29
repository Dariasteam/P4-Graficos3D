#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include "Shaders.hpp"
#include "MeshInstance.hpp"

#include <map>
#include <vector>
#include <unordered_set>
#include <string>

struct Program : OglObject{

	const VertexShader* vertex;
	const FragmentShader* fragment;

	// IDs for Uniforms and Attributes
	std::map<std::string, int> uniforms;
	std::map<std::string, int> attributes;

	std::unordered_set<MeshInstance*> associated_meshes;

	void detach () {
    glDetachShader(id, vertex->id);
    glDetachShader(id, fragment->id);
	}

	~Program () {
		glDeleteProgram(id);
	}

};

#endif // _PROGRAM_H_