#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "../Spatial.h"
#include "../../shader/Shaders.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <map>

struct AbstractLight : public Spatial {
public:
  SP_Vec3f color;
  SP_Vec4f position;

protected:
  AbstractLight () {}
  virtual void upload_data(const glm::mat4& view) = 0;

public:
  glm::mat4 get_model_matrix () {}
  void update(float dummy_time) {}

  void adjust_to_view(const glm::mat4& view) {
    position.vec_4 = view * get_model_matrix() * glm::vec4{0, 0, 0, 1};
  }
};

struct PointLight : public AbstractLight {
  static std::map<std::string, int> uniform_ids;

  void upload_data(const glm::mat4& view) {
    auto aux_pos = position.vec_4;

    // FIXME: Use translation here
    auto model = glm::translate(glm::mat4(1), glm::vec3(position.vec_4.x, position.vec_4.y, position.vec_4.z));

    position.vec_4 = view * model * glm::vec4(0, 0, 0, 1);
    color.upload_data(uniform_ids["PointLightC"]);
    position.upload_data(uniform_ids["PointLightP"]);

    position.vec_4 = aux_pos;
  }
};

struct DirectionalLight : public AbstractLight {
  static std::map<std::string, int> uniform_ids;

  SP_Vec3f direction;

  void upload_data(const glm::mat4& view) {
    color.upload_data(uniform_ids["DirLightC"]);
    direction.upload_data(uniform_ids["DirLightD"]);
  }
};

struct FocalLight : public AbstractLight {
  static std::map<std::string, int> uniform_ids;

  SP_Vec3f direction;
  SP_Valuef aperture;

  void upload_data(const glm::mat4& view) {
    color.upload_data(uniform_ids["FocalLightC"]);
    position.upload_data(uniform_ids["FocalLightP"]);
    aperture.upload_data(uniform_ids["FocalLightA"]);
    direction.upload_data(uniform_ids["FocalLightD"]);
  }
};

#endif // _LIGHT_H_