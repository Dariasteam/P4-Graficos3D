#include "MeshLoader.h"
#include "Camera.h"
#include "Material.hpp"
#include "Spatial.h"
#include "auxiliar.h"
#include "OpenGLManager.h"
#include "VBOHandler.hpp"

#include <GL/glew.h>
#include <glm/fwd.hpp>
#define SOLVE_FGLUT_WARNING
#include <GL/freeglut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

OGLManager opengl_manager;
AbstractCameraHandler* camera = new FPSCameraHandler;
std::vector<Spatial*> scene_objects;
VBOHandler vbo_handler;
MeshLoader loader;

int w, h;

//Declaraci�n de CB
void renderFunc();
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMotionFunc (int, int);

//Funciones de inicializaci�n y destrucci�n
void initContext(int argc, char** argv);
void initOGL();
void initShader(const std::string& vname, const std::string& fname);
void initObj();
void destroy();

int main(int argc, char** argv) {
	std::locale::global(std::locale("es_ES.UTF-8")); // acentos ;)

	initContext(argc, argv);
	initOGL();

	// LOAD TEXTURES
	if (!opengl_manager.load_texture("img/color2.png", "colorTex")) exit(-1);
	if (!opengl_manager.load_texture("img/emissive.png", "emiTex")) exit(-1);

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
	if (!opengl_manager.load_vertex_shader("shaders_P3/shader.v0.vert", "v0")) exit(-1);
	if (!opengl_manager.load_fragment_shader("shaders_P3/shader.v0.frag", "f0")) exit(-1);

	if (!opengl_manager.load_vertex_shader("shaders_P3/shader.v1.vert", "v1")) exit(-1);
	if (!opengl_manager.load_fragment_shader("shaders_P3/shader.v1.frag", "f1")) exit(-1);

	// COMPILING PROGRAMS
	if (!opengl_manager.create_program("p0",
																		 *opengl_manager.vertex_shaders["v0"],
																		 *opengl_manager.fragment_shaders["f0"],
																		 uniforms, attributes)) exit(-1);

	if (!opengl_manager.create_program("p1",
																		 *opengl_manager.vertex_shaders["v1"],
																		 *opengl_manager.fragment_shaders["f1"],
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

	opengl_manager.bound_program_attributes(*opengl_manager.programs["p0"], attribute_name_location);
	opengl_manager.bound_program_attributes(*opengl_manager.programs["p1"], attribute_name_location);

	auto ogl_meshes = vbo_handler.get_meshes();

	MeshInstance* robotmesh = new MeshInstance (ogl_meshes[0]);
	MeshInstance* cubemesh2 = new MeshInstance (ogl_meshes[1]);
 	MeshInstance* cubemesh3 = new MeshInstance (ogl_meshes[1]);

	Material* mat_a = new Material;
	mat_a->shader_parameters["colorTex"] = new SP_Texture(opengl_manager.texture_ids["colorTex"]);
	mat_a->shader_parameters["emiTex"] = new SP_Texture(opengl_manager.texture_ids["emiTex"]);
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

	scene_objects.push_back(robotmesh);
	scene_objects.push_back(cubemesh2);
	scene_objects.push_back(cubemesh3);

	opengl_manager.set_mesh_per_program(*opengl_manager.programs["p1"], cubemesh2);
	opengl_manager.set_mesh_per_program(*opengl_manager.programs["p1"], robotmesh);
	opengl_manager.set_mesh_per_program(*opengl_manager.programs["p1"], cubemesh3);

	glutMainLoop();
	destroy();

	return 0;
}

//////////////////////////////////////////
// Funciones auxiliares
void initContext(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Prácticas OGL");


	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		exit(-1);
	}
	const GLubyte* oglVersion = glGetString(GL_VERSION);
	std::cout << "This system supports OpenGL Version: " << oglVersion << std::endl;

	glutReshapeFunc(resizeFunc);
	glutDisplayFunc(renderFunc);
	glutIdleFunc(idleFunc);
	glutKeyboardFunc(keyboardFunc);
	glutMotionFunc(mouseMotionFunc);
}

void initOGL()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);

	if (glutExtensionSupported ("GL_EXT_texture_filter_anisotropic")) {
		GLfloat fLargest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
	}
}

void destroy() {
	opengl_manager.destroy();
}

void renderFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, w, h);

	const auto& view = camera->get_view_matrix();
	const auto& proj = camera->get_projection_matrix();

	for (auto p : opengl_manager.programs) {
		Program& program = *p.second;

		glUseProgram(program.id);
		glBindVertexArray(vbo_handler.get_vao());

		for (MeshInstance* mesh_instance : program.associated_meshes) {
			const OglMesh* ogl_mesh = mesh_instance->mesh;
			const auto model = mesh_instance->get_model_matrix();
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
}

void resizeFunc(int width, int height)
{
	camera->update_aspect_ratio(width, height);
	w = width; h = height;
	glutPostRedisplay();
}

float dummy_time = 0;
void idleFunc() {
	for (auto object : scene_objects) {
		object->update(dummy_time);
	}
	dummy_time += .1;
	glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y) {
	camera->handle_keys(key);
}

void mouseFunc(int button, int state, int x, int y) {
	camera->handle_mouse_buttons(button, state, x, y);
}

void mouseMotionFunc(int x, int y) {
	camera->handle_mouse_motion(x, y);
}