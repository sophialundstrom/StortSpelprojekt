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
	Model(const std::string& fileName)
	{
		Timer timer;
		timer.Start();

		Assimp::Importer importer;
		std::filesystem::current_path(std::filesystem::path(FileSystem::ProjectDirectory::path));
		const aiScene* scene = importer.ReadFile("Models/" + fileName + ".fbx", aiProcess_SortByPType | aiProcess_FlipUVs);
		if (!scene)
		{
			Print("COULD NOT LOAD .FBX FILE");
			return;
		}

		if (scene->HasMeshes())
			mesh = Mesh(scene->mMeshes[0]);

		if (scene->HasMaterials())
			MaterialLoader::Load(scene->mMaterials[0]);


		Print(timer.DeltaTime());
	}

	Model(const Model& other)
		:mesh(other.mesh) 
	{
		this->parent = other.parent;
	}

	void Draw(bool useTextures = true, bool useMaterial = true) { if (useTextures) Resources::Inst().BindMaterial(mesh.materialID, useMaterial); Resources::Inst().Draw(mesh.vertexCount, mesh.bufferID); }

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