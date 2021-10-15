#include "LevelEditor.h"
#include "Event.h"
#include "GameLoader.h"
#include "FBXLoader.h"

void LevelEditor::BindDrawables()
{
	for (auto& [name, drawable] : scene.GetDrawables())
	{
		auto model = std::dynamic_pointer_cast<Model>(drawable);
		if (model)
		{
			scene.GetObjectNames().push_back(name);
			model->SetID(scene.GetObjectNames().size());
			modelRenderer.Bind(model);
			idRenderer.Bind(model);
		}

		auto particleSystem = std::dynamic_pointer_cast<ParticleSystem>(drawable);
		if (particleSystem)
		{
			//SAME BUT PS->
		}
	}
}

void LevelEditor::Save(const std::string& file)
{

}

void LevelEditor::Load(const std::string& file)
{
	std::filesystem::path path = std::filesystem::path(file);
	if (file == "" || path.extension() != ".fbx")
		return;

	std::string fileName = path.stem().string();
	fileName = scene.AddModel(fileName, path.string());
	auto model = scene.Get<Model>(fileName);
	model->SetID(scene.GetObjectNames().size());
	idRenderer.Bind(model);
	modelRenderer.Bind(model);
	windows["SCENE COMPONENTS"].AddTextComponent(scene.GetObjectNames()[scene.GetObjectNames().size() - 1]);
}

void LevelEditor::CreateBoundingBox()
{
	if (selectedObject == "")
		return;

	auto selected = scene.Get<Drawable>(selectedObject);
	auto box = std::make_shared<BoxVolume>();
	int numInstances = 0;

	box->SetPosition(selected->GetPosition());

	for (auto name : scene.GetObjectNames())
		if (name.find("BoxVolume") != std::string::npos)
			numInstances++;
	std::string name = "BoxVolume";
	if (numInstances > 0)
		name += std::to_string(numInstances);
	
	scene.AddBoundingVolume(name, box);
	selectedObject = name;

	box->SetID(scene.GetObjectNames().size());

	windows["GAME OBJECT"].SetValue<TextComponent, std::string>("ObjectName", name);
	windows["SCENE COMPONENTS"].AddTextComponent(scene.GetObjectNames()[scene.GetObjectNames().size() - 1]);

	volumeRenderer.Bind(box);
	idRenderer.Bind(box);

	Print("BoundingBox Created!");
}

void LevelEditor::CreateBoundingSphere()
{

	if (selectedObject == "")
		return;

	auto selected = scene.Get<Drawable>(selectedObject);
	auto sphere = std::make_shared<SphereVolume>();
	int numInstances = 0;

	sphere->SetPosition(selected->GetPosition());

	for (auto name : scene.GetObjectNames())
		if (name.find("SphereVolume") != std::string::npos)
			numInstances++;
	std::string name = "SphereVolume";
	if (numInstances > 0)
		name += std::to_string(numInstances);

	scene.AddBoundingVolume(name, sphere);
	selectedObject = name;

	sphere->SetID(scene.GetObjectNames().size());

	windows["GAME OBJECT"].SetValue<TextComponent, std::string>("ObjectName", name);
	windows["SCENE COMPONENTS"].AddTextComponent(scene.GetObjectNames()[scene.GetObjectNames().size() - 1]);

	volumeRenderer.Bind(sphere);
	idRenderer.Bind(sphere);

	Print("BoundingSphere Created!");
}

void LevelEditor::Update()
{
	if (Event::LeftIsClicked())
	{
		GetCursorPos(&cursor);
		ScreenToClient(appWindow, &cursor);

		int id = idRenderer.GetObjectID(cursor.x, cursor.y);
		Print(id);

		if (id > 0)
		{
			std::string name = scene.GetObjectNames()[id - 1];
			if (name != "")
			{
				auto model = scene.Get<Drawable>(name);
				auto& window = windows["GAME OBJECT"];

				window.SetValue<TextComponent, std::string>("ObjectName", name);

				window.SetValue<SliderFloatComponent, float>("X", model->GetPosition().x);
				window.SetValue<SliderFloatComponent, float>("Y", model->GetPosition().y);
				window.SetValue<SliderFloatComponent, float>("Z", model->GetPosition().z);

				window.SetValue<SliderFloatComponent, float>("Around X", model->GetRotation().x * 180 / PI);
				window.SetValue<SliderFloatComponent, float>("Around Y", model->GetRotation().y * 180 / PI);
				window.SetValue<SliderFloatComponent, float>("Around Z", model->GetRotation().z * 180 / PI);

				window.SetValue<SliderFloatComponent, float>("X-axis", model->GetScale().x);
				window.SetValue<SliderFloatComponent, float>("Y-axis", model->GetScale().y);
				window.SetValue<SliderFloatComponent, float>("Z-axis", model->GetScale().z);

				selectedObject = name;
			}
		}
	}

	if (Event::KeyIsPressed('C'))
	{
		ClearToolUI();
		selectedObject = "";
	}

	if (Event::RightIsClicked())
	{
		if (Event::ReadRawDelta().y > 0)
			scene.GetCamera()->Rotate(0, 3);

		if (Event::ReadRawDelta().y < 0)
			scene.GetCamera()->Rotate(0, -3);

		if (Event::ReadRawDelta().x > 0)
			scene.GetCamera()->Rotate(3, 0);

		if (Event::ReadRawDelta().x < 0)
			scene.GetCamera()->Rotate(-3, 0);
	}

	if (Event::KeyIsPressed('W'))
		scene.GetCamera()->MoveForward();

	if (Event::KeyIsPressed('A'))
		scene.GetCamera()->MoveRight(-1);

	if (Event::KeyIsPressed('S'))
		scene.GetCamera()->MoveForward(-1);

	if (Event::KeyIsPressed('D'))
		scene.GetCamera()->MoveRight();

	if (Event::KeyIsPressed(VK_SPACE))
		scene.GetCamera()->MoveUp();

	if (Event::KeyIsPressed('Z'))
		scene.GetCamera()->MoveUp(-1);
  
	if (Event::KeyIsPressed(VK_SHIFT))
		scene.GetCamera()->SetSpeedMultiplier(4);
	else
		scene.GetCamera()->SetSpeedMultiplier(1);

	if (selectedObject != "") //CHECKS EVERY FRAME, USE CHANGED()?
	{
		auto& window = windows["GAME OBJECT"];
		float newXPos = window.GetValue<SliderFloatComponent>("X");
		float newYPos = window.GetValue<SliderFloatComponent>("Y");
		float newZPos = window.GetValue<SliderFloatComponent>("Z");

		float newXRot = window.GetValue<SliderFloatComponent>("Around X");
		float newYRot = window.GetValue<SliderFloatComponent>("Around Y");
		float newZRot = window.GetValue<SliderFloatComponent>("Around Z");

		float newXScale = window.GetValue<SliderFloatComponent>("X-axis");
		float newYScale = window.GetValue<SliderFloatComponent>("Y-axis");
		float newZScale = window.GetValue<SliderFloatComponent>("Z-axis");

		auto model = scene.Get<Drawable>(selectedObject);
		model->SetPosition(newXPos, newYPos, newZPos);
		model->SetRotation(newXRot * PI / 180, newYRot * PI / 180, newZRot * PI / 180);
		model->SetScale(newXScale, newYScale, newZScale);
		if (window.GetValue<CheckBoxComponent>("Uniform scaling") == true)
		{
			if (window.Changed("X-axis"))
			{
				model->SetScale(newXScale, newXScale, newXScale);
				window.SetValue<SliderFloatComponent, float>("Y-axis", newXScale);
				window.SetValue<SliderFloatComponent, float>("Z-axis", newXScale);
			}
			if (window.Changed("Y-axis"))
			{
				model->SetScale(newYScale, newYScale, newYScale);
				window.SetValue<SliderFloatComponent, float>("X-axis", newYScale);
				window.SetValue<SliderFloatComponent, float>("Z-axis", newYScale);
			}
			if (window.Changed("Z-axis"))
			{
				model->SetScale(newZScale, newZScale, newZScale);
				window.SetValue<SliderFloatComponent, float>("Y-axis", newZScale);
				window.SetValue<SliderFloatComponent, float>("X-axis", newZScale);
			}
		}
	}

	auto& window = windows["GAME OBJECT"];
	if (window.GetValue<ButtonComponent>("Delete") || Event::KeyIsPressed(VK_DELETE))
	{
		if (selectedObject != "")
		{
			RemoveItem(selectedObject);
			ClearToolUI();
			selectedObject = "";
		}
	}

	scene.Update();

	Event::ClearRawDelta();
}

void LevelEditor::Render()
{
	idRenderer.Render();

	BeginFrame();

	//TO DO: ADD PARTICLE RENDER PASS (IN OPEN WORLD)
	//ADD RENDERER THAT RENDERS TO TEXTURE THAT CAN BE SHOWN AS "INGAME"-PREVIEW IN MATERIAL EDITOR 
	//(ONLY NEEDS ONE POINT LIGHT & DIRECTIONAL LIGHT, MAYBE A POSITION SLIDER FOR POINT TO PLAY WITH SPECULAR (OR ROTATING MESH))
	//PREVIEW EITHER ON A SPHERE OR THE SELECTED MESH

	terrainRenderer.Render(*terrain);

	animatedModelRenderer.Render();

	modelRenderer.Render();
	
	volumeRenderer.Render();

	EndFrame();

	Resources::Inst().Reset();
}

LevelEditor::LevelEditor(UINT clientWidth, UINT clientHeight, HWND window)
	:modelRenderer(FORWARD, false),
	terrainRenderer(FORWARD),
	animatedModelRenderer(FORWARD, false),
	colliderRenderer(FORWARD)
{
	//LOAD SCENE
	FBXLoader levelLoader("Models");

	GameLoader gameLoader;
	gameLoader.Load("Default", scene.GetDrawables());
	BindDrawables();

	scene.SetCamera(PI_DIV4, float(clientWidth) / float(clientHeight), 0.1f, 1000.0f, 1.0f, 25.0f, {0, 90, 0});
	scene.SetDirectionalLight(40);

	//CLIENT INFORMATION (PICKING) TO BE REMOVED?
	wWidth = clientWidth;
	wHeight = clientHeight;

	appWindow = window;
	wRatioX = (float)clientWidth / GetSystemMetrics(SM_CXSCREEN);
	wRatioY = (float)clientHeight / GetSystemMetrics(SM_CYSCREEN);

	terrain = new Terrain();

	//WINDOWS
	{
		AddWindow("TOOLS");
		auto& window = windows["TOOLS"];				
		window.AddButtonComponent("LOAD FBX", 120, 30);
		window.AddButtonComponent("SAVE WORLD", 120, 30, true);
		window.AddSliderIntComponent("TERRAIN START SUBDIVISIONS", 0, 5);
		window.AddCheckBoxComponent("WIREFRAME", false);
		window.AddButtonComponent("CREATE BBOX", 120, 30);
		window.AddButtonComponent("CREATE BSPHERE", 120, 30, true);
		window.AddButtonComponent("RETURN TO MENU", 120, 30);
	}

	{
		AddWindow("GAME OBJECT");
		auto& window = windows["GAME OBJECT"];	
		window.AddTextComponent("ObjectName");
		window.AddTextComponent("Position");
		window.AddSliderFloatComponent("X", -300, 300, 0, false);
		window.AddSliderFloatComponent("Y", -300, 300, 0, false);
		window.AddSliderFloatComponent("Z", -300, 300, 0, false);

		window.AddTextComponent("Rotation");
		window.AddSliderFloatComponent("Around X", -180, 180, 0, false);
		window.AddSliderFloatComponent("Around Y", -180, 180, 0, false);
		window.AddSliderFloatComponent("Around Z", -180, 180, 0, false);

		window.AddTextComponent("Scale");
		window.AddSliderFloatComponent("X-axis", -30, 30, 0, false);
		window.AddSliderFloatComponent("Y-axis", -30, 30, 0, false);
		window.AddSliderFloatComponent("Z-axis", -30, 30, 0, false);
		window.AddCheckBoxComponent("Uniform scaling");
		window.AddButtonComponent("Delete", 120, 30);
	}

	{
		AddWindow("SCENE COMPONENTS");
		auto& window = windows["SCENE COMPONENTS"];
		for(int i = 0; i < scene.GetObjectNames().size(); i++)
		{
			window.AddTextComponent(scene.GetObjectNames()[i]);
		}
	}

	(void)Run();
}

void LevelEditor::RemoveItem(const std::string name)
{
	auto model = scene.Get<Drawable>(name);
	volumeRenderer.Unbind(model);
	modelRenderer.Unbind(model);
	idRenderer.Unbind(model);

	scene.DeleteDrawable(name);
	scene.GetObjectNames()[model.get()->GetID() -1] = "";
}

void LevelEditor::ClearToolUI()
{
	auto& window = windows["GAME OBJECT"];

	window.SetValue<TextComponent, std::string>("ObjectName", "");
	window.SetValue<SliderFloatComponent, float>("X", 0.0f);
	window.SetValue<SliderFloatComponent, float>("Y", 0.0f);
	window.SetValue<SliderFloatComponent, float>("Z", 0.0f);

	window.SetValue<SliderFloatComponent, float>("Around X", 0.0f);
	window.SetValue<SliderFloatComponent, float>("Around Y", 0.0f);
	window.SetValue<SliderFloatComponent, float>("Around Z", 0.0f);

	window.SetValue<SliderFloatComponent, float>("X-axis", 0.0f);
	window.SetValue<SliderFloatComponent, float>("Y-axis", 0.0f);
	window.SetValue<SliderFloatComponent, float>("Z-axis", 0.0f);
}


LevelEditor::~LevelEditor()
{
	delete terrain;
	Resources::Inst().Clear();
}

State LevelEditor::Run()
{
	Update();
	Render();

	auto& window = windows["TOOLS"];
	if (window.GetValue<ButtonComponent>("LOAD FBX"))
		Load(FileSystem::LoadFile("Models"));

	if (window.GetValue<ButtonComponent>("CREATE BBOX"))
		CreateBoundingBox();

	if (window.GetValue<ButtonComponent>("CREATE BSPHERE"))
		CreateBoundingSphere();

	if (window.Changed("WIREFRAME"))
		Graphics::Inst().ToggleWireframe();

	if (window.Changed("TERRAIN START SUBDIVISIONS"))
	{
		if (terrain)
			delete terrain;
		terrain = new Terrain(window.GetValue<SliderIntComponent>("TERRAIN START SUBDIVISIONS"));
	}
	
	if (window.GetValue<ButtonComponent>("SAVE WORLD"))
	{
		GameLoader loader;
		loader.Save("Default", scene.GetDrawables());
	}

	if (window.GetValue<ButtonComponent>("RETURN TO MENU"))
		return State::MENU;

	return State::NO_CHANGE;
}