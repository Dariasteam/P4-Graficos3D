#ifndef _DEMO_H_
#define _DEMO_H_

#include "../spatial/light/LightManager.hpp"
#include "../scene/SceneManager.hpp"
#include "../spatial/mesh/MeshLoader.h"
#include "../spatial/WorldManager.hpp"
#include "../shader/ShaderManager.hpp"
#include "../texture/TextureManager.hpp"
#include "../material/MaterialManager.hpp"
#include "../ogl/VBOManager.hpp"
#include "../scriptable/ScriptableManager.hpp"

namespace demo_default_objs {
  WorldManager& world_manager = WorldManager::get();
  ShaderManager& shader_manager = ShaderManager::get();
  TextureManager& texture_manager = TextureManager::get();
  VBOManager& vbo_manager = VBOManager::get();
  MaterialManager& material_manager = MaterialManager::get();
  LightManager& light_manager = LightManager::get();
  SceneManager& scene_manager = SceneManager::get();
  MeshLoader& mesh_loader = MeshLoader::get();
  ScriptableManager& scriptable_manager = ScriptableManager::get();
}


#endif // _DEMO_H_