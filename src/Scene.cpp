#include "Scene.hpp"

ShaderManager Scene::shader_manager;
TextureManager Scene::texture_manager;
VBOHandler Scene::vbo_handler;
MeshLoader Scene::loader;

Scene* Scene::generate_default() {
  Scene& default_scene = *new Scene;

  // INIT
	default_scene.init = [&] () {
		default_scene.camera = new FPSCameraHandler;

		texture_manager.prepare();

		// LOAD TEXTURES
		if (!texture_manager.load_texture("img/color2.png", "colorTex")) exit(-1);
		if (!texture_manager.load_texture("img/emissive.png", "emiTex")) exit(-1);

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
		if (!shader_manager.load_vertex_shader("shaders_P3/shader.v0.vert", "v0")) exit(-1);
		if (!shader_manager.load_fragment_shader("shaders_P3/shader.v0.frag", "f0")) exit(-1);

		if (!shader_manager.load_vertex_shader("shaders_P3/shader.v1.vert", "v1")) exit(-1);
		if (!shader_manager.load_fragment_shader("shaders_P3/shader.v1.frag", "f1")) exit(-1);

		// COMPILING PROGRAMS
		if (!shader_manager.create_program("p0", "v0", "f0",
																			uniforms, attributes)) exit(-1);

		if (!shader_manager.create_program("p1", "v1", "f1",
																			uniforms, attributes)) exit(-1);

		// LOADING MESHES
		loader.import_from_file("meshes/bitxo_piernas.glb");
		loader.import_default_cube();

		vbo_handler.generate_VBOs();
		vbo_handler.populate_VBOs(loader.get_meshes());

		// TANKING ADVANTAGE OF LAYOUT / LOCATION
		const std::map<std::string, unsigned> attribute_name_location {
			{"inPos", 0},
			{"inColor", 1},
			{"inNormal", 2},
			{"inTexCoord", 3},
		};

		shader_manager.bound_program_attributes("p0", attribute_name_location);
		shader_manager.bound_program_attributes("p1", attribute_name_location);

		// GENERATE ISNTANCES OF THE MESHES ALREADY LOADED IN THE VBO
		const auto& ogl_meshes = vbo_handler.get_meshes();

		MeshInstance* robotmesh = new MeshInstance (ogl_meshes[0]);
		MeshInstance* cubemesh2 = new MeshInstance (ogl_meshes[1]);
		MeshInstance* cubemesh3 = new MeshInstance (ogl_meshes[1]);

		// GENERATE MATERIAL (INPUTS FOR SHADERS)
		Material* mat_a = new Material;
		mat_a->shader_parameters["colorTex"] = new SP_Texture(texture_manager.get_texture("colorTex"));
		mat_a->shader_parameters["emiTex"] = new SP_Texture(texture_manager.get_texture("emiTex"));
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
		shader_manager.set_mesh_per_program("p0", cubemesh2);
		shader_manager.set_mesh_per_program("p1", robotmesh);
		shader_manager.set_mesh_per_program("p1", cubemesh3);
	};


  // ON_RESIZE
  default_scene.on_resize = [&](int W, int H) {
		default_scene.camera->update_aspect_ratio(W, H);
    glViewport(0, 0, W, H);
		glutPostRedisplay();
  };


  // ON KEYBOARD
  default_scene.on_keyboard = [&] (unsigned char key) {
    default_scene.camera->handle_keys(key);
  };


  // ON MOUSE BUTTON
  default_scene.on_mouse_button = [&](int button, int state, int x, int y) {
    default_scene.camera->handle_mouse_buttons(button, state, x, y);
  };


  // ON MOUSE MOTION
  default_scene.on_mouse_motion = [&](int x, int y) {
    default_scene.camera->handle_mouse_motion(x, y);
  };


  // ON IDLE
  default_scene.on_idle = [&] () {
		for (auto object : default_scene.scene_objects) {
			object->update(default_scene.dummy_time);
		}
		default_scene.dummy_time += .1;
		glutPostRedisplay();
  };


  // ON RENDER
  default_scene.render = [&]() {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		const auto& view = default_scene.camera->get_view_matrix();
		const auto& proj = default_scene.camera->get_projection_matrix();

		for (auto p : shader_manager.programs) {
			Program& program = *p.second;

			glUseProgram(program.id);
			glBindVertexArray(vbo_handler.get_vao());

			for (MeshInstance* mesh_instance : program.associated_meshes) {
				const auto model = mesh_instance->get_model_matrix();
				const OglMesh* ogl_mesh = mesh_instance->mesh;
				Material* material = mesh_instance->mat;

				material->calculate_matrices(model,view, proj);

				// Upload Uniforms
				for (const auto& uniform : program.uniforms) {
					const std::string& name = uniform.first;
					const int parameter_id = uniform.second;
					mesh_instance->mat->get_parameter(name, parameter_id);
				}

				// Upload Attributes (Layout / Location)
				glBindBuffer(GL_ARRAY_BUFFER, vbo_handler.posVBO);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)ogl_mesh->pos_offset);

				glBindBuffer(GL_ARRAY_BUFFER, vbo_handler.colorVBO);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)ogl_mesh->color_offset);

				glBindBuffer(GL_ARRAY_BUFFER, vbo_handler.normalVBO);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)ogl_mesh->normal_offset);

				glBindBuffer(GL_ARRAY_BUFFER, vbo_handler.texCoordVBO);
				glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)ogl_mesh->tex_coord_offset);

				// Draw call
				glDrawElements(GL_TRIANGLES, ogl_mesh->n_triangles * 3,
											GL_UNSIGNED_INT, (GLvoid*)(ogl_mesh->gl_draw_offset));
			}
		}

		glutSwapBuffers();
  };



	return &default_scene;
}
