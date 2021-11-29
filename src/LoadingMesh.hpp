#ifndef _LOADING_MESH_
#define _LOADING_MESH_

#include "BOX.h"
#include <algorithm>

struct LoadingMesh {
	unsigned n_triangles;
	unsigned n_vertices;

	float* vertexPos;
	float* normals;
	float* tangents;
	float* vertexColors;
	float* texCoords;

	unsigned* facesIndex;

  LoadingMesh (const unsigned n_t, const unsigned n_v) :
    n_triangles (n_t),
    n_vertices (n_v),
    vertexPos (new float[n_v * 3]),
    normals (new float[n_v * 3]),
    tangents (new float[n_v * 3]),
    vertexColors (new float[n_v * 3]),
    texCoords (new float[n_v * 2]),
    facesIndex (new unsigned[n_t * 3])
  {}

	virtual ~LoadingMesh() {
    delete[] vertexPos;
    delete[] normals;
    delete[] tangents;
    delete[] vertexColors;
    delete[] texCoords;
    delete[] facesIndex;
  };
};

struct CubeLoadingMesh : public LoadingMesh {
	CubeLoadingMesh () :
    LoadingMesh (cubeNTriangleIndex, cubeNVertex)
  {
    std::copy(cubeVertexPos, cubeVertexPos + n_vertices * 3, vertexPos);
    std::copy(cubeVertexNormal, cubeVertexNormal + n_vertices * 3, normals);
    std::copy(cubeVertexTangent, cubeVertexTangent + n_vertices * 3, tangents);
    std::copy(cubeVertexColor, cubeVertexColor + n_vertices * 3, vertexColors);
    std::copy(cubeVertexTexCoord, cubeVertexTexCoord + n_vertices * 2, texCoords);

    std::copy(cubeTriangleIndex, cubeTriangleIndex + n_triangles * 3, facesIndex);
  }

	~CubeLoadingMesh () {}
};

struct RegularLoadingMesh : public LoadingMesh {

  RegularLoadingMesh (const unsigned n_t, const unsigned n_v) :
    LoadingMesh (n_t, n_v) {}
};

#endif
