#pragma once
#include "ForwardParticleRenderer.h"
#include "Editor.h"
#include "FileSystem.h"
#include "GameState.h"

class ParticleEditor : public Editor, public GameState
{
private:
	Camera camera;
	std::string loadedParticleSystem = "default.ps";
	ParticleSystem* particleSystem = nullptr;
	ForwardParticleRenderer renderer;

	// Inherited via Editor
	virtual void Save(const std::string& file) override;
	virtual void Load(const std::string& file) override;
	virtual void Update() override;
	virtual void Render() override;
public:
	ParticleEditor(UINT clientWidth, UINT clientHeight);
	~ParticleEditor() { delete particleSystem; }

	// Inherited via GameState
	State Run();
};