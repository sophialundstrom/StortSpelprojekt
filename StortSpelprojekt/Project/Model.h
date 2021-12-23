#pragma once
#include "MaterialLoader.h"
#include "Drawable.h"
#include "Mesh.h"

#include "assimp\Importer.hpp"
#include "assimp\postprocess.h"
#include "Time.h"
#include "FileSystem.h"
#include "Audio.h"
#include "SoundEffect.h"

class Model : public Drawable
{
	friend class GameLoader;
	friend class LevelEditor;
private:
	Mesh mesh;
	bool overrideModelsToCube = true;
public:
	Model() = default;
	Model(const std::string& meshName, const std::string name)
	{
		SetName(name);
		ApplyMesh(meshName);	
		ApplyMaterial(meshName);
	}

	Model(const std::string& name, const Model& other)
		:mesh(other.mesh) 
	{
		SetName(name);
		position = other.position;
		rotation = other.rotation;
		scale = other.scale;
	}

	void Draw(bool useTextures = true, bool useMaterial = true) 
	{ 
		if (useTextures) 
			Resources::Inst().BindMaterial(mesh.materialID, useMaterial); 

		Resources::Inst().Draw(mesh.vertexCount, mesh.bufferID); 
	}

	void ApplyMaterial(const std::string& name)
	{
		UINT ID;
		std::string altName = "debugCube";
		if (overrideModelsToCube)
			ID = Resources::Inst().GetMaterialIDFromName(altName);
		else
			ID = Resources::Inst().GetMaterialIDFromName(name);

		if (ID != ID_INVALID)
			mesh.materialID = ID;
	}

	void ApplyMesh(const std::string& name)
	{
		UINT ID;
		std::string altName = "debugCube";
		if(overrideModelsToCube)
			ID = Resources::Inst().GetBufferIDFromName(altName);
		else
			ID = Resources::Inst().GetBufferIDFromName(name);

		if (ID != ID_INVALID) 
		{
			mesh.bufferID = ID;
			mesh.vertexCount = Resources::Inst().GetVertexCountFromID(ID);
		}
	}

	Vector3* GetMeshBoundingBoxValues()
	{
		return Resources::Inst().GetMinMaxBoundsFromID(mesh.bufferID);
	}

	// Inherited via Drawable
	virtual void Update() override
	{
		UpdateMatrix();
	}
};