#include "LevelEditor.h"

#include "Event.h"

void LevelEditor::Save(const std::string& file)
{

}

void LevelEditor::Load(const std::string& file)
{

}

void LevelEditor::Update()
{
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
	BeginFrame();

	//TO DO: ADD PARTICLE RENDER PASS (IN OPEN WORLD)
	//ADD RENDERER THAT RENDERS TO TEXTURE THAT CAN BE SHOWN AS "INGAME"-PREVIEW IN MATERIAL EDITOR 
	//(ONLY NEEDS ONE POINT LIGHT & DIRECTIONAL LIGHT, MAYBE A POSITION SLIDER FOR POINT TO PLAY WITH SPECULAR (OR ROTATING MESH))
	//PREVIEW EITHER ON A SPHERE OR THE SELECTED MESH

	animatedModelRenderer.Render();

	modelRenderer.Render();

	terrainRenderer.Render(terrain);

	EndFrame();
}

LevelEditor::LevelEditor(UINT clientWidth, UINT clientHeight)
	:modelRenderer(FORWARD, false),
	terrainRenderer(FORWARD, 63),
	animatedModelRenderer(FORWARD, false),
	terrain(10)
{
	//BOTH MUST BE SET (PERSPECTIVE MATRIX ISSUES OTHERWISE), OR WE JUS DO DEFAULT CONSTRUCTOR
	scene.SetCamera(PI_DIV4, float(clientWidth) / float(clientHeight), 0.1f, 500.0f, 1.0f, 5.0f);
	scene.SetDirectionalLight(40);

	//DO THIS WHEN "ADD MODEL"-BUTTON IS PRESSED IN SCENE WINDOW, 
	//OPEN DIRECTORY AND SELECT AN FBX (USING FILESYSTEM HEADER SAME AS PARTICLE SYSTEM)
	scene.AddModel("world");
	scene.AddModel("testSphere");

	modelRenderer.Bind(scene.Get<Model>("world"));
	modelRenderer.Bind(scene.Get<Model>("testSphere"));

	scene.Get<Model>("testSphere")->SetScale(0.1f);

	//ADD BUTTONS FOR LIGHT/MODEL/PARTICLE SYSTEM & SHOW SCENE HIERARCHY
	{
		AddWindow("SCENE");	
		auto& window = windows["SCENE"];				
		window.AddTextComponent("Scene Name");
		window.AddButtonComponent("RETURN TO MENU", 120, 30);
	}

	//MAKE INTERACTIVE WITH SELECTED OBJECT
	{
		AddWindow("GAME OBJECT");
		auto& window = windows["GAME OBJECT"];	
		window.AddTextComponent("Game Object Name");
	}

	//RENDER PREVIEW TO IMAGE TO SHOW IN WINDOW & MAKE INTERACTIVE WITH SELECTED OBJECTS MATERIAL (AND ADD "COPY MATERIAL" THING)
	{
		AddWindow("MATERIAL");		
		auto& window = windows["MATERIAL"];
		window.AddTextComponent("Material Name");
	}

	(void)Run();
}

LevelEditor::~LevelEditor()
{
	Resources::Inst().Clear();
}

State LevelEditor::Run()
{
	Update();
	Render();

	auto& window = windows["SCENE"];
	if (window.GetValue<ButtonComponent>("RETURN TO MENU"))
		return State::MENU;

	return State::NO_CHANGE;
}