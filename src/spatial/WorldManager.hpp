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

  std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<long, std::ratio<1, 1000000000>>> prev;

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
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> delta_time = now - prev;
    for (auto* object : world_objects) {
      object->on_update(delta_time.count());
    }
    prev = now;
  }

  void clear () {
    for (auto* object : world_objects)
      delete object;

    delete camera;

    world_objects.clear();
  }
};

#endif // _WORLD_MANAGER_H_