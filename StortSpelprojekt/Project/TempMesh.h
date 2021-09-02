#pragma once
#include "Material.h"
#include "TempResources.h"
#include "Vertex.h"
#include "assimp\scene.h"

//MESH
struct TempMesh
{
	std::string name = "";
	UINT bufferID = -1;
	UINT materialID = -1;

	TempMesh(aiMesh* mesh)
		:name(mesh->mName.C_Str())
	{
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

		materialID = TempResources::I().NumMaterials() + mesh->mMaterialIndex;

		//Create Vertex Buffer
	}
};