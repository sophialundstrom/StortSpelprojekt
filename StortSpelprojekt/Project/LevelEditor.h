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
	bool renderWater = true;
	bool divideFlipped = false;
	bool renderVolumes = true;
	bool renderTerrain = true;

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

private:
	std::ofstream out;
	std::string n;
	std::string e;
	std::string lastSelectedObject;
	void AddNode();
	void AddEdge();
	void test();
	std::ifstream in;
	void LoadNodes();
	Pathfinding* path;
public:
	LevelEditor(UINT clientWidth, UINT clientHeight, HWND window);

	~LevelEditor();
	
	// Inherited via GameState
	APPSTATE Run();
};




