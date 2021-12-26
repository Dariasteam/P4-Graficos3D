#ifndef _MESH_H_
#define _MESH_H_

#include "../shader/Shaders.hpp"
#include "../scriptable/Scriptable.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <iostream>
#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_FORCE_RADIANS

struct Spatial : public Scriptable{
protected:
	glm::vec3 _translation {0, 0, 0};
	glm::vec3 _rotation_angle {0, 0, 0};
	glm::vec3 _scale {1, 1, 1};

	SP_Mat4f _local_transform {glm::mat4(1.0f)}; // Transform / View

	SP_Mat4f prev_transform = _local_transform.mat_4;

	bool modified = false;
public:

  glm::vec3& translation() {
    modified = true;
    return _translation;
  }

	glm::vec3& scale() {
		modified = true;
		return _scale;
	}

  glm::vec3& rotation() {
    modified = true;
    return _rotation_angle;
  }

	virtual glm::mat4 get_model_matrix () = 0;

	std::function<void (const float dummy_time)> update_logic
		= [](const float dummy_time) {};
};


#endif