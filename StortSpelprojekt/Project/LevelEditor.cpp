#include "LevelEditor.h"
#include "Event.h"
#include "GameLoader.h"
#include "FBXLoader.h"
#include "Renderers.h"

void LevelEditor::BindDrawables()
{
	for (auto& [name, drawable] : scene.GetDrawables())
	{
		auto model = std::dynamic_pointer_cast<Model>(drawable);
		if (model)
		{
			scene.GetObjectNames().push_back(name);
			model->SetID((UINT)scene.GetObjectNames().size());
			MR->Bind(model);
			IDR->Bind(model);
			ListBoxComponent* component = windows["SCENE COMPONENTS"].Get<ListBoxComponent>("NameList");
			component->AddName(name);
			totalPolygonCount += model->mesh.vertexCount / 3.0f;
		}

		auto box = std::dynamic_pointer_cast<BoundingBox>(drawable);
		if(box)
		{
			auto volume = std::make_shared<BoxVolume>();
			volume->SetMatrix(box->GetMatrix());
			volume->SetPosition(box->GetPosition());
			volume->SetRotation(box->GetRotation());
			volume->SetScale(box->GetScale());
			volume->SetName(name);
			scene.GetDrawables()[name] = volume;
			scene.GetObjectNames().push_back(name);
			volume->SetID((UINT)scene.GetObjectNames().size());
			VR->Bind(volume);
			IDR->Bind(volume);
			ListBoxComponent* component = windows["SCENE COMPONENTS"].Get<ListBoxComponent>("NameList");
			component->AddName(name);
		}

		auto sphere = std::dynamic_pointer_cast<BoundingSphere>(drawable);
		if (sphere)
		{
			auto volume = std::make_shared<SphereVolume>();
			volume->SetMatrix(sphere->GetMatrix());
			volume->SetPosition(sphere->GetPosition());
			volume->SetRotation(sphere->GetRotation());
			volume->SetScale(sphere->GetScale());
			volume->SetName(name);
			scene.GetDrawables()[name] = volume;
			scene.GetObjectNames().push_back(name);
			volume->SetID((UINT)scene.GetObjectNames().size());
			VR->Bind(volume);
			IDR->Bind(volume);
			ListBoxComponent* component = windows["SCENE COMPONENTS"].Get<ListBoxComponent>("NameList");
			component->AddName(name);
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
	model->SetID((UINT)scene.GetObjectNames().size());
	IDR->Bind(model);
	MR->Bind(model);
	ListBoxComponent* component = windows["SCENE COMPONENTS"].Get<ListBoxComponent>("NameList");
	component->AddName(fileName);
	totalPolygonCount += model->mesh.vertexCount / 3.0f;
}

void LevelEditor::DuplicateObject()
{
	if (selectedObject == "")
		return;

	auto originModel = scene.Get<Model>(selectedObject);
	std::string meshName = Resources::Inst().GetBufferNameFromID(originModel->mesh.bufferID);

	UINT instances = 0;
	for (auto& name : scene.GetObjectNames())
	{
		if (name.find(meshName) != std::string::npos)
			instances++;
	}

	if (instances > 0)
	{
		auto model = std::make_shared<Model>(meshName + std::to_string(instances), *scene.Get<Model>(selectedObject));
		std::string modelName = model->GetName();

		scene.AddModel(modelName, model);
		model->SetID((UINT)scene.GetObjectNames().size());
		IDR->Bind(model);
		MR->Bind(model);
		ListBoxComponent* component = windows["SCENE COMPONENTS"].Get<ListBoxComponent>("NameList");
		component->AddName(modelName);
		totalPolygonCount += model->mesh.vertexCount / 3.0f;
		selectedObject = modelName;
	}
}

void LevelEditor::DuplicateVolume()
{
	auto selected = scene.Get<Drawable>(selectedObject);
	auto box = std::dynamic_pointer_cast<BoxVolume>(selected);
	if (box)
	{
		int numInstances = 0;
		for (auto& name : scene.GetObjectNames())
			if (name.find("BoxVolume") != std::string::npos)
				numInstances++;
		std::string name = "BoxVolume";
		if (numInstances > 0)
			name += std::to_string(numInstances) + "d";


		auto volume = std::make_shared<BoxVolume>();
		volume->SetMatrix(box->GetMatrix());
		volume->SetPosition(box->GetPosition());
		volume->SetRotation(box->GetRotation());
		volume->SetScale(box->GetScale());
		scene.AddBoundingVolume(name, volume);
		volume->SetID((UINT)scene.GetObjectNames().size());
		VR->Bind(volume);
		IDR->Bind(volume);
		ListBoxComponent* component = windows["SCENE COMPONENTS"].Get<ListBoxComponent>("NameList");
		component->AddName(name);
		selectedObject = name;

		Print("BoundingBox Duplicated!");
	}
	auto sphere = std::dynamic_pointer_cast<SphereVolume>(selected);
	if (sphere)
	{
		int numInstances = 0;
		for (auto& name : scene.GetObjectNames())
			if (name.find("SphereVolume") != std::string::npos)
				numInstances++;
		std::string name = "SphereVolume";
		if (numInstances > 0)
			name += std::to_string(numInstances) + "d";

		auto volume = std::make_shared<SphereVolume>();
		volume->SetMatrix(sphere->GetMatrix());
		volume->SetPosition(sphere->GetPosition());
		volume->SetRotation(sphere->GetRotation());
		volume->SetScale(sphere->GetScale());
		scene.AddBoundingVolume(name, volume);
		volume->SetID((UINT)scene.GetObjectNames().size());
		VR->Bind(volume);
		IDR->Bind(volume);
		ListBoxComponent* component = windows["SCENE COMPONENTS"].Get<ListBoxComponent>("NameList");
		component->AddName(name);
		selectedObject = name;

		Print("BoundingSphere Duplicated!");
	}
}

void LevelEditor::CreateBoundingBox()
{
	if (selectedObject == "")
		return;

	auto selected = scene.Get<Drawable>(selectedObject);
	auto box = std::make_shared<BoxVolume>();
	int numInstances = 0;

	box->SetPosition(selected->GetPosition());

	for (auto& name : scene.GetObjectNames())
		if (name.find("BoxVolume") != std::string::npos)
			numInstances++;
	std::string name = "BoxVolume";
	if (numInstances > 0)
		name += std::to_string(numInstances);
	
	scene.AddBoundingVolume(name, box);
	selectedObject = name;

	box->SetID((UINT)scene.GetObjectNames().size());

	windows["GAME OBJECT"].SetValue<TextComponent, std::string>("ObjectName", name);
	ListBoxComponent* component = windows["SCENE COMPONENTS"].Get<ListBoxComponent>("NameList");
	component->AddName(name);

	VR->Bind(box);
	IDR->Bind(box);

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

	for (auto& name : scene.GetObjectNames())
		if (name.find("SphereVolume") != std::string::npos)
			numInstances++;
	std::string name = "SphereVolume";
	if (numInstances > 0)
		name += std::to_string(numInstances);

	scene.AddBoundingVolume(name, sphere);
	selectedObject = name;

	sphere->SetID((UINT)scene.GetObjectNames().size());

	windows["GAME OBJECT"].SetValue<TextComponent, std::string>("ObjectName", name);
	ListBoxComponent* component = windows["SCENE COMPONENTS"].Get<ListBoxComponent>("NameList");
	component->AddName(name);

	VR->Bind(sphere);
	IDR->Bind(sphere);

	Print("BoundingSphere Created!");
}

void LevelEditor::ShowWater()
{
	bool changed = false;
	if (renderWater)
	{
		renderWater = false;
		changed = true;
	}
	if (!renderWater && !changed)
	{
		renderWater = true;
	}
}

void LevelEditor::DivideRendering()
{
	int divX = windows["TOOLS"].GetValue<SliderIntComponent>("RENDER DIVIDE");
	auto& drawables = scene.GetDrawables();
	for (auto& [drawableName, drawable] : drawables)
	{
		if (!divideFlipped)
		{
			if (drawable->GetPosition().x < divX)
			{
				auto model = std::dynamic_pointer_cast<Model>(drawable);
				if (model)
				{
					MR->Unbind(drawable);
					IDR->Unbind(drawable);
				}
				else
				{
					VR->Unbind(drawable);
					IDR->Unbind(drawable);
				}
			}
			else
			{
				auto model = std::dynamic_pointer_cast<Model>(drawable);
				if (model)
				{
					if (!MR->IsBound(drawable))
					{
						MR->Bind(drawable);
						IDR->Bind(drawable);
					}
				}
				else
				{
					if (!VR->IsBound(drawable))
					{
						VR->Bind(drawable);
						IDR->Bind(drawable);
					}
				}
			}
		}
		else
		{
			if (drawable->GetPosition().x > divX)
			{
				auto model = std::dynamic_pointer_cast<Model>(drawable);
				if (model)
				{
					MR->Unbind(drawable);
					IDR->Unbind(drawable);
				}
				else
				{
					VR->Unbind(drawable);
					IDR->Unbind(drawable);
				}
			}
			else
			{
				auto model = std::dynamic_pointer_cast<Model>(drawable);
				if (model)
				{
					if (!MR->IsBound(drawable))
					{
						MR->Bind(drawable);
						IDR->Bind(drawable);
					}
				}
				else
				{
					if (!VR->IsBound(drawable))
					{
						VR->Bind(drawable);
						IDR->Bind(drawable);
					}
				}
			}
		}
	}
}

void LevelEditor::FlipRenderingDivider()
{
	bool changed = false;
	if (divideFlipped)
	{
		divideFlipped = false;
		changed = true;
	}
	if (!divideFlipped && !changed)
	{
		divideFlipped = true;
	}
}

void LevelEditor::ShowVolumes()
{
	bool changed = false;
	if (renderVolumes)
	{
		renderVolumes = false;
		changed = true;
	}
	if (!renderVolumes && !changed)
	{
		renderVolumes = true;
	}
}

void LevelEditor::ShowTerrain()
{
	bool changed = false;
	if (renderTerrain)
	{
		renderTerrain = false;
		changed = true;
	}
	if (!renderTerrain && !changed)
	{
		renderTerrain = true;
	}
}

void LevelEditor::Update()
{
	if (Event::LeftIsClicked() && !ImGuizmo::IsOver() && viewportPanel.Hovered())
	{
		auto mousePos = viewportPanel.GetMousePosition();
	
		if (mousePos.x >= 0 && mousePos.y >= 0)
		{
			int id = IDR->GetObjectID((UINT)mousePos.x, (UINT)mousePos.y);

			if (id > 0)
			{
				std::string name = scene.GetObjectNames()[id - 1];
				if (name != "")
					UpdateToolUI(name);
			}

			if (id == 0)
				ClearToolUI();
		}
	}

	if (Event::KeyIsPressed('C'))
		ClearToolUI();

	if (Event::KeyIsPressed('F'))
		FocusObject();

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
  

	static float lastClick = 0;
	if (Time::Get() - lastClick > 0.5f)
	{
		if (Event::KeyIsPressed(VK_CONTROL) && Event::KeyIsPressed('D'))
		{
			auto selected = scene.Get<Drawable>(selectedObject);
			auto model = std::dynamic_pointer_cast<Model>(selected);
			if (model)
				DuplicateObject();
			else
				DuplicateVolume();
			lastClick = Time::Get();
		}
	}

	if (Event::KeyIsPressed(VK_SHIFT))
		scene.GetCamera()->SetSpeedMultiplier(8);
	else
		scene.GetCamera()->SetSpeedMultiplier(1);

	if (selectedObject != "") //CHECKS EVERY FRAME, USE CHANGED()?
	{
		auto& window = windows["GAME OBJECT"];

		float newXPos = window.GetValue<SliderFloatComponent>("X");
		float newYPos = window.GetValue<SliderFloatComponent>("Y");
		float newZPos = window.GetValue<SliderFloatComponent>("Z");

		float newXScale = window.GetValue<SliderFloatComponent>("X-axis");
		float newYScale = window.GetValue<SliderFloatComponent>("Y-axis");
		float newZScale = window.GetValue<SliderFloatComponent>("Z-axis");

		auto model = scene.Get<Drawable>(selectedObject);
		model->SetPosition(newXPos, newYPos, newZPos);

		//model->SetRotation(newXRot * PI / 180, newYRot * PI / 180, newZRot * PI / 180);

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

	if (Event::KeyIsPressed('1'))
		operation = ImGuizmo::TRANSLATE;
	else if (Event::KeyIsPressed('2'))
		operation = ImGuizmo::ROTATE;
	else if (Event::KeyIsPressed('3'))
		operation = ImGuizmo::SCALE;

	auto& window = windows["GAME OBJECT"];
	if (window.GetValue<ButtonComponent>("Delete") || Event::KeyIsPressed(VK_DELETE))
	{
		if (selectedObject != "")
		{
			RemoveItem(selectedObject);
			ClearToolUI();
		}
	}

	if (windows["SCENE COMPONENTS"].Changed("NameList"))
	{
		std::string name = windows["SCENE COMPONENTS"].GetValue<ListBoxComponent>("NameList");
		UpdateToolUI(name);
	}


	scene.Update();
	scene.GetCamera()->Update();
	ShaderData::Inst().Update(*scene.GetCamera(), scene.GetDirectionalLight(), 0, nullptr);

	Event::ClearRawDelta();

	if (viewportPanel.GetWidth() != 0)
		IDR->UpdateViewport((UINT)viewportPanel.GetWidth(), (UINT)viewportPanel.GetHeight());
}

void LevelEditor::Render()
{
	IDR->BeginFrame(dsv, viewport);
	IDR->Render();

	BeginViewportFrame();

	if(renderTerrain)
		TR->Render(*terrain);

	if(renderWater)
		WR->Render(water);

	MR->Render();

	if(renderVolumes)
		VR->Render();


	BeginFrame();

	if (!selectedObject.empty())
	{
		ImGUI::BeginGizmo(viewportPanel.GetWidth(), viewportPanel.GetHeight());
		auto selected = scene.Get<Drawable>(selectedObject);

		Matrix matrix = selected->GetMatrix().Transpose();
		
		ImGUI::Gizmo(matrix, scene.GetCamera()->GetViewMatrix(), scene.GetCamera()->GetProjectionMatrix(), operation);

		selected->SetMatrix(matrix);

		Vector3 translation;
		Quaternion quaternion;
		Vector3 scale;

		matrix.Decompose(scale, quaternion, translation);

		selected->SetScale(scale);
		selected->SetPosition(translation);
		selected->SetRotation(quaternion);

		UpdateToolUI(selectedObject);
	}

	ImGui::End();
	ImGui::PopStyleVar();

	EndFrame();

	Resources::Inst().Reset();
}

LevelEditor::LevelEditor(UINT clientWidth, UINT clientHeight, HWND window)
	:water(5000)
{

	RND.InitModelRenderer(false);
	RND.InitIDRenderer();
	RND.InitTerrainRenderer();
	RND.InitWaterRenderer();
	RND.InitVolumeRenderer();

	//WINDOWS
	{
		AddWindow("TOOLS");
		auto& window = windows["TOOLS"];
		window.AddTextComponent("FPS");
		window.AddTextComponent("SCENE POLYGON COUNT");
		window.AddButtonComponent("LOAD FBX", 120, 30);
		window.AddButtonComponent("SAVE WORLD", 120, 30, true);
		window.AddButtonComponent("CREATE BBOX", 120, 30);
		window.AddButtonComponent("CREATE BSPHERE", 120, 30, true);
		window.AddTextComponent("");
		window.AddSliderIntComponent("TERRAIN SUBDIV", 0, 5);
		window.AddCheckBoxComponent("WIREFRAME", false);
		window.AddTextComponent("SHOW:");
		window.AddCheckBoxComponent("TERRAIN", true);
		window.AddCheckBoxComponent("WATER", true);
		window.AddCheckBoxComponent("VOLUMES", true);
		window.AddTextComponent("CULL:");
		window.AddSliderIntComponent("RENDER DIVIDE", -2000, 2000, -2000, false);
		window.AddCheckBoxComponent("FLIP DIVIDE", false);
		window.AddButtonComponent("RETURN TO MENU", 120, 30);
	}

	{
		AddWindow("GAME OBJECT");
		auto& window = windows["GAME OBJECT"];
		window.AddTextComponent("ObjectName");
		window.AddTextComponent("PolygonCount");
		window.AddTextComponent("Position");
		window.AddSliderFloatComponent("X", -700, 700, 0, false);
		window.AddSliderFloatComponent("Y", -50, 200, 0, false);
		window.AddSliderFloatComponent("Z", -700, 700, 0, false);

		window.AddTextComponent("Scale");
		window.AddSliderFloatComponent("X-axis", -1, 50, 0, false);
		window.AddSliderFloatComponent("Y-axis", -1, 50, 0, false);
		window.AddSliderFloatComponent("Z-axis", -1, 50, 0, false);
		window.AddCheckBoxComponent("Uniform scaling", false);
		window.AddButtonComponent("Delete", 120, 30);
		window.AddButtonComponent("Duplicate", 120, 30);
	}

	{
		AddWindow("SCENE COMPONENTS");
		auto& window = windows["SCENE COMPONENTS"];
		window.AddListBoxComponent("NameList", false);
	}

	{
		AddWindow("HELP");
		auto& window = windows["HELP"];
		window.AddTextComponent("Movement", false);
		window.SetValue<TextComponent, std::string>("Movement", "- Use WASD to move around.\n- Use SPACEBAR to go up.\n- Use Z to go down.\n- Use SHIFT to move faster.\n- Look around by holding RMB.\n- Select items by pressing LMB.\nSelected items show up in the Game Object window.\nYou can also select items by clicking on them in the Scene Component list.\n\n");
		window.AddTextComponent("Shortcuts", false);
		window.SetValue<TextComponent, std::string>("Shortcuts", "Selecting an item brings up the gizmo.\n- Keyboard key 1: MoveTool\n- Keyboard key 2: RotateTool\n- Keyboard key 3: ScaleTool\n\nPressing F on your keyboard focuses on the selected item.\n\nYou can also modify items by using the sliders in the Game Object window.\nBy holding CTRL and pressing LMB on a slider, you can type in an exact value.\n\n");
		window.AddTextComponent("Tools", false);
		window.SetValue<TextComponent, std::string>("Tools","Using the 'Delete' button will remove the selected item.\nThis can also be achieved by pressing 'Del' on your keyboard.\nWhen items are removed, the Scene Component list will update and sort itself.\n\nThe 'Duplicate' button creates a copy of the selected item.\nThe copy will automatically be selected.\nThis can also be achieved by pressing CTRL + D on your keyboard.\n\nWhen an item is selected, use 'CreateBBox' or 'CreateBSphere' to create an appropriate bounding-volume for the item.\nThe volume can be moved, scaled and rotated.\nThe volume is Not connected to any item, but an item must be selected to create a new volume.\nLike items, bounding-volumes can be duplicated.\n\n");
		window.AddTextComponent("Scene", false);
		window.SetValue<TextComponent, std::string>("Scene", "For now the editor only has one scene-file. This scene-file is the one that the game loads.\nTo Save changes to the file, press 'Save World'.\nUse the 'Load FBX' button to import new items.");
	}	

	//LOAD SCENE
	FBXLoader levelLoader("Models");

	GameLoader gameLoader;
	//GAME
	//gameLoader.Load("Default", scene.GetDrawables());
	//MAIN MENU
	gameLoader.Load("MainMenu", scene.GetDrawables());
	BindDrawables();

	scene.SetCamera(PI_DIV4, float(clientWidth) / float(clientHeight), 0.1f, 10000.0f, 1.0f, 25.0f, {0, 90, 0});
	scene.SetDirectionalLight(40);

	InitCamera(scene.GetCamera());

	//CLIENT INFORMATION (PICKING) TO BE REMOVED?
	wWidth = clientWidth;
	wHeight = clientHeight;

	appWindow = window;
	wRatioX = (float)clientWidth / GetSystemMetrics(SM_CXSCREEN);
	wRatioY = (float)clientHeight / GetSystemMetrics(SM_CYSCREEN);

	terrain = new Terrain(2);

	(void*)Run();
}

void LevelEditor::RemoveItem(std::string name)
{
	selectedObject = "";
	std::vector<std::shared_ptr <Drawable>> sameNameArray;

	ListBoxComponent* component = windows["SCENE COMPONENTS"].Get<ListBoxComponent>("NameList");
	component->Clear();

	auto deleted = scene.Get<Drawable>(name);

	scene.GetObjectNames().clear();

	auto model = std::dynamic_pointer_cast<Model>(deleted);
	if (model)
	{
		totalPolygonCount -= model->mesh.vertexCount / 3.0f;
		MR->Unbind(model);
		IDR->Unbind(model);

		scene.DeleteDrawable(name);
		name = Resources::Inst().GetBufferNameFromID(model->mesh.bufferID);

		auto& drawables = scene.GetDrawables();
		for (auto& [drawableName, drawable] : drawables)
		{
			if (drawableName.find(name) != std::string::npos)
				sameNameArray.emplace_back(drawable);
		}

		for (auto& drawable : sameNameArray)
		{
			scene.DeleteDrawable(drawable->GetName());
		}

		for (int i = 0; i < sameNameArray.size(); i++)
		{
			std::string newName;
			if (i != 0)
				newName = name + std::to_string(i);
			else
				newName = name;
			sameNameArray[i]->SetName(newName);
			drawables[newName] = sameNameArray[i];
		}

		auto& names = scene.GetObjectNames();
		int ID = 0;
		for (auto& [drawableName, drawable] : drawables)
		{
			drawable->SetID(ID + 1);
			ID++;
			names.emplace_back(drawableName);
			component->AddName(drawableName);
		}
	}

	else
	{
		auto box = std::dynamic_pointer_cast<BoxVolume>(deleted);
		if (box)
		{
			VR->Unbind(box);
			IDR->Unbind(box);
			scene.DeleteDrawable(name);
			name = "BoxVolume";
		}

		auto sphere = std::dynamic_pointer_cast<SphereVolume>(deleted);
		if (sphere)
		{
			VR->Unbind(sphere);
			IDR->Unbind(sphere);
			scene.DeleteDrawable(name);
			name = "SphereVolume";
		}

		auto& drawables = scene.GetDrawables();
		for (auto& [drawableName, drawable] : drawables)
		{
			if (drawableName.find(name) != std::string::npos)
				sameNameArray.emplace_back(drawable);
		}

		for (auto& drawable : sameNameArray)
		{
			scene.DeleteDrawable(drawable->GetName());
		}

		for (int i = 0; i < sameNameArray.size(); i++)
		{
			std::string newName;
			if (i != 0)
				newName = name + std::to_string(i);
			else
				newName = name;
			sameNameArray[i]->SetName(newName);
			drawables[newName] = sameNameArray[i];
		}

		for (int i = 0; i < sameNameArray.size(); i++)
			Print(sameNameArray[i]->GetName());

		for (auto& [drawableName, drawable] : drawables)
			Print(drawable->GetName());
	
		auto& names = scene.GetObjectNames();
		int ID = 0;
		for (auto& [drawableName, drawable] : drawables)
		{
			drawable->SetID(ID + 1);
			ID++;
			names.emplace_back(drawableName);
			component->AddName(drawableName);
		}
	}
}

void LevelEditor::ClearToolUI()
{
	auto& window = windows["GAME OBJECT"];

	window.SetValue<TextComponent, std::string>("ObjectName", "");
	window.SetValue<TextComponent, std::string>("PolygonCount", "");
	window.SetValue<SliderFloatComponent, float>("X", 0.0f);
	window.SetValue<SliderFloatComponent, float>("Y", 0.0f);
	window.SetValue<SliderFloatComponent, float>("Z", 0.0f);

	window.SetValue<SliderFloatComponent, float>("X-axis", 0.0f);
	window.SetValue<SliderFloatComponent, float>("Y-axis", 0.0f);
	window.SetValue<SliderFloatComponent, float>("Z-axis", 0.0f);

	selectedObject = "";
}

void LevelEditor::UpdateToolUI(std::string name)
{
	auto model = scene.Get<Drawable>(name);
	auto& window = windows["GAME OBJECT"];

	window.SetValue<TextComponent, std::string>("ObjectName", name);
	auto modelmodel = std::dynamic_pointer_cast<Model>(model);
	if (modelmodel)
	{
		window.SetValue<TextComponent, std::string>("PolygonCount", "PolygonCount: " + std::to_string(int(modelmodel->mesh.vertexCount / 3.0f)));
	}

	window.SetValue<SliderFloatComponent, float>("X", model->GetPosition().x);
	window.SetValue<SliderFloatComponent, float>("Y", model->GetPosition().y);
	window.SetValue<SliderFloatComponent, float>("Z", model->GetPosition().z);

	window.SetValue<SliderFloatComponent, float>("X-axis", model->GetScale().x);
	window.SetValue<SliderFloatComponent, float>("Y-axis", model->GetScale().y);
	window.SetValue<SliderFloatComponent, float>("Z-axis", model->GetScale().z);

	selectedObject = name;
}

void LevelEditor::FocusObject()
{
	if (selectedObject != "")
	{
		auto model = scene.Get<Drawable>(selectedObject);
		Vector3 objPos = model.get()->GetPosition();
		Vector3 camDir = scene.GetCamera()->GetDirection();

		scene.GetCamera()->SetPosition(objPos - camDir * 10);
	}
}

LevelEditor::~LevelEditor()
{
	RND.ShutDown();
	delete terrain;
	Resources::Inst().Clear();
}

APPSTATE LevelEditor::Run()
{
	Update();
	Render();

	{
		auto& window = windows["TOOLS"];
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
		if (totalPolygonCount > totalPolygonsLastFrame || totalPolygonCount < totalPolygonsLastFrame)
		{
			window.SetValue<TextComponent, std::string>("SCENE POLYGON COUNT", "SCENE POLYGON COUNT: " + std::to_string(totalPolygonCount));
			totalPolygonsLastFrame = totalPolygonCount;
		}
		if (window.GetValue<ButtonComponent>("LOAD FBX"))
			Load(FileSystem::LoadFile("Models"));

		if (window.GetValue<ButtonComponent>("CREATE BBOX"))
			CreateBoundingBox();

		if (window.GetValue<ButtonComponent>("CREATE BSPHERE"))
			CreateBoundingSphere();

		if (window.Changed("WIREFRAME"))
			Graphics::Inst().ToggleWireframe();

		if (window.Changed("TERRAIN"))
			ShowTerrain();

		if (window.Changed("WATER"))
			ShowWater();

		if (window.Changed("VOLUMES"))
			ShowVolumes();

		if (window.Changed("TERRAIN SUBDIV"))
		{
			if (terrain)
				delete terrain;
			terrain = new Terrain(window.GetValue<SliderIntComponent>("TERRAIN SUBDIV"));
		}

		if (window.Changed("RENDER DIVIDE"))
			DivideRendering();

		if (window.Changed("FLIP DIVIDE"))
			FlipRenderingDivider();
			DivideRendering();

		if (window.GetValue<ButtonComponent>("SAVE WORLD"))
		{
			GameLoader loader;
			loader.Save("MainMenu", scene.GetDrawables());
		}

		if (window.GetValue<ButtonComponent>("RETURN TO MENU"))
			return APPSTATE::MAIN_MENU;
	}

	{
		auto &window = windows["GAME OBJECT"];
		if (window.GetValue<ButtonComponent>("Duplicate"))
		{
			if (selectedObject != "")
			{
				auto selected = scene.Get<Drawable>(selectedObject);
				auto model = std::dynamic_pointer_cast<Model>(selected);
				if (model)
					DuplicateObject();
				else
					DuplicateVolume();
			}
		}
	}

	return APPSTATE::NO_CHANGE;
}