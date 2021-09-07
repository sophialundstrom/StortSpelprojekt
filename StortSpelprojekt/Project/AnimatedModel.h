#pragma once
#include "MaterialLoader.h"
#include "Drawable.h"
#include "AnimatedMesh.h"

#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"
#include "Time.h"

class AnimatedModel : public Drawable
{
private:
	AnimatedMesh mesh;
	Skeleton skeleton;
public:
	AnimatedModel(const std::string& fileName)
	{
		Timer timer;
		timer.Start();

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile("Models/" + fileName + ".fbx", aiProcess_SortByPType);
		if (!scene)
		{
			Print("COULD NOT LOAD .FBX FILE");
			return;
		}

		if (scene->HasMeshes())
			mesh = AnimatedMesh(scene->mMeshes[0], skeleton);

		if (scene->HasMaterials())
			MaterialLoader::Load(scene->mMaterials[0]);

		if (scene->HasAnimations())


		Print(timer.DeltaTime());
	}

	AnimatedModel(const AnimatedModel& other)
		:mesh(other.mesh)
	{
		this->parent = other.parent;
	}

	void Draw(bool useTextures = true, bool useMaterial = true) 
	{ 
		if (useTextures) Resources::Inst().BindMaterial(mesh.materialID, useMaterial); 
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
			mesh.bufferID = ID;
	}

	// Inherited via Drawable
	virtual void Update() override
	{
		UpdateMatrix();
	}
};