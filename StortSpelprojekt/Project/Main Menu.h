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
#include "DeferredRenderer.h"

class MainMenu : public ApplicationState
{
private:
	Canvas* currentCanvas;
	std::map<std::string, Canvas*> canvases;

	Terrain terrain;

	TerrainRenderer terrainRenderer;
	ParticleRenderer particleRenderer;
	ModelRenderer modelRenderer;
	ShadowRenderer shadowRenderer;
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
	void HoveringPlay();
public:
	MainMenu() = default;
	MainMenu(UINT clientWidth, UINT clientHeight, HWND window);
	~MainMenu();

	virtual APPSTATE Run() override;
};