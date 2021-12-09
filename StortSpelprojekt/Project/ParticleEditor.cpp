#include "ParticleEditor.h"
#include <fstream>
#include "Renderers.h"
#include "FBXLoader.h"

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
	writer << particleSystem->GetRotation() << space;
	writer << particleSystem->GetMinRotation() << space;
	writer << particleSystem->GetMaxRotation() << space;

	writer << particleSystem->GetAlphaMode() << space;

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

	{
		auto& window = windows["PARTICLE SYSTEM EDITOR"];

		window.SetValue<TextComponent, std::string>(loadedParticleSystem, std::filesystem::path(file).filename().string());
		window.SetValue<SliderIntComponent, int>("MAX PARTICLES", particleSystem->GetMaxParticles());
		window.SetValue<SliderFloatComponent, float>("LIFETIME", particleSystem->GetParticlesLifetime());
		window.SetValue<SliderFloatComponent, float>("DELTA SPAWN", particleSystem->GetTimeBetweenParticles());
		window.SetValue<SliderFloatComponent, float>("SYSTEM SIZE", particleSystem->GetSize());
		window.SetValue<SliderFloatComponent, float>("WIDTH", particleSystem->GetWidth());
		window.SetValue<SliderFloatComponent, float>("DEPTH", particleSystem->GetDepth());
		window.SetValue<SliderFloatComponent, float>("X-POS", particleSystem->GetPosition().x);
		window.SetValue<SliderFloatComponent, float>("Y-POS", particleSystem->GetPosition().y);
		window.SetValue<SliderFloatComponent, float>("Z-POS", particleSystem->GetPosition().z);
		window.SetValue<CheckBoxComponent, int>("USE ALPHA", particleSystem->GetAlphaMode());
		window.SetValue<CheckBoxComponent, int>("USE OPACITY", particleSystem->GetOpacityMode());
		window.SetValue<ImageComponent, ID3D11ShaderResourceView*>("First Image", particleSystem->GetFirstTexture());
		window.SetValue<ImageComponent, ID3D11ShaderResourceView*>("Second Image", particleSystem->GetSecondTexture());
		window.SetValue<ImageComponent, ID3D11ShaderResourceView*>("Opacity Image", particleSystem->GetOpacityTexture());
		window.SetValue<RadioButtonComponent, UINT>("EMITTER TYPES", (UINT)particleSystem->GetType());
		window.SetValue<CheckBoxComponent, bool>("Up", particleSystem->GetParticleDir());
	}
	

	{
		auto& window = windows["PER PARTICLE"];
	

		window.SetValue<SliderFloatComponent, float>("MIN", particleSystem->GetMinVelocity());
		window.SetValue<SliderFloatComponent, float>("MAX", particleSystem->GetMaxVelocity());


		if (particleSystem->GetParticleWidth() == particleSystem->GetParticleHeight())
			window.SetValue<CheckBoxComponent, bool>("KEEP SQUARE", true);
		else
			window.SetValue<CheckBoxComponent, bool>("KEEP SQUARE", false);

		window.SetValue<SliderFloatComponent, float>("WIDTH", particleSystem->GetParticleWidth());
		window.SetValue<SliderFloatComponent, float>("HEIGHT", particleSystem->GetParticleHeight());

		window.SetValue<CheckBoxComponent, bool>("ROTATION", particleSystem->GetRotation());
		window.SetValue<SliderFloatComponent, float>("R-MIN", particleSystem->GetMinRotation());
		window.SetValue<SliderFloatComponent, float>("R-MAX", particleSystem->GetMaxRotation());

	}
	
	source->SetPosition(particleSystem->GetPosition());

	source->Update();

}

void ParticleEditor::LoadModel(const std::string& file)
{
	
	std::filesystem::path path = std::filesystem::path(file);
	if (file == "" || path.extension() != ".fbx")
		return;

	if (model)
	{
		MR->Unbind(model);
		scene.DeleteDrawable(model->GetName());
		model.reset();
	}

	std::string fileName = path.stem().string();
	fileName = scene.AddModel(fileName, path.string());
	model = scene.Get<Model>(fileName);
	model->SetPosition({ 0.0f, -1.5f, 0.0f });
	model->Update();
	MR->Bind(model);
	auto &window = windows["MODEL"];
	window.SetValue<TextComponent, std::string>("MODEL NAME", model->GetName());
}

void ParticleEditor::DeleteModel()
{
	auto& window = windows["MODEL"];
	window.SetValue<TextComponent, std::string>("MODEL NAME", "");
	MR->Unbind(model);
	scene.DeleteDrawable(model->GetName());
	model.reset();
}

void ParticleEditor::Update()
{
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

	source->SetPosition(particleSystem->GetPosition());
	source->Update();

	particleSystem->Update();
	camera->Update();
	Event::ClearRawDelta();
	ShaderData::Inst().Update(*camera);
}

void ParticleEditor::Render()
{
	BeginViewportFrame();

	ShaderData::Inst().BindFrameConstants();

	MR->Render();

	CR->Render();

	SBR->Render();

	PR->Render();

	BeginFrame();

	ImGui::End();
	ImGui::PopStyleVar();

	EndFrame();
	Resources::Inst().Reset();
}

ParticleEditor::ParticleEditor(UINT clientWidth, UINT clientHeight)
{
	RND.InitColliderRenderer();
	RND.InitParticleRenderer();
	RND.InitSkyBoxRenderer("ParticleEditor", "ParticleEditor");
	RND.InitModelRenderer(false);

	camera = new Camera(PI_DIV4, float(clientWidth) / float(clientHeight), 0.1f, 10000.0f, 0.75f, 2.0f, { 0.0f, 0.0f, -2.5f });
	//camera = new Camera(PI_DIV4, (float)clientWidth / (float)clientHeight, 0.1f, 200.0f, 0.25f, 2.0f, { -2.5f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f });

	FBXLoader meshLoader("Models");
	auto terrain = std::make_shared<Model>("MainMenuTerrain", "");
	terrain->SetPosition({ 0.0f,-2.0f,0.0f });
	terrain->Update();
	MR->Bind(terrain);

	source = std::make_shared<BoundingSphere>();
	source->SetScale(0.5f);
	CR->Bind(source);

	{
		AddWindow("MODEL");
		auto& window = windows["MODEL"];
		window.AddTextComponent("MODEL");
		window.AddTextComponent("MODEL NAME");
		window.SetValue<TextComponent, std::string>("MODEL NAME", "");
		window.AddTextComponent("FPS");
		window.AddButtonComponent("LOAD FBX", 120, 30);
		window.AddButtonComponent("DELETE", 120, 30);
		window.AddCheckBoxComponent("RENDER MODEL", true);
	}

	{
		AddWindow("PARTICLE SYSTEM EDITOR");
		auto& window = windows["PARTICLE SYSTEM EDITOR"];

		window.AddTextComponent(loadedParticleSystem);
		window.AddSeperatorComponent();
		window.AddTextComponent("                     SYSTEM");
		window.AddSliderIntComponent("MAX PARTICLES", 1, 1500 /*ParticleSystem::ABSOLUTE_MAX_PARTICLES*/);
		window.AddSliderFloatComponent("DELTA SPAWN");
		window.AddSliderFloatComponent("LIFETIME", 0.0f, 10.0f);
		window.AddSliderFloatComponent("X-POS", -20.0f, 20.0f, 0.0f);
		window.AddSliderFloatComponent("Y-POS", -20.0f, 20.0f, 0.0f);
		window.AddSliderFloatComponent("Z-POS", -20.0f, 20.0f, 0.0f);
		window.AddSeperatorComponent();
		
		window.AddTextComponent("                     EMITTERS");
		const std::string names[] = { "SPHERE", "CUBE", "CONE" };
		window.AddRadioButtonComponent("EMITTER TYPES", 0, 3, names, true);
		window.AddSeperatorComponent();

		window.AddTextComponent("                  CONE AND SPHERE");
		window.AddSliderFloatComponent("SYSTEM SIZE", 0.0f, 50.0f);
		window.AddSeperatorComponent();

		window.AddTextComponent("                   CUBE EMITTER");
		window.AddSliderFloatComponent("WIDTH", 0.0f, 50.0f);
		window.AddSliderFloatComponent("DEPTH", 0.0f, 50.0f);
		window.AddTextComponent("                   DIRECTION");
		window.AddTextComponent("|| Checked = Up --- Unchecked = Down ||");
		window.AddCheckBoxComponent("Up", false, false);
		window.AddSeperatorComponent();

		window.AddTextComponent("  |||| -----> IN CASE OF DELTA TIME BUG <----- ||||");
		window.AddButtonComponent("RESET", 380, 20, false);
		window.AddSeperatorComponent();

		// CHANGE TEXTURE BUTTON
		window.AddTextComponent("   ||| ------> TURN ON/OFF ALPHA & OPACITY <------ |||");
		window.AddCheckBoxComponent("USE ALPHA", 1, false);
		window.AddCheckBoxComponent("USE OPACITY", 1, true);
		window.AddTextComponent("   |||| ---------> CHANGE TEXTURES <--------- ||||");
		window.AddButtonComponent("CHANGE FIRST", 200, 75);
		window.AddTextComponent("\t\t\t", true);
		window.AddImageComponent("First Image", true, nullptr, 75, 75);

		// CHANGE TEXTURE BUTTON
		window.AddButtonComponent("CHANGE SECOND", 200, 75);
		window.AddTextComponent("\t\t\t", true);
		window.AddImageComponent("Second Image", true, nullptr, 75, 75);

		// CHANGE OPACITY BUTTON
		window.AddButtonComponent("CHANGE OPACITY", 200, 75);
		window.AddTextComponent("\t\t\t", true);
		window.AddImageComponent("Opacity Image", true, nullptr, 75, 75);
		window.AddSeperatorComponent();

		window.AddButtonComponent("LOAD", 185, 50);
		//window.AddTextComponent("\t\t\t\t\t", true);
		window.AddButtonComponent("SAVE AS", 185, 50, true);
		window.AddSeperatorComponent();

		window.AddButtonComponent("RETURN TO MENU", 380, 70);
	}

	{
		AddWindow("PER PARTICLE");
		auto& window = windows["PER PARTICLE"];
		window.AddTextComponent("     VELOCITY");
		window.AddSliderFloatComponent("MIN", 0.0f, 50.0f);
		window.AddSliderFloatComponent("MAX", 0.0f, 50.0f);
		window.AddSeperatorComponent();

		window.AddTextComponent("    DIMENSIONS");
		window.AddCheckBoxComponent("KEEP SQUARE", true);
		window.AddSliderFloatComponent("WIDTH", 0.0f, 0.5f);
		window.AddSliderFloatComponent("HEIGHT", 0.0f, 0.5f);
		window.AddSeperatorComponent();

		window.AddTextComponent("    PARTICLE ROTATION");
		window.AddCheckBoxComponent("ROTATION", false);
		window.AddSliderFloatComponent("R-MIN", 0.0f, 5.0f, 0.0f);
		window.AddSliderFloatComponent("R-MAX", 0.0f, 5.0f, 0.0f);

	}
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
	timer.Start();

	Update();
	Render();

	{
		auto& window = windows["MODEL"];
		static int frames = 0;
		static float time = 0.0f;
		time += Time::GetDelta();
		frames++;
		if (time >= 1)
		{
			window.SetValue<TextComponent, std::string>("FPS", std::to_string(frames));
			frames = 0;
			time = 0.0f;
		}

		if (window.GetValue<ButtonComponent>("LOAD FBX"))
			LoadModel(FileSystem::LoadFile("Models"));

		if (window.GetValue<ButtonComponent>("DELETE"))
			DeleteModel();

		if (window.Changed("RENDER MODEL"))
			RenderModel();
	}
	
	{
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
		else if (window.GetValue<ButtonComponent>("CHANGE FIRST"))
		{
			std::filesystem::path filePath = FileSystem::LoadFile("ParticleTextures");

			particleSystem->ChangeFirstTexture(filePath.string(), filePath.filename().string());

			window.SetValue<ImageComponent, ID3D11ShaderResourceView*>("First Image", particleSystem->GetFirstTexture());

			return APPSTATE::NO_CHANGE;
		}

		else if (window.GetValue<ButtonComponent>("CHANGE SECOND"))
		{
			std::filesystem::path filePath = FileSystem::LoadFile("ParticleTextures");

			particleSystem->ChangeSecondTexture(filePath.string(), filePath.filename().string());

			window.SetValue<ImageComponent, ID3D11ShaderResourceView*>("Second Image", particleSystem->GetSecondTexture());

			return APPSTATE::NO_CHANGE;
		}

		else if (window.GetValue<ButtonComponent>("CHANGE OPACITY"))
		{
			std::filesystem::path filePath = FileSystem::LoadFile("ParticleTextures");

			particleSystem->ChangeOpacityTexture(filePath.string(), filePath.filename().string());

			window.SetValue<ImageComponent, ID3D11ShaderResourceView*>("Opacity Image", particleSystem->GetOpacityTexture());

			return APPSTATE::NO_CHANGE;
		}

		if (window.GetValue<ButtonComponent>("RESET"))
			particleSystem->Reset();

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

		else if (window.Changed("X-POS"))
		{
			particleSystem->SetXPosition(window.GetValue<SliderFloatComponent>("X-POS"));
			particleSystem->Reset();
		}

		else if (window.Changed("Y-POS"))
		{
			particleSystem->SetYPosition(window.GetValue<SliderFloatComponent>("Y-POS"));
			particleSystem->Reset();
		}

		else if (window.Changed("Z-POS"))
		{
			particleSystem->SetZPosition(window.GetValue<SliderFloatComponent>("Z-POS"));
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

		else if (window.Changed("Up"))
		{
			auto value = window.GetValue<CheckBoxComponent>("Up");
			particleSystem->SetParticleDir(value);
			particleSystem->Reset();
		}

		else if (window.Changed("EMITTER TYPES"))
		{
			particleSystem->SetType((EmitterType)window.GetValue<RadioButtonComponent>("EMITTER TYPES"));
			particleSystem->Reset();
		}

		else if (window.Changed("USE ALPHA"))
		{
			auto value = window.GetValue<CheckBoxComponent>("USE ALPHA");
			if (value == true)
			{
				value = 1;
			}
			else
				value = 0;
			particleSystem->SetAlphaMode(value);
			particleSystem->Reset();
		}

		else if (window.Changed("USE OPACITY"))
		{
			auto value = window.GetValue<CheckBoxComponent>("USE OPACITY");
			if (value == true)
			{
				value = 1;
			}
			else
				value = 0;
			particleSystem->SetOpacityMode(value);
			particleSystem->Reset();
		}
	}

	{
		auto& window = windows["PER PARTICLE"];
		if (window.Changed("MIN"))
		{
			float minValue = window.GetValue<SliderFloatComponent>("MIN");
			float maxValue = window.GetValue<SliderFloatComponent>("MAX");

			if (maxValue > minValue)
			{
				particleSystem->SetMinVelocity(minValue);
				particleSystem->Reset();
			}

			else
				window.SetValue<SliderFloatComponent, float>("MIN", maxValue);
		}

		else if (window.Changed("MAX"))
		{
			float minValue = window.GetValue<SliderFloatComponent>("MIN");
			float maxValue = window.GetValue<SliderFloatComponent>("MAX");

			if (minValue < maxValue)
			{
				particleSystem->SetMaxVelocity(maxValue);
				particleSystem->Reset();
			}

			else
				window.SetValue<SliderFloatComponent, float>("MAX", minValue);
		}

		else if (window.Changed("WIDTH"))
			{
			float value = window.GetValue<SliderFloatComponent>("WIDTH");

			if (window.GetValue<CheckBoxComponent>("KEEP SQUARE"))
			{
				particleSystem->SetParticleHeight(value);
				window.SetValue<SliderFloatComponent, float>("HEIGHT", value);
			}

			particleSystem->SetParticleWidth(value);
			particleSystem->Reset();
		}

		else if (window.Changed("HEIGHT"))
		{
			float value = window.GetValue<SliderFloatComponent>("HEIGHT");

			if (window.GetValue<CheckBoxComponent>("KEEP SQUARE"))
			{
				particleSystem->SetParticleWidth(value);
				window.SetValue<SliderFloatComponent, float>("WIDTH", value);
			}

			particleSystem->SetParticleHeight(value);
			particleSystem->Reset();
		}

		else if (window.Changed("ROTATION"))
		{
			bool value = window.GetValue<CheckBoxComponent>("ROTATION");
			
			particleSystem->SetRotation(value);
			particleSystem->Reset();
		}

		else if (particleSystem->GetRotation())
		{
			if (window.Changed("R-MAX"))
			{
				
				auto maxValue = window.GetValue<SliderFloatComponent>("R-MAX");
				float minValue = window.GetValue<SliderFloatComponent>("R-MIN");
				if (maxValue > minValue)
				{
					particleSystem->SetMaxRotationSpeed(maxValue);
					particleSystem->Reset();
				}
				else
					window.SetValue<SliderFloatComponent, float>("R-MAX", minValue);
			}

			if (window.Changed("R-MIN"))
			{
				auto maxValue = window.GetValue<SliderFloatComponent>("R-MAX");
				float minValue = window.GetValue<SliderFloatComponent>("R-MIN");
				if (maxValue > minValue)
				{
					particleSystem->SetMinRotationSpeed(minValue);
					particleSystem->Reset();
				}
				else
					window.SetValue<SliderFloatComponent, float>("R-MIN", maxValue);
			}
		}
		
	}
	
	float dt = timer.DeltaTime();

	if (dt < tickInterval)
	{
		float timeToSleep = tickInterval - dt;
		Sleep(timeToSleep * 500.0f);
	}

	if (renderModel)
	{
		MR->Bind(model);
	}
	else
		MR->Unbind(model);

	return APPSTATE::NO_CHANGE;
}
