#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include "../material/Material.hpp"
#include "Shaders.hpp"
#include "../spatial/mesh/MeshInstance.hpp"

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

	std::unordered_set<Material*> associated_materials;

	void detach () {
    glDetachShader(id, vertex->id);
    glDetachShader(id, fragment->id);
	}

	~Program () {
		glDeleteProgram(id);
	}

	void add_associated_material (Material& material) {
		associated_materials.insert(&material);

		for (const auto& uniform : uniforms) {
			material.bind(uniform.first, uniform.second);
		}
	}

};

#endif // _PROGRAM_H_