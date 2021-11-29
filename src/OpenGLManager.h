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

public:
  void initContext(int argc, char **argv);
  void initOGL();

  ~OGLManager() { destroy(); }

  OGLManager() {}

  void init_context();
  void init_OGL();

  void destroy();
};

#endif
