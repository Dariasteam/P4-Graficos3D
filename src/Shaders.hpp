#ifndef _SHADERS_H_
#define _SHADERS_H_

#include "OGLObject.hpp"

struct FragmentShader : public OglObject {
	~FragmentShader() {
		glDeleteShader(id);
	}
};

struct VertexShader : public OglObject {
	~VertexShader() {
		glDeleteShader(id);
	}
};

#endif //_SHADERS_H_