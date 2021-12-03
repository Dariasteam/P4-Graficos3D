#include "../aux/demo.hpp"
#include <cstdlib>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/matrix.hpp>

using namespace demo_default_objs;

namespace demo_3 {

  // INIT
  const std::function<void (void)> init = [] () {
    world_manager.camera = new OrbitalCameraHandler;

    FboManager::get().init();
    FboManager::get().resizeFBO(500, 500);

    texture_manager.prepare();
    // LOAD TEXTURES
    if (!texture_manager.load_texture("img/color2.png", "colorTex")) exit(-1);
    if (!texture_manager.load_texture("img/emissive.png", "emiTex")) exit(-1);
    if (!texture_manager.load_texture("img/normal.png", "normalTex")) exit(-1);
    if (!texture_manager.load_texture("img/specMap.png", "specTex")) exit(-1);

    // COMPILING SHADERS
    if (!shader_manager.load_vertex_shader("shaders_P4/shader_material.vert", "v0")) exit(-1);
    if (!shader_manager.load_fragment_shader("shaders_P4/shader_material.frag", "f0")) exit(-1);

    // LINKING POST PROCESS PROGRAMS
    if (!shader_manager.create_program("p0", "v0", "f0")) exit(-1);

    // LOADING MESHES
    mesh_loader.import_default_cube();
    mesh_loader.import_from_file("meshes/suzanne_smooth.glb");

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

    shader_manager.bind_program_attributes("p0", attribute_name_location);

    // GENERATE INSTANCES OF THE MESHES ALREADY LOADED IN THE VBO
    const auto& ogl_meshes = vbo_manager.get_meshes();

    MeshInstance& robotmesh = world_manager.create_mesh_instance(ogl_meshes[1]);

    // GENERATE MATERIAL (INPUTS FOR SHADERS)

    Material& mat_a = material_manager.create_material();

    mat_a.shader_uniforms["colorTex"] = new SP_Texture(texture_manager.get_texture("colorTex"));
    mat_a.shader_uniforms["emiTex"] = new SP_Texture(texture_manager.get_texture("emiTex"));
    mat_a.shader_uniforms["normalTex"] = new SP_Texture(texture_manager.get_texture("normalTex"));
    mat_a.shader_uniforms["specularTex"] = new SP_Texture(texture_manager.get_texture("specTex"));

    // ASSIGN MATERIALS TO MESH INSTANCES
    robotmesh.mat = &mat_a;

    // BIND LIGHT IDS IN PROGRAM TO LIGHTS
    light_manager.bind_program_ids("p_p0", ShaderManager::P_LIGHTING);

    // INSTANTIATE LIGHTS
    PointLight& point_light = light_manager.create_point_light();
    point_light.color.vec_3 = {0, 0, 1};
    point_light.translation() = {2, 0, 0};

    srand(NULL);
    std::cout << rand()  << std::endl;

    for (unsigned i = 0; i < 40; i++) {
      double r = ((double) rand() / (RAND_MAX));
      FocalLight& focal_light = light_manager.create_focal_light();
      focal_light.color.vec_3 = {i * .1, 4 - i * .1, r};
      focal_light.translation() = {-0.5 + i * 0.025, -1, 3};
      focal_light.direction.vec_3 = glm::normalize(glm::vec3{0, r / 2, -1});
      focal_light.aperture.value = .03;
    }

    DirectionalLight& dir_light = light_manager.create_directional_light();
    dir_light.color.vec_3 = {1, 1, 1};
    dir_light.direction.vec_3 = glm::normalize(glm::vec3{1, -1, 0});

    AmbientLight& ambient_light = light_manager.get_ambient_light();
    ambient_light.color.vec_3 = {.1, .1, .1};

    // CREATE BEHAVIOUR LOGIC FOR MESH INSTANCES
    robotmesh.update_logic = [](Spatial& self, const float dummy_time) {
      //self.rotation().y = dummy_time / 4;
    };

    // ASSIGN PROGRAMS TO MESHES
    shader_manager.set_mesh_per_program("p0", robotmesh);
  };


  // ON_RESIZE
  const std::function<void (int, int)> on_resize = [](int W, int H) {
    world_manager.camera->update_aspect_ratio(W, H);
    glViewport(0, 0, W, H);
    glutPostRedisplay();
  };



  // ON KEYBOARD
  const std::function<void (unsigned char key)> on_keyboard = [](unsigned char key) {
    world_manager.camera->handle_keys(key);

    if (key == 'G' || key == 'g') {
      std::cout << "Cambiando de escena" << std::endl;
      scene_manager.change_scene("scene_1");
    }

    if (key == 'F' || key == 'f') {
      auto& value = light_manager.point_lights[0]->color.vec_3.z;

      if (value > 10)
        value = 0;
      else
        value += .1;
    }

    if (key == 'V' || key == 'v') {
      auto& value = light_manager.dir_lights[0]->direction.vec_3;

      if (value.x > .5)
        value.x = -1;
      else
        value.x += .1;

      value = glm::normalize(value);
    }

    if (key == 'B' || key == 'b') {
      for (auto& v : light_manager.focal_lights) {
        v->direction.vec_3.y += .01;
        v->direction.vec_3 = glm::normalize(v->direction.vec_3);
      }
    } else if (key == 'N' || key == 'n') {
      for (auto& v : light_manager.focal_lights) {
        v->direction.vec_3.y -= .01;
        v->direction.vec_3 = glm::normalize(v->direction.vec_3);
      }
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

    // RENDER TO FBO
    glBindFramebuffer(GL_FRAMEBUFFER, FboManager::get().fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const auto& view = world_manager.camera->get_view_matrix();
    const auto& proj = world_manager.camera->get_projection_matrix();

    for (auto p : shader_manager.programs_shading) {
      Program& program = *p.second;

      glUseProgram(program.id);
      glBindVertexArray(vbo_manager.get_vao());

      for (MeshInstance* mesh_instance : program.associated_meshes) {
        const auto model = mesh_instance->get_model_matrix();
        const OglMesh* ogl_mesh = mesh_instance->mesh;
        Material* material = mesh_instance->mat;

        material->calculate_matrices(model, view, proj);

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

    // DEFERRED
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_CULL_FACE);
	  glDisable(GL_DEPTH_TEST);

    glBindVertexArray(FboManager::get().planeVAO);




    // SINGLE PASS LIGHTS
    auto& program = shader_manager.programs_lightning["p_pbase"];
    glUseProgram(program->id);

    light_manager.bind_program_ids("p_pbase", ShaderManager::P_LIGHTING);
    light_manager.upload_ambient_light();

    for (const auto& uniform : program->uniforms) {
      const std::string& name = uniform.first;
      const int parameter_id = uniform.second;
      FboManager::get().mat_lightning_base.upload_uniform(name, parameter_id);
    }

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glEnable (GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);


    // MULTIPLE PASS LIGHTS
    auto& program2 = shader_manager.programs_lightning["p_p0"];

    glUseProgram(program2->id);
    for (const auto& uniform : program2->uniforms) {
      const std::string& name = uniform.first;
      const int parameter_id = uniform.second;
      FboManager::get().mat_lightning_passes.upload_uniform(name, parameter_id);
    }

    light_manager.bind_program_ids("p_p0", ShaderManager::P_LIGHTING);

    while(light_manager.upload_next_light_pass(view)) {
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }


    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // POST PROCESSING
    glutSwapBuffers();
  };
}