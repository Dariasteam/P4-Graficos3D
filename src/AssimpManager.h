#ifndef _ASSIMP_MANAGER_H_
#define _ASSIMP_MANAGER_H_

#include "IGlib.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <string>
#include <iostream>

using namespace Assimp;

class AssimpManager {
public:
	static int import_from_file (const std::string& pFile) {
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
			return -1;
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

			float* auxMeshVertexPos = new float[n_vertices * 3];
			float* auxMeshNormals = new float [n_vertices * 3];
			float* auxMeshTangents = new float [n_vertices * 3];
			float* auxMeshvertexColors = new float[n_vertices * 3];
			float* auxMeshTexCoords = new float[n_vertices * 2];

			unsigned* auxMeshFacesIndex = new unsigned[n_faces * 3];

			for (unsigned j = 0; j < n_vertices; j++) {
				const auto& auxVertex = mesh.mVertices[j];
				const auto& auxNormal = mesh.mNormals[j];
				const auto& auxTexCoord = mesh.mTextureCoords[0][j];
				const auto& auxTangent = mesh.mTangents[j];

				auxMeshVertexPos[j * 3 + 0] = auxVertex.x;
				auxMeshVertexPos[j * 3 + 1] = auxVertex.y;
				auxMeshVertexPos[j * 3 + 2] = auxVertex.z;

				auxMeshNormals[j * 3 + 0] = auxNormal.x;
				auxMeshNormals[j * 3 + 1] = auxNormal.y;
				auxMeshNormals[j * 3 + 2] = auxNormal.z;

				auxMeshTangents[j * 3 + 0] = auxTangent.x;
				auxMeshTangents[j * 3 + 1] = auxTangent.y;
				auxMeshTangents[j * 3 + 2] = auxTangent.z;

				auxMeshvertexColors[j * 3 + 0] = 1.f;
				auxMeshvertexColors[j * 3 + 1] = 1.f;
				auxMeshvertexColors[j * 3 + 2] = 1.f;

				auxMeshTexCoords[j * 2 + 0] = auxTexCoord.x;
				auxMeshTexCoords[j * 2 + 1] = auxTexCoord.y;
			}

			for (unsigned j = 0; j < n_faces; j++) {
				const auto& auxFace = mesh.mFaces[j];

				auxMeshFacesIndex[j * 3 + 0] = auxFace.mIndices[0];
				auxMeshFacesIndex[j * 3 + 1] = auxFace.mIndices[1];
				auxMeshFacesIndex[j * 3 + 2] = auxFace.mIndices[2];
			}

			return IGlib::createObj(n_faces,
															n_vertices,
															auxMeshFacesIndex,
															auxMeshVertexPos,
															auxMeshvertexColors,
															auxMeshNormals,
															auxMeshTexCoords,
															auxMeshTangents);
		}

		return true;
	}
};

#endif // _ASSIMP_MANAGER_H_