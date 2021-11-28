#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "OGLObject.hpp"

struct Texture : public OglObject {
	unsigned n_texture;	// offset to acces the Texture Unit (TEXTURE_0)
};


#endif // _TEXTURE_H_