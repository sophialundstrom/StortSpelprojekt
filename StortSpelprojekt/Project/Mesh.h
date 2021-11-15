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
	Vector3 vertMinVals = {0, 0, 0};
	Vector3 vertMaxVals = { 0, 0, 0 };

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

			//Xvalues
			if (val.x > vertMaxVals.x)
				vertMaxVals.x = val.x;
			if (val.x < vertMinVals.x)
				vertMinVals.x = val.x;

			//Yvalues
			if (val.y > vertMaxVals.y)
				vertMaxVals.y = val.y;
			if (val.y < vertMinVals.y)
				vertMinVals.y = val.y;

			//Zvalues
			if (val.z > vertMaxVals.z)
				vertMaxVals.z = val.z;
			if (val.z < vertMinVals.z)
				vertMinVals.z = val.z;


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