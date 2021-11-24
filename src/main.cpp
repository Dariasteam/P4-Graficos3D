#include "BOX.h"
#include "Camera.h"
#include "Spatial.h"
#include "auxiliar.h"
#include "OpenGLManager.h"

#include <GL/glew.h>
#define SOLVE_FGLUT_WARNING
#include <GL/freeglut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

OpenGLManager opengl_manager;
AbstractCameraHandler* camera = new FPSCameraHandler;
std::vector<Spatial*> scene_objects;

//////////////////////////////////////////////////////////////
// Datos que se almacenan en la memoria de la CPU
//////////////////////////////////////////////////////////////

//Matrices
int w, h;

//////////////////////////////////////////////////////////////
// Variables que nos dan acceso a Objetos OpenGL
//////////////////////////////////////////////////////////////
/*
unsigned int vshader;
unsigned int fshader;
unsigned int program;
*/

//Variables Uniform
/*
int uModelViewMat;
int uModelViewProjMat;
int uNormalMat;
*/

//Texturas Uniform
int uColorTex; // Identificadores de variables del shader (puerto del texture unit)
int uEmiTex;

/*
//Atributos
int inPos;
int inColor;
int inNormal;
int inTexCoord;

unsigned int vao;
//VBOs que forman parte del objeto
unsigned int posVBO;
unsigned int colorVBO;
unsigned int normalVBO;
unsigned int texCoordVBO;
unsigned int triangleIndexVBO;

*/
//Textures identificadro de la textura en si
unsigned int colorTexId;
unsigned int emiTexId;

//////////////////////////////////////////////////////////////
// Funciones auxiliares
//////////////////////////////////////////////////////////////
//!!Por implementar

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


//Carga el shader indicado, devuele el ID del shader
//!Por implementar
GLuint loadShader(const char *fileName, GLenum type);


int main(int argc, char** argv) {
	std::locale::global(std::locale("es_ES.UTF-8")); // acentos ;)

	initContext(argc, argv);
	initOGL();

	std::vector<std::string> uniforms {
		"normal",
		"modelView",
		"modelViewProj"
	};

	std::vector<std::string> attributes {
		"inPos",
		"inColor",
		"inNormal",
	};

	if (!opengl_manager.load_vertex_shader("shaders_P3/shader.v0.vert")) exit(-1);
	if (!opengl_manager.load_fragment_shader("shaders_P3/shader.v0.frag")) exit(-1);
	if (!opengl_manager.create_program(opengl_manager.vertex_shaders[0],
																		 opengl_manager.fragment_shaders[0],
																		 uniforms, attributes)) exit(-1);

	opengl_manager.instantiateMesh(cubeNVertex,
																 cubeNTriangleIndex,
																 cubeTriangleIndex,
																 cubeVertexPos,
																 cubeVertexColor,
																 cubeVertexNormal,
																 cubeVertexTexCoord,
																 cubeVertexTangent);

	Mesh* cubemesh1 = new Mesh;
	Mesh* cubemesh2 = new Mesh;

	cubemesh2->translation().x = -2;
	cubemesh2->translation().y = 2;

	cubemesh1->set_obj_id(0);
	cubemesh2->set_obj_id(0);

	scene_objects.push_back(cubemesh1);
	scene_objects.push_back(cubemesh2);

	opengl_manager.set_mesh_per_program(opengl_manager.programs.begin()->second.id, cubemesh1);
	opengl_manager.set_mesh_per_program(opengl_manager.programs.begin()->second.id, cubemesh2);

	if (uColorTex != -1) glUniform1i(uColorTex, 0);
	if (uEmiTex != -1) glUniform1i(uEmiTex, 1);

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
}

void destroy() {
	opengl_manager.destroy();
}

void initShader(const std::string& vname, const std::string& fname)
{
	/*
	vshader = loadShader(vname, GL_VERTEX_SHADER);
	fshader = loadShader(fname, GL_FRAGMENT_SHADER);

	program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);
	glLinkProgram(program);


	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		char* logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		glDeleteProgram(program);
		program = 0;
		exit(-1);
	}

	uNormalMat = glGetUniformLocation(program, "normal");
	uModelViewMat = glGetUniformLocation(program, "modelView");
	uModelViewProjMat = glGetUniformLocation(program, "modelViewProj");

	opengl_manager.inPos = glGetAttribLocation(program, "inPos");
	opengl_manager.inColor = glGetAttribLocation(program, "inColor");
	opengl_manager.inNormal = glGetAttribLocation(program, "inNormal");
	opengl_manager.inTexCoord = glGetAttribLocation(program, "inTexCoord");
	*/

	std::vector<std::string> uniforms {
		"normal",
		"modelView",
		"modelViewProj"
	};

	std::vector<std::string> attributes {
		"inPos",
		"inColor",
		"inNormal",
		"inTexCoord",
	};

	if (!opengl_manager.load_vertex_shader(vname)) exit(-1);
	if (!opengl_manager.load_fragment_shader(fname)) exit(-1);
	if (!opengl_manager.create_program(0, 0, uniforms, attributes)) exit(-1);

//	glUseProgram(opengl_manager.programs[0].id);

	if (uColorTex != -1) glUniform1i(uColorTex, 0);
	if (uEmiTex != -1) glUniform1i(uEmiTex, 1);
}



void renderFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glUseProgram(program);

	glViewport(0, 0, w, h);

	//Texturas
	if (uColorTex != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorTexId);
	} else {
		std::cout << "ERROR cargando textura 1\n";
	}

	if (uEmiTex != -1) {
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, emiTexId);
	} else {
		std::cout << "ERROR cargando textura 2\n";
	}

	// Meshes
	const auto view = camera->get_view_matrix();
	const auto proj = camera->get_projection_matrix();

	for (auto p : opengl_manager.programs) {
		Program program = p.second;
		glUseProgram(program.id);

		for (Mesh* mesh : program.asociated_meshes) {
			const auto model = mesh->get_model_matrix();

			glm::mat4 modelView = view * model;
			glm::mat4 modelViewProj = proj * modelView;
			glm::mat4 normal = glm::transpose(glm::inverse(modelView));

			if (program.uniforms["modelView"] != -1)
				glUniformMatrix4fv(program.uniforms["modelView"], 1, GL_FALSE, &(modelView[0][0]));
			if (program.uniforms["modelViewProj"] != -1)
				glUniformMatrix4fv(program.uniforms["modelViewProj"], 1, GL_FALSE, &(modelViewProj[0][0]));
			if (program.uniforms["normal"] != -1)
				glUniformMatrix4fv(program.uniforms["normal"], 1, GL_FALSE, &(normal[0][0]));

			glBindVertexArray(opengl_manager.vao);
			glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3,
								   	 GL_UNSIGNED_INT, (void*)0);
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
	dummy_time += 1;
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