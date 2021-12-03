#ifndef _LGIHT_MANAGER_H_
#define _LGIHT_MANAGER_H_

#include "Light.hpp"
#include "../WorldManager.hpp"
#include "../../shader/ShaderManager.hpp"

#include <glm/fwd.hpp>
#include <vector>
#include <map>

class LightManager {
public:
  std::vector<DirectionalLight*> dir_lights;
  std::vector<PointLight*> point_lights;
  std::vector<FocalLight*> focal_lights;

  AmbientLight ambient_light;

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

  inline AmbientLight& get_ambient_light () {
    return ambient_light;
  }

  inline DirectionalLight& create_directional_light() {
    DirectionalLight* light = new DirectionalLight;

    dir_lights.push_back(light);

    WorldManager::get().add(light);
    return *light;
  }

  inline PointLight& create_point_light() {
    PointLight* light = new PointLight;

    point_lights.push_back(light);

    WorldManager::get().add(light);
    return *light;
  }

  inline FocalLight& create_focal_light() {
    FocalLight* light = new FocalLight;

    focal_lights.push_back(light);

    WorldManager::get().add(light);
    return *light;
  }

  bool bind_program_ids(const std::string& program_name,
                        const unsigned prog_type) {

    if (!ShaderManager::get().check_program_exist(program_name, prog_type)) {
      std::cout << "Error. No se pueden bindear las luces. "
                << "No existe el programa " << program_name << "\n";
      return false;
    }

    const auto* program = ShaderManager::get().get_program(program_name, prog_type);

    auto& dir_uniforms = DirectionalLight::uniform_ids;
    auto& point_uniforms = PointLight::uniform_ids;
    auto& focal_uniforms = FocalLight::uniform_ids;
    auto& ambient_uniforms = AmbientLight::uniform_ids;

    for (const auto uniform : program->uniforms) {
      const auto& name = uniform.first;
      const auto& value = uniform.second;

      if (dir_uniforms.find(name) != dir_uniforms.end())
        dir_uniforms[name] = value;

      if (point_uniforms.find(name) != point_uniforms.end())
        point_uniforms[name] = value;

      if (focal_uniforms.find(name) != focal_uniforms.end())
        focal_uniforms[name] = value;

      if (ambient_uniforms.find(name) != ambient_uniforms.end())
        ambient_uniforms[name] = value;
    }

    return true;
  };

  bool upload_ambient_light () {
    ambient_light.upload_data();
  }

  bool upload_next_light_pass (const glm::mat4& view) {
    bool end = true;

    if (i_dir < dir_lights.size()) {
      dir_lights[i_dir]->adjust_to_view (view);
      i_dir++;
      end = false;
    } else {
      dir_lights[0]->upload_black("_dirLightC");
    }

    if (i_point < point_lights.size()) {
      point_lights[i_point]->adjust_to_view (view);
      i_point++;
      end = false;
    } else {
      point_lights[0]->upload_black("_pointLightC");
    }

    if (i_focal < focal_lights.size()) {
      focal_lights[i_focal]->adjust_to_view (view);
      i_focal++;
      end = false;
    } else {
      focal_lights[0]->upload_black("_focalLightC");
    }


    if (end) {
      i_dir = 0;
      i_focal = 0;
      i_point = 0;
    }

    return end;
  }

  void clean () {
    // We do not delete the lights since they are managed by the world
    dir_lights.clear();
    point_lights.clear();
    focal_lights.clear();
  }
};

#endif // _LGIHT_MANAGER_H_