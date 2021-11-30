#ifndef _MATERTIAL_H_
#define _MATERTIAL_H_

#include "../texture/Texture.hpp"
#include "../spatial/light/Light.hpp"
#include "../shader/Shaders.hpp"

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

public:
  Material () {
    shader_uniforms["modelView"] = new SP_Mat4f(glm::mat4(1));
    shader_uniforms["modelViewProj"] = new SP_Mat4f(glm::mat4(1));
    shader_uniforms["normal"] = new SP_Mat4f(glm::mat4(1));
  }


  // FIXME: We could have this pre binded
  bool upload_uniform (const std::string& parameter_name, const unsigned id) const {
    const auto& it = shader_uniforms.find(parameter_name);
    if (it != shader_uniforms.end()) {
      it->second->upload_data(id);
      return true;
    } else {
      return false;
    }
  }

  void calculate_matrices (const glm::mat4& model,
                           const glm::mat4& view,
                           const glm::mat4& proj) {

    glm::mat4& modelView = static_cast<SP_Mat4f*>(shader_uniforms["modelView"])->mat_4;
    glm::mat4& modelViewProj = static_cast<SP_Mat4f*>(shader_uniforms["modelViewProj"])->mat_4;
    glm::mat4& normal = static_cast<SP_Mat4f*>(shader_uniforms["normal"])->mat_4;


    modelView = view * model;
    modelViewProj = proj * modelView;
    normal = glm::transpose(glm::inverse(modelView));
  }

  ~Material () {
    for (auto parameter : shader_uniforms)
      delete parameter.second;

    shader_uniforms.clear();
  }
};

#endif // _MATERTIAL_H_