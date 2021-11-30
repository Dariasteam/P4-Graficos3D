#ifndef _MESH_H_
#define _MESH_H_

#include "Shaders.hpp"
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

	SP_Mat4f _local_transform {glm::mat4(1.0f)}; // Transform / View

	SP_Mat4f prev_transform = _local_transform.mat_4;
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


#endif