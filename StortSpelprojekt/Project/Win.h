#pragma once
#include "ApplicationState.h"
#include "ParticleRenderer.h"
#include "ShadowRenderer.h"
#include "ModelRenderer.h"
#include "Canvas.h"
#include "FBXLoader.h"
#include "GameLoader.h"
#include "ParticleRenderer.h"
#include "Building.h"
#include "UI.h"
#include "TerrainRenderer.h"
#include "Terrain.h"
#include "Water.h"
#include "WaterRenderer.h"


class Win : public ApplicationState
{
private:
	Canvas* currentCanvas;
	std::map<std::string, Canvas*> canvases;

	ShadowRenderer shadowRenderer;
	WaterRenderer waterRenderer;
	TerrainRenderer terrainRenderer;
	ParticleRenderer particleRenderer;
	ModelRenderer modelRenderer;

	Terrain terrain;
	Water water;

	bool play = false;
	bool quit = false;
	bool backToMenu = false;


	void Render();
	void Initialize();

	void Continue();
	void HoveringContinue();

	void MainMenu();
	void BackToMainMenu();
	void HoveringMainMenu();

	void QuitGame();
	void HoveringQuit();

	void Form();
	void HoveringForm();

	void Exit();
	void Back();

	void HoveringYes();
	void HoveringNo();;


public:

	Win() = default;
	Win(UINT clientWidth, UINT clientHeight, HWND window);
	~Win();

	virtual APPSTATE Run() override;
};