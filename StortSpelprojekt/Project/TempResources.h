#pragma once
#include "Singleton.h"
#include "Print.h"
#include "Material.h"
#include "Vertex.h"
#include <map>

typedef enum { ID_INVALID = 9999 } ID_FLAG;

class TempResources : public Singleton<TempResources>
{
private:
	const UINT stride = sizeof(TempVertex);
	const UINT offset = 0;

	Material* currentMaterial = nullptr;
	ID3D11Buffer* materialBuffer = nullptr;

	ID3D11Buffer* currentVertexBuffer = nullptr;
	std::map<std::string, ID3D11Buffer*> vertexBuffers;

	std::map<UINT, Material*> materials;
public:
	TempResources()
		:Singleton(this) { CreateBuffer(materialBuffer, sizeof(Material::Data)); }

	~TempResources()
	{
		materialBuffer->Release();

		for (auto& [ID, material] : materials)
			delete material;

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
	void AddVertexBuffer(const std::string& name, ID3D11Buffer* buffer) { vertexBuffers[name] = buffer; }

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
		UINT ID = 0;
		for (auto& [n, buffer] : vertexBuffers)
		{
			ID++;
			if (n == name)
				return ID;
		}
					
		return ID_INVALID;
	}

	//BIND MATERIAL AT GIVEN INDEX
	void BindMaterial(UINT materialID)
	{
		Material* material = materials[materialID];

		if (currentMaterial != material)
		{
			UpdateBuffer(materialBuffer, material->data);
			Graphics::Inst().GetContext().PSSetConstantBuffers(0, 1, &materialBuffer);
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
};