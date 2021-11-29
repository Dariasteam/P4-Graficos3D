#ifndef _MATERIAL_MANAGER_H_
#define _MATERIAL_MANAGER_H_

#include "Material.hpp"

#include <vector>

class MaterialManger {
public:
  std::vector<Material*> materials;

  void clean () {
    for (auto* material : materials)
      delete material;

    materials.clear();
  }
};

#endif // #ifndef _MATERIAL_MANAGER_H_