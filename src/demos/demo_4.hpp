#include "../aux/demo.hpp"

#include "scripts/rotate_dir_light.h"
#include "scripts/orbital_camera.h"
#include "scripts/mesh_rotator.h"
#include "scripts/blue_light.h"

#include <cstdlib>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/matrix.hpp>
#include <chrono>

#include <thread>

using namespace demo_default_objs;

namespace demo_4 {

  // INIT
  const std::function<void (void)> init = [] () {
    auto& camera = world_manager.create_camera();

    FboManager::get().init();
    FboManager::get().resizeFBO(500, 500);

    // LOAD TEXTURES
    if (!texture_manager.load_texture("img/color2.png", "colorTex")) exit(-1);
    if (!texture_manager.load_texture("img/emissive.png", "emiTex")) exit(-1);
    if (!texture_manager.load_texture("img/normal.png", "normalTex")) exit(-1);
    if (!texture_manager.load_texture("img/specMap.png", "specTex")) exit(-1);

    if (!texture_manager.load_texture("img/helmet_albedo.png", "helmet_albedo")) exit(-1);
    if (!texture_manager.load_texture("img/helmet_normal.png", "helmet_normal")) exit(-1);
    if (!texture_manager.load_texture("img/helmet_spec.png",    "helmet_spec")) exit(-1);
    if (!texture_manager.load_texture("img/helmet_emissive.png", "helmet_emissive")) exit(-1);

    // COMPILING SHADERS
    if (!shader_manager.load_vertex_shader("shaders_P4/shader_material.vert", "v0")) exit(-1);
    if (!shader_manager.load_fragment_shader("shaders_P4/shader_material.frag", "f0")) exit(-1);

    // LINKING POST PROCESS PROGRAMS
    if (!shader_manager.create_program("p0", "v0", "f0")) exit(-1);

    // LOADING MESHES
    mesh_loader.import_default_cube();
    //mesh_loader.import_from_file("meshes/suzanne_smooth.glb");
    mesh_loader.import_from_file("meshes/helmet.fbx");

    vbo_manager.generate_VBOs();
    vbo_manager.populate_VBOs(mesh_loader.get_meshes());

    mesh_loader.clean();

    // TANKING ADVANTAGE OF LAYOUT / LOCATION
    const std::map<std::string, unsigned> attribute_name_location {
      {"inPos", 0},
      {"inColor", 1},
      {"inNormal", 2},
      {"inTangent", 3},
      {"inTexCoord", 4},
    };

    shader_manager.bind_program_attributes("p0", attribute_name_location);

    // GENERATE INSTANCES OF THE MESHES ALREADY LOADED IN THE VBO
    const auto& ogl_meshes = vbo_manager.get_meshes();

    //MeshInstance& cubemesh = world_manager.create_mesh_instance(ogl_meshes[0]);
    MeshInstance& helmetmesh = world_manager.create_mesh_instance(ogl_meshes[1]);

    // GENERATE MATERIAL (INPUTS FOR SHADERS)
    Material& mat_a = material_manager.create_material();
    Material& mat_b = material_manager.create_material();

    mat_a.shader_mat_uniforms["colorTex"] = new SP_Texture(texture_manager.get_texture("colorTex"));
    mat_a.shader_mat_uniforms["emiTex"] = new SP_Texture(texture_manager.get_texture("emiTex"));
    mat_a.shader_mat_uniforms["normalTex"] = new SP_Texture(texture_manager.get_texture("normalTex"));
    mat_a.shader_mat_uniforms["specularTex"] = new SP_Texture(texture_manager.get_texture("specTex"));

    mat_b.shader_mat_uniforms["colorTex"] = new SP_Texture(texture_manager.get_texture("helmet_albedo"));
    mat_b.shader_mat_uniforms["emiTex"] = new SP_Texture(texture_manager.get_texture("helmet_emissive"));
    mat_b.shader_mat_uniforms["normalTex"] = new SP_Texture(texture_manager.get_texture("helmet_normal"));
    mat_b.shader_mat_uniforms["specularTex"] = new SP_Texture(texture_manager.get_texture("helmet_spec"));

    // BIND MATERIAL - SHADER - MESH INSTANCE
    shader_manager.bind_material("p0", mat_a);
    shader_manager.bind_material("p0", mat_b);

    //mat_a.associate_mesh_instance(&cubemesh);
    mat_b.associate_mesh_instance(&helmetmesh);

    // INSTANTIATE LIGHTS
    PointLight& point_light = light_manager.create_point_light();
    point_light.color.vec_3 = {0, 0, 1};
    point_light.translation() = {2, -2, 0};

    srand(NULL);
    for (unsigned i = 0; i < 14; i++) {
      double r = ((double) rand() / (RAND_MAX));
      FocalLight& focal_light = light_manager.create_focal_light();
      focal_light.color.vec_3 = {2 + i * .04, 4 - i * .04, r};
      focal_light.translation() = {-0.5 + i * 0.009, -1, 3};
      focal_light.direction.vec_3 = glm::normalize(glm::vec3{0, r / 2, -1});
      focal_light.aperture.value = .08;
    }

    AmbientLight& ambient_light = light_manager.get_ambient_light();
    ambient_light.color.vec_3 = {.2, .2, .2};

    DirectionalLight& dir_light = light_manager.get_directional_light();
    dir_light.color.vec_3 = {1, 1, 1};
    dir_light.direction.vec_3 = glm::normalize(glm::vec3{1, -1, 0});

    // CREATE BEHAVIOUR LOGIC


    dir_light.script(rotate_dir_light);
    helmetmesh.script(mesh_rotator);
    point_light.script(blue_light);
    camera.script(orbital_camera);

    double* value;

    for (unsigned i = 0; i < 10000; i++) {
      std::cout << "e" << std::endl;
    };

    if (helmetmesh.get_parameter("value", &value)) {
      (*value) = 0.1;
    }

    std::function<void (void)>* c;
    helmetmesh.get_parameter("call", &c);
    (*c)();
  };


  // ON_RESIZE
  const std::function<void (int, int)> on_resize = [](int W, int H) {
    world_manager.camera->update_aspect_ratio(W, H);
    glViewport(0, 0, W, H);
    glutPostRedisplay();
  };


  // ON KEYBOARD
  const std::function<void (unsigned char key)> on_keyboard = [](unsigned char key) {
    scriptable_manager.on_keyboard(key);

    if (key == 'G' || key == 'g') {
      std::cout << "Cambiando de escena" << std::endl;
      scene_manager.change_scene("scene_1");
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
    scriptable_manager.on_mouse_button(button, state, x, y);
  };


  // ON MOUSE MOTION
  const std::function<void (int, int)> on_mouse_motion = [](int x, int y) {
    scriptable_manager.on_mouse_motion(x, y);
  };


  // ON IDLE
  const std::function<void (void)> on_idle = [] () {
    scriptable_manager.on_update();
    glutPostRedisplay();
  };

  // ON RENDER
  const std::function<void (void)> render = []() {

    // RENDER TO FBO
    glBindFramebuffer(GL_FRAMEBUFFER, FboManager::get().deferred_fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto& camera = world_manager.camera;

    for (auto p : shader_manager.programs) {
      Program& program = *p.second;

      glUseProgram(program.id);
      glBindVertexArray(vbo_manager.get_vao());

      for (Material* material : program.associated_materials) {
        material->upload_mat_uniforms();

        for (MeshInstance* mesh_instance : material->associated_mesh_instances) {
          const OglMesh* ogl_mesh = mesh_instance->mesh;

          material->set_geometry_uniforms(*mesh_instance, camera);
          material->upload_geometry_uniforms();

          vbo_manager.upload_attributes_for_mesh (*ogl_mesh);

          // Draw call
          glDrawElements(GL_TRIANGLES, ogl_mesh->n_triangles * 3,
                        GL_UNSIGNED_INT, (GLvoid*)(ogl_mesh->gl_draw_offset));
        }
      }
    }



    // DEFERRED
    glBindFramebuffer(GL_FRAMEBUFFER, FboManager::get().post_processing_fbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_CULL_FACE);
	  glDisable(GL_DEPTH_TEST);

    glBindVertexArray(FboManager::get().planeVAO);

    // SINGLE PASS LIGHTS
    auto& program_l1 = shader_manager.programs["p_pbase"];
    glUseProgram(program_l1->id);
    light_manager.upload_single_lights(camera->get_view_matrix());

    FboManager::get().mat_lightning_base.upload_mat_uniforms();

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glEnable (GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);

    // MULTIPLE PASS LIGHTS
    auto& program2 = shader_manager.programs["p_p0"];
    glUseProgram(program2->id);
    FboManager::get().mat_lightning_passes.upload_mat_uniforms();

    while(light_manager.upload_next_light_pass(camera->get_view_matrix())) {
      glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }



    // POST PROCESSING

    glDisable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ZERO);
    glBlendEquation(GL_FUNC_ADD);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto& program3 = shader_manager.programs["post_processing"];
    glUseProgram(program3->id);
    FboManager::get().mat_post_processing.upload_mat_uniforms();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glutSwapBuffers();
  };
}