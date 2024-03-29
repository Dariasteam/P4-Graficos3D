#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "../../shader/Shaders.hpp"
#include "../Spatial.h"


#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>

#include <iostream>

class Camera : public Spatial {
public:
	glm::vec4 t {0, 0, 0, 0};
	glm::vec2 r {0, 0};
	glm::vec2 last_mouse_pos {0, 0};
	glm::vec2 last_rot {0, 0};

	glm::mat4 proj = glm::mat4(0.0f);

	glm::mat4 view = glm::mat4(1.0f); //Identity matrix

	glm::vec4 look_at = glm::vec4 (0.f, 0.f, -1.f, 0.0f);
	glm::vec4 up = glm::vec4 (0.f, 1.f, 0.f, 0.0f);
	glm::vec4 right = glm::vec4 (1.f, 0.f, 0.f, 0.0f);

  unsigned w = 0;
  unsigned h = 0;

	glm::mat4 get_model_matrix() {}

public:

	glm::mat4 get_view_matrix() { return view; }
	glm::mat4 get_projection_matrix() { return proj; };


	void update_aspect_ratio(int width, int height);
	void update_projection (double a);

  inline void set_w (const unsigned W) { w = W; }
  inline void set_h (const unsigned H) { h = H; }

	inline unsigned get_w () { return w; }
	inline unsigned get_h () { return h; }

	Camera () {
		update_projection(1);
	}

};

#endif // _CAMERA_H_