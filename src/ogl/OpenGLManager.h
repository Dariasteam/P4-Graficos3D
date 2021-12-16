#ifndef _OPENGL_MANAGER_H_
#define _OPENGL_MANAGER_H_

#include "../aux/auxiliar.h"
#include "../aux/statistics.hpp"
#include "../shader/Shaders.hpp"
#include "../shader/Program.hpp"
#include "../texture/Texture.hpp"
#include "../scene/SceneManager.hpp"

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
public:
  ~OGLManager() { destroy(); }

	static Statistics statistics;

  void init_context(int argc, char **argv);
  void init_OGL();
	void start_loop ();

	// Callbacks
	static void resizeFunc(int width, int height);
	static void idleFunc();
	static void keyboardFunc(unsigned char key, int x, int y);
	static void mouseFunc(int button, int state, int x, int y);
	static void mouseMotionFunc(int x, int y);
	static void renderFunc();

	void destroy();
	void init_callbacks();
};

#endif
