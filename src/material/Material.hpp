#ifndef _MATERTIAL_H_
#define _MATERTIAL_H_

#include "../spatial/camera/Camera.h"
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
  std::map<std::string, AbstractShaderParameter*> shader_mat_uniforms;
  std::map<std::string, AbstractShaderParameter*> shader_geometry_uniforms;

  std::vector<MeshInstance*> associated_mesh_instances;
public:
  SP_Mat4f view;
  SP_Mat4f model;
  SP_Mat4f modelView;
  SP_Mat4f modelViewProj;
  SP_Mat4f normal;

  Material () {
    shader_geometry_uniforms["_view"] = &view;
    shader_geometry_uniforms["_model"] = &model;
    shader_geometry_uniforms["_modelView"] = &modelView;
    shader_geometry_uniforms["_modelViewProj"] = &modelViewProj;
    shader_geometry_uniforms["_normal"] = &normal;
  }

  void associate_mesh_instance(MeshInstance* mesh) {
    associated_mesh_instances.push_back(mesh);
  }

  bool bind (const std::string uniform_name, const unsigned uniform_id) {
    const auto& it = shader_mat_uniforms.find(uniform_name);
    const auto& it2 = shader_geometry_uniforms.find(uniform_name);

    if (it != shader_mat_uniforms.end())
      it->second->uniform_id = uniform_id;
    else if (it2 != shader_geometry_uniforms.end())
      it2->second->uniform_id = uniform_id;
    else
      return false;
    return true;
  }

  void upload_mat_uniforms () const {
    for (const auto& parameter : shader_mat_uniforms) {
      parameter.second->upload_data();
    }
  }

  void upload_geometry_uniforms () const {
    for (const auto& parameter : shader_geometry_uniforms) {
      parameter.second->upload_data();
    }
  }

  void set_geometry_uniforms(MeshInstance mesh, Camera* camera) {
    const auto& view_2 = camera->get_view_matrix();
    const auto& proj_2 = camera->get_projection_matrix();

    model.mat_4 = mesh.get_model_matrix();
		modelView.mat_4 = view_2 * model.mat_4;
    modelViewProj.mat_4 = proj_2 * modelView.mat_4;
    normal.mat_4 = glm::transpose(glm::inverse(modelView.mat_4));
  }

  ~Material () {
    for (auto parameter : shader_mat_uniforms)
      delete parameter.second;

    shader_mat_uniforms.clear();
  }
};

#endif // _MATERTIAL_H_