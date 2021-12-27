#include "../../spatial/light/Light.hpp"
#include "../../scriptable/Scriptable.hpp"

__START_SCRIPT__ (blue_light, PointLight)

  $.on_input = [&](const InputEvent& ev) {
    if (ev.type == InputEvent::INPUT_KEYBOARD &&
                (ev.key == 'f' || ev.key == 'F')) {
      auto& value = $.color.vec_3.b;

      if (value > 10)
        value = 0;
      else
        value += .1;
    }
  };

__END_SCRIPT__
