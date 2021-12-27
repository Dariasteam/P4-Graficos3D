#include "../../spatial/light/Light.hpp"
#include "../../scriptable/Scriptable.hpp"

__START_SCRIPT__ (rotate_dir_light, DirectionalLight)

  $.on_input = [&](const InputEvent& ev) {
    if (ev.type == InputEvent::INPUT_KEYBOARD &&
                (ev.key == 'v' || ev.key == 'V')) {
      auto& value = $.direction.vec_3;
      if (value.x > .5)
        value.x = -1;
      else
        value.x += .1;

      value = glm::normalize(value);
    }
  };

__END_SCRIPT__
