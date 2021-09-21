#include "LevelEditor.h"

#include "Event.h"

void LevelEditor::Save(const std::string& file)
{

}

void LevelEditor::Load(const std::string& file)
{
	std::filesystem::path path = std::filesystem::path(file);
	if (file == "" || path.extension() != ".fbx")
		return;

	std::string fileName = path.stem().string();
	scene.AddModel(fileName);
	modelRenderer.Bind(scene.Get<Model>(fileName));
	windows["SCENE COMPONENTS"].AddTextComponent(scene.GetObjectNames()[scene.GetObjectNames().size()-1]);
}

void LevelEditor::Update()
{
	//TO DO: FIGURE OUT A NICE MOVEMENT IN EDITOR
	if (Event::RightIsClicked())
	{
		if (Event::ReadRawDelta().y > 0)
			scene.GetCamera().Rotate(0, 3);

		if (Event::ReadRawDelta().y < 0)
			scene.GetCamera().Rotate(0, -3);

		if (Event::ReadRawDelta().x > 0)
			scene.GetCamera().Rotate(3, 0);

		if (Event::ReadRawDelta().x < 0)
			scene.GetCamera().Rotate(-3, 0);
	}

	if (Event::KeyIsPressed('W'))
		scene.GetCamera().MoveForward();

	if (Event::KeyIsPressed('A'))
		scene.GetCamera().MoveRight(-1);

	if (Event::KeyIsPressed('S'))
		scene.GetCamera().MoveForward(-1);

	if (Event::KeyIsPressed('D'))
		scene.GetCamera().MoveRight();

	if (Event::KeyIsPressed(32)) //SPACE
		scene.GetCamera().MoveUp();

	if (Event::KeyIsPressed('Z')) //SHIFT
		scene.GetCamera().MoveUp(-1);
  
	if (Event::KeyIsPressed(16)) //SHIFT
		scene.GetCamera().SetSpeedMultiplier(4);
	else
		scene.GetCamera().SetSpeedMultiplier(1);

	Event::ClearRawDelta();
	scene.Update();
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

	EndFrame();
}

LevelEditor::LevelEditor(UINT clientWidth, UINT clientHeight)
	:modelRenderer(FORWARD, false),
	terrainRenderer(FORWARD, 1),
	animatedModelRenderer(FORWARD, false)
{
	//BOTH MUST BE SET (PERSPECTIVE MATRIX ISSUES OTHERWISE), OR WE JUS DO DEFAULT CONSTRUCTOR
	scene.SetCamera(PI_DIV4, float(clientWidth) / float(clientHeight), 0.1f, 500.0f, 1.0f, 5.0f);
	scene.SetDirectionalLight(40);

	terrain = new Terrain(20, 0);

	//DO THIS WHEN "ADD MODEL"-BUTTON IS PRESSED IN SCENE WINDOW, 
	//OPEN DIRECTORY AND SELECT AN FBX (USING FILESYSTEM HEADER SAME AS PARTICLE SYSTEM)
	

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
		window.AddTextComponent("Name");
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
		

	if (window.GetValue<ButtonComponent>("RETURN TO MENU"))
		return State::MENU;

	return State::NO_CHANGE;
}