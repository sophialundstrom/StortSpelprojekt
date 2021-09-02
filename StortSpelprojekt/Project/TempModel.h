#pragma once
#include "Math.h"

#include "Texture.h"
#include "Print.h"
#include "DirectXHelp.h"
#include "TempMesh.h"

#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"

//MODEL
class TempModel
{
private:
	std::vector<TempMesh> meshes;
public:
	TempModel(const std::string fileName)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate);
		if (!scene)
		{
			Print("COULD NOT LOAD .FBX FILE");
			return;
		}

		if (scene->HasMaterials())
			Print("Skapa material");

		if (scene->HasMeshes())
			for (UINT i = 0; i < scene->mNumMeshes; ++i)
				meshes.emplace_back(TempMesh(scene->mMeshes[i]));
	}
};