#ifndef _MATERTIAL_H_
#define _MATERTIAL_H_

#include "Texture.hpp"

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

struct AbstractShaderParameter {
  virtual void upload_data (const unsigned parameter_id) const = 0;
  virtual ~AbstractShaderParameter () {}
};

struct SP_Mat4f : public AbstractShaderParameter {
  glm::mat4 mat_4;
  SP_Mat4f (const glm::mat4 m) : mat_4 (m) {}
  void upload_data (const unsigned parameter_id) const {
    glUniformMatrix4fv(parameter_id, 1, GL_FALSE, &(mat_4[0][0]));
  }
};

struct SP_Mat3f : public AbstractShaderParameter {
  SP_Mat3f (const glm::mat3 m) : mat_3 (m) {}
  glm::mat3 mat_3;
  void upload_data (const unsigned parameter_id) const {
    glUniformMatrix3fv(parameter_id, 1, GL_FALSE, &(mat_3[0][0]));
  }
};

struct SP_Mat2f : public AbstractShaderParameter {
  SP_Mat2f (const glm::mat2 m) : mat_2 (m) {}
  glm::mat2 mat_2;
  void upload_data (const unsigned parameter_id) const {
    glUniformMatrix2fv(parameter_id, 1, GL_FALSE, &(mat_2[0][0]));
  }
};

struct SP_Vec4f : public AbstractShaderParameter {
  SP_Vec4f (const glm::vec4 v) : vec_4 (v) {}
  glm::vec4 vec_4;
  void upload_data (const unsigned parameter_id) const {
    glUniform4fv(parameter_id, 1, &(vec_4[0]));
  }
};

struct SP_Vec3f : public AbstractShaderParameter {
  SP_Vec3f (const glm::vec3 v) : vec_3 (v) {}
  glm::vec3 vec_3;
  void upload_data (const unsigned parameter_id) const {
    glUniform3fv(parameter_id, 1, &(vec_3[0]));
  }
};

struct SP_Vec2f : public AbstractShaderParameter {
  SP_Vec2f (const glm::vec2 v) : vec_2 (v) {}
  glm::vec2 vec_2;
  void upload_data (const unsigned parameter_id) const {
    glUniform2fv(parameter_id, 1, &(vec_2[0]));
  }
};

struct SP_Valuef : public AbstractShaderParameter {
  SP_Valuef (const float v) : value (v) {}
  float value;
  void upload_data (const unsigned parameter_id) const {
    glUniform1f(parameter_id, value);
  }
};

struct SP_Texture : public AbstractShaderParameter {
  const Texture* texture;

  SP_Texture (const Texture& tex) {
    texture = &tex;
  }

  void upload_data (const unsigned parameter_id) const {
    glActiveTexture(GL_TEXTURE0 + texture->n_texture);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glUniform1i(parameter_id, texture->n_texture);
  }
};

struct Material {
  std::map<std::string, AbstractShaderParameter*> shader_uniforms;

  bool get_parameter (const std::string& parameter_name, const unsigned id) const {
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

  Material () {
    shader_uniforms["modelView"] = new SP_Mat4f(glm::mat4(1));
    shader_uniforms["modelViewProj"] = new SP_Mat4f(glm::mat4(1));
    shader_uniforms["normal"] = new SP_Mat4f(glm::mat4(1));
  }

  ~Material () {
    for (auto parameter : shader_uniforms)
      delete parameter.second;

    shader_uniforms.clear();
  }
};

#endif // _MATERTIAL_H_