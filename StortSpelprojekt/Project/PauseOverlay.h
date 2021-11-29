#pragma once
#include "Overlay.h"

class PauseOverlay : public Overlay
{
private:
	enum class INTERNALSTATE { MAIN, OPTIONS, HOW_TO_PLAY, QUIT_CHOICE };
	INTERNALSTATE internalState = INTERNALSTATE::MAIN;
	//BECAUSE PAUSEOVERLAY INHERITS FROM CANVAS 
	//IT CAN HAVE A BASE THAT ALWAYS RENDERS 
	//AND THEN CANVASES WITHIN THAT WE CAN SWAP BETWEEN

	float lastInput;

	Canvas* currentCanvas = nullptr;
	std::map<std::string, Canvas*> canvases;

	void HideLeaves();
public:
	PauseOverlay();
	~PauseOverlay();

	// Inherited via Overlay
	virtual void Render() override;
	virtual OVERLAYSTATE Update() override;
};