#pragma once
#include "Vertex.h"
#include "Resources.h"

#include "assimp\scene.h"

struct Mesh
{
	//std::string name = "";

	UINT bufferID = -1;
	UINT materialID = -1;
	UINT vertexCount = -1;
	float xMin = 0;
	float xMax = 0;
	float yMin = 0;
	float yMax = 0;
	float zMin = 0;
	float zMax = 0;

	Mesh() = default;
	Mesh(aiMesh* mesh)
	{
		auto& resources = Resources::Inst();
		std::vector<Vertex> vertices(mesh->mNumVertices);

		for (UINT i = 0; i < mesh->mNumVertices; ++i)
		{
			aiVector3D val;

			val = mesh->mVertices[i];
			vertices[i].position = { val.x, val.y, val.z };

			//Collecting minMax values for boundingbox of mesh
			if (val.x < xMin)
				xMin = val.x;
			if (val.x > xMax)
				xMax = val.x;
			if (val.y < yMin)
				yMin = val.y;
			if (val.y > yMax)
				yMax = val.y;
			if (val.z < zMin)
				zMin = val.z;
			if (val.z > zMax)
				zMax = val.z;


			val = mesh->mNormals[i];
			vertices[i].normal = { val.x, val.y, val.z };

			val = mesh->mTextureCoords[0][i];
			vertices[i].texCoords = { val.x, val.y };
		}

		vertexCount = mesh->mNumVertices;

		bufferID = Resources::Inst().NumBuffers();

		materialID = Resources::Inst().NumMaterials();

		ID3D11Buffer* buffer;
		CreateVertexBuffer(buffer, sizeof(Vertex), sizeof(Vertex) * vertexCount, vertices.data());

		Resources::Inst().AddVertexBuffer(mesh->mName.C_Str(), buffer, vertexCount);
	}
};