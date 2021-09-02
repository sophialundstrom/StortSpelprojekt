#pragma once
#include "Singleton.h"
#include "Print.h"
#include "DirectXHelp.h"
#include "Material.h"
#include "Vertex.h"
#include <map>

class TempResources : public Singleton<TempResources>
{
private:
	const UINT stride = sizeof(TempVertex);
	const UINT offset = 0;

	ID3D11Buffer* materialBuffer = nullptr;

	std::vector<ID3D11Buffer*> vertexBuffers;

	std::map<UINT, Material> materials;
public:
	TempResources()
		:Singleton(this) {}

	UINT NumMaterials() const { return (UINT)materials.size(); }
	void AddMaterial(Material& material) { materials[0] = material; }

	void Test() { Print(offset); }
};