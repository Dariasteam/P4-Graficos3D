#include "ogl/OpenGLManager.h"

#include <iostream>
#include <vector>

#include "scene/Scene.hpp"


//#include "demos/demo_3.hpp"
#include "demos/demo_4.hpp"

int main(int argc, char** argv) {
	// No es necesario en Linux ;D
	std::locale::global(std::locale("es_ES.UTF-8")); // acentos ;)

	Scene scene_4 (scene_data_4);

	SceneManager::get().add_scene("scene_4", &scene_4);
	SceneManager::get().set_init_scene("scene_4");

	OGLManager opengl_manager;

	opengl_manager.init_context(argc, argv);
	opengl_manager.init_OGL();
	opengl_manager.init_callbacks();

	SceneManager::get().init();
	opengl_manager.start_loop();

	return 0;
}