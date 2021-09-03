#pragma once
#include "Vertex.h"
#include "TempResources.h"

#include "assimp\scene.h"

struct TempMesh
{
	std::string name = "";

	UINT bufferID = -1;
	UINT materialID = -1;
	UINT vertexCount = -1;

	TempMesh() = default;
	TempMesh(aiMesh* mesh)
		:name(mesh->mName.C_Str())
	{
		auto& resources = TempResources::Inst();
		std::vector<TempVertex> vertices(mesh->mNumVertices);

		for (UINT i = 0; i < mesh->mNumVertices; ++i)
		{
			aiVector3D val;

			val = mesh->mVertices[i];
			vertices[i].position = { val.x, val.y, val.z };

			val = mesh->mNormals[i];
			vertices[i].normal = { val.x, val.y, val.z };

			val = mesh->mTextureCoords[0][i];
			vertices[i].texCoords = { val.x, val.y };
		}

		vertexCount = mesh->mNumVertices;

		bufferID = resources.NumBuffers();

		materialID = resources.NumMaterials();

		ID3D11Buffer* buffer;
		CreateVertexBuffer(buffer, sizeof(TempVertex), sizeof(TempVertex) * vertexCount, vertices.data());

		resources.AddVertexBuffer(mesh->mName.C_Str(), buffer);
	}
};