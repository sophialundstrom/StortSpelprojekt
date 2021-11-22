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

class GameOver : public ApplicationState
{
private:
	Canvas* currentCanvas;
	std::map<std::string, Canvas*> canvases;
	TerrainRenderer terrainRenderer;
	ParticleRenderer particleRenderer;
	ModelRenderer modelRenderer;
	ShadowRenderer shadowRenderer;
	std::shared_ptr<Building> building;

	bool goToMenu = false;
public:
	GameOver() = default;
	GameOver(UINT clientWidth, UINT clientHeight, HWND window);
	~GameOver();

	bool quit = false;
	bool backToMenu = false;

	void Render();
	void MainMenu();
	void QuitGame();

	virtual APPSTATE Run() override;

};