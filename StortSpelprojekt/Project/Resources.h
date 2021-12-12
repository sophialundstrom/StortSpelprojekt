#pragma once
#include "Singleton.h"
#include "Print.h"
#include "Material.h"
#include "Vertex.h"
#include "AnimatedVertex.h"
#include <map>

typedef enum { ID_INVALID = 9999 } ID_FLAG;

class Resources : public Singleton<Resources>
{
private:
	struct MinMaxBounds
	{
		Vector3 maxBounds;
		Vector3 minBounds;
	};
	std::vector<MinMaxBounds> minMaxBounds;

	const UINT animatedStride = sizeof(AnimatedVertex);
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

	std::shared_ptr<Material> GetMaterialFromID(UINT id) { return materials[id]; }

	//NUM MATERIALS
	UINT NumMaterials() const { return (UINT)materials.size(); }

	//NUM BUFFERS
	UINT NumBuffers() const { return (UINT)vertexBuffers.size(); }
	
	//ADD NEW MATERIAL
	void AddMaterial(Material* material) { materials.emplace(NumMaterials(), material); }

	//ADD NEW VERTEX BUFFER
	void AddVertexBuffer(const std::string& name, ID3D11Buffer* buffer, UINT vertexCount, Vector3 maxBounds, Vector3 minBounds)
	{ 
		vertexBuffers.emplace(NumBuffers(), buffer); 
		bufferNames.emplace_back(name); 
		vertexCounts.emplace_back(vertexCount);
		minMaxBounds.emplace_back(MinMaxBounds{ maxBounds, minBounds });
	}

	Vector3* GetMinMaxBoundsFromID(UINT ID)
	{
		if (vertexBuffers.find(ID) != vertexBuffers.end())
			return (Vector3*)&minMaxBounds[ID];

		return nullptr;
	}

	UINT GetVertexCountFromID(UINT ID) 
	{
		return vertexCounts[ID];
	}

	//GET MATERIAL ID FROM NAME
	UINT GetMaterialIDFromName(const std::string& name)
	{
		for (auto& [ID, material] : materials)
			if (material->name == name || material->name.substr(0, material->name.find_first_of('_')) == name)
				return ID;
		return ID_INVALID;
	}

	std::string GetmaterialnamefromID(UINT ID)
	{
		if (materials.find(ID) != materials.end())
			return materials[ID]->name;

		return "";
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

	std::string GetBufferNameFromID(UINT ID)
	{
		if (vertexBuffers.find(ID) != vertexBuffers.end())
			return bufferNames[ID];

		return "";
	}

	//BIND MATERIAL AT GIVEN INDEX
	void BindMaterial(UINT materialID, bool useMaterial)
	{
		std::shared_ptr<Material> material = materials[materialID];
		if (!material)
			return;

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
		if (bufferID > 999999999)
			return;

		ID3D11Buffer* vertexBuffer = std::next(vertexBuffers.begin(), bufferID)->second;

		if (currentVertexBuffer != vertexBuffer)
			Graphics::Inst().GetContext().IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

		Graphics::Inst().GetContext().Draw(vertexCount, 0);
	}

	void DrawAnimated(UINT vertexCount, UINT bufferID)
	{
		ID3D11Buffer* vertexBuffer = std::next(vertexBuffers.begin(), bufferID)->second;

		if (currentVertexBuffer != vertexBuffer)
			Graphics::Inst().GetContext().IASetVertexBuffers(0, 1, &vertexBuffer, &animatedStride, &offset);

		Graphics::Inst().GetContext().Draw(vertexCount, 0);
	}

	void Reset()
	{
		BindShaders(nullptr, nullptr, nullptr, nullptr, nullptr);
		currentMaterial = nullptr;
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