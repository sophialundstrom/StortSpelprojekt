#pragma once
#include "MaterialLoader.h"
#include "Drawable.h"
#include "Mesh.h"

#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"

class Model : public Drawable
{
private:
	Mesh mesh;
public:
	Model(const std::string& fileName)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile("AssimpModels/" + fileName + ".fbx", aiProcess_Triangulate | aiProcess_SortByPType);
		if (!scene)
		{
			Print("COULD NOT LOAD .FBX FILE");
			return;
		}

		if (scene->HasMeshes())
			mesh = Mesh(scene->mMeshes[0]);

		if (scene->HasMaterials())
			MaterialLoader::Load(scene->mMaterials[0]);
	}

	Model(const Model& other)
		:mesh(other.mesh) 
	{
		this->parent = other.parent;
	}

	void Draw(bool useMaterial = true) { if (useMaterial) Resources::Inst().BindMaterial(mesh.materialID); Resources::Inst().Draw(mesh.vertexCount, mesh.bufferID); }

	void ApplyMaterial(const std::string& name)
	{
		UINT ID = Resources::Inst().GetMaterialIDFromName(name);
		if (ID != ID_INVALID)
			mesh.materialID = ID;
	}

	void ApplyMesh(const std::string& name)
	{
		UINT ID = Resources::Inst().GetBufferIDFromName(name);
		if (ID != ID_INVALID)
			mesh.bufferID = ID;
	}

	// Inherited via Drawable
	virtual void Update() override
	{
		UpdateMatrix();
	}
};