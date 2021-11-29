#include "OpenGLManager.h"

#include <iostream>
#include <vector>

int main(int argc, char** argv) {
	// No es necesario en Linux ;D
	std::locale::global(std::locale("es_ES.UTF-8")); // acentos ;)

	OGLManager opengl_manager;
	opengl_manager.init_context(argc, argv);
	opengl_manager.init_OGL();
	opengl_manager.init_callbacks();

	Scene* default_scene = Scene::generate_default();

	opengl_manager.scene_manager.add_scene("main", default_scene);
	opengl_manager.scene_manager.set_active_scene("main");

	opengl_manager.start_loop();

	return 0;
}