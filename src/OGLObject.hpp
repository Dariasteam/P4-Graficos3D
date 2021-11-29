#ifndef _OGL_OBJECT_
#define _OGL_OBJECT_

#include <GL/glew.h>
#include <GL/freeglut.h>

#define SOLVE_FGLUT_WARNING
#define GLM_FORCE_RADIANS

// An object that holds an opengl id

struct OglObject {
	unsigned id;
};

#endif //_OGL_OBJECT_