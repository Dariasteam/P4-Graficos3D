#include "ogl/OpenGLManager.h"

#include <iostream>
#include <vector>

#include "scene/Scene.hpp"


#include "demos/demo_3.hpp"

int main(int argc, char** argv) {
	// No es necesario en Linux ;D
	std::locale::global(std::locale("es_ES.UTF-8")); // acentos ;)

	Scene scene_3;

	scene_3.init = demo_3::init;
	scene_3.on_idle = demo_3::on_idle;
	scene_3.on_keyboard = demo_3::on_keyboard;
	scene_3.on_mouse_button = demo_3::on_mouse_button;
	scene_3.on_mouse_motion = demo_3::on_mouse_motion;
	scene_3.on_resize = demo_3::on_resize;
	scene_3.render = demo_3::render;

	SceneManager::get().add_scene("scene_3", &scene_3);

	SceneManager::get().set_init_scene("scene_3");

	OGLManager opengl_manager;
	opengl_manager.init_context(argc, argv);
	opengl_manager.init_OGL();
	opengl_manager.init_callbacks();

	SceneManager::get().init();
	opengl_manager.start_loop();

	return 0;
}