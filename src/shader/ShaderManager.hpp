#ifndef _SHADER_H_
#define _SHADER_H_

#include "Shaders.hpp"
#include "Program.hpp"
#include "../aux/auxiliar.h"

#include <map>
#include <string>
#include <vector>


class ShaderManager {
private:
  GLuint loadShader(const char *fileName, GLenum type);
  ShaderManager () {}
public:

  enum {
    P_PROJECTION,
    P_SHADING,
    P_LIGHTING,
    P_POST_PROCESSING,
  };


  inline static ShaderManager& get () {
    static ShaderManager instance;
    return instance;
  }

  ShaderManager (const ShaderManager&) = delete;
  void operator= (const ShaderManager&) = delete;

  std::map<std::string, VertexShader *> vertex_shaders;
  std::map<std::string, FragmentShader *> fragment_shaders;

  std::map<std::string, Program *> programs_projection;
  std::map<std::string, Program *> programs_shading;
  std::map<std::string, Program *> programs_lightning;
  std::map<std::string, Program *> programs_post_processing;

  std::map<std::string, Program *>& get_container (const unsigned program_type) {
    switch (program_type) {
      case P_PROJECTION:
        return programs_projection;
      break;

      case P_SHADING:
        return programs_shading;
      break;

      case P_LIGHTING:
        return programs_lightning;
      break;

      case P_POST_PROCESSING:
        return programs_post_processing;
      break;
    }
  }

  std::map<std::string, int> get_uniforms_in_program (const std::string& program_name) {
    std::map<std::string, int> uniforms;

    if (check_program_exist(program_name)) {
      auto* program = get_program(program_name);
      int count;
      glGetProgramiv(program->id, GL_ACTIVE_UNIFORMS, &count);

      const GLsizei bufSize = 16; // maximum name length
      GLchar name[bufSize]; // variable name in GLSL
      GLsizei length; // name length
      GLint size; // size of the variable
      GLenum type; // type of the variable (float, vec3 or mat4, etc)

      for (unsigned i = 0; i < count; i++) {
        glGetActiveUniform(program->id, (GLuint)i, bufSize, &length, &size, &type, name);
        uniforms[name] = type;
      }
    } else {
      std::cout << "Error obteniendo la lista de parámetros uniform. "
                << "El programa " << program_name << " no existe" << std::endl;
    }

    return uniforms;
  }

  inline bool check_program_exist (const std::string& program) {
    return (programs_shading.find(program) != programs_shading.end());
  }

  inline bool check_v_shader_exist (const std::string& v_shader) {
    return (vertex_shaders.find(v_shader) != vertex_shaders.end());
  }

  inline bool check_f_shader_exist (const std::string& f_shader) {
    return (fragment_shaders.find(f_shader) != fragment_shaders.end());
  }

  inline VertexShader* get_v_shader (const std::string& v_shader) {
    return vertex_shaders[v_shader];
  }

  inline FragmentShader* get_f_shader (const std::string& f_shader) {
    return fragment_shaders[f_shader];
  }

  inline Program* get_program (const std::string& program) {
    return programs_shading[program];
  }

  void clean();

  bool load_vertex_shader(const std::string &path, const std::string &name);
  bool load_fragment_shader(const std::string &path, const std::string &name);

  bool create_program(const std::string &name,
                      const std::string &v_name,
                      const std::string &f_name,
                      int type = P_SHADING);

  bool set_mesh_per_program(const std::string& program_name, MeshInstance& mesh,
                            const unsigned program_type = P_SHADING);

  bool bound_program_attributes(const std::string& program_name,
                const std::map<std::string, unsigned> &attribute_name_location,
                const unsigned program_type = P_SHADING);

  void detach_programs_in_container(const unsigned program_type);
  void delete_programs_in_container(const unsigned program_type);
};



#endif // _SHADER_H_