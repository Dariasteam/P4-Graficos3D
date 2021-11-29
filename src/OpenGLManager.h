#ifndef _OPENGL_MANAGER_H_
#define _OPENGL_MANAGER_H_

#include "Spatial.h"
#include "auxiliar.h"
#include "Shaders.hpp"
#include "Program.hpp"
#include "Texture.hpp"

#include "SceneManager.hpp"

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
	static unsigned w;
	static unsigned h;
	static float dummy_time;

public:
	static SceneManager scene_manager;

  ~OGLManager() { destroy(); }

  void init_context(int argc, char **argv);
  void init_OGL();
	void start_loop ();

	static void resizeFunc(int width, int height) {
		scene_manager.get_current_scene ()->on_resize(width, height);
	}

	static void idleFunc() {
		scene_manager.get_current_scene ()->on_idle();
	}

	static void keyboardFunc(unsigned char key, int x, int y) {
		scene_manager.get_current_scene()->on_keyboard(key);
	}

	static void mouseFunc(int button, int state, int x, int y) {
		scene_manager.get_current_scene()->on_mouse_button(button, state, x, y);
	}

	static void mouseMotionFunc(int x, int y) {
		scene_manager.get_current_scene()->on_mouse_motion(x, y);
	}

  void destroy();

	static void renderFunc() {
		scene_manager.get_current_scene()->render();
	}

	void init_callbacks() {
		glutReshapeFunc(resizeFunc);
		glutDisplayFunc(renderFunc);
		glutIdleFunc(idleFunc);
		glutKeyboardFunc(keyboardFunc);
		glutMotionFunc(mouseMotionFunc);
	}
};

#endif
