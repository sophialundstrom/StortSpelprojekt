#include "ParticleEditor.h"
#include <fstream>

void ParticleEditor::Save(const std::string& file)
{
	if (file == "" || std::filesystem::path(file).extension() != ".ps")
		return;

	std::ofstream writer;
	writer.open(file, std::ios::trunc);

	std::string space = " ";
	writer << particleSystem->GetMaxParticles() << space;
	writer << particleSystem->GetTimeBetweenParticles() << space;
	writer << particleSystem->GetParticlesLifetime() << space;
	writer << particleSystem->GetSize() << space;

	writer << particleSystem->GetMinVelocity() << space;
	writer << particleSystem->GetMaxVelocity() << space;

	writer << particleSystem->GetParticleWidth() << space;
	writer << particleSystem->GetParticleHeight() << space;

	writer << (UINT)particleSystem->GetType() << space;

	writer << particleSystem->GetPosition().x << space;
	writer << particleSystem->GetPosition().y << space;
	writer << particleSystem->GetPosition().z;

	writer.close();
}

void ParticleEditor::Load(const std::string& file)
{
	if (file == "" || std::filesystem::path(file).extension() != ".ps")
		return;

	renderer.Clear();
	particleSystem = std::make_shared<ParticleSystem>(file, true);
	renderer.Bind(particleSystem);

	auto& window = windows["PARTICLE SYSTEM EDITOR"];

	window.SetValue<TextComponent, std::string>(loadedParticleSystem, std::filesystem::path(file).filename().string());
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
	particleSystem->Update();

	ShaderData::Inst().Update(camera);
}

void ParticleEditor::Render()
{
	BeginFrame();

	renderer.Render();

	EndFrame();
}

ParticleEditor::ParticleEditor(UINT clientWidth, UINT clientHeight)
{
	camera = Camera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 20.0f, 0, 0, { -2.5f, 5.0f, -15.0f }, { -2.5f, 0.0f, 0.0f });

	AddWindow("PARTICLE SYSTEM EDITOR");
	auto& window = windows["PARTICLE SYSTEM EDITOR"];

	window.AddTextComponent(loadedParticleSystem);
	window.AddSeperatorComponent();

	window.AddTextComponent("SYSTEM");
	window.AddSliderIntComponent("MAX PARTICLES", 1, ParticleSystem::ABSOLUTE_MAX_PARTICLES);
	window.AddSliderFloatComponent("DELTA SPAWN");
	window.AddSliderFloatComponent("LIFETIME", 0.0f, 10.0f);
	window.AddSliderFloatComponent("SYSTEM SIZE", 0.0f, 50.0f);
	window.AddSeperatorComponent();

	window.AddTextComponent("VELOCITY");
	window.AddSliderFloatComponent("MIN VELOCITY", 0.0f, 100.0f);
	window.AddSliderFloatComponent("MAX VELOCITY", 0.0f, 100.0f);
	window.AddSeperatorComponent();

	window.AddTextComponent("DIMENSIONS");
	window.AddCheckBoxComponent("KEEP SQUARE", true);
	window.AddSliderFloatComponent("PARTICLE WIDTH", 0.0f, 0.5f);
	window.AddSliderFloatComponent("PARTICLE HEIGHT", 0.0f, 0.5f);
	window.AddSeperatorComponent();

	window.AddTextComponent("EMITTER");
	const std::string names[] = { "SPHERE", "CUBE", "CONE" };
	window.AddRadioButtonComponent("EMITTER TYPES", 0, 3, names);
	window.AddSeperatorComponent();

	window.AddTextComponent("IN CASE OF DELTA TIME BUG");
	window.AddButtonComponent("RESET", 50, 20);
	window.AddSeperatorComponent();

	window.AddButtonComponent("LOAD", 100, 50);
	window.AddTextComponent("\t\t\t\t\t", true);
	window.AddButtonComponent("SAVE AS", 100, 50, true);
	window.AddSeperatorComponent();

	window.AddButtonComponent("RETURN TO MENU", 120, 30);

	Load("default.ps");
	(void)Run();
}

State ParticleEditor::Run()
{
	Update();
	Render();

	auto& window = windows["PARTICLE SYSTEM EDITOR"];

	if (window.GetValue<ButtonComponent>("LOAD"))
	{
		Load(FileSystem::LoadFile("ParticleSystems"));
		return State::NO_CHANGE;
	}

	else if (window.GetValue<ButtonComponent>("SAVE AS"))
		Save(FileSystem::SaveFile("ParticleSystems"));

	else if (window.GetValue<ButtonComponent>("RETURN TO MENU"))
		return State::MENU;

	else if (window.Changed("MAX PARTICLES"))
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

	return State::NO_CHANGE;
}