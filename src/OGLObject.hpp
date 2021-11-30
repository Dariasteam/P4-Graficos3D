#ifndef _OGL_OBJECT_
#define _OGL_OBJECT_

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define SOLVE_FGLUT_WARNING
#define GLM_FORCE_RADIANS

// An object that holds an opengl id

struct OglObject {
	unsigned id;
};

#endif //_OGL_OBJECT_