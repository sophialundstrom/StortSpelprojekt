#pragma once

#include "Material.h"
#include <map>
#include <array>

class Resources
{
private:
	static ID3D11Buffer* materialBuffer;
	static std::map<std::string, std::map<std::string, Material*>> materials;
public:
	static bool CreateMaterialsFromFile(std::string file, std::string mtllib)
	{
		if (materials.find(mtllib) != materials.end())
			return true;

		std::ifstream reader;

		reader.open("Models/" + file + "/" + mtllib, std::ios::beg);
		if (!reader.is_open())
		{
			Print("FAILED TO LOAD MTL-FILE: " + mtllib);
			return false;
		}

		Material* material = nullptr;
		while (!reader.eof())
		{
			std::string text;
			reader >> text;

			if (text == "newmtl")
			{
				if (material)
					materials[mtllib][material->name] = material;

				if (materials.count(mtllib) == 0)
					material = new Material(0, mtllib);
				else
					material = new Material((UINT)materials.at(mtllib).size(), mtllib);

				reader >> material->name;
			}

			if (text == "Kd")
			{
				Vector4 diffuse;

				reader >> diffuse.x;
				reader >> diffuse.y;
				reader >> diffuse.z;
				diffuse.w = 0;

				if (material)
					material->data.diffuse = diffuse;
			}

			if (text == "Ka")
			{
				Vector4 ambient;

				reader >> ambient.x;
				reader >> ambient.y;
				reader >> ambient.z;
				ambient.w = 0;

				if (material)
					material->data.ambient = ambient;
			}

			if (text == "Ks")
			{
				Vector4 specular;

				reader >> specular.x;
				reader >> specular.y;
				reader >> specular.z;
				specular.w = 0;

				if (material)
					material->data.specular = specular;
			}

			if (text == "Ns")
			{
				float specularPower;
				reader >> specularPower;

				if (material)
					material->data.specularPower = specularPower;
			}

			if (text == "map_Kd")
			{
				std::string fileName;
				reader >> fileName;

				if (material)
					material->diffuseTextures.emplace_back(new Texture("Models/" + file + "/" + fileName, fileName));
			}

			if (text == "")
				if (material)
					materials[mtllib][material->name] = material;
		}

		if (materialBuffer == nullptr)
			CreateBuffer(materialBuffer, sizeof(Material::Data));

		return true;
	}

	static bool MaterialHasDisplacementTexture(std::string mtllib, UINT ID = 0)
	{
		for (auto& [lib, material] : materials)
			if (lib == mtllib)
				for (auto& [name, mat] : material)
					if (mat->displacementTexture)
						return true;
		return false;
	}

	static void BindMaterial(std::string mtllib, UINT ID = 0, UINT slot = 0, Shader shader = Shader::PS)
	{
		for (auto& [lib, material] : materials)
			if (lib == mtllib)
			{
				UpdateBuffer(materialBuffer, std::next(material.begin(), ID)->second->data);
				Graphics::Inst().GetContext().PSSetConstantBuffers(slot, 1, &materialBuffer);
			}
	}

	static void BindMaterialTextures(std::string mtllib, UINT ID = 0, UINT startSlot = 0, Shader shader = Shader::PS)
	{
		for (auto& [lib, material] : materials)
			if (lib == mtllib)
				std::next(material.begin(), ID)->second->BindDiffuseTextures(startSlot, shader);
	}

	static void BindMaterialDisplacementTexture(std::string mtllib, UINT ID = 0, UINT slot = 0, Shader shader = Shader::DS)
	{
		for (auto& [lib, material] : materials)
			if (lib == mtllib)
				std::next(material.begin(), ID)->second->BindDisplacementTexture(slot, shader);
	}

	static int GetMaterialID(std::string mtllib, std::string name)
	{
		for (auto& [lib, material] : materials)
			if (lib == mtllib)
				for (auto& [matName, mat] : material)
					if (name == matName)
						return mat->ID;
		return -1;
	}

	static void AddTexture(std::string mtllib, std::string name, std::string file)
	{
		for (auto& [lib, material] : materials)
			if (lib == mtllib)
				for (auto& [matName, mat] : material)
					mat->diffuseTextures.emplace_back(new Texture("Models/" + name + "/" + file, file));
	}

	static void AddDisplacementTexture(std::string mtllib, std::string name, std::string file)
	{
		for (auto& [lib, material] : materials)
			if (lib == mtllib)
				for (auto& [matName, mat] : material)
					if (!mat->displacementTexture) mat->displacementTexture = new Texture("Models/" + name + "/" + file, file);
	}

	static void ShutDown()
	{
		materialBuffer->Release();

		for (auto& [mtllib, material] : materials)
			for (auto& [name, mat] : material)
				delete mat;

		materials.clear();
	}
};

inline ID3D11Buffer* Resources::materialBuffer;
inline std::map<std::string, std::map<std::string, Material*>> Resources::materials;