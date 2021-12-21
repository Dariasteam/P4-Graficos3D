#ifndef _OGL_MEHS_
#define _OGL_MEHS_

#include "../../ogl/OGLObject.hpp"

struct OglMesh : public OglObject {
  unsigned n_vertices;
	unsigned n_triangles;

  unsigned gl_draw_offset;

  unsigned pos_offset;
  unsigned color_offset;
  unsigned normal_offset;
  unsigned tex_coord_offset;
  unsigned tangent_offset;
};

#endif // _OGL_MEHS_