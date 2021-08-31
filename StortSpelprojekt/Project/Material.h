#pragma once
#include "Texture.h"
#include "Math.h"

struct Material
{
	std::string mtllib;
	std::string name;
	UINT ID;

	struct Data
	{
		Vector4 diffuse = { 0.0f, 0.0f, 0.0f, 1.0f };
		Vector4 ambient = { 0.0f, 0.0f, 0.0f, 1.0f };
		Vector4 specular = { 0.0f, 0.0f, 0.0f, 1.0f };
		float specularPower = 1.0f;
		Vector3 padding = { 0.0f, 0.0f, 0.0f };
	} data;

	std::vector<Texture*> diffuseTextures;

	Texture* displacementTexture = nullptr;

	void BindDiffuseTextures(UINT startSlot = 0, Shader shader = Shader::PS)
	{
		UINT slot = startSlot;

		for (auto& texture : diffuseTextures)
		{
			texture->Bind(slot, shader);
			slot++;
		}
	}

	void BindDisplacementTexture(UINT slot = 0, Shader shader = Shader::DS)
	{
		displacementTexture->Bind(slot, shader);
	}

	Material(UINT ID = 0, std::string mtllib = "")
		:ID(ID), mtllib(mtllib) {}

	~Material()
	{
		for (auto& texture : diffuseTextures)
			delete texture;

		if (displacementTexture) delete displacementTexture;
	}
};