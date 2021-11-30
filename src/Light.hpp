#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Spatial.h"
#include "Shaders.hpp"
#include <glm/fwd.hpp>

struct AbstractLight : public Spatial {
public:
  SP_Vec3f color;
  SP_Vec4f position;

protected:
  AbstractLight () = delete;

  void upload_color (const unsigned id) {
    color.upload_data(id);
  }

  glm::mat4 get_model_matrix () {
    glm::mat4 t = glm::translate(_local_transform, _translation);
    return t * _local_transform;
  }

public:
  void adjust_to_view(const glm::mat4& view) {
    position.vec_4 = view * get_model_matrix() * glm::vec4{0, 0, 0, 1};
  }
};

struct PointLight : public AbstractLight {
  SP_Valuef intensity;
};

struct DrectionalLight : public AbstractLight {
  SP_Vec3f direction;
};

struct SpotLight : public AbstractLight {
  SP_Valuef intensity;
  SP_Valuef apperture;
};

#endif // _LIGHT_H_