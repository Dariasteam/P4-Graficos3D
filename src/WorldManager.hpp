#ifndef _WORLD_MANAGER_H_
#define _WORLD_MANAGER_H_

#include "Light.hpp"
#include "MeshInstance.hpp"
#include "OGLMesh.hpp"
#include "LightManager.hpp"

#include <vector>

class WorldManager {
private:
  WorldManager () {}

  std::vector<Spatial*> world_objects;
public:
  inline static WorldManager& get () {
    static WorldManager instance;
    return instance;
  }

  WorldManager (const WorldManager&) = delete;
  void operator= (const WorldManager&) = delete;

  inline MeshInstance& create_mesh_instance(const OglMesh& ogl_mesh) {
    MeshInstance* mesh = new MeshInstance (ogl_mesh);
    world_objects.push_back(mesh);
    return *mesh;
  }

  inline DirectionalLight& create_directional_light() {
    DirectionalLight* light = new DirectionalLight;

    LightManager::get().dir_lights.push_back(light);

    world_objects.push_back(light);
    return *light;
  }

  inline PointLight& create_point_light() {
    PointLight* light = new PointLight;

    LightManager::get().point_lights.push_back(light);

    world_objects.push_back(light);
    return *light;
  }

  inline FocalLight& create_focal_light() {
    FocalLight* light = new FocalLight;

    LightManager::get().focal_lights.push_back(light);

    world_objects.push_back(light);
    return *light;
  }


};

#endif // _WORLD_MANAGER_H_