#pragma once
#include "MaterialLoader.h"
#include "Drawable.h"
#include "AnimatedMesh.h"
#include "Animator.h"

#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"
#include "Time.h"

class AnimatedModel : public Drawable
{
private:
	Assimp::Importer importer;
	const aiScene* scene;

	AnimatedMesh mesh;
	Skeleton skeleton;
	Animator animator;
public:
	AnimatedModel(const std::string& fileName)
	{
		Timer timer;
		timer.Start();

		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

		scene = importer.ReadFile("Models/" + fileName + ".fbx", aiProcess_SortByPType | aiProcess_FlipUVs);

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
			for (UINT i = 0; i < scene->mNumAnimations; ++i)
				animator.AddAnimation(scene->mAnimations[i]);
	}

	AnimatedModel(const AnimatedModel& other)
		:mesh(other.mesh)
	{
		this->parent = other.parent;
	}

	void Draw(bool useTextures = true, bool useMaterial = true) 
	{ 
		animator.BindMatricesBuffer();

		if (useTextures) 
			Resources::Inst().BindMaterial(mesh.materialID, useMaterial); 
		Resources::Inst().DrawAnimated(mesh.vertexCount, mesh.bufferID); 
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

	void PlayAnimation(const std::string& animation)
	{
		animator.PlayAnimation(animation);
	}

	// Inherited via Drawable
	virtual void Update() override
	{
		UpdateMatrix();

		animator.Update(scene, skeleton);
	}
};