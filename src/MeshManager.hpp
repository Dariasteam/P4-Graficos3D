#ifndef _MESH_MANAGER_
#define _MESH_MANAGER_

#include "OGLMesh.hpp"
#include "LoadingMesh.hpp"

#include <vector>
#include <iostream>

class MeshManager {
public:
	unsigned vao;

	unsigned posVBO;
	unsigned colorVBO;
	unsigned normalVBO;
	unsigned texCoordVBO;
	unsigned triangleIndexVBO;

	std::vector<OglMesh> meshes;
public:
  const std::vector<OglMesh>& get_meshes() { return meshes; }

  void generate_VBOs () {
    glGenBuffers(1, &posVBO);
		glGenBuffers(1, &colorVBO);
		glGenBuffers(1, &normalVBO);
		glGenBuffers(1, &texCoordVBO);
		glGenBuffers(1, &triangleIndexVBO);

		glGenVertexArrays(1, &vao);
  	glBindVertexArray(vao);
  }

  ~MeshManager () {
    std::cout << "Closing VBOs" << std::endl;
    glDeleteBuffers(1, &posVBO);
    glDeleteBuffers(1, &colorVBO);
    glDeleteBuffers(1, &normalVBO);
    glDeleteBuffers(1, &texCoordVBO);
    glDeleteBuffers(1, &triangleIndexVBO);
    glDeleteVertexArrays(1, &vao);
  }

	bool populate_VBOs (const std::vector<LoadingMesh*>& loadable_meshes) {

		unsigned n_total_vertices{0};
		unsigned n_total_triangles{0};

		// Get buffers sizes

		for (const LoadingMesh* aux_mesh : loadable_meshes) {
			n_total_vertices += aux_mesh->n_vertices;
			n_total_triangles += aux_mesh->n_triangles;
		}

		// Generate buffers for all meshes

		glBindBuffer(GL_ARRAY_BUFFER, posVBO);
		glBufferData(GL_ARRAY_BUFFER, n_total_vertices * sizeof(float) * 3, nullptr,
								 GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		glBufferData(GL_ARRAY_BUFFER, n_total_vertices * sizeof(float) * 3, nullptr,
								 GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, n_total_vertices * sizeof(float) * 3, nullptr,
									GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, n_total_vertices * sizeof(float) * 2, nullptr,
								 GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_total_triangles * sizeof(unsigned) * 3,
								 NULL, GL_STATIC_DRAW);

		// Populate buffers with meshes data

		unsigned offset_triangles {0};
		unsigned offset_vertices {0};

		for (const LoadingMesh* aux_mesh : loadable_meshes) {
			// Initialization

			const unsigned n_vertices = aux_mesh->n_vertices;
			const unsigned n_triangles = aux_mesh->n_triangles;

			const float* vertexCoord = aux_mesh->vertexPos;
			const float* normals = aux_mesh->normals;
			const float* tangents = aux_mesh->tangents;
			const float* vertexColors = aux_mesh->vertexColors;
			const float* texCoords = aux_mesh->texCoords;

			const unsigned* facesIndex = aux_mesh->facesIndex;

			// Populate

      // POS
			glBindBuffer(GL_ARRAY_BUFFER, posVBO);
			glBufferSubData(GL_ARRAY_BUFFER, offset_vertices * 3,
											n_vertices * sizeof(float) * 3, vertexCoord);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

      // COLOR
      glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
			glBufferSubData(GL_ARRAY_BUFFER, offset_vertices * 3,
											n_vertices * sizeof(float) * 3, vertexColors);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

      // NORMAL
			glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
			glBufferSubData(GL_ARRAY_BUFFER, offset_vertices * 3,
											n_vertices * sizeof(float) * 3, normals);
      glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

      // TEX COORDS
			glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
			glBufferSubData(GL_ARRAY_BUFFER, offset_vertices * 2,
											n_vertices * sizeof(float) * 2, texCoords);
			glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);

      // TRIANGLE INDEX
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset_triangles * 3,
											n_triangles * sizeof(unsigned) * 3, facesIndex);

			offset_vertices += n_vertices;
			offset_triangles += n_triangles;

      OglMesh ogl_mesh;
      ogl_mesh.n_triangles = n_triangles;
      ogl_mesh.n_vertices = n_vertices;
      meshes.push_back(ogl_mesh);
		}
		return true;
	}
};

#endif // _MESH_MANAGER_