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

  // FIXME: This should be a vector 3
  SP_Vec4f position;

protected:
  AbstractLight () {}
  virtual void upload_data() = 0;

public:
  glm::mat4 get_model_matrix () {
    auto t = glm::translate(_local_transform.mat_4, _translation);
    return t;
  }
  void update(float dummy_time) {}

  virtual void adjust_to_view(const glm::mat4& view) = 0;
};

struct PointLight : public AbstractLight {
  static std::map<std::string, int> uniform_ids;

  void upload_data() {
    color.upload_data(uniform_ids["PointLightC"]);
    position.upload_data(uniform_ids["PointLightP"]);
  }

  void adjust_to_view(const glm::mat4& view) {
    position.vec_4 = view * get_model_matrix() * glm::vec4{0, 0, 0, 1};
  }
};

struct DirectionalLight : public AbstractLight {
  static std::map<std::string, int> uniform_ids;

  SP_Vec3f direction;

  void upload_data() {
    color.upload_data(uniform_ids["DirLightC"]);
    direction.upload_data(uniform_ids["DirLightD"]);
  }

  void adjust_to_view(const glm::mat4& view) {
    position.vec_4 = view * get_model_matrix() * glm::vec4{0, 0, 0, 1};
    direction.vec_3 = view * get_model_matrix() * glm::vec4{0, 0, 0, 1};
  }
};

struct FocalLight : public AbstractLight {
  static std::map<std::string, int> uniform_ids;

  SP_Vec3f direction;
  SP_Valuef aperture;

  void upload_data() {
    color.upload_data(uniform_ids["FocalLightC"]);
    position.upload_data(uniform_ids["FocalLightP"]);
    aperture.upload_data(uniform_ids["FocalLightA"]);
    direction.upload_data(uniform_ids["FocalLightD"]);
  }

  void adjust_to_view(const glm::mat4& view) {
    position.vec_4 = view * get_model_matrix() * glm::vec4{0, 0, 0, 1};
    direction.vec_3 = view * get_model_matrix() * glm::vec4{0, 0, -1, 0};
  }
};

#endif // _LIGHT_H_