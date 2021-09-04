#pragma once
#include "ForwardParticleRenderer.h"
#include "Camera.h"
#include "Editor.h"
#include "FileSystem.h"

class ParticleEditor : public Editor
{
private:
	Camera camera;
	ParticleSystem* particleSystem;
	ForwardParticleRenderer renderer;

	void Save(const std::string& file);
	void Load(const std::string& file);
public:
	ParticleEditor(UINT windowWidth, UINT windowHeight)
		:camera(PI_DIV4, (float)windowWidth / (float)windowHeight, 0.1f, 20.0f, 0, 0, { -2.5f, 5.0f, -15.0f }, { -2.5f, 0.0f, 0.0f })
	{
		particleSystem = new ParticleSystem("default.ps", true);

		AddWindow("PARTICLE SYSTEM EDITOR");
		auto& window = windows["PARTICLE SYSTEM EDITOR"];

		window.AddTextComponent("LOAD PARTICLE SYSTEM");
		window.AddButtonComponent("LOAD", 50, 20);
		window.AddSeperatorComponent();

		window.AddTextComponent("SYSTEM");
		window.AddSliderIntComponent("MAX PARTICLES", 1.0f, particleSystem->ABSOLUTE_MAX_PARTICLES, particleSystem->GetMaxParticles());
		window.AddSliderFloatComponent("LIFETIME", 0.0f, 10.0f, particleSystem->GetParticlesLifetime());
		window.AddSliderFloatComponent("DELTA SPAWN", particleSystem->GetTimeBetweenParticles());
		window.AddSliderFloatComponent("SYSTEM SIZE", 0.0f, 50.0f, particleSystem->GetSize());
		window.AddSeperatorComponent();

		window.AddTextComponent("VELOCITY");
		window.AddSliderFloatComponent("MIN VELOCITY", 0.0f, 500.0f, particleSystem->GetMinVelocity());
		window.AddSliderFloatComponent("MAX VELOCITY", 0.0f, 500.0f, particleSystem->GetMaxVelocity());
		window.AddSeperatorComponent();

		window.AddTextComponent("DIMENSIONS");
		window.AddCheckBoxComponent("KEEP SQUARE", true);
		window.AddSliderFloatComponent("PARTICLE WIDTH", 0.0f, 0.5f, particleSystem->GetParticleWidth());
		window.AddSliderFloatComponent("PARTICLE HEIGHT", 0.0f, 0.5f, particleSystem->GetParticleHeight());
		window.AddSeperatorComponent();

		window.AddTextComponent("EMITTER");
		const std::string names[] = { "SPHERE", "CUBE", "CONE" };
		window.AddRadioButtonComponent("EMITTER TYPES", (UINT)particleSystem->GetType(), 3, names);
		window.AddSeperatorComponent();

		window.AddTextComponent("IN CASE OF DELTA TIME BUG");
		window.AddButtonComponent("RESET", 50, 20);
		window.AddSeperatorComponent();

		window.AddTextComponent("SAVE PARTICLE SYSTEM");
		window.AddButtonComponent("SAVE", 100, 50);
	}

	// Inherited via Editor
	virtual void Update() override;

	virtual void Render() override
	{
		BeginFrame();

		renderer.Render(particleSystem);

		EndFrame();
	}
};