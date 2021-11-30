#ifndef _WORLD_MANAGER_H_
#define _WORLD_MANAGER_H_

#include "camera/Camera.h"
#include "light/LightManager.hpp"
#include "mesh/MeshInstance.hpp"
#include "mesh/OGLMesh.hpp"

#include <vector>

class WorldManager {
private:
  WorldManager () {}

  float dummy_time = 0;

  std::vector<Spatial*> world_objects;
public:
  AbstractCameraHandler* camera;

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

  void update () {
    for (auto* object : world_objects) {
      object->update(dummy_time);
    }
    dummy_time += .1;
  }

  void clear () {
    for (auto* object : world_objects)
      delete object;

    delete camera;

    world_objects.clear();
    dummy_time = 0;
  }
};

#endif // _WORLD_MANAGER_H_