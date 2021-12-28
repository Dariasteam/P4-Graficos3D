#include "../../spatial/camera/Camera.h"
#include "../../scriptable/Scriptable.hpp"

__START_SCRIPT__ (fps_camera, Camera)
  glm::vec4 t {0.1, 0.1, -10, 0};
  glm::vec2 r {0, 0};
  glm::vec2 last_mouse_pos {0, 0};
  glm::vec2 last_rot {0, 0};

  int mouse_button {0};

  glm::vec4 right = glm::vec4 (1.f, 0.f, 0.f, 0.0f);
  glm::vec4 look_at = glm::vec4 (1.f, 0.f, 0.f, 0.0f);

  $.view[3].z = -6.0f;

  $.on_update = [&](const double delta_time) {

    glm::mat4 rot_horizontal(1.0f);
    glm::mat4 rot_vertical(1.0f);
    glm::mat4 translation(1.0f);

    translation = glm::translate(translation, glm::vec3(t.x, t.y, t.z));
    rot_horizontal =
        glm::rotate(rot_horizontal, r.x, glm::vec3(0.0f, 1.0f, 0.0f));
    rot_vertical = glm::rotate(rot_vertical, r.y, glm::vec3(1.0f, 0.0f, 0.0f));

    look_at = glm::vec4(0, 0, 1, 0) * rot_horizontal;
    right = glm::vec4(1, 0, 0, 0) * rot_horizontal;

    // Rotation order guarantees correct horizon
    $.view = rot_vertical * rot_horizontal * translation * glm::mat4(1);;
  };

  $.on_input = [&](const InputEvent& ev) {
    if (ev.type == InputEvent::INPUT_MOUSE_BUTTON) {

      if (ev.state == 0) {
        std::cout << "Se ha pulsado el botón ";
        last_mouse_pos.x = ev.x;
        last_mouse_pos.y = ev.y;
        std::cout << last_mouse_pos.x << " " << last_mouse_pos.y << std::endl;
      } else {
        last_rot = r;
        std::cout << "Se ha soltado el botón ";
      }

    } else if (ev.type == InputEvent::INPUT_MOUSE_MOTION) {

      r.x = last_rot.x + float(ev.x - last_mouse_pos.x) / $.w;
      r.y = last_rot.y + float(ev.y - last_mouse_pos.y) / $.h;

    } else {

      const float linear_speed = 0.05f;  // Move per frame
      const float angular_speed = 0.01f; // Move per frame

      switch (ev.key) {
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

  };

__END_SCRIPT__
