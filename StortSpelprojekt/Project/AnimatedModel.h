#pragma once
#include "MaterialLoader.h"
#include "Drawable.h"
#include "AnimatedMesh.h"
#include "AnimationStateMachine.h"

#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"
#include "Time.h"

class AnimatedModel : public Drawable
{
protected:
	Assimp::Importer importer;
	const aiScene* scene;

	AnimationStateMachine* animationController;

	AnimatedMesh mesh;
	Skeleton skeleton;
	Animator* animator = nullptr;

	void ProcessAnimations(const std::string& fileName)
	{
		using directory_iterator = std::filesystem::directory_iterator;
		for (const auto& dirEntry : directory_iterator("Models/Animated/" + fileName + "/Clips/"))
		{
			if (dirEntry.path().extension() == ".fbx")
			{
				Assimp::Importer tempImporter;
				const aiScene* tempScene = tempImporter.ReadFile(dirEntry.path().string(), aiProcess_FlipUVs);

				if (tempScene->HasAnimations())
				{
					animator->AddAnimation(tempScene->mAnimations[0]);
				}
			}
		}
	}

public:
	AnimatedModel(const std::string& fileName, const std::string& name)
	{
		SetName(name);
		scene = importer.ReadFile("Models/Animated/" + fileName + "/" + fileName + ".fbx", aiProcess_FlipUVs);

		if (!scene)
		{
			Print("COULD NOT LOAD .FBX FILE");
			return;
		}

		UINT foundID = Resources::Inst().GetBufferIDFromName(scene->mMeshes[0]->mName.C_Str());
		if (foundID == ID_INVALID)
			mesh = AnimatedMesh(scene->mMeshes[0], skeleton, scene);
		else
			ApplyMesh(scene->mMeshes[0]->mName.C_Str());

		const std::string materialName = std::string(scene->mMeshes[0]->mName.C_Str()) + "_" + std::string(scene->mMaterials[0]->GetName().C_Str());
		foundID = Resources::Inst().GetMaterialIDFromName(materialName);
		if (foundID == ID_INVALID)
			MaterialLoader::Load(scene->mMeshes[0]->mName.C_Str(), scene->mMaterials[0]);

		ApplyMaterial(materialName);

		animator = new Animator(scene, skeleton);

		ProcessAnimations(fileName);

		animationController = new AnimationStateMachine(animator, scene, skeleton);
	}
	
	~AnimatedModel() { delete animator; if (animationController) delete animationController; }

	void Draw(bool useTextures = true, bool useMaterial = true) 
	{ 
		animationController->BindMatrices();

		if (useTextures) 
			Resources::Inst().BindMaterial(mesh.materialID, useMaterial); 
		Resources::Inst().DrawAnimated(mesh.vertexCount, mesh.bufferID); 
	}

	void DrawSkeleton()
	{
		skeleton.BindBuffer();
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

	void PlayAnimation(const std::string& animation, bool onRepeat, float speedFactor = 1.f)
	{
		animator->PlayAnimation(animation, onRepeat, speedFactor);
	}

	void PlayAnimation(const std::string& name)
	{
		animationController->PlayAnimation(name);
	}

	void PlayOverrideAnimation(const std::string& name, const std::string& startBone, bool hold, bool fullImpact = false)
	{
		animationController->PlayOverrideAnimation(name, startBone, hold, fullImpact);
	}

	void Update(Camera* camera = nullptr, const std::string& rotationJoint = "")
	{
		UpdateMatrix();

		animationController->Update(skeleton, scene, camera, rotation, rotationJoint);
	}
};