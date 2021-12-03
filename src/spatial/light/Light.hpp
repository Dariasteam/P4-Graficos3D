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
  SP_Vec3f black;
protected:
  AbstractLight () {
    black.vec_3 = {0, 0, 0};
  }
  virtual void upload_data() = 0;

public:
  glm::mat4 get_model_matrix () {
    auto t = glm::translate(_local_transform.mat_4, _translation);
    return t;
  }
  void update(float dummy_time) {}

  virtual void upload_black() = 0;

  virtual void adjust_to_view(const glm::mat4& view) = 0;
};

struct AmbientLight : public AbstractLight {
  void adjust_to_view(const glm::mat4& view) {
    upload_data();
  }

  void upload_data() {
    color.upload_data();
  }
  void upload_black() {
  }
};

struct PointLight : public AbstractLight {
  static std::map<std::string, int> uniform_ids;
  SP_Vec4f position;

  void upload_data() {
    color.upload_data();
    position.upload_data();
  }

  void adjust_to_view(const glm::mat4& view) {
    position.vec_4 = view * get_model_matrix() * glm::vec4{0, 0, 0, 1};
    upload_data();
  }
  void upload_black() {
    black.upload_data();
  }
};

struct DirectionalLight : public AbstractLight {
  SP_Vec3f direction;

  void upload_data() {
    color.upload_data();
    direction.upload_data();
  }

  void adjust_to_view(const glm::mat4& view) {

    const auto v = direction.vec_3;
    direction.vec_3 = view * get_model_matrix() * glm::vec4{v.x, v.y, v.z, 0};
    upload_data();
    direction.vec_3 = v;
  }
  void upload_black() {
    black.upload_data();
  }
};

struct FocalLight : public AbstractLight {
  static std::map<std::string, int> uniform_ids;
  SP_Vec4f position;
  SP_Vec3f direction;
  SP_Valuef aperture;

  void upload_data() {
    color.upload_data();
    position.upload_data();
    aperture.upload_data();
    direction.upload_data();
  }
  void upload_black() {
    black.upload_data();
  }

  // FIXME: This is a mess, we shouldn't be modifying and restoring light properties
  void adjust_to_view(const glm::mat4& view) {
    position.vec_4 = view * get_model_matrix() * glm::vec4{0, 0, 0, 1};

    const auto v = direction.vec_3;
    direction.vec_3 = view * get_model_matrix() * glm::vec4{v.x, v.y, v.z, 0};
    upload_data();
    direction.vec_3 = v;
  }
};

#endif // _LIGHT_H_