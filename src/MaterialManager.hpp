#ifndef _MATERIAL_MANAGER_H_
#define _MATERIAL_MANAGER_H_

#include "Material.hpp"

#include <vector>

class MaterialManger {
private:
  MaterialManger () {}
public:
  std::vector<Material*> materials;

  inline static MaterialManger& get () {
    static MaterialManger instance;
    return instance;
  }

  MaterialManger (const MaterialManger&) = delete;
  void operator= (const MaterialManger&) = delete;

  // FIXME: We could receibve the shader and prebind here
  inline Material& create_material () {
    Material* mat = new Material;
    materials.push_back(mat);
    return *mat;
  }


  void clean () {
    for (auto* material : materials)
      delete material;

    materials.clear();
  }
};

#endif // #ifndef _MATERIAL_MANAGER_H_