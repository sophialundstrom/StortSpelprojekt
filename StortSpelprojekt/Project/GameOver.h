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

class GameOver : public ApplicationState
{
private:

	std::shared_ptr<Canvas> currentCanvas;
	std::map<std::string, std::shared_ptr<Canvas>> canvases;
	TerrainRenderer terrainRenderer;
	ParticleRenderer particleRenderer;
	ModelRenderer modelRenderer;
	ShadowRenderer shadowRenderer;
	WaterRenderer waterRenderer;
	std::shared_ptr<Building> building;

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
	void HoveringNo();


	Terrain terrain;
	Water water;

	bool play = false;
	bool quit = false;
	bool backToMenu = false;
public:
	GameOver() = default;
	GameOver(UINT clientWidth, UINT clientHeight, HWND window);
	~GameOver();





	virtual APPSTATE Run() override;

};