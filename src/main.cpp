#include "ogl/OpenGLManager.h"

#include <iostream>
#include <vector>

#include "scene/Scene.hpp"

#include "demos/demo_1.hpp"
#include "demos/demo_2.hpp"

int main(int argc, char** argv) {
	// No es necesario en Linux ;D
	std::locale::global(std::locale("es_ES.UTF-8")); // acentos ;)

	Scene scene_1;
	Scene scene_2;

	scene_1.init = demo_1::init;
	scene_1.on_idle = demo_1::on_idle;
	scene_1.on_keyboard = demo_1::on_keyboard;
	scene_1.on_mouse_button = demo_1::on_mouse_button;
	scene_1.on_mouse_motion = demo_1::on_mouse_motion;
	scene_1.on_resize = demo_1::on_resize;
	scene_1.render = demo_1::render;

	scene_2.init = demo_2::init;
	scene_2.on_idle = demo_2::on_idle;
	scene_2.on_keyboard = demo_2::on_keyboard;
	scene_2.on_mouse_button = demo_2::on_mouse_button;
	scene_2.on_mouse_motion = demo_2::on_mouse_motion;
	scene_2.on_resize = demo_2::on_resize;
	scene_2.render = demo_2::render;

	SceneManager::get().add_scene("scene_1", &scene_1);
	SceneManager::get().add_scene("scene_2", &scene_2);

	SceneManager::get().set_init_scene("scene_1");

	OGLManager opengl_manager;
	opengl_manager.init_context(argc, argv);
	opengl_manager.init_OGL();
	opengl_manager.init_callbacks();

	SceneManager::get().init();
	opengl_manager.start_loop();

	return 0;
}