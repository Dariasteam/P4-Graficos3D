#ifndef _SCRIPTABLE_MANAGER_H_
#define _SCRIPTABLE_MANAGER_H_

#include "Scriptable.hpp"

#include <unordered_set>

class ScriptableManager {
private:
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

  void on_mouse_button (int button, int state, int x, int y) {}
  void on_mouse_motion (int x, int y) {}
  void on_keyboard (unsigned char k) {}
  void on_update() {}

  void clear () {
    scriptable_objects.clear();
  }
};

#endif // _SCRIPTABLE_MANAGER_H_