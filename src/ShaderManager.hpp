#ifndef _SHADER_H_
#define _SHADER_H_

#include "Shaders.hpp"
#include "Program.hpp"
#include "auxiliar.h"

#include <map>
#include <string>
#include <vector>

class ShaderManager {
private:
  GLuint loadShader(const char *fileName, GLenum type);
public:
  std::map<std::string, VertexShader *> vertex_shaders;
  std::map<std::string, FragmentShader *> fragment_shaders;
  std::map<std::string, Program *> programs;

  void clean();

  bool load_vertex_shader(const std::string &path, const std::string &name);
  bool load_fragment_shader(const std::string &path, const std::string &name);

  bool create_program(const std::string &name,
                      const std::string &v_name,
                      const std::string &f_name,
                      const std::vector<std::string> &uniforms_names);

  bool set_mesh_per_program(const std::string& program_name, MeshInstance *mesh) const;

  bool bound_program_attributes(const std::string& program_name,
                const std::map<std::string, unsigned> &attribute_name_location);
};



#endif // _SHADER_H_