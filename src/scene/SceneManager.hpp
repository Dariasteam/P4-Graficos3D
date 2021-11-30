#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "Scene.hpp"

#include <map>
#include <functional>

class SceneManager {
private:
  std::map<std::string, Scene*> scenes;
  Scene* current_scene {nullptr};

  SceneManager () {}
public:

  inline static SceneManager& get () {
    static SceneManager instance;
    return instance;
  }

  SceneManager (const SceneManager&) = delete;
  void operator= (const SceneManager&) = delete;


  std::function<void (void)> end_scene = [](){};

  inline Scene* get_current_scene () { return current_scene; }

  void init ();
  void add_scene(const std::string &name, Scene *scene);
  bool set_init_scene (const std::string& scene_name);
  bool change_scene(const std::string &scene_name);
};

#endif // _SCENE_MANAGER_H_