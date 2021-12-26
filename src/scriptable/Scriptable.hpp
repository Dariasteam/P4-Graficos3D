#ifndef _SCRIPTABLE_H_
#define _SCRIPTABLE_H_

#include <functional>

struct InputEvent {
  enum {
    INPUT_MOUSE_BUTTON,
    INPUT_MOUSE_MOTION,
    INPUT_KEYBOARD,
  };

  unsigned type;
  unsigned x, y;
  char key;
};

struct Scriptable {
  std::function<void (const float dummy_time)> on_update = [](const float){};
  std::function<void (const InputEvent& event)> on_input = [](const InputEvent& event) {};
};

#endif // _SCRIPTABLE_