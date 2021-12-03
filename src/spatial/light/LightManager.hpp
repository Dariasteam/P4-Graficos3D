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
  std::vector<PointLight*> point_lights;
  std::vector<FocalLight*> focal_lights;

  AmbientLight ambient_light;
  DirectionalLight dir_light;

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

  inline DirectionalLight& get_directional_light () {
    return dir_light;
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

  bool bind_program_ids(const std::string& program_name) {

    const auto* program = ShaderManager::get().get_program(program_name);

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

  bool upload_single_lights (const glm::mat4& view) {
    ambient_light.upload_data();
    dir_light.adjust_to_view(view);
  }

  bool upload_next_light_pass (const glm::mat4& view) {
    bool new_unpainted_lights = false;

    point_lights[0]->upload_black();
    focal_lights[0]->upload_black();

    if (i_point < point_lights.size()) {
      point_lights[i_point]->adjust_to_view (view);
      i_point++;
      new_unpainted_lights = true;
    }

    if (i_focal < focal_lights.size()) {
      focal_lights[i_focal]->adjust_to_view (view);
      i_focal++;
      new_unpainted_lights = true;
    }

    if (!new_unpainted_lights) {
      i_focal = 0;
      i_point = 0;
    }

    return new_unpainted_lights;
  }

  void clean () {
    // We do not delete the lights since they are managed by the world
    point_lights.clear();
    focal_lights.clear();
  }
};

#endif // _LGIHT_MANAGER_H_