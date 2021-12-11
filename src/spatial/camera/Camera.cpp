#include "Camera.h"

// ABSTRACT

void AbstractCamera::update_projection(double a) {
	float n = 1.0f;
	float f = 15.0f;
	float x = 1.0f / (glm::tan(30.0f * 3.1419f / 180.0f));
	proj[0].x = x * 1.0 / a;
	proj[1].y = x;
	proj[2].z = (f + n) / (n - f);
	proj[2].w = -1.0f;
	proj[3].z = 2.0f * n * f / (n - f);
}

void AbstractCamera::update_aspect_ratio(int width, int height) {
  set_w (width);
  set_h (height);

  update_projection(float (w) / float(h));
}

// FPS

FPSCamera::FPSCamera() { view[3].z = -6.0f; }

void FPSCamera::handle_keys(unsigned char key) {

  std::cout << "Se ha pulsado la tecla " << key << std::endl << std::endl;

  const float linear_speed = 0.05f;  // Move per frame
  const float angular_speed = 0.01f; // Move per frame

  switch (key) {
    // TRANSLATIONS

  case 'w':
    t += look_at * linear_speed;
    break;

  case 's':
    t -= look_at * linear_speed;
    break;

  case 'a':
    t += right * linear_speed;
    break;

  case 'd':
    t -= right * linear_speed;
    break;

    // ROTATIONS

  case 'i':
    r.y -= angular_speed;
    break;

  case 'k':
    r.y += angular_speed;
    break;

  case 'j':
    r.x -= angular_speed;
    break;

  case 'l':
    r.x += angular_speed;
    break;

  default:
    return;
  }
}
void FPSCamera::handle_mouse_buttons(int button, int state, int x,
                                            int y) {
  if (state == 0) {
    std::cout << "Se ha pulsado el botón ";
    last_mouse_pos.x = x;
    last_mouse_pos.y = y;
    std::cout << last_mouse_pos.x << " " << last_mouse_pos.y << std::endl;
  } else {
    last_rot = r;
    std::cout << "Se ha soltado el botón ";
  }

  if (button == 0)
    std::cout << "de la izquierda del ratón " << std::endl;
  if (button == 1)
    std::cout << "central del ratón " << std::endl;
  if (button == 2)
    std::cout << "de la derecha del ratón " << std::endl;

  std::cout << "en la posición " << x << " " << y << std::endl << std::endl;
}

void FPSCamera::handle_mouse_motion(int x, int y) {
  r.x = last_rot.x + float(x - last_mouse_pos.x) / w;
  r.y = last_rot.y + float(y - last_mouse_pos.y) / h;
}

glm::mat4 FPSCamera::get_view_matrix() {
  glm::mat4 rot_horizontal(1.0f);
  glm::mat4 rot_vertical(1.0f);
  glm::mat4 translation(1.0f);

  translation = glm::translate(translation, glm::vec3(t.x, t.y, t.z));
  rot_horizontal =
      glm::rotate(rot_horizontal, r.x, glm::vec3(0.0f, 1.0f, 0.0f));
  rot_vertical = glm::rotate(rot_vertical, r.y, glm::vec3(1.0f, 0.0f, 0.0f));

  look_at = view[2] * rot_horizontal;
  right = view[0] * rot_horizontal;

  // Rotation order guarantees correct horizon
  return rot_vertical * rot_horizontal * translation * view;
}

// ORBITAL

OrbitalCamera::OrbitalCamera() {
  // t.z used as radius of the rotation
  t.z = -10;
  t.x = 0.1;
  t.y = 0.1;

  // Put camera in the center of the world (origin of the rotation)
  view[3].z = 0;
}

glm::mat4 OrbitalCamera::get_view_matrix() {
  glm::mat4 rot_horizontal(1.0f);
  glm::mat4 rot_vertical(1.0f);
  glm::mat4 translation(1.0f);

  rot_horizontal =
      glm::rotate(rot_horizontal, r.x, glm::vec3(0.0f, 1.0f, 0.0f));
  rot_vertical =
      glm::rotate(rot_vertical, r.y, glm::vec3(right.x, right.y, right.z));

  // Move position in base of the rotation
  auto t2 = t * rot_horizontal * rot_vertical;

  // Adjust right vector to obtain correct vertical rotation
  right = view[0] * rot_horizontal;

  translation = glm::translate(translation, glm::vec3(t2.x, t2.y, t2.z));

  return rot_horizontal * rot_vertical * translation * view;
}

void OrbitalCamera::handle_keys(unsigned char key) {

  switch (key) {
    // TRANSLATIONS
    case 'w':
      t.z += .1;
      break;

    case 's':
      t.z -= .1;
      break;
  }
}

void OrbitalCamera::handle_mouse_buttons(int button, int state, int x,
                                                int y) {
  mouse_button = button;
  if (state == 0) { // PRESS
    last_mouse_pos.x = x;
    last_mouse_pos.y = y;
  } else { // RELEASE
    last_rot = r;
  }
};

void OrbitalCamera::handle_mouse_motion(int x, int y) {
  if (mouse_button == 0) { // PAN
    r.x = last_rot.x + float(x - last_mouse_pos.x) / w;
    r.y = last_rot.y + float(y - last_mouse_pos.y) / h;
  } else if (mouse_button == 1) { // ZOOM
    t.z += float(y - last_mouse_pos.y) / (h / 2);
    last_mouse_pos.y = y;
  }
}
