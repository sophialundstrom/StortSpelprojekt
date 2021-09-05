#include "LevelEditor.h"

#include "Event.h"

void LevelEditor::Save(const std::string& file)
{
}

void LevelEditor::Load(const std::string& file)
{
}

void LevelEditor::Initialize(UINT windowWidth, UINT windowHeight)
{
	//BOTH MUST BE SET (PERSPECTIVE MATRIX ISSUES OTHERWISE), OR WE JUS DO DEFAULT CONSTRUCTOR
	scene.SetCamera(PI_DIV4, float(windowWidth) / float(windowHeight), 0.1f, 100.0f, 1.0f, 2.0f);
	scene.SetDirectionalLight(40);

	//DO THIS WHEN "ADD MODEL"-BUTTON IS PRESSED IN SCENE WINDOW, 
	//WHEN ADDING MODEL - EITHER OPEN DIRECTORY AND SELECT AN FBX OR MAKE A DYNAMIC ARRAY THAT STORES MESHES IN FOLDER TO CHOOSE FROM
	scene.AddModel("world");
	modelRenderer.Bind(scene.Get<Drawable>("world"));

	{
		AddWindow("SCENE");
		auto& window = windows["SCENE"]; //ADD BUTTONS FOR LIGHT/MODEL/PARTICLE SYSTEM & SHOW SCENE HIERARCHY
		window.AddTextComponent("Scene Name");
		window.AddButtonComponent("RETURN TO MENU", 120, 30);
	}

	{
		AddWindow("GAME OBJECT");
		auto& window = windows["GAME OBJECT"];	//MAKE INTERACTIVE WITH SELECTED OBJECT
		window.AddTextComponent("Game Object Name");
	}
	
	{
		AddWindow("MATERIAL");		//RENDER PREVIEW TO IMAGE TO SHOW IN WINDOW & MAKE INTERACTIVE WITH SELECTED OBJECTS MATERIAL (AND ADD "COPY MATERIAL" THING)
		auto& window = windows["MATERIAL"];
		window.AddTextComponent("Material Name");
	}
}

void LevelEditor::Update()
{
	if (firstFrame)
		return;

	auto& window = windows["SCENE"];
	if (window.GetValue<ButtonComponent>("RETURN TO MENU"))
	{
		done = true;
		return;
	}

	//TO DO: FIGURE OUT A NICE MOVEMENT IN EDITOR
	if (Event::KeyIsPressed('Q'))
		scene.GetCamera().Rotate(0, 1);

	if (Event::KeyIsPressed('E'))
		scene.GetCamera().Rotate(0, -1);

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

	if (Event::KeyIsPressed(16)) //SHIFT
		scene.GetCamera().MoveUp(-1);

	scene.Update();
}

void LevelEditor::Render()
{
	if (firstFrame)
		firstFrame = false;

	BeginFrame();

	//TO DO: ADD PARTICLE RENDER PASS (IN OPEN WORLD)
	//ADD RENDERER THAT RENDERS TO TEXTURE THAT CAN BE SHOWN AS "INGAME"-PREVIEW IN MATERIAL EDITOR 
	//(ONLY NEEDS ONE POINT LIGHT & DIRECTIONAL LIGHT, MAYBE A POSITION SLIDER FOR POINT TO PLAY WITH SPECULAR (OR ROTATING MESH))
	//PREVIEW EITHER ON A SPHERE OR THE SELECTED MESH

	modelRenderer.Render();

	EndFrame();
}

void LevelEditor::Reset()
{
	ClearWindows();
	scene.Clear();
	modelRenderer.Clear();
	firstFrame = true;
	done = false;
	TempResources::Inst().Clear();
}