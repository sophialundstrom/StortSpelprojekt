#pragma once

#include "MaterialLoader.h"
#include "TempMesh.h"

#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"

class TempModel : public Drawable
{
private:
	TempMesh mesh;
public:
	TempModel(const std::string& fileName)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile("AssimpModels/" + fileName + ".fbx", aiProcess_Triangulate | aiProcess_SortByPType);
		if (!scene)
		{
			Print("COULD NOT LOAD .FBX FILE");
			return;
		}

		if (scene->HasMeshes())
			mesh = TempMesh(scene->mMeshes[0]);

		if (scene->HasMaterials())
			MaterialLoader::Load(scene->mMaterials[0]);
	}

	TempModel(const TempModel& other)
		:mesh(other.mesh) 
	{
		this->parent = other.parent;
	}

	Matrix Matrix() const { return matrix; }

	void Draw() { TempResources::Inst().BindMaterial(mesh.materialID); TempResources::Inst().Draw(mesh.vertexCount, mesh.bufferID); }

	void ApplyMaterial(const std::string& name)
	{
		UINT ID = TempResources::Inst().GetMaterialIDFromName(name);
		if (ID != ID_INVALID)
			mesh.materialID = ID;
	}

	void ApplyMesh(const std::string& name)
	{
		UINT ID = TempResources::Inst().GetBufferIDFromName(name);
		if (ID != ID_INVALID)
			mesh.bufferID = ID;
	}

	// Inherited via Drawable
	virtual void Update() override
	{
		UpdateMatrix();
	}
};