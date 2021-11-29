#include "MeshLoader.h"
#include "ShaderManager.hpp"
#include "TextureManager.hpp"
#include "Camera.h"
#include "Material.hpp"
#include "Spatial.h"
#include "auxiliar.h"
#include "OpenGLManager.h"
#include "VBOHandler.hpp"
#include "ShaderManager.hpp"
#include "SceneManager.hpp"

#include <GL/glew.h>
#include <glm/fwd.hpp>
#define SOLVE_FGLUT_WARNING
#include <GL/freeglut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

int main(int argc, char** argv) {
	std::locale::global(std::locale("es_ES.UTF-8")); // acentos ;)

	OGLManager opengl_manager;
	opengl_manager.init_context(argc, argv);
	opengl_manager.init_OGL();
	opengl_manager.init_callbacks();

	Scene sample_scene;

	sample_scene.init = [&] () {
		sample_scene.camera = new FPSCameraHandler;

		sample_scene.texture_manager.prepare();

		// LOAD TEXTURES
		if (!sample_scene.texture_manager.load_texture("img/color2.png", "colorTex")) exit(-1);
		if (!sample_scene.texture_manager.load_texture("img/emissive.png", "emiTex")) exit(-1);

		// DEFINE SHADERS PARAMETERS
		std::vector<std::string> uniforms {
			"normal",
			"modelView",
			"modelViewProj",
			"colorTex",
			"emiTex",
			"color_override"
		};

		std::vector<std::string> attributes {
			"inPos",
			"inColor",
			"inNormal",
			"inTexCoord"
		};

		// COMPILING SHADERS
		if (!sample_scene.shader_manager.load_vertex_shader("shaders_P3/shader.v0.vert", "v0")) exit(-1);
		if (!sample_scene.shader_manager.load_fragment_shader("shaders_P3/shader.v0.frag", "f0")) exit(-1);

		if (!sample_scene.shader_manager.load_vertex_shader("shaders_P3/shader.v1.vert", "v1")) exit(-1);
		if (!sample_scene.shader_manager.load_fragment_shader("shaders_P3/shader.v1.frag", "f1")) exit(-1);

		// COMPILING PROGRAMS
		if (!sample_scene.shader_manager.create_program("p0", "v0", "f0",
																			uniforms, attributes)) exit(-1);

		if (!sample_scene.shader_manager.create_program("p1", "v1", "f1",
																			uniforms, attributes)) exit(-1);

		// LOADING MESHES
		sample_scene.loader.import_from_file("meshes/bitxo_piernas.glb");
		sample_scene.loader.import_default_cube();

		sample_scene.vbo_handler.generate_VBOs();
		sample_scene.vbo_handler.populate_VBOs(sample_scene.loader.get_meshes());

		// TANKING ADVANTAGE OF LAYOUT / LOCATION
		const std::map<std::string, unsigned> attribute_name_location {
			{"inPos", 0},
			{"inColor", 1},
			{"inNormal", 2},
			{"inTexCoord", 3},
		};

		sample_scene.shader_manager.bound_program_attributes("p0", attribute_name_location);
		sample_scene.shader_manager.bound_program_attributes("p1", attribute_name_location);

		const auto& ogl_meshes = sample_scene.vbo_handler.get_meshes();

		MeshInstance* robotmesh = new MeshInstance (ogl_meshes[0]);
		MeshInstance* cubemesh2 = new MeshInstance (ogl_meshes[1]);
		MeshInstance* cubemesh3 = new MeshInstance (ogl_meshes[1]);

		Material* mat_a = new Material;
		mat_a->shader_parameters["colorTex"] = new SP_Texture(sample_scene.texture_manager.get_texture("colorTex"));
		mat_a->shader_parameters["emiTex"] = new SP_Texture(sample_scene.texture_manager.get_texture("emiTex"));
		mat_a->shader_parameters["color_override"] = new SP_Vec4f({0,0,0,0});

		robotmesh->mat = mat_a;
		cubemesh2->mat = mat_a;
		cubemesh3->mat = mat_a;

		cubemesh3->update_logic = [](Spatial& self, const float dummy_time) {
			self.rotation().x = dummy_time / 4;
			self.rotation().y = dummy_time / 4;
		};

		cubemesh2->translation().x = -2;
		cubemesh2->translation().y = 2;

		robotmesh->translation().x = 2;
		robotmesh->translation().y = -2;

		sample_scene.scene_objects.push_back(robotmesh);
		sample_scene.scene_objects.push_back(cubemesh2);
		sample_scene.scene_objects.push_back(cubemesh3);

		sample_scene.shader_manager.set_mesh_per_program("p0", cubemesh2);
		sample_scene.shader_manager.set_mesh_per_program("p1", robotmesh);
		sample_scene.shader_manager.set_mesh_per_program("p1", cubemesh3);
	};

	opengl_manager.scene_manager.add_scene("main", &sample_scene);
	opengl_manager.scene_manager.set_active_scene("main");

	opengl_manager.start_loop();

	return 0;
}