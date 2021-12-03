#ifndef _VBO_MANAGER_
#define _VBO_MANAGER_

#include "../spatial/mesh/OGLMesh.hpp"
#include "../spatial/mesh/LoadingMesh.hpp"

#include <vector>
#include <iostream>

class VBOManager {
public:
	unsigned vao;

	unsigned posVBO;
	unsigned colorVBO;
	unsigned normalVBO;
	unsigned texCoordVBO;
	unsigned triangleIndexVBO;

	std::vector<OglMesh> meshes;

	VBOManager () {}
public:

	inline static VBOManager& get () {
    static VBOManager instance;
    return instance;
  }

  VBOManager (const VBOManager&) = delete;
  void operator= (const VBOManager&) = delete;

  const std::vector<OglMesh>& get_meshes() { return meshes; }

  const unsigned get_vao () { return vao; }

  void generate_VBOs () {
    glGenBuffers(1, &posVBO);
		glGenBuffers(1, &colorVBO);
		glGenBuffers(1, &normalVBO);
		glGenBuffers(1, &texCoordVBO);
		glGenBuffers(1, &triangleIndexVBO);

		glGenVertexArrays(1, &vao);
  	glBindVertexArray(vao);
  }

  ~VBOManager () {
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
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		glBufferData(GL_ARRAY_BUFFER, n_total_vertices * sizeof(float) * 3, nullptr,
								 GL_STATIC_DRAW);
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, n_total_vertices * sizeof(float) * 3, nullptr,
									GL_STATIC_DRAW);
		//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glBufferData(GL_ARRAY_BUFFER, n_total_vertices * sizeof(float) * 2, nullptr,
								 GL_STATIC_DRAW);
		//glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);

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
			glBufferSubData(GL_ARRAY_BUFFER, offset_vertices * 3 * sizeof(float),
											n_vertices * sizeof(float) * 3, vertexCoord);

      // COLOR
      glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
			glBufferSubData(GL_ARRAY_BUFFER, offset_vertices * 3 * sizeof(float),
											n_vertices * sizeof(float) * 3, vertexColors);

      // NORMAL
			glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
			glBufferSubData(GL_ARRAY_BUFFER, offset_vertices * 3 * sizeof(float),
											n_vertices * sizeof(float) * 3, normals);

      // TEX COORDS
			glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
			glBufferSubData(GL_ARRAY_BUFFER, offset_vertices * 2 * sizeof(float),
											n_vertices * sizeof(float) * 2, texCoords);

      // TRIANGLE INDEX
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset_triangles * 3 * sizeof(unsigned),
											n_triangles * sizeof(unsigned) * 3, facesIndex);


      OglMesh ogl_mesh;
      ogl_mesh.n_triangles = n_triangles;
      ogl_mesh.n_vertices = n_vertices;

			ogl_mesh.pos_offset = offset_vertices * 3 * sizeof(float);
			ogl_mesh.color_offset = offset_vertices * 3 * sizeof(float);
			ogl_mesh.normal_offset = offset_vertices * 3 * sizeof(float);
			ogl_mesh.tex_coord_offset = offset_vertices * 2 * sizeof(float);

      ogl_mesh.gl_draw_offset = offset_triangles * 3 * sizeof(unsigned);

      offset_vertices += n_vertices;
			offset_triangles += n_triangles;

      meshes.push_back(ogl_mesh);
		}

		return true;
	}

	void clean () {
		glDeleteBuffers (1, &posVBO);
		glDeleteBuffers (1, &colorVBO);
		glDeleteBuffers (1, &normalVBO);
		glDeleteBuffers (1, &texCoordVBO);
		glDeleteBuffers (1, &triangleIndexVBO);

		glDeleteVertexArrays(1, &vao);

		meshes.clear();
	}

	void upload_attributes_for_mesh(const OglMesh& ogl_mesh) {
		glBindBuffer(GL_ARRAY_BUFFER, posVBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)ogl_mesh.pos_offset);

		glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)ogl_mesh.color_offset);

		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)ogl_mesh.normal_offset);

		glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, (void*)ogl_mesh.tex_coord_offset);
	}
};

#endif // _VBO_MANAGER_