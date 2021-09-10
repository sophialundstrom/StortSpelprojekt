#pragma once
#include "ParticleRenderer.h"
#include "Editor.h"
#include "FileSystem.h"
#include "GameState.h"

class ParticleEditor : public Editor, public GameState
{
private:
	Camera camera;
	std::string loadedParticleSystem = "default.ps";
	std::shared_ptr<ParticleSystem> particleSystem;

	ParticleRenderer renderer;

	// Inherited via Editor
	virtual void Save(const std::string& file) override;
	virtual void Load(const std::string& file) override;
	virtual void Update() override;
	virtual void Render() override;
public:
	ParticleEditor(UINT clientWidth, UINT clientHeight);

	// Inherited via GameState
	State Run();
};