#ifndef _SCRIPTABLE_H_
#define _SCRIPTABLE_H_

#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/matrix.hpp>
#include <glm/ext/matrix_transform.hpp>
#define GLM_FORCE_RADIANS

#include <functional>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <iostream>
#include <string>
#include <map>

#define __START_SCRIPT__(_NAME_, _EXTENDS_) auto _NAME_ = [](Scriptable& self) -> void { _EXTENDS_& $ = *static_cast<_EXTENDS_*> (&self);
#define __END_SCRIPT__  std::condition_variable cv; std::mutex m; std::unique_lock<std::mutex> lock(m); cv.wait(lock, []{return false;}); std::cout << "ERROR: This should never be reach in execution" << std::endl; };
#define public_(_NAME_) _NAME_; $.public_parameters[#_NAME_] = (void*)&_NAME_ ; _NAME_

struct InputEvent {
  enum {
    INPUT_MOUSE_BUTTON,
    INPUT_MOUSE_MOTION,
    INPUT_KEYBOARD,
  };

  unsigned type;
  unsigned x;
  unsigned y;
  unsigned state;
  unsigned button;
  char key;
};

template<class T>
struct public_parameter {
  T* parameter;
};

struct Scriptable {
  std::function<void (const float dummy_time)> on_update = [](const float){};
  std::function<void (const InputEvent& event)> on_input = [](const InputEvent& event) {};

  std::map<std::string, void*> public_parameters;
  std::function<void (const std::string& name)> get = [](const std::string& name) {};

  template<class T>
  bool get_parameter(const std::string& s, T** t) {
    auto it = public_parameters.find(s);
    if (it == public_parameters.end()) {
      return false;
    } else {
      *t = (T*)it->second;
      return true;
    }
  };

  void script(const std::function<void (Scriptable& self)>& s) {
    std::thread t ([=]() {
      s (*this);
    });
    t.detach();
  }
};

#endif // _SCRIPTABLE_