#ifndef _ASSIMP_MANAGER_H_
#define _ASSIMP_MANAGER_H_

#include "LoadingMesh.hpp"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <string>
#include <iostream>
#include <vector>

using namespace Assimp;

class MeshLoader {
private:
	std::vector<LoadingMesh*> loadable_meshes;

	int insert_mesh_and_get_index (LoadingMesh* mesh) {
		loadable_meshes.push_back(mesh);
		return loadable_meshes.size() - 1;
	}

public:
	const std::vector<LoadingMesh*>& get_meshes() { return loadable_meshes; }

	int import_default_cube () {
		return insert_mesh_and_get_index(new CubeLoadingMesh);
	}

	int import_from_file (const std::string& pFile) {
		// Create an instance of the Importer class
		Assimp::Importer importer;

		// And have it read the given file with some example postprocessing
		// Usually - if speed is not the most important aspect for you - you'll
		// probably to request more postprocessing than we do in this example.
		const aiScene* scene = importer.ReadFile(
			pFile,
			aiProcess_GenNormals						 |
			aiProcess_GenUVCoords						 |
			aiProcess_CalcTangentSpace       |
			aiProcess_RemoveComponent				 |
			aiProcess_Triangulate            |
			aiProcess_JoinIdenticalVertices  |
			aiProcess_SortByPType);

		importer.ApplyPostProcessing(
			aiProcess_CalcTangentSpace
		);

		// If the import failed, report it
		if (nullptr == scene) {
			std::cerr << "Cannot load file " << pFile << std::endl;
			return false;
		}

		for (unsigned i = 0; i < scene->mNumMeshes; i++) {
			aiMesh& mesh = *scene->mMeshes[i];

			unsigned n_vertices = 0;
			unsigned n_faces = 0;

			if (!mesh.HasPositions()) {
				std::cerr << "No positions" << std::endl;
				return -1;
			}

			n_vertices = mesh.mNumVertices;

			if (!mesh.HasFaces()) {
				std::cerr << "No faces" << std::endl;
				return -1;
			}

			n_faces = mesh.mNumFaces;

			if (!mesh.HasNormals()) {
				std::cerr << "No normals" << std::endl;
				return -1;
			}

			if (!mesh.HasTangentsAndBitangents()) {
				std::cerr << "No tangents and bitangents" << std::endl;
				return -1;
			}

			if (!mesh.HasTextureCoords(0)) {
				std::cerr << "No texture coords in channel 0" << std::endl;
				return -1;
			}

			if (!mesh.HasVertexColors(0))
				std::cerr << "No vertex colors in channel 0" << std::endl;

			LoadingMesh* aux_mesh = new RegularLoadingMesh (n_faces, n_vertices);

			for (unsigned j = 0; j < n_vertices; j++) {
				const auto& auxVertex = mesh.mVertices[j];
				const auto& auxNormal = mesh.mNormals[j];
				const auto& auxTexCoord = mesh.mTextureCoords[0][j];
				const auto& auxTangent = mesh.mTangents[j];

				aux_mesh->vertexPos[j * 3 + 0] = auxVertex.x;
				aux_mesh->vertexPos[j * 3 + 1] = auxVertex.y;
				aux_mesh->vertexPos[j * 3 + 2] = auxVertex.z;

				aux_mesh->normals[j * 3 + 0] = auxNormal.x;
				aux_mesh->normals[j * 3 + 1] = auxNormal.y;
				aux_mesh->normals[j * 3 + 2] = auxNormal.z;

				aux_mesh->tangents[j * 3 + 0] = auxTangent.x;
				aux_mesh->tangents[j * 3 + 1] = auxTangent.y;
				aux_mesh->tangents[j * 3 + 2] = auxTangent.z;

				aux_mesh->vertexColors[j * 3 + 0] = 1.f;
				aux_mesh->vertexColors[j * 3 + 1] = 1.f;
				aux_mesh->vertexColors[j * 3 + 2] = 1.f;

				aux_mesh->texCoords[j * 2 + 0] = auxTexCoord.x;
				aux_mesh->texCoords[j * 2 + 1] = auxTexCoord.y;
			}

			for (unsigned j = 0; j < n_faces; j++) {
				const auto& auxFace = mesh.mFaces[j];

				aux_mesh->facesIndex[j * 3 + 0] = auxFace.mIndices[0];
				aux_mesh->facesIndex[j * 3 + 1] = auxFace.mIndices[1];
				aux_mesh->facesIndex[j * 3 + 2] = auxFace.mIndices[2];
			}

			// FIXME: We are getting the first mesh only
			return insert_mesh_and_get_index (aux_mesh);
		}

		return -1;
	}
};

#endif // _ASSIMP_MANAGER_H_