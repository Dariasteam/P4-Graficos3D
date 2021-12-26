#ifndef _WORLD_MANAGER_H_
#define _WORLD_MANAGER_H_

#include "camera/Camera.h"
#include "light/Light.hpp"
#include "mesh/MeshInstance.hpp"
#include "mesh/OGLMesh.hpp"

#include <vector>

class WorldManager {
private:
  WorldManager () {}

  float dummy_time = 0;

  std::vector<Spatial*> world_objects;
public:
  AbstractCamera* camera;

  void add (Spatial* sp) { world_objects.push_back(sp); }

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

  void update () {
    for (auto* object : world_objects) {
      object->on_update(dummy_time);
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