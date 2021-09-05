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
	particleSystem = nullptr;

	particleSystem = new ParticleSystem(file, true);

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

void ParticleEditor::Initialize(UINT windowWidth, UINT windowHeight)
{
	camera = Camera(PI_DIV4, (float)windowWidth / (float)windowHeight, 0.1f, 20.0f, 0, 0, { -2.5f, 5.0f, -15.0f }, { -2.5f, 0.0f, 0.0f });

	particleSystem = new ParticleSystem("default.ps", true);

	AddWindow("PARTICLE SYSTEM EDITOR");
	auto& window = windows["PARTICLE SYSTEM EDITOR"];

	window.AddTextComponent(loadedParticleSystem);
	window.AddSeperatorComponent();

	window.AddTextComponent("SYSTEM");
	window.AddSliderIntComponent("MAX PARTICLES", 1, particleSystem->ABSOLUTE_MAX_PARTICLES, particleSystem->GetMaxParticles());
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

	window.AddButtonComponent("LOAD", 100, 50);
	window.AddTextComponent("\t\t\t\t\t", true);
	window.AddButtonComponent("SAVE AS", 100, 50, true);
	window.AddSeperatorComponent();

	window.AddButtonComponent("RETURN TO MENU", 120, 30);
}

void ParticleEditor::Update()
{
	if (firstFrame)
		return;

	auto& window = windows["PARTICLE SYSTEM EDITOR"];

	if (window.GetValue<ButtonComponent>("LOAD"))
	{
		Load(FileSystem::LoadFile("ParticleSystems"));
		return;
	}

	else if (window.GetValue<ButtonComponent>("SAVE AS"))
		Save(FileSystem::SaveFile("ParticleSystems"));

	else if (window.GetValue<ButtonComponent>("RETURN TO MENU"))
	{
		done = true;
		return;
	}

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

	particleSystem->Update();

	ShaderData::Inst().Update(camera);
}

void ParticleEditor::Render()
{
	if (firstFrame)
		firstFrame = false;

	BeginFrame();

	renderer.Render(particleSystem);

	EndFrame();
}

void ParticleEditor::Reset()
{
	ClearWindows();
	firstFrame = true;
	done = false;
	delete particleSystem;
	particleSystem = nullptr;
	loadedParticleSystem = "default.ps";
}