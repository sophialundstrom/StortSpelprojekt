#pragma once
#include "UI.h"
#include "ParticleRenderer.h"
#include "ModelRenderer.h"
#include "Canvas.h"
#include "FBXLoader.h"
#include "GameLoader.h"
#include "ParticleRenderer.h"
#include "ShadowRenderer.h"
#include "ApplicationState.h"

class GameOver : public ApplicationState
{
private:
	Canvas* currentCanvas;
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