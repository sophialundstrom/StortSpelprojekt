#pragma once
#include "Editor.h"
#include "Scene.h"
#include "AnimatedModelRenderer.h"
#include "ModelRenderer.h"
#include "TerrainRenderer.h"
#include "ApplicationState.h"
#include "Building.h"
#include "PRay.h"
#include "Collision.h"
#include "ColliderRenderer.h"
#include "IDRenderer.h"
#include "VolumeRenderer.h"
#include "WaterRenderer.h"

class LevelEditor : public Editor, public ApplicationState
{
private:
	ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;

	int totalVertexCountLastFrame;
	int totalVertexCount;
	std::string selectedObject;
	HWND appWindow;
	float wRatioX;
	float wRatioY;
	POINT cursor;
	Vector3 screenSpaceCoordinates;
	Scene scene;
	std::map<std::string, std::shared_ptr<BoundingSphere>> pickBoxes;
	Terrain* terrain;
	std::shared_ptr<Building> building;
	Water water;

	//ADD RENDERERS
	TerrainRenderer terrainRenderer;
	ModelRenderer modelRenderer;
	IDRenderer idRenderer;
	VolumeRenderer volumeRenderer;
	WaterRenderer waterRenderer;

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
public:
	LevelEditor(UINT clientWidth, UINT clientHeight, HWND window);

	~LevelEditor();
	
	// Inherited via GameState
	APPSTATE Run();
};




