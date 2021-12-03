#ifndef _MATERTIAL_H_
#define _MATERTIAL_H_

#include "../texture/Texture.hpp"
#include "../spatial/light/Light.hpp"
#include "../shader/Shaders.hpp"
#include "../spatial/mesh/MeshInstance.hpp"

#include <map>
#include <string>

#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <vector>
#define SOLVE_FGLUT_WARNING
#include <GL/freeglut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


struct Material {
public:
  std::map<std::string, AbstractShaderParameter*> shader_uniforms;

  std::vector<MeshInstance*> associated_mesh_instances;

public:

  void associate_mesh_instance(MeshInstance* mesh) {
    associated_mesh_instances.push_back(mesh);
  }

  bool bind (const std::string uniform_name, const unsigned uniform_id) {
    const auto& it = shader_uniforms.find(uniform_name);
    if (it != shader_uniforms.end()) {
        it->second->uniform_id = uniform_id;
      return true;
    } else {
      return false;
    }
  }

  void upload_uniforms () const {
    for (const auto& parameter : shader_uniforms) {
      parameter.second->upload_data();
    }
  }

  ~Material () {
    for (auto parameter : shader_uniforms)
      delete parameter.second;

    shader_uniforms.clear();
  }
};

#endif // _MATERTIAL_H_