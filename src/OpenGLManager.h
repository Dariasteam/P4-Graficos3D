#ifndef _OPENGL_MANAGER_H_
#define _OPENGL_MANAGER_H_

#include "Spatial.h"
#include "auxiliar.h"
#include "Shaders.hpp"
#include "Program.hpp"
#include "Texture.hpp"

#include <GL/glew.h>
#include <GL/freeglut.h>

#define SOLVE_FGLUT_WARNING
#define GLM_FORCE_RADIANS

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>

class OGLManager {
private:

//  GLuint loadShader(const char *fileName, GLenum type);
public:
  void initContext(int argc, char **argv);
  void initOGL();

  // linked shaders
  //std::map<std::string, Program *> programs;
  // std::map<std::string, Texture> texture_ids;				  // Texture
  // identifiers
/*
  std::map<std::string, VertexShader *> vertex_shaders;
  std::map<std::string, FragmentShader *> fragment_shaders;
  bool load_vertex_shader(const std::string &path, const std::string &name);
  bool load_fragment_shader(const std::string &path, const std::string &name);

  bool create_program(const std::string &name,
                      const VertexShader &vertex_shader,
                      const FragmentShader &fragment_shader,
                      const std::vector<std::string> &uniforms_names,
                      const std::vector<std::string> &attributes_names);

  void set_mesh_per_program(Program &program, MeshInstance *mesh) const;
*/
  /*
  bool load_texture(const std::string& path,
                                                                          const
  std::string& name);
  */
  // Crea una textura, la configura, la sube a OpenGL,
  // y devuelve el identificador de la textura
  // unsigned int loadTex(const char *fileName);
/*
  int bound_program_attributes(
      Program &program,
      const std::map<std::string, unsigned> &attribute_name_location);
*/
  ~OGLManager() { destroy(); }

  OGLManager() {}

  void init_context();
  void init_OGL();

  void destroy();
};

#endif
