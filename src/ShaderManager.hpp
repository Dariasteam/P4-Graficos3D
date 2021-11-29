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

  bool load_vertex_shader(const std::string &path, const std::string &name);
  bool load_fragment_shader(const std::string &path, const std::string &name);

  bool create_program(const std::string &name,
                      const VertexShader &vertex_shader,
                      const FragmentShader &fragment_shader,
                      const std::vector<std::string> &uniforms_names,
                      const std::vector<std::string> &attributes_names);

  void set_mesh_per_program(Program &program, MeshInstance *mesh) const;

  int bound_program_attributes(Program &program,
                const std::map<std::string, unsigned> &attribute_name_location);
};



#endif // _SHADER_H_