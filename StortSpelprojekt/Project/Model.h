#pragma once
#include "MaterialLoader.h"
#include "Drawable.h"
#include "Mesh.h"

#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"
#include "Time.h"
#include "FileSystem.h"

class Model : public Drawable
{
	friend class GameLoader;
private:
	Mesh mesh;
public:
	Model() = default;
	Model(const std::string& name, const std::string& path)
		:name(name)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_FlipUVs);

		if (!scene)
		{
			Print("COULD NOT LOAD .FBX FILE");
			return;
		}

		if (scene->HasMeshes())
			ApplyMesh(scene->mMeshes[0]->mName.C_Str());

		if (scene->HasMaterials())
			ApplyMaterial(scene->mMaterials[0]->GetName().C_Str()));
	}

	Model(const Model& other)
		:mesh(other.mesh) 
	{
		this->parent = other.parent;
	}

	void Draw(bool useTextures = true, bool useMaterial = true) 
	{ 
		if (useTextures) 
			Resources::Inst().BindMaterial(mesh.materialID, useMaterial); 

		Resources::Inst().Draw(mesh.vertexCount, mesh.bufferID); 
	}

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
		{
			mesh.bufferID = ID;
			mesh.vertexCount = Resources::Inst().GetVertexCountFromID(ID);
		}
	}

	// Inherited via Drawable
	virtual void Update() override
	{
		UpdateMatrix();
	}
};