#pragma once
#include "Editor.h"
#include "Scene.h"
#include "ApplicationState.h"
#include "ImGui.h"
#include "WaterRenderer.h"

class LevelEditor : public Editor, public ApplicationState
{
private:
	ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;

	int totalPolygonsLastFrame;
	int totalPolygonCount;
	std::string selectedObject;
	HWND appWindow;
	float wRatioX;
	float wRatioY;
	POINT cursor;
	Vector3 screenSpaceCoordinates;
	Scene scene;
	std::map<std::string, std::shared_ptr<BoundingSphere>> pickBoxes;
	Terrain* terrain;
	Water water;

	//CheckBoxStuff
	bool renderWater = true;
	bool renderVolumes = true;
	bool renderTerrain = true;
	bool renderShadows = true;
	bool divideFlipped = false;

	//PerformanceStuff
	float frameTime = 0.0f;
	float renderTime = 0.0f;
	float updateTime = 0.0f;
	float updateUITime = 0.0f;


	UINT wWidth, wHeight;

	void BindDrawables();

	// Inherited via Editor
	virtual void Save(const std::string& file) override;
	virtual void Load(const std::string& file) override;
	virtual void Update() override;
	virtual void Render() override;
	void CreateBoundingBox();
	void CreateBoundingSphere();
	void RemoveItem(std::string name);
	void ClearToolUI();
	void UpdateToolUI(std::string name);
	void FocusObject();
	void DuplicateObject();
	void DuplicateVolume();
	void ShowWater();
	void DivideRendering();
	void FlipRenderingDivider();
	void ShowVolumes();
	void ShowTerrain();
	void ShowShadows();
public:
	LevelEditor(UINT clientWidth, UINT clientHeight, HWND window);

	~LevelEditor();
	
	// Inherited via GameState
	APPSTATE Run();
};




