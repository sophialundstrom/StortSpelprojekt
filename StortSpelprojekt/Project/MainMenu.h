#pragma once
#include "ApplicationState.h"
#include "Canvas.h"
#include "FBXLoader.h"
#include "GameLoader.h"
#include "Building.h"
#include "UI.h"
#include "Terrain.h"
#include "Renderers.h"

class MainMenu : public ApplicationState
{
private:
	Canvas* currentCanvas;
	std::map<std::string, Canvas*> canvases;
	std::shared_ptr<Building> building;

	void Initialize();
	void Render();

	bool quit;
	bool play;

	void Options();
	void HowToPlay();
	void BacktoMenu();
	void Play();
	void Quit();
	void Form();
	//LEAVES
	void HoveringNewGame();
	void HoveringContinue();
	void HoveringQuit();
	void HoveringHowToPlay();
	void HoveringForm();
	void HoveringOptions();
	void HoveringBackControls();
	void HoveringBackOptions();
public:
	MainMenu() = default;
	MainMenu(UINT clientWidth, UINT clientHeight, HWND window);
	~MainMenu();

	virtual APPSTATE Run() override;
};