#pragma once
#include "ParticleRenderer.h"
#include "Editor.h"
#include "FileSystem.h"
#include "ApplicationState.h"
#include "ColliderRenderer.h"

class ParticleEditor : public Editor, public ApplicationState
{
private:
	Camera camera;
	std::string loadedParticleSystem = "default.ps";
	std::shared_ptr<ParticleSystem> particleSystem;

	ColliderRenderer colliderRenderer;
	std::shared_ptr<BoundingSphere>source;
	ParticleRenderer renderer;

	// Inherited via Editor
	virtual void Save(const std::string& file) override;
	virtual void Load(const std::string& file) override;
	virtual void Update() override;
	virtual void Render() override;
public:
	ParticleEditor(UINT clientWidth, UINT clientHeight);

	// Inherited via GameState
	APPSTATE Run();
};