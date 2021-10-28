#include "Main Menu.h"

MainMenu::MainMenu(UINT clientWidth, UINT clientHeight, HWND window)
	:modelRenderer(DEFERRED, true),
	particleRenderer(DEFERRED)
{
	currentCanvas = new Canvas();
	currentCanvas->AddImage({ clientWidth / 2.0f, (float)clientHeight }, "juan.png", "CompassBase.png");
	
}

MainMenu::~MainMenu()
{
	delete currentCanvas;
}

void MainMenu::Initialize()
{
	//LOAD SCENE
	FBXLoader levelLoader("Models");

	GameLoader gameLoader;
	gameLoader.Load("Default", scene.GetDrawables());

	for (auto& [name, drawable] : scene.GetDrawables())
	{
		auto model = std::dynamic_pointer_cast<Model>(drawable);
		if (model)
		{
			modelRenderer.Bind(model);
			shadowRenderer.Bind(model);
		}

		auto particleSystem = std::dynamic_pointer_cast<ParticleSystem>(drawable);
		if (particleSystem)
		{
			//SAME BUT PS->
		}
	}

}

APPSTATE MainMenu::Run()
{
	Graphics::Inst().BeginFrame();

	currentCanvas->Render();

	Graphics::Inst().EndFrame();

	if (Event::KeyIsPressed('G'))
		return APPSTATE::GAME;

	if (Event::KeyIsPressed(VK_ESCAPE))
		return APPSTATE::EXIT;

	return APPSTATE::NO_CHANGE;
}
