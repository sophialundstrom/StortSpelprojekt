#include "ParticleEditor.h"
#include <fstream>

void ParticleEditor::Save(const std::string& file)
{
	std::ofstream writer;
	writer.open(file, std::ios::trunc);

	writer << particleSystem->GetMaxParticles();
	writer << " ";
	writer << particleSystem->GetTimeBetweenParticles();
	writer << " ";
	writer << particleSystem->GetParticlesLifetime();
	writer << " ";
	writer << particleSystem->GetMinVelocity();
	writer << " ";
	writer << particleSystem->GetMaxVelocity();
	writer << " ";
	writer << particleSystem->GetSize();
	writer << " ";
	writer << particleSystem->GetParticleWidth();
	writer << " ";
	writer << particleSystem->GetParticleHeight();
	writer << " ";
	writer << particleSystem->GetPosition().x;
	writer << " ";
	writer << particleSystem->GetPosition().y;
	writer << " ";
	writer << particleSystem->GetPosition().z;
	writer << " ";
	writer << (UINT)particleSystem->GetType();
	writer << " ";

	writer.close();
}

void ParticleEditor::Load(const std::string& file)
{
	delete particleSystem;
	particleSystem = new ParticleSystem(file, true);

	auto& window = windows["PARTICLE SYSTEM EDITOR"];

	window.SetValue<SliderIntComponent, int>("MAX PARTICLES", particleSystem->GetMaxParticles());
	window.SetValue<SliderFloatComponent, float>("LIFETIME", particleSystem->GetParticlesLifetime());
	window.SetValue<SliderFloatComponent, float>("DELTA SPAWN", particleSystem->GetTimeBetweenParticles());
	window.SetValue<SliderFloatComponent, float>("SYSTEM SIZE", particleSystem->GetSize());
	window.SetValue<SliderFloatComponent, float>("MIN VELOCITY", particleSystem->GetMinVelocity());
	window.SetValue<SliderFloatComponent, float>("MAX VELOCITY", particleSystem->GetMaxVelocity());
	
	if (particleSystem->GetParticleWidth() == particleSystem->GetParticleHeight())
		window.SetValue<CheckBoxComponent, bool>("KEEP SQUARE", true);
	else
		window.SetValue<CheckBoxComponent, bool>("KEEP SQUARE", false);

	window.SetValue<SliderFloatComponent, float>("PARTICLE WIDTH", particleSystem->GetParticleWidth());
	window.SetValue<SliderFloatComponent, float>("PARTICLE HEIGHT", particleSystem->GetParticleHeight());
	window.SetValue<RadioButtonComponent, UINT>("EMITTER TYPES", (UINT)particleSystem->GetType());
}

void ParticleEditor::Update()
{
	auto& window = windows["PARTICLE SYSTEM EDITOR"];

	if (window.GetValue<ButtonComponent>("LOAD"))
	{
		Load(FileSystem::LoadFile("ParticleSystems"));
		return;
	}

	if (window.GetValue<ButtonComponent>("SAVE"))
		Save(FileSystem::SaveFile("ParticleSystems"));

	if (window.Changed("MAX PARTICLES"))
	{
		particleSystem->SetMaxParticles(window.GetValue<SliderIntComponent>("MAX PARTICLES"));
		particleSystem->Reset();
	}

	else if (window.Changed("LIFETIME"))
	{
		particleSystem->SetParticlesLifetime(window.GetValue<SliderFloatComponent>("LIFETIME"));
		particleSystem->Reset();
	}

	else if (window.Changed("DELTA SPAWN"))
	{
		particleSystem->SetTimeBetweenPartilces(window.GetValue<SliderFloatComponent>("DELTA SPAWN"));
		particleSystem->Reset();
	}

	else if (window.Changed("MIN VELOCITY"))
	{
		float minValue = window.GetValue<SliderFloatComponent>("MIN VELOCITY");
		float maxValue = window.GetValue<SliderFloatComponent>("MAX VELOCITY");

		if (maxValue > minValue)
		{
			particleSystem->SetMinVelocity(minValue);
			particleSystem->Reset();
		}

		else
			window.SetValue<SliderFloatComponent, float>("MIN VELOCITY", maxValue);
	}

	else if (window.Changed("MAX VELOCITY"))
	{
		float minValue = window.GetValue<SliderFloatComponent>("MIN VELOCITY");
		float maxValue = window.GetValue<SliderFloatComponent>("MAX VELOCITY");

		if (minValue < maxValue)
		{
			particleSystem->SetMaxVelocity(maxValue);
			particleSystem->Reset();
		}

		else
			window.SetValue<SliderFloatComponent, float>("MAX VELOCITY", minValue);
	}

	else if (window.Changed("PARTICLE WIDTH"))
	{
		float value = window.GetValue<SliderFloatComponent>("PARTICLE WIDTH");

		if (window.GetValue<CheckBoxComponent>("KEEP SQUARE"))
		{
			particleSystem->SetParticleHeight(value);
			window.SetValue<SliderFloatComponent, float>("PARTICLE HEIGHT", value);
		}

		particleSystem->SetParticleWidth(value);
		particleSystem->Reset();
	}

	else if (window.Changed("PARTICLE HEIGHT"))
	{
		float value = window.GetValue<SliderFloatComponent>("PARTICLE HEIGHT");

		if (window.GetValue<CheckBoxComponent>("KEEP SQUARE"))
		{
			particleSystem->SetParticleWidth(value);
			window.SetValue<SliderFloatComponent, float>("PARTICLE WIDTH", value);
		}

		particleSystem->SetParticleHeight(value);
		particleSystem->Reset();
	}

	else if (window.Changed("SYSTEM SIZE"))
	{
		particleSystem->SetSize(window.GetValue<SliderFloatComponent>("SYSTEM SIZE"));
		particleSystem->Reset();
	}

	else if (window.Changed("EMITTER TYPES"))
	{
		particleSystem->SetType((EmitterType)window.GetValue<RadioButtonComponent>("EMITTER TYPES"));
		particleSystem->Reset();
	}

	if (window.GetValue<ButtonComponent>("RESET"))
		particleSystem->Reset();

	particleSystem->Update();

	ShaderData::Inst().Update(camera);
}