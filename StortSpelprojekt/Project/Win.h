#pragma once
#include "ApplicationState.h"
#include "Canvas.h"
#include "FBXLoader.h"
#include "GameLoader.h"
#include "Building.h"
#include "UI.h"
#include "Terrain.h"
#include "Water.h"

class Win : public ApplicationState
{
private:
	std::shared_ptr<Canvas> currentCanvas;
	std::map<std::string, std::shared_ptr<Canvas>> canvases;

	Terrain terrain;
	Water water;

	bool backToMenu = false;

	void Render();
	void Initialize();

	void MainMenu();
	void BackToMainMenu();
	void HoveringMainMenu();

	void Form();
	void HoveringForm();

	void QuitCanvas();

	void HoveringYes();
	void HoveringNo();;
public:
	Win() = default;
	Win(UINT clientWidth, UINT clientHeight, HWND window);
	~Win();

	virtual APPSTATE Run() override;
};