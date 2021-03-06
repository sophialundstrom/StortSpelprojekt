#pragma once
#include "Editor.h"
#include "FileSystem.h"
#include "ApplicationState.h"

class ParticleEditor : public Editor, public ApplicationState
{
private:
	Camera* camera;
	std::string loadedParticleSystem = "default.ps";
	std::shared_ptr<ParticleSystem> particleSystem;

	std::shared_ptr<Model> model;

	std::shared_ptr<BoundingSphere>source;

	std::shared_ptr<Model> terrain;

	Timer timer;
	float FPS = 60.0f;
	float tickInterval = 1.0f / FPS;

	// Inherited via Editor
	virtual void Save(const std::string& file) override;
	virtual void Load(const std::string& file) override;
	void LoadModel(const std::string& file);
	void DeleteModel();
	virtual void Update() override;
	virtual void Render() override;

	bool renderTerrain = true;
	void RenderTerrain()
	{
		bool changed = false;
		if (renderTerrain)
		{
			renderTerrain = false;
			changed = true;
		}

		if(!renderTerrain && !changed)
		{
			renderTerrain = true;
		}
	}

	bool renderModel = true;
	void RenderModel()
	{
		bool changed = false;
		if (renderModel)
		{
			renderModel = false;
			changed = true;
		}
		if (!renderModel && !changed)
		{
			renderModel = true;
		}
	}
	
public:
	ParticleEditor(UINT clientWidth, UINT clientHeight);
	~ParticleEditor();

	// Inherited via GameState
	APPSTATE Run();
};