#include "Scene.hpp"

Scene* Scene::generate_default() {
  Scene& default_scene = *new Scene;

	default_scene.init = [&] () {
		default_scene.camera = new FPSCameraHandler;

		default_scene.texture_manager.prepare();

		// LOAD TEXTURES
		if (!default_scene.texture_manager.load_texture("img/color2.png", "colorTex")) exit(-1);
		if (!default_scene.texture_manager.load_texture("img/emissive.png", "emiTex")) exit(-1);

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
		if (!default_scene.shader_manager.load_vertex_shader("shaders_P3/shader.v0.vert", "v0")) exit(-1);
		if (!default_scene.shader_manager.load_fragment_shader("shaders_P3/shader.v0.frag", "f0")) exit(-1);

		if (!default_scene.shader_manager.load_vertex_shader("shaders_P3/shader.v1.vert", "v1")) exit(-1);
		if (!default_scene.shader_manager.load_fragment_shader("shaders_P3/shader.v1.frag", "f1")) exit(-1);

		// COMPILING PROGRAMS
		if (!default_scene.shader_manager.create_program("p0", "v0", "f0",
																			uniforms, attributes)) exit(-1);

		if (!default_scene.shader_manager.create_program("p1", "v1", "f1",
																			uniforms, attributes)) exit(-1);

		// LOADING MESHES
		default_scene.loader.import_from_file("meshes/bitxo_piernas.glb");
		default_scene.loader.import_default_cube();

		default_scene.vbo_handler.generate_VBOs();
		default_scene.vbo_handler.populate_VBOs(default_scene.loader.get_meshes());

		// TANKING ADVANTAGE OF LAYOUT / LOCATION
		const std::map<std::string, unsigned> attribute_name_location {
			{"inPos", 0},
			{"inColor", 1},
			{"inNormal", 2},
			{"inTexCoord", 3},
		};

		default_scene.shader_manager.bound_program_attributes("p0", attribute_name_location);
		default_scene.shader_manager.bound_program_attributes("p1", attribute_name_location);

		// GENERATE ISNTANCES OF THE MESHES ALREADY LOADED IN THE VBO
		const auto& ogl_meshes = default_scene.vbo_handler.get_meshes();

		MeshInstance* robotmesh = new MeshInstance (ogl_meshes[0]);
		MeshInstance* cubemesh2 = new MeshInstance (ogl_meshes[1]);
		MeshInstance* cubemesh3 = new MeshInstance (ogl_meshes[1]);

		// GENERATE MATERIAL (INPUTS FOR SHADERS)
		Material* mat_a = new Material;
		mat_a->shader_parameters["colorTex"] = new SP_Texture(default_scene.texture_manager.get_texture("colorTex"));
		mat_a->shader_parameters["emiTex"] = new SP_Texture(default_scene.texture_manager.get_texture("emiTex"));
		mat_a->shader_parameters["color_override"] = new SP_Vec4f({0,0,0,0});

		// ASSIGN MAERIALS TO MESH INSTANCES
		robotmesh->mat = mat_a;
		cubemesh2->mat = mat_a;
		cubemesh3->mat = mat_a;

		// CREATE BEHAVIOUR LOGIC FOR MESH INSTANCES
		cubemesh3->update_logic = [](Spatial& self, const float dummy_time) {
			self.rotation().x = dummy_time / 4;
			self.rotation().y = dummy_time / 4;
		};

		cubemesh2->translation().x = -2;
		cubemesh2->translation().y = 2;

		robotmesh->translation().x = 2;
		robotmesh->translation().y = -2;

		// ADD MESH INSTANCES TO SCENES
		default_scene.scene_objects.push_back(robotmesh);
		default_scene.scene_objects.push_back(cubemesh2);
		default_scene.scene_objects.push_back(cubemesh3);

		// ASSIGN SHADER TO MESHES
		default_scene.shader_manager.set_mesh_per_program("p0", cubemesh2);
		default_scene.shader_manager.set_mesh_per_program("p1", robotmesh);
		default_scene.shader_manager.set_mesh_per_program("p1", cubemesh3);
	};

	return &default_scene;
}