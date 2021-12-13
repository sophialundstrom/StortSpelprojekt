#pragma once
#include "Editor.h"
#include "Scene.h"
#include "ApplicationState.h"
#include "ImGui.h"
#include "WaterRenderer.h"
#include "Pathfinding.h"

class LevelEditor : public Editor, public ApplicationState
{
private:
	ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
	std::string sceneName = "Default";

	int totalPolygonsLastFrame;
	int totalPolygonCount;
	int targetFPS = 60;
	int mrTimeFactor = 20;
	int nrOfModels = 0;
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
	bool renderSkybox = true;
	bool renderPerformance = false;
	bool divideFlipped = false;

	//PerformanceStuff
	float frameTime = 0.0f;
	float renderTime = 0.0f;
	float updateTime = 0.0f;
	float updateUITime = 0.0f;
	//Rendereres
	float shadowTime = 0.0f;
	float modelTime = 0.0f;
	float waterTime = 0.0f;
	float terrainTime = 0.0f;
	float volumeTime = 0.0f;
	float renderUITime = 0.0f;
	float skyboxTime = 0.0f;


	UINT wWidth, wHeight;

	void BindDrawables();

	// Inherited via Editor
	virtual void Save(const std::string& file) override;
	virtual void Load(const std::string& file) override;
	virtual void Update() override;
	virtual void Render() override;
	void LoadScene(const std::string& file);
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

private:
	std::ofstream out;
	std::vector<std::string> nodes;
	std::vector<std::string> edges;

	std::string n;
	std::string e;
	std::string lastSelectedObject;
	void AddNode();
	void AddEdge();
	void test();
	std::ifstream in;
	void LoadNodes();
	Pathfinding* path;
	void ShowShadows();
	void ShowSkybox();
	void ShowPerformance();
	void UpdatePerformanceLimit();
public:
	LevelEditor(UINT clientWidth, UINT clientHeight, HWND window);

	~LevelEditor();
	
	// Inherited via GameState
	APPSTATE Run();
};




