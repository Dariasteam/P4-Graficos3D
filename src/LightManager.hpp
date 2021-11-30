#ifndef _LGIHT_MANAGER_H_
#define _LGIHT_MANAGER_H_

#include "Light.hpp"
#include "Program.hpp"
#include "ShaderManager.hpp"

#include <vector>
#include <map>

class LightManager {
public:
  std::vector<DirectionalLight*> dir_lights;
  std::vector<PointLight*> point_lights;
  std::vector<FocalLight*> focal_lights;

  unsigned i_dir = 0;
  unsigned i_point = 0;
  unsigned i_focal = 0;

private:
  LightManager () {}
public:
  LightManager (const LightManager&) = delete;
  void operator= (const LightManager&) = delete;

  inline static LightManager& get () {
    static LightManager instance;
    return instance;
  }

  bool bind_program_ids(const std::string& program_name) {
    if (!ShaderManager::get().check_program_exist(program_name)) {
      std::cout << "Error. No se pueden bindear las luces. "
                << "No existe el programa " << program_name << "\n";
      return false;
    }

    const auto* program = ShaderManager::get().get_program(program_name);

    auto& dir_uniforms = DirectionalLight::uniform_ids;
    auto& point_uniforms = PointLight::uniform_ids;
    auto& focal_uniforms = FocalLight::uniform_ids;

    for (const auto uniform : program->uniforms) {
      const auto& name = uniform.first;
      const auto& value = uniform.second;

      if (dir_uniforms.find(name) != dir_uniforms.end())
        dir_uniforms[name] = value;

      if (point_uniforms.find(name) != point_uniforms.end())
        point_uniforms[name] = value;

      if (focal_uniforms.find(name) != focal_uniforms.end())
        focal_uniforms[name] = value;
    }

    return true;
  };

  bool calculate_next_light_pass () {
    bool end = true;
    std::cout << "LLEGUE " <<  dir_lights.size() << std::endl;

    if (i_dir < dir_lights.size()) {
      dir_lights[i_dir]->upload_data();
      i_dir++;
      end = false;
    }

    if (i_point < point_lights.size()) {
      point_lights[i_point]->upload_data();
      i_point++;
      end = false;
    }

    if (i_focal < focal_lights.size()) {
      point_lights[i_focal]->upload_data();
      i_focal++;
      end = false;
    }

    if (end) {
      i_dir = 0;
      i_focal = 0;
      i_point = 0;
    }

    return end;
  }

  void clean () {
    for (auto* light : dir_lights)
      delete light;

    for (auto* light : point_lights)
      delete light;

    for (auto* light : focal_lights)
      delete light;

    dir_lights.clear();
    point_lights.clear();
    focal_lights.clear();
  }
};

#endif // _LGIHT_MANAGER_H_