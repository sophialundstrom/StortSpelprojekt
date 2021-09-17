#pragma once
#include "Singleton.h"
#include "Print.h"
#include "Material.h"
#include "Vertex.h"
#include <map>

typedef enum { ID_INVALID = 9999 } ID_FLAG;

class Resources : public Singleton<Resources>
{
private:
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0;

	std::shared_ptr<Material> currentMaterial = nullptr;
	ID3D11Buffer* materialBuffer = nullptr;

	ID3D11Buffer* currentVertexBuffer = nullptr;

	std::vector<std::string> bufferNames;

	std::vector<UINT> vertexCounts;

	std::map<UINT, ID3D11Buffer*> vertexBuffers;

	std::map<UINT, std::shared_ptr<Material>> materials;
public:
	Resources()
		:Singleton(this) { CreateBuffer(materialBuffer, sizeof(Material::Data)); }

	~Resources()
	{
		materialBuffer->Release();
		materials.clear();

		for (auto& [ID, buffer] : vertexBuffers)
			buffer->Release();
	}

	//CHECK IF MATERIAL ALREADY EXISTS
	bool MaterialExists(const std::string& name)
	{
		for (auto& [ID, material] : materials)
			if (material->name == name)
				return true;
		return false;
	}

	//NUM MATERIALS
	UINT NumMaterials() const { return (UINT)materials.size(); }

	//NUM BUFFERS
	UINT NumBuffers() const { return (UINT)vertexBuffers.size(); }
	
	//ADD NEW MATERIAL
	void AddMaterial(Material* material) { materials.emplace(NumMaterials(), material); }

	//ADD NEW VERTEX BUFFER
	void AddVertexBuffer(const std::string& name, ID3D11Buffer* buffer, UINT vertexCount) { vertexBuffers.emplace(NumBuffers(), buffer); bufferNames.emplace_back(name); vertexCounts.emplace_back(vertexCount); }

	UINT GetVertexCountFromID(UINT ID) 
	{
		return vertexCounts[ID];
	}

	//GET MATERIAL ID FROM NAME
	UINT GetMaterialIDFromName(const std::string& name)
	{
		for (auto& [ID, material] : materials)
			if (material->name == name)
				return ID;
		return ID_INVALID;
	}

	//GET VERTEX BUFFER ID FROM NAME
	UINT GetBufferIDFromName(const std::string& name)
	{
		for (int i = 0; i < bufferNames.size(); i++)
		{
			if (name == bufferNames[i])
				return i;
		}		
		return ID_INVALID;
	}

	//BIND MATERIAL AT GIVEN INDEX
	void BindMaterial(UINT materialID, bool useMaterial)
	{
		std::shared_ptr<Material> material = materials[materialID];

		if (currentMaterial != material)
		{
			if (useMaterial)
			{
				UpdateBuffer(materialBuffer, material->data);
				Graphics::Inst().GetContext().PSSetConstantBuffers(0, 1, &materialBuffer);
			}
	
			currentMaterial = material;
			material->BindDiffuseTextures();
		}
	}

	//BIND VERTEX BUFFER AND DRAW
	void Draw(UINT vertexCount, UINT bufferID)
	{
		ID3D11Buffer* vertexBuffer = std::next(vertexBuffers.begin(), bufferID)->second;

		if (currentVertexBuffer != vertexBuffer)
			Graphics::Inst().GetContext().IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

		Graphics::Inst().GetContext().Draw(vertexCount, 0);
	}

	//CLEAR
	void Clear()
	{		
		materials.clear();

		for (auto& [ID, buffer] : vertexBuffers)
			buffer->Release();

		vertexBuffers.clear();
	}
};