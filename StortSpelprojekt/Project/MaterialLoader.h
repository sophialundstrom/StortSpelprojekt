#pragma once
#include "Resources.h"
#include "assimp\scene.h"
#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"

namespace MaterialLoader
{
	inline std::string Load(const std::string& meshName, aiMaterial* material)
	{
		Material* newMaterial = new Material();

		newMaterial->name = meshName + "_" + material->GetName().C_Str();

		aiString path;
		UINT numTextures = material->GetTextureCount(aiTextureType_DIFFUSE);
		if (numTextures > 0)
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				std::string fileName = std::string(path.C_Str()).substr(std::string(path.C_Str()).find_last_of("\\") + 1);
				newMaterial->diffuseTextures.emplace_back(new Texture("Textures/" + fileName, fileName));
			}
			
		float specularPower = 1.0f;
		aiColor3D rgb(0.0f, 0.0f, 0.0f);

		material->Get(AI_MATKEY_COLOR_DIFFUSE, rgb);
		newMaterial->data.diffuse = Vector4(rgb.r, rgb.g, rgb.b, 1.0f);

		material->Get(AI_MATKEY_COLOR_AMBIENT, rgb);
		newMaterial->data.ambient = Vector4(rgb.r, rgb.g, rgb.b, 1.0f);

		material->Get(AI_MATKEY_COLOR_SPECULAR, rgb);
		newMaterial->data.specular = Vector4(rgb.r, rgb.g, rgb.b, 1.0f);

		material->Get(AI_MATKEY_SHININESS, specularPower);
		newMaterial->data.specularPower = specularPower;

		Resources::Inst().AddMaterial(newMaterial);

		return newMaterial->name;
	}
};