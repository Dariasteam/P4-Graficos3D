#include "../../spatial/camera/Camera.h"
#include "../../scriptable/Scriptable.hpp"

__START_SCRIPT__ (orbital_camera, Camera)
  glm::vec4 t {0.1, 0.1, -10, 0};
  glm::vec2 r {0, 0};
  glm::vec2 last_mouse_pos {0, 0};
  glm::vec2 last_rot {0, 0};

  int mouse_button {0};

  glm::vec4 right = glm::vec4 (1.f, 0.f, 0.f, 0.0f);

  $.on_update = [&](const double delta_time) {
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
    right = glm::vec4(1, 0, 0, 0) * rot_horizontal;

    translation = glm::translate(translation, glm::vec3(t2.x, t2.y, t2.z));
    $.view = rot_horizontal * rot_vertical * translation * glm::mat4(1);
  };

  $.on_input = [&](const InputEvent& ev) {
    if (ev.type == InputEvent::INPUT_MOUSE_BUTTON) {

      mouse_button = ev.button;
      if (ev.state == 0) { // PRESS
        last_mouse_pos.x = ev.x;
        last_mouse_pos.y = ev.y;
      } else { // RELEASE
        last_rot = r;
      }

    } else if (ev.type == InputEvent::INPUT_MOUSE_MOTION) {

      int x = ev.x;
      int y = ev.y;

      unsigned w = $.get_w();
      unsigned h = $.get_h();

      if (mouse_button == 0) { // PAN
        r.x = last_rot.x + float(x - last_mouse_pos.x) / w;
        r.y = last_rot.y + float(y - last_mouse_pos.y) / h;
      } else if (mouse_button == 1) { // ZOOM
        t.z += float(y - last_mouse_pos.y) / (h / 2);
        last_mouse_pos.y = y;
      }

    } else {

      switch (ev.key) {
        // ZOOM TRANSLATION
        case 'w':
          t.z += .1;
          break;

        case 's':
          t.z -= .1;
          break;
      }

    }

  };

__END_SCRIPT__
};