#ifndef _DEMO_H_
#define _DEMO_H_

#include "../scene/SceneManager.hpp"
#include "../spatial/mesh/MeshLoader.h"
#include "../spatial/WorldManager.hpp"
#include "../shader/ShaderManager.hpp"
#include "../texture/TextureManager.hpp"
#include "../material/MaterialManager.hpp"
#include "../ogl/VBOManager.hpp"

const WorldManager& world_manager = WorldManager::get();
const ShaderManager& shader_manager = ShaderManager::get();
const TextureManager& texture_manager = TextureManager::get();
const VBOManager& vbo_manager = VBOManager::get();
const MaterialManager& material_manager = MaterialManager::get();
const MeshLoader& mesh_loader = MeshLoader::get();

#endif // _DEMO_H_