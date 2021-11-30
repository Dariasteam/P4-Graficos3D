#ifndef _MESH_INSTANCE_H_
#define _MESH_INSTANCE_H_

#include "OGLObject.hpp"
#include "Spatial.h"
#include "OGLMesh.hpp"
#include "Material.hpp"
#include "Shaders.hpp"

struct MeshInstance : public Spatial {
protected:
	int obj_id = -1;
	std::map<std::string, AbstractShaderParameter*> shader_uniforms;
public:
	Material* mat;
	const OglMesh* mesh;

	// FIXME: memory leak here
	MeshInstance () : mat(new Material) {}

	int get_obj_id () { return obj_id; }

	MeshInstance (const OglMesh& instanced_mesh) {
		mesh = &instanced_mesh;
	}

  glm::mat4 get_model_matrix () {
    glm::mat4 t = glm::translate(_local_transform.mat_4, _translation);
    glm::mat4 r_x = glm::rotate(_local_transform.mat_4, _rotation_angle.x, glm::vec3{1, 0, 0});
    glm::mat4 r_y = glm::rotate(_local_transform.mat_4, _rotation_angle.y, glm::vec3{0, 1, 0});
    glm::mat4 r_z = glm::rotate(_local_transform.mat_4, _rotation_angle.z, glm::vec3{0, 0, 1});

		// NOTE: This is an ordered rotation (gymball lock?)
		if (modified)
    	prev_transform.mat_4 = t * r_x * r_y * r_z * _local_transform.mat_4;

		modified = false;
		return prev_transform.mat_4;
  }



	void update (float dummy_time) {
    update_logic (*this, dummy_time);
	}
};


#endif // _MESH_INSTANCE_H_