#pragma once
#include "Texture.h"
#include "Math.h"

struct Material
{
	std::string name;

	struct Data
	{
		Vector4 diffuse = { 0.0f, 0.0f, 0.0f, 1.0f };
		Vector4 ambient = { 0.0f, 0.0f, 0.0f, 1.0f };
		Vector4 specular = { 0.0f, 0.0f, 0.0f, 1.0f };
		float specularPower = 1.0f;
		Vector3 padding = { 0.0f, 0.0f, 0.0f };
	} data;

	std::vector<Texture*> diffuseTextures;

	void BindDiffuseTextures(UINT startSlot = 0, Shader shader = Shader::PS)
	{
		UINT slot = startSlot;

		for (auto& texture : diffuseTextures)
		{
			texture->Bind(slot, shader);
			slot++;
		}
	}

	Material() = default;
	~Material()
	{
		for (auto& texture : diffuseTextures)
			delete texture;
	}
};