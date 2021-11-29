#ifndef _SCENE_H_
#define _SCENE_H_

#include "ShaderManager.hpp"
#include "TextureManager.hpp"
#include "Camera.h"
#include "Material.hpp"
#include "Spatial.h"
#include "auxiliar.h"
#include "VBOHandler.hpp"
#include "ShaderManager.hpp"
#include "MeshLoader.h"

#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <functional>

struct Scene {
public:
  double dummy_time = 0;

  AbstractCameraHandler* camera;
  std::vector<Spatial*> scene_objects;

  static ShaderManager shader_manager;
  static TextureManager texture_manager;
  static VBOHandler vbo_handler;
  static MeshLoader loader;

  void end () {
    // clean all handlers
  }

public:
  std::function<void (const std::string& scene_name)> change_scene;
  std::function<void (void)> init;

  std::function<void (int width, int height)>  on_resize = [&](int W, int H) {
		camera->update_aspect_ratio(W, H);
    glViewport(0, 0, W, H);
		glutPostRedisplay();
  };

  std::function<void (unsigned char key)>  on_keyboard = [&] (unsigned char key) {
    camera->handle_keys(key);
  };

  std::function<void (int button, int state, int x, int y)>  on_mouse_button =
    [&](int button, int state, int x, int y) {
      camera->handle_mouse_buttons(button, state, x, y);
  };

  std::function<void (int x, int y)>  on_mouse_motion= [&](int x, int y) {
    camera->handle_mouse_motion(x, y);
  };

  std::function<void (void)> on_idle = [&] () {
		for (auto object : scene_objects) {
			object->update(dummy_time);
		}
		dummy_time += .1;
		glutPostRedisplay();
  };

  std::function<void (void)> render = [&]() {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		const auto& view = camera->get_view_matrix();
		const auto& proj = camera->get_projection_matrix();

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

};

#endif //_SCENE_H_