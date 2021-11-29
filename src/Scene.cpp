#include "Scene.hpp"
#include "SceneManager.hpp"

ShaderManager Scene::shader_manager;
TextureManager Scene::texture_manager;
VBOManager Scene::vbo_manager;
MeshLoader Scene::loader;

Scene* Scene::generate_default() {
  Scene& default_scene = *new Scene;

  // INIT
	default_scene.init = [&] () {
    default_scene.dummy_time = 0;
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

		// COMPILING SHADERS
		if (!shader_manager.load_vertex_shader("shaders_P3/shader.v0.vert", "v0")) exit(-1);
		if (!shader_manager.load_fragment_shader("shaders_P3/shader.v0.frag", "f0")) exit(-1);

		if (!shader_manager.load_vertex_shader("shaders_P3/shader.v1.vert", "v1")) exit(-1);
		if (!shader_manager.load_fragment_shader("shaders_P3/shader.v1.frag", "f1")) exit(-1);

		// COMPILING PROGRAMS
		if (!shader_manager.create_program("p0", "v0", "f0", uniforms)) exit(-1);
		if (!shader_manager.create_program("p1", "v1", "f1", uniforms)) exit(-1);

		// LOADING MESHES
		loader.import_from_file("meshes/bitxo_piernas.glb");
		loader.import_default_cube();

		vbo_manager.generate_VBOs();
		vbo_manager.populate_VBOs(loader.get_meshes());

    loader.clean();

		// TANKING ADVANTAGE OF LAYOUT / LOCATION
		const std::map<std::string, unsigned> attribute_name_location {
			{"inPos", 0},
			{"inColor", 1},
			{"inNormal", 2},
			{"inTexCoord", 3},
		};

		shader_manager.bound_program_attributes("p0", attribute_name_location);
		shader_manager.bound_program_attributes("p1", attribute_name_location);

		// GENERATE INSTANCES OF THE MESHES ALREADY LOADED IN THE VBO
		const auto& ogl_meshes = vbo_manager.get_meshes();

		MeshInstance* robotmesh = new MeshInstance (ogl_meshes[0]);
		MeshInstance* cubemesh2 = new MeshInstance (ogl_meshes[1]);
		MeshInstance* cubemesh3 = new MeshInstance (ogl_meshes[1]);

		// GENERATE MATERIAL (INPUTS FOR SHADERS)
		Material* mat_a = new Material;
		mat_a->shader_uniforms["colorTex"] = new SP_Texture(texture_manager.get_texture("colorTex"));
		mat_a->shader_uniforms["emiTex"] = new SP_Texture(texture_manager.get_texture("emiTex"));
		mat_a->shader_uniforms["color_override"] = new SP_Vec4f({0,0,0,0});

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

		// ASSIGN PROGRAMS TO MESHES
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

    if (default_scene.dummy_time > 10) {
      default_scene.clean();
      std::cout << "Cambiando de escena" << std::endl;
      default_scene.change_scene("scene_2");
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
			glBindVertexArray(vbo_manager.get_vao());

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
				glBindBuffer(GL_ARRAY_BUFFER, vbo_manager.posVBO);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)ogl_mesh->pos_offset);

				glBindBuffer(GL_ARRAY_BUFFER, vbo_manager.colorVBO);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)ogl_mesh->color_offset);

				glBindBuffer(GL_ARRAY_BUFFER, vbo_manager.normalVBO);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)ogl_mesh->normal_offset);

				glBindBuffer(GL_ARRAY_BUFFER, vbo_manager.texCoordVBO);
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

void Scene::clean() {
  texture_manager.clean();
  shader_manager.clean();
  vbo_manager.clean();

  delete camera;

  for (auto* object : scene_objects)
    delete object;

  scene_objects.clear();
}

Scene* Scene::generate_scene_2() {
  Scene& scene_2 = *new Scene;

  scene_2.init = [&] () {
    scene_2.dummy_time = 0;
    scene_2.camera = new FPSCameraHandler;

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

    // COMPILING SHADERS
    if (!shader_manager.load_vertex_shader("shaders_P3/shader.v0.vert", "v0")) exit(-1);
    if (!shader_manager.load_fragment_shader("shaders_P3/shader.v0.frag", "f0")) exit(-1);

    if (!shader_manager.load_vertex_shader("shaders_P3/shader.v1.vert", "v1")) exit(-1);
    if (!shader_manager.load_fragment_shader("shaders_P3/shader.v1.frag", "f1")) exit(-1);

    // COMPILING PROGRAMS
    if (!shader_manager.create_program("p0", "v0", "f0", uniforms)) exit(-1);
    if (!shader_manager.create_program("p1", "v1", "f1", uniforms)) exit(-1);

    // LOADING MESHES
    loader.import_default_cube();
    loader.import_from_file("meshes/suzanne.glb");

    vbo_manager.generate_VBOs();
    vbo_manager.populate_VBOs(loader.get_meshes());

    loader.clean();

    // TANKING ADVANTAGE OF LAYOUT / LOCATION
    const std::map<std::string, unsigned> attribute_name_location {
      {"inPos", 0},
      {"inColor", 1},
      {"inNormal", 2},
      {"inTexCoord", 3},
    };

    shader_manager.bound_program_attributes("p0", attribute_name_location);
    shader_manager.bound_program_attributes("p1", attribute_name_location);

    // GENERATE INSTANCES OF THE MESHES ALREADY LOADED IN THE VBO
    const auto& ogl_meshes = vbo_manager.get_meshes();

    MeshInstance* robotmesh = new MeshInstance (ogl_meshes[1]);

    // GENERATE MATERIAL (INPUTS FOR SHADERS)
    Material* mat_a = new Material;
    mat_a->shader_uniforms["colorTex"] = new SP_Texture(texture_manager.get_texture("colorTex"));
    mat_a->shader_uniforms["emiTex"] = new SP_Texture(texture_manager.get_texture("emiTex"));
    mat_a->shader_uniforms["color_override"] = new SP_Vec4f({0,1,0,0});

    // ASSIGN MAERIALS TO MESH INSTANCES
    robotmesh->mat = mat_a;

    // CREATE BEHAVIOUR LOGIC FOR MESH INSTANCES
    robotmesh->update_logic = [](Spatial& self, const float dummy_time) {
      self.rotation().y = dummy_time / 4;
    };

    // ADD MESH INSTANCES TO SCENES
    scene_2.scene_objects.push_back(robotmesh);

    // ASSIGN PROGRAMS TO MESHES
    shader_manager.set_mesh_per_program("p1", robotmesh);
  };


  // ON_RESIZE
  scene_2.on_resize = [&](int W, int H) {
    scene_2.camera->update_aspect_ratio(W, H);
    glViewport(0, 0, W, H);
    glutPostRedisplay();
  };


  // ON KEYBOARD
  scene_2.on_keyboard = [&] (unsigned char key) {
    scene_2.camera->handle_keys(key);
  };


  // ON MOUSE BUTTON
  scene_2.on_mouse_button = [&](int button, int state, int x, int y) {
    scene_2.camera->handle_mouse_buttons(button, state, x, y);
  };


  // ON MOUSE MOTION
  scene_2.on_mouse_motion = [&](int x, int y) {
    scene_2.camera->handle_mouse_motion(x, y);
  };


  // ON IDLE
  scene_2.on_idle = [&] () {
    for (auto object : scene_2.scene_objects) {
      object->update(scene_2.dummy_time);
    }

    if (scene_2.dummy_time > 10) {
      scene_2.clean();
      std::cout << "Cambiando de escena" << std::endl;
      scene_2.change_scene("main");
    }

    scene_2.dummy_time += .1;
    glutPostRedisplay();
  };


  // ON RENDER
  scene_2.render = [&]() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const auto& view = scene_2.camera->get_view_matrix();
    const auto& proj = scene_2.camera->get_projection_matrix();

    for (auto p : shader_manager.programs) {
      Program& program = *p.second;

      glUseProgram(program.id);
      glBindVertexArray(vbo_manager.get_vao());

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
        glBindBuffer(GL_ARRAY_BUFFER, vbo_manager.posVBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)ogl_mesh->pos_offset);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_manager.colorVBO);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)ogl_mesh->color_offset);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_manager.normalVBO);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)ogl_mesh->normal_offset);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_manager.texCoordVBO);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)ogl_mesh->tex_coord_offset);

        // Draw call
        glDrawElements(GL_TRIANGLES, ogl_mesh->n_triangles * 3,
                      GL_UNSIGNED_INT, (GLvoid*)(ogl_mesh->gl_draw_offset));
      }
    }

    glutSwapBuffers();
  };

  return &scene_2;
}