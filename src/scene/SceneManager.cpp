#include "SceneManager.hpp"
#include "Scene.hpp"

void SceneManager::add_scene(const std::string &name, Scene *scene) {
  scenes[name] = scene;
}

bool SceneManager::set_init_scene (const std::string &scene_name) {
  const auto &it = scenes.find(scene_name);

  if (it == scenes.end()) {
    std::cerr << "Error cambiando de escena. No existe la escena " << scene_name
              << std::endl;
    return false;
  }
  Scene &scene = *it->second;
  current_scene = &scene;
}

void SceneManager::init () {
  current_scene->init();
}

bool SceneManager::change_scene (const std::string &scene_name) {
  const auto &it = scenes.find(scene_name);

  if (it == scenes.end()) {
    std::cerr << "Error cambiando de escena. No existe la escena " << scene_name
              << std::endl;
    return false;
  }

  end_scene();

  TextureManager::get().clean();
  ShaderManager::get().clean();
  VBOManager::get().clean();
  MaterialManager::get().clean();
  LightManager::get().clean();
  WorldManager::get().clear();
  MeshLoader::get().clean(); // NOTE: We are doing this in the init lambdas of the scenes

  Scene &scene = *it->second;

  current_scene = &scene;
  scene.init();

  WorldManager::get().camera->set_w(w);
  WorldManager::get().camera->set_h(h);
}
