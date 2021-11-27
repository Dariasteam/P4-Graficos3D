#ifndef _OGL_MEHS_
#define _OGL_MEHS_

#include "OGLObject.hpp"

struct OglMesh : public OglObject {
  unsigned n_vertices;
	unsigned n_triangles;

  unsigned gl_draw_offset;
};

#endif // _OGL_MEHS_