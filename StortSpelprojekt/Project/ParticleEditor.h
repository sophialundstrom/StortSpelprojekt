#pragma once
#include "ForwardParticleRenderer.h"
#include "Camera.h"
#include "Editor.h"
#include "FileSystem.h"

class ParticleEditor : public Editor
{
private:
	Camera camera;
	std::string loadedParticleSystem = "default.ps";
	ParticleSystem* particleSystem = nullptr;
	ForwardParticleRenderer renderer;

	// Inherited via Editor
	virtual void Save(const std::string& file) override;
	virtual void Load(const std::string& file) override;
public:
	ParticleEditor() = default;

	// Inherited via Editor
	virtual void Initialize(UINT windowWidth, UINT windowHeight) override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Reset() override;
};