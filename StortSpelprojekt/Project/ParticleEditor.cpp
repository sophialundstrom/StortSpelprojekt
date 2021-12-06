#include "ParticleEditor.h"
#include <fstream>
#include "Renderers.h"

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
	writer << particleSystem->GetWidth() << space;
	writer << particleSystem->GetDepth() << space;


	writer << particleSystem->GetMinVelocity() << space;
	writer << particleSystem->GetMaxVelocity() << space;

	writer << particleSystem->GetParticleWidth() << space;
	writer << particleSystem->GetParticleHeight() << space;

	writer << (UINT)particleSystem->GetType() << space;

	writer << particleSystem->GetPosition().x << space;
	writer << particleSystem->GetPosition().y << space;
	writer << particleSystem->GetPosition().z << space;

	writer << "'" << particleSystem->GetFirstTextureFile() << "'" << space;
	writer << "'" << particleSystem->GetSecondTextureFile() << "'" << space;
	writer << "'" << particleSystem->GetOpacityTextureFile() << "'";

	writer.close();
}

void ParticleEditor::Load(const std::string& file)
{
	if (file == "" || std::filesystem::path(file).extension() != ".ps")
		return;

	PR->Clear();
	particleSystem = std::make_shared<ParticleSystem>(file, true);
	PR->Bind(particleSystem);

	auto& window = windows["PARTICLE SYSTEM EDITOR"];

	window.SetValue<TextComponent, std::string>(loadedParticleSystem, std::filesystem::path(file).filename().string());
	window.SetValue<SliderIntComponent, int>("MAX PARTICLES", particleSystem->GetMaxParticles());
	window.SetValue<SliderFloatComponent, float>("LIFETIME", particleSystem->GetParticlesLifetime());
	window.SetValue<SliderFloatComponent, float>("DELTA SPAWN", particleSystem->GetTimeBetweenParticles());
	window.SetValue<SliderFloatComponent, float>("SYSTEM SIZE", particleSystem->GetSize());

	window.SetValue<SliderFloatComponent, float>("WIDTH", particleSystem->GetWidth());
	window.SetValue<SliderFloatComponent, float>("DEPTH", particleSystem->GetDepth());

	window.SetValue<SliderFloatComponent, float>("MIN VELOCITY", particleSystem->GetMinVelocity());
	window.SetValue<SliderFloatComponent, float>("MAX VELOCITY", particleSystem->GetMaxVelocity());


	if (particleSystem->GetParticleWidth() == particleSystem->GetParticleHeight())
		window.SetValue<CheckBoxComponent, bool>("KEEP SQUARE", true);
	else
		window.SetValue<CheckBoxComponent, bool>("KEEP SQUARE", false);

	window.SetValue<SliderFloatComponent, float>("PARTICLE WIDTH", particleSystem->GetParticleWidth());
	window.SetValue<SliderFloatComponent, float>("PARTICLE HEIGHT", particleSystem->GetParticleHeight());
	window.SetValue<RadioButtonComponent, UINT>("EMITTER TYPES", (UINT)particleSystem->GetType());

	window.SetValue<ImageComponent, ID3D11ShaderResourceView*>("First Image", particleSystem->GetFirstTexture());
	window.SetValue<ImageComponent, ID3D11ShaderResourceView*>("Second Image", particleSystem->GetSecondTexture());
	window.SetValue<ImageComponent, ID3D11ShaderResourceView*>("Opacity Image", particleSystem->GetOpacityTexture());

	source->SetPosition(particleSystem->GetPosition());

	source->Update();

}

void ParticleEditor::Update()
{
	//if (Event::ScrolledUp())
	//{
	//	Vector3 newPos = { camera->GetPosition().x + 1, camera->GetPosition().y, camera->GetPosition().z };
	//	camera->SetPosition(newPos);
	//}

	//else if (Event::ScrolledDown())
	//{
	//	Vector3 newPos = { camera->GetPosition().x - 1, camera->GetPosition().y, camera->GetPosition().z };

	//	camera->SetPosition(newPos);
	//}

	if (GetAsyncKeyState('W'))
	{
		camera->MoveForward();
	}

	if (GetAsyncKeyState('S'))
	{
		camera->MoveForward(-1);
	}

	if (GetAsyncKeyState('A'))
	{
		camera->MoveRight(-1);
	}

	if (GetAsyncKeyState('D'))
	{
		camera->MoveRight();
	}

	if (Event::RightIsClicked())
	{
		if (Event::ReadRawDelta().y > 0)
			camera->Rotate(0, 3);

		if (Event::ReadRawDelta().y < 0)
			camera->Rotate(0, -3);

		if (Event::ReadRawDelta().x > 0)
			camera->Rotate(3, 0);

		if (Event::ReadRawDelta().x < 0)
			camera->Rotate(-3, 0);
	}

	if (Event::KeyIsPressed(VK_SPACE))
		camera->MoveUp();

	if (Event::KeyIsPressed('Z'))
		camera->MoveUp(-1);

	if (Event::KeyIsPressed(VK_SHIFT))
		camera->SetSpeedMultiplier(3);
	else
		camera->SetSpeedMultiplier(1);


	particleSystem->Update();
	camera->Update();
	Event::ClearRawDelta();
	ShaderData::Inst().Update(*camera);
}

void ParticleEditor::Render()
{
	BeginViewportFrame();

	CR->Render();
	SBR->Render();

	PR->Render();

	BeginFrame();

	ImGui::End();
	ImGui::PopStyleVar();

	EndFrame();
}

ParticleEditor::ParticleEditor(UINT clientWidth, UINT clientHeight)
{
	RND.InitColliderRenderer();
	RND.InitParticleRenderer();
	RND.InitSkyBoxRenderer("ParticleEditor", "ParticleEditor");

	camera = new Camera(PI_DIV4, float(clientWidth) / float(clientHeight), 0.1f, 10000.0f, 0.75f, 2.0f, { 0.0f, 0.0f, -2.5f });
	//camera = new Camera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 200.0f, 0.25f, 2.0f, { -2.5f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f });
	
	source = std::make_shared<BoundingSphere>();
	source->SetScale(0.5f);
	CR->Bind(source);

	AddWindow("PARTICLE SYSTEM EDITOR");
	auto& window = windows["PARTICLE SYSTEM EDITOR"];

	window.AddTextComponent(loadedParticleSystem);
	window.AddSeperatorComponent();

	window.AddTextComponent("SYSTEM");
	window.AddSliderIntComponent("MAX PARTICLES", 1, 1500 /*ParticleSystem::ABSOLUTE_MAX_PARTICLES*/);
	window.AddSliderFloatComponent("DELTA SPAWN");
	window.AddSliderFloatComponent("LIFETIME", 0.0f, 10.0f);
	window.AddSeperatorComponent();
	window.AddTextComponent("CONE AND SPHERE");
	window.AddSliderFloatComponent("SYSTEM SIZE", 0.0f, 50.0f);
	window.AddSeperatorComponent();
	window.AddTextComponent("CUBE EMITTER");
	window.AddSliderFloatComponent("WIDTH", 0.0f, 50.0f);
	window.AddSliderFloatComponent("DEPTH", 0.0f, 50.0f);
	window.AddSeperatorComponent();

	window.AddTextComponent("VELOCITY");
	window.AddSliderFloatComponent("MIN VELOCITY", 0.0f, 50.0f);
	window.AddSliderFloatComponent("MAX VELOCITY", 0.0f, 50.0f);
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

	// CHANGE TEXTURE BUTTON
	window.AddButtonComponent("CHANGE FIRST IMAGE", 100, 50);
	window.AddTextComponent("\t\t\t\t\t", true);
	window.AddImageComponent("First Image", true, nullptr, 75, 75);
	window.AddSeperatorComponent();

	// CHANGE TEXTURE BUTTON
	window.AddButtonComponent("CHANGE SECOND IMAGE", 100, 50);
	window.AddTextComponent("\t\t\t\t\t", true);
	window.AddImageComponent("Second Image", true, nullptr, 75, 75);
	window.AddSeperatorComponent();

	// CHANGE OPACITY BUTTON
	window.AddButtonComponent("CHANGE OPACITY IMAGE", 100, 50);
	window.AddTextComponent("\t\t\t\t\t", true);
	window.AddImageComponent("Opacity Image", true, nullptr, 75, 75);
	window.AddSeperatorComponent();

	window.AddButtonComponent("LOAD", 100, 50);
	window.AddTextComponent("\t\t\t\t\t", true);
	window.AddButtonComponent("SAVE AS", 100, 50, true);
	window.AddSeperatorComponent();

	window.AddButtonComponent("RETURN TO MENU", 120, 30);

	InitCamera(camera);

	Load("default.ps");
	(void)Run();
}

ParticleEditor::~ParticleEditor()
{
	RND.ShutDown();
}

APPSTATE ParticleEditor::Run()
{
	Update();
	Render();

	auto& window = windows["PARTICLE SYSTEM EDITOR"];

	if (window.GetValue<ButtonComponent>("LOAD"))
	{
		Load(FileSystem::LoadFile("ParticleSystems"));
		return APPSTATE::NO_CHANGE;
	}

	else if (window.GetValue<ButtonComponent>("SAVE AS"))
		Save(FileSystem::SaveFile("ParticleSystems"));

	else if (window.GetValue<ButtonComponent>("RETURN TO MENU"))
		return APPSTATE::MAIN_MENU;

	// CHANGE TEXTURE
	else if (window.GetValue<ButtonComponent>("CHANGE FIRST IMAGE"))
	{
		std::filesystem::path filePath = FileSystem::LoadFile("ParticleTextures");

		particleSystem->ChangeFirstTexture(filePath.string(), filePath.filename().string());

		window.SetValue<ImageComponent, ID3D11ShaderResourceView*>("First Image", particleSystem->GetFirstTexture());

		return APPSTATE::NO_CHANGE;
	}

	else if (window.GetValue<ButtonComponent>("CHANGE SECOND IMAGE"))
	{
		std::filesystem::path filePath = FileSystem::LoadFile("ParticleTextures");

		particleSystem->ChangeSecondTexture(filePath.string(), filePath.filename().string());

		window.SetValue<ImageComponent, ID3D11ShaderResourceView*>("Second Image", particleSystem->GetSecondTexture());

		return APPSTATE::NO_CHANGE;
	}

	else if (window.GetValue<ButtonComponent>("CHANGE OPACITY IMAGE"))
	{
		std::filesystem::path filePath = FileSystem::LoadFile("ParticleTextures");

		particleSystem->ChangeOpacityTexture(filePath.string(), filePath.filename().string());

		window.SetValue<ImageComponent, ID3D11ShaderResourceView*>("Opacity Image", particleSystem->GetOpacityTexture());

		return APPSTATE::NO_CHANGE;
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

	else if (window.Changed("WIDTH"))
	{
		particleSystem->SetCubeWidth(window.GetValue<SliderFloatComponent>("WIDTH"));
		particleSystem->Reset();
	}
	else if (window.Changed("DEPTH"))
	{
		particleSystem->SetCubeDepth(window.GetValue<SliderFloatComponent>("DEPTH"));
		particleSystem->Reset();
	}

	else if (window.Changed("EMITTER TYPES"))
	{
		particleSystem->SetType((EmitterType)window.GetValue<RadioButtonComponent>("EMITTER TYPES"));
		particleSystem->Reset();
	}

	if (window.GetValue<ButtonComponent>("RESET"))
		particleSystem->Reset();

	return APPSTATE::NO_CHANGE;
}
