#include "BOX.h"
#include "Camera.h"
#include "Spatial.h"
#include "auxiliar.h"


#include <GL/glew.h>
#define SOLVE_FGLUT_WARNING
#include <GL/freeglut.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

class OpenGLManager {
public:
	// We use this to preallocate all possible objects of the scene
	unsigned vao;

	unsigned posVBO;
	unsigned colorVBO;
	unsigned normalVBO;
	unsigned texCoordVBO;
	unsigned triangleIndexVBO;

	int inPos;
	int inColor;
	int inNormal;
	int inTexCoord;

	// Identificadores de texturas opengl
	int colorTexId;
	int emiTexId;

	int load_textures() {
		colorTexId = loadTex("img/color2.png");
		emiTexId = loadTex("img/emissive.png");
	}

	//Crea una textura, la configura, la sube a OpenGL,
	//y devuelve el identificador de la textura
	unsigned int loadTex(const char* fileName) {
		unsigned char* map;
		unsigned int w, h;
		map = loadTexture(fileName, w, h);
		if (!map)
		{
			std::cout << "Error cargando el fichero: "
				<< fileName << std::endl;
			exit(-1);
		}

		unsigned int texId;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, (GLvoid*)map);

		delete[] map;

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

		return texId;
	}

	int instantiateMesh(const unsigned n_vertices,
											const unsigned n_faces,
											const unsigned* faceIndices,
											const float* vertexCoord,
											const float* vertexColors,
											const float* normals,
											const float* texCoords,
											const float* tangents) {

		glGenBuffers(1, &posVBO);
		glGenBuffers(1, &colorVBO);
		glGenBuffers(1, &normalVBO);
		glGenBuffers(1, &texCoordVBO);

		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, n_vertices * sizeof(float) * 2,
								 texCoords, GL_STATIC_DRAW);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, posVBO);
		glBufferData(GL_ARRAY_BUFFER, n_vertices * sizeof(float) * 3,
								 NULL, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, n_vertices * sizeof(float) * 3,
										vertexCoord);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		if (inPos != -1)
			glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		glBufferData(GL_ARRAY_BUFFER, n_vertices * sizeof(float) * 3,
								 vertexColors, GL_STATIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		if (inColor != -1)
			glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, n_vertices * sizeof(float) * 3,
								 normals, GL_STATIC_DRAW);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

		if (inNormal != -1)
			glEnableVertexAttribArray(inNormal);


		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
		if (inTexCoord != -1)
			glEnableVertexAttribArray(3);

		glGenBuffers(1, &triangleIndexVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			n_faces * sizeof(unsigned int) * 3, faceIndices,
			GL_STATIC_DRAW);
	}

	~OpenGLManager() {
		glDeleteBuffers(1, &posVBO);
		glDeleteBuffers(1, &colorVBO);
		glDeleteBuffers(1, &normalVBO);
		glDeleteBuffers(1, &texCoordVBO);
		glDeleteBuffers(1, &triangleIndexVBO);
		glDeleteVertexArrays(1, &vao);
	}
};

OpenGLManager opengl_manager;

//////////////////////////////////////////////////////////////
// Datos que se almacenan en la memoria de la CPU
//////////////////////////////////////////////////////////////

//Matrices
glm::mat4	model = glm::mat4(1.0f);
int w, h;

AbstractCameraHandler* camera = new FPSCameraHandler;
std::vector<Spatial*> scene_objects;

//////////////////////////////////////////////////////////////
// Variables que nos dan acceso a Objetos OpenGL
//////////////////////////////////////////////////////////////
unsigned int vshader;
unsigned int fshader;
unsigned int program;

//Variables Uniform
int uModelViewMat;
int uModelViewProjMat;
int uNormalMat;


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

//Funciones de inicializaci�n y destrucci�n
void initContext(int argc, char** argv);
void initOGL();
void initShader(const char *vname, const char *fname);
void initObj();
void destroy();


//Carga el shader indicado, devuele el ID del shader
//!Por implementar
GLuint loadShader(const char *fileName, GLenum type);


int main(int argc, char** argv)
{
	std::locale::global(std::locale("es_ES.UTF-8")); // acentos ;)

	initContext(argc, argv);
	initOGL();
	initShader("shaders_P3/shader.v0.vert", "shaders_P3/shader.v0.frag");
	opengl_manager.instantiateMesh(cubeNVertex,
																 cubeNTriangleIndex,
																 cubeTriangleIndex,
																 cubeVertexPos,
																 cubeVertexColor,
																 cubeVertexNormal,
																 cubeVertexTexCoord,
																 cubeVertexTangent);

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
	//glutMotionFunc(mouseMotionFunc);
}

void initOGL()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
}

void destroy()
{
	glDetachShader(program, vshader);
	glDetachShader(program, fshader);
	glDeleteShader(vshader);
	glDeleteShader(fshader);
	glDeleteProgram(program);
}

void initShader(const char *vname, const char *fname)
{
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

	glUseProgram(program);

	if (uColorTex != -1) glUniform1i(uColorTex, 0);
	if (uEmiTex != -1) glUniform1i(uEmiTex, 1);
}


GLuint loadShader(const char *fileName, GLenum type)
{
	unsigned int fileLen;
	char* source = loadStringFromFile(fileName, fileLen);

	//////////////////////////////////////////////
	//Creación y compilación del Shader
	GLuint shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1,
		(const GLchar**)&source, (const GLint*)&fileLen);
	glCompileShader(shader);
	delete[] source;

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		//Calculamos una cadena de error
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		char* logString = new char[logLen];
		glGetShaderInfoLog(shader, logLen, NULL, logString);
		std::cout << "Error: " << logString << std::endl;
		delete[] logString;
		glDeleteShader(shader);
		exit(-1);
	}

	return shader;
}

void renderFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	glViewport(0, 0, w, h);

	const auto view = camera->get_view_matrix();
	const auto proj = camera->get_projection_matrix();

	glm::mat4 modelView = view * model;
	glm::mat4 modelViewProj = proj * modelView;
	glm::mat4 normal = glm::transpose(glm::inverse(modelView));

	if (uModelViewMat != -1)
		glUniformMatrix4fv(uModelViewMat, 1, GL_FALSE, &(modelView[0][0]));
	if (uModelViewProjMat != -1)
		glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE, &(modelViewProj[0][0]));
	if (uNormalMat != -1)
		glUniformMatrix4fv(uNormalMat, 1, GL_FALSE, &(normal[0][0]));

	//Texturas
	if (uColorTex != -1) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, colorTexId);
	}

	if (uEmiTex != -1) {
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, emiTexId);
	}

	glBindVertexArray(opengl_manager.vao);
	glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3,
		GL_UNSIGNED_INT, (void*)0);

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