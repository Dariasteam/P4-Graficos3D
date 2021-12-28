#include "Camera.h"
#include <glm/fwd.hpp>

// ABSTRACT

void Camera::update_projection(double a) {
	float n = .2f;
	float f = 15.0f;
	float x = 1.0f / (glm::tan(30.0f * 3.1419f / 180.0f));
	proj[0].x = x * 1.0 / a;
	proj[1].y = x;
	proj[2].z = (f + n) / (n - f);
	proj[2].w = -1.0f;
	proj[3].z = 2.0f * n * f / (n - f);
}

void Camera::update_aspect_ratio(int width, int height) {
  set_w (width);
  set_h (height);

  update_projection(float (w) / float(h));
}