#ifndef _MESH_H_
#define _MESH_H_

#include "BOX.h"
//#include "IGlib.h"
#include "AssimpManager.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <iostream>
#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_FORCE_RADIANS

struct Spatial {
protected:
	glm::vec3 _translation {0, 0, 0};
	glm::vec3 _rotation_angle {0, 0, 0};
	glm::vec3 _scale {0, 0, 0};

	glm::mat4 _local_transform = glm::mat4(1.0f);; // Transform / View

	glm::mat4 prev_transform = _local_transform;
public:

  glm::vec3& translation() {
    modified = true;
    return _translation;
  }

  glm::vec3& rotation() {
    modified = true;
    return _rotation_angle;
  }

	bool modified = false;

	virtual glm::mat4 get_model_matrix () = 0;
	virtual void update (float dummy_time) = 0;

	std::function<void (Spatial& self, const float dummy_time)> update_logic
    = [](Spatial& self, const float dummy_time) -> bool {
		return false;
	};
};

struct MeshInstance : public Spatial {
protected:
	int obj_id = -1;

public:
	int get_obj_id () { return obj_id; }
	void set_obj_id (int id) { obj_id = id; }

  glm::mat4 get_model_matrix () {
    glm::mat4 t = glm::translate(_local_transform, _translation);
    glm::mat4 r_x = glm::rotate(_local_transform, _rotation_angle.x, glm::vec3{1, 0, 0});
    glm::mat4 r_y = glm::rotate(_local_transform, _rotation_angle.y, glm::vec3{0, 1, 0});
    glm::mat4 r_z = glm::rotate(_local_transform, _rotation_angle.z, glm::vec3{0, 0, 1});

		// NOTE: This is an ordered rotation (gymball lock?)
		if (modified)
    	prev_transform = t * r_x * r_y * r_z * _local_transform;

		modified = false;
		return prev_transform;
  }

	void update (float dummy_time) {
    update_logic (*this, dummy_time);
	}
};

struct CubeMesh : public MeshInstance {
	CubeMesh () {
		/*
		obj_id = IGlib::createObj(cubeNTriangleIndex,
						   								cubeNVertex,
						   								cubeTriangleIndex,
						   								cubeVertexPos,
						   								cubeVertexColor,
						   								cubeVertexNormal,
						   								cubeVertexTexCoord,
						   								cubeVertexTangent);
		*/
	}
};

struct LoadableMesh : public MeshInstance {
	int load_mesh (const std::string& filename) {
		int aux_obj_id = AssimpManager::import_from_file (filename);
		if (aux_obj_id < 0) {
			std::cerr << "No se pudo cargar el modelo " << filename << std::endl;
			return -3;
		} else {
			obj_id = aux_obj_id;
		}
		return 0;
	}
};

#endif