#include "../aux/demo.hpp"

using namespace demo_default_objs;

namespace demo_1 {

  // INIT
  const std::function<void (void)> init = [] () {

    world_manager.camera = new FPSCameraHandler;

    texture_manager.prepare();

    // LOAD TEXTURES
    if (!texture_manager.load_texture("img/color2.png", "colorTex")) exit(-1);
    if (!texture_manager.load_texture("img/emissive.png", "emiTex")) exit(-1);

    // COMPILING SHADERS
    if (!shader_manager.load_vertex_shader("shaders_P4/shader.v0.vert", "v0")) exit(-1);
    if (!shader_manager.load_fragment_shader("shaders_P4/shader.v0.frag", "f0")) exit(-1);

    if (!shader_manager.load_vertex_shader("shaders_P4/shader.v1.vert", "v1")) exit(-1);
    if (!shader_manager.load_fragment_shader("shaders_P4/shader.v1.frag", "f1")) exit(-1);

    // COMPILING PROGRAMS
    if (!shader_manager.create_program("p0", "v0", "f0")) exit(-1);
    if (!shader_manager.create_program("p1", "v1", "f1")) exit(-1);

    // LOADING MESHES
    mesh_loader.import_from_file("meshes/bitxo_piernas.glb");
    mesh_loader.import_default_cube();

    vbo_manager.generate_VBOs();
    vbo_manager.populate_VBOs(mesh_loader.get_meshes());

    mesh_loader.clean();

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

    MeshInstance& robotmesh = world_manager.create_mesh_instance (ogl_meshes[0]);
    MeshInstance& cubemesh2 = world_manager.create_mesh_instance (ogl_meshes[1]);
    MeshInstance& cubemesh3 = world_manager.create_mesh_instance (ogl_meshes[1]);

    // GENERATE MATERIAL (INPUTS FOR SHADERS)
    Material& mat_a = material_manager.create_material();
    mat_a.shader_uniforms["colorTex"] = new SP_Texture(texture_manager.get_texture("colorTex"));
    mat_a.shader_uniforms["emiTex"] = new SP_Texture(texture_manager.get_texture("emiTex"));
    mat_a.shader_uniforms["color_override"] = new SP_Vec4f({0,0,0,0});

    // ASSIGN MAERIALS TO MESH INSTANCES
    robotmesh.mat = &mat_a;
    cubemesh2.mat = &mat_a;
    cubemesh3.mat = &mat_a;

    // CREATE BEHAVIOUR LOGIC FOR MESH INSTANCES
    cubemesh3.update_logic = [](Spatial& self, const float dummy_time) {
      self.rotation().x = dummy_time / 4;
      self.rotation().y = dummy_time / 4;
    };

    cubemesh2.translation().x = -2;
    cubemesh2.translation().y = 2;

    robotmesh.translation().x = 2;
    robotmesh.translation().y = -2;

    // ASSIGN PROGRAMS TO MESHES
    shader_manager.set_mesh_per_program("p0", cubemesh2);
    shader_manager.set_mesh_per_program("p1", robotmesh);
    shader_manager.set_mesh_per_program("p1", cubemesh3);
  };


  // ON_RESIZE
  const std::function<void (int, int)> on_resize = [](int W, int H) {
    world_manager.camera->update_aspect_ratio(W, H);
    glViewport(0, 0, W, H);
    glutPostRedisplay();
  };


  // ON KEYBOARD
  const std::function<void (unsigned char)> on_keyboard = [] (unsigned char key) {
    world_manager.camera->handle_keys(key);

    if (key == 'G' || key == 'g') {
      std::cout << "Cambiando de escena" << std::endl;
      scene_manager.change_scene("scene_2");
    }
  };


  // ON MOUSE BUTTON
  const std::function<void (int, int, int, int)> on_mouse_button = [](int button, int state, int x, int y) {
    world_manager.camera->handle_mouse_buttons(button, state, x, y);
  };


  // ON MOUSE MOTION
  const std::function<void (int, int)> on_mouse_motion = [](int x, int y) {
    world_manager.camera->handle_mouse_motion(x, y);
  };


  // ON IDLE
  const std::function<void (void)> on_idle = [] () {
    world_manager.update();
    glutPostRedisplay();
  };


  // ON RENDER
  const std::function<void (void)> render = []() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const auto& view = world_manager.camera->get_view_matrix();
    const auto& proj = world_manager.camera->get_projection_matrix();

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
          mesh_instance->mat->upload_uniform(name, parameter_id);
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
}