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
			modelRenderer.Bind(model);

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
	modelRenderer.Bind(scene.Get<Model>(fileName));
	windows["SCENE COMPONENTS"].AddTextComponent(scene.GetObjectNames()[scene.GetObjectNames().size() - 1]);

	auto boundingSphere = std::make_shared<BoundingSphere>();
	pickBoxes.emplace(fileName, boundingSphere);
	boundingSphere->SetPosition(0, 2, 0);
	boundingSphere->SetParent(scene.Get<Model>(fileName));

	colliderRenderer.Bind(boundingSphere);
}

void LevelEditor::Update()
{
	if (Event::LeftIsClicked())
	{
		GetCursorPos(&cursor);
		ScreenToClient(appWindow, &cursor);
		screenSpaceCoordinates.x = (((4.0f * cursor.x * wRatioX) / (float)wWidth) - 2 * wRatioX) / scene.GetCamera()->GetProjectionMatrix()._11;
		screenSpaceCoordinates.y = (((-4.0f * cursor.y * wRatioY) / (float)wHeight) + 2 * wRatioY) / scene.GetCamera()->GetProjectionMatrix()._22;
		if (screenSpaceCoordinates.x > 1)
			screenSpaceCoordinates.x = 1;
		if (screenSpaceCoordinates.x < -1)
			screenSpaceCoordinates.x = -1;
		if (screenSpaceCoordinates.y > 1)
			screenSpaceCoordinates.y = 1;
		if (screenSpaceCoordinates.y < -1)
			screenSpaceCoordinates.y = -1;

		Matrix inverseView = scene.GetCamera()->GetViewMatrix().Invert();

		pickRay.direction.x = (screenSpaceCoordinates.x * inverseView._11) + (screenSpaceCoordinates.y * inverseView._21) + inverseView._31;
		pickRay.direction.y = (screenSpaceCoordinates.x * inverseView._12) + (screenSpaceCoordinates.y * inverseView._22) + inverseView._32;
		pickRay.direction.z = (screenSpaceCoordinates.x * inverseView._13) + (screenSpaceCoordinates.y * inverseView._23) + inverseView._33;

		pickRay.origin = scene.GetCamera()->GetPosition();

		for (auto& [name, boundingSphere] : pickBoxes)
		{
			Vector3::Transform(pickRay.origin, boundingSphere->GetMatrix().Invert());
			Vector3::Transform(pickRay.direction, boundingSphere->GetMatrix().Invert());
			pickRay.direction.Normalize();

			if (Collision::Intersection(*std::dynamic_pointer_cast<BoundingSphere>(boundingSphere), pickRay.origin, pickRay.direction, pickRay.length) == true)
			{
				auto model = scene.Get<Model>(name);
				auto& window = windows["GAME OBJECT"];

				window.SetValue<TextComponent, std::string>("ObjectName", name);

				window.SetValue<SliderFloatComponent, float>("X", model->GetPosition().x);
				window.SetValue<SliderFloatComponent, float>("Y", model->GetPosition().y);
				window.SetValue<SliderFloatComponent, float>("Z", model->GetPosition().z);

				window.SetValue<SliderFloatComponent, float>("Around X", model->GetRotation().x);
				window.SetValue<SliderFloatComponent, float>("Around Y", model->GetRotation().y);
				window.SetValue<SliderFloatComponent, float>("Around Z", model->GetRotation().z);

				window.SetValue<SliderFloatComponent, float>("X-axis", model->GetScale().x);
				window.SetValue<SliderFloatComponent, float>("Y-axis", model->GetScale().y);
				window.SetValue<SliderFloatComponent, float>("Z-axis", model->GetScale().z);

				selectedObject = name;
			}
		}
	}

	if (Event::KeyIsPressed('C'))
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

		auto model = scene.Get<Model>(selectedObject);
		model->SetPosition(newXPos, newYPos, newZPos);
		model->SetRotation(newXRot * PI / 180, newYRot * PI / 180, newZRot * PI / 180);
		model->SetScale(newXScale, newXScale, newXScale);
	}

	for (auto& [name, boundingSphere] : pickBoxes)
		boundingSphere->Update();

	scene.Update();

	Event::ClearRawDelta();
}

void LevelEditor::Render()
{
	BeginFrame();

	//TO DO: ADD PARTICLE RENDER PASS (IN OPEN WORLD)
	//ADD RENDERER THAT RENDERS TO TEXTURE THAT CAN BE SHOWN AS "INGAME"-PREVIEW IN MATERIAL EDITOR 
	//(ONLY NEEDS ONE POINT LIGHT & DIRECTIONAL LIGHT, MAYBE A POSITION SLIDER FOR POINT TO PLAY WITH SPECULAR (OR ROTATING MESH))
	//PREVIEW EITHER ON A SPHERE OR THE SELECTED MESH

	terrainRenderer.Render(*terrain);

	animatedModelRenderer.Render();

	modelRenderer.Render();

	colliderRenderer.Render();

	EndFrame();

	Resources::Inst().Reset();
}

LevelEditor::LevelEditor(UINT clientWidth, UINT clientHeight, HWND window)
	:modelRenderer(FORWARD, false),
	terrainRenderer(FORWARD),
	animatedModelRenderer(FORWARD, false)
{
	//LOAD SCENE
	FBXLoader levelLoader("Models");

	GameLoader gameLoader;
	gameLoader.Load("Default", scene.GetDrawables());
	BindDrawables();

	scene.SetCamera(PI_DIV4, float(clientWidth) / float(clientHeight), 0.1f, 500.0f, 1.0f, 15.0f, {0, 50, 0});
	scene.SetDirectionalLight(40);

	//CLIENT INFORMATION (PICKING) TO BE REMOVED?
	wWidth = clientWidth;
	wHeight = clientHeight;

	appWindow = window;
	wRatioX = (float)clientWidth / GetSystemMetrics(SM_CXSCREEN);
	wRatioY = (float)clientHeight / GetSystemMetrics(SM_CYSCREEN);

	//TERRAIN
	terrain = new Terrain(20, 0);

	//WINDOWS
	{
		AddWindow("TOOLS");
		auto& window = windows["TOOLS"];				
		window.AddButtonComponent("LOAD FBX", 120, 30);
		window.AddButtonComponent("SAVE WORLD", 120, 30, true);
		window.AddSliderIntComponent("TERRAIN START SUBDIVISIONS", 0, 5);
		window.AddButtonComponent("RETURN TO MENU", 120, 30);
	}

	{
		AddWindow("GAME OBJECT");
		auto& window = windows["GAME OBJECT"];	
		window.AddTextComponent("ObjectName");
		window.AddTextComponent("Position");
		window.AddSliderFloatComponent("X", -50, 50, 0, false);
		window.AddSliderFloatComponent("Y", -50, 50, 0, false);
		window.AddSliderFloatComponent("Z", -50, 50, 0, false);

		window.AddTextComponent("Rotation");
		window.AddSliderFloatComponent("Around X", -180, 180, 0, false);
		window.AddSliderFloatComponent("Around Y", -180, 180, 0, false);
		window.AddSliderFloatComponent("Around Z", -180, 180, 0, false);

		window.AddTextComponent("Scale");
		window.AddSliderFloatComponent("X-axis", -10, 10, 0, false);
		window.AddSliderFloatComponent("Y-axis", -10, 10, 0, false);
		window.AddSliderFloatComponent("Z-axis", -10, 10, 0, false);
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

	if (window.Changed("TERRAIN START SUBDIVISIONS"))
	{
		if (terrain)
			delete terrain;
		terrain = new Terrain(20.0f, window.GetValue<SliderIntComponent>("TERRAIN START SUBDIVISIONS"));
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