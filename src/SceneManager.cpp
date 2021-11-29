#include "SceneManager.hpp"
#include "MaterialManager.hpp"
#include "MeshLoader.h"
#include "ShaderManager.hpp"
#include "TextureManager.hpp"
#include "VBOManager.hpp"

void SceneManager::add_scene(const std::string &name, Scene *scene) {
  // Gives each scene a lambda to call when it wants to finish
  scene->change_scene = [&](const std::string &scene_name) {
    set_active_scene(scene_name);
  };
  scenes[name] = scene;
}

bool SceneManager::set_active_scene(const std::string &scene_name) {
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
  MaterialManger::get().clean();
  MeshLoader::get().clean(); // NOTE: We are doing this in the init lambdas of the scenes

  Scene &scene = *it->second;

  current_scene = &scene;
  scene.init();
}
