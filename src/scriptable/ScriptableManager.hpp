#ifndef _SCRIPTABLE_MANAGER_H_
#define _SCRIPTABLE_MANAGER_H_

#include "Scriptable.hpp"

#include <unordered_set>

class ScriptableManager {
private:
  std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<long, std::ratio<1, 1000000000>>> prev;
  ScriptableManager () {}
  std::unordered_set<Scriptable*> scriptable_objects;
public:
  inline static ScriptableManager& get () {
    static ScriptableManager instance;
    return instance;
  }

  ScriptableManager (const ScriptableManager&) = delete;
  void operator= (const ScriptableManager&) = delete;

  void add_object(Scriptable* obj) {
    scriptable_objects.insert(obj);
  }

  void send_event (const InputEvent& ev) {
    for (auto* object : scriptable_objects) {
      object->on_input(ev);
    }
  }

  void on_mouse_button (unsigned button, unsigned state, unsigned x, unsigned y) {
    InputEvent ev {
      InputEvent::INPUT_MOUSE_BUTTON,
      x, y,
      state
    };

    ev.button = button;

    send_event(ev);
  }

  void on_mouse_motion (unsigned x, unsigned y) {
    InputEvent ev {
      InputEvent::INPUT_MOUSE_MOTION,
      x, y,
    };

    send_event(ev);
  }

  void on_keyboard (unsigned char k) {
    InputEvent ev {
      InputEvent::INPUT_KEYBOARD,
    };
    ev.key = k;

    send_event(ev);
  }

  void on_update() {
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> delta_time = now - prev;
    for (auto* object : scriptable_objects) {
      object->on_update(delta_time.count());
    }
    prev = now;
  }

  void clear () {
    scriptable_objects.clear();
  }
};

#endif // _SCRIPTABLE_MANAGER_H_