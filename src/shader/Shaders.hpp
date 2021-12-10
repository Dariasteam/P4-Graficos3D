#ifndef _SHADERS_H_
#define _SHADERS_H_

#include "../ogl/OGLObject.hpp"
#include "../texture/Texture.hpp"

struct FragmentShader : public OglObject {
	~FragmentShader() {
		glDeleteShader(id);
	}
};

struct VertexShader : public OglObject {
	~VertexShader() {
		glDeleteShader(id);
	}
};

// FIXME: Change name to AbstractShaderUniform
struct AbstractShaderParameter {
	int uniform_id {-1};

  virtual void upload_data () const = 0;
  virtual ~AbstractShaderParameter () {}
};

struct SP_Mat4f : public AbstractShaderParameter {
  glm::mat4 mat_4;
	SP_Mat4f() {}
  SP_Mat4f (const glm::mat4 m) : mat_4 (m) {}
  void upload_data () const {
    if (uniform_id != -1)
      glUniformMatrix4fv(uniform_id, 1, GL_FALSE, &(mat_4[0][0]));
  }
};

struct SP_Mat3f : public AbstractShaderParameter {
	SP_Mat3f() {}
  SP_Mat3f (const glm::mat3 m) : mat_3 (m) {}
  glm::mat3 mat_3;
  void upload_data () const {
    if (uniform_id != -1)
      glUniformMatrix3fv(uniform_id, 1, GL_FALSE, &(mat_3[0][0]));
  }
};

struct SP_Mat2f : public AbstractShaderParameter {
	SP_Mat2f() {}
  SP_Mat2f (const glm::mat2 m) : mat_2 (m) {}
  glm::mat2 mat_2;
  void upload_data () const {
    if (uniform_id != -1)
      glUniformMatrix2fv(uniform_id, 1, GL_FALSE, &(mat_2[0][0]));
  }
};

struct SP_Vec4f : public AbstractShaderParameter {
	SP_Vec4f() {}
  SP_Vec4f (const glm::vec4 v) : vec_4 (v) {}
  glm::vec4 vec_4;
  void upload_data () const {
    if (uniform_id != -1)
      glUniform4fv(uniform_id, 1, &(vec_4[0]));
  }
};

struct SP_Vec3f : public AbstractShaderParameter {
	SP_Vec3f() {}
  SP_Vec3f (const glm::vec3 v) : vec_3 (v) {}
  glm::vec3 vec_3;
  void upload_data () const {
    glUniform3fv(uniform_id, 1, &(vec_3[0]));
  }
};

struct SP_Vec2f : public AbstractShaderParameter {
	SP_Vec2f() {}
  SP_Vec2f (const glm::vec2 v) : vec_2 (v) {}
  glm::vec2 vec_2;
  void upload_data () const {
    if (uniform_id != -1)
      glUniform2fv(uniform_id, 1, &(vec_2[0]));
  }
};

struct SP_Valuef : public AbstractShaderParameter {
	SP_Valuef () {}
  SP_Valuef (const float v) : value (v) {}
  float value;
  void upload_data () const {
    if (uniform_id != -1)
      glUniform1f(uniform_id, value);
  }
};

struct SP_Texture : public AbstractShaderParameter {
  const Texture* texture;

  SP_Texture (const Texture& tex) {
    texture = &tex;
  }

  void upload_data () const {
    glActiveTexture(GL_TEXTURE0 + texture->n_texture);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glUniform1i(uniform_id, texture->n_texture);
  }
};


#endif //_SHADERS_H_